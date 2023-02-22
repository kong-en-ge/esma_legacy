#include "errhand.hpp"

void handleError(CErrorHandler* handler, int code, char const *message)
{
  if (handler != NULL)
    handler->_onError(handler->_context, code, message);
}

CErrorHandler::CErrorHandler(errorEvent onError, void* context)
{
  _onError = onError;
  _context = context;
}