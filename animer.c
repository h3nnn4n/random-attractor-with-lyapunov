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
    if ( argc <= 3 ) { printf("I need at least 3 files to work with\n"); return EXIT_FAILURE; }

    int 	i, j, k, iframes, n, ix, iy,
                iters, images, frames,
                screenx, screeny,
                drawit;
    long	secs;
    double	xmin=1e32, xmax=-1e32, ymin=1e32, ymax=-1e32,
                ax [6], ay [6],
                ax1[6], ay1[6],
                ax2[6], ay2[6],
                sens,
                t1, t2,
                d0, dd, dx, dy, lyapunov,
                xe, ye, xenew, yenew;
    char        fname[256], tmp[256];
    FILE	*fptr;

    srand48(time(&secs));
    srand  (time(&secs));

    screenx = 1920/1; // Resolution
    screeny = 1080/1;
    iters   = 10000000;  // Maximum number of iterations in EACH image, If the attractors goes to infinity or to zero a new set will be started.
    images  = 25;
    sens    = 0.005/1.0; // Here the brightness is defined. The bigger the number of iterations the small this value should be.

    double *x     = malloc(iters*sizeof(double));
    double *y     = malloc(iters*sizeof(double));
    _color *image = malloc(screenx*screeny*sizeof(_color));
    _color col;

    if(image==NULL){
        printf("Output error!\n");
    }

    xmin =  1000.0;
    ymin =  1000.0;
    xmax = -1000.0;
    ymax = -1000.0;

    for ( iframes = 0 ; iframes < argc-1 ; iframes++) {
        fptr = fopen(argv[iframes+1], "rb");
        double xmin2, ymin2, xmax2, ymax2; fread(&xmin2, sizeof( double ), 1, fptr); fread(&ymin2, sizeof( double ), 1, fptr);
        fread(&xmax2, sizeof( double ), 1, fptr);
        fread(&ymax2, sizeof( double ), 1, fptr);

        xmin = MIN(xmin, xmin2);
        ymin = MIN(ymin, ymin2);
        xmax = MAX(xmax, xmax2);
        ymax = MAX(ymax, ymax2);

        fclose(fptr);
    }

    for ( iframes = 0 ; iframes < argc-1 ; iframes++) {
        int first   =  iframes + 1;
        int second  = (iframes + 2) % argc;
        second = second == 0 ? second + 1 : second;

        /*printf("%d %d\n", first, second);*/

        double dump;

        fptr = fopen(argv[first], "rb");

        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);

        for ( i = 0 ; i < 6 ; i++ ){
            fread(&ax1[i], sizeof( double ), 1, fptr);
            fread(&ay1[i], sizeof( double ), 1, fptr);
        }

        fclose(fptr);

        fptr = fopen(argv[second], "rb");

        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);

        for ( i = 0 ; i < 6 ; i++ ){
            fread(&ax2[i], sizeof( double ), 1, fptr);
            fread(&ay2[i], sizeof( double ), 1, fptr);
        }

        fclose(fptr);

        for ( k = 0 ; k < images ; k++ ) {
            memset( image, 0, screenx*screeny*sizeof(_color));

            /*puts("----------");*/
            for ( i = 0 ; i < 6 ; i++ ){
                ax[i] = ax1[i] + ((double)k/images) * (ax2[i] - ax1[i]);
                ay[i] = ay1[i] + ((double)k/images) * (ay2[i] - ay1[i]);
                /*printf("%f \t%f\n", ax[i], ay[i]);*/
            }

            for (i=0;i<screenx*screeny;i++) {
                image[i].r = 0;
                image[i].g = 0;
                image[i].b = 0;
            }

            for(i = 0;i<iters;i++){
                /* Calculate next term */
                x[i]   = ax[0] + ax[1]*x[i-1] + ax[2]*x[i-1]*x[i-1] + ax[3]*x[i-1]*y[i-1] + ax[4]*y[i-1] + ax[5]*y[i-1]*y[i-1];
                y[i]   = ay[0] + ay[1]*x[i-1] + ay[2]*x[i-1]*x[i-1] + ay[3]*x[i-1]*y[i-1] + ay[4]*y[i-1] + ay[5]*y[i-1]*y[i-1];

                xmin = MIN(xmin, x[i]);
                ymin = MIN(ymin, y[i]);
                xmax = MAX(xmax, x[i]);
                ymax = MAX(ymax, y[i]);
            }

            for (i = 0;i<iters;i++) {
                ix = (screenx*0.9) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
                iy = (screeny*0.9) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);
                if (i > 100){
                    image[iy*screenx+ix].r += sin(ix*M_PI/360)+2.0;
                    image[iy*screenx+ix].g += cos(iy*M_PI/270)+2.0;
                    image[iy*screenx+ix].b += sin(iy*M_PI/360)+2.0;
                }
            }

            sprintf(fname,"anim_%05d.ppm", iframes * images + k);
            fptr = fopen(fname,"wt");
            fprintf(fptr, "P3\n%d %d\n255\n", screenx, screeny);

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

            sprintf(tmp, "convert %s animer_%05d.png", fname,  iframes * images + k);

            system(tmp);

            sprintf(tmp,"rm %s",fname);

            system(tmp);

            printf("\r%.3f%%", ((double)(iframes * images + k)/((argc-1)*images))*100.0);
            fflush(stdout);
        }
    }

    printf("Done!\n");

    return EXIT_SUCCESS;
}
