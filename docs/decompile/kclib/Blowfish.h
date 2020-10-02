

#ifndef KCLIB_BLOWFISH_H
#define KCLIB_BLOWFISH_H

#include "kcBaseClass.h"


// Blowfish cipher
class Blowfish : kcClassBase
{
  unsigned long* PArray;
  unsigned long* SBoxes;
};


#endif  /* KCLIB_BLOWFISH_H */
