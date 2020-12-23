#include "bmp.h"
#include "stego.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int base = 10;
const int files_for_crop_rotate = 8;
const int files_for_insert = 6;
const int files_for_extract = 5;

void free_array_of_pixels(struct pixel * * array) {
	free(array);
}

int main(int argc, char * * argv) {
	if (argc < files_for_extract) {
		printf("Wrong number of arguments!\n");
		return 1;
	}
	char * mode = argv[1];
	FILE *input_file = NULL;
	input_file = fopen(argv[2], "rb");
	if (input_file == NULL) {
		printf("Missing input file!\n");
		return 1;
	}
	
	struct bmp_header header;
	struct bmp_info info;
	
	struct pixel * * image = load_bmp(&header, &info, input_file);
	if (image == NULL) return 1;
	fclose(input_file);
	
	if (strcmp(mode, "crop-rotate") == 0) {	
		if (argc != files_for_crop_rotate) {
			printf("Wrong number of arguments!");
			free_array_of_pixels(image);
			return 1;
		}
		
		FILE *output_file = NULL;
		output_file = fopen(argv[3], "wb");
		if (output_file == NULL) {
			printf("Missing output file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		
		int x = strtol(argv[4], NULL, base), y = strtol(argv[5], NULL, base);
		int w = strtol(argv[6], NULL, base), h = strtol(argv[7], NULL, base);
		
		struct pixel * * cropped = crop(x, y, w, h, image);
		if (cropped == NULL) return 1;
		
		struct pixel * * rotated = rotate(w, h, cropped);
		if (rotated == NULL) return 1;
		
		save_bmp(h, w, output_file, &header, &info, rotated);
		fclose(output_file);
	}
	
	if (strcmp(mode, "insert") == 0) {
		if (argc != files_for_insert) {
			printf("Wrong number of arguments!");
			free_array_of_pixels(image);
			return 1;
		}
		
		FILE *output_file = NULL, *key = NULL, *message = NULL;
		output_file = fopen(argv[3], "wb");
		if (output_file == NULL) {
			printf("Missing output file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		key = fopen(argv[4], "r");
		if (key == NULL) {
			printf("Missing key file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		message = fopen(argv[5], "r");
		if (message == NULL) {
			printf("Missing message file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		
		char ch;
		while (fscanf(message, "%c", &ch) != -1 && ch != '\n') {
			insert(ch, image, key);
		}
		
		int W = info.bi_width, H = info.bi_height;
		save_bmp(W, H, output_file, &header, &info, image);
		
		fclose(key);
		fclose(message);
		fclose(output_file);
	}
	
	if (strcmp(mode, "extract") == 0) {
		if (argc != files_for_extract) {
			printf("Wrong number of arguments!");
			free_array_of_pixels(image);
			return 1;
		}
		
		FILE *key = NULL, *message = NULL;
		key = fopen(argv[3], "r");
		if (key == NULL) {
			printf("Missing key file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		message = fopen(argv[4], "w");
		if (message == NULL) {
			printf("Missing message file!\n");
			free_array_of_pixels(image);
			return 1;
		}
		
		char ch = extract(image, key);
		while (ch != '\0') {
			fprintf(message, "%c", ch);
			ch = extract(image, key);
		}
		fprintf(message, "\n");
		free_array_of_pixels(image);
		
		fclose(key);
		fclose(message);
	}
	return 0;
}
