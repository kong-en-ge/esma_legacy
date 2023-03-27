#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

template <class T> struct CPoint
{
public:
  T x, y;

  CPoint(void);
  CPoint(T x, T y);

  CPoint const& operator =(CPoint const &value);
  CPoint const& operator +=(CPoint const &value);
  CPoint const& operator -=(CPoint const &value);
};

template <class T> CPoint<T>::CPoint(void)
{
  x = 0;
  y = 0;
}

template <class T> CPoint<T>::CPoint(T x, T y)
{
  this->x = x;
  this->y = y;
}

template <class T> CPoint<T> const& CPoint<T>::operator=(CPoint<T> const & value)
{
  this->x = value.x;
  this->y = value.y;
  return *this;
}

template <class T> CPoint<T> const& CPoint<T>::operator+=(CPoint<T> const & value)
{
  this->x += value.x;
  this->y += value.y;
  return *this;
}

template <class T> CPoint<T> const& CPoint<T>::operator-=(CPoint<T> const & value)
{
  this->x -= value.x;
  this->y -= value.y;
  return *this;
}

typedef CPoint<short int> CPointS;
typedef CPoint<long int> CPointL;
typedef CPoint<float> CPointF;
typedef CPoint<double> CPointD;

template <class T> struct CRect
{
public:
  T left, top, right, bottom;

  T getHeight(void) const;
  T getWidth(void) const;
  int containsPoint(T x, T y) const;
  void getCenter(CPoint<T> *center) const;
  void getCenter(T *x, T *y) const;
  void getSize(CPoint<T> *size) const;
  void inflate(CPoint<T> const& value);
  void inflate(T dx, T dy);
  void initByPoint(CPoint<T> const& value);
  void initByPoint(T x, T y);
  int intersect(CRect<T> const& src1, CRect<T> const& src2);
  int intersect(CRect<T> const& src);
  void offset(T x, T y);
  void offset(CPoint<T> const& value);
  void putPointInside(T *x, T *y) const;
  void setValues(T left, T top, T right, T bottom);
};

template <class T> inline T CRect<T>::getWidth(void) const
{
  return right - left;
}

template <class T> inline T CRect<T>::getHeight(void) const
{
  return bottom - top;
}

template <class T> void CRect<T>::inflate(CPoint<T> const& value)
{
  left -= value.x;
  top -= value.y;
  right += value.x;
  bottom += value.y;
}

template <class T> inline void CRect<T>::initByPoint(CPoint<T> const& value)
{
  setValues(value.x, value.y, value.x, value.y);
}

template <class T> inline void CRect<T>::initByPoint(T x, T y)
{
  setValues(x, y, x, y);
}

template <class T> inline int CRect<T>::intersect(CRect<T> const& src)
{
  return intersect(*this, src);
}

template <class T> inline void CRect<T>::setValues(T left, T top, T right, T bottom)
{
  this->left = left;
  this->top = top;
  this->right = right;
  this->bottom = bottom;
}

typedef CRect<short int> CRectS;
typedef CRect<long int> CRectL;
typedef CRect<float> CRectF;
typedef CRect<double> CRectD;

#endif
