#include "kde_util.h"
#include <stdio.h>
#include <fftw3.h>
#include <math.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

int print_vec(double *v, char* title, int start, int end)
{
	XML_IN_T(title);
	//printf("%s: ",title);
	for(int i=start;i<end;i++)
		printf(" %g",v[i]);
	printf("\n");
	XML_OUT;
}

void array_write_ascii(double *v, int n, const char* filename)
{
	FILE *fid = fopen(filename, "w");

	fprintf(fid, "%d\n", n);

	for (int i = 0; i < n; i++) {
		fprintf(fid, "%18.12f\n", v[i]);
	}

	fclose(fid);
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

void kde_idct_fftw(double *in, int n, double* out)
{
	double *in_n=malloc(n*sizeof(*in_n));
	fftw_plan idct =	fftw_plan_r2r_1d(n, in_n, out, FFTW_REDFT01, FFTW_MEASURE);

	for(int i = 0; i < n; i++)
		in_n[i]=in[i];

	in_n[0]*=2;

	fftw_execute(idct);

	for(int i = 0; i < n; i++)
		out[i]*=n;

	fftw_destroy_plan(idct);
	free(in_n);
}

void kde_dct_fftw(double *in, int n, double* out)
{
	fftw_plan dct =	fftw_plan_r2r_1d(n, in, out, FFTW_REDFT10, FFTW_MEASURE);

	fftw_execute(dct);

	out[0]/=2.0;

	fftw_destroy_plan(dct);
}

void dct_fftw(double *in, int n, double* out)
{

	fftw_plan dct =	fftw_plan_r2r_1d(n, in, out, FFTW_REDFT10, FFTW_MEASURE);

	fftw_execute(dct);

	fftw_destroy_plan(dct);

}


void idct_fftw(double *in, int n, double* out)
{
	fftw_plan idct =	fftw_plan_r2r_1d(n, in, out, FFTW_REDFT01, FFTW_MEASURE);

	fftw_execute(idct);

	fftw_destroy_plan(idct);
}





double fixed_point(double t, int N, double *It, double *a2, int n)
{
	int verbose=0;
	if(verbose) XML_IN;
	/*	function  out=fixed_point(t,N,I,a2)
		% this implements the function t-zeta*gamma^[l](t)
		*/
	//l=7;
	int l=7;
	double sum_f=0;
	double sum_f_2=0;
	double f=0;
	for(int i=0;i<n-1;i++)
		sum_f+=pow(It[i],l)*a2[i]*exp(-It[i]*pow(M_PI,2.0)*t);
	f=2*pow(M_PI,2.0*l)*sum_f;

	for(int i=0;i<n-1;i++)
			sum_f_2+=pow(It[i],l)*exp(-It[i]*pow(M_PI,2.0)*t);

	if(verbose) printf("t: %g\n",t);
	if(verbose) printf("sum_f_2: %g sum_f: %g f: %g\n",sum_f_2,sum_f,f);

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
	for(int s=l-1;s>=2;s--)
	{
		double k0=1;
		for(int i=1;i<2*s;i+=2)
			k0*=i;
		k0/=sqrt(2*M_PI);

		double c=(1.0+pow(0.5,s+0.5))/3.0;
		double tim=pow(2.0*c*k0/N/f,2.0/(3.0+2.0*s));
		double tim2=2.0*c*k0/N/f;
		if(verbose)  printf("N: %d tim2: %g\n",N,tim2);
		sum_f=0;
		for(int i=0;i<n-1;i++)
			sum_f+=pow(It[i],s)*a2[i]*exp(-It[i]*pow(M_PI,2.0)*tim);
		f=2*pow(M_PI,2.0*s)*sum_f;
		if(verbose) printf("s: %d c: %g k0: %g tim: %g f: %g \n",s,c,k0,tim,f);
		if(verbose) printf("sum_f: %g\n",sum_f);
	}
	double out=t-pow(2*N*sqrt(M_PI)*f,-2.0/5.0);
	if(verbose) XML_OUT;
	return out;
}

void peakdet( int n, double *x, double *v, double delta, int* l_min, double** min_x, int* l_max, double** max_x)
{
	XML_IN;

	int verbose=0;

	double* maxtab=malloc(n*sizeof(*maxtab));
	double* mintab=malloc(n*sizeof(*mintab));

	int idx_min=0,idx_max=0;

	//add a minimum in 0
	mintab[idx_min++]=x[0];

	double mn=999,mx=-999;
	double mnpos=-1,mxpos=-1;

	int lookformax=1;

	for(int i=0;i<n;i++)
	{
		double this=v[i];
		if(this>mx)
		{
			mx=this; mxpos=x[i];
		}

		if(this<mn)
		{
			mn=this; mnpos=x[i];
		}

		if(lookformax)
		{
			if(this<mx-delta)
			{
				maxtab[idx_max]=mxpos;
				mn=this; mnpos=x[i];
				idx_max++;
				lookformax=0;
			}
		}else
		{
			if(this>mn+delta)
			{
				mintab[idx_min]=mnpos;
				mx=this; mxpos=x[i];
				idx_min++;
				lookformax=1;
			}
		}
	}

	mintab[idx_min++]=x[n-1];

	mintab=realloc(mintab,idx_min*sizeof(*mintab));
	maxtab=realloc(maxtab,idx_max*sizeof(*maxtab));

	//for debug only
	if(verbose)
	{
		print_vec(mintab,"mintab",0,idx_min);
		print_vec(maxtab,"maxtab",0,idx_max);
	}

	//assign output
	*min_x=mintab;
	*max_x=maxtab;
	*l_min=idx_min;
	*l_max=idx_max;

	XML_OUT;
}

#define swap(a,b,t) ((t)=(a),(a)=(b),(b)=(t))

void quick(int array[], int start, int end)
{
	   static int t;
    if(start < end){
        int l=start+1, r=end, p = array[start];
        while(l<r){
            if(array[l] <= p)
                l++;
            else if(array[r] >= p)
                r--;
            else
                swap(array[l],array[r],t);
        }
        if(array[l] < p){
            swap(array[l],array[start],t);
            l--;
        }
        else{
            l--;
            swap(array[l],array[start],t);
        }
        quick(array, start, l);
        quick(array, r, end);
    }
}
