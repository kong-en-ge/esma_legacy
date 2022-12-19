#ifndef __APP_HPP__
#define __APP_HPP__

class CApplication
{
private:
  char* pSaveDir;
public:
  CApplication(int argc, char *argv[]);
  virtual ~CApplication(void);

  virtual int run(void) = 0;
};


#endif