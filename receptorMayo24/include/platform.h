//And�n de v�a.
#include "mtfItem.h"

#ifndef  _MONTEFARO_PLATFORM_
#define _MONTEFARO_PLATFORM_

class platform : public mtfItem
{
public:
	int16_t location[2]; //Coordenadas del and�n.
	int16_t length; //Longitud.
};
#endif // ! _MONTEFARO_PLATFORM_
