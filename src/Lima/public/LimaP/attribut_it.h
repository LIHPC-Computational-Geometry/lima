#ifndef _ATTRIBUT_IT_H
#define _ATTRIBUT_IT_H

#include <Lima/size.h>
#include <Lima/vect2d.h>
#include <Lima/vect3d.h>
#include "reference.h"
#include <Lima/attribut.h>

BEGIN_NAMESPACE_LIMA

// Classe _Attribut
class _Attribut : public _ObjetReference
{
  public:
  //! Constructeur d'attribut
  _Attribut(const IN_STD string& nom);
  //! Destructeur d'attribut
  virtual ~_Attribut() { }

  //! Mise a jour du nombre d'entites pointees par l'attribut
  void                set_max_elt(size_type size_max);
  //! Lecture du nombre d'entites pointees par l'attribut
  size_type           get_max_elt();

  //! Donne le nom de l'attribut
  const IN_STD string&       nom() const;

  //! Affecte un nom a l'attribut
  void                nommer(const IN_STD string& str);

  //! Teste l'egalite de deux attributs
  /*!  L'egalite est testee uniquement en fonction du nom de 
      l'attribut  */
  bool                compare(const _Attribut& att);

  //! Nom de la classe
  virtual Attribut::TYPE type() const = 0;

  //! 
  //  virtual _Attribut& operator = (const _Attribut& att);
  _Attribut& operator = (const _Attribut& att);

  //! Creation d'un attribut de meme type et de meme nom
  //  virtual _Attribut*  copy() const;

  //! Retire la valeur d'indice i de l'attribut
  virtual void        retirer(size_type i);
  //! Ajoute une valeur non definie a l'attribut
  virtual void        augmenter();

  virtual size_type   nb_elements() const {return 0;}

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
  void                set_val(size_type indice,const IN_STD string&   val);
  //! valorisation d'un element d'un attribut vect2d
  void                set_val(size_type indice,const Vect2d& val);
  //! valorisation d'un element d'un attribut vect3d
  void                set_val(size_type indice,const Vect3d& val);
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

  //! valeur constante d'un attribut entier
  void                get_val(int&     val);
  //! valeur constante d'un attribut reel
  void                get_val(double&  val);
  //! valeur constante d'un attribut chaine
  void                get_val(IN_STD string&    val);
  //! valeur constante d'un attribut vect2d
  void                get_val(Vect2d& val);
  //! valeur constante d'un attribut vect3d
  void                get_val(Vect3d& val);
  //! valeur d'un element d'un attribut entier
  void                get_val(size_type indice,int&     val);
  //! valeur d'un element d'un attribut reel
  void                get_val(size_type indice,double&  val);
  //! valeur d'un element d'un attribut chaine
  void                get_val(size_type indice,IN_STD string&    val);
  //! valeur d'un element d'un attribut vect2d
  void                get_val(size_type indice,Vect2d& val);
  //! valeur d'un element d'un attribut vect3d
  void                get_val(size_type indice,Vect3d& val);
  //! valorisation d'un attribut entier par un tableau
  void                get_val(IN_STD vector<int>&    tab_val);
  //! valorisation d'un attribut reel par un tableau
  void                get_val(IN_STD vector<double>& tab_val);
  //! valorisation d'un attribut chaine par un tableau
  void                get_val(IN_STD vector<IN_STD string>&  tab_val);
  //! valorisation d'un attribut vect2d par un tableau
  void                get_val(IN_STD vector<Vect2d>& tab_val);
  //! valorisation d'un attribut vect3d par un tableau
  void                get_val(IN_STD vector<Vect3d>& tab_val);

private:

  _Attribut (const _Attribut&);

  //! Nom de l'attribut
  IN_STD string      m_nom;

  //! Nombre d'element maximum de l'attribut
  size_type m_max_elt;
};	// class _Attribut

Attribut::TYPE type_att(int& a);
Attribut::TYPE type_att(double& a);
Attribut::TYPE type_att(IN_STD string& a);
Attribut::TYPE type_att(Vect2d& a);
Attribut::TYPE type_att(Vect3d& a);
int non_def(int& a);
double non_def(double& a);
IN_STD string non_def(IN_STD string& a);
Vect2d non_def(Vect2d& a);
Vect3d non_def(Vect3d& a);


END_NAMESPACE_LIMA

#endif
