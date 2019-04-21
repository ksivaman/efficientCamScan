# efficientCamScan
An implementation of the paper https://nomis80.org/ctmf.pdf in C (pthreads have been used instead of SMID instructions). 

The executable named 'scan' has been provided. To use, run the following:

>./scan pathtonoisyimage pathtocleanimage radius epsilon

Here, radius is the kernel size applied for filtering.
