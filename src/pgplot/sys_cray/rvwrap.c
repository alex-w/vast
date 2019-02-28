#include <fortran.h>

/*
 * This is a wrapper function used to call the /xrv driver from
 * CRAY FORTRAN.
 *--
 * 24-Mar-1997 - [mcs]
 */
void RVDRIV( int *ifunc, float *rbuf, int *nbuf, _fcd chr, int *lchr, int *mode ) {
 rvdriv_( ifunc, rbuf, nbuf, _fcdtocp( chr ), lchr, mode, _fcdlen( chr ) );
}
