/*--------------------------------------------------------------------------------------*/
// File: hi240.cpp
// Desription: Hi240 class implementation
// Author: Ben Bridgwater
// History:
// 09/16/00 Initial revision.
/*--------------------------------------------------------------------------------------*/

#include "hi240.h"

/*--------------------------------------------------------------------------------------*/

void Hi240BuildPalette(long numberOfColors, Hi240StorePaletteEntry *storeEntry, void *p)
{
  const int nRed      = 5;
  const int nGreen    = 9;
  const int nBlue     = 5;
  const int cubeStart = 16;

  long rSpace, gSpace, bSpace;
  int  index, r, g, b;


  rSpace = numberOfColors / (nRed - 1);
  gSpace = numberOfColors / (nGreen - 1);
  bSpace = numberOfColors / (nBlue - 1);

  // Blue varies fastest, then red, then green.

  index = cubeStart;
  for (g = 0; g < nGreen; g++) {
    for (r = 0; r < nRed; r++) {
      for (b = 0; b < nBlue; b++) {
	storeEntry(p, index,
	  (r * rSpace) - ((r == 0) ? 0 : 1),
	  (g * gSpace) - ((g == 0) ? 0 : 1),
	  (b * bSpace) - ((b == 0) ? 0 : 1)
	  );
	index++;
      } // end for
    } // end for
  } // end for

} // end Hi240BuildPalette

/*--------------------------------------------------------------------------------------*/

