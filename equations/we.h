#ifndef   __equations
#define   __equations

#define NPARAMS 13

#define   __x_equations              x[i] = ax[1]*x[i-1]*x[i-1] + ax[3]*y[i-1]*y[i-1] + ax[7] + ax[2]*cos(x[i-1]*ax[4]) + ax[8]*sin(y[i-1]*ax[9]);
#define   __y_equations              y[i] = ay[1]*x[i-1]*x[i-1] + ay[3]*y[i-1]*y[i-1] + ay[7] + ay[2]*sin(x[i-1]*ay[4]) + ay[8]*cos(y[i-1]*ay[9]);

#define   __x_equations_lyaps       xenew = ax[1]*xe*xe         + ax[3]*ye*ye         + ax[7] + ax[2]*cos(xe*ax[4])     + ax[8]*sin(ye*ax[9])    ;
#define   __y_equations_lyaps       yenew = ay[1]*xe*xe         + ay[3]*ye*ye         + ay[7] + ay[2]*sin(xe*ay[4])     + ay[8]*cos(ye*ay[9])    ;

#endif  //__equations
