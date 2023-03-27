#include <math.h>

#include "transfrm.hpp"

#define DEFINE_DR_CONVERT(Name, S, D) \
  void CRcRTransform::Name(const void* src, void* dst, unsigned count)\
  {\
    if (!valid) \
      reCalc();\
\
    CPoint##S const* srcPoint = (CPoint##S const*)src;\
    CPoint##D* dstPoint = (CPoint##D*)dst;\
\
    while (count)\
    {\
      dstPoint->x = srcPoint->y * drCoeffs.y.mult + drCoeffs.y.add;\
      dstPoint->y = srcPoint->x * drCoeffs.x.mult + drCoeffs.x.add;\
      srcPoint++;\
      dstPoint++;\
      count--;\
    }\
  }

#define DEFINE_RD_CONVERT(Name, S, D) \
  void CRcRTransform::Name(const void* src, void* dst, unsigned count)\
  {\
    if (!valid) \
      reCalc();\
\
    CPoint##S const* srcPoint = (CPoint##S const*)src;\
    CPoint##D* dstPoint = (CPoint##D*)dst;\
\
    while (count)\
    {\
      dstPoint->x = srcPoint->y * rdCoeffs.y.mult + rdCoeffs.y.add;\
      dstPoint->y = srcPoint->x * rdCoeffs.x.mult + rdCoeffs.x.add;\
      srcPoint++;\
      dstPoint++;\
      count--;\
    }\
  }

#define DEFINE_CONVERT_PROC(Name, T1, T2) \
  void Name(const void* src, void* dst, unsigned count)\
  {\
    T1 const* srcValue = (T1*)src;\
    T2* dstValue = (T2*)dst;\
  \
    while (count)\
    {\
      *dstValue = *srcValue;\
      srcValue++;\
      dstValue++;\
      count--;\
    }\
  }

DEFINE_CONVERT_PROC(LtoD, long, double);
DEFINE_CONVERT_PROC(DtoL, double, long);

void setIdentityTransform(CRcRTransform* pTransform)
{
  CRectL rcOrd;
  CRectD rcFloat;
  rcOrd.setValues(0, 0, 1, 1);
  LtoD(&rcOrd, &rcFloat, 2);
  pTransform->setBounds(&rcOrd, &rcFloat);
}

int sameValue(double a, double b)
{
  return abs(a - b) < 1e-6;
}

void calcScaling(double srcBase, double dstBase, 
  double srcSize, double dstSize, 
  AxisScaling *scaling)
{
  scaling->mult = dstSize / srcSize;
  scaling->add = dstBase - srcBase * scaling->mult;
}

/*****************
 * CRcRTransform *
 *****************/

CRcRTransform::CRcRTransform(void)
{
  setIdentityTransform(this);
}

void CRcRTransform::getBounds(CRectL* pOrdBounds, CRectD* pFloatBounds) const
{
  if (pOrdBounds)
    *pOrdBounds = boundsD;
  if (pFloatBounds)
    *pFloatBounds = boundsR;
}

int CRcRTransform::isCorrect(void) const
{
  return (boundsD.left != boundsD.right) &&
    (boundsD.top != boundsD.bottom) &&
    !sameValue(boundsR.left, boundsR.right) &&
    !sameValue(boundsR.top, boundsR.bottom);
}

DEFINE_DR_CONVERT(StoR, S, D);
DEFINE_DR_CONVERT(LtoR, L, D);
DEFINE_RD_CONVERT(RtoS, D, S);
DEFINE_RD_CONVERT(RtoL, D, L);

void CRcRTransform::reCalc(void)
{
  calcScaling(boundsD.left, boundsR.top, boundsD.getWidth(), boundsR.getHeight(), &drCoeffs.x);
  calcScaling(boundsD.top, boundsR.left, boundsD.getHeight(), boundsR.getWidth(), &drCoeffs.y);
  calcScaling(boundsR.left, boundsD.top, boundsR.getWidth(), boundsD.getHeight(), &rdCoeffs.x);
  calcScaling(boundsR.top, boundsD.left, boundsR.getHeight(), boundsD.getWidth(), &rdCoeffs.y);
  valid = 1;
}

void CRcRTransform::setBounds(CRectL const* pOrdBounds, CRectD const* pFloatBounds)
{
  if (pOrdBounds)
    boundsD = *pOrdBounds;
  if (pFloatBounds)
    boundsR = *pFloatBounds;
  valid = 0;
}
