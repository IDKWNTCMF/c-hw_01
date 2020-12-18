#include "bmp.h"
#include "stego.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct pixel * * make_array_of_pixels(int w, int h) {
	int array_size = sizeof(struct pixel *) * h + sizeof(struct pixel) * h * w;
	struct pixel * * array = (struct pixel * *)malloc(array_size);
	if (array == NULL) {
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

int main(int argc, char * * argv) {
	if (argc < 3) {
		printf("Wrong number of arguments!\n");
		return 1;
	}
	FILE *input_file = NULL;
	input_file = fopen(argv[2], "rb");
	if (input_file == NULL) {
		printf("Missing input file!\n");
		return 2;
	}
	
	struct bmp_header header;
	struct bmp_info info;
	fread(&header, sizeof(struct bmp_header), 1, input_file);
	fread(&info, sizeof(struct bmp_info), 1, input_file);
	
	int W = info.bi_width, H = info.bi_height;
	struct pixel * * image = make_array_of_pixels(W, H);
	if (image == NULL) {
		printf("Error in malloc!");
		return 3;
	}
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
			return 4;
		}
		
		int x = strtol(argv[4], NULL, 10), y = strtol(argv[5], NULL, 10);
		int w = strtol(argv[6], NULL, 10), h = strtol(argv[7], NULL, 10);
		
		struct pixel * * cropped = make_array_of_pixels(w, h);
		if (cropped == NULL) {
			printf("Error in malloc!");
			return 3;
		}
		
		crop(x, y, W, H, w, h, image, cropped);
		free_array_of_pixels(image);
		
		int rotated_w = h, rotated_h = w;
		struct pixel * * rotated = make_array_of_pixels(rotated_w, rotated_h);
		if (rotated == NULL) {
			free_array_of_pixels(cropped);
			printf("Error in malloc!");
			return 3;
		}
		
		rotate(w, h, cropped, rotated);
		free_array_of_pixels(cropped);
		
		save_bmp(h, w, output_file, &header, &info, rotated);
		free_array_of_pixels(rotated);
		
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
			return 4;
		}
		key = fopen(argv[4], "r");
		if (key == NULL) {
			printf("Missing key file!\n");
			return 5;
		}
		message = fopen(argv[5], "r");
		if (message == NULL) {
			printf("Missing message file!\n");
			return 6;
		}
		
		char ch;
		while (fscanf(message, "%c", &ch) != -1) {
			insert(ch, H, image, key);
		}
		
		save_bmp(W, H, output_file, &header, &info, image);
		free_array_of_pixels(image);
		
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
			return 5;
		}
		message = fopen(argv[4], "w");
		if (message == NULL) {
			printf("Missing message file!\n");
			return 6;
		}
		
		char ch = extract(H, image, key);
		while (ch != '\0') {
			fprintf(message, "%c", ch);
			ch = extract(H, image, key);
		}
		free_array_of_pixels(image);
		
		fclose(key);
		fclose(message);
	}
	return 0;
}
