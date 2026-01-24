#ifndef TEXTFILEHPP
#define TEXTFILEHPP

#include "coretype.hpp"

#define T_ERR -1
#define T_EOF -2
#define T_EOL -3
#define T_NOT -4

class TEXTFILE
{
private:
  int _status;

public:
  int handle;
  PSTR buf;
  unsigned int start, end, bufsize;

  TEXTFILE();
  ~TEXTFILE();

  int status();
  int open(PCSTR name, int bufsize);
  void close();
  int read();
  int readstr(PSTR str, int maxlen);
  // readfromstr moves internal pointer
  int readfromstr(PSTR str, int maxlen, PSTR razd);
  int gostart();
  int goendstr();
};

inline int TEXTFILE::status()
{
  return _status;
}

#endif
