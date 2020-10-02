

#ifndef KCLIB_KCIMAGE_H
#define KCLIB_KCIMAGE_H

#include "kcBaseClass.h"


// Image base class
class kcImage : kcClassBase
{

};

// HG-2 CatSystem image
class kcImageHg2 : kcImage
{

};

// HG-3 CatSystem2 image
class kcImageHg3 : kcImage
{

};

// PNG image
class kcImagePng : kcImage
{

};

// JPEG image
class kcImageJpeg : kcImage
{

};

// Photoshop image
class kcImagePsd : kcImage
{

};

// WebP image
class kcImageWebp : kcImage
{

};

// KISS texture image(?)  (for cel-shaded 3d models)
class kcImageKcf : kcImage
{

};

#endif  /* KCLIB_KCIMAGE_H */
