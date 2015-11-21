#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "color.h"

#define TRUE  1
#define FALSE 0
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x < y ? x : y)

int main(int argc,char *argv[]){
	int 	i,j,n,ix,iy,
			iters,images,
			screenx,screeny,
			drawit;
	long	secs;
	double	xmin=1e32,xmax=-1e32,ymin=1e32,ymax=-1e32,
			ax[6],ay[6],
			sens,
			t1,t2,
			d0,dd,dx,dy,lyapunov,
			xe,ye,xenew,yenew;
	char	fname[256];
	FILE	*fptr;
	
	screenx=1600/0.5;
	screeny=1200/0.5;
	iters=1000000*15.0;
	sens=0.0125/1.0;
	
	//~ screenx=1600/2.0;
	//~ screeny=1200/2.0;
	//~ iters=10000000;
	//~ sens=0.0125/1.0;
	
	double *x	= malloc(iters*sizeof(double));
	double *y	= malloc(iters*sizeof(double));
	_color *image=malloc(screenx*screeny*sizeof(_color));
	_color col;
	
	if(image==NULL){
		printf("Output error!\n");
	}
	
	xmin=-0.889550;
	xmax=2.504761;
	ymin=-0.337961;
	ymax=1.812964;
	
	//~ ax[0]= 1.300000;	ay[0]= 1.200000;
	//~ ax[1]=-0.900000;	ay[1]= 0.000000;
	//~ ax[2]= 0.000000;	ay[2]= 0.500000;
	//~ ax[3]=-0.300000;	ay[3]=-1.200000;
	//~ ax[4]= 0.800000;	ay[4]=-1.100000;
	//~ ax[5]= 0.000000;	ay[5]= 0.800000;
	
	ax[0]= 1.300000;	ay[0]= 1.175000;
	ax[1]=-0.910000;	ay[1]= 0.005000;
	ax[2]= 0.012000;	ay[2]= 0.500000;
	ax[3]=-0.315000;	ay[3]=-1.200000;
	ax[4]= 0.810000;	ay[4]=-1.100000;
	ax[5]= 0.020000;	ay[5]= 0.800000;
	
	printf("Boundaries are:\n");
	printf("xmin=%f\txmax=%f\nymin=%f\tymax=%f\n",xmin,xmax,ymin,ymax);
	printf("Parameter are:\n");
	
	for (i=0;i<6;i++){
		printf("%f %f\n",ax[i],ay[i]);
	}
	
	printf("Starting processing\n");
	
	for (i=0;i<screenx*screeny;i++) {
		image[i].r=0;
		image[i].g=0;
		image[i].b=0;
	}
	
	sprintf(fname,"quad.ppm");
	
	fptr = fopen(fname,"wt");
	
	for(i=0;i<iters;i++){
		/* Calculate next term */
		x[i]=	ax[0]+ ax[1]  *x[i-1] + ax[2]*x[i-1] *  x[i-1] + 
				ax[3] * x[i-1]*y[i-1] + ax[4]*y[i-1] + ax[5]   * y[i-1] * y[i-1];
		y[i]=	ay[0]+ ay[1]  *x[i-1] + ay[2]*x[i-1] *  x[i-1] + 
				ay[3] * x[i-1]*y[i-1] + ay[4]*y[i-1] + ay[5]   * y[i-1] * y[i-1];
		xmin = MIN(xmin,x[i]);
		ymin = MIN(ymin,y[i]);
		xmax = MAX(xmax,x[i]);
		ymax = MAX(ymax,y[i]);
	}
	
	for (i=0;i<iters;i++) {
		ix = (screenx*0.9) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
		iy = (screeny*0.9) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);
		if (i > 100){
			image[iy*screenx+ix].r+=sin(ix*M_PI/360)+2.0;
			image[iy*screenx+ix].g+=cos(iy*M_PI/270)+2.0;
			image[iy*screenx+ix].b+=sin(iy*M_PI/360)+2.0;
		}
	}
	
	fprintf(fptr,"P3\n%d %d\n255\n",screenx,screeny);
	
	for(i=0;i<screeny;i++){
		for(j=0;j<screenx;j++){
			col=image[i*screenx+j];
			col.r=((1.0-exp(-sens*col.r))*255.0);
			col.g=((1.0-exp(-sens*col.g))*255.0);
			col.b=((1.0-exp(-sens*col.b))*255.0);
			fprintf(fptr,"%d %d %d ",(int)col.r,(int)col.g,(int)col.b);
		}
		fputc('\n',fptr);
	}
	fclose(fptr);
	printf("Done!\n");

	return EXIT_SUCCESS;
}
