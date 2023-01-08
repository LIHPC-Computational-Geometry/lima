#ifndef _POINT_H
#define _POINT_H

#include <math.h>
#include <functional>

#include "config_it.h"
#include <Lima/enum.h>

BEGIN_NAMESPACE_LIMA

//! Un point 3D.
class _Point
{
public:
  //! Un point par ses coordonnées.
  _Point(double x=0, double y=0, double z=0);

  //! Coordonnée X du point.
  double          x() const;
  //! Coordonnée Y du point. 
  double          y() const;
  //! Coordonnée Z du point. 
  double          z() const;
    
  //! Coordonnée \a dim du point. 
  double          coo(dim_t dim) const;

  //! Coordonnée X du point.
  void            set_x(double d);
  //! Coordonnée Y du point. 
  void            set_y(double d);
  //! Coordonnée Z du point. 
  void            set_z(double d);
 
//! Carré de la précision utilisé dans la comparaison de 2 points.
static double epsilon2;

private:
  //! Les coordonnées.
  double            m_coo[3];
};


inline 
_Point::_Point(double x, double y, double z)
{
  m_coo[0] = x;
  m_coo[1] = y;
  m_coo[2] = z;
}

// Coordonnée X du point.
inline
double _Point::x() const
{
  return m_coo[0];
}

// Coordonnée Y du point. 
inline
double _Point::y() const
{
  return m_coo[1];
}

// Coordonnée Z du point. 
inline
double _Point::z() const
{
  return m_coo[2];
}
// Coordonnée \a dim du point. 
inline
double _Point::coo(dim_t dim) const
{
  if (dim == D1)
    return m_coo[0];
  else if (dim == D2)
    return m_coo[1];
  else if (dim == D3)
    return m_coo[2];
  else
    return m_coo[0];
}

// Coordonnée X du point.
inline
void _Point::set_x(double d)
{
  m_coo[0] = d;
}

// Coordonnée Y du point. 
inline
void _Point::set_y(double d)
{
  m_coo[1] = d;
}

// Coordonnée Z du point. 
inline
void _Point::set_z(double d)
{
  m_coo[2] = d;
}


inline bool operator == (const _Point& p1, const _Point& p2)
{
	IN_STD less_equal<double>	le;
	double	dx	= p1.x ( ) - p2.x ( );
	double	dy	= p1.y ( ) - p2.y ( );
	double	dz	= p1.z ( ) - p2.z ( );
	double	distance2	= dx * dx + dy * dy + dz * dz;
	return	le (distance2, _Point::epsilon2);
}	// operator == (const _Point& p1, const _Point& p2)


inline bool operator != (const _Point& p1, const _Point& p2)
{
	return !(p1 == p2);
}	// operator != (const _Point& p1, const _Point& p2)


END_NAMESPACE_LIMA

#endif
