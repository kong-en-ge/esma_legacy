#ifndef fileshpp
#define fileshpp

#include "coretype.hpp"

class Files
{
private:
  int handle;
  unsigned filelen;
  unsigned bufsize;

public:
  unsigned int headsize, recsize;
  unsigned start, end;
  char far *buf;
  //_headsize,_recsize в байтах,_bufsize в записях

  Files();
  ~Files();

  int getHandle() const;
  unsigned getBufSize() const;
  unsigned int getFileLen() const;

  void Init(int _headsize, int _recsize, int _bufsize);
  int seek(unsigned no);
  int open(PCSTR path, int readonly = 1);
  int close();
  void flush();
  int readhead(PVOID buf);
  int writehead(PCVOID buf);
  int readtobuf(unsigned _start, unsigned _end, void *buf);
  int read(unsigned _start, unsigned _end, void **buf);
  int write(unsigned _start, unsigned _len, const void *buf);
  int writefrombuf(unsigned _start, unsigned _len);
  int write_end(unsigned _len, const void *buf);
};

inline unsigned Files::getFileLen() const
{
  return filelen;
}

inline unsigned Files::getBufSize() const
{
  return bufsize;
}

inline int Files::getHandle() const
{
  return handle;
}

inline int Files::write_end(unsigned _len, const void *buf)
{
  return write(filelen, _len, buf);
}

#endif