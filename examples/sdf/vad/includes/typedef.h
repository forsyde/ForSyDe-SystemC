/*_____________________
 |                     |
 | Basic types.        |
 |_____________________|
*/
#ifndef _TYPEDEF_H
#define _TYPEDEF_H

// macro for easy enabling/disabling messages
//#define PRINT_TASK(task_name, msg) cout << sc_time_stamp() << "\t" << TASK_NAME << ": " << msg << endl;
#define PRINT_TASK(task_name, msg)

// The following was for SCoPE, I have to remove it, 
// any consequence for SCoPE estimation?
// It should not, becase this is applied only to task code
// which is not instrumented with scope-g++
// #define sizeof(x) 2 //sizeof(uc_adapt_type(x))

//#define unsigned

#ifdef SIM


#define main func_main2

extern inline int no_hance_nada(void *x,...);
extern inline int no_hance_nada(int x,...);
extern inline int no_hance_nada(const char * x,...);



#ifdef SYS

#define exit my_exit
extern int my_exit(int);

#define fprintf (int)1?(int)1:(int)no_hance_nada
#define strcmp(x,y) strcmp(uc_adapt_type(x),uc_adapt_type(y))
#define fopen(x,y) fopen(uc_adapt_type(x),uc_adapt_type(y))

#else 

#define fprintf (int)1?(int)1:(int)no_hance_nada
#define printf (int)1?(int)1:(int)no_hance_nada 
#define fopen (int)1?(int*)1:(int*)no_hance_nada
#define fclose (int)1?(int)1:(int)no_hance_nada

//#define fread(x,y,z,a) f_read_aux(x,y,z,a)

//#define fwrite (int)1?(int)1:(int)no_hance_nada

#define strcmp (int)1?(int)1:(int)no_hance_nada


#define NULL 0
#define FILE int
static FILE stderr;

#endif

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#endif


#if defined(__BORLANDC__) || defined(__WATCOMC__) || defined(_MSC_VER) || defined(__ZTC__)
typedef short Word16;
typedef long Word32;
typedef int Flag;

#elif defined(__sun)
typedef short Word16;
typedef long Word32;
typedef int Flag;

#elif defined(__unix__) || defined(__unix)
typedef short Word16;
typedef int Word32;
typedef int Flag;

#else
typedef short Word16;
typedef int Word32;
typedef int Flag;

#endif

#endif
