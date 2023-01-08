#ifndef XLMLIMA_CONFIG_H
#define XLMLIMA_CONFIG_H

#include "Lima/config.h"

#define BEGIN_NAMESPACE_XLMLIMA	namespace XLMLIMA {
#define END_NAMESPACE_XLMLIMA	}

#ifndef USING_LIMA
#define IN_LIMA		Lima::
#define USING_LIMA	using namespace Lima;
#endif	// USING_LIMA

#ifndef USING_STD
#define USING_STD	using namespace std;
#endif	// USING_STD

#ifndef IN_XLMLIMA
#define IN_XLMLIMA		XLMLIMA::
#define USING_XLMLIMA	using namespace XLMLIMA;
#endif	// IN_XLMLIMA

#endif	// XLMLIMA_CONFIG_H
