#ifndef _QUADRANGLE_IT_H
#define _QUADRANGLE_IT_H

#include "config_it.h"
#include "polygone_it.h"

BEGIN_NAMESPACE_LIMA

//! Quadrangle du maillage.
/*!
  Spécialisation de la classe _PolygoneInterne dans le cas du quadrangle.
*/
class _QuadrangleInterne : public _PolygoneInterne
{
public:
  //! Surface du quadrangle.
  /*!
    Redéfinition pour des raisons d'efficacité.
  */
  virtual double        surface2D() const;

  //! Inverse le sens de parcours du quadrangle.
  /*!
    Redéfinition pour des raisons d'efficacité.
  */
  virtual void          retourner();
 
  //! Retourne l'indice local du bras, -1 sinon.
  /*!
    Redéfinition pour des raisons d'efficacité.
  */
  virtual int           contenir(const _BrasInterne* br) const;
  _BrasInterne*         element_oppose(const _BrasInterne* br) const;

protected:
  _QuadrangleInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
		     _NoeudInterne* n2, _NoeudInterne* n3);
  _QuadrangleInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		     _NoeudInterne* n2, _NoeudInterne* n3);

  friend class _PolygoneInterne;
};

inline 
_QuadrangleInterne::_QuadrangleInterne(_NoeudInterne* n0, _NoeudInterne* n1,
				       _NoeudInterne* n2, _NoeudInterne* n3):
  _PolygoneInterne(n0, n1, n2, n3)
{
}

inline 
_QuadrangleInterne::_QuadrangleInterne(size_type n, _NoeudInterne* n0, 
				       _NoeudInterne* n1, _NoeudInterne* n2,
				       _NoeudInterne* n3):
  _PolygoneInterne(n, n0, n1, n2, n3)
{
}


END_NAMESPACE_LIMA

#endif
