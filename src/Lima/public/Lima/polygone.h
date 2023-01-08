#ifndef _POLYGONE_H
#define _POLYGONE_H

#include "config.h"
#include "handle.h"
#include "size.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Noeud;
class Bras;
class _PolygoneInterne;
class Polyedre;

#ifndef SWIG
typedef _Handle<_PolygoneInterne> Handle_PolygoneInterne;
#endif

// Polygone du maillage.
class Polygone : public Handle_PolygoneInterne
{
public:
  Polygone();
#ifdef SWIG
  %name(Triangle)
#endif
  Polygone(const Noeud& n0,  const Noeud& n1, const Noeud& n2);
#ifdef SWIG
  %name(Quadrangle)
#endif
  Polygone(const Noeud& n0,  const Noeud& n1, 
	   const Noeud& n2,  const Noeud& n3);
#ifdef SWIG
  %name(Pentagone)
#endif
  Polygone(const Noeud& n0,  const Noeud& n1, const Noeud& n2,
	   const Noeud& n3,  const Noeud& n4);
#ifdef SWIG
  %name(Hexagone)
#endif
  Polygone(const Noeud& n0,  const Noeud& n1, const Noeud& n2,
	   const Noeud& n3,  const Noeud& n4, const Noeud& n5);
#ifdef SWIG
  %name(Triangle_id)
#endif
  Polygone(size_type n, const Noeud& n0,  const Noeud& n1, const Noeud& n2);
#ifdef SWIG
  %name(Quadrangle_id)
#endif
  Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
	   const Noeud& n2,  const Noeud& n3);
#ifdef SWIG
  %name(Pentagone_id)
#endif
  Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
	   const Noeud& n2,  const Noeud& n3,  const Noeud& n4);
#ifdef SWIG
  %name(Hexagone_id)
#endif
  Polygone(size_type n, const Noeud& n0,  const Noeud& n1, 
	   const Noeud& n2,  const Noeud& n3,  const Noeud& n4,
	   const Noeud& n5);

#ifndef SWIG
  Polygone(_PolygoneInterne* ptr);
  Polygone(const Polygone& pg);
#endif
  ~Polygone();
  
  // Numero d'identification
  size_type       id() const;
  
  // Nombre de noeuds.
  size_type       nb_noeuds() const;
  // Noeud numéro n.
  Noeud           noeud(size_type n) const;
  // Nombre de bras.
  size_type       nb_bras() const;
  // Bras numéro n.
  Bras            bras(size_type n) const;
  // Nombre de polyedres.
  size_type       nb_polyedres() const;
  // Polyedre numéro n.
  Polyedre        polyedre(size_type n) const;
  
  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (const Noeud& n1, const Noeud& n2);

#ifndef SWIG
  const Polygone& operator = (const Polygone& pg);
  bool operator == (const Polygone& pg);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
