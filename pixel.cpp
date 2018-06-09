
#include "pixel.h"

pixel::pixel()
{ x=0; y=0; d=0; }

pixel::pixel(int xp, int yp)
{ x=xp; y=yp; d=0; }

pixel::pixel(int xp, int yp, int dv)
{ x=xp; y=yp; d=dv; }
