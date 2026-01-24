#include <stdlib.h>
#include "button.hpp"

enum MESSAGE_MODE
{
  mmError,
  mmWarning
};

static PCSTR windowTitles[] =
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
 PCSTR *_texts;
 int _count;

 PCSTR getMessage(int aCode) const;
public:
 CMessageKeeper();

 void displayMessage(int code, MESSAGE_MODE mode, PCSTR text) const;
 void setMessages(PCSTR texts[], int count);
};

CMessageKeeper::CMessageKeeper()
{
  setMessages(NULL, 0);
}

PCSTR CMessageKeeper::getMessage(int aCode) const
{
  if ((aCode >=0) && (aCode < _count))
    return _texts[aCode];
  else
    return "Unknown error code";
}

void CMessageKeeper::setMessages(PCSTR texts[], int count)
{
  _texts =  texts;
  _count = count;
}

void CMessageKeeper::displayMessage(int code, MESSAGE_MODE mode, PCSTR text) const
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

void setWarningMessages(PCSTR texts[], int count)
{
  gWarnings.setMessages(texts, count);
}

void setErrorMessages(PCSTR texts[], int count)
{
  gErrors.setMessages(texts, count);
}

void displayError(int code, PCSTR text)
{
  gErrors.displayMessage(code, mmError, text);
                                                                }

void displayWarning(int code, PCSTR text)
{
  gWarnings.displayMessage(code, mmWarning, text);
}

