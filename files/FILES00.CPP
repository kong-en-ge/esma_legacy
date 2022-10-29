#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include "files.hpp"

pascal Files::Files() {
    setmem(this,sizeof(Files),0);
    handle=-1;
}
void pascal Files::Init(int _headsize,int _recsize,int _bufsize) {
    if (handle!=-1) close();
    headsize=_headsize;
    recsize =_recsize ;
    bufsize =_bufsize  ;
    buf=new char[_bufsize*_recsize+10];
    if ((buf==NULL)&&((bufsize=(coreleft()-10)/_recsize)!=0))
	   buf=new char[bufsize*_recsize+10];
};
int pascal Files::open(const char *path, int read_only) {
    if (handle!=-1) close();
    int oflag;
    oflag= read_only ? O_RDONLY | O_DENYWRITE |  O_BINARY:
       O_RDWR | O_BINARY | O_DENYALL;
    if ((handle=::open(path, oflag))==-1) {
      delete buf;buf=NULL;
      return -1;
    }
    filelen=((unsigned long)(filelength(handle)-headsize))/recsize;
    return 0;
};
int pascal Files::close() {
    flush();
    delete buf;
    buf=NULL;int ret=0;
    if (handle!=-1) ret=::close(handle);
    handle=-1;
    return ret;
};
void pascal Files::flush() {
    start=end=0;
};
pascal Files::~Files(void) {
 delete buf;
 buf=NULL;
};
