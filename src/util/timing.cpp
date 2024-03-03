#include "timing.h"
#include <cstdint>

int fps = 60;

void __set_fps(int _fps){
    fps = _fps;
}


uint64_t seconds(float sec)
{
    return sec * fps;
}