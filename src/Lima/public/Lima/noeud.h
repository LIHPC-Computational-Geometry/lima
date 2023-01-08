#ifndef _NOEUD_H
#define _NOEUD_H

#include "config.h"
#include "handle.h"
#include "size.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class _NoeudInterne;
class Bras;
class Polygone;
class Polyedre;

#ifndef SWIG
typedef _Handle<_NoeudInterne> Handle_NoeudInterne;
#endif

// Noeud du maillage.
class Noeud : public Handle_NoeudInterne
{
public:
  Noeud(double x=0, double y=0, double z=0);
#ifdef SWIG
  %name(Noeud_id)
#endif
  Noeud(size_type n, double x=0, double y=0, double z=0);

#ifndef SWIG
  Noeud(_NoeudInterne* ptr);
  Noeud(const Noeud& nd);
#endif
  ~Noeud();
    
  /// Coordonnée X du noeud.
  double          x() const;
  /// Coordonnée Y du noeud. 
  double          y() const;
  /// Coordonnée Z du noeud. 
  double          z() const;
    
  /// Coordonnée X du noeud. 
  void            set_x(double d);
  /// Coordonnée Y du noeud. 
  void            set_y(double d);
  /// Coordonnée Z du noeud. 
  void            set_z(double d);

  // Numero d'identification
  size_type       id() const;
    
  // Nombre de bras.
  size_type       nb_bras() const;
  // bras numéro n.
  Bras            bras(size_type n) const;
  // Nombre de polygones.
  size_type       nb_polygones() const;
  // Polygone numéro n.
  Polygone        polygone(size_type n) const;
  // Nombre de polyedres.
  size_type       nb_polyedres() const;
  // Polyedre numéro n.
  Polyedre        polyedre(size_type n) const;

#ifndef SWIG
  const Noeud& operator = (const Noeud& nd);
  bool operator == (const Noeud& nd);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
