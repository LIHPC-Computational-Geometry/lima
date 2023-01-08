#ifndef _NUAGE_H
#define _NUAGE_H

#include "config.h"
#include "handle.h"
#include "size.h"
#include "composition.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Noeud;
class _NuageInterne;
class Liste_attribut;  

#ifndef SWIG
typedef _Handle<_NuageInterne> Handle_NuageInterne;
#endif

// Ensemble de noeuds.
class Nuage : public Handle_NuageInterne
{
public:
#ifndef SWIG
  Nuage(const IN_STD string& nom = "");
#else
  Nuage(const char* nom = "");
#endif
#ifndef SWIG
  Nuage(_NuageInterne* ptr);
  Nuage(const Nuage& nu); 
#endif
  ~Nuage();

  Composition composition ( ) const;
  
  void                ajouter(const Noeud& nd);
  void                retirer(const Noeud& nd);
  
  /// Nombre de noeuds.
  size_type           nb_noeuds() const;
  
  const IN_STD string&       nom() const;
#ifndef SWIG
  void                nommer(const IN_STD string& str);  
#else
  void                nommer(const char* str);  
#endif
  
  // Noeud numéro n.
  Noeud               noeud(size_type n) const;
  // Noeud d'ID n.
  Noeud               noeud_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_noeud)
#endif
  bool                contenir(const Noeud& nd) const;
  //  Liste des attributs des noeuds.
  Liste_attribut&         att_noeuds();
  // Initialise la liste d'attributs du nuage.
  void                    init_att();

#ifndef SWIG
  const Nuage& operator = (const Nuage& nu);
  bool operator == (const Nuage& nu);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
