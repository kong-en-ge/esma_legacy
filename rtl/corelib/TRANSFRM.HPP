#ifndef __TRANSFRM_HPP__
#define __TRANSFRM_HPP__

#include "geometry.hpp"

struct AxisScaling
{
  double mult, add;
};

struct TransformData {
  AxisScaling x;
  AxisScaling y;
};

#define DECLARE_CONVERT_PROC(Name) \
  void Name(const void* src, void* dst, unsigned count);

class CRcRTransform {
private:
  CRectD boundsR;
  CRectL boundsD;
  TransformData drCoeffs, rdCoeffs;
  bool valid;

  void reCalc(void);
public:
  CRcRTransform(void);

  void getBounds(CRectL* pOrdBounds, CRectD* pFloatBounds) const;
  void setBounds(CRectL const* pOrdBounds, CRectD const* pFloatBounds);
  bool isCorrect(void) const;

  DECLARE_CONVERT_PROC(LtoR)
  DECLARE_CONVERT_PROC(StoR)
  DECLARE_CONVERT_PROC(RtoL)
  DECLARE_CONVERT_PROC(RtoS)
};

DECLARE_CONVERT_PROC(LtoD)

#endif