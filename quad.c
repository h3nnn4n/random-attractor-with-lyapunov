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
    int           i,j,n,ix,iy,
                  iters,
                  screenx,screeny,
                  drawit;
    unsigned long images,secs;
    double        xmin=1e32,xmax=-1e32,ymin=1e32,ymax=-1e32,zmin=1e32,zmax=-1e32,
                  ax[12],ay[12],
                  sens,
                  d0,dd,dx,dy,lyapunov,
                  xe,ye,ze,xenew,yenew,zenew;
    char          fname[256],tmp[256];
    FILE          *fptr;

    screenx = 1920/1; // Resolution
    screeny = 1080/1;

    iters   = 10000000;  // Maximum number of iterations in EACH image, If the attractors goes to infinity or to zero a new set will be started.
    images  = 1000000;   // Number of Images to be TESTED. THis inst how many will be generated, only how many will be tested.
    sens    = 0.005/1.0; // Here the brightness is defined. The bigger the number of iterations the small this value should be.

    double *x     = (double*) malloc(sizeof(double) * iters            );
    double *y     = (double*) malloc(sizeof(double) * iters            );
    double *z     = (double*) malloc(sizeof(double) * iters            );
    _color *image = (_color*) malloc(sizeof(_color) * screenx * screeny);
    _color col;

    srand48(time(&secs));
    srand  (time(&secs));

    for (n = 0 ; n<images ; n++) {
        //fprintf(stderr, ".");
        /* Initialize stuff */
        for (i=0 ; i<6 ; i++) {					// Here the parameters are randomly initialized
            //ax[i] = 4 * (drand48() - 0.5);	// -2.0 to 2.0 double precision
            //ay[i] = 4 * (drand48() - 0.5);

            //ax[i] = 20 * (drand48())-4; 	// 0.0 to 16.0 double precision
            //ay[i] = 20 * (drand48())-4;

            ax[i] = ((rand()%26 + 1) - 13) / 10.0;	// -1.2 to 1.2 10e-1 precision
            ay[i] = ((rand()%26 + 1) - 13) / 10.0;
        }

        lyapunov = 0;
        xmin =  1e32;
        xmax = -1e32;
        ymin =  1e32;
        ymax = -1e32;

        drawit = TRUE;
        x[0] = drand48() - 0.5; 	// This defines the initial values which are random
        y[0] = drand48() - 0.5;

        do {	// This checks if the initial values are suitable
            xe = x[0] + (drand48() - 0.5) / 1000.0;
            ye = y[0] + (drand48() - 0.5) / 1000.0;

            dx = x[0] - xe;
            dy = y[0] - ye;

            d0 = sqrt(dx*dx + dy*dy);
        } while (d0 <= 0);

        //printf("\nax = ");
        //for (i=0 ; i<6 ; i++) {
            //printf("%.4f ", ax[i]);
        //}

        //printf("\nay = ");
        //for (i=0 ; i<6 ; i++) {
            //printf("%.4f ", ay[i]);
        //}
        //puts("\n");

        for(i = 1 ; i < iters ; i++){
            // The set of equations from http://mathworld.wolfram.com/StrangeAttractor.html
            x[i]   = ax[0] + ax[1]*x[i-1] + ax[2]*x[i-1]*x[i-1] + ax[3]*x[i-1]*y[i-1] + ax[4]*y[i-1] + ax[5]*y[i-1]*y[i-1];
            y[i]   = ay[0] + ay[1]*x[i-1] + ay[2]*x[i-1]*x[i-1] + ay[3]*x[i-1]*y[i-1] + ay[4]*y[i-1] + ay[5]*y[i-1]*y[i-1];

            xenew  = ax[0] + ax[1]*xe     + ax[2]*xe*xe         + ax[3]*xe*ye         + ax[4]*ye     + ax[5]*ye*ye;
            yenew  = ay[0] + ay[1]*xe     + ay[2]*xe*xe         + ay[3]*xe*ye         + ay[4]*ye     + ay[5]*ye*ye;

#ifdef __debug
            printf("---------\n");
            printf("%f %f\n", x[i], y[i]);
            printf("%f %f\n", xenew, yenew);
#endif

            /* Update the bounds */
            xmin = MIN(xmin, x[i]);
            ymin = MIN(ymin, y[i]);
            xmax = MAX(xmax, x[i]);
            ymax = MAX(ymax, y[i]);

            /* Check if the series tend to infinity */
            if (xmin < -1e7 || ymin < -1e7 || xmax > 1e7 || ymax > 1e7 || isnan(x[i]) || isnan(y[i] || isnan(xenew) || isnan(yenew))) {
                drawit = FALSE;
                //printf("infinite attractor\n");
                break;
            }

            /* Checks if the series tend to a point */
            dx = x[i] - x[i-1];
            dy = y[i] - y[i-1];

            if (abs(dx) < 1e-6 && abs(dy) < 1e-6) {
                drawit = FALSE;
                //printf("point attractor\n");
                break;
            }

            /* Calculate the lyap */
            if (i > 1000) {
                dx        = x[i] - xenew;
                dy        = y[i] - yenew;
                dd        = sqrt(dx * dx + dy * dy);
                lyapunov += log(fabs(dd / d0));
                xe        = x[i] + d0 * dx / dd;
                ye        = y[i] + d0 * dy / dd;

#ifdef __debug
                printf("lyap = %f \t dx, dy = %f %f \t dd = %f  \n", lyapunov, dx, dy, dd);
#endif
            }
        }

        /* Classify the series according to lyapunov */
        if (drawit) {
            if ((lyapunov) < 10) {
                printf("neutrally stable %f \n", lyapunov);
                drawit = FALSE;
            } else if (lyapunov < 0) {
                printf("periodic %f \n", lyapunov);
                drawit = FALSE;
            } else {
                printf("img %d# is chaotic %f \n",n,lyapunov);

                sprintf(fname,"%05d.txt",n);
                fptr = fopen(fname,"wb");

                fprintf(fptr,"%g %g %g %g\n",xmin,ymin,xmax,ymax);
                fwrite(&xmin,sizeof(double),1,fptr);
                fwrite(&ymin,sizeof(double),1,fptr);
                fwrite(&xmax,sizeof(double),1,fptr);
                fwrite(&ymax,sizeof(double),1,fptr);

                for (i=0;i<6;i++){
                    fprintf(fptr,"%g %g\n",ax[i],ay[i]);
                    fwrite(&ax[i],sizeof(double),1,fptr);
                    fwrite(&ay[i],sizeof(double),1,fptr);
                }
                fclose(fptr);

                for (i=0;i<screenx*screeny;i++) {
                    image[i].r=0;
                    image[i].g=0;
                    image[i].b=0;
                }

                sprintf(fname,"%05d.ppm",n); // Here the attractor is draw
                fptr = fopen(fname,"wt");

                for (i=0;i<iters;i++) {
                    ix = (screenx*0.90) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
                    iy = (screeny*0.90) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);
                    if (i > 100){
                        //~ image[iy*screenx+ix].r+=1; // Density plot
                        //~ image[iy*screenx+ix].g+=1;
                        //~ image[iy*screenx+ix].b+=1;

                        image[iy*screenx+ix].r+=sin(ix*M_PI/360)+2.0; // Kind of a density plot // This is something about colors
                        image[iy*screenx+ix].g+=cos(iy*M_PI/270)+2.0;
                        image[iy*screenx+ix].b+=sin(iy*M_PI/360)+2.0;
                    }
                }

                fprintf(fptr,"P3\n%d %d\n255\n",screenx,screeny);

                for(i = 0;i<screeny;i++){
                    for(j = 0;j<screenx;j++){
                        col   = image[i*screenx+j];
                        col.r = ((1.0 - exp(-sens * col.r)) * 255.0);
                        col.g = ((1.0 - exp(-sens * col.g)) * 255.0);
                        col.b = ((1.0 - exp(-sens * col.b)) * 255.0);
                        col   = invert_color(col);
                        fprintf(fptr,"%d %d %d ",(int)col.r,(int)col.g,(int)col.b);
                    }
                    fputc('\n',fptr);
                }
                fclose(fptr);

                sprintf(tmp,"convert %s w_%05d.png",fname,n);

                system(tmp);

                sprintf(tmp,"rm %s",fname);

                system(tmp);
            }
        }
    }
    return EXIT_SUCCESS;
}
