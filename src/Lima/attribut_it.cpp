#include <iostream>
#include "LimaP/attribut_it.h"
#include <Lima/erreur.h>
#include "LimaP/attribut_type.h"
#include "LimaP/using_it.h"

#include <values.h>

BEGIN_NAMESPACE_LIMA


_Attribut::_Attribut(const string& nom) 
{
  nommer(nom);
  set_max_elt(0);
}

void _Attribut::set_max_elt(size_type size_max) 
{
  m_max_elt=size_max;
}

size_type _Attribut::get_max_elt()
{
  return m_max_elt;
}

const string& _Attribut::nom() const 
{
  return m_nom;
}

void _Attribut::nommer(const string& str) 
{
  m_nom=str;
}

_Attribut& _Attribut::operator = (const _Attribut& att)
{
  nommer(att.nom());
  return *this;
}

//_Attribut* _Attribut::copy() const
//{
//  return new _Attribut(nom());
//}

bool _Attribut::compare(const _Attribut& att) 
{
  return (nom()==att.nom());
}

void _Attribut::retirer(size_type indice)
{;}

void _Attribut::augmenter()
{;}

void                _Attribut::set_val(int     val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->set_val(val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(double  val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->set_val(val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const string&   val)
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->set_val(val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const Vect2d& val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->set_val(val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const Vect3d& val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->set_val(val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(size_type indice,int     val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->set_val(indice,val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(size_type indice,double  val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->set_val(indice,val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(size_type indice,const string&   val)
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->set_val(indice,val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(size_type indice,const Vect2d& val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->set_val(indice,val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(size_type indice,const Vect3d& val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->set_val(indice,val);
  else
    throw att_erreur("set_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const vector<int>& tab_val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->set_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const vector<double>& tab_val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->set_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const vector<string>& tab_val)   
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->set_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const vector<Vect2d>& tab_val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->set_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::set_val(const vector<Vect3d>& tab_val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->set_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(int&  val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->get_val(val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(double&  val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->get_val(val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(string&  val)
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->get_val(val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(Vect2d&  val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->get_val(val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(Vect3d&  val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->get_val(val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(size_type indice,int&  val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->get_val(indice,val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(size_type indice,double&  val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->get_val(indice,val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(size_type indice,string&  val)
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->get_val(indice,val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(size_type indice,Vect2d&  val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->get_val(indice,val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(size_type indice,Vect3d&  val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->get_val(indice,val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(vector<int>& tab_val)
{
  if (type() == Attribut::ENTIER)
    ((_AttributInterne<int>*) (this))->get_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(vector<double>& tab_val)
{
  if (type() == Attribut::REEL)
    ((_AttributInterne<double>*) (this))->get_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(vector<string>& tab_val)   
{
  if (type() == Attribut::CHAINE)
    ((_AttributInterne<string>*) (this))->get_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(vector<Vect2d>& tab_val)
{
  if (type() == Attribut::VECT2D)
    ((_AttributInterne<Vect2d>*) (this))->get_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

void                _Attribut::get_val(vector<Vect3d>& tab_val)
{
  if (type() == Attribut::VECT3D)
    ((_AttributInterne<Vect3d>*) (this))->get_val(tab_val);
  else
    throw att_erreur("get_val : Erreur de type sur l'attribut \n");
}

Attribut::TYPE type_att(int& a)
{
  return Attribut::ENTIER;
}
  
Attribut::TYPE type_att(double& a)
{
  return Attribut::REEL;
}
  
Attribut::TYPE type_att(string& a)
{
  return Attribut::CHAINE;
}
  
Attribut::TYPE type_att(Vect2d& a)
{
  return Attribut::VECT2D;
}
  
Attribut::TYPE type_att(Vect3d& a)
{
  return Attribut::VECT3D;
}
  
int non_def(int& a)
{
  return ENTIER_ND;
}
  
double non_def(double& a)
{
  return REEL_ND;
}
  
string non_def(string& a)
{
  return CHAINE_ND;
}
  
Vect2d non_def(Vect2d& a)
{
  return VECT2D_ND;
}
  
Vect3d non_def(Vect3d& a)
{
  return VECT3D_ND;
} 

END_NAMESPACE_LIMA

