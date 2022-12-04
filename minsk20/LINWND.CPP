int get_p(int u, int u1, int u2, int v1, int v2)
{
  long du1 = u - u1;
  long du2 = u2 - u1;
  long dv = v2 - v1;
  long np = du1 * dv / du2;
  return v1 + np;
  // return(v1+(u-u1)*(v2-v1)/(u2-u1));
}

int check(int *x1, int *y1, int *x2, int *y2,
          int Xmin, int Ymin, int Xmax, int Ymax)
{
  int xmn, ymn, xmx, ymx;
  if (*x1 < *x2)
  {
    xmn = *x1;
    xmx = *x2;
  }
  else
  {
    xmn = *x2;
    xmx = *x1;
  }
  if (*y1 < *y2)
  {
    ymn = *y1;
    ymx = *y2;
  }
  else
  {
    ymn = *y2;
    ymx = *y1;
  }
  if ((xmn >= Xmin) && (ymn >= Ymin) && (xmx <= Xmax) && (ymx <= Ymax))
    return 0;
  if ((xmn > Xmax) || (ymn > Ymax) || (xmx < Xmin) || (ymx < Ymin))
    return -1;
  return 1;
}

int lin_wnd(int *x1, int *y1, int *x2, int *y2,
            int Xmin, int Ymin, int Xmax, int Ymax)
{
  int fl, fl0 = 0;
  while ((fl = check(x1, y1, x2, y2, Xmin, Ymin, Xmax, Ymax)) == 1)
  {
    if (*x1 < Xmin)
    {
      *y1 = get_p(Xmin, *x1, *x2, *y1, *y2);
      *x1 = Xmin;
    }
    else if (*x1 > Xmax)
    {
      *y1 = get_p(Xmax, *x1, *x2, *y1, *y2);
      *x1 = Xmax;
    }
    else if (*y1 < Ymin)
    {
      *x1 = get_p(Ymin, *y1, *y2, *x1, *x2);
      *y1 = Ymin;
    }
    else if (*y1 > Ymax)
    {
      *x1 = get_p(Ymax, *y1, *y2, *x1, *x2);
      *y1 = Ymax;
    }
    else if (*x2 < Xmin)
    {
      *y2 = get_p(Xmin, *x2, *x1, *y2, *y1);
      *x2 = Xmin;
      fl0 = 1;
    }
    else if (*x2 > Xmax)
    {
      *y2 = get_p(Xmax, *x2, *x1, *y2, *y1);
      *x2 = Xmax;
      fl0 = 1;
    }
    else if (*y2 < Ymin)
    {
      *x2 = get_p(Ymin, *y2, *y1, *x2, *x1);
      *y2 = Ymin;
      fl0 = 1;
    }
    else if (*y2 > Ymax)
    {
      *x2 = get_p(Ymax, *y2, *y1, *x2, *x1);
      *y2 = Ymax;
      fl0 = 1;
    }
  }
  return (fl0) ? fl0 : fl;
}
