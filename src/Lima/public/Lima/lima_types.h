#ifndef _LIMA_TYPES_H
#define _LIMA_TYPES_H

/* EXPURGE_BEGINNING_TAG LOCAL_DEPENDENCIES */
/* EXPURGE_COMPLETION_TAG */

#ifndef _MACHINE_TYPES_H

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

#endif	/* _MACHINE_TYPES_H */

#endif	/* _LIMA_TYPES_H */
