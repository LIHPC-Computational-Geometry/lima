#ifndef _PYRAMIDE_IT_H
#define _PYRAMIDE_IT_H

#include "config_it.h"
#include "polyedre_it.h"

BEGIN_NAMESPACE_LIMA

//! Pyramide.
/*!
  Spécialisation de la classe _PolyedreInterne dans le cas de la pyramide.
*/
class _PyramideInterne : public _PolyedreInterne
{
public:
  //! Volume orienté du polyèdre.
  double                volume() const;
  //! Inverse le sens de parcours du polyèdre.
  void                  retourner();

  //! Nombre potentiel de faces.
  size_type             nb_faces() const;
  //! Nombre potentiel d'arêtes.
  size_type             nb_aretes() const;

  //! Crée l'arête n.
  _BrasInterne*         extraire_arete(size_type n) const;
  //! Crée l'arête n.
  _PolygoneInterne*     extraire_face(size_type n) const;

  //! Retourne l'indice local du bras, -1 sinon.
  int                   contenir(const _BrasInterne* br) const;
  //! Retourne l'indice local du polygone, -1 sinon.
  int                   contenir(const _PolygoneInterne* pg) const;

protected:
  _PyramideInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4);
  _PyramideInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4);

  static size_type POS[8][5];
  bool comparer(const _PolyedreInterne* p) const;

  friend class _PolyedreInterne;

  // Type de polyedre
  virtual Polyedre::PolyedreType type() {return Polyedre::PYRAMIDE;};

private:
  enum {NB_NOEUDS=5, NB_ARETES=8, NB_FACES=5};
  
  static size_type      m_noeud_aretes[NB_NOEUDS][5][2];
  static size_type      m_noeud_faces[NB_NOEUDS][9][5];
  static size_type      m_faces[NB_FACES][5];
  static size_type      m_aretes[NB_ARETES][2];
};
  
inline 
_PyramideInterne::_PyramideInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
				   _NoeudInterne* n2, _NoeudInterne* n3,
				   _NoeudInterne* n4):
  _PolyedreInterne(n0, n1, n2, n3, n4)
{
}

inline 
_PyramideInterne::_PyramideInterne(size_type n, _NoeudInterne* n0, 
				   _NoeudInterne* n1, _NoeudInterne* n2, 
				   _NoeudInterne* n3, _NoeudInterne* n4):
  _PolyedreInterne(n, n0, n1, n2, n3, n4)
{
}


END_NAMESPACE_LIMA

#endif
