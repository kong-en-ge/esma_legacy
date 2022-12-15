#ifndef FWRITEHPP
#define FWRITEHPP

class FWRITE
{
public:
  char *buff;
  int handle;
  int rec_size;
  int buff_size;
  int head_size;
  int current;

  ~FWRITE();
  int open(char const *path, int h_size, int r_size, int b_size);
  int w_head(void const *ptr);
  int add(void *);
  int close(int n = 1);
};

#endif