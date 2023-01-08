#ifndef _ATTRIBUT_H
#define _ATTRIBUT_H

#include "config.h"
#include "handle.h"
#include "size.h"
#include "vect2d.h"
#include "vect3d.h"


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

class _Attribut;

#ifndef SWIG
typedef _Handle<_Attribut> Handle_Attribut;
#endif

class Attribut : public Handle_Attribut
{
public:
  enum TYPE {ENTIER, REEL, CHAINE, VECT2D, VECT3D};

#ifndef SWIG
  Attribut(TYPE t, const IN_STD string& nom);
#else
  Attribut(TYPE t, const char* nom);
#endif	// SWIG
#ifndef SWIG
  Attribut();

  Attribut(_Attribut* ptr);
  Attribut(const Attribut& a);
#endif
  ~Attribut();
    
  //! Donne le nom de l'attribut
  const IN_STD string&       nom() const;

  //! Affecte un nom a l'attribut
#ifndef SWIG
  void                nommer(const IN_STD string& str);
#else
  void                nommer(const char* str);
#endif	// SWIG

  //! type des elements
  TYPE type() const;

  //! Retire la valeur d'indice i de l'attribut
  void        retirer(size_type i);
  //! Ajoute une valeur non definie a l'attribut
  void        augmenter();

  size_type   nb_elements() const;

  //! Valorisation d'un attribut entier constant
  void                set_val(int     val);
  //! Valorisation d'un attribut reel constant
  void                set_val(double  val);
  //! Valorisation d'un attribut chaine constant
  void                set_val(const IN_STD string&   val);
  //! Valorisation d'un attribut vect2d constant
  void                set_val(const Vect2d& val);
  //! Valorisation d'un attribut vect3d constant
  void                set_val(const Vect3d& val);
  //! valorisation d'un element d'un attribut entier
  void                set_val(size_type indice,int     val);
  //! valorisation d'un element d'un attribut reel
  void                set_val(size_type indice,double  val);
  //! valorisation d'un element d'un attribut chaine
  void                set_val(size_type indice,const IN_STD string& val);
  //! valorisation d'un element d'un attribut vect2d
  void                set_val(size_type indice,const Vect2d& val);
  //! valorisation d'un element d'un attribut vect3d
  void                set_val(size_type indice,const Vect3d& val);
#ifndef SWIG
  //! valorisation d'un attribut entier par un tableau
  void                set_val(const IN_STD vector<int>& tab_val);
  //! valorisation d'un attribut reel par un tableau
  void                set_val(const IN_STD vector<double>& tab_val);
  //! valorisation d'un attribut chaine par un tableau
  void                set_val(const IN_STD vector<IN_STD string>& tab_val);
  //! valorisation d'un attribut vect2d par un tableau
  void                set_val(const IN_STD vector<Vect2d>& tab_val);
  //! valorisation d'un attribut vect3d par un tableau
  void                set_val(const IN_STD vector<Vect3d>& tab_val);
#endif

  //! valeur constante d'un attribut entier
  void                get_val(int&     val) const;
  //! valeur constante d'un attribut reel
  void                get_val(double&  val) const;
  //! valeur constante d'un attribut chaine
  void                get_val(IN_STD string&   val) const;
  //! valeur constante d'un attribut vect2d
  void                get_val(Vect2d& val) const;
  //! valeur constante d'un attribut vect3d
  void                get_val(Vect3d& val) const;
  //! valeur d'un element d'un attribut entier
  void                get_val(size_type indice,int&     val) const;
  //! valeur d'un element d'un attribut reel
  void                get_val(size_type indice,double&  val) const;
  //! valeur d'un element d'un attribut chaine
  void                get_val(size_type indice,IN_STD string&   val) const;
  //! valeur d'un element d'un attribut vect2d
  void                get_val(size_type indice,Vect2d& val) const;
  //! valeur d'un element d'un attribut vect3d
  void                get_val(size_type indice,Vect3d& val) const;
#ifndef SWIG
  //! valorisation d'un attribut entier par un tableau
  void                get_val(IN_STD vector<int>&    tab_val) const;
  //! valorisation d'un attribut reel par un tableau
  void                get_val(IN_STD vector<double>& tab_val) const;
  //! valorisation d'un attribut chaine par un tableau
  void                get_val(IN_STD vector<IN_STD string>&  tab_val) const;
  //! valorisation d'un attribut vect2d par un tableau
  void                get_val(IN_STD vector<Vect2d>& tab_val) const;
  //! valorisation d'un attribut vect3d par un tableau
  void                get_val(IN_STD vector<Vect3d>& tab_val) const;
#endif

#ifndef SWIG
  const Attribut& operator = (const Attribut& a);
  bool operator == (const Attribut& a);
#endif
};

#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif
