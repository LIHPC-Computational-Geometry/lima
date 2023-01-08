#ifndef _ALLOCATION_T_H
#define _ALLOCATION_T_H

#include <malloc.h>
#include <Lima/erreur.h>

BEGIN_NAMESPACE_LIMA

// Initialisation des éléments statique de la classe GestionnaireMemoire.
template<size_t SIZE, size_t NB_ELEM>
typename GestionnaireMemoire<SIZE, NB_ELEM>::ElementLibre* 
GestionnaireMemoire<SIZE, NB_ELEM>::m_liste_elem = 0;

#ifdef LIMA_MT
template<size_t SIZE, size_t NB_ELEM>
				LimaMutex GestionnaireMemoire<SIZE, NB_ELEM>::m_mutex;
#endif	// LIMA_MT
  
template<size_t SIZE, size_t NB_ELEM>
typename GestionnaireMemoire<SIZE, NB_ELEM>::BlocMemoire* 
GestionnaireMemoire<SIZE, NB_ELEM>::m_liste_bloc = 0;

template<size_t SIZE, size_t NB_ELEM>
size_t GestionnaireMemoire<SIZE, NB_ELEM>::m_nb_clients = 0;

#ifdef LIMA_MT

template<class T> LimaMutex		ClassAllocateur<T>::m_mutex;
template<class T> LimaMutex		Allocateur<T>::m_mutex;

#endif	// LIMA_MT


template<size_t SIZE, size_t NB_ELEM>
void* GestionnaireMemoire<SIZE, NB_ELEM>::allouer()
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  // Si la liste des éléments libres est vide ont ajoute un bloc.
  if(!m_liste_elem)
    ajouter_bloc();
    
  // On retourne le premier élément libre de la liste.
  void* result = m_liste_elem->buf;

  ++(m_nb_clients);
  m_liste_elem = m_liste_elem->elem_suivant;

  return result;
}

template<size_t SIZE, size_t NB_ELEM>
void GestionnaireMemoire<SIZE, NB_ELEM>::liberer(void* ptr)
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  // L'élément libéré est juste ajouté à la liste des éléments libres.
  ElementLibre* elem_cur = (ElementLibre*)ptr;
  elem_cur->elem_suivant = m_liste_elem;
  m_liste_elem = elem_cur;

  // Si tous les éléments sont libres, l'ensemble des blocs est libéré.
  // Rem CP : commenter les deux lignes suivantes semble eviter la fragmentation
  // de la memoire, mais presente alors pour inconvenient de ne pas restituer de
  // la memoire qui ne sera peut etre jamais plus utilisee.
#ifndef LIMA_MT
	if(!(--m_nb_clients))
		retirer_blocs();
#endif	// LIMA_MT
}

template<size_t SIZE, size_t NB_ELEM>
void GestionnaireMemoire<SIZE, NB_ELEM>::ajouter_bloc()
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  // Allocation d'un nouveau bloc.
  BlocMemoire* new_bloc =
    (BlocMemoire*) malloc(sizeof(BlocMemoire));
  if(!new_bloc)
    throw memory_erreur("Plus de memoire !\n");

  // Ajout du bloc à la liste des blocs.
  new_bloc->bloc_suivant = m_liste_bloc;
  m_liste_bloc = new_bloc;
    
  // Ajout de l'ensemble des éléments du bloc à la liste des éléments libres.
  char* buffer = new_bloc->buf;
  m_liste_elem = (ElementLibre*) buffer;
    
  ElementLibre* elem_cur = m_liste_elem;
  for(size_t i=1; i<NB_ELEM; ++i){
    buffer += SIZE;
    elem_cur->elem_suivant = (ElementLibre*)buffer;
    elem_cur = elem_cur->elem_suivant;
  }
  elem_cur->elem_suivant = 0;
}  

template<size_t SIZE, size_t NB_ELEM>
void GestionnaireMemoire<SIZE, NB_ELEM>::retirer_blocs()
{
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  // Parcours de la liste des blocs et libération.
  do{
    BlocMemoire* bloc_courant = m_liste_bloc->bloc_suivant;
    free(m_liste_bloc);
    m_liste_bloc = bloc_courant;
  } while(m_liste_bloc);
  m_liste_elem = 0;
}
  
template<class T>
T* Allocateur<T>::allocate(size_t size, const void*)
{
  if(!size) return 0;
  
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
  // Appel du gestionnaire mémoire correspondant.
  switch(range(size)){
  case 0:
    return (T*) GestionnaireMemoire<DEUX, NB_ELEM>::allouer();
  case 1:
    return (T*) GestionnaireMemoire<QUATRE, NB_ELEM>::allouer();
  case 2:
    return (T*) GestionnaireMemoire<SIX, NB_ELEM>::allouer();
  case 3:
    return (T*) GestionnaireMemoire<HUIT, NB_ELEM>::allouer();
  case 4:
    return (T*) GestionnaireMemoire<DIX, NB_ELEM>::allouer();
  case 5:
    return (T*) GestionnaireMemoire<DOUZE, NB_ELEM>::allouer();
  default: 
    {
      T* res = (T*) malloc(capacity(size)*ATOM);
      if(!res)
	throw memory_erreur("Plus de memoire !\n");
      return res;
    }
  }
}

template<class T>
void Allocateur<T>::deallocate(T* ptr, size_t size)
{
  if(size){
#ifdef LIMA_MT
	AutoMutex	autoMutex (m_mutex);
#endif	// LIMA_MT
    // Appel du gestionnaire mémoire correspondant.
    switch(range(size)){
    case 0:
      GestionnaireMemoire<DEUX, NB_ELEM>::liberer(ptr);
      break;
    case 1:
      GestionnaireMemoire<QUATRE, NB_ELEM>::liberer(ptr);
      break;
    case 2:
      GestionnaireMemoire<SIX, NB_ELEM>::liberer(ptr);
      break;
    case 3:
      GestionnaireMemoire<HUIT, NB_ELEM>::liberer(ptr);
      break;
    case 4:
      GestionnaireMemoire<DIX, NB_ELEM>::liberer(ptr);
      break;
    case 5:
      GestionnaireMemoire<DOUZE, NB_ELEM>::liberer(ptr);
      break;
    default:
      if(ptr) free(ptr);
      break;
    }
  }
}

END_NAMESPACE_LIMA

#endif
