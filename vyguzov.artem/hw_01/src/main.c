#include "bmp.h"
#include "stego.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * * argv) {
	if (argc < 3) {
		printf("Wrong number of arguments!\n");
		return 1;
	}
	FILE *input_file = NULL;
	input_file = fopen(argv[2], "rb");
	if (input_file == NULL) {
		printf("Missing input file!\n");
		return 1;
	}
	
	struct bmp_header header;
	struct bmp_info info;
	fread(&header, sizeof(struct bmp_header), 1, input_file);
	fread(&info, sizeof(struct bmp_info), 1, input_file);
	
	int W = info.bi_width, H = info.bi_height;
	struct pixel image[H][W];
	load_bmp(W, H, input_file, image);
	fclose(input_file);
	
	if (strcmp(argv[1], "crop-rotate") == 0) {	
		if (argc != 8) {
			printf("Wrong number of arguments!");
			return 1;
		}
		
		FILE *output_file = NULL;
		output_file = fopen(argv[3], "wb");
		if (output_file == NULL) {
			printf("Missing output file!\n");
			return 1;
		}
		
		int x = strtol(argv[4], NULL, 10), y = strtol(argv[5], NULL, 10);
		int w = strtol(argv[6], NULL, 10), h = strtol(argv[7], NULL, 10);
		
		struct pixel cropped[h][w];
		crop(x, y, W, H, w, h, image, cropped);
		
		struct pixel rotated[w][h];
		rotate(w, h, cropped, rotated);
		
		save_bmp(h, w, output_file, &header, &info, rotated);
		
		fclose(output_file);
	}
	
	if (strcmp(argv[1], "insert") == 0) {
		if (argc != 6) {
			printf("Wrong number of arguments!");
			return 1;
		}
		
		FILE *output_file = NULL, *key = NULL, *message = NULL;
		output_file = fopen(argv[3], "wb");
		if (output_file == NULL) {
			printf("Missing output file!\n");
			return 1;
		}
		key = fopen(argv[4], "r");
		if (key == NULL) {
			printf("Missing key file!\n");
			return 1;
		}
		message = fopen(argv[5], "r");
		if (message == NULL) {
			printf("Missing message file!\n");
			return 1;
		}
		
		char ch;
		while (fscanf(message, "%c", &ch) != -1) {
			insert(ch, W, H, image, key);
		}
		
		save_bmp(W, H, output_file, &header, &info, image);
		
		fclose(key);
		fclose(message);
		fclose(output_file);
	}
	
	if (strcmp(argv[1], "extract") == 0) {
		if (argc != 5) {
			printf("Wrong number of arguments!");
			return 1;
		}
		
		FILE *key = NULL, *message = NULL;
		key = fopen(argv[3], "r");
		if (key == NULL) {
			printf("Missing key file!\n");
			return 1;
		}
		message = fopen(argv[4], "w");
		if (message == NULL) {
			printf("Missing message file!\n");
			return 1;
		}
		
		char ch = extract(W, H, image, key);
		while (ch != '\0') {
			fprintf(message, "%c", ch);
			ch = extract(W, H, image, key);
		}
	}
	return 0;
}
