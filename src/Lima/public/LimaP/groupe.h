
#ifndef _GROUPE_H
#define _GROUPE_H

#include "config_it.h"
#include "element_maillage.h"
#include <Lima/composition.h>



BEGIN_NAMESPACE_LIMA

//! Classe de base des groupes d'éléments finis.
/*!
  A chaque groupe est associé un nom.
*/
class _Groupe : public _ElementMaillage
{
public:
  typedef IN_STD string id_type;

  //! Groupe de nom str.
  _Groupe(const IN_STD string& str);
 
  //! La composition du groupe.
  Composition composition ( ) const
  { return m_composition; }

  //! La composition du groupe : pour les readers.
  void composition (const Composition& c)
  { m_composition = c; }
 
  //! Le nom du groupe. 
  const IN_STD string& nom() const;
  const IN_STD string& id() const;

  /*! \brief Le numero d'ordre d'apparition du groupe dans le fichier.
    pour le 2eme nuage du fichier ordre() retourne 2
    pour le 2eme volume du fichier ordre() retourne 2
  */
  size_type ordre() const;
  void ordre(const size_type n);

  /*! acces a m_nb_elements */
  size_type nb_elements() const;
  void nb_elements(const size_type n);

  /*!
    \brief Nombre d'éléments du groupe (nombre de bras)
    \warning Différent de {\e nb_elements()}.

  */
  virtual size_type       nb_elm() const = 0;

  //! Nomme le groupe par la chaîne de caractère contenu dans str.
  void          nommer(const IN_STD string& str);  
   
  //! Formate le nom du groupe.
  /*! 
    Le nom du groupe est mise en forme de la manière suivante	.
    Suppression des blancs en début et fin. Changement des blancs 
    intermédiaires en '_ '. Mise en majuscules des caractères. 
  */
  void          formater_nom();
  static void   formater_nom(char* nom);

  //! Fonction de comparaison de groupes pour les algos de recherche
  static bool comparer_nom(const _Groupe* gr1, const _Groupe* gr2);

  //! Classe functor de recherche d'un groupe par sont nom.
  class chercher
  {
  private:
    //! Le nom du groupe.
    IN_STD string m_str;
    
  public:
    //! Initialisation à partir du nom du groupe.
    chercher(const IN_STD string& str): m_str(str) {;};

    //! Fonction de comparaison.
    bool operator () (const _Groupe* gr);
  };


protected:

  //! Le  numero d'ordre d'apparition du groupe dans le fichier.
  size_type m_ordre;

  /*! Le nombre d'elements constitutifs du groupe.
    sert essentiellement pour la lecture partielle de fichiers
      - si le groupe est completement charge en memoire,
        m_nb_elements egale m_noeuds.nb_elements() (dans le cas d'un nuage)
      - si le  groupe n'est pas completement charge en memoire, m_nb_elements 
        et m_noeuds.nb_elements() sont differents
         - m_nb_elements = 0 tant que le nombre d'element du groupe 
           n'a pas ete lu.
         - m_nb_elements > 0 quand le nombre d'element du groupe a ete lu 
  */
  size_type m_nb_elements;

  //! La composition du groupe.
  Composition		m_composition;


private:
  //! Le nom.
  IN_STD string		m_nom;
};

inline
_Groupe::_Groupe(const IN_STD string& str)
  : m_composition ( ), m_nom(str)
{
//  formater_nom();		// Modif CP Lima++ 5.4.4
  m_ordre					= -1;
  m_nb_elements				= 0;
  m_composition.renseigne	= true;
}

inline
const IN_STD string& _Groupe::nom() const
{
  return m_nom;
}

inline
bool _Groupe::chercher::operator () (const _Groupe* gr) 
{
  return  gr->nom()==m_str;
}

inline
const IN_STD string& _Groupe::id() const
{
  return m_nom;
}

inline
void _Groupe::nommer(const IN_STD string& str)
{
  m_nom = str;
//  formater_nom();		// Modif CP Lima++ 5.4.4
}

inline
bool _Groupe::comparer_nom(const _Groupe* gr1, const _Groupe* gr2)
{
  return gr1->nom()==gr2->nom();
}

inline
size_type _Groupe::ordre() const 
{
  return m_ordre;
}

inline
void _Groupe::ordre(const size_type n) 
{
  m_ordre = n;
}

inline
size_type _Groupe::nb_elements() const 
{
  return m_nb_elements;
}

inline
void _Groupe::nb_elements(const size_type n) 
{
  m_composition.nb_elements	= n;
  m_nb_elements				= n;
}


END_NAMESPACE_LIMA

#endif
