#ifndef _OCTOEDRE_IT_H
#define _OCTOEDRE_IT_H

#include "config_it.h"
#include "polyedre_it.h"

BEGIN_NAMESPACE_LIMA

//! Octoèdre.
/*!
  Spécialisation de la classe _PolyedreInterne dans le cas de l'octoèdre.
*/
class _OctoedreInterne : public _PolyedreInterne
{
public:
  //! Volume orienté du polyèdre.
  double                volume() const;
  //! Inverse le sens de parcours du polyèdre.
  void                  retourner();

  //! Nombre potentiel de faces.
  size_type             nb_faces() const;
  //! Nombre potentiel d'arètes.
  size_type             nb_aretes() const;

  //! Crée l'arête n.
  _BrasInterne*         extraire_arete(size_type n) const;
  //! Crée l'arête n.
  _PolygoneInterne*     extraire_face(size_type n) const;

  //! Retourne l'indice local du bras, -1 sinon.
  int                   contenir(const _BrasInterne* br) const;
  //! Retourne l'indice local du polygone, -1 sinon.
  int  
                 contenir(const _PolygoneInterne* pg) const;
  // Type de polyedre
  Polyedre::PolyedreType type() {return Polyedre::OCTOEDRE;};

protected:
  _OctoedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
		   _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
		   _NoeudInterne* n9, _NoeudInterne* n10, _NoeudInterne* n11);
  _OctoedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4,
		   _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7,
		   _NoeudInterne* n8, _NoeudInterne* n9, _NoeudInterne* n10,
		   _NoeudInterne* n11);

  bool comparer(const _PolyedreInterne* p) const;

  friend class _PolyedreInterne;

private:
  enum {NB_NOEUDS=12, NB_ARETES=18, NB_FACES=8};
  
  static size_type      m_noeud_aretes[NB_NOEUDS][4][2];
  static size_type      m_noeud_faces[NB_NOEUDS][7][7];
  static size_type      m_faces[NB_FACES][7];
  static size_type      m_aretes[NB_ARETES][2];
};
  
inline 
_OctoedreInterne::_OctoedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
				   _NoeudInterne* n2, _NoeudInterne* n3,
				   _NoeudInterne* n4, _NoeudInterne* n5,
				   _NoeudInterne* n6, _NoeudInterne* n7,
				   _NoeudInterne* n8, _NoeudInterne* n9,
				   _NoeudInterne* n10, _NoeudInterne* n11):
  _PolyedreInterne(n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11)
{
}

inline 
_OctoedreInterne::_OctoedreInterne(size_type n, _NoeudInterne* n0, 
				   _NoeudInterne* n1, _NoeudInterne* n2, 
				   _NoeudInterne* n3, _NoeudInterne* n4,  
				   _NoeudInterne* n5, _NoeudInterne* n6, 
				   _NoeudInterne* n7, _NoeudInterne* n8, 
				   _NoeudInterne* n9, _NoeudInterne* n10, 
				   _NoeudInterne* n11):
  _PolyedreInterne(n, n0, n1, n2, n3, n4, n5, n6, n7, n8, n9, n10, n11)
{
}


END_NAMESPACE_LIMA

#endif
