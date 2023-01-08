#ifndef _SIZE_H
#define _SIZE_H

#include <stddef.h>
#include "config.h"
#include "Lima/lima_types.h"

#ifndef __INTERNE_MACHINE_TYPES

#include <stdint.h>
#ifndef MALIPP_UINT_TYPES
#define MALIPP_UINT_TYPES
typedef uint32_t    u_int_4;
typedef int32_t     int_4;
typedef int16_t     int_2;
#endif	// MALIPP_UINT_TYPES

#endif	// __INTERNE_MACHINE_TYPES

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

//! Paramétrisation du type pour indiquer l'indice d'un tableau.
typedef size_t size_type;

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
