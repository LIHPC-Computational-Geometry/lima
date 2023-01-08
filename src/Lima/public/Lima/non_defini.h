#include <float.h>


BEGIN_NAMESPACE_LIMA

// Valeur d'un entier non defini :
#ifndef ENTIER_ND
#define ENTIER_ND INT_MAX
#endif

// Valeur d'un reel non defini :
#ifndef REEL_ND
#define REEL_ND DBL_MAX
#endif

// Valeur d'une chaine de caracteres non definie :
#ifndef CHAINE_ND
#define CHAINE_ND ""
#endif

// Valeur d'un vecteur 2D non defini :
#ifndef VECT2D_ND
#include "vect2d.h"
#define VECT2D_ND Vect2d(DBL_MAX,DBL_MAX)
#endif

// Valeur d'un vecteur 3D non defini :
#ifndef VECT3D_ND
#include "vect3d.h"
#define VECT3D_ND Vect3d(DBL_MAX,DBL_MAX,DBL_MAX)
#endif

END_NAMESPACE_LIMA
