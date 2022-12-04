#include <bios.h>
#include <dos.h>
#include <memory.h>

#include "mouse.hpp"
#include "tpedit.hpp"
#include "button.hpp"

Menu_but::Menu_but()
{
  activ = first = NULL;
}

Menu_but Menu_but::operator+(button *but)
{
  button *item;
  if (first == NULL)
  {
    first = activ = but;
  }
  else
  {
    for (item = first; item->next != NULL; item = item->next)
      ;
    item->next = but;
  };
  but->next = NULL;
  return *this;
}

void Menu_but::draw()
{
  for (button *item = first; item != NULL; item = item->next)
    item->draw((item == activ) ? B_ACT | B_OFF : B_OFF);
}

int Menu_but::menu_key(void)
{
  draw();
  return menu_key2();
}

int Menu_but::menu_key2(void)
{
  int status, xcoor, ycoor, nbut, nbutold, bool, bol2, excod = -1;
  button *item;
  char *mous_buf = new char[mouse_size_buff()];
  union
  {
    int status2;
    char ch2[2];
  };
  mouse_show(OFF);
  {
    int xmin, ymin, xmax, ymax;
    xmin = first->left;
    ymin = first->top;
    xmax = first->right;
    ymax = first->bot;
    for (item = first->next; item != NULL; item = item->next)
    {
      if (xmin > item->left)
        xmin = item->left;
      if (ymin > item->top)
        ymin = item->top;
      if (xmax < item->right)
        xmax = item->right;
      if (ymax < item->bot)
        ymax = item->bot;
    };
    if (mous_buf != NULL)
      mouse_store(mous_buf);
    mouse_screen_size(xmin, ymin, xmax, ymax);
    set_position((activ->left + activ->right) / 2, (activ->top + activ->bot) / 2);
  }
  mouse_show(ON);
label1:
  do
  {
    bol2 = 0;
    bool = 0;
    status = 0;
    position_status(&status, &xcoor, &ycoor);
    status2 = bioskey(1);
  } while (((status & 3) == 0) && (status2 == 0));
  if (status & 2)
  {
    do
      position_status(&status, &xcoor, &ycoor);
    while (status);
    goto escexit;
  };
  if (status)
  {
    do
    {
      for (item = first; item != NULL; item = item->next)
      {
        nbut = 1;
        if (xyinregion(xcoor, ycoor, item))
          break;
        else
          nbut = 0;
      };
      if (!nbut)
      {
        if (!bol2)
          activ->draw(B_OFF | B_ACT);
        bol2 = 1;
        bool = 0;
        goto label2;
      };
      bol2 = 0;
      if (item != activ)
      {
        activ->draw(B_OFF);
        (activ = item)->draw(B_ON | B_ACT);
      };
      if (!bool)
      {
        bool = 1;
        activ->draw(B_ON | B_ACT);
      };
    label2:
      position_status(&status, &xcoor, &ycoor);
    } while (status);
    if (!nbut)
      goto label1;
    activ->draw(B_OFF | B_ACT);
    excod = activ->key;
    goto escexit;
  };
  if (status2)
  {
    while (bioskey(1))
      bioskey(0);
    int RSC = CallRScommand(&status2);
    switch (RSC)
    {
    case RSenter:
      activ->draw(B_ON | B_ACT);
      delay(200);
      activ->draw(B_OFF | B_ACT);
      excod = activ->key;
    case RSquit:
      goto escexit;
    case RSup:
    case RSleft:
    {
      button *b = (first != activ) ? activ : NULL;
      for (item = first; item->next != b; item = item->next)
        ;
      activ->draw(B_OFF);
      (activ = item)->draw(B_OFF | B_ACT);
      goto label1;
    }
    case RSdn:
    case RSright:
    {
      button *b = (activ->next != NULL) ? activ->next : first;
      activ->draw(B_OFF);
      (activ = b)->draw(B_OFF | B_ACT);
    };
    };
    goto label1;
  };
escexit:
  mouse_show(OFF);
  if (mous_buf != NULL)
  {
    mouse_restore(mous_buf);
    delete mous_buf;
  }
  mouse_show(ON);
  return excod;
}
