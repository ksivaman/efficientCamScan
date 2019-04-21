#include "utils.h"

//check for a valid header
int Is_BMPHeader_Valid(BMPHeader* header, FILE *fptr) {
	if (header -> type != 0x4D42)
	{
		return FALSE;
	}
	if (header -> offset != BMP_HEADER_SIZE)
	{
		return FALSE;
	}
	if ((header -> bits) != 24)
	{
		return FALSE;
	}
	if ((header -> planes) != 1)
	{
		return FALSE;
	}
	if ((header -> compression) != 0)
	{
		return FALSE;
	}
	if ((header -> DIB_header_size) != DIB_HEADER_SIZE)
	{
		return FALSE;
	}
	if ((header->ncolours) != 0)
	{
		return FALSE;
	}
	if ((header->importantcolours) != 0)
	{
		return FALSE;
	}
	if ((header->imagesize) != (header->width)*(header->height)*(header->bits)/8)
	{
		return FALSE;
	}
	if ((header->size) != 54 + (header->width)*(header->height)*(header->bits)/8)
	{
		return FALSE;
	}
	return TRUE;
}

BMPImage *BMP_Open(const char *filename) {

	FILE *fptr = fopen(filename,"r");

	BMPImage *bmpImage = (BMPImage *)malloc(sizeof(BMPImage));
	//check for memory allocation errors
	if (bmpImage == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(bmpImage);
		return NULL;
	}
	if (fptr == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return NULL;
	}

	//Read the first 54 bytes of the source into the header
	fseek(fptr, 0, SEEK_SET);
	fread(&(bmpImage->header), sizeof(BMPHeader), 1, fptr);

	//check if the header is valid
	if (Is_BMPHeader_Valid(&(bmpImage->header), fptr) == 0)
	{
		return NULL;
	}
	// Allocate memory for image data

	bmpImage->data = (unsigned char *)malloc(sizeof(unsigned char)*((int)((bmpImage->header).imagesize)));
	//check error
	if (bmpImage->data == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		free(bmpImage);
		return NULL;
	}

	fseek(fptr, (bmpImage->header).offset , SEEK_SET);

	if (fread(bmpImage->data, sizeof(unsigned char)*(int)(bmpImage->header).imagesize, 1, fptr) != 1)
	{
		return NULL;
	}

	fclose(fptr);
	return bmpImage;
}

int BMP_Write(const char * outfile, BMPImage* image){

	FILE *fptr = fopen(outfile, "w");
	//open file and check for error
	if (fptr == NULL)
	{
		fclose(fptr);
		return FALSE;
	}

	fseek(fptr, 0, SEEK_SET);
	//check error for writing
	if (fwrite(& (image -> header), sizeof(image->header), 1, fptr) != 1)
	{
		fclose (fptr);
		return FALSE;
	}

	if (fwrite(image->data, (image->header).imagesize, 1, fptr) != 1)
	{
		fclose(fptr);
		return FALSE;
	}
	fclose(fptr);
	return TRUE;
}

void BMP_Free(BMPImage* image) 
{
	free(image->data);
	free(image);
}
