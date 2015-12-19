#ifndef   __equations
#define   __equations

#define NPARAMS 13

#define   __x_equations        x[i] = ax[0] + ax[1]*x[i-1]                 + ax[ 2]*x[i-1]*y[i-1]                  + ax[ 3]*x[i-1]*y[i-1]*y[i-1]                  + ax[ 4]*x[i-1]*y[i-1]*y[i-1]*y[i-1]                  + \
                                              ax[5]*x[i-1]*x[i-1]          + ax[ 6]*x[i-1]*x[i-1]*y[i-1]           + ax[ 7]*x[i-1]*x[i-1]*y[i-1]*y[i-1]           + ax[ 8]*x[i-1]*x[i-1]*y[i-1]*y[i-1]*y[i-1]           + \
                                              ax[9]*x[i-1]*x[i-1]*x[i-1]   + ax[10]*x[i-1]*x[i-1]*x[i-1]*y[i-1]    + ax[11]*x[i-1]*x[i-1]*x[i-1]*y[i-1]*y[i-1]    + ax[12]*x[i-1]*x[i-1]*x[i-1]*y[i-1]*y[i-1]*y[i-1]    ;

#define   __y_equations        y[i] = ay[0] + ay[1]*x[i-1]                 + ay[ 2]*x[i-1]*y[i-1]                  + ay[ 3]*x[i-1]*y[i-1]*y[i-1]                  + ay[ 4]*x[i-1]*y[i-1]*y[i-1]*y[i-1]                  + \
                                              ay[5]*x[i-1]*x[i-1]          + ay[ 6]*x[i-1]*x[i-1]*y[i-1]           + ay[ 7]*x[i-1]*x[i-1]*y[i-1]*y[i-1]           + ay[ 8]*x[i-1]*x[i-1]*y[i-1]*y[i-1]*y[i-1]           + \
                                              ay[9]*x[i-1]*x[i-1]*x[i-1]   + ay[10]*x[i-1]*x[i-1]*x[i-1]*y[i-1]    + ay[11]*x[i-1]*x[i-1]*x[i-1]*y[i-1]*y[i-1]    + ay[12]*x[i-1]*x[i-1]*x[i-1]*y[i-1]*y[i-1]*y[i-1]    ;

#define   __x_equations_lyaps xenew = ax[0] + ax[1]*xe                     + ax[ 2]*xe*ye                          + ax[ 3]*xe*ye*ye                              + ax[ 4]*xe*ye*ye*ye                                  + \
                                              ax[5]*xe*xe                  + ax[ 6]*xe*xe*ye                       + ax[ 7]*xe*xe*ye*ye                           + ax[ 8]*xe*xe*ye*ye*ye                               + \
                                              ax[9]*xe*xe*xe               + ax[10]*xe*xe*xe*ye                    + ax[11]*xe*xe*xe*ye*ye                        + ax[12]*xe*xe*xe*ye*ye*ye                            ;

#define   __y_equations_lyaps yenew = ay[0] + ay[1]*xe                     + ay[ 2]*xe*ye                          + ay[ 3]*xe*ye*ye                              + ay[ 4]*xe*ye*ye*ye                                  + \
                                              ay[5]*xe*xe                  + ay[ 6]*xe*xe*ye                       + ay[ 7]*xe*xe*ye*ye                           + ay[ 8]*xe*xe*ye*ye*ye                               + \
                                              ay[9]*xe*xe*xe               + ay[10]*xe*xe*xe*ye                    + ay[11]*xe*xe*xe*ye*ye                        + ay[12]*xe*xe*xe*ye*ye*ye                            ;

#endif  //__equations
