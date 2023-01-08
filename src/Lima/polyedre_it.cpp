#include <Lima/config.h>
#include "LimaP/polyedre_it.h"
#include "LimaP/pyramide_it.h"
#include "LimaP/tetraedre_it.h"
#include "LimaP/prisme_it.h"
#include "LimaP/hexaedre_it.h"
#include "LimaP/heptaedre_it.h"
#include "LimaP/octoedre_it.h"

BEGIN_NAMESPACE_LIMA

#ifndef LIMA_MT
size_type _PolyedreInterne::m_max_id = 0;
#endif	// LIMA_MT


_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3)
{
  return new _TetraedreInterne(n0, n1, n2, n3);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3)
{
  return new _TetraedreInterne(n, n0, n1, n2, n3);
}

_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3, 
			 _NoeudInterne* n4)
{
  return new _PyramideInterne(n0, n1, n2, n3, n4);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4)
{
  return new _PyramideInterne(n, n0, n1, n2, n3, n4);
}

_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3, 
			 _NoeudInterne* n4, _NoeudInterne* n5)
{
  return new _PrismeInterne(n0, n1, n2, n3, n4, n5);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5)
{
  return new _PrismeInterne(n, n0, n1, n2, n3, n4, n5);
}

_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3, 
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7,
			 _NoeudInterne* n8, _NoeudInterne* n9)
{
  return new _HeptaedreInterne(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7,
			 _NoeudInterne* n8, _NoeudInterne* n9)
{
  return new _HeptaedreInterne(n, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9);
}

_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3, 
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7,
			 _NoeudInterne* n8, _NoeudInterne* n9,
			 _NoeudInterne* n10, _NoeudInterne* n11)
{
  return new _OctoedreInterne(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7,
			 _NoeudInterne* n8, _NoeudInterne* n9,
			 _NoeudInterne* n10, _NoeudInterne* n11)
{
  return new _OctoedreInterne(n, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11);
}

_PolyedreInterne* 
_PolyedreInterne::create(_NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3, 
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7)
{
  return new _HexaedreInterne(n0, n1, n2, n3, n4, n5, n6, n7);
}

_PolyedreInterne* 
_PolyedreInterne::create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
			 _NoeudInterne* n2, _NoeudInterne* n3,
			 _NoeudInterne* n4, _NoeudInterne* n5,
			 _NoeudInterne* n6, _NoeudInterne* n7)
{
  return new _HexaedreInterne(n, n0, n1, n2, n3, n4, n5, n6, n7);
}


void _PolyedreInterne::remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2)
{
	const size_type	nombre	= m_noeuds.size ( );

	for (size_type i = 0; i < nombre; i++)
		if (m_noeuds [i] == n1)
			m_noeuds [i]	= n2;
}	// _PolyedreInterne::remplacer_noeud

END_NAMESPACE_LIMA

