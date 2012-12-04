#include "kde_util.h"
#include <stdio.h>
#include <fftw3.h>

int print_vec(double *v, char* title, int start, int end)
{
	XML_IN_T(title);
	//printf("%s: ",title);
	for(int i=start;i<end;i++)
		printf(" %g",v[i]);
	printf("\n");
	XML_OUT;
}

int file_read_into_array_doubles(const char *filename , double **out_data, int *length)
{
    FILE *in_file;
    in_file = fopen(filename, "r");
	 double *data=NULL;

    if (in_file == NULL)
    {
        return -1;
    }
    else
    {
		 fscanf(in_file, "%d", length);
		 printf("length: %d\n", *length);
		 data =(double*)malloc((*length)*sizeof(*data));

        for(int j=0; j<*length; j++)
        {
            fscanf(in_file, "%lg", data+j);
        }
        fclose(in_file);
		  *out_data=data;
    }
    return 0;
}

void kde_dct_fftw(double *in, int n, double* out)
{

	double* in_n=malloc(n*sizeof(*in_n));
	fftw_plan dct =	fftw_plan_r2r_1d(n, in_n, out, FFTW_REDFT10, FFTW_MEASURE);

	for(int i = 0; i < n; i++)
		in_n[i]=in[i]*2*n;

	fftw_execute(dct);

	out[0]/=2.0;

	fftw_destroy_plan(dct);
	free(in_n);
}


void kde_idct_fftw(double *in, int n, double* out)
{

	double* in_n=malloc(n*sizeof(*in_n));
	fftw_plan idct =	fftw_plan_r2r_1d(3, in_n, out, FFTW_REDFT01, FFTW_MEASURE);
	

	for(int i = 0; i < n; i++)
		in_n[i]=in[i];

		in_n[0]*=2;

	fftw_execute(idct);
	
	for(int i = 0; i < n; i++)
		out[i]/=2.0;

	fftw_destroy_plan(idct);
	free(in_n);
}



double fixed_point(double t, int N, double *It, double *a2, int n)
{
	/*	function  out=fixed_point(t,N,I,a2)
		% this implements the function t-zeta*gamma^[l](t)
		*/
	//l=7;
	int l=7;
	double sum_f=0;
	double f=0;
	for(int i=0;i<n;i++)
		sum_f+=pow(It[i],l)*a2[i]*exp(-It[i]*pow(M_PI,2.0)*t);
	f=2*pow(M_PI,2.0*l)*sum_f;

	/*
		f=2*pi^(2*l)*sum(I.^l.*a2.*exp(-I*pi^2*t));
		for s=l-1:-1:2
		K0=prod([1:2:2*s-1])/sqrt(2*pi);  const=(1+(1/2)^(s+1/2))/3;
		time=(2*const*K0/N/f)^(2/(3+2*s));
		f=2*pi^(2*s)*sum(I.^s.*a2.*exp(-I*pi^2*time));
		end
		out=t-(2*N*sqrt(pi)*f)^(-2/5);
		end
		*/
	for(int s=l-1;l>=2;l--)
	{
		double c=(1+pow(0.5,(s+1)/2.0))/3;
		double k0=1;

		for(int i=1;i<s;i+=2)
			k0*=i/sqrt(2*M_PI);
		double tim=pow(2*c*k0/N/f,2.0/(3+2*s));
		
		for(int i=0;i<n;i++)
			sum_f+=pow(It[i],s)*a2[i]*exp(-It[i]*pow(M_PI,2.0)*tim);
		f=2*pow(M_PI,2.0*s)*sum_f;

	
	}
	double out=t-pow(2*N*sqrt(M_PI)*f,-2/5.0);
	return out;
}

