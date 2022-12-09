#ifndef TEXTFILEHPP
#define TEXTFILEHPP

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
  char *buf;
  unsigned int start, end, bufsize;

  TEXTFILE();
  ~TEXTFILE();

  int status();
  int open(const char *name, int bufsize);
  void close();
  int read();
  int readstr(char *str, int maxlen);
  // readfromstr moves internal pointer
  int readfromstr(char *str, int maxlen, char *razd);
  int gostart();
  int goendstr();
};

inline int TEXTFILE::status()
{
  return _status;
}

#endif
