#pragma once
#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)

struct bmp_header {
	int16_t bf_type;
	int32_t bf_size;
	int16_t bf_reserved1, bf_reserved2;
	int32_t bf_off_bits;
};

struct bmp_info {
	int32_t bi_size, bi_width, bi_height;
	int16_t bi_planes, bi_bit_count;
	int32_t bi_compression, bi_size_image;
	int32_t bi_x_pels_per_meter, bi_y_pels_per_meter;
	int32_t bi_clr_used, bi_clr_important;
};

struct bmp_all {
	struct bmp_header header;
	struct bmp_info info;
};

struct pixel {
	char b, g, r;
};

#pragma pack(pop)

void free_array_of_pixels(struct pixel * * array);

struct pixel * * load_bmp(struct bmp_all *all, FILE *input_file);

struct pixel * * crop(int x, int y, int w, int h, struct pixel * * src);

struct pixel * * rotate(int W, int H, struct pixel * * src);

void save_bmp(int W, int H, FILE *output_file, struct bmp_all *all, struct pixel * * src);
