#ifndef _VECTEUR_H
#define _VECTEUR_H

#if defined(linux)
extern "C" double sqrt (double) throw ( );
#else
extern "C" double sqrt (double);
#endif	// #if defined(linux)
#include "point.h"

BEGIN_NAMESPACE_LIMA
  
// Un vecteur
class _Vecteur
{
public:
  _Vecteur(double x=0, double y=0, double z=0);
  _Vecteur(const _Point& origine, const _Point& extremite);
  
  // Coordonnée X du vecteur.
  double        x() const;
  // Coordonnée Y du vecteur. 
  double        y() const;
  // Coordonnée Z du vecteur. 
  double        z() const;
    
  // Coordonnée X du vecteur.
  void          set_x(double d);
  // Coordonnée Y du vecteur. 
  void          set_y(double d);
  // Coordonnée Z du vecteur. 
  void          set_z(double d);

  // Norme du vecteur.
  double        norme() const;

private:
  double        m_coo[3];
};


inline 
_Vecteur::_Vecteur(double x, double y, double z)
{
  m_coo[0] = x;
  m_coo[1] = y;
  m_coo[2] = z;
}

inline 
_Vecteur::_Vecteur(const _Point& origine, const _Point& extremite)
{
  m_coo[0] = extremite.x()-origine.x();
  m_coo[1] = extremite.y()-origine.y();
  m_coo[2] = extremite.z()-origine.z();
}

inline
double _Vecteur::x() const
{
  return m_coo[0];
}

inline
double _Vecteur::y() const
{
  return m_coo[1];
}

inline
double _Vecteur::z() const
{
  return m_coo[2];
}

inline
void _Vecteur::set_x(double d)
{
  m_coo[0] = d;
}

inline
void _Vecteur::set_y(double d)
{
  m_coo[1] = d;
}

inline
void _Vecteur::set_z(double d)
{
  m_coo[2] = d;
}

inline
double operator * (const _Vecteur& v, const _Vecteur& w)
{
  return v.x()*w.x()+v.y()*w.y()+v.z()*w.z();
} 

inline
_Vecteur operator ^ (const _Vecteur& v, const _Vecteur& w)
{ 
  return _Vecteur(v.y()*w.z()-w.y()*v.z(), 
		  w.x()*v.z()-v.x()*w.z(), 
		  v.x()*w.y()-w.x()*v.y());
} 

inline
double determinant(const _Vecteur& u, const _Vecteur& v, 
		   const _Vecteur& w)
{
  return u*(v^w);
}

inline
double _Vecteur::norme() const
{
  return sqrt((*this)*(*this));
}

inline
_Vecteur operator + (const _Vecteur& v, const _Vecteur& w)
{
  return _Vecteur(v.x()+w.x(), v.y()+w.y(), v.z()+w.z());
}


END_NAMESPACE_LIMA

#endif
