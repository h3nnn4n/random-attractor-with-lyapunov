#ifndef   __equations
#define   __equations

#define NPARAMS 13

// The set of equations from http://mathworld.wolfram.com/StrangeAttractor.html
#define   __x_equations        x[i] = ax[0] + ax[1]*x[i-1] + ax[2]*x[i-1]*x[i-1] + ax[3]*x[i-1]*y[i-1] + ax[4]*y[i-1] + ax[5]*y[i-1]*y[i-1];
#define   __y_equations        y[i] = ay[0] + ay[1]*x[i-1] + ay[2]*x[i-1]*x[i-1] + ay[3]*x[i-1]*y[i-1] + ay[4]*y[i-1] + ay[5]*y[i-1]*y[i-1];

#define   __x_equations_lyaps xenew = ax[0] + ax[1]*xe     + ax[2]*xe*xe         + ax[3]*xe*ye         + ax[4]*ye     + ax[5]*ye*ye;
#define   __y_equations_lyaps yenew = ay[0] + ay[1]*xe     + ay[2]*xe*xe         + ay[3]*xe*ye         + ay[4]*ye     + ay[5]*ye*ye;

#endif  //__equations
