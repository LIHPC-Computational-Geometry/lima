#ifndef _LIGNE_H
#define _LIGNE_H

#include "config.h"
#include "handle.h"
#include "size.h"
#include "composition.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Bras;
class Noeud;
class _LigneInterne;
class Nuage;
class Liste_attribut;  

#ifndef SWIG
typedef _Handle<_LigneInterne> Handle_LigneInterne;
#endif

// Ensemble de bras.
class Ligne : public Handle_LigneInterne
{
public:
#ifndef SWIG
  Ligne(const IN_STD string& nom = "");
#else
  Ligne(const char* nom = "");
#endif
#ifndef SWIG
  Ligne(_LigneInterne* ptr);
  Ligne(const Ligne& li); 
#endif
  ~Ligne();

  Composition composition ( ) const;
  
  void                ajouter(const Bras& br);
  void                retirer(const Bras& br);
  
  /// Nombre de bras.
  size_type           nb_bras() const;
  /// Nombre de noeuds.
  size_type           nb_noeuds() const;
  
  const IN_STD string&       nom() const;
#ifndef SWIG
  void                nommer(const IN_STD string& str);  
#else
  void                nommer(const char* str);  
#endif
  
  // Bras numéro n.
  Bras                bras(size_type n) const;
  // Bras d'ID n.
  Bras                bras_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_bras)
#endif
  bool                contenir(const Bras& br) const;
  //  Liste des attributs des bras.
  Liste_attribut&     att_bras();
  // Initialise les listes d'attributs de la ligne.
  void                init_att();

  
  // Noeud numéro n.
  Noeud               noeud(size_type n) const;
  // Noeud d'ID n.
  Noeud               noeud_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_noeud)
#endif
  bool                contenir(const Noeud& nd) const;
  //  Liste des attributs des noeuds.
  Liste_attribut&     att_noeuds();
  
  Nuage               contour() const;
  size_type           longueur_i() const;
  
#ifndef SWIG
  const Ligne& operator = (const Ligne& li);
  bool operator == (const Ligne& li);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
