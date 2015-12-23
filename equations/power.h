#ifndef   __equations
#define   __equations

#define NPARAMS 7

#define   __x_equations        x[i] = ax[0] + ax[1]*x[i-1] + ay[2]*y[i-1] + ax[3]*(x[i-1]) + ax[4]*pow(ax[5], (y[i-1])); // + ax[6]*x[i-1]*x[i-1] + ax[7]*y[i-1]*y[i-1];
#define   __y_equations        y[i] = ay[0] + ay[1]*x[i-1] + ay[2]*y[i-1] + ay[3]*(x[i-1]) + ay[4]*pow(ay[5], (y[i-1])); // + ay[6]*x[i-1]*x[i-1] + ay[7]*y[i-1]*y[i-1];

#define   __x_equations_lyaps xenew = ax[0] + ax[1]*xe     + ay[2]*ye     + ax[3]*(xe)     + ax[4]*pow(ax[5], (ye)    ); // + ax[6]*xe*xe         + ax[7]*ye*ye        ;
#define   __y_equations_lyaps yenew = ay[0] + ay[1]*xe     + ay[2]*ye     + ay[3]*(xe)     + ay[4]*pow(ay[5], (ye)    ); // + ay[6]*xe*xe         + ay[7]*ye*ye        ;

#endif  //__equations
