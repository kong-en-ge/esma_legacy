#include <alloc.h>
#include <io.h>
#include <fcntl.h>

#include "graphics.h"
#include "indexmap.hpp"
#include "apputils.hpp"

CIndexMapImage::CIndexMapImage(void)
{
  imageData = NULL;
}

CIndexMapImage::~CIndexMapImage(void)
{
  freeImageData();
}

void CIndexMapImage::freeImageData(void)
{
  if (hasData())
  {
    free(imageData);
    imageData = NULL;
  }
}

bool CIndexMapImage::load(char const * filename)
{
  freeImageData();
  int handle;
  if ((handle = open(filename, O_BINARY)) > 0)
  {
    long size = filelength(handle);
    if ((imageData = malloc(size)) == NULL)
    {
      displayError(0);
      return false;
    }
    read(handle, imageData, size);

    close(handle);
  }
  else
    return false;

  return true;
}

void CIndexMapImage::paintImage(int x, int y) const
{
  if (hasData())
    putimage(x, y, imageData, COPY_PUT);
}
