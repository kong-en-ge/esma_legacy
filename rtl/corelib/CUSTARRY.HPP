#ifndef __CUSTARRY_HPP__
#define __CUSTARRY_HPP__

#include <memory.h>
#include <assert.h>

#include "coretype.hpp"
#include "softasrt.hpp"

#define PRECONDITION(p)   ((p) ? (void)0 : (void) __trap(      \
                    "Precondition violated: %s, file %s, line %d\n", \
                    #p, __FILE__, __LINE__ ) )

template <class T> class CCustomArray {
private:
  int count_;
  int capacity_;
  bool allowResize_;
  T* items_;

  void setCapacity(unsigned newCount);

public:
  CCustomArray(unsigned capacity = 0);

  ~CCustomArray(void)
  {
    delete[] items_;
  }

  T& operator [](unsigned index) const
  {
    PRECONDITION(index < count_);
    return items_[index];
  }

  T& operator [](unsigned index)
  {
    PRECONDITION(index < count_);
    return items_[index];
  }

  void add(T const& item)
  {
    insert(count_, item);
  }

  void clear(void);

  int getCount(void) const
  {
    return count_;
  }

  T const& getLast(void) const
  {
    PRECONDITION(count_);
    return items_[count_ - 1];
  }

  void insert(unsigned index, T const& item);

  bool isEmpty(void) const
  {
    return !count_;
  }

  void qsort(int(* fcmp)(T const&, T const&));

  T popLast(void)
  {
    T retValue = getLast();
    remove(count_ - 1);
    return retValue;
  }

  void remove(unsigned index);

  void removeLast(void)
  {
    remove(count_ - 1);
  }
};

template <class T>
CCustomArray<T>::CCustomArray(unsigned capacity) :
  count_(0),
  capacity_(0),
  items_(NULL)
{
  setCapacity(capacity);
  allowResize_ = !capacity;
  count_ = capacity;
}

template <class T>
void CCustomArray<T>::clear(void)
{
  PRECONDITION(allowResize_);
  setCapacity(0);
}

template <class T>
void CCustomArray<T>::insert(unsigned index, T const& item)
{
  PRECONDITION(allowResize_ && index <= count_);
  setCapacity(count_ + 1);
  count_++;
  memmove(items_ + index + 1, items_ + index,
    (count_ - index - 1) * sizeof(T));
  items_[index] = item;
}

template <class T>
void CCustomArray<T>::qsort(int(* fcmp)(T const&, T const&))
{
  ::qsort(items_, count_, sizeof(T), (int(*)(void const*, void const*))fcmp);
}

template <class T>
void CCustomArray<T>::remove(unsigned index)
{
  PRECONDITION(allowResize_ && index < count_);
  memmove(items_ + index, items_ + index + 1,
    (count_ - index - 1) * sizeof(T));
  count_--;
  setCapacity(count_);
}

template <class T>
void CCustomArray<T>::setCapacity(unsigned newCount)
{
  int capacity = newCount < 32 ? (newCount + 3) & ~3 : (newCount + 15) & ~15;
  if (capacity == capacity_)
    return;

  T* temp = capacity ? new T[capacity] : NULL;

  if (items_ && temp)
    memmove(temp, items_,
      (count_ < newCount ? count_ : newCount) * sizeof(T));

  if (count_ > newCount)
    count_ = newCount;

  delete[] items_;
  items_ = temp;
  capacity_ = capacity;
}

#endif