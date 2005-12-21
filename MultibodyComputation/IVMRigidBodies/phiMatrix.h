#ifndef __phiMatrix_hh__
#define __phiMatrix_hh__

// support for efficient matrix multiplication involving the special phi
// matrix

#include "cdsVec3.h"
#include <fixedVector.h>
#include <fixedMatrix.h>
#include <subVector.h>
#include <subMatrix.h>

typedef FixedVector<double,6>   CDSVec6;
typedef FixedMatrix<double,6,6> CDSMat66;

class PhiMatrixTranspose;

class PhiMatrix {
public:
    typedef PhiMatrixTranspose TransposeType;

    PhiMatrix() { l_(0) = 1.31e30; }
    PhiMatrix(const CDSVec3& l) : l_(l) {}

    const CDSVec3& l() const { assert( l_(0) != 1.31e30 ); return l_; }
private:
    CDSVec3 l_;
};

class PhiMatrixTranspose {
public:
  PhiMatrixTranspose(const PhiMatrix& phi) : phi(phi) {}
  const CDSVec3 l() const {return phi.l();}
private:
  const PhiMatrix& phi;
};

inline PhiMatrixTranspose
transpose(const PhiMatrix& phi)
{
    PhiMatrixTranspose ret(phi);
    return ret;
}
  
inline CDSVec6
operator*(const PhiMatrix& phi,
          const CDSVec6&      vec)
{
    const SubVector<const CDSVec6> v1(vec,0,3);
    const SubVector<const CDSVec6> v2(vec,3,3);

    FixedVector<double,6> ret;
    SubVector<CDSVec6> rv1(ret,0,3);
    SubVector<CDSVec6> rv2(ret,3,3);

    rv1 = (v1 + cross(phi.l() , v2.vector())).vector();
    rv2 = v2.vector();

    return ret;
}

inline CDSVec6
operator*(const PhiMatrixTranspose& phiT,
          const CDSVec6&            vec)
{
    const SubVector<const CDSVec6> v1(vec,0,3);
    const SubVector<const CDSVec6> v2(vec,3,3);

    CDSVec6 ret;
    SubVector<CDSVec6> rv1(ret,0,3);
    SubVector<CDSVec6> rv2(ret,3,3);

    rv1 = v1.vector();
    rv2 = (v2 + cross(v1.vector() , phiT.l())).vector();

    return ret;
}

inline CDSMat66
operator*(const CDSMat66&           mat,
          const PhiMatrixTranspose& phiT)
{
    SubMatrix<const CDSMat66> m11(mat,0,0,3,3);
    SubMatrix<const CDSMat66> m12(mat,0,3,3,3);
    SubMatrix<const CDSMat66> m21(mat,3,0,3,3);
    SubMatrix<const CDSMat66> m22(mat,3,3,3,3);

    CDSMat66 ret;
    SubMatrix<CDSMat66> rm11(ret,0,0,3,3);
    SubMatrix<CDSMat66> rm12(ret,0,3,3,3);
    SubMatrix<CDSMat66> rm21(ret,3,0,3,3);
    SubMatrix<CDSMat66> rm22(ret,3,3,3,3);

    rm11 = m11 - m12 * crossMat(phiT.l());
    rm12 = m12;
    rm21 = m21 - m22 * crossMat(phiT.l());
    rm22 = m22;

    return ret;
}

#endif /*  __phiMatrix_hh__ */
