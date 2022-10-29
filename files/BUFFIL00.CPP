#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <mem.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include "buffiles.hpp"

pascal BufFiles::BufFiles() {
    setmem(this,sizeof(BufFiles),0);
    handle=-1;
}

void pascal BufFiles::Init(unsigned  _headsize,unsigned  _recsize,unsigned _bufsize) {
    if (handle!=-1) close();
    headsize=_headsize;
    recsize =_recsize ;
    bufsize =_bufsize  ;
    buf=new char[_bufsize*_recsize+10];
    if ((buf==NULL)&&((bufsize=(coreleft()-10)/_recsize)!=0))
	   buf=new char[bufsize*_recsize+10];
};

int pascal BufFiles::open(const char far  *path) {
    if (handle!=-1) close();
    if ((handle=::open(path,O_RDONLY | O_BINARY))==-1) {
      delete buf;buf=NULL;
      return -1;
    }
    filelen=((unsigned long)(filelength(handle)-headsize))/recsize;
    return 0;
};

int pascal BufFiles::close() {
    flush();
    delete buf;
    buf=NULL;int ret=0;
    if (handle!=-1) ret=::close(handle);
    handle=-1;
    return ret;
};

void pascal BufFiles::flush() {
    start=end=0;
};

pascal BufFiles::~BufFiles(void) {
 delete buf;
 buf=NULL;
};

int pascal BufFiles::read(unsigned long _start,unsigned long _end,void **_buf) {
   if (buf==NULL) {
	errno=ENOMEM;
	return -1;
   };
   unsigned long st,en,smb=0;
   if (_start>=filelen) return 0;
   if (_end>=filelen) _end=filelen-1;
   if ((_start<start) || (end<=_end)) {//отсутствует в буфуре
	en=_start+bufsize>filelen ? filelen : _start+bufsize;
	st=en>bufsize ? en-bufsize : 0;
	if ((st>=start) && (end>st)) {
	  smb=(end-st)*recsize;
	  memmove(buf,buf+(st-start)*recsize,smb);
	  start=st;st=end;end=en;
	} else if ((st<start)&&(en<=end)&&(en>start)) {
	  memmove(buf+(end-en)*recsize,buf,(en-start)*recsize);
	  end=en;en=start;start=st;
	} else {
	  start=st;end=en;
	};
	if (makebuf(st,(en-st)*recsize,buf+smb)!=0) {
	  return -1;
	}
   }
   *_buf=buf+(_start-start)*recsize;
   return _end-_start+1;
};

int BufFiles::makebuf(unsigned long _start,unsigned _len,void far *_buf) {
       _start=_start*recsize+headsize;
       if (::lseek(handle,_start,SEEK_SET)==-1 || ::read(handle,_buf,_len)==-1) {
	  flush();
	  return -1;
       };
       return 0;
};

int pascal BufFiles::readhead(void far *_buf) {
  int ret;
  if ((ret=::lseek(handle,0,SEEK_SET))!=-1) {
    ret=::read(handle,_buf,headsize);
  };
  return ret;
};
int pascal BufFiles::readtobuf(unsigned long _start,unsigned long _end,void *buf) {
  void  *bf;
  int ret;
  if ((ret=read(_start,_end,&bf))>0) memmove(buf,bf,recsize*ret);
  return ret;
}