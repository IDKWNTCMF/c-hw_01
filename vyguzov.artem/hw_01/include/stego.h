#pragma once

#include "bmp.h"

void insert(char c, int W, int H, struct pixel image[H][W], FILE *key);

char extract(int W, int H, struct pixel image[H][W], FILE *key);
