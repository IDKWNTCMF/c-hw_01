#include "bmp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int required_number_of_bytes = 4;

struct pixel * * make_array_of_pixels(int w, int h) {
	int array_size = sizeof(struct pixel *) * h + sizeof(struct pixel) * h * w;
	struct pixel * * array = (struct pixel * *)malloc(array_size);
	if (array == NULL) {
		printf("Error in malloc!");
		return NULL;
	}
	struct pixel * array_begin = (struct pixel *)(array + h);
	for (int i = 0; i < h; i++) {
		array[i] = (array_begin + w * i);
	}
	return array;
}

void free_array_of_pixels(struct pixel * * array) {
	free(array);
}

struct pixel * * load_bmp(struct bmp_all *all, FILE *input_file) {
	fread(all, sizeof(struct bmp_all), 1, input_file);

	int W = all->info.bi_width, H = all->info.bi_height;
	struct pixel * * image = make_array_of_pixels(W, H);
	if (image == NULL) return NULL;
	
	int buf_sz = W % required_number_of_bytes;
	char buf[buf_sz];
	for (int h = H - 1; h >= 0; h--) {
		fread(image[h], sizeof(struct pixel), W, input_file);
		fread(buf, sizeof(char), buf_sz, input_file);
	}
	return image;
}

struct pixel * * crop(int x, int y, int w, int h, struct pixel * * src) {
	struct pixel * * dest = make_array_of_pixels(w, h);
	if (dest == NULL) {
		free(src);
		return NULL;
	}
	
	for (int cur_y = y; cur_y < y + h; cur_y++) {
		memcpy(&dest[cur_y - y][0], &src[cur_y][x], w * sizeof(struct pixel));
	}
	free(src);
	return dest;
}

struct pixel * * rotate(int W, int H, struct pixel * * src) {
	struct pixel * * dest = make_array_of_pixels(H, W);
	if (dest == NULL) {
		free(src);
		return NULL;
	}
	for (int cur_y = 0; cur_y < H; cur_y++) {
		for (int cur_x = 0; cur_x < W; cur_x++) {
			int rotated_y = cur_x, rotated_x = H - 1 - cur_y;
			dest[rotated_y][rotated_x] = src[cur_y][cur_x];
		}
	}
	free(src);
	return dest;
}

void save_bmp(int W, int H, FILE *output_file, struct bmp_all *all, struct pixel * * src) {
	int buf_sz = W % required_number_of_bytes;
	char buf[buf_sz];
	memset(buf, 0, buf_sz);
	
	all->info.bi_width = W;
	all->info.bi_height = H;
	all->info.bi_size_image = H * W * sizeof(struct pixel) + H * buf_sz;
	all->header.bf_size = sizeof(struct bmp_all) + all->info.bi_size_image;
	
	fwrite(all, sizeof(struct bmp_all), 1, output_file);
	for (int h = H - 1; h >= 0; h--) {
		fwrite(src[h], sizeof(struct pixel), W, output_file);
		fwrite(buf, sizeof(char), buf_sz, output_file);
	}
	free(src);
}
