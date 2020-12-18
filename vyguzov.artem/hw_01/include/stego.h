#pragma once

#include "bmp.h"

void insert(char c, int H, struct pixel * * image, FILE *key);

char extract(int H, struct pixel * * image, FILE *key);
