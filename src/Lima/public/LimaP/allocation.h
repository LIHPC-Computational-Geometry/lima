#ifndef _ALLOCATION_H
#define _ALLOCATION_H

#include "config_it.h"
#include <stddef.h>


#ifdef LIMA_MT
#include "lima_mt.h"
#endif	// LIMA_MT


#if defined(__IBMCPP__)
#pragma implementation ("allocation_t.h")
#endif


BEGIN_NAMESPACE_LIMA


//! Stockage par bloc de NB_ELEM de taille SIZE.
/*!
  Cette classe gère pour les autres classes l'allocation de petits
  blocs de mémoire de taille SIZE en les regroupant par gros blocs
  de NB_ELEM éléments.

  Les gros blocs de mémoire sont alloués au fur et à mesure des
  besoins et sont chaînés entre eux.

  Les éléments libres à l'intérieur des blocs sont gérés également
  par une liste chaînée. Un compteur de référence indique le nombre
  d'éléments utilisés. Si ce compteur tombe à zéro, l'ensemble des
  blocs est libéré.
*/
template <size_t SIZE, size_t NB_ELEM>
class GestionnaireMemoire
{
public:
  //! Retourne l'adresse d'un élément libre.
  static void* allouer();
  //! Indique que l'élément n'est plus utilisé.
  static void liberer(void* ptr);

private:
  //! Bloc mémoire pour le stockage de NB_ELEM éléments de taille SIZE.
  struct BlocMemoire
  {
    //! Pointeur pour le chaînage des blocs.
    BlocMemoire* bloc_suivant;
    //! Garantie l'alignement sur un multiple de 8 octets.
    void*  aligne;
    //! L'espace mémoire.
    char   buf[SIZE*NB_ELEM];
  };

  //! Élément libre constituant un bloc.
  /*!
    Lorsque l'élément est  libre, les 4 (ou 8) premiers octets servent à
    stocker l'adresse de l'élément libre suivant.
    SIZE doit donc être supérieur à 4 (ou 8).
  */
  union ElementLibre
  {
    ElementLibre* elem_suivant;
    char buf[SIZE];
  };

  //! Ajoute un bloc si la liste des éléments libres est vide.
  static void ajouter_bloc();
  //! Libère tous les blocs si le compteur de référence est à zéro.
  static void retirer_blocs();

  //! Liste des éléments libres.
  static ElementLibre* m_liste_elem;
  //! liste des blocs mémoire.
  static BlocMemoire*  m_liste_bloc;
  //! Compteur de référence.
  static size_t        m_nb_clients;

#ifdef LIMA_MT
	static LimaMutex	m_mutex;
#endif	// LIMA_MT
};

//! Allocateur pour les objets de la classe T.
/*!
  Cette classe effectue juste la liaison avec la classe
  GestionnaireMemoire pour des éléments de taille sizeof(T).
*/
template<class T>
class ClassAllocateur
{
public:
  //! Alloue la place mémoire pour un objet de classe T.
  static void* allouer();
  //! Libère la place mémoire.
  static void liberer(void* ptr);
  //! Par défaut les objets sont alloués par bloc de NB_ELEM.
  enum {NB_ELEM=1024};


#ifdef LIMA_MT

	private :

	static LimaMutex		m_mutex;

#endif	// LIMA_MT
};

template<class T> inline void* ClassAllocateur<T>::allouer()
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
	return GestionnaireMemoire<sizeof(T), NB_ELEM>::allouer();
}

template<class T> inline void ClassAllocateur<T>::liberer(void* ptr)
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
	GestionnaireMemoire<sizeof(T), NB_ELEM>::liberer(ptr);
}

//! Allocateur pour les petits tableaux d'éléments de type T.
/*!
  Un tableau de n éléments de type T sera alloué par la classe
  GestionnaireMemoire avec SIZE=n*sizeof(T).

  On prend pour n le plus petit multiple de 2 immédiatement supérieur
  à la dimension réelle du tableau. Au delà de 12 éléments, c'est le
  malloc classique qui est utilisé.
*/
template<class T>
class Allocateur
{
public:
  typedef size_t     size_type;
  typedef ptrdiff_t  difference_type;

  //! Alloue un tableau de size éléments de type T.
  static T* allocate(size_t size, const void* hint=0);
  //! Libère un tableau de size éléments de type T.
  static void deallocate(T* ptr, size_t size);
  //! Retourne le nombre d'éléments réellement alloués.
  static size_t capacity(size_t size);

private:
  static size_t range(size_t size);

  enum {ATOM=sizeof(T)};
  enum {DEUX=2*ATOM, QUATRE=4*ATOM, SIX=6*ATOM, HUIT=8*ATOM,
	DIX=10*ATOM, DOUZE=12*ATOM};
  enum {NB_ELEM=1024};

#ifdef LIMA_MT

	static LimaMutex	m_mutex;

#endif	// LIMA_MT
};

template<class T> inline size_t Allocateur<T>::capacity(size_t size)
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  return ((size+1)/2)*2;
}

template<class T> inline
size_t Allocateur<T>::range(size_t size)
{
  return (size-1)/2;
}

END_NAMESPACE_LIMA

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "allocation_t.h"
#endif

#endif
