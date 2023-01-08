#include <Lima/attribut.h>
#include "LimaP/attribut_it.h"
#include "LimaP/attribut_type.h"
#include "LimaP/handle_t.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

Attribut::Attribut(TYPE t, const string& nom):
  _Handle<_Attribut>(NULL)
{
  switch(t){
  case ENTIER:
    m_ptr = new _AttributInterne<int>(nom);
    break;
  case REEL:
    m_ptr = new _AttributInterne<double>(nom);
    break;
  case CHAINE:
    m_ptr = new _AttributInterne<string>(nom);
    break;
  case VECT2D:
    m_ptr = new _AttributInterne<Vect2d>(nom);
    break;
  case VECT3D:
    m_ptr = new _AttributInterne<Vect3d>(nom);
    break;
  }
  m_ptr->ref_plus();
}

Attribut::Attribut(_Attribut* ptr):
  _Handle<_Attribut>(ptr)
{
}

Attribut::Attribut(const Attribut& at):
  _Handle<_Attribut>(at)
{
}

Attribut::Attribut():
  _Handle<_Attribut>(NULL)
{
}

Attribut::~Attribut()
{
}

const string& Attribut::nom() const
{
  return m_ptr->nom();
}

void Attribut::nommer(const string& str)
{
  m_ptr->nommer(str);
}

Attribut::TYPE Attribut::type() const
{
  return m_ptr->type();
}

void Attribut::retirer(size_type i)
{
  m_ptr->retirer(i);
}

void Attribut::augmenter()
{
  m_ptr->augmenter();
}

size_type Attribut::nb_elements() const
{
  return m_ptr->nb_elements();
}

void Attribut::set_val(int val)
{
  m_ptr->set_val(val);
}

void Attribut::set_val(double val)
{
  m_ptr->set_val(val);
}

void Attribut::set_val(const string& val)
{
  m_ptr->set_val(val);
}

void Attribut::set_val(const Vect2d& val)
{
  m_ptr->set_val(val);
}

void Attribut::set_val(const Vect3d& val)
{
  m_ptr->set_val(val);
}

void Attribut::set_val(size_type indice,int val)
{
  m_ptr->set_val(indice, val);
}

void Attribut::set_val(size_type indice,double val)
{
  m_ptr->set_val(indice, val);
}

void Attribut::set_val(size_type indice,const string& val)
{
  m_ptr->set_val(indice, val);
}

void Attribut::set_val(size_type indice,const Vect2d& val)
{
  m_ptr->set_val(indice, val);
}

void Attribut::set_val(size_type indice,const Vect3d& val)
{
  m_ptr->set_val(indice, val);
}

void Attribut::set_val(const vector<int>& tab_val)
{
  m_ptr->set_val(tab_val);
}

void Attribut::set_val(const vector<double>& tab_val)
{
  m_ptr->set_val(tab_val);
}

void Attribut::set_val(const vector<string>& tab_val)
{
  m_ptr->set_val(tab_val);
}

void Attribut::set_val(const vector<Vect2d>& tab_val)
{
  m_ptr->set_val(tab_val);
}

void Attribut::set_val(const vector<Vect3d>& tab_val)
{
  m_ptr->set_val(tab_val);
}

void Attribut::get_val(int& val) const
{
  m_ptr->get_val(val);
}

void Attribut::get_val(double& val) const
{
  m_ptr->get_val(val);
}

void Attribut::get_val(string& val) const
{
  m_ptr->get_val(val);
}

void Attribut::get_val(Vect2d& val) const
{
  m_ptr->get_val(val);
}

void Attribut::get_val(Vect3d& val) const
{
  m_ptr->get_val(val);
}

void Attribut::get_val(size_type indice,int& val) const
{
  m_ptr->get_val(indice, val);
}

void Attribut::get_val(size_type indice,double& val) const
{
  m_ptr->get_val(indice, val);
}

void Attribut::get_val(size_type indice,string& val) const
{
  m_ptr->get_val(indice, val);
}

void Attribut::get_val(size_type indice,Vect2d& val) const
{
  m_ptr->get_val(indice, val);
}

void Attribut::get_val(size_type indice,Vect3d& val) const
{
  m_ptr->get_val(indice, val);
}

void Attribut::get_val(vector<int>& tab_val) const
{
  m_ptr->get_val(tab_val);
}

void Attribut::get_val(vector<double>& tab_val) const
{
  m_ptr->get_val(tab_val);
}

void Attribut::get_val(vector<string>& tab_val) const
{
  m_ptr->get_val(tab_val);
}

void Attribut::get_val(vector<Vect2d>& tab_val) const
{
  m_ptr->get_val(tab_val);
}

void Attribut::get_val(vector<Vect3d>& tab_val) const
{
  m_ptr->get_val(tab_val);
}

const Attribut& Attribut::operator = (const Attribut& a)
{
  _Handle<_Attribut>::operator = (a);
  return *this;
}

bool Attribut::operator == (const Attribut& a)
{
  return _Handle<_Attribut>::operator == (a);
}

END_NAMESPACE_LIMA
