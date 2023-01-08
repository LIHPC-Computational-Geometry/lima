#include <iostream>
#include <Lima/size.h>
#include <Lima/vect3d.h>
#include <Lima/non_defini.h>

BEGIN_NAMESPACE_LIMA


Vect3d::Vect3d(double x, double y, double z)
{
  m_coo[0] = x;
  m_coo[1] = y;
  m_coo[2] = z;
}

double Vect3d::x() const
{
  return m_coo[0];
}

double Vect3d::y() const
{
  return m_coo[1];
}

double Vect3d::z() const
{
  return m_coo[2];
}

double Vect3d::coo(size_type i) const
{
  switch(i){
  case 1:
  case 2:
  case 3:
    return m_coo[i-1];
  default:
    return REEL_ND;
  }
}

void Vect3d::set_x(double d)
{
  m_coo[0] = d;
}

void Vect3d::set_y(double d)
{
  m_coo[1] = d;
}

void Vect3d::set_z(double d)
{
  m_coo[2] = d;
}

void Vect3d::coo(size_type i, double d)
{
  switch(i){
  case 1:
  case 2:
  case 3:
    m_coo[i-1]=d;
  }
}

END_NAMESPACE_LIMA
