#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<pthread.h>

#define BMP_HEADER_SIZE 54
#define DIB_HEADER_SIZE 40

#define TRUE 1
#define FALSE 0
#define BINS 256
#define MAX_THREADS 3000

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// not to add spaces b/w attributes --for mem access
#pragma pack(1)

// A BMP file has a header (54 bytes) and data
typedef struct _BMPHeader {
	uint16_t type;			// Magic identifier 0x4d42
	uint32_t size;			// File size in bytes
	uint16_t reserved1;			// Not used
	uint16_t reserved2;			// Not used
	uint32_t offset;			// Offset to image data in bytes from beginning of file (54 bytes)
	uint32_t DIB_header_size;		// DIB Header size in bytes (40 bytes)
	int32_t  width;			// Width of the image
	int32_t  height;			// Height of image
	uint16_t planes;			// Number of color planes
	uint16_t bits;			// Bits per pixel
	uint32_t compression;		// Compression type
	uint32_t imagesize;			// Image size in bytes
	int32_t  xresolution;		// Pixels per meter
	int32_t  yresolution;		// Pixels per meter
	uint32_t ncolours;			// Number of colors
	uint32_t importantcolours;		// Important colors
} BMPHeader;

typedef struct {
	BMPHeader header;
	unsigned char * data;
} BMPImage;

typedef struct {
	BMPImage* adaptive;
	BMPImage* gray;
	int** col_hist;
	int radius;
	int mid_height;
} HistInfo;

BMPImage *BMP_Open(const char *filename);
int Is_BMPHeader_Valid(BMPHeader *bmp_hdr, FILE *fptr);
int BMP_Write(const char * outfile, BMPImage *image);
void BMP_Free(BMPImage *image);

int RGB2Gray(unsigned char red, unsigned char green, unsigned char blue);
BMPImage * ImgToGray(BMPImage * image);
BMPImage * AdaptiveThresholding(BMPImage * grayImage, int radius);

void deallocate_mem(int*** arr, int n);
int** allocate_mem(int bins, int width);
int* initializeHist(int radius, BMPImage* adaptive, int* kernel_hist);
int calcMedian(int* kernel_hist, int radius);
int** initializeColHist(BMPImage* grayImage, int radius, int max_width, int** chist);

int* updateKernelRow(int* kernel_hist, int** col_hists, int i, int j, int radius);
int** updateColBox(BMPImage* gray, int** col_hists, int i, int radius, int max_width);
int* updateKernelCol(BMPImage* gray, int* kernel_hist, int i, int radius);

void* shiftKernel(void* args);