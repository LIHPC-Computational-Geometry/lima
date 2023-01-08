#ifndef _TABLEAU_H
#define _TABLEAU_H

#include <string.h>
#include "config_it.h"
#include "allocation.h"

BEGIN_NAMESPACE_LIMA

//! Tableau de pointeurs pour le stockage des connectivités.
/*!
  Cette classe est conçue pour allouer de manière efficace un grand
  nombre de petits tableaux. Pour cela elle utilise la classe 
  Allocateur<T> pour la gestion mémoire.

  Le tableau est redimensionnable. A la différence de la classe vector
  de la STL, la classe Tableau ne contient que 2 pointeurs, un sur le
  début des données et un sur la fin. C'est la classe Allocateur qui
  renseigne la classe Tableau sur la place mémoire encore disponible
  avant de devoir effectuer une réallocation.

  Cette classe n'a pas une vocation générique, elle est spécialisée
  pour le stockage des connectivités. La stratégie d'allocation
  retenue est de faire grossir de deux éléments à la fois le tableau,
  ce qui correspond au nombres de connectivités 
*/

template <class T> class Tableau
{
public:
  typedef T value_type;
  typedef value_type* pointer;
  typedef const value_type* const_pointer;
  typedef value_type* iterator;
  typedef const value_type* const_iterator;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;

private:
  //! Pointeur sur le début des données.
  iterator start;
  //! Pointeur sur la fin (+1) des données.
  iterator finish;

public:
  //! Iterateur sur le début des données.
  iterator begin() { return start; }
  //! Iterateur sur le début des données (version const).
  const_iterator begin() const { return start; }
  //! Iterateur sur la fin (+1) des données.
  iterator end() { return finish; }
  //! Iterateur sur la fin (+1) des données (version const).
  const_iterator end() const { return finish; }
    
  //! Nombre d'éléments.
  size_type size() const { return size_type(end()-begin()); }
  //! Place disponible pour le stockage sans réallocation.
  size_type capacity() const { return Allocateur<T>::capacity(size());}
  //! Indique si le tableau est vide.
  bool empty() const { return end()==begin(); }
  //! Renvoie l'élément n.
  /*!
    \param n compris entre 0 et size()-1.
  */
  reference operator[] (size_type n) { return *(begin()+n); }
  //! Renvoie l'élément n (version const).
  const_reference operator[] (size_type n) const { return *(begin()+n); }

  //! Tableau vide.
  Tableau() : start(0), finish(0) {};
  //! Tableau de n éléments initialisés à NULL.
  Tableau(size_type n) { 
    start=Allocateur<T>::allocate(n);       
    memset(start, 0, n*sizeof(T));
    finish=start+n; 
  }   

  Tableau(const Tableau<T>& x) { 
    start=Allocateur<T>::allocate(x.size()); 
    finish=start+x.size();
    memcpy(start, x.start, size()*sizeof(T));
  };

  ~Tableau() { Allocateur<T>::deallocate(start, size()); };  
    
  Tableau<T>& operator= (const Tableau<T>& x) {
    iterator tmp = Allocateur<T>::allocate(x.size());
    memcpy(tmp, x.start, x.size()*sizeof(T));
    Allocateur<T>::deallocate(start, size());
    start = tmp;
    finish = start+x.size();
    return *this;
  };

  //! Renvoie le premier élément.
  reference front() { return *begin(); }
  //! Renvoie le premier élément (version const).
  const_reference front() const { return *begin(); }
  //! Renvoie le dernier élément.
  reference back() { return *(end()-1); }
  //! Renvoie le dernier élément (version const).
  const_reference back() const { return *(end()-1); }
    
  //! Redimensionne le tableau à n éléments.
  /*!
    Les anciens éléments sont copiés dans le nouveau tableau.
    Si le nouveau tableau est plus grand, les nouveaux éléments sont
    initialisés à NULL. Les adresses des anciens éléments deviennent
    invalides.
   */
  void resize(size_type n) {
    iterator tmp = Allocateur<T>::allocate(n);
    memset(tmp, 0, n*sizeof(T));
    memcpy(tmp, start, IN_STD min(n, size())*sizeof(T));
    Allocateur<T>::deallocate(start, size());
    start = tmp;
    finish = start+n;
  }
    
  //! Ajoute un élément à la fin du tableau et redimensionne en conséquence.
  void push_back(T x) {
    if(size() == capacity()){
      resize(size()+1);
      --finish;
    }
    *finish=x;
    ++finish;
  }
};

END_NAMESPACE_LIMA


#endif
