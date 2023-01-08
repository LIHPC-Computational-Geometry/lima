#ifndef _ENSEMBLE_T_H
#define _ENSEMBLE_T_H

#include <iostream>


BEGIN_NAMESPACE_LIMA


/*
  Pour accelerer la recherche on garde constament une trace du min
  et max des ID pour savoir si la numerotation est compacte.
  Dans ce cas l'acces a un element est direct sans recherche.
*/

template<class T>
typename Ensemble<T>::iterator Ensemble<T>::position(size_type id)
{
  if(compacte()){
    size_type indice = IN_STD max(id, m_min_id);
    indice = IN_STD min(this->size(), indice-m_min_id);
    return this->begin()+indice;
  }
  _ElementFini target(id);
  return IN_STD lower_bound(this->begin(), this->end(), &target, _ElementFini::comparer_id);
}

template<class T>
typename Ensemble<T>::const_iterator Ensemble<T>::position(size_type id) const
{
  if(compacte()){
    size_type indice = IN_STD max(id, m_min_id);
    indice = IN_STD min(this->size(), indice-m_min_id);
    return this->begin()+indice;
  }
  _ElementFini target(id);
  return IN_STD lower_bound(this->begin(), this->end(), &target,  _ElementFini::comparer_id);
}

template<class T>
size_type Ensemble<T>::indice(size_type id)
{
  if(compacte()){
    size_type ind = IN_STD max(id, m_min_id);
    return IN_STD min(this->size(), ind-m_min_id);
  }
  size_type    cpt=0;
  for (iterator it=this->begin();it!=this->end();++it) {
    if ((*it)->id()==id) return cpt;
    cpt++;
  }
  return -1;
}

template<class T>
T* Ensemble<T>::element_id(const typename T::id_type& id) const
{
  if(compacte()){
    if(id <= m_max_id && id >= m_min_id)
      return element(id-m_min_id);
  }
  else{
    _ElementFini target(id);

    const_iterator pos = IN_STD lower_bound(this->begin(), this->end(), &target,
				     _ElementFini::comparer_id);

    if(pos != this->end() && (*pos)->id() == id)
      return *pos;
  }

  throw range_erreur( T::nom_class() + IN_STD string(" d'ID ") + to_str(id) +
		      " inconnu dans le groupe\n");
}

template<class T>
bool Ensemble<T>::contenir(T* e) const
{
  if(compacte()){
    return (e->id() <= m_max_id && e->id() >= m_min_id &&
	    element(e->id()-m_min_id) == e);
  }
  _ElementFini target(e->id());

  const_iterator pos = IN_STD lower_bound(this->begin(), this->end(), &target,
				   _ElementFini::comparer_id);

  return (pos != this->end() && *pos == e);
}

template<class T>
bool Ensemble<T>::ajouter(T* e)
{
  iterator pos = position(e->id());

  if(pos == this->end() || *pos != e){
    pos	= this->insert(pos, e);
    m_max_id = IN_STD max(e->id(), m_max_id);
    m_min_id = IN_STD min(e->id(), m_min_id);
	return	true;	// v 6.6.0
  }

	return false;	// v 6.6.0
}

template<class T>
bool Ensemble<T>::retirer(T* e)
{
  iterator pos = position(e->id());

	if(pos != this->end() && *pos == e)
	{
		this->erase(pos);
		return true;
	}

	return false;	// v 6.6.0
}


END_NAMESPACE_LIMA


#endif
