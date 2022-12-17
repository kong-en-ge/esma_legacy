#ifndef __INDEXMAP_HPP__
#define __INDEXMAP_HPP__

#include <stdlib.h>

class CIndexMapImage
{
private:
  void* imageData;
  void freeImageData(void);
public:
  CIndexMapImage(void);
  ~CIndexMapImage(void);

  int hasData(void) const;
  int load(char const *filename);
  void paintImage(int x, int y) const;
};

inline int CIndexMapImage::hasData(void) const
{
  return (imageData != NULL);
}

#endif