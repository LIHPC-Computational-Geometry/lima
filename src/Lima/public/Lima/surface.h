#ifndef _SURFACE_H
#define _SURFACE_H

#include "config.h"
#include "handle.h"
#include "size.h"
#include "composition.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Polygone;
class Bras;
class Noeud;
class _SurfaceInterne;
class Ligne;
class Liste_attribut;  

#ifndef SWIG
typedef _Handle<_SurfaceInterne> Handle_SurfaceInterne;
#endif

// Ensemble de polygone.
class Surface : public Handle_SurfaceInterne
{
public:
#ifndef SWIG
  Surface(const IN_STD string& nom = "");
#else
  Surface(const char* nom = "");
#endif
#ifndef SWIG
  Surface(_SurfaceInterne* ptr);
  Surface(const Surface& su); 
#endif
  ~Surface();
  
  Composition composition ( ) const;

  void                ajouter(const Polygone& pg);
  void                retirer(const Polygone& pg);
  
  /// Nombre de polygones.
  size_type           nb_polygones() const;
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
  
  // Polygone numéro n.
  Polygone                polygone(size_type n) const;
  // Polygone d'ID n.
  Polygone                polygone_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_polygone)
#endif
  bool                    contenir(const Polygone& pg) const;
  //  Liste des attributs des polygones.
  Liste_attribut&         att_polygones();
  // Initialise les listes d'attributs de la surface.
  void                    init_att();
  
  // Bras numéro n.
  Bras                    bras(size_type n) const;
  // Bras d'ID n.
  Bras                    bras_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_bras)
#endif
  bool                    contenir(const Bras& br) const;
  //  Liste des attributs des bras.
  Liste_attribut&         att_bras();

  // Noeud numéro n.
  Noeud               noeud(size_type n) const;
  // Noeud d'ID n.
  Noeud               noeud_id(size_type id) const;
  //  Liste des attributs des noeuds.
  Liste_attribut&         att_noeuds();
  
#ifdef SWIG
  %name(contenir_noeud)
#endif
  bool                contenir(const Noeud& nd) const;
  
  Ligne               contour() const;
  size_type           longueur_i() const;
  size_type           longueur_j() const;    
  
#ifndef SWIG
  const Surface& operator = (const Surface& su);
  bool operator == (const Surface& su);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
