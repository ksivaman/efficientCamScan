#include"utils.h"
#define BINS 256

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

int* initializeHist(int radius, BMPImage* adaptive, int* kernel_hist)
{
	int max_width = (adaptive->header).width;

    //initialize the histogram
	for (int row = 0; row < 2*radius+1; row++)
	{
		for(int col = 0; col < 2*radius+1; col++)
		{
			int index = adaptive->data[(row*max_width + col)*3];
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