#include <iostream>
#include <Lima/size.h>
#include <Lima/vect2d.h>
#include <Lima/non_defini.h>

BEGIN_NAMESPACE_LIMA

Vect2d::Vect2d(double x, double y)
{
  m_coo[0] = x;
  m_coo[1] = y;
}

double Vect2d::x() const
{
  return m_coo[0];
}

double Vect2d::y() const
{
  return m_coo[1];
}

double Vect2d::coo(size_type i) const
{
  switch(i){
  case 1:
  case 2:
    return m_coo[i-1];
  default:
    return REEL_ND;
  }
}

void Vect2d::set_x(double d)
{
  m_coo[0] = d;
}

void Vect2d::set_y(double d)
{
  m_coo[1] = d;
}

void Vect2d::coo(size_type i, double d)
{
  switch(i){
  case 1:
  case 2:
    m_coo[i-1]=d;
  }
}

END_NAMESPACE_LIMA
