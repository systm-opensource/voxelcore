
#include "color.h"

color::color()
{ r=0; g=0; b=0; a=255; }

color::color(int rv, int gv, int bv)
{ r=rv; g=gv; b=bv; a=255; }

color::color(int rv, int gv, int bv, int av)
{ r=rv; g=gv; b=bv; a=av; }
