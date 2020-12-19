#pragma once
#include <stdio.h>

#pragma pack(push, 1)

struct bmp_header {
	short int bf_type;
	int bf_size;
	short int bf_reserved1, bf_reserved2;
	int bf_off_bits;
};

struct bmp_info {
	int bi_size, bi_width, bi_height;
	short int bi_planes, bi_bit_count;
	int bi_compression, bi_size_image;
	int bi_x_pels_per_meter, bi_y_pels_per_meter;
	int bi_clr_used, bi_clr_important;
};

struct pixel {
	char b, g, r;
};

#pragma pack(pop)

void load_bmp(int W, int H, FILE *input_file, struct pixel * * dest);

void crop(int x, int y, int w, int h, struct pixel * * src, struct pixel * * dest);

void rotate(int W, int H, struct pixel * * src, struct pixel * * dest);

void save_bmp(int W, int H, FILE *output_file, struct bmp_header *header, struct bmp_info *info, struct pixel * * src);
