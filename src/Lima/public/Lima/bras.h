#ifndef _BRAS_H
#define _BRAS_H

#include "config.h"
#include "handle.h"
#include "size.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Noeud;
class _BrasInterne;
class Polygone;
class Polyedre;

#ifndef SWIG
typedef _Handle<_BrasInterne> Handle_BrasInterne;
#endif

// Bras du maillage.
class Bras : public Handle_BrasInterne
{
public:
  Bras(const Noeud& n0,  const Noeud& n1);
#ifdef SWIG
  %name(Bras_id)
#endif
  Bras(size_type n, const Noeud& n0,  const Noeud& n1);

#ifndef SWIG
  Bras(_BrasInterne* ptr);
  Bras(const Bras& br);
  Bras();
#endif
  ~Bras();
  
  // Numero d'identification
  size_type       id() const;
  
  // Nombre de noeuds.
  size_type       nb_noeuds() const;
  // Noeud numéro n.
  Noeud           noeud(size_type n) const;
  // Nombre de polygones.
  size_type       nb_polygones() const;
  // Polygone numéro n.
  Polygone        polygone(size_type n) const;
  // Nombre de polyedres.
  size_type       nb_polyedres() const;
  // Polyedre numéro n.
  Polyedre        polyedre(size_type n) const;
  
  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (const Noeud& n1, const Noeud& n2);

#ifndef SWIG
  const Bras& operator = (const Bras& br);
  bool operator == (const Bras& br);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
