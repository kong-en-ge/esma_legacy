#ifndef __INDEXMAP_HPP__
#define __INDEXMAP_HPP__

#include <stdlib.h>

#include "coretype.hpp"

class CIndexMapImage
{
private:
  void* imageData;
  void freeImageData(void);
public:
  CIndexMapImage(void);
  ~CIndexMapImage(void);

  bool hasData(void) const;
  bool load(char const *filename);
  void paintImage(int x, int y) const;
};

inline bool CIndexMapImage::hasData(void) const
{
  return (imageData != NULL);
}

#endif