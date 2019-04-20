#include "utils.h"

int RGB2Gray(unsigned char red, unsigned char green, unsigned char blue){
	// this is a commonly used formula --https://en.wikipedia.org/wiki/Grayscale#Converting_color_to_grayscale
	int gray = 0.2989 * red + 0.5870 * green + 0.1140 * blue;
	return gray;
}

BMPImage * ImgToGray(BMPImage * image){

	BMPImage *gray_image = (BMPImage *)malloc(sizeof(BMPImage));

	if (gray_image == NULL)
	{
		fprintf(stderr, "Error in allocating memory\n");
		free(gray_image);
		return NULL;
	}

	gray_image->header = image->header;

	(gray_image->header).imagesize = (image->header).imagesize;

	if ((gray_image->data = (unsigned char *)malloc(sizeof(unsigned char)*(int)(gray_image->header).imagesize)) == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(gray_image->data);
		return NULL;
	}

	int pixel=0;

	for (pixel = 0; pixel < (gray_image->header).imagesize; pixel+=3)
	{
		int gray_val = RGB2Gray(image -> data[pixel+2],image -> data[pixel + 1], image -> data[pixel]);
		gray_image->data[pixel+2] = gray_val;
		gray_image->data[pixel+1] = gray_val;
		gray_image->data[pixel] = gray_val;
	}

	return gray_image;
}

BMPImage * AdaptiveThresholding(BMPImage * grayImage, int radius){
	// allocate space for the image
	BMPImage * adaptive = (BMPImage *)malloc(sizeof(BMPImage));
	if (adaptive == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		free(adaptive);
		return NULL;
	}

	adaptive->header = grayImage->header;
	adaptive->data = malloc(sizeof(unsigned char)*(adaptive->header).imagesize);

	if (adaptive->data == NULL) 
	{
		fprintf(stderr, "Error allocating memory\n");
		free(adaptive);
		return NULL;
	}

	//copying data
	memcpy((adaptive->data), (grayImage->data), sizeof(unsigned char)*(adaptive->header).imagesize);

	int max_width = (grayImage->header).width;
	int max_height = (grayImage->header).height;

	int *kernel_hist = malloc(sizeof(int) * BINS);

	for (int v=0; v<BINS; v++)
		kernel_hist[v] = 0;

	kernel_hist = initializeHist(radius, grayImage, kernel_hist);
	int** col_hists = allocate_mem(BINS, max_width);
	col_hists = initializeColHist(grayImage, radius, max_width, col_hists);

	BMP_Write("grayscale/outputRV2Small.bmp", adaptive);

	for(int i = radius; i <= max_height - radius - 1; i++)
	{
		for(int j = radius; j < max_width - radius - 1; j++)
		{
			int median = calcMedian(kernel_hist, radius);
			adaptive->data[(i*max_width+j)*3] = median;
			adaptive->data[(i*max_width+j)*3 + 1] = median;
			adaptive->data[(i*max_width+j)*3 + 2] = median;

			kernel_hist = updateKernelRow(kernel_hist, col_hists, i, j, radius);
		}
		if (i != (max_height - radius - 1))
		{
			col_hists = updateColBox(grayImage, col_hists, i, radius, max_width);
			kernel_hist = updateKernelCol(grayImage, kernel_hist, i, radius);
		}
	}

	for(int l=0; l<BINS; l++)
		free(col_hists[l]);

	free(col_hists);
	free(kernel_hist);

	return adaptive;        
}
