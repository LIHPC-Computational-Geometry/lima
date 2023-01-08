#ifndef _LISTE_T_H
#define _LISTE_T_H


BEGIN_NAMESPACE_LIMA


template<class T>
T* Liste<T>::element_id(const typename T::id_type& id) const
{
  typedef typename T::chercher chercher;

  const_iterator pos = IN_STD find_if(this->begin(), this->end(), chercher(id));
  
  if(pos != this->end() && (*pos)->id() == id)
    return *pos;
  
  throw range_erreur(T::nom_class() + IN_STD string(" ") + to_str(id) + 
		     " inconnu(e) dans le maillage\n");
}

template<class T>
bool Liste<T>::contenir(T* e) const
{
  typedef typename T::chercher chercher;
  
  const_iterator pos = IN_STD find(this->begin(), this->end(), e);
    
  if(pos != this->end())
    return true;
  
  return false;
}

template<class T>
void Liste<T>::ajouter(T* e)
{ 
  this->push_back(e);
}

template<class T>
void Liste<T>::retirer(T* e)
{
  typedef typename T::chercher chercher;

  iterator pos = IN_STD find(this->begin(), this->end(), e);
    
  if(pos != this->end())
    this->erase(pos);
}

template<class T>
size_type Liste<T>::indice(T* e)
{
  size_type    cpt=0;
  for (iterator pos=this->begin();pos!=this->end();++pos) {
    if ((*pos)->id()==e->id() && *pos == e) return cpt;
    cpt++;
  }
  return -1;
}


END_NAMESPACE_LIMA


#endif
