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

int main(){
	int 	i,j,n,ix,iy,
			iters,images,
			screenx,screeny,
			mode,
			drawit;
	long	secs;
	double	xmin=1e32,xmax=-1e32,ymin=1e32,ymax=-1e32,zmin=1e32,zmax=-1e32,
			ax[12],ay[12],az[12],
			sens,
			d0,dd,dx,dy,dz,lyapunov,
			xe,ye,ze,xenew,yenew,zenew;
	char	fname[256];
	FILE	*fptr;
	
	screenx=1600/2;
	screeny=1200/2;
	
	screenx=1024;
	screeny=768;
	
	iters=50000;
	sens=0.0025/0.85;
	
	mode=3;
	
	double *x	= malloc(iters*sizeof(double));
	double *y	= malloc(iters*sizeof(double));
	double *z	= malloc(iters*sizeof(double));
	_color *image=malloc(screenx*screeny*sizeof(_color));
	_color col;
	
	time(&secs);
	srand48(secs);
	srand(secs);
	
	for (n=0;n<images;n++) {
		/* Initialise things for this attractor */
		for (i=0;i<6;i++) {
			//~ ax[i] = 4 * (drand48() - 0.5); // -2.0 to 2.0 double precision
			//~ ay[i] = 4 * (drand48() - 0.5);
			//~ az[i] = 4 * (drand48() - 0.5);
			
			ax[i] = 20 * (drand48())-4; // 0.0 to 16.0 double precision
			ay[i] = 20 * (drand48())-4;
			az[i] = 20 * (drand48())-4;
			
			//~ ax[i]=((rand()%26+1)-13)/10.0; // -1.2 to 1.2 10e-1 precision
			//~ ay[i]=((rand()%26+1)-13)/10.0; // -1.2 to 1.2 10e-1 precision
			//~ az[i]=((rand()%26+1)-13)/10.0; // -1.2 to 1.2 10e-1 precision
		}
		
		lyapunov = 0;
		xmin =  1e32;
		xmax = -1e32;
		ymin =  1e32;
		ymax = -1e32;
		zmin =  1e32;
		zmax = -1e32;
		
		drawit = TRUE;
		x[0] = drand48() - 0.5;
		y[0] = drand48() - 0.5;
		z[0] = drand48() - 0.5;
		do {
			xe = x[0] + (drand48() - 0.5) / 1000.0;
			ye = y[0] + (drand48() - 0.5) / 1000.0;
			
			dx = x[0] - xe;
			dy = y[0] - ye;
			
			if(mode==1||mode==2){
				d0 = sqrt(dx*dx + dy*dy + dz*dz);
			}else if(mode==3||mode==4||mode==5){
				ze = y[0] + (drand48() - 0.5) / 1000.0;
				dz = z[0] - ze;
				d0 = cbrt(dx*dx + dy*dy + dz*dz);
			}
		} while (d0 <= 0);
		
		for(i=1;i<iters;i++){
			if(mode==1){  // Simple polynomial form
				x[i]=	ax[0] + ax[1]  * x[i-1] + ax[2]*x[i-1] * x [i-1] + 
						ax[3] * x [i-1]* y[i-1] + ax[4]*y[i-1] + ax[5]  * y[i-1]*y[i-1];
				y[i]=	ay[0] + ay[1]  * x[i-1] + ay[2]*x[i-1] * x [i-1] + 
						ay[3] * x [i-1]* y[i-1] + ay[4]*y[i-1] + ay[5]  * y[i-1]*y[i-1];
				xenew=	ax[0] + ax[1]*xe + ax[2]*xe*xe +
						ax[3]*xe*ye + ax[4]*ye + ax[5]*ye*ye;
				yenew=	ay[0] + ay[1]*xe + ay[2]*xe*xe +
						ay[3]*xe*ye + ay[4]*ye + ay[5]*ye*ye;
			}else if(mode==2){  // Abs
				//~  // 3D version
				//~ x[i]=	ax[0] + ax[1]*x[i-1] + ax[2]*y[i-1] + ax[3]*z[i-1] + ax[4]*fabs(x[i-1]) + ax[5]*fabs(y[i-1]) + ax[6]*fabs(z[i-1]);
				//~ y[i]=	ay[0] + ay[1]*x[i-1] + ay[2]*y[i-1] + ay[3]*z[i-1] + ay[4]*fabs(x[i-1]) + ay[5]*fabs(y[i-1]) + ay[6]*fabs(z[i-1]);
				//~ z[i]=	az[0] + az[1]*x[i-1] + az[2]*y[i-1] + az[3]*z[i-1] + az[4]*fabs(x[i-1]) + az[5]*fabs(y[i-1]) + az[6]*fabs(z[i-1]);
				//~ 
				//~ xenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + ax[6]*fabs(ze);
				//~ yenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + ax[6]*fabs(ze);
				//~ zenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + ax[6]*fabs(ze);
				
				 // 2D
				x[i]=	ax[0] + ax[1]*x[i-1] + ay[2]*y[i-1] + ax[3]*abs(x[i-1]) + ax[4]*abs(y[i-1]);
				y[i]=	ay[0] + ay[1]*x[i-1] + ay[2]*y[i-1] + ay[3]*abs(x[i-1]) + ay[4]*abs(y[i-1]);
				
				xenew=	ax[0] + ax[1]*xe + ay[2]*ye + ax[3]*abs(xe) + ax[4]*abs(ye);
				yenew=	ay[0] + ay[1]*xe + ay[2]*ye + ay[3]*abs(xe) + ay[4]*abs(ye);
			}else if(mode==3){  // Power
				x[i]=	ax[0] + ax[1]*x[i-1] + ax[2]*y[i-1] + ax[3]*z[i-1] + ax[4]*fabs(x[i-1]) + ax[5]*fabs(y[i-1]) + pow(ax[6]*fabs(z[i-1]),ax[7]);
				y[i]=	ay[0] + ay[1]*x[i-1] + ay[2]*y[i-1] + ay[3]*z[i-1] + ay[4]*fabs(x[i-1]) + ay[5]*fabs(y[i-1]) + pow(ay[6]*fabs(z[i-1]),ay[7]);
				z[i]=	az[0] + az[1]*x[i-1] + az[2]*y[i-1] + az[3]*z[i-1] + az[4]*fabs(x[i-1]) + az[5]*fabs(y[i-1]) + pow(az[6]*fabs(z[i-1]),az[7]);
				
				xenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + pow(ax[6]*fabs(ze),ax[7]);
				yenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + pow(ax[6]*fabs(ze),ay[7]);
				zenew=	ax[0] + ax[1]*xe + ax[2]*ye + ax[3]*ze + ax[4]*fabs(xe) + ax[5]*fabs(ye) + pow(ax[6]*fabs(ze),az[7]);
			}else if(mode==4){
				x[i]=ax[0]+y[i-1]-z[i-1]*y[i-1];
				y[i]=ay[0]+z[i-1]-z[i-1]*x[i-1];
				z[i]=az[0]+x[i-1]-x[i-1]*y[i-1];
				
				xenew=ax[0]+ye-ze*ye;
				yenew=ay[0]+ze-ze*xe;
				zenew=az[0]+xe-xe*ye;
			}else if(mode==5){
				x[i]=x[i-1]+ax[1]*ax[0]*(y[i-1]-x[i-1]);
				y[i]=y[i-1]+ax[1]*(ay[0]*x[i-1]-y[i-1]-z[i-1]*x[i-1]);
				z[i]=z[i-1]+ax[1]*(x[i-1]*y[i-1]-az[0]*z[i-1]);
				
				xenew=xe+ax[1]*ax[0]*(ye-xe);
				yenew=ye+ax[1]*(ay[0]*xe-ye-ze*xe);
				zenew=ze+ax[1]*(xe*ye-az[0]*ze);
			}
			
			if(mode==1 || mode==2){
				/* Update the bounds */
				xmin = MIN(xmin,x[i]);
				ymin = MIN(ymin,y[i]);
				xmax = MAX(xmax,x[i]);
				ymax = MAX(ymax,y[i]);
				
				/* Does the series tend to infinity? */
				if (xmin < -1e10 || ymin < -1e10 || xmax > 1e10 || ymax > 1e10) {
					drawit = FALSE;
					//~ printf("infinite attractor ");
					break;
				}
				
				/* Does the series tend to a point? */
				dx = x[i] - x[i-1];
				dy = y[i] - y[i-1];
				
				if (abs(dx) < 1e-10 && abs(dy) < 1e-10) {
					drawit = FALSE;
					//~ printf("point attractor ");
					break;
				}
				
				/* Calculate the lyap */
				if (i > 1000) {
					dx = x[i] - xenew;
					dy = y[i] - yenew;
					dd = sqrt(dx * dx + dy * dy);
					lyapunov += log(fabs(dd / d0));
					xe = x[i] + d0 * dx / dd;
					ye = y[i] + d0 * dy / dd;
				}
			}else if(mode==3 || mode==4 || mode==5){
				/* Update the bounds */
				xmin = MIN(xmin,x[i]);
				ymin = MIN(ymin,y[i]);
				zmin = MIN(zmin,z[i]);
				xmax = MAX(xmax,x[i]);
				ymax = MAX(ymax,y[i]);
				zmax = MAX(zmax,z[i]);
				
				/* Does the series tend to infinity? */
				if (xmin < -1e10 || ymin < -1e10 || zmin < -1e10 ||xmax > 1e10 || ymax > 1e10 || zmax > 1e10) {
					drawit = FALSE;
					//~ printf("infinite attractor ");
					break;
				}
				
				/* Does the series tend to a point? */
				dx = x[i] - x[i-1];
				dy = y[i] - y[i-1];
				dz = z[i] - z[i-1];
				
				if (abs(dx) < 1e-10 && abs(dy) < 1e-10 && abs(dz) < 1e-10) {
					drawit = FALSE;
					//~ printf("point attractor ");
					break;
				}
				
				/* Calculate the lyap */
				if (i > 1000) {
					dx = x[i] - xenew;
					dy = y[i] - yenew;
					dz = z[i] - zenew;
					dd = sqrt(dx * dx + dy * dy +dz*dz);
					//~ dd = cbrt(dx*dx + dy*dy + dz*dz);
					lyapunov += log(fabs(dd / d0));
					xe = x[i]+d0*dx/dd;
					ye = y[i]+d0*dy/dd;
					ze = z[i]+d0*dz/dd;
				}
			}
		}
		
		/* Classify the series according to lyapunov */
		if (drawit) {
			if ((lyapunov) < 10) {
				//~ printf("neutrally stable ");
				drawit = FALSE;
			} else if (lyapunov < 0) {
				//~ printf("periodic %g ",lyapunov);
				drawit = FALSE; 
			} else {
				printf("img %d# is chaotic %g \n",n,lyapunov); 
				
				sprintf(fname,"%05d.txt",n); 
				fptr = fopen(fname,"wb");
				
				//~ fprintf(fptr,"%g %g %g %g\n",xmin,ymin,xmax,ymax);
				fwrite(&xmin,sizeof(double),1,fptr);
				fwrite(&ymin,sizeof(double),1,fptr);
				fwrite(&xmax,sizeof(double),1,fptr);
				fwrite(&ymax,sizeof(double),1,fptr);
				
				for (i=0;i<6;i++){
					//~ fprintf(fptr,"%g %g\n",ax[i],ay[i]);
					fwrite(&ax[i],sizeof(double),1,fptr);
					fwrite(&ay[i],sizeof(double),1,fptr);
				}
				fclose(fptr);
				
				for (i=0;i<screenx*screeny;i++) {
					image[i].r=0;
					image[i].g=0;
					image[i].b=0;
				}
				
				sprintf(fname,"%05d.ppm",n);
				fptr = fopen(fname,"wt");
				
				for (i=0;i<iters;i++) {
					ix = (screenx*0.90) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
					iy = (screeny*0.90) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);
					if (i > 100){
						//~ image[iy*screenx+ix].r+=1; // Density plot
						//~ image[iy*screenx+ix].g+=1;
						//~ image[iy*screenx+ix].b+=1;
						
						image[iy*screenx+ix].r+=sin(ix*M_PI/360)+2.0; // Kind of a density plot
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
			}
		}
	}
	return EXIT_SUCCESS;
}
