#ifndef MINHEAP_H
#define MINHEAP_H

#include <functional>
#include <vector>
#include "../graphics/2D/Shape.h"

/*!
* MinHeap is a bit of a misnomer. It is a MinHeap with 
* special additional functionality that is specifically added
* for the Scene object.
* \todo Use a better name than MinHeap. It is for Shapes only. Could do Template here but the special functionality will not be needed elsewhere.
*/

class MinHeap 
{
public:
    MinHeap();
    ~MinHeap();
    void add(Shape* s);

    void freeze();
    Shape* pop();
    void restore();

    bool isempty(){return _size == 0;};    

private:
    void bubble_up(unsigned int idx);
    void bubble_down(unsigned int idx);

    void swap(int idx1, int idx2);

    std::vector<Shape*> _prefreeze_buffer;

    Shape **_data = nullptr, **_backup_data = nullptr;
    int _size = 0, _capacity = 0;

};

#endif // MINHEAP_H