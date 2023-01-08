#ifndef _MAILLAGE_IT_H
#define _MAILLAGE_IT_H

#include "config_it.h"
#include <Lima/enum.h>
#include "reference.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "polyedre_it.h"
#include "nuage_it.h"
#include "ligne_it.h"
#include "surface_it.h"
#include "volume_it.h"
#include "liste.h"
#include "ensemble.h"
#include <Lima/composition.h>
#include <Lima/callback.h>

#include "file.h"
#include "filecpp.h"
#include <Lima/liste_attribut.h>
#include "enum_apif.h"

#include <vector>


BEGIN_NAMESPACE_LIMA


/* Cet enum definit les flags d'existence des differentes tables de connectivite.
 * L'entier associe a chaque table represente le nombre de decalages a gauche du 
 * bit 1 dans le motif binaire d'un entier. Les entites de cet enum sont utilisees
 * dans la fonction prepararer de _MaillageInterne et permettent de signaler si la
 * table correspondante a ete calculee ou videe.
 * Il y a egalement un flag d'existence des contours .*/ 
enum {
  noe_br = 1 <<  0 ,
  noe_pg = 1 <<  1 ,
  noe_pd = 1 <<  2 ,
  pg_br  = 1 <<  3 ,
  br_pg  = 1 <<  4 ,
  pd_br  = 1 <<  5 , 
  br_pd  = 1 <<  6 ,
  pd_pg  = 1 <<  7 ,
  pg_pd  = 1 <<  8 ,
  li_noe = 1 <<  9 ,
  su_noe = 1 << 10 ,
  su_br  = 1 << 11 ,
  vo_noe = 1 << 12 ,
  vo_br  = 1 << 13 ,
  vo_pg  = 1 << 14 ,
  cont_lig = 1 << 15,
  cont_sur = 1 << 16,
  cont_sur_noe = 1 << 17,
  cont_vol = 1 << 18
} ;


//! Le maillage.
class _MaillageInterne : public _ObjetReference 
{
public:
  typedef Ensemble<_NoeudInterne>::iterator iterator_noeud;
  typedef Ensemble<_NoeudInterne>::const_iterator const_iterator_noeud;
  typedef Ensemble<_BrasInterne>::iterator iterator_bras;
  typedef Ensemble<_BrasInterne>::const_iterator const_iterator_bras;
  typedef Ensemble<_PolygoneInterne>::iterator iterator_polygone;
  typedef Ensemble<_PolygoneInterne>::const_iterator const_iterator_polygone;
  typedef Ensemble<_PolyedreInterne>::iterator iterator_polyedre;
  typedef Ensemble<_PolyedreInterne>::const_iterator const_iterator_polyedre;
    
  typedef Liste<_NuageInterne>::iterator iterator_nuage;
  typedef Liste<_NuageInterne>::const_iterator const_iterator_nuage;
  typedef Liste<_LigneInterne>::iterator iterator_ligne;
  typedef Liste<_LigneInterne>::const_iterator const_iterator_ligne;
  typedef Liste<_SurfaceInterne>::iterator iterator_surface;
  typedef Liste<_SurfaceInterne>::const_iterator const_iterator_surface;
  typedef Liste<_VolumeInterne>::iterator iterator_volume;
  typedef Liste<_VolumeInterne>::const_iterator const_iterator_volume;

  //! Maillage de nom str.
  _MaillageInterne(const IN_STD string& str="");
  //! Destructeur
  ~_MaillageInterne();
    
  //! La composition du maillage. */
  Composition composition_noeuds ( ) const
  { return m_composition_noeuds; }
  Composition composition_bras ( ) const
  { return m_composition_bras; }
  Composition composition_polygones ( ) const
  { return m_composition_polygones; }
  Composition composition_polyedres ( ) const
  { return m_composition_polyedres; }

  //! La composition du maillage, pour les readers. */
  void composition_noeuds (const Composition& c)
  { m_composition_noeuds = c; }
  void composition_bras (const Composition& c)
  { m_composition_bras = c; }
  void composition_polygones (const Composition& c)
  { m_composition_polygones = c; }
  void composition_polyedres (const Composition& c)
  { m_composition_polyedres = c; }

  //! Libère toutes les ressources allouées par le maillage. */
  void          vider();

  //! Initialisation de la lecture
  void					attachReader(const IN_STD string& nom_fichier, 
						             format_t format=SUFFIXE, size_type num=1);

  /*! Lire la structure d'un fichier de un maillage.
     \exception renvoie une erreur en cas de problème.
  */
  void     		lire_structure(const IN_STD string& nom_fichier, 
				               format_t format=SUFFIXE, size_type num=1);

  /*! Lire la totalite d'un fichier de un maillage.
     \exception renvoie une erreur en cas de problème.
  */
  void     		lire(const IN_STD string& nom_fichier, format_t format=SUFFIXE,
				     size_type num=1);

  //!  Écrire un maillage.
  /*!  
    \exception renvoie une erreur en cas de problème 
  */
  void         	ecrire(const IN_STD string& nom_fichier, 
					format_t format=SUFFIXE, size_type num=1,
					bool compacter = true);

  //! Préparation.
  /*!
    Effectue les traitements de vérification et de
    calcul selon le niveau de préparation demande.
    \exception renvoie une erreur en cas de problème.
  */
  void          preparer(preparation_t preparation);
  void  		preparation_parametrable(TypeMasque preparation, 
				                         bool init = false);
    
  /** Vérifie que le maillage est conforme au code dont l'identifiant est
   *  tranmis en argument.
   *  @exception Lève une exception de type Lima::erreur contenant les non
   *             conformités rencontrées.
   */    
  void          verification_code (TypeVerificationCode type) const;

  /** Vérifie que le maillage est conforme aux spécifications demandées
   *  dans le masque transmis en argument.
   *  @exception Lève une exception de type Lima::erreur contenant les non
   *             conformités rencontrées.
   */
  void          verification_parametrable (TypeMasqueVerification masque) const;

  //! Dimension du maillage, 1D, 2D ou 3D.
  dim_t         dimension() const;
  //! Unité longueur.
  /*! Coefficient multiplicatif à appliquer sur les coordonnées
    pour obtenir des mètres 
  */
  double        unite_longueur() const;
  //! Unité d'angle.
  /*! 
    Coefficient multiplicatif à appliquer sur les angles
    pour obtenir des radians 
  */
  double        unite_angle() const;
  //! Retourne le type de coordonnées.
  coordonnee_t  type_coordonnee() const;
  //! Retourne le type de géométrie.
  geometrie_t   type_geometrie() const;
  //! Retourne le type de structuration.
  structuration_t   structuration() const;
  //! Retourne le titre du maillage.
  const IN_STD string& titre() const;
  //! Retourne la date de création du maillage.
  const IN_STD string& date() const;

  //! Dimension du maillage.
  void          dimension(dim_t dim);
  //! Unité de longueur du maillage.
  void          unite_longueur(double unilo);
  //! Unité d'angle du maillage.
  void          unite_angle(double unia);
  //! Type de géométrie du maillage.
  void          type_geometrie(geometrie_t geom);
  //! Type de coordonnée du maillage.
  void          type_coordonnee(coordonnee_t tycoo);
  //! Titre du maillage.
  void          titre(const IN_STD string& titre);
  //! Date du maillage.
  void          date(const IN_STD string& date);
    
  //! Nombre de noeuds.
  size_type             nb_noeuds() const;
  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Noeud d'ID n.
  _NoeudInterne*        noeud_id(size_type id) const;    
  //! Vérifie l'appartenance du noeud au maillage.
  bool                  contenir(_NoeudInterne* nd) const;
  //! Ajoute un noeud.
  void                  ajouter(_NoeudInterne* nd);
  //! Retirer un noeud.
  void                  retirer(_NoeudInterne* nd);
  //! Deplacer un noeud.
  void                  deplacer(size_type id, double x, double y, double z);

  //! Nombre de bras.
  size_type             nb_bras() const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Bras d'ID n.
  _BrasInterne*         bras_id(size_type id) const;    
  //! Vérifie l'appartenance du bras au maillage.
  bool                  contenir(_BrasInterne* br) const;
  //! Ajoute un bras.
  void                  ajouter(_BrasInterne* br);
  //! Retirer un bras.
  void                  retirer(_BrasInterne* br);

  //! Nombre de polygones.
  size_type             nb_polygones() const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;
  //! Polygone d'ID n.
  _PolygoneInterne*     polygone_id(size_type id) const;    
  //! Vérifie l'appartenance du polygone a l'ensemble.
  bool                  contenir(_PolygoneInterne* pg) const;
  //! Ajoute un polygone.
  void                  ajouter(_PolygoneInterne* pg);
  //! Retirer un polygone.
  void                  retirer(_PolygoneInterne* pg);

  //! Nombre de polyèdres.
  size_type             nb_polyedres() const;
  //! Polyèdre numéro n.
  _PolyedreInterne*     polyedre(size_type n) const;
  //! Polyèdre d'ID n.
  _PolyedreInterne*     polyedre_id(size_type id) const;    
  //! Vérifie l'appartenance du polyèdre a l'ensemble.
  bool                  contenir(_PolyedreInterne* pd) const;
  //! Ajoute un polyèdre.
  void                  ajouter(_PolyedreInterne* pd);
  //! Retirer un polyèdre.
  void                  retirer(_PolyedreInterne* pd);

  //! Nombre de nuages.
  size_type             nb_nuages() const;
  //! Nuage numéro n.
  _NuageInterne*        nuage(size_type id) const;    
  //! Nuage de nom str.
  _NuageInterne*        nuage(const IN_STD string& str) const;
  //! Nuage d'ordre n.
  _NuageInterne*        nuage_ordre(size_type n) const;  
  
  //! Vérifie l'appartenance du nuage a l'ensemble.
  bool                  contenir(_NuageInterne* nu) const;
  //! Ajoute un nuage.
  void                  ajouter(_NuageInterne* nu);
  //! Retire un nuage.
  void                  retirer(_NuageInterne* nu);

  //! Nombre de lignes.
  size_type             nb_lignes() const;
  //! Ligne numéro n.
  _LigneInterne*        ligne(size_type id) const;    
  //! Ligne de nom str. 
  _LigneInterne*        ligne(const IN_STD string& str) const;
  //! Ligne d'ordre n.
  _LigneInterne*        ligne_ordre(size_type n) const;  
  
  //! Vérifie l'appartenance du ligne a l'ensemble.
  bool                  contenir(_LigneInterne* li) const;
  //! Ajoute une ligne.
  void                  ajouter(_LigneInterne* li);
  //! Retire une ligne.
  void                  retirer(_LigneInterne* li);

  //! Nombre de surfaces.
  size_type             nb_surfaces() const;
  //! Surface numéro n.
  _SurfaceInterne*      surface(size_type id) const;    
  //! Surface de nom str. 
  _SurfaceInterne*      surface(const IN_STD string& str) const;
  //! Surface d'ordre n.
  _SurfaceInterne*      surface_ordre(size_type n) const;    

  //! Vérifie l'appartenance du surface a l'ensemble.
  bool                  contenir(_SurfaceInterne* su) const;
  //! Ajoute une surface.
  void                  ajouter(_SurfaceInterne* su);
  //! Retire une surface.
  void                  retirer(_SurfaceInterne* su);

  //! Nombre de volumes.
  size_type             nb_volumes() const;
  //! Volume numéro n.
  _VolumeInterne*       volume(size_type id) const;    
  //! Volume de nom str. 
  _VolumeInterne*       volume(const IN_STD string& str) const;
  //! Volume d'ordre n.
  _VolumeInterne*       volume_ordre(size_type n) const;    

  //! Vérifie l'appartenance du volume a l'ensemble.
  bool                  contenir(_VolumeInterne* vo) const;
  //! Ajoute un volume.
  void                  ajouter(_VolumeInterne* vo);
  //! Retirer un volume.
  void                  retirer(_VolumeInterne* vo);

  //! Iterator premier noeud.
  iterator_noeud        premier_noeud() 
    { return m_noeuds.premier(); }
  const_iterator_noeud  premier_noeud() const 
    { return m_noeuds.premier(); }

  //! Iterator dernier noeud.
  iterator_noeud        dernier_noeud() 
    { return m_noeuds.dernier(); }
  const_iterator_noeud  dernier_noeud() const 
    { return m_noeuds.dernier(); }

  //! Iterator premier bras.
  iterator_bras         premier_bras() 
    { return m_bras.premier(); }
  const_iterator_bras   premier_bras() const 
    { return m_bras.premier(); }

  //! Iterator dernier bras.
  iterator_bras         dernier_bras() 
    { return m_bras.dernier(); }
  const_iterator_bras dernier_bras() const 
    { return m_bras.dernier(); }

  //! Iterator premier polygone.
  iterator_polygone     premier_polygone() 
    { return m_polygones.premier(); }
  const_iterator_polygone premier_polygone() const 
    { return m_polygones.premier(); }

  //! Iterator dernier polygone.
  iterator_polygone     dernier_polygone() 
    { return m_polygones.dernier(); }
  const_iterator_polygone dernier_polygone() const 
    { return m_polygones.dernier(); }

  //! Iterator premier polyèdre.
  iterator_polyedre     premier_polyedre() 
    { return m_polyedres.premier(); }
  const_iterator_polyedre premier_polyedre() const 
    { return m_polyedres.premier(); }

  //! Iterator dernier polyèdre.
  iterator_polyedre     dernier_polyedre() 
    { return m_polyedres.dernier(); }
  const_iterator_polyedre dernier_polyedre() const 
    { return m_polyedres.dernier(); }

  //! Iterator premier nuage.
  iterator_nuage        premier_nuage() 
    { return m_nuages.premier(); }
  const_iterator_nuage  premier_nuage() const 
    { return m_nuages.premier(); }

  //! Iterator dernier nuage.
  iterator_nuage        dernier_nuage() 
    { return m_nuages.dernier(); }
  const_iterator_nuage  dernier_nuage() const 
    { return m_nuages.dernier(); }

  //! Iterator premier ligne.
  iterator_ligne        premier_ligne() 
    { return m_lignes.premier(); }
  const_iterator_ligne  premier_ligne() const 
    { return m_lignes.premier(); }

  //! Iterator dernier ligne.
  iterator_ligne        dernier_ligne() 
    { return m_lignes.dernier(); }
  const_iterator_ligne  dernier_ligne() const 
    { return m_lignes.dernier(); }

  //! Iterator premier surface.
  iterator_surface        premier_surface() 
    { return m_surfaces.premier(); }
  const_iterator_surface  premier_surface() const 
    { return m_surfaces.premier(); }

  //! Iterator dernier surface.
  iterator_surface        dernier_surface() 
    { return m_surfaces.dernier(); }
  const_iterator_surface  dernier_surface() const 
    { return m_surfaces.dernier(); }

  //! Iterator premier volume.
  iterator_volume        premier_volume() 
    { return m_volumes.premier(); }
  const_iterator_volume  premier_volume() const 
    { return m_volumes.premier(); }

  //! Iterator dernier volume.
  iterator_volume        dernier_volume() 
    { return m_volumes.dernier(); }
  const_iterator_volume  dernier_volume() const 
    { return m_volumes.dernier(); }

  void			formater_noms();
  void          compacter_numerotation();

  void          calculer_lignes_noeuds();
  void          calculer_contours_lignes();
  void          calculer_structuration_lignes();
    
  void          calculer_surfaces_noeuds();
  void          calculer_surfaces_bras();
  void          calculer_contours_surfaces();
  void          calculer_contours_surfaces_noeuds();
  void          calculer_structuration_surfaces();
  void          calculer_structuration_contours_surfaces();
  //! Oriente dans le sens trigonométrique les contours de surface.
  void          calculer_orientation_contours_surfaces();

 
  void          calculer_volumes_noeuds();
  void          calculer_volumes_bras();
  void          calculer_volumes_polygones();
  void          calculer_contours_volumes();
  void          calculer_contours_volumes_noeuds();
  void          calculer_contours_volumes_bras();
    
  void          calculer_noeuds_bras();
  void          calculer_noeuds_polygones();
  void          calculer_noeuds_polyedres();
    
  void          calculer_bras_polygones();
  void          calculer_bras_polyedres();
    
  void          calculer_polygones_bras();
  void          calculer_polygones_polyedres();
    
  void          calculer_polyedres_polygones();
  void          calculer_polyedres_bras();
    
  void          orientation_positive();
  //! Supprime les noeuds en multiples exemplaires (coordonnées identiques).
  // Un compactage de la numérotation peut être nécessaire après cette opération.
  void          supprimer_doublon_noeuds();
  void          supprimer_doublon_bras();
  void          supprimer_doublon_polygones();
  void          supprimer_doublon_polyedres();
    
  //! Supprime les entités qui ne sont pas dans un groupe
  // Un compactage de la numérotation peut être nécessaire après cette opération.
  void          supprimer_bras_inutilises();
  void          supprimer_polygones_inutilises();

  void          invalider_noeuds_bras();
  void          invalider_noeuds_polygones();
  void          invalider_noeuds_polyedres();
  void          invalider_bras_polygones();
  void          invalider_polygones_bras();
  void          invalider_polyedres_bras();
  void          invalider_bras_polyedres();
  void          invalider_polyedres_polygones();
  void          invalider_polygones_polyedres();
    
  //! Liste des attributs attachés au maillage
  Liste_attribut&    att_maillage();
  //! Liste des attributs attachés aux noeuds du maillage
  Liste_attribut&    att_noeuds();
  //! Liste des attributs attachés aux bras du maillage
  Liste_attribut&    att_bras();
  //! Liste des attributs attachés aux polygones du maillage
  Liste_attribut&    att_polygones();
  //! Liste des attributs attachés aux polyèdres du maillage
  Liste_attribut&    att_polyedres();
  //! Liste des attributs attachés aux nuages du maillage
  Liste_attribut&    att_nuages();
  //! Liste des attributs attachés aux lignes du maillage
  Liste_attribut&    att_lignes();
  //! Liste des attributs attachés aux surfaces du maillage
  Liste_attribut&    att_surfaces();
  //! Liste des attributs attachés aux volumes du maillage
  Liste_attribut&    att_volumes();

  //! Initialisation des attributs du maillage
  void               init_att_maillage();
  //! Initialisation des attributs aux noeuds du maillage
  void               init_att_noeuds();
  //! Initialisation des attributs aux bras du maillage
  void               init_att_bras();
  //! Initialisation des attributs aux polygones du maillage
  void               init_att_polygones();
  //! Initialisation des attributs aux polyedres du maillage
  void               init_att_polyedres();
  //! Initialisation des attributs aux nuages du maillage
  void               init_att_nuages();
  //! Initialisation des attributs aux lignes du maillage
  void               init_att_lignes();
  //! Initialisation des attributs aux surfaces du maillage
  void               init_att_surfaces();
  //! Initialisation des attributs aux volumes du maillage
  void               init_att_volumes();
  //! Initialisation de tous les attributs du maillage
  void               init_att();

  //! Retourne la liste des attributs selon le point d'attache
  Liste_attribut&	get_pt_attache(att_loc pt_attach, 
							               IN_STD string  nom_groupe);

  //! Ajoute un callback sur le chargement du maillage.
  void ajouter_reader_callback (ReaderCallback* rc);

  //! Enlève un callback sur le chargement du maillage.
  void enlever_reader_callback (ReaderCallback* rc);

  //! Avertit les écouteurs de callbacks du début de la lecture des 
  // données.
  void debut_lecture (const IN_STD string& fichier, size_type num);

  //! Avertit les écouteurs de callbacks de la fin de la lecture des 
  // données.
  void fin_lecture (const IN_STD string& fichier, size_type num);

  //! Avertit les écouteurs de callbacks sur la lecture d'un évènement.
  void donnees_lues (const IN_STD string& fichier, size_type num,
                     ReaderCallback::Type type, const IN_STD string& nom, 
                     double pourcent, bool& arreter);

protected :

  //! Remplace le noeud n1 par le noeud n2 dans tout élément du maillage. Ne
  // supprime pas n1 du maillage.
  // Les tables de connectivités inverses noeuds -> éléments sont supposées
  // existantes.
  void remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2);


  //! Suppression des noeuds en double dans un maillage 2D, 
  // version optimisé en s'inspirant de ce qui est fait dans Magix
  // Utilise un tableau 2D pour une décomposition spaciale de l'espace
  void supprimer_doublon_noeuds_2D();
  //! comme supprimer_doublon_noeuds_2D, met en 3D
  void supprimer_doublon_noeuds_3D();

#ifdef DEBUG
  // pour y comprendre qque chose dans ce souk
  void _afficheMaillage();
#endif

private:

  void              init();

  // La composition du maillage
  Composition         m_composition_noeuds;
  Composition         m_composition_bras;
  Composition         m_composition_polygones;
  Composition         m_composition_polyedres;

  // Titre du maillage, # ou ! interdit en premier caractère
  IN_STD string       m_titre;
  //  Date en ASCII
  IN_STD string       m_date;
  // Dimension.
  dim_t             m_dim;
  // Type de géométrie.
  geometrie_t       m_geom;
  // Type de structuration.
  structuration_t   m_struct;
  // Type des coordonnées
  coordonnee_t      m_tycoo;
  // Unité de longueur
  double            m_unilo;
  // Unité d'angle
  double            m_unia;

  _SurfaceInterne* m_contour3D;
  _LigneInterne*   m_contour2D;

  Ensemble<_NoeudInterne>       m_noeuds;
  Ensemble<_BrasInterne>        m_bras;
  Ensemble<_PolygoneInterne>    m_polygones;
  Ensemble<_PolyedreInterne>    m_polyedres;

  Liste<_NuageInterne>          m_nuages;
  Liste<_LigneInterne>          m_lignes;
  Liste<_SurfaceInterne>        m_surfaces;
  Liste<_VolumeInterne>         m_volumes;

  // Liste des attributs attaches au maillage
  Liste_attribut                m_liste_att_maillage;
  // Liste des attributs attaches aux noeuds du maillage
  Liste_attribut                m_liste_att_noeuds;
  // Liste des attributs attaches aux bras du maillage
  Liste_attribut                m_liste_att_bras;
  // Liste des attributs attaches aux polygones du maillage
  Liste_attribut                m_liste_att_polygones;
  // Liste des attributs attaches aux polyedres du maillage
  Liste_attribut                m_liste_att_polyedres;
  // Liste des attributs attaches aux nuages du maillage
  Liste_attribut                m_liste_att_nuages;
  // Liste des attributs attaches aux lignes du maillage
  Liste_attribut                m_liste_att_lignes;
  // Liste des attributs attaches aux surfaces du maillage
  Liste_attribut                m_liste_att_surfaces;
  // Liste des attributs attaches aux volumes du maillage
  Liste_attribut                m_liste_att_volumes;

  /* Etat de la preparation du maillage.
   * Donne la liste des tables de connectivite calculees et
   * renseigne sur le calcul ou non des contours.
   */
  int m_etat_prepa ;

  //! Les écouteurs d'évènements de chargement du maillage.
  IN_STD vector<ReaderCallback*>	m_readers_callback;

#ifdef LIMA_MT

	//! Les identifiants maximum des noeuds, bras, polygones et polyèdres
	// du maillage.
	size_type						m_noeuds_max_id;
	size_type						m_bras_max_id;
	size_type						m_polygones_max_id;
	size_type						m_polyedres_max_id;
#endif	// LIMA_MT
};

inline
dim_t _MaillageInterne::dimension() const
{
  return m_dim;
}

inline
double _MaillageInterne::unite_longueur() const
{
  return m_unilo;
}

inline
double _MaillageInterne::unite_angle() const
{
  return m_unia;
}

inline
coordonnee_t _MaillageInterne::type_coordonnee() const
{
  return m_tycoo;
}

inline
geometrie_t _MaillageInterne::type_geometrie() const
{
  return m_geom;
}
  
inline
structuration_t _MaillageInterne::structuration() const
{
  return m_struct;
}

inline
const IN_STD string& _MaillageInterne::titre() const
{
  return m_titre;
}
  
inline
const IN_STD string& _MaillageInterne::date() const
{
  return m_date;
}

inline
size_type _MaillageInterne::nb_noeuds() const
{
  return m_noeuds.nb_elements();
}

inline
size_type _MaillageInterne::nb_bras() const
{
  return m_bras.nb_elements();
}

inline
size_type _MaillageInterne::nb_polygones() const
{
  return m_polygones.nb_elements();
}

inline
size_type _MaillageInterne::nb_polyedres() const
{
  return m_polyedres.nb_elements();
}

inline
size_type _MaillageInterne::nb_nuages() const
{
  return m_nuages.nb_elements();
}

inline
size_type _MaillageInterne::nb_lignes() const
{
  return m_lignes.nb_elements();
}

inline
size_type _MaillageInterne::nb_surfaces() const
{
  return m_surfaces.nb_elements();
}

inline
size_type _MaillageInterne::nb_volumes() const
{
  return m_volumes.nb_elements();
}

inline
bool _MaillageInterne::contenir(_NoeudInterne* nd) const
{
  return nd->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_BrasInterne* br) const
{
  return br->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_PolygoneInterne* pg) const
{
  return pg->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_PolyedreInterne* pd) const
{
  return pd->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_NuageInterne* nu) const
{
  return nu->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_LigneInterne* li) const
{
  return li->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_SurfaceInterne* su) const
{
  return su->maillage() == this;
}

inline
bool _MaillageInterne::contenir(_VolumeInterne* vo) const
{
  return vo->maillage() == this;
}

inline
_NoeudInterne* _MaillageInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}

inline
_BrasInterne* _MaillageInterne::bras(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
 return m_bras.element(n);
}

inline
_PolygoneInterne* _MaillageInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones.element(n);
}

inline
_PolyedreInterne* _MaillageInterne::polyedre(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polyedres())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de polyedre " +
		       to_str(n) + " non valide\n");
#endif
  return m_polyedres.element(n);
}

inline
_NuageInterne* _MaillageInterne::nuage(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_nuages())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de nuage " +
		       to_str(n) + " non valide\n");
#endif
  return m_nuages.element(n);
}

inline
_LigneInterne* _MaillageInterne::ligne(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_lignes())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de ligne " +
		       to_str(n) + " non valide\n");
#endif
 return m_lignes.element(n);
}

inline
_SurfaceInterne* _MaillageInterne::surface(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_surfaces())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de surface " +
		       to_str(n) + " non valide\n");
#endif
  return m_surfaces.element(n);
}

inline
_VolumeInterne* _MaillageInterne::volume(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_volumes())
    throw range_erreur("Maillage " + titre() + 
		       ", Indice local de volume " +
		       to_str(n) + " non valide\n");
#endif
  return m_volumes.element(n);
}

inline
_NoeudInterne* _MaillageInterne::noeud_id(size_type n) const
{
  return m_noeuds.element_id(n);
}

inline
_BrasInterne* _MaillageInterne::bras_id(size_type n) const
{
  return m_bras.element_id(n);
}

inline
_PolygoneInterne* _MaillageInterne::polygone_id(size_type n) const
{
  return m_polygones.element_id(n);
}

inline
_PolyedreInterne* _MaillageInterne::polyedre_id(size_type n) const
{
  return m_polyedres.element_id(n);
}

inline
_NuageInterne* _MaillageInterne::nuage(const IN_STD string& str) const
{
  return m_nuages.element_id(str);
}

inline
_LigneInterne* _MaillageInterne::ligne(const IN_STD string& str) const
{
  return m_lignes.element_id(str);
}

inline
_SurfaceInterne* _MaillageInterne::surface(const IN_STD string& str) const
{
  return m_surfaces.element_id(str);
}

inline
_VolumeInterne* _MaillageInterne::volume(const IN_STD string& str) const
{
  return m_volumes.element_id(str);
}

inline
_NuageInterne* _MaillageInterne::nuage_ordre(size_type n) const
{
  for (size_type in = 0; in < nb_nuages(); in++)
    if (m_nuages.element(in)->ordre() == n)
      return m_nuages.element(in);
  return NULL;
}

inline
_LigneInterne* _MaillageInterne::ligne_ordre(size_type n) const
{
  for (size_type in = 0; in < nb_lignes(); in++)
    if (m_lignes.element(in)->ordre() == n)
      return m_lignes.element(in);
  return NULL;
}

inline
_SurfaceInterne* _MaillageInterne::surface_ordre(size_type n) const
{
  for (size_type in = 0; in < nb_surfaces(); in++)
    if (m_surfaces.element(in)->ordre() == n)
      return m_surfaces.element(in);
  return NULL;
}

inline
_VolumeInterne* _MaillageInterne::volume_ordre(size_type n) const
{
  for (size_type in = 0; in < nb_volumes(); in++)
    if (m_volumes.element(in)->ordre() == n)
      return m_volumes.element(in);
  return NULL;
}

inline
Liste_attribut&    _MaillageInterne::att_maillage()
{
  return m_liste_att_maillage;
}

inline
Liste_attribut&    _MaillageInterne::att_noeuds()
{
  return m_liste_att_noeuds;
}

inline
Liste_attribut&    _MaillageInterne::att_bras()
{
  return m_liste_att_bras;
}

inline
Liste_attribut&    _MaillageInterne::att_polygones()
{
  return m_liste_att_polygones;
}

inline
Liste_attribut&    _MaillageInterne::att_polyedres()
{
  return m_liste_att_polyedres;
}

inline
Liste_attribut&    _MaillageInterne::att_nuages()
{
  return m_liste_att_nuages;
}

inline
Liste_attribut&    _MaillageInterne::att_lignes()
{
  return m_liste_att_lignes;
}

inline
Liste_attribut&    _MaillageInterne::att_surfaces()
{
  return m_liste_att_surfaces;
}

inline
Liste_attribut&    _MaillageInterne::att_volumes()
{
  return m_liste_att_volumes;
}

inline
void               _MaillageInterne::init_att_maillage()
{
  att_maillage().set_max_val(1);
}

inline
void               _MaillageInterne::init_att_noeuds()
{
  att_noeuds().set_max_val(nb_noeuds());
}

inline
void               _MaillageInterne::init_att_bras()
{
  att_bras().set_max_val(nb_bras());
}

inline
void               _MaillageInterne::init_att_polygones()
{
  att_polygones().set_max_val(nb_polygones());
}

inline
void               _MaillageInterne::init_att_polyedres()
{
  att_polyedres().set_max_val(nb_polyedres());
}

inline
void               _MaillageInterne::init_att_nuages()
{
  att_nuages().set_max_val(nb_nuages());
}

inline
void               _MaillageInterne::init_att_lignes()
{
  att_lignes().set_max_val(nb_lignes());
}

inline
void               _MaillageInterne::init_att_surfaces()
{
  att_surfaces().set_max_val(nb_surfaces());
}

inline
void               _MaillageInterne::init_att_volumes()
{
  att_volumes().set_max_val(nb_volumes());
}

inline
void               _MaillageInterne::init_att()
{
  init_att_maillage();
  init_att_noeuds();
  init_att_bras();
  init_att_polygones();
  init_att_polyedres();
  init_att_nuages();
  init_att_lignes();
  init_att_surfaces();
  init_att_volumes();
}

END_NAMESPACE_LIMA

#endif
