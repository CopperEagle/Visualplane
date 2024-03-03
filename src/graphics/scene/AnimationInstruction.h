#ifndef ANIMATIONINSTRUCTION_H
#define ANIMATIONINSTRUCTION_H

#include <cstdint>

#define MOVE 0u
#define ROTATE 1u
#define SCALE 2u
#define SETCOLOR 3u
#define SETOPACITY 4u
#define FADEOUT 5u
#define FRESHEVENT 128u

class Shape;

typedef uint32_t n_frames;

struct MoveData{
    int xy[4];
};

struct RotateData{
    float phi[2];
};

struct ScaleData{
    float alpha[2];
};

struct OpacityData{
    float alpha[2];
};

struct CustomData{
    void* data;
    uint8_t type;
};

struct ColorData{
    void* ok[2];
};

struct AnimationInstruction
{
	n_frames start, end;
	Shape* shape;
	union data{
        // int xy[4];
        // float alpha[2];

        MoveData move;
        OpacityData opac;
        ColorData col;
        RotateData rot;
        ScaleData scal;
    } data;
    void* interpolFunc;
    uint8_t animation_type;
};




#endif // ANIMATIONINSTRUCTION_H