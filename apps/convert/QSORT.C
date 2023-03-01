#pragma  inline
#include <stdlib.h>


typedef int _CType comparF (const void _FAR *, const void _FAR *);
#   define  _LT_(x,y)  (x < y)
#   define  LES_  LES
#   define  ES_   ES:
#   define  LDS_  LDS

static  comparF    *Fcmp;
static  unsigned    qWidth;
static  void  near pascal  Exchange (void  huge*leftP, void huge*rightP) {
asm     push    DS
asm     cld
asm     mov     cx, qWidth
asm     LES_    di, rightP
asm     LDS_    si, leftP
asm     shr     cx, 1                   /* test for an odd number of bytes */
asm     jnc     xch_wordLoop

asm     mov     al, ES_ [di]
asm     movsb                           /* swap bytes, advancing pointers */
asm     mov     [si-1], al
asm     jz      xch_end                 /* if CX was originally 1       */

xch_wordLoop:
asm     mov     ax, ES_ [di]
asm     movsw                           /* swap words, advancing pointers */
asm     mov     [si-2], ax
asm     loop    xch_wordLoop

xch_end:
asm     pop     DS
    return;
}

char huge *addP(void huge *p,long a) {
 asm	{
	 mov	ax,word ptr p
	 mov	dx,word ptr p[2]
	 mov	cl,4
	 rol	dx,cl
	 mov	bx,dx
	 and	bx,0xfff0
	 add	ax,bx
	 xor	dx,bx
	 mov	bx,word ptr a
	 mov	cx,word ptr a[2]
	 add	ax,bx
	 adc	dx,cx
	 push	ax
	 mov	cl,4
	 ror	dx,cl
	 shr	ax,cl
	 add	dx,ax
	 pop	ax
	 and	ax,0xf
 }
}


static void  near pascal  qSortHelp (char huge *pivotP, size_t nElem)
{
    char    huge *leftP, huge *rightP;
    char    huge *pivotEnd, huge *pivotTemp, huge *leftTemp;
    unsigned  lNum;
    int       retval;
tailRecursion:
    if (nElem <= 2)
	{
	if (nElem == 2)
	    {
	    if (Fcmp (pivotP, rightP = qWidth + pivotP) > 0)
		Exchange (pivotP, rightP);
	    }
	return;
	}
    rightP = addP( pivotP,((long)(nElem - 1)) * qWidth);
    leftP  = addP( pivotP,((long)(nElem >> 1)) * qWidth);
/*  sort the pivot, left, and right elements for "median of 3" */
    if (Fcmp (leftP, rightP) > 0)
	Exchange (leftP, rightP);
    if (Fcmp (leftP, pivotP) > 0)
	Exchange (leftP, pivotP);
    else if (Fcmp (pivotP, rightP) > 0)
	Exchange (pivotP, rightP);
    if (nElem == 3)
	{
	Exchange (pivotP, leftP);
	return;
	}
/*  now for the classic Hoare algorithm */
    leftP = pivotEnd = pivotP + qWidth;
    do
	{
	while ((retval = Fcmp(leftP, pivotP)) <= 0)
	    {
	    if (retval == 0)
		{
		Exchange(leftP, pivotEnd);
		pivotEnd += qWidth;
		}
	    if (_LT_ (leftP, rightP))
		leftP += qWidth;
	    else
		goto qBreak;
	    }
	while (_LT_(leftP, rightP))
	    {
	    if ((retval = Fcmp(pivotP, rightP)) < 0)
		rightP -= qWidth;
	    else
		{
		Exchange (leftP, rightP);
		if (retval != 0)
		    {
		    leftP += qWidth;
		    rightP -= qWidth;
		    }
		break;
		}
	    }
	}   while (_LT_(leftP, rightP));

qBreak:

    if (Fcmp(leftP, pivotP) <= 0)
	leftP = leftP + qWidth;
    leftTemp = leftP - qWidth;
    pivotTemp = pivotP;
    while ((pivotTemp < pivotEnd) && (leftTemp >= pivotEnd))
	{
	Exchange(pivotTemp, leftTemp);
	pivotTemp += qWidth;
	leftTemp -= qWidth;
	}
    lNum = (leftP - pivotEnd) / qWidth;
    nElem = (addP(pivotP,((long)nElem) * qWidth) - leftP)/qWidth;
    // Sort smaller partition first to reduce stack usage
    if (nElem < lNum)
	{
	qSortHelp (leftP, nElem);
	nElem = lNum;
	}
    else
	{
	qSortHelp (pivotP, lNum);
	pivotP = leftP;
	}
    goto tailRecursion;
}

#pragma warn .sig
#if defined(__HUGE__) || defined(__FARFUNCS__)
#pragma warn .sus
#endif

#if defined(__FARFUNCS__)
#include <_farfunc.h>
#endif
void  _CType _FARFUNC qsort (void _FAR * baseP, size_t nElem, size_t width,
			     comparF *compar)
{
    char huge *bs;
    bs=baseP;
    if ((qWidth = width) == 0)
	return;
    Fcmp = compar;
    qSortHelp (bs, nElem);
}
