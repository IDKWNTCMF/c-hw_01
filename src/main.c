#include "bmp.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * * argv) {
	if (argc != 8) {
		printf("Wrong number of arguments!\n");
		return 1;
	}
	if (strcmp(argv[1], "crop-rotate") == 0) {
		FILE *input_file = NULL, *output_file = NULL;
		input_file = fopen(argv[2], "rb");
		output_file = fopen(argv[3], "wb");
		
		struct bmp_header header;
		struct bmp_info info;
		fread(&header, sizeof(struct bmp_header), 1, input_file);
		fread(&info, sizeof(struct bmp_info), 1, input_file);
		
		int x = strtol(argv[4], NULL, 10), y = strtol(argv[5], NULL, 10);
		int w = strtol(argv[6], NULL, 10), h = strtol(argv[7], NULL, 10);
		int W = info.bi_width, H = info.bi_height;
		
		struct pixel image[H][W];
		load_bmp(W, H, input_file, image);
		
		struct pixel cropped[h][w];
		crop(x, y, W, H, w, h, image, cropped);
		
		struct pixel rotated[w][h];
		rotate(w, h, cropped, rotated);
		
		save_bmp(h, w, output_file, &header, &info, rotated);
		
		fclose(input_file);
		fclose(output_file);
	} 
	return 0;
}
