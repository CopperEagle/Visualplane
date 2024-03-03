#include <cstddef>
#include <cstring>

#include "Path.h"
#include "Shape.h"
#include "../scene/Scene.h"
#include "../../dependencies/glad.h"


#include <glm/exponential.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/vector_relational.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "../../util/logging.h"

typedef glm::vec2 Point;
typedef glm::vec2 LineSegment;
typedef glm::vec2 Vertex2D;

/**
* Known problems: 
* - The path may interfere with itself if the points are too close
* (less than a pixel) which may create visible artifacts if the path has lower
* opacity and big enough line width. If you cannot change the distance of your pixels
* (e.g. by subsampling) then consider setting line_width = original_width/k
* and scale your Path Shape by a factor of k. ((Is there a cast from float to int somewhere?))
* 
* - The Path currently does not accept three successive collinear points. If you
* do submit three successive collinear points, it will remove the second point.
* ((To solve this, take a look at what to do if side is 0.))
**/
Path::Path(Scene& s, unsigned line_width, int z_layer) : ModifiableCenterShape(s, 0, 0, z_layer), shader(
    s.get_config().shaders_dir + "line.vert", 
	s.get_config().shaders_dir + "line.frag"),
    vao(1), s(s), width(line_width)
{
    shader.registerUniform("camMat");
    shader.registerUniform("col");
}

/*!
* @brief Add a point to the path.
**/
void Path::add_point(float x, float y)
{
    int len = p.size();
    if (len >= 2){
        
        Point u = p.at(len - 2);
        Point v = p.at(len - 1);

        LineSegment line1 = v - u;
        LineSegment line2 = glm::vec2(x, y) - v;
        
        if (line1.x*line2.y == line1.y*line2.x)
        {
            p.pop_back();
        }
    }
    p.push_back(glm::vec2(x, y));//, 0.0f, 1.0f));
}

static float sign(float x)
{
    if (x > 0.0f) return 1.0f;
    return -1.0f; 
}

/*!
* @brief Takes a path described by a list of points and computes the vertices for the inner
* points. (That is those points that are neither at the beginning nor at the end
* of the list.) Each point iterated on generates two vertices.
* It does not feature any linejoin arguments yet.
*
* @param[in] points The list of points.
* @param[in] start_point_index The index of the point from which the algorithm should compute the vertices. Inclusive.
* @param[in] end_point_index The index of the last point which should have the vertices computed. Inclusive.
* @param[in] vtx The array in which the vertices should be stored.
* @param[out] index Where in the vtx array to start writing in the results. Will be manipulated
*                   such that after this function, index will be the next free spot in the array.
*                   There is no guarantee that the index is valid.
* @param[in] line_width The required line width.
**/
static float inner_line_segments(const std::vector<Point>& points, unsigned start_point_index, unsigned end_point_index, Vertex2D* vtx, int& vtx_index, float line_width)
{   
    const float wei = line_width/2.0f;
    float change = -1; // this is to regulate which order the vertices need to be inserted.
    for (unsigned i = start_point_index; i <= end_point_index; i++)
    {
        
        // Extracting the three vertices in question, where we add the vertices
        // that correspond to point v.
        const Point u = points.at(i-1);
        const Point v = points.at(i);
        const Point w = points.at(i+1);

        const LineSegment line1 = glm::normalize(v - u);
        const LineSegment line2 = glm::normalize(v - w); // both pointing to the same vertex
        
        const LineSegment line1_n = -wei*glm::vec2(-line1.y, line1.x); // length is wei
        const LineSegment line2_n = wei*glm::vec2(-line2.y, line2.x); // length is wei

        glm::vec2 miter = glm::normalize(line1_n + line2_n); //*(wei*4.0f);

        const float cos_phi = glm::dot(miter, (1.0f/wei)*line1_n);

        miter = (wei/cos_phi) * miter; // len_miter * cos(phi) = len(line1_n)

        // This part decides in which order the two vertices are entered
        // In short, if two consecutive lines curve to the same side,
        // we need to enter them in opposite order.
        const float side = line1.x*line2.y - line2.x*line1.y;
        float mul = sign(change) == sign(side) ? -1.0f : 1.0f;
        change = sign(change) == sign(side) ? -side : side;// ? -1 : 1;
        if (side > 0)
        {
            vtx[vtx_index++] = v + mul*miter;
            vtx[vtx_index++] = v - mul*miter;
        }
        else
        {
            vtx[vtx_index++] = v - mul*miter;
            vtx[vtx_index++] = v + mul*miter; 
        }
    }
    return change;
}

static bool equal(glm::vec2 a, glm::vec2 b)
{
    float o = a.x - b.x;
    float o2 = a.y - b.y;
    if (glm::abs(o) + glm::abs(o2) < 0.00000001) return true;
    return false;
}

/*!
* @brief Closes and ends the path. Either end() or close() need to be called before rendering.
* It does what end() does but it ensures that the path ends where it started. 
* After this is called, further points that are added will be ignored.
**/
void Path::close()
{
    // The method will compute the vertices that are needed given the points provided by
    // the user. Note that these are not the same thing as vertices need to take line width
    // and other variables into account.

    if (p.size() < 2)
        logging::error("Path::close", "Cannot draw Path with less than 2 vertices.");
    else if (p.size() == 2)
        return end();
    
    // Step -1: Ensure that the path ends where it starts and make it "wrap around":
    // Add a linesegment to the end that equals the first line segment and vice versa.
    // That way, the loop we are interested in is fully described by linesegments, that
    // are not at the end or beginning.
    Point start = p.at(p.size() - 1);
    if (!equal(p.at(0), p.at(p.size() - 1)))
    {
        add_point(p.at(0).x, p.at(0).y);
    } else {
        start = p.at(p.size() - 2);
    }
    add_point(p.at(1).x, p.at(1).y);
    p.insert(p.begin(), start);
    
    //Step 0: Initialization.
    // There will be two vertices per point that the user has entered.
    // We added two invisible points (wrap around) so we subtract 4
    Vertex2D* vtx = new Vertex2D[p.size() * 2 - 4];
    // Totally, we are drawing two triangles per line segment
    // and there are num_points - 1 line segments.
    // We added two invisible line segments (wrap around), hence subtract 12
    GLuint* ebo = new GLuint[6*(p.size() - 1) - 12];
    _nVertices = 6*(p.size() - 1) - 12;
    int index = 0;
    const float wei = width/2.0f;

   

    // Step 2: All lines that have a predecessor and a successor,
    // as we did the wrap around above.
    float change = inner_line_segments(p, 1, 
            p.size() - 2, vtx, index, width);
    
    // Step 3: Preparing the EBO
    int co = 0;
    for (unsigned i = 0; i < p.size() - 1 - 2; i++){
        ebo[6*i  ] = 0 + co;
        ebo[6*i+1] = 1 + co;
        ebo[6*i+2] = 2 + co;
        ebo[6*i+3] = 0 + co;
        ebo[6*i+4] = 3 + co;
        ebo[6*i+5] = 2 + co;
        
        co += 2;
    }

    // Step 4: Attaching the data to the VAO.
    vao.attachNewVBO((GLfloat*)vtx, (p.size()*2 - 4)*2, buildCommonLayout(0, 2), GL_STATIC_DRAW);
    vao.attachEBO(ebo, 6*(p.size() - 1) - 12, GL_STATIC_DRAW);

    delete[] ebo;
    delete[] vtx;
}

/*!
* @brief Terminates the path. Either end() or close() need to be called before rendering.
* It does what close() does but close() will make the path end where it started. 
* After this is called, further points that are added will be ignored.
**/
void Path::end()
{
    // The method will compute the vertices that are needed given the points provided by
    // the user. Note that these are not the same thing as vertices need to take line width
    // and other variables into account.

    if (p.size() < 2)
        logging::error("Path::end", "Cannot draw Path with less than 2 vertices.");
    
    // Step 0: Initialization.
    // There will be two vertices per point that the user has entered.
    Vertex2D* vtx = new Vertex2D[p.size() * 2];
    // Totally, we are drawing two triangles per line segment
    // and there are num_points - 1 line segments.
    GLuint* ebo = new GLuint[6*(p.size() - 1)];
    _nVertices = 6*(p.size() - 1);
    int index = 0;
    const float wei = width/2.0f;

    // Step 1: the first line. It is different as the end
    // does not border to any other line and hence the 
    // line end is perpendicular to the line itself.
    Point u = p.at(0);
    Point v = p.at(1);

    LineSegment line0 = v - u;
    LineSegment line0_n = wei * glm::normalize(glm::vec2(-line0.y, line0.x));
    vtx[index++] = u + line0_n;
    vtx[index++] = u - line0_n;

    // Step 2: All lines that have a predecessor and a successor
    float change = inner_line_segments(p, 1, 
            p.size() - 2, vtx, index, width);
    
    // Step 3: The final line. It needs to be treated seperately
    // just like the first line.
    if (p.size() == 2)
    {
        vtx[index++] = v - line0_n;
        vtx[index++] = v + line0_n;
    } 
    else 
    {
        Point t = p.at(p.size() - 3);
        Point u = p.at(p.size() - 2);
        Point v = p.at(p.size() - 1);
        
        LineSegment line0 = v - u;
        LineSegment line0_n = wei * glm::normalize(glm::vec2(-line0.y, line0.x));

        LineSegment line1 = u - t;
        LineSegment line2 = u - v;
        float side = line1.x*line2.y - line2.x*line1.y;
        float mul = sign(change) == sign(side) ? -1.0f : 1.0f;
        if (side < 0)
        {
            vtx[index++] = v + mul*line0_n;
            vtx[index++] = v - mul*line0_n;   
        }
        else 
        {
            vtx[index++] = v - mul*line0_n;
            vtx[index++] = v + mul*line0_n; 
        }
    }
    
    // Step 4: Preparing the EBO
    int co = 0;
    for (unsigned i = 0; i < p.size() - 1; i++){
        ebo[6*i  ] = 0 + co;
        ebo[6*i+1] = 1 + co;
        ebo[6*i+2] = 2 + co;
        ebo[6*i+3] = 0 + co;
        ebo[6*i+4] = 3 + co;
        ebo[6*i+5] = 2 + co;
        
        co += 2;
    }

    // Step 5: Attaching the data to the VAO.
    vao.attachNewVBO((GLfloat*)vtx, (p.size()*2)*2, buildCommonLayout(0, 2), GL_STATIC_DRAW);
    vao.attachEBO(ebo, 6*(p.size() - 1), GL_STATIC_DRAW);

    delete[] ebo;
    delete[] vtx;
}


void Path::draw(n_frames frame, const glm::mat4& cam_mat)
{

    const glm::mat4 mat = cam_mat * get_matrix();

    shader.setUniformValue4fv("camMat", glm::value_ptr(mat));
    shader.setUniformValue4F("col", pt2);
    shader.activate();
    vao.draw(_nVertices);//6*(p.size() - 1)
}


void Path::set_color(const ColorObject &col)
{
    memcpy(pt2, &col, 4*sizeof(float));
}