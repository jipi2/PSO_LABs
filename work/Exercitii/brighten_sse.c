#include <stdio.h>
#include <x86intrin.h>
#include <stdint.h>
#include <time.h>

char _mem[16] = {0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28};

int main() {
	FILE *fid = fopen("atm.bmp", "rb");
	FILE *fout = fopen("atm2.bmp", "wb");

	
	int header_size;
	fseek(fid, 10, SEEK_SET);
	fread(&header_size, 4, 1, fid);
	
	int bits_per_pixel = 0;
	fseek(fid, 0x1C, SEEK_SET);
	fread(&bits_per_pixel, 4, 1, fid);

	printf("header_size = %d\n", header_size);
	printf("bits per pixel = %d\n", bits_per_pixel);

	rewind(fid);	

	unsigned char buffer[1024];
	int read = 0;
	clock_t start = clock();

	// Copy header
	fread(buffer, 1, header_size, fid);
	fwrite(buffer, 1, header_size, fout);

	__m128i load_mem = _mm_load_si128((__m128i const*)_mem);

	__m128i load_buff;
	
	// Read bitmap and increase brightness
	while ( !feof(fid) ) {
		read = fread(buffer, 1, 1024, fid);
		for (int i = 0; i < read; i += 16) {
			//TODO:
			load_buff = _mm_load_si128((__m128i const *)(buffer + i));
			load_buff = _mm_adds_epu8(load_buff, load_mem);
			_mm_store_si128((__m128i *)(buffer+i), load_buff);
		}
		fwrite(buffer, read, 1, fout);
	}
	
	clock_t stop = clock();
	printf("Elapsed: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);

	fclose(fid);
	fclose(fout);
	
	return 0;
}
