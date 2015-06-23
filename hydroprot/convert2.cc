/* Copyright (C) 1989, 1990, 1991, 1992 Free Software Foundation, Inc.
     Written by James Clark (jjc@jclark.com)

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with groff; see the file COPYING.  If not, write to the Free Software
Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */

#define DIGMAX       30            /* max # of digits in string */
#define DIGPREC      17            /* max # of significant digits */
#define ECVT  false
#define FCVT  true
static char digstr[DIGMAX + 2];    /* +1 for end of string         */

static char ret[DIGMAX+2];

#define INT_DIGITS 19		/* enough for 64 bit integer */

void fastitoa(char** buf, int i) {

}

const long long divisors[] = {
	10000,
	100000000,
	1000000000000,
	10000000000000000
};

const int SIZE = 10000;
const int digits[SIZE];

for (int i = 0; i < SIZE; i++) { 
  digits[i] =
		('0' + i / 1000) << 24 +
		('0' + i / 1000) << 16 +
		('0' + i / 1000) << 8 +
		'0' + i / 1000;
 }

"0000000100020003000400050006000700080009" 
	"0010001100120013001400150016001700180019" ;


void fastlltoa(char** buf, long long i) {
  /* Room for INT_DIGITS digits, - and '\0' */
  // static char buf[INT_DIGITS + 2];

	// find first bit
	//https://en.wikipedia.org/wiki/Find_first_set
	// pos = 2^k
	// find equivalent 10^k


	if (k > 32) {

		if (k > 48) {



		} else {



		}
	} else {
		if (k > 16) {



		} else {


		}
	}
	long long j = i / div; // print j
	i = i - j * div;  //TODO what about negative case?
	j = i / div2; // print out next four digits
  *((int*)p) = digits[j]
	p += 4;
  char *p = *buf;
  if (i >= 0) {
    do {
      *--p = '0' + (i % 10);
      i /= 10;
    } while (i != 0);
    return p;
  }
  else {			/* i < 0 */
		*p++ = '-';
    do {
      *--p = '0' - (i % 10);
      i /= 10;
    } while (i != 0);
    *--p = '-';
  }
  return p;
}

void fastulltoa(unsigned long long i) {
  /* Room for INT_DIGITS digits, - and '\0' */
  // static char buf[INT_DIGITS + 3];
  char *p = ret + INT_DIGITS + 2;	/* points to terminating '\0' */
  do {
    *--p = '0' + (i % 10);
    i /= 10;
  } while (i != 0);
  return p;
}

// http://sourcecodebrowser.com/linux86/0.16.17/ecvt_8c.html

// #define DIGMAX       30            /* max # of digits in string */
// #define DIGPREC      17             max # of significant digits 
// #define ECVT  false
// #define FCVT  true
// static char digstr[DIGMAX + 1 + 1];    /* +1 for end of string         */

// static char ret[DIGMAX+2];

    /* +1 in case rounding adds     */
    /* another digit                */
static double negtab[] =
    { 1e-256, 1e-128, 1e-64, 1e-32, 1e-16, 1e-8, 1e-4, 1e-2, 1e-1, 1.0 };
static double postab[] =
    { 1e+256, 1e+128, 1e+64, 1e+32, 1e+16, 1e+8, 1e+4, 1e+2, 1e+1 };

static char *_cvt();

/*************************
 * Convert double val to a string of
 * decimal digits.
 *     ndig = # of digits in resulting string
 * Returns:
 *     *pdecpt = position of decimal point from left of first digit
 *     *psign  = nonzero if value was negative
 */

static char *
_cvt(bool cnvflag, double val, int ndig, int *pdecpt, int *psign)
// boolean cnvflag, double val, int ndig, int *pdecpt, int *psign;
{
	int   decpt, pow, i;
	char *p;
	*psign = (val < 0) ? ((val = -val), 1) : 0;
	ndig = (ndig < 0) ? 0 : (ndig < DIGMAX) ? ndig : DIGMAX;
	if (val == 0)
	{
	  for (p = &digstr[0]; p < &digstr[ndig]; p++)
	    *p = '0';
	  decpt = 0;
	}
	else
	{
	  /* Adjust things so that 1 <= val < 10  */
	  /* in these loops if val == MAXDOUBLE)  */
	  decpt = 1;
	  pow = 256;
	  i = 0;
	  while (val < 1)
	  {
	    while (val < negtab[i + 1])
	    {
	       val /= negtab[i];
	       decpt -= pow;
	    }
	    pow >>= 1;
	    i++;
	  }
	  pow = 256;
	  i = 0;
	  while (val >= 10)
	  {
	    while (val >= postab[i])
	    {
	       val /= postab[i];
	       decpt += pow;
	    }
	    pow >>= 1;
	    i++;
	  }
	  if (cnvflag == FCVT)
	  {
	    ndig += decpt;
	    ndig = (ndig < 0) ? 0 : (ndig < DIGMAX) ? ndig : DIGMAX;
	  }

	  /* Pick off digits 1 by 1 and stuff into digstr[]       */
	  /* Do 1 extra digit for rounding purposes               */
	  for (p = &digstr[0]; p <= &digstr[ndig]; p++)
	  {
	    int   n;

	    /* 'twould be silly to have zillions of digits  */
	    /* when only DIGPREC are significant            */
	    if (p >= &digstr[DIGPREC])
	       *p = '0';

	    else
	    {
	       n = val;
	       *p = n + '0';
	       val = (val - n) * 10;   /* get next digit */
	    }
	  }
	  if (*--p >= '5')
	  {    /* if we need to round              */
	    while (1)
	    {
	       if (p == &digstr[0])
	       {  /* if at start      */
	          ndig += cnvflag;
	          decpt++;             /* shift dec pnt */
	          digstr[0] = '1';     /* "100000..." */
	          break;
	       }
	       *p = '0';
	       --p;
	       if (*p != '9') {
	          (*p)++;
	          break;
	       }
	    }                   /* while */
	  }                     /* if */
	}                       /* else */
	*pdecpt = decpt;
	digstr[ndig] = 0;        /* terminate string             */
	return &digstr[0];
}

char *
ecvt(double val, int ndig, int *pdecpt,int *psign)
// double val, int ndig, int *pdecpt,int *psign;
{
   return _cvt(ECVT, val, ndig, pdecpt, psign);
}

#include <cmath>
#include <iostream>
using namespace std;

char *
convert(double val,int prec)
{
	unsigned int d = (val < 0) ? (-val) : val;
	// cout << d << endl;
	if(d)
	{
		int dec,sign;
		d = ceil(log10(d));
		// cout << d << endl;
		char *s = ecvt(val,d+prec,&dec,&sign);
		// sign = sign ? 1 : 0;
		// char r[d+sign+prec+2];
		if(sign)
			ret[0] = '-';
		memcpy(ret+sign,s,dec);
		ret[dec+sign] = '.';
		memcpy(ret+dec+sign+1,s+dec,prec);
		ret[d+sign+prec+1] = '\0';
		return ret;
	}
	else
	{
		ret[0] = '0'; ret [1] = '\0';
		return ret;
	}
}

char *
convert(unsigned long long val)
{
	return ulitoa(val);
}

char *
convert(unsigned int val)
{
	return ulitoa(val);
}

char *
convert(unsigned long val)
{
	return ulitoa(val);
}

char *
convert(long long val)
{
	return litoa(val);
}

char *
convert(int val)
{
	return litoa(val);
}
