/* ************************************************************************** */
/* ** Data Conversion Functions Hex <-> Chars **/
/* ************************************************************************** */

#include <ctype.h>

unsigned char Hex2Char(unsigned char HexHigh, unsigned char HexLow)
{
  unsigned char CharValue;

  if (HexHigh==' ')
  {
    HexHigh='0';
  }
  HexHigh = toupper(HexHigh);
  HexLow  = toupper(HexLow);

  if ((isxdigit(HexHigh)) && (isxdigit(HexLow)))
  {
    CharValue = toupper(HexHigh) - (((HexHigh)<'A') ? '0' : ('A'-10));
    CharValue = CharValue<<4;
    CharValue += (toupper(HexLow) - (((HexLow)<'A') ? '0' : ('A'-10)));
    return (CharValue);
  }
  return(0);
}

unsigned char Char2HexHigh(unsigned char CharValue)
{
  CharValue = (CharValue & 0xF0) >> 4;
  if (CharValue<10)
  {
    CharValue += '0';
  } else {
    CharValue += ('A'-10);
  }
  return (CharValue);
}

unsigned char Char2HexLow(unsigned char CharValue)
{
  CharValue = (CharValue & 0x0F);

  if (CharValue<0x0A)
  {
    CharValue += '0';
  } else {
    CharValue += ('A'-10);
  }
  return (CharValue);
}
