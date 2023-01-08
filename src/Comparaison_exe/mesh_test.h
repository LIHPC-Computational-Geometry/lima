#ifndef MESH_TEST_H
#define MESH_TEST_H

#include "Lima/lima++.h"

#if !defined(_IBMR2) && !defined(CRAY) && !defined(__hpux)
using namespace Lima;
#else
#if __IBMCPP__ >= 500
using namespace Lima;
#endif  // #if __IBMCPP__ >= 500
#endif  // #if !defined(_IBMR2) && ...


void compare_attributs(const Liste_attribut& a, const Liste_attribut& b);
	// throw (...)
void compare_attribut(const Attribut& a, const Attribut& b);
	// throw (...)
void print_attribut(const Attribut& a);
void print_attributs(const Liste_attribut& a);
void compareMeshes (Maillage& mesh1, Maillage& mesh2);
	// throw (...):


#endif	// MESH_TEST_H
