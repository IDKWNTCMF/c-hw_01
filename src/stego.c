#include "stego.h"
#include "bmp.h"

const int mask = 254;

void insert(char c, int W, int H, struct pixel image[H][W], FILE *key) {
	int code = 0;
	if ('A' <= c && c <= 'Z') code = c - 'A';
	if (c == ' ') code = 26;
	if (c == '.') code = 27;
	if (c == ',') code = 28;
	
	for (int i = 0; i < 5; i++) {
		int bit = 0;
		if ((code & (1 << i)) != 0) bit = 1;
		
		int x, y;
		char colour;
		if (fscanf(key, "%d %d %c", &x, &y, &colour) != 3) return;
		int correct_y = H - 1 - y;
		
		if (colour == 'R') {
			if (bit == 0) image[correct_y][x].r &= mask; else image[correct_y][x].r |= 1;
		}
		if (colour == 'G') {
			if (bit == 0) image[correct_y][x].g &= mask; else image[correct_y][x].g |= 1;
		}
		if (colour == 'B') {
			if (bit == 0) image[correct_y][x].b &= mask; else image[correct_y][x].b |= 1;
		}
	}
}

char extract(int W, int H, struct pixel image[H][W], FILE *key) {
	int code = 0;
	for (int i = 0; i < 5; i++) {
		int x, y;
		char colour;
		if (fscanf(key, "%d %d %c", &x, &y, &colour) != 3) {
			return '\0';
		}
		int correct_y = H - 1 - y;
		int bit = 0;
		
		if (colour == 'R') {
			bit = (image[correct_y][x].r & 1);
		}
		if (colour == 'G') {
			bit = (image[correct_y][x].g & 1);
		}
		if (colour == 'B') {
			bit = (image[correct_y][x].b & 1);
		}
		
		code += (1 << i) * bit;
	}
	if (code < 26) return code + 'A';
	if (code == 26) return ' ';
	if (code == 27) return '.';
	if (code == 28) return ',';
	return '\0';
}
