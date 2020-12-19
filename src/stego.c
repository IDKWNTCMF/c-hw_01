#include "stego.h"
#include "bmp.h"

const int mask = 254;

void insert(char c, struct pixel * * image, FILE *key) {
	int code = 0;
	switch (c) {
		case ' ':
			code = 26;
			break;
		case '.':
			code = 27;
			break;
		case ',':
			code = 28;
			break;
		default:
			code = c - 'A';
			break;
	}
	
	for (int i = 0; i < 5; i++) {
		int bit = 0;
		if ((code & (1 << i)) != 0) bit = 1;
		
		int x, y;
		char colour;
		if (fscanf(key, "%d %d %c", &x, &y, &colour) != 3) return;
		
		switch (colour) {
			case 'R':
				if (bit == 0) image[y][x].r &= mask; else image[y][x].r |= 1;
				break;
			case 'G':
				if (bit == 0) image[y][x].g &= mask; else image[y][x].g |= 1;
				break;
			case 'B':
				if (bit == 0) image[y][x].b &= mask; else image[y][x].b |= 1;
				break;
			default:
				return;
		}
	}
}

char extract(struct pixel * * image, FILE *key) {
	int code = 0;
	for (int i = 0; i < 5; i++) {
		int x, y;
		char colour;
		if (fscanf(key, "%d %d %c", &x, &y, &colour) != 3) {
			return '\0';
		}
		int bit = 0;
		
		switch (colour) {
			case 'R':
				bit = (image[y][x].r & 1);
				break;
			case 'G':
				bit = (image[y][x].g & 1);
				break;
			case 'B':
				bit = (image[y][x].b & 1);
				break;
			default:
				return '\0';
		}
		
		code += (1 << i) * bit;
	}
	switch (code) {
		case 26:
			return ' ';
		case 27:
			return '.';
		case 28:
			return ',';
		default:
			return code + 'A';
	}
}
