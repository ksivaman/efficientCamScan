#include "bmpfunc.h"
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

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

	if ((adaptive->data = malloc(sizeof(unsigned char)*(adaptive->header).imagesize)) == NULL) {
		fprintf(stderr, "Error allocating memory\n");
		free(adaptive);
		return NULL;
	}

	int height = 0;
	int width = 0;

	for (height = 0; height < (grayImage->header).height; height++)
	{
		for (width = 0; width < (grayImage->header).width; width++)
		{
			int toprow = MAX(0, height-radius);
			int bottomrow = MIN((grayImage->header).height-1, height+radius);
			int leftcol = MAX(0, width-radius);
			int rightcol = MIN((grayImage->header).width-1, width+radius);
			int i = 0;
			int count = 0;
			int average = 0;
			int j = 0;
			int sum = 0; 
			for (i = toprow; i<=bottomrow; i++)
			{
				for (j = leftcol; j<=rightcol; j++)
				{
					sum += grayImage->data[(i*(grayImage->header).width+j)*3];
					count++; 
				}              
			}
			average = sum / count;
			if (average > grayImage->data[(height*(grayImage->header).width+width)*3])
			{
				adaptive->data[(height*(grayImage->header).width+width)*3]=0;
				adaptive->data[(height*(grayImage->header).width+width)*3+1]=0;
				adaptive->data[(height*(grayImage->header).width+width)*3+2]=0;      
			}
			else
			{
				adaptive->data[(height*(grayImage->header).width+width)*3]=255;
				adaptive->data[(height*(grayImage->header).width+width)*3+1]=255;
				adaptive->data[(height*(grayImage->header).width+width)*3+2]=255;   
			}
		}
	}

	return adaptive;        
}
