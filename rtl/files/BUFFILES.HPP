#ifndef BUFFILESHPP
#define BUFFILESHPP

class BufFiles
{
public:
  int handle;
  unsigned int headsize, recsize;
  unsigned long start, end, bufsize;
  unsigned long filelen;
  char far *buf;
  //_headsize,_recsize � �����,_bufsize � �������

  BufFiles();
  ~BufFiles();

  int getHandle() const;
  void Init(unsigned _headsize, unsigned _recsize, unsigned _bufsize);
  void flush();
  int readtobuf(unsigned long _start, unsigned long _end, void far *buf);
  int read(unsigned long _start, unsigned long _end, void far *far *buf);
  int open(const char far *path);
  int close();
  int readhead(void far *buf);
  virtual int makebuf(unsigned long _start, unsigned len, void far *buf);
};

//������ ����� � ࠧ��� ��������� ������ ���� ��묨

class XMSFiles : public BufFiles
{
public:
  unsigned hanxms;

  XMSFiles();
  ~XMSFiles();

  int open(const char *path);
  int close();
  virtual int makebuf(unsigned long _start, unsigned len, void far *buf);
  int readhead(void *buf);
};

inline int BufFiles::getHandle() const
{
  return handle;
}

#endif