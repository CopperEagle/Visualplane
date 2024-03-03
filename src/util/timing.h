#ifndef TIMING_H
#define TIMING_H

#include <cstdint>

uint64_t seconds(float sec);
/*!
* DO NOT use. This is for intern use only,
*/
void __set_fps(int fps);
#endif // TIMING_H