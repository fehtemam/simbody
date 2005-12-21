
#ifndef __linemin_hh__
#define __linemin_hh__

#include "sthead.h"
#include "mnbrak.h"
#include "brent.h"

template<class VEC, class S, class Func>
void 
linemin(Func func,
	VEC& p,
	S&   fret,
	VEC& xi  ,
	S&   stepsize);


//template<class S,class T>
//T
//f1dim(S& x)
//{
// VEC xt = pcom + x*xicom;
// f = nrfunc(xt);
// return f;
//}

template<class VEC, class S, class Func>
class F1Dim {
  const VEC& p;
  const VEC& xi;
  Func func;
public:
  F1Dim(const VEC& p,
	const VEC& xi,
	      Func func) :
    p(p), xi(xi), func(func) {}
  S operator()(const S& x) {
    VEC xt = p + x*xi;
    return func(xt);
  }
};



template<class VEC, class  S, class Func>
void 
linemin(Func func,
	VEC& p,
	S&   fret,
	VEC& xi  ,
	S&   stepsize)
{
 S ax = 0;
 S bx;
 S xx = stepsize;
 S fa, fb, fx;
 mnbrak(ax,xx,bx,fa,fx,fb,
	F1Dim<VEC,S,Func>(p,xi,func));
// if (ax < 0.0) cout << "linemin: ax<0: " << ax << '\n';
// if (xx < 0.0) cout << "linemin: xx<0: " << xx << '\n';
// if (bx < 0.0) cout << "linemin: bx<0: " << bx << '\n';
// if ( !(ax<xx&&xx<bx) )
//   cout << "linemin: abscissas out of order: " 
//	  << ax << ' ' << xx << ' ' << bx << '\n';
// if ( !( fx<fa && fx<fb) )
//   cout << "linemin: minimum not bracketed: "
//	  << fa << ' ' << fx << ' ' << fb << '\n';
   
 
 fret = brent(ax,xx,bx,
	      F1Dim<VEC,S,Func>(p,xi,func),stepsize);
// cout << "linemin: fret: " << fret << '\n';
 xi *= stepsize;
 p += xi;
} /* linemin */

#endif /* __linemin_hh__ */
