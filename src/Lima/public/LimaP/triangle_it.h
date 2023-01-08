#ifndef _TRIANGLE_IT_H
#define _TRIANGLE_IT_H

#include "config_it.h"
#include "polygone_it.h"

BEGIN_NAMESPACE_LIMA

//! Triangle du maillage.
/*!
  Spécialisation de la classe _PolygoneInterne dans le cas du triangle.
*/
class _TriangleInterne : public _PolygoneInterne
{
public:
  //! Surface du triangle.
  /*!
    Redéfinition pour des raisons d'efficacité.
  */
  virtual double        surface2D() const;

  //! Inverse le sens de parcours du triangle.
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
  _TriangleInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2);
  _TriangleInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2);

  friend class _PolygoneInterne;
};

inline 
_TriangleInterne::_TriangleInterne(_NoeudInterne* n0, _NoeudInterne* n1,
				   _NoeudInterne* n2):
  _PolygoneInterne(n0, n1, n2)
{
}

inline 
_TriangleInterne::_TriangleInterne(size_type n, _NoeudInterne* n0, 
				   _NoeudInterne* n1, _NoeudInterne* n2):
  _PolygoneInterne(n, n0, n1, n2)
{
}


END_NAMESPACE_LIMA

#endif
