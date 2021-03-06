#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "color.h"

#include "equations.h"

#define TRUE  1
#define FALSE 0
#define MAX(x,y) (x > y ? x : y)
#define MIN(x,y) (x < y ? x : y)

int main(int argc,char *argv[]){
    if ( argc != 2 ) { return EXIT_FAILURE; }

    int 	i,j,k,n,ix,iy,
                iters,images,
                screenx,screeny,
                drawit;
    long	secs;
    double	xmin=1e32,xmax=-1e32,ymin=1e32,ymax=-1e32,
                ax[NPARAMS],ay[NPARAMS],
                sens,
                t1,t2,
                d0,dd,dx,dy,lyapunov,
                xe,ye,xenew,yenew;
    char        fname[256],tmp[256];
    FILE	*fptr;

    srand48(time(&secs));
    srand  (time(&secs));

    screenx = 1920/1;
    screeny = 1080/1;
    iters   = 10000000;
    images  = 1000;
    sens    = 0.005/1.0;

    double *x     = malloc(iters*sizeof(double));
    double *y     = malloc(iters*sizeof(double));
    _color *image = malloc(screenx*screeny*sizeof(_color));
    _color col;

    if(image==NULL){
        printf("Output error!\n");
    }

    for ( k = 0 ; k < images ; k++ ) {
        memset( image, 0, screenx*screeny*sizeof(_color));

        fptr = fopen(argv[1], "rb");

        x[0] = drand48() - 0.5;
        y[0] = drand48() - 0.5;

        fread(&xmin, sizeof( double ), 1, fptr);
        fread(&ymin, sizeof( double ), 1, fptr);
        fread(&xmax, sizeof( double ), 1, fptr);
        fread(&ymax, sizeof( double ), 1, fptr);

        for ( i = 0 ; i < NPARAMS ; i++ ){
            fread(&ax[i], sizeof( double ), 1, fptr);
            fread(&ay[i], sizeof( double ), 1, fptr);
        }

        fclose(fptr);

        printf("Boundaries are:\n");
        printf("xmin=%f \txmax=%f \nymin=%f \tymax=%f\n", xmin, xmax, ymin, ymax);
        printf("Parameter are:\n");

        for (i=0;i<NPARAMS;i++){
            ax[i] += (drand48() - 0.5) / 10.0;
            ay[i] += (drand48() - 0.5) / 10.0;

            printf("%f \t %f\n",ax[i],ay[i]);
        }

        printf("Starting processing\n");

        for (i=0;i<screenx*screeny;i++) {
            image[i].r=0;
            image[i].g=0;
            image[i].b=0;
        }


        for(i = 1;i<iters;i++){
            __x_equations
            __y_equations

            xmin = MIN(xmin, x[i]);
            ymin = MIN(ymin, y[i]);
            xmax = MAX(xmax, x[i]);
            ymax = MAX(ymax, y[i]);
        }

        for (i = 0;i<iters;i++) {
            ix = (screenx*0.9) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
            iy = (screeny*0.9) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);

            if ( ix < 0 || ix > screenx || iy < 0 || iy > screeny ){
                continue;
            }

            if (i > 100){
                image[iy*screenx+ix].r += sin(ix*M_PI/360)+2.0;
                image[iy*screenx+ix].g += cos(iy*M_PI/270)+2.0;
                image[iy*screenx+ix].b += sin(iy*M_PI/360)+2.0;
            }
        }

        sprintf(fname,"quad_%d.png", k);

        for(i = 0;i<screeny;i++){
            for(j = 0;j<screenx;j++){
                col   = image[i*screenx+j];

                col.r = ((1.0 - exp(-sens * col.r)) * 255.0);
                col.g = ((1.0 - exp(-sens * col.g)) * 255.0);
                col.b = ((1.0 - exp(-sens * col.b)) * 255.0);

                col   = invert_color(col);

                image[i*screenx+j] = col;
            }
        }

        save_png_to_file(image, screenx, screeny, fname);

        sprintf(fname,"quad_%d.txt", k);

        fptr = fopen(fname,"wb");

        fwrite(&xmin,sizeof(double),1,fptr);
        fwrite(&ymin,sizeof(double),1,fptr);
        fwrite(&xmax,sizeof(double),1,fptr);
        fwrite(&ymax,sizeof(double),1,fptr);

        for (i=0;i<NPARAMS;i++){
            fwrite(&ax[i],sizeof(double),1,fptr);
            fwrite(&ay[i],sizeof(double),1,fptr);
        }
        fclose(fptr);

        printf("Done!\n");
    }

    return EXIT_SUCCESS;
}
