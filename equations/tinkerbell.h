#ifndef   __equations
#define   __equations

#define NPARAMS 3


#define   __x_equations                 x[i] =     x[i-1]*x[i-1] -  y[i-1]*y[i-1] + ax[0]*x[i-1] + ax[1]*y[i-1];
#define   __y_equations                 y[i] = 2.0*x[i-1]*y[i-1] - ay[  0]*x[i-1] + ay[1]*y[i-1];

#define   __x_equations_lyaps           xenew =     xe*xe -    ye*ye + ax[0]*xe + ax[1]*ye;
#define   __y_equations_lyaps           yenew = 2.0*xe*ye - ay[0]*xe + ay[1]*ye;

#endif  //__equations
