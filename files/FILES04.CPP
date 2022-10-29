#include <errno.h>
#include <io.h>
#include "mem.h"
#include "files.hpp"

int pascal Files::write(unsigned _start,unsigned _len,const void *_buf){
  int ret;
  if ((seek(_start)!=-1)&&(::write(handle,_buf,_len*recsize))!=-1) {
    if (filelen<_start+_len) filelen=_start+_len;
    if ((bufsize>end-start)&&(_start==end))
      end+= (bufsize-(end-start)) > _len ? _len : (bufsize-(end-start));
    int cop=end>_start+_len ? _start+_len : end;
    if ((start<=_start)&&(_start<end)) {
	 memmove(buf+(_start-start)*recsize,_buf,(cop-_start)*recsize);
    } else if ((_start<start)&&(_start+_len>=start)) {
	 memmove(buf,(char *)_buf+(start-_start)*recsize,(cop-start)*recsize);
    }
    return _len;
  };
  flush();
  return -1;
}
