#ifndef _LIMA_TYPES_H
#define _LIMA_TYPES_H

#ifdef __INTERNE_MACHINE_TYPES

#include <machine_types.h>

#else	/* __INTERNE_MACHINE_TYPES */

#ifndef LIMA_INT_TYPE
#define LIMA_INT_TYPE
#include <stdint.h>
#ifdef INTEGER_32
typedef int32_t int_type;
#endif	/* INTEGER_32 */
#ifdef INTEGER_64
typedef int64_t int_type;
#endif	/* INTEGER_64 */

#endif	/* LIMA_INT_TYPE */

#endif	/* __INTERNE_MACHINE_TYPES */

#endif	/* _LIMA_TYPES_H */
