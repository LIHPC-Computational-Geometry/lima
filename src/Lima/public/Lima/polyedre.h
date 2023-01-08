#ifndef _POLYEDRE_H
#define _POLYEDRE_H

#include "config.h"
#include "handle.h"
#include "size.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Noeud;
class Bras;
class Polygone;
class _PolyedreInterne;

#ifndef SWIG
typedef _Handle<_PolyedreInterne> Handle_PolyedreInterne;
#endif

// Polyedre du maillage.
class Polyedre : public Handle_PolyedreInterne
{
public:
#ifndef SWIG
  typedef enum {
    TETRAEDRE = 0, 
    PYRAMIDE,
    PRISME,
    HEXAEDRE,
    HEPTAEDRE,
    OCTOEDRE,
    maxPolyedreType
  } PolyedreType;

  // Voir polyedre.C pour la definition du nombre de noeuds suivant le 
  // type de polyedre.
  static int PolyedreNbNode[maxPolyedreType];
#endif

  Polyedre();
#ifdef SWIG
  %name(Tetraedre)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3);
#ifdef SWIG
  %name(Pyramide)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1, const Noeud& n2,
	   const Noeud& n3,  const Noeud& n4);
#ifdef SWIG
  %name(Prisme)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5);
#ifdef SWIG
  %name(Hexaedre)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7);
#ifdef SWIG
  %name(Heptaedre)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7,
	   const Noeud& n8,  const Noeud& n9);
#ifdef SWIG
  %name(Octoedre)
#endif
  Polyedre(const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7,
	   const Noeud& n8,  const Noeud& n9,
	   const Noeud& n10,  const Noeud& n11);
#ifdef SWIG
  %name(Tetraedre_id)
#endif
  Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3);
#ifdef SWIG
  %name(Prisme_id)
#endif
  Polyedre(size_type n, const Noeud& n0, const Noeud& n1, 
	   const Noeud& n2, const Noeud& n3,  const Noeud& n4);
#ifdef SWIG
  %name(Pyramide_id)
#endif
  Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5);
#ifdef SWIG
  %name(Hexaedre_id)
#endif
  Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7);
#ifdef SWIG
  %name(Heptaedre_id)
#endif
  Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7,
	   const Noeud& n8,  const Noeud& n9);
#ifdef SWIG
  %name(Octoedre_id)
#endif
  Polyedre(size_type n, const Noeud& n0,  const Noeud& n1,
	   const Noeud& n2,  const Noeud& n3,
	   const Noeud& n4,  const Noeud& n5,
	   const Noeud& n6,  const Noeud& n7,
	   const Noeud& n8,  const Noeud& n9,
	   const Noeud& n10,  const Noeud& n11);

  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (const Noeud& n1, const Noeud& n2);

#ifndef SWIG
  Polyedre(_PolyedreInterne* ptr);
  Polyedre(const Polyedre& pd);
#endif
  ~Polyedre();
  
  // Numero d'identification
  size_type       id() const;
  
  // Type de polyedre
  Polyedre::PolyedreType  type() const;

  // Nombre de noeuds.
  size_type       nb_noeuds() const;
  // Noeud numéro n.
  Noeud           noeud(size_type n) const;
  // Nombre de bras.
  size_type       nb_bras() const;
  // Bras numéro n.
  Bras            bras(size_type n) const;
  // Nombre de polygones.
  size_type       nb_polygones() const;
  // Polygone numéro n.
  Polygone        polygone(size_type n) const;
  
  double          volume() const;

#ifndef SWIG
  const Polyedre& operator = (const Polyedre& pd);
  bool operator == (const Polyedre& pd);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
