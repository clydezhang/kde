#ifndef KDE_H_
#define KDE_H_

#if defined(__cplusplus) && !defined(WIN32)
extern "C" {
#endif


#include <complex.h>

extern int verbose;

int file_read_into_array_doubles_l(const char *filename , double *data, int *length);

int find_max_min_array_doubles(double *a, int length, double *max, double *min);

int compare_doubles (const void *a, const void *b);

double log2(double x);

int histc(double *data, int length, double *xmesh, int n_bins, double *bins);


void kde(double *data, int length, int n ,double dataMIN, double dataMAX, double **out_density, double **out_x, double *bw);

void bones_get_threshold(double* data, int length, double* ths);

#if defined(__cplusplus) && !defined(WIN32)
}
#endif

#endif //KDE_H_
