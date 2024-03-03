#ifndef RECORD_H
#define RECORD_H
#include "../config.h"

int initialize(const Config& config); // 0 good, -1 error
void save_frame(const Config& config);
void finish();

#endif