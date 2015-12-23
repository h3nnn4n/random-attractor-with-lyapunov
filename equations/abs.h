#ifndef   __equations
#define   __equations

#define NPARAMS 7

#define   __x_equations        x[i] = ax[0] + ax[1]*x[i-1] + ay[2]*y[i-1] + ax[3]*abs(x[i-1]) + ax[4]*abs(y[i-1]) + ax[5]*x[i-1]*x[i-1] + ax[6]*y[i-1]*y[i-1];
#define   __y_equations        y[i] = ay[0] + ay[1]*x[i-1] + ay[2]*y[i-1] + ay[3]*abs(x[i-1]) + ay[4]*abs(y[i-1]) + ay[5]*x[i-1]*x[i-1] + ay[6]*y[i-1]*y[i-1];

#define   __x_equations_lyaps xenew = ax[0] + ax[1]*xe     + ay[2]*ye     + ax[3]*abs(xe)     + ax[4]*abs(ye)     + ax[5]*xe*xe         + ax[6]*ye*ye        ;
#define   __y_equations_lyaps yenew = ay[0] + ay[1]*xe     + ay[2]*ye     + ay[3]*abs(xe)     + ay[4]*abs(ye)     + ay[5]*xe*xe         + ay[6]*ye*ye        ;

#endif  //__equations
