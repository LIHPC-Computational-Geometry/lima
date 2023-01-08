#ifndef _VOLUME_H
#define _VOLUME_H

#include "config.h"
#include "handle.h"
#include "size.h"
#include "composition.h"

#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class Polyedre;
class Polygone;
class Bras;
class Noeud;
class _VolumeInterne;
class Surface;
class Liste_attribut;  

#ifndef SWIG
typedef _Handle<_VolumeInterne> Handle_VolumeInterne;
#endif

// Ensemble de polygone.
class Volume : public Handle_VolumeInterne
{
public:
#ifndef SWIG
  Volume(const IN_STD string& nom = "");
#else
  Volume(const char* nom = "");
#endif
#ifndef SWIG
  Volume(_VolumeInterne* ptr);
  Volume(const Volume& vo); 
#endif
  ~Volume();

  Composition composition ( ) const;
  
  void                ajouter(const Polyedre& pd);
  void                retirer(const Polyedre& pd);
  
  /// Nombre de polyedres.
  size_type           nb_polyedres() const;
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
  
  // Polyedre numéro n.
  Polyedre                polyedre(size_type n) const;
  // Polyedre d'ID n.
  Polyedre                polyedre_id(size_type id) const;
  
#ifdef SWIG
  %name(contenir_polyedre)
#endif
  bool                    contenir(const Polyedre& pd) const;
  //  Liste des attributs des polyèdres.
  Liste_attribut&         att_polyedres();
  // Initialise les listes d'attributs du volume.
  void                    init_att();
  
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
  
#ifdef SWIG
  %name(contenir_noeud)
#endif
  bool                contenir(const Noeud& nd) const;
  //  Liste des attributs des noeuds.
  Liste_attribut&     att_noeuds();
  
  Surface             contour() const;
  
#ifndef SWIG
  const Volume& operator = (const Volume& vo);
  bool operator == (const Volume& vo);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
