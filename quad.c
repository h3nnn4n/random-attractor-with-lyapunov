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
    if ( argc <= 3 ) { printf("I need at least 3 files to work with\n"); return EXIT_FAILURE; }

    int         i, j, k, iframes, n, ix, iy,
                iters, images, frames,
                screenx, screeny,
                drawit;
    long        secs;
    double      xmin=1e32, xmax=-1e32, ymin=1e32, ymax=-1e32,
                ax [NPARAMS], ay [NPARAMS],
                ax1[NPARAMS], ay1[NPARAMS],
                ax2[NPARAMS], ay2[NPARAMS],
                sens,p,
                t1, t2,
                d0, dd, dx, dy, lyapunov,
                xe, ye, xenew, yenew;
    char        fname[256], tmp[256];
    FILE        *fptr;

    srand48(time(&secs));
    srand  (time(&secs));

    screenx = 1920/1;
    screeny = 1080/1;
    iters   = 1000000;
    images  = 500;
    sens    = 0.001/2.0;

    double *x     = malloc ( iters * sizeof(double));
    double *y     = malloc ( iters * sizeof(double));
    _color *image = malloc ( screenx * screeny * sizeof(_color));
    _color *pal   = malloc ( sizeof(_color) * 255) ;
    _color col;

    populatePal(pal);

    if(image==NULL){
        printf("Output error!\n");
    }

    xmin =  1000.0;
    ymin =  1000.0;
    xmax = -1000.0;
    ymax = -1000.0;

    for ( iframes = 0 ; iframes < argc-1 ; iframes++) {
        fptr = fopen(argv[iframes+1], "rb");
        double xmin2, ymin2, xmax2, ymax2;
        fread(&xmin2, sizeof( double ), 1, fptr);
        fread(&ymin2, sizeof( double ), 1, fptr);
        fread(&xmax2, sizeof( double ), 1, fptr);
        fread(&ymax2, sizeof( double ), 1, fptr);

        xmin = MIN(xmin, xmin2);
        ymin = MIN(ymin, ymin2);
        xmax = MAX(xmax, xmax2);
        ymax = MAX(ymax, ymax2);

        fclose(fptr);
    }

    time_t timer = clock();

    for ( iframes = 0 ; iframes < argc-1 ; iframes++) {
        int first   =  iframes + 1;
        int second  = (iframes + 2) % argc;
        second = second == 0 ? second + 1 : second;

        double dump;

        fptr = fopen(argv[first], "rb");

        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);

        for ( i = 0 ; i < NPARAMS ; i++ ){
            fread(&ax1[i], sizeof( double ), 1, fptr);
            fread(&ay1[i], sizeof( double ), 1, fptr);
        }

        fclose(fptr);

        fptr = fopen(argv[second], "rb");

        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);
        fread(&dump, sizeof( double ), 1, fptr);

        for ( i = 0 ; i < NPARAMS ; i++ ){
            fread(&ax2[i], sizeof( double ), 1, fptr);
            fread(&ay2[i], sizeof( double ), 1, fptr);
        }

        fclose(fptr);

        for ( k = 0 ; k < images ; k++ ) {
            p = (double)(k + iframes * images) / (images * (argc-1));

            for ( i = 0 ; i < NPARAMS ; i++ ){
                ax[i] = ax1[i] + ((double)k/images) * (ax2[i] - ax1[i]);
                ay[i] = ay1[i] + ((double)k/images) * (ay2[i] - ay1[i]);
            }

            for(i = 0;i<iters;i++){
                __x_equations
                __y_equations

                xmin = MIN(xmin, x[i]);
                ymin = MIN(ymin, y[i]);
                xmax = MAX(xmax, x[i]);
                ymax = MAX(ymax, y[i]);
            }

            _color col = getPalMem(p, pal);
            /*_color col = getPal(p);*/

            for (i = 0;i<iters;i++) {
                ix = (screenx*0.9) * (x[i] - xmin) / (xmax - xmin)+(screenx*0.05);
                iy = (screeny*0.9) * (y[i] - ymin) / (ymax - ymin)+(screeny*0.05);
                if (i > 100){
                    image[iy*screenx+ix].r += col.r;
                    image[iy*screenx+ix].g += col.g;
                    image[iy*screenx+ix].b += col.b;
                }
            }

            printf("\r%.3f%% Time: %f   \t ETA: %f", ((double)(iframes * images + (k+1))/((argc-1)*images))*100.0,
                                                     (((double)(clock()-timer)/CLOCKS_PER_SEC)),
                                                     (((((double)(clock()-timer)/CLOCKS_PER_SEC)) * (images*(argc-1)) ) / (iframes * images + (k+1))) - (((double)(clock()-timer)/CLOCKS_PER_SEC))
                  );
            fflush(stdout);
        }
    }

    sprintf(fname,"supreme.png");

    for(i = 0 ; i<screeny ; i++){
        for(j = 0 ; j<screenx ; j++){
            col                = image[i*screenx+j];

            col.r              = ((1.0 - exp(-sens * col.r)) * 255.0);
            col.g              = ((1.0 - exp(-sens * col.g)) * 255.0);
            col.b              = ((1.0 - exp(-sens * col.b)) * 255.0);

            col                = invert_color(col);

            image[i*screenx+j] = col;
        }
    }

    save_png_to_file(image, screenx, screeny, fname);

    printf("\nDone!\n");

    return EXIT_SUCCESS;
}
