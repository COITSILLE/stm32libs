#ifndef SSTRTOTYPE_H
#define SSTRTOTYPE_H

#include "stdint.h"
#include <stdint.h>

char* s_strtok(char *str, const char delim);
uint8_t s_atoi(const char num);
float s_atof(const char *str);

#endif