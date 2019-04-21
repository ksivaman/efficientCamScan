#include"utils.h"

void *shiftKernel(void* args)
{
    HistInfo* hi = (HistInfo*) args;
    
    pthread_exit(0);
}

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