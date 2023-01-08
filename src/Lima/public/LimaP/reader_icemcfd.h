#ifdef __INTERNE_ICEMCFD
#ifndef _READER_ICEMCFD_H
#define _READER_ICEMCFD_H

#include "reader.h"
#include "writer.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "polyedre_it.h"

#include "nuage_it.h"
#include "ligne_it.h"
#include "surface_it.h"
#include "volume_it.h"


BEGIN_NAMESPACE_LIMA

// Class interface pour la lecture des fichiers icemcfd.
class _ReaderIcemcfd : public _Reader
{
public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderIcemcfd(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderIcemcfd();

    
private:
  //! Lecture d'un fichier icemcfd.
  void          lire (bool loadData);

  /*! Ajoute le noeud reçu en argument au nuage dont le nom est donné 
  en argument. Créé le nuage si nécessaire. 
  */
  void ajouter (_NoeudInterne* noeud, const IN_STD string& nom_nuage);

  /*! Ajoute le bras reçu en argument à la ligne dont le nom est donné 
  en argument. Créé la ligne si nécessaire. 
  */
  void ajouter (_BrasInterne* bras, const IN_STD string& nom_ligne);

  /*! Ajoute le polygone reçu en argument à la surface dont le nom est 
  donné en argument. Créé la surface si nécessaire. 
  */
  void ajouter (_PolygoneInterne* polygone, const IN_STD string& nom_surface);

  /*! Ajoute le polyedre reçu en argument au volume dont le nom est 
  donné en argument. Créé le volume si nécessaire. 
  */
  void ajouter (_PolyedreInterne* polyedre, const IN_STD string& nom_volume);

  /*!Lève une exception de type read_erreur contenant le message
  d'erreur d'icem. */
  void erreur();

  /*! Met à jour la composition du maillage par addition du nombre
   * d'éléments reçus en argument au type ICEM associé. */
  void updateComposition (int type, int nb);

  /** Ajoute le groupe reçu en argument au maillage. Le type de groupe est
   * déterminé à l'aide du type d'élément ICEM.
   */
  void ajouterGroupe (int type, const IN_STD string& name);
};

END_NAMESPACE_LIMA

#endif
#endif
