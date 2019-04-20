#include "utils.h"

int main(int argc, char **argv){
	if (argc != 4)
	{
		fprintf(stderr, "Wrong arguments\n");
		return EXIT_FAILURE;
	}
	// check radius and epsilon values -  read readme for the validity of argument
	char *Rad = argv[3];
	int radius = atoi(Rad); 

	// open the BMP file
	BMPImage *bmpImage = BMP_Open(argv[1]);
	if (bmpImage == NULL)
	{
		fprintf(stderr, "Error opening BMP file\n");
		return EXIT_FAILURE;
	}

	// convert to gray scale
	// check for error in converting to gray scale
	BMPImage *grayImage = ImgToGray(bmpImage);
	// check for error in converting to gray scale
	if (grayImage == NULL)
	{
		fprintf(stderr, "Error converting BMP file\n");
		return EXIT_FAILURE;
	}
	// call adaptive threshold function
	BMPImage *adaptive = AdaptiveThresholding(grayImage, radius);

	if (adaptive == NULL)
	{
		fprintf(stderr, "Error converting BMP file\n");
		return EXIT_FAILURE;
	}
	// check for errors after calling adaptive threshold
	// write the adaptive threshold image to file
	// free all the images

	if (BMP_Write(argv[2], adaptive) != 1)
	{
		fprintf(stderr, "Error writing BMP image to file\n");
		return EXIT_FAILURE;
	}
	// free all the images
	BMP_Free(bmpImage);
	BMP_Free(grayImage);
	BMP_Free(adaptive);
	return EXIT_SUCCESS;
}
