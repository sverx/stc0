#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;

__declspec(dllexport) const char* getName() {
	return "Simple Tile Compressor Zero";
}

__declspec(dllexport) const char* getExt() {
	return "stc0compr";
}

__declspec(dllexport) int compressTiles(const uint8_t* pSource, const uint32_t numTiles, uint8_t* pDestination, const uint32_t destinationLength) {

  if (destinationLength<(numTiles*40+1))
    return (0);                 // please give me more space for the data (up to 40 bytes per tile needed worst case scenario, plus 1 byte terminator)

  unsigned int tilenum;
  unsigned char i,j,mask,lastvalue;
  bool lastvalid;
  unsigned char values[4];
  unsigned char valuescount;
  unsigned int finalsize=0;

  for (tilenum=0;tilenum<numTiles;tilenum++) {  // loop over all tiles
    for (i=0;i<8;i++) {                         // loop over all the 8 rows of the tile
      mask=0b00000000;
      lastvalid=false;
      valuescount=0;

      if (pSource[0]==0x00)             mask|=0b10000000;
      else if (pSource[0]==0xFF)        mask|=0b11000000;
      else {
                                        mask|=0b01000000;  // raw
        lastvalid=true;
        lastvalue=pSource[0];
        values[valuescount++]=pSource[0];
      }

      if (pSource[1]==0x00)             mask|=0b00100000;
      else if (pSource[1]==0xFF)        mask|=0b00110000;
      else if ((lastvalid==false) ||
               (pSource[1]!=lastvalue)) {
                                        mask|=0b00010000;  // raw
        lastvalid=true;
        lastvalue=pSource[1];
        values[valuescount++]=pSource[1];
      }

      if (pSource[2]==0x00)             mask|=0b00001000;
      else if (pSource[2]==0xFF)        mask|=0b00001100;
      else if ((lastvalid==false) ||
               (pSource[2]!=lastvalue)) {
                                        mask|=0b00000100;  // raw
        lastvalid=true;
        lastvalue=pSource[2];
        values[valuescount++]=pSource[2];
      }

      if (pSource[3]==0x00)             mask|=0b00000010;
      else if (pSource[3]==0xFF)        mask|=0b00000011;
      else if ((lastvalid==false) ||
               (pSource[3]!=lastvalue)) {
                                        mask|=0b00000001;  // raw
        lastvalid=true;
        lastvalue=pSource[3];
        values[valuescount++]=pSource[3];
      }

      *pDestination++=mask;             // write mask byte

      for (j=0;j<valuescount;j++)
        *pDestination++=values[j];      // dump uncompressed values

      finalsize+=(1+valuescount);       // add this to finalsize

      pSource+=4;                       // skip to next 4 bytes
    }
  }

  *pDestination=0x00;                   // end of data
  finalsize++;

  return (finalsize);                   // report size to caller
}
