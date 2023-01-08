#ifndef ATTRIBUT_TYPE_T
#define ATTRIBUT_TYPE_T


BEGIN_NAMESPACE_LIMA


// Fonctions utilisees pour recuperer le type des variables manipulees

// Definitions des fonctions membres de _AttributInterne<T> :

template <class T>  _AttributInterne<T>::_AttributInterne (const IN_STD string& nom)
	: _Attribut (nom), m_tab ( )
{
  m_tab.insert(m_tab.begin(),val_non_def());
}

template <class T>  Attribut::TYPE _AttributInterne<T>::type() const
{
  T t;
  return type_att(t);
}

template <class T> void _AttributInterne<T>::set_val(const T& val)
{
  m_tab.clear();
  m_tab.insert(m_tab.begin(),val);
}

template <class T> void _AttributInterne<T>::set_val(size_type indice,const T& val)
{
  if (get_max_elt()>=indice)
    {
      if (nb_elements()<=1)
        m_tab.insert(m_tab.begin(),get_max_elt()-1,*m_tab.begin());
      m_tab[indice]=val;
    }
  else
    throw att_erreur("Indice de tableau depassant les limites \n"); 
}

template <class T> void _AttributInterne<T>::set_val(const IN_STD vector<T>& tab_val)
{
  if (tab_val.size()==get_max_elt()){
    m_tab.clear();
    m_tab.insert(m_tab.begin(),tab_val.begin(),tab_val.end());
  }
  else
    throw att_erreur("Taille de tableau incompatible avec l'attribut \n");
}

template <class T> void _AttributInterne<T>::get_val(T& val)
{
  if (nb_elements()==1)
    val=m_tab[0];
  else
    throw att_erreur("Attribut non constant \n");
}

template <class T> void _AttributInterne<T>::get_val(size_type indice,T& val)
{
  if (get_max_elt()>=indice)
    {
      if (nb_elements()<=1) 
        // Si l'attribut est constant
        val=m_tab[0];
      else 
        // Si l'attribut est variable
        val=m_tab[indice];
    }
  else
    throw att_erreur("Indice de tableau depassant les limites \n"); 
}

template <class T> void _AttributInterne<T>::get_val(IN_STD vector<T>& tab_val)
{ 
  if (tab_val.size()>=get_max_elt())
    {
      int fin=get_max_elt()-1;
      if (nb_elements()>=2) {
        // Si l'attribut est variable
        for (int indice=0;indice!=fin;indice++){
          tab_val[indice]=m_tab[indice];
        } 
      } else {
        // Si l'attribut est constant
        for (int indice=0;indice!=fin;indice++){
          tab_val[indice]=m_tab[0];
        }
      }
    } 
  else
    throw att_erreur("Tableau insuffisant pour la lecture des attributs \n");
}

template <class T> void _AttributInterne<T>::retirer(size_type indice)
{
  // Si l'attribut est non constant, on enleve l'element
  if (nb_elements()>=2) {
    m_tab.erase(m_tab.begin()+indice);
  }
}

template <class T> void _AttributInterne<T>::augmenter()
{
  // Si l'attribut est non constant, on ajoute un element de valeur non definie
  if (nb_elements()>=2) {
    m_tab.push_back(val_non_def());
  }
}

template <class T> size_type _AttributInterne<T>::nb_elements() const
{
  return (size_type) m_tab.size();
}

template <class T> _Attribut* _AttributInterne<T>::copy() const
{
  return new _AttributInterne<T>(nom());
}

template <class T> T _AttributInterne<T>::val_non_def() const
{
  T t;
  return non_def(t);
}

END_NAMESPACE_LIMA

#endif	// ATTRIBUT_TYPE_T
