#ifndef FWRITEHPP
#define FWRITEHPP

class FWRITE
{ public:
  char *buff;
  int  handle;
  int  rec_size;
  int  buff_size;
  int  head_size;
  int  current;
  pascal ~FWRITE();
  int  pascal open(char *path,int h_size,int r_size,int b_size);
  int  pascal w_head(void* ptr);
  int  pascal add(void*);
  int  pascal close(int n=1);
};

#endif