#ifndef UTIL_H
#define UTIL_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

void die(const char *fmt, ...);
long file_size(FILE *file);

#endif
