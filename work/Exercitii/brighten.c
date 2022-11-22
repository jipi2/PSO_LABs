#include <stdio.h>
#include <time.h>
#include <mmintrin.h>

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

	// Read bitmap and increase brightness
	// TODO: Modify it to use MMX and SSE2 extensions for paralelism
	while ( !feof(fid) ) {
		read = fread(buffer, 1, 1024, fid);
		for (int i = 0; i < read; i++) {
			if (buffer[i] + 40 > 255)
				buffer[i] = 0xff;
			else
				buffer[i] += 40;
		}
		fwrite(buffer, read, 1, fout);
	}
	clock_t stop = clock();
	printf("Elapsed: %f seconds\n", (double)(stop - start) / CLOCKS_PER_SEC);

	fclose(fid);
	fclose(fout);
	
	return 0;
}
