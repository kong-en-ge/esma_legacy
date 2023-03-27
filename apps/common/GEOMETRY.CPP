#include <stdlib.h>

#include "geometry.hpp"

#define min(a, b) (a)<(b)?(a):(b)
#define max(a, b) (a)>(b)?(a):(b)

template <class T> int CRect<T>::containsPoint(T x, T y) const
{
  return (x >= left) && (x < right) & (y >= top) && (y < bottom);
}

template <class T> void CRect<T>::getCenter(CPoint<T>* center) const
{
  center->x = (right + left) / 2;
  center->y = (bottom + top) / 2;
}

template <class T> void CRect<T>::getCenter(T* x, T* y) const
{
  if (x)
    *x = (right + left) / 2;
  if (y)
    *y = (bottom + top) / 2;
}

template <class T> void CRect<T>::getSize(CPoint<T>* size) const
{
  size->x = getWidth();
  size->y = getHeight();
}

template <class T> void CRect<T>::inflate(T dx, T dy)
{
  left -= dx;
  top -= dy;
  right += dx;
  bottom += dy;
}

template <class T> int CRect<T>::intersect(CRect<T> const& src1,
  CRect<T> const& src2)
{
  left = max(src1.left, src2.left);
  top = max(src1.top, src2.top);
  right = min(src1.right, src2.right);
  bottom = min(src1.bottom, src2.bottom);
  return left < right && top < bottom;
}

template <class T> void CRect<T>::offset(T dx, T dy)
{
  left += dx;
  top += dy;
  right += dx;
  bottom += dy;
}

template <class T> void offset(CPoint<T> const& value)
{
  offset(value.x, value.y);
}

template <class T> void CRect<T>::putPointInside(T *x, T *y) const
{
  if (*x > right)
    *x = right;
  if (*y > bottom)
    *y = bottom;
  if (*x < left)
    *x = left;
  if (*y < top)
    *y = top;
}