#include <stdlib.h>
#include "button.hpp"

enum MESSAGE_MODE
{
  mmError,
  mmWarning
};

static char const *windowTitles[] =
{
  "Ошибка!",
  "Внимание!"
};

static int const windowColors[] =
{
  LIGHTRED,
  LIGHTBLUE
};

class CMessageKeeper
{
private:
 const char** _texts;
 int _count;

 const char* getMessage(int aCode) const;
public:
 CMessageKeeper();

 void displayMessage(int code, MESSAGE_MODE mode, char const *text) const;
 void setMessages(char const *texts[], int count);
};

CMessageKeeper::CMessageKeeper()
{
  setMessages(NULL, 0);
}

const char* CMessageKeeper::getMessage(int aCode) const
{
  if ((aCode >=0) && (aCode < _count))
    return _texts[aCode];
  else
    return "Unknown error code";
}

void CMessageKeeper::setMessages(char const *texts[], int count)
{
  _texts =  texts;
  _count = count;
}

void CMessageKeeper::displayMessage(int code, MESSAGE_MODE mode, char const *text) const
{
  int delta = (text != NULL) ? 30 : 0;

  GRwnd wnd(170, 100, 450, 180 + delta, windowTitles[mode], windowColors[mode], 1, 1);
  button bt1(275, 155 + delta, 345, 171 + delta, -1, "Выход");
  wnd.text("");
  wnd.text(getMessage(code));
  if (text != NULL)
    wnd.text(text);
  Menu_but menu;
  menu + &bt1;
  menu.menu_key();
}

static CMessageKeeper gErrors, gWarnings;

void setWarningMessages(char const *texts[], int count)
{
  gWarnings.setMessages(texts, count);
}

void setErrorMessages(char const *texts[], int count)
{
  gErrors.setMessages(texts, count);
}

void displayError(int code, char const *text)
{
  gErrors.displayMessage(code, mmError, text);
}

void displayWarning(int code, char const *text)
{
  gWarnings.displayMessage(code, mmWarning, text);
}

