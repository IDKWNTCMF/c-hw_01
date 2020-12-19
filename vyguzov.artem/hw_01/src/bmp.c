#include "bmp.h"
#include <stdio.h>
#include <string.h>

void load_bmp(int W, int H, FILE *input_file, struct pixel * * dest) {
	char buf[W % 4];
	for (int h = H - 1; h >= 0; h--) {
		fread(dest[h], sizeof(struct pixel), W, input_file);
		fread(buf, sizeof(char), W % 4, input_file);
	}
}

void crop(int x, int y, int w, int h, struct pixel * * src, struct pixel * * dest) {
	for (int cur_y = y; cur_y < y + h; cur_y++) {
		memcpy(&dest[cur_y - y][0], &src[cur_y][x], w * sizeof(struct pixel));
	}
}

void rotate(int W, int H, struct pixel * * src, struct pixel * * dest) {
	for (int cur_y = 0; cur_y < H; cur_y++) {
		for (int cur_x = 0; cur_x < W; cur_x++) {
			int rotated_y = cur_x, rotated_x = H - 1 - cur_y;
			dest[rotated_y][rotated_x] = src[cur_y][cur_x];
		}
	}
}

void save_bmp(int W, int H, FILE *output_file, struct bmp_header *header, struct bmp_info *info, struct pixel * * src) {
	char buf[W % 4];
	memset(buf, 0, sizeof(buf));
	header->bf_size = sizeof(struct bmp_header) + sizeof(struct bmp_info) + H * W * sizeof(struct pixel) + H * sizeof(buf);
	info->bi_width = W;
	info->bi_height = H;
	fwrite(header, sizeof(struct bmp_header), 1, output_file);
	fwrite(info, sizeof(struct bmp_info), 1, output_file);
	for (int h = H - 1; h >= 0; h--) {
		fwrite(src[h], sizeof(struct pixel), W, output_file);
		fwrite(buf, sizeof(char), W % 4, output_file);
	} 
}
