#include "convert.hh"

static char digstr[DIGMAX + 2];    /* +1 for end of string         */
// static char ret[DIGMAX+2];

// void fastitoa(char* buf, int i)
// {
// }

int fastlltoa(char **buff, long long i)
{
  char *p = *buff;

	if(i < 0)
  {
  	p[0] = '-';
  	p++;
  	i *= -1;														// doesn't work with -9223372036854775808
  }
  if(!i)
  {
  	p[0] = '0'; p[1] = '\0';
  	return 1;
  }

  short aux;
  int j;
  for(aux = 4; aux >= 0; aux--)
  {
  	if(i >= divisors[aux])
  	{
  		j = i / divisors[aux];						// get first numbers
  		i -= j*divisors[aux];							// remove first numbers from entry number
  		unsigned short c = 4;							// trailing zero counter (number of non zero digits)
  		int k = *((int*)(&digits[j<<2]));	// get matching string for first numbers as an int
  		while((k & 255) == '0')						// while string starts with '0'
  		{
  			c--;														// one less non zero digit at start of string
				k >>= 8;												// remove initial '0'
  		}
  		*((int*)p) = k;										// copy resulting string to buffer (as integer)
  		p += c;														// jump to next position on buffer
  		break;
  	}
  }

  aux--;

  for(;aux >= 0; aux--)
  {
  	j = i / divisors[aux];
  	i -= j*divisors[aux];
  	*((int*)p) = *((int*)(&digits[j<<2]));
  	p += 4;
  }
  *(p) = '\0';
  return (p-*buff);
}

int fastulltoa(char **buff, unsigned long long i)
{
	char *p = *buff;

  if(!i)
  {
  	p[0] = '0'; p[1] = '\0';
  	return 1;
  }

  short aux;
  int j;
  for(aux = 4; aux >= 0; aux--)
  {
  	if(i >= divisors[aux])
  	{
  		j = i / divisors[aux];						// get first numbers
  		i -= j*divisors[aux];							// remove first numbers from entry number
  		unsigned short c = 4;							// trailing zero counter (number of non zero digits)
  		int k = *((int*)(&digits[j<<2]));	// get matching string for first numbers as an int
  		while((k & 255) == '0')						// while string starts with '0'
  		{
  			c--;														// one less non zero digit at start of string
				k >>= 8;												// remove initial '0'
  		}
  		*((int*)p) = k;										// copy resulting string to buffer (as integer)
  		p += c;														// jump to next position on buffer
  		break;
  	}
  }

  aux--;

  for(;aux >= 0; aux--)
  {
  	j = i / divisors[aux];
  	i -= j*divisors[aux];
  	*((int*)p) = *((int*)(&digits[j<<2]));
  	p += 4;
  }
  *(p)='\0';
  return (p-*buff);
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
// static double negtab[] =
//     { 1e-256, 1e-128, 1e-64, 1e-32, 1e-16, 1e-8, 1e-4, 1e-2, 1e-1, 1.0 };
// static double postab[] =
//     { 1e+256, 1e+128, 1e+64, 1e+32, 1e+16, 1e+8, 1e+4, 1e+2, 1e+1 };

// static char *_cvt();

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

// void convert(char **buff, double val,int prec)
// {
// 	unsigned int d = (val < 0) ? (-val) : val;
// 	// cout << d << endl;
// 	if(d)
// 	{
// 		int dec,sign;
// 		d = ceil(log10(d));
// 		// cout << d << endl;
// 		char *s = ecvt(val,d+prec,&dec,&sign);
// 		// sign = sign ? 1 : 0;
// 		// char r[d+sign+prec+2];
// 		if(sign)
// 			ret[0] = '-';
// 		memcpy(ret+sign,s,dec);
// 		ret[dec+sign] = '.';
// 		memcpy(ret+dec+sign+1,s+dec,prec);
// 		ret[d+sign+prec+1] = '\0';
// 		return ret;
// 	}
// 	else
// 	{
// 		ret[0] = '0'; ret [1] = '\0';
// 		return ret;
// 	}
// }

int convert(char **buff, unsigned long long val)
{
	return fastulltoa(buff,val);
}

int convert(char **buff, unsigned int val)
{
	return fastulltoa(buff,val);
}

int convert(char **buff, unsigned long val)
{
	return fastulltoa(buff,val);
}

int convert(char **buff, long long val)
{
	return fastlltoa(buff,val);
}

int convert(char **buff, int val)
{
	return fastlltoa(buff,val);
}