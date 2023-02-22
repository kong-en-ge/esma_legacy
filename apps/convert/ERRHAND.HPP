#ifndef __ERRHAND_HPP__
#define __ERRHAND_HPP__

#include <stdlib.h>

typedef void(*errorEvent)(void* context, int code, char const *message = NULL);

class CErrorHandler
{
private:
  void* _context;
  errorEvent _onError;
public:
  CErrorHandler(errorEvent onError, void* context = NULL);

  friend void handleError(CErrorHandler* handler,
    int code, char const *message = NULL);
};

#endif