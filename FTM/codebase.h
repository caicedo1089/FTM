// (c) by Stefan Roettger

#ifndef CODEBASE_H
#define CODEBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <math.h>

#define BOOLINT char

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

#define ERRORMSG() errormsg(__FILE__,__LINE__)

inline void errormsg(char *file,int line)
   {
   fprintf(stderr,"fatal error in <%s> at line %d!\n",file,line);
   exit(EXIT_FAILURE);
   }


inline int imin(const int a,const int b) {return((a<b)?a:b);}

inline double FMIN(const double a,const double b) {return((a<b)?a:b);}
#define fmin(a,b) FMIN(a,b)

inline int imax(const int a,const int b) {return((a>b)?a:b);}

inline double FMAX(const double a,const double b) {return((a>b)?a:b);}
#define fmax(a,b) FMAX(a,b)

#define snprintf _snprintf

#undef ffloor
#define ffloor(x) floor((double)(x))
#undef fceil
#define fceil(x) ceil((double)(x))
#define ftrc(x) (int)ffloor(x)
#endif
