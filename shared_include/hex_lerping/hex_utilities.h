#ifndef _HEX_UTILITIES_H_
#define _HEX_UTILITIES_H_
#include <stddef.h>
#include <stdbool.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
extern bool parse_valid_hex(const char col_str[static 7]);
extern void hex_lerp(int c1num, int c2num, int percentage);
#endif
