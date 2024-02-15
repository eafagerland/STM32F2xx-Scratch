#ifndef __ESLSTRING_H
#define __ESLSTRING_H

#include "stm32f207xx.h"

UInt32 stringlen(const char* string);
void stringcopy(char* dst, const char* src);
void stringcat(char* dst, const char* src);
void stringset(char* string, char c, UInt32 len);
void print(const char* string);

#endif // __ESLSTRING_H