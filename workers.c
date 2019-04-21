#include"utils.h"

/*thread worker to shift the filter kernel to the right, and calculate the median 
for the middle cell and assign to output image*/
void *shiftKernel(void* args)
{
    HistInfo* hi = (HistInfo*) args;
    BMPImage* gray = hi->gray;

    int *kernel_hist = malloc(sizeof(int) * BINS);

    //initializing relevant histograms
    kernel_hist = initializeHist(hi->radius, gray, kernel_hist, hi->mid_height);
	int** col_hists = allocate_mem(BINS, hi->max_width);
	col_hists = initializeColHist(gray, hi->radius, hi->max_width, col_hists, hi->mid_height);

    int i = hi->mid_height + hi->radius;

    //moving along one row
    for(int j = hi->radius; j < (hi->max_width) - (hi->radius) - 1; j++)
    {
        int median = calcMedian(kernel_hist, hi->radius);
        (hi->adaptive)->data[(i*(hi->max_width)+j)*3] = median;
        (hi->adaptive)->data[(i*(hi->max_width)+j)*3 + 1] = median;
        (hi->adaptive)->data[(i*(hi->max_width)+j)*3 + 2] = median;

        kernel_hist = updateKernelRow(kernel_hist, col_hists, i, j, hi->radius);
    }

    pthread_exit(0);
}