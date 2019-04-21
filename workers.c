#include"utils.h"

void *shiftKernel(void* args)
{
    HistInfo* hi = (HistInfo*) args;
    BMPImage* gray = hi->gray;
    //BMPImage* adaptive = hi->adaptive;

    int *kernel_hist = malloc(sizeof(int) * BINS);

    kernel_hist = initializeHist(hi->radius, gray, kernel_hist, hi->mid_height);
	int** col_hists = allocate_mem(BINS, hi->max_width);
	col_hists = initializeColHist(gray, hi->radius, hi->max_width, col_hists, hi->mid_height);

    int i = hi->mid_height + hi->radius;

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