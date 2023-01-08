#ifndef _ALGORITHME_T_H
#define _ALGORITHME_T_H


BEGIN_NAMESPACE_LIMA

template <class Iter, class Func, class Cont> void
extraire_elements(Iter debut, Iter fin, Func extracteur, Cont& les_elements)
{
  size_type min_id=0;
  size_type max_id=0;
  Iter maille;

  // on vide la liste pour éviter de cumuler les noeuds lors d'appels successifs (EBI)
  les_elements.vider();
  
  // Amplitude.	
  for(maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      size_type id = (*element)->id();
      min_id = IN_STD min(id, min_id);            
      max_id = IN_STD max(id, max_id);
    }  
  }

  IN_STD vector<typename Func::value_type> marqueur(max_id-min_id+1, 0);

  // marquage des noeuds.	
  for(maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      size_type id = (*element)->id();
      marqueur[id-min_id] = *element; 
    }  
  }

  // Comptage.
  size_type nb_el=0;
  typename IN_STD vector<typename Func::value_type>::iterator ptr;
  for(ptr = marqueur.begin(); ptr != marqueur.end(); ++ptr){
    if(*ptr!=NULL){
      ++nb_el;
    }
  }  
   
  les_elements.reserver(nb_el);

  // Recuperation des noeuds
  for(ptr = marqueur.begin(); ptr != marqueur.end(); ++ptr){
    if(*ptr!=NULL){
      les_elements.ajouter(*ptr);
    }
  }
}

template <class Iter, class Func, class Grp> void
_extraire_contour(Iter debut, Iter fin, Func extracteur, Grp le_contour)
{
  size_type min_id=0;
  size_type max_id=0;
  Iter maille;
  
  // Amplitude.	
  for(maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      size_type id = (*element)->id();
      min_id = IN_STD min(id, min_id);            
      max_id = IN_STD max(id, max_id);
    }  
  }

  IN_STD vector<size_type> marqueur(max_id-min_id+1, 0);
    
  // marquage des bras.	
  for(maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      size_type id = (*element)->id();
      marqueur[id-min_id] += 1; 
    }  
  }

  // Comptage.
  size_type nb_el=0;
  IN_STD vector<size_type>::iterator ptr;
  for(ptr = marqueur.begin(); ptr != marqueur.end(); ++ptr){
    if(*ptr == 1){
      ++nb_el;
    }
  }  
   
  le_contour->reserver(nb_el);

  // Recuperation des bras.
  for(maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      size_type id = (*element)->id();
      if(marqueur[id-min_id] == 1){
	le_contour->ajouter(*element);
      }
    }  
  }
}

template <class Iter, class Func> void
connexions_inverses(Iter debut, Iter fin, Func extracteur)
{
  for(Iter maille = debut; maille != fin; ++maille){
    for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
      (*element)->connecter(*maille);
    }
  }
}

template <class Iter, class E> void
orienter(Iter debut, Iter fin, double (E::*encombrement)() const)
{
  for(Iter maille = debut; maille != fin; ++maille){
    E* elem = *maille;
    if((elem->*encombrement)()<0){
      elem->retourner();
    }
  }
}

#if defined(__hpux) || defined(__GNUC__)
END_NAMESPACE_LIMA
#include "maillage_it.h"
BEGIN_NAMESPACE_LIMA
#endif

template <class Iter> void
ajouter_bras(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind)
{
  switch(fin-debut){
  case 2:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      mai->ajouter(_BrasInterne::create(ind, n1, n2));
    }
    break;
  default:
    throw argument_erreur("Le bras " + to_str(ind) + " comporte zero ou "
			  "plus de deux noeuds \n");
  }
}
  
template <class Iter> void
ajouter_polygone(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind)
{
  switch(fin-debut){
  case 3:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      mai->ajouter(_PolygoneInterne::create(ind, n1, n2, n3));
    }
    break;
  case 4:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      mai->ajouter(_PolygoneInterne::create(ind, n1, n2, n3, n4));
    }
    break;
  case 5:
    {
      Tableau<_NoeudInterne*> t(5);
      t[0] = mai->noeud_id(*debut);
      ++debut;
      t[1] = mai->noeud_id(*debut);
      ++debut;
      t[2] = mai->noeud_id(*debut);
      ++debut;
      t[3] = mai->noeud_id(*debut);
      ++debut;
      t[4] = mai->noeud_id(*debut);
      mai->ajouter(_PolygoneInterne::create(ind, t));
    }
    break;
  case 6:
    {
      Tableau<_NoeudInterne*> t(6);
      t[0] = mai->noeud_id(*debut);
      ++debut;
      t[1] = mai->noeud_id(*debut);
      ++debut;
      t[2] = mai->noeud_id(*debut);
      ++debut;
      t[3] = mai->noeud_id(*debut);
      ++debut;
      t[4] = mai->noeud_id(*debut);
      ++debut;
      t[5] = mai->noeud_id(*debut);
      mai->ajouter(_PolygoneInterne::create(ind, t));
    }
    break;
  default:
    throw argument_erreur("Le polygone " + to_str(ind) + " comporte "
			  "moins de trois ou plus de quatre noeuds \n");
  }
}
  
template <class Iter> void
ajouter_polyedre(_MaillageInterne* mai, Iter debut, Iter fin, size_type ind)
{
  switch(fin-debut){
  case 4:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4));
    }
    break;
  case 5:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n5 = mai->noeud_id(*debut);
      mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4, n5));
    }
    break;
  case 6:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 =mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n5 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n6 = mai->noeud_id(*debut);
      mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4, n5, n6));
    }
    break;
  case 8:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n5 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n6 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n7 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n8 = mai->noeud_id(*debut);
      mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4, 
					    n5, n6, n7, n8));
    }
    break;
   case 10:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n5 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n6 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n7 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n8 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n9 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n10 = mai->noeud_id(*debut);
      mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4, 
					    n5, n6, n7, n8, n9, n10));
    }
    break;
   case 12:
    {
      _NoeudInterne* n1 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n2 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n3 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n4 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n5 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n6 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n7 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n8 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n9 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n10 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n11 = mai->noeud_id(*debut);
      ++debut;
      _NoeudInterne* n12 = mai->noeud_id(*debut);
     mai->ajouter(_PolyedreInterne::create(ind, n1, n2, n3, n4, n5, n6, 
					   n7, n8, n9, n10, n11, n12));
    }
    break;
 default:
    throw argument_erreur("Le polyedre " + to_str(ind) + " comporte un "
			  "nombre incorrect de noeuds \n");
    break;
  }
}

template <class Iter, class Func> size_type
nombre_connexions(Iter debut, Iter fin, Func extracteur)
{
  size_type nombre = 0;
  for(Iter maille = debut; maille != fin; ++maille){
    nombre += extracteur.dernier(*maille) - extracteur.premier(*maille);
  }
  return nombre;
}

template <class Int, class Iter, class Func> void
lire_connexions(Int* pointage, Int* liste, Iter debut, Iter fin, Func extracteur)
{
  *pointage = 1;
   for(Iter maille = debut; maille != fin; ++maille){
     Int temp = *pointage;
     for(typename Func::iterator element = extracteur.premier(*maille);
	element != extracteur.dernier(*maille); ++element){
       *liste++ = (*element)->id();
       ++temp;
     }
     *++pointage = temp;
   }
}

template <class Int, class Iter> void
lire_elements(Int* liste, Iter debut, Iter fin)
{
  for(Iter element = debut; element != fin; ++element){
    *liste++ = (*element)->id();
  }
}

template <class Cont, class Elem>
Elem* element_oppose_(const Cont& les_elements, const Elem* target)
{
  if(impaire(les_elements.size()))
    return NULL;
    
  typename Cont::const_iterator ptr = les_elements.begin();
    
  for(; ptr<les_elements.end(); ++ptr){
    if(*ptr == target){
      return
	les_elements[((ptr-les_elements.begin())+(les_elements.size()+1)/2) %
		    les_elements.size()];
    }
  }
  return NULL;
}

template <class Cont, class Elem>
Elem* element_suivant(const Cont& les_elements, const Elem* target)
{
  typename Cont::const_iterator ptr = les_elements.begin();
    
  for(; ptr<les_elements.end(); ++ptr){
    if(*ptr == target){
      return
	les_elements[(++ptr-les_elements.begin()) % les_elements.size()];
    }
  }
  return NULL;
}

template <class Cont, class Elem>
Elem* element_precedent(const Cont& les_elements, const Elem* target)
{
  typename Cont::const_iterator ptr = les_elements.begin();
    
  for(; ptr<les_elements.end(); ++ptr){
    if(*ptr == target){
      return
	les_elements[((--ptr-les_elements.begin())+les_elements.size()) %
		    les_elements.size()];
    }
  }
  return NULL;
}

END_NAMESPACE_LIMA

#endif
