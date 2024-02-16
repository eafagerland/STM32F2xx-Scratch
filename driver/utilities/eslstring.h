#ifndef __ESLSTRING_H
#define __ESLSTRING_H

#include "stm32f207xx.h"

UInt32 stringlen(const char* string);
void stringcopy(char* dst, const char* src);
void stringcat(char* dst, const char* src);
void stringset(char* string, char c, UInt32 len);
void uint_to_string(UInt32 num, char* str);
void int_to_string(Int32 num, char* str);
void print(const char* string);
void println(char* string);

#endif // __ESLSTRING_H