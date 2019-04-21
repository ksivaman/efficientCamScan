#include"utils.h"

int** initializeColHist(BMPImage* grayImage, int radius, int max_width, int** chist, int top)
{
    for (int i = 0 + top; i < 2*radius+1 + top; i++)
    {
        for (int j = 0; j < max_width; j++)
        {
            int val = grayImage->data[(i*max_width+j)*3];
            chist[val][j] += 1;
        }
    }

    return chist;
}

void deallocate_mem(int*** arr, int n){
    for (int i = 0; i < n; i++)
        free((*arr)[i]);
    free(*arr); 
}

int** allocate_mem(int bins, int width)
{
    int** arr = malloc(bins*sizeof(int*));
    for(int i=0; i<bins; i++)
        arr[i] = malloc(width*sizeof(int));

    int cnt_i = 0, cnt_j = 0;
    for (int i = 0; i < bins; i++)
    {
        cnt_i++;
        for (int j = 0; j < width; j++)
        {
            cnt_j++;
            arr[i][j] = 0;
        }
    }

    return arr;
} 

int* initializeHist(int radius, BMPImage* gray, int* kernel_hist, int top)
{
	int max_width = (gray->header).width;

    //initialize the histogram
	for (int row = 0 + top; row < 2*radius+1 + top; row++)
	{
		for(int col = 0; col < 2*radius+1; col++)
		{
			int index = gray->data[(row*max_width + col)*3];
			kernel_hist[index] += 1;
		}
	}

    return kernel_hist;
}

int calcMedian(int* kernel_hist, int rad)
{
    int k = 0;
    int sum = 0;
    while(sum < ((2*rad+1)*(2*rad+1)/2))
    {
        sum += kernel_hist[k];
        k++;
    }
    return k;
}

int* updateKernelRow(int* kernel_hist, int** col_hists, int i, int j, int radius)
{
    for (int k = 0; k < BINS; k++)
    {   
        int new_pix = col_hists[k][j+radius+1];
        int old_pix = col_hists[k][j-radius];
        kernel_hist[new_pix] += 1;
        kernel_hist[old_pix] -= 1;
    }

    return kernel_hist;
}

int** updateColBox(BMPImage* gray, int** col_hists, int i, int radius, int max_width)
{
    for (int k = 0; k < max_width; k++)
    {
        int old_pix = gray->data[( (max_width * (i - radius)) + k )*3];
        int new_pix = gray->data[( (max_width * (i + radius + 1)) + k )*3];

        col_hists[new_pix][k] += 1;
        col_hists[old_pix][k] -= 1;
    }

    return col_hists;
}

int* updateKernelCol(BMPImage* gray, int* kernel_hist, int i, int radius)
{
    int max_width = (gray->header).width;

    for (int p = 0; p < BINS; p++)
    {
        kernel_hist[p] = 0;
    }

	for (int row = i - radius + 1; row < radius + i + 2; row++)
	{
		for(int col = 0; col < 2*radius+1; col++)
		{
			int index = gray->data[(row*max_width + col)*3];
			kernel_hist[index] += 1;
		}
	}

    return kernel_hist;
}