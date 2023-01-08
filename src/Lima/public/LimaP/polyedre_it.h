#ifndef _POLYEDRE_IT_H
#define _POLYEDRE_IT_H

#include "config_it.h"
#include "element_fini.h"
#include "allocation.h"
#include <Lima/erreur.h>
#include <Lima/polyedre.h>
#include "tableau.h"


BEGIN_NAMESPACE_LIMA


class _NoeudInterne;
class _BrasInterne;
class _PolygoneInterne;

//! Polyedre du maillage.
/*!
  Cette classe virtuelle pure décrit un polyèdre générique. Elle  
  est spécialisée dans le cas du tetraèdre, de la pyramide, du prisme
  et de l'hexaèdre. Les sous classes redéfinissent le calcul du volume 
  ou l'extraction des côtés. Afin d'instancier le bon type suivant le
  nombre de noeuds, il faut passer par la fonction create.
*/
class _PolyedreInterne : public _ElementFini
{
public:
  typedef Tableau<_NoeudInterne*>::iterator iterator_noeud;
  typedef Tableau<_NoeudInterne*>::const_iterator const_iterator_noeud;
  typedef Tableau<_BrasInterne*>::iterator iterator_bras;
  typedef Tableau<_BrasInterne*>::const_iterator const_iterator_bras;
  typedef Tableau<_PolygoneInterne*>::iterator iterator_polygone;
  typedef Tableau<_PolygoneInterne*>::const_iterator const_iterator_polygone;

  //! Utile pour les messages d'erreur.
  static const char* nom_class() { return "POLYEDRE"; }

  //! Constructeur par défaut d'un tetraèdre.
  static _PolyedreInterne*
  create (_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, 
	  _NoeudInterne* n3);    
  //! Constructeur par défaut d'une pyramide.
  static _PolyedreInterne*
  create(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, 
	 _NoeudInterne* n3,  _NoeudInterne* n4);    
  //! Constructeur par défaut d'un prisme.
  static _PolyedreInterne*
  create(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5);    
  //! Constructeur par défaut d'un hexaèdre.
  static _PolyedreInterne*
  create(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7);    
  //! Constructeur par défaut d'un heptaèdre.
  static _PolyedreInterne*
  create(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
	 _NoeudInterne* n9);    
   //! Constructeur par défaut d'un octoèdre.
  static _PolyedreInterne*
  create(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
	 _NoeudInterne* n9, _NoeudInterne* n10, _NoeudInterne* n11);    

  //! Constructeur avec indice d'un tetraèdre.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3);    
  //! Constructeur avec indice d'une pyramide.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3,  _NoeudInterne* n4);
  //! Constructeur avec indice d'un prisme.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5);
  //! Constructeur avec indice d'un hexaèdre.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7);    
  //! Constructeur avec indice d'un heptaèdre.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
	 _NoeudInterne* n9);    
  //! Constructeur avec indice d'un octoèdre.
  static _PolyedreInterne*
  create(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	 _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
	 _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
	 _NoeudInterne* n9, _NoeudInterne* n10, _NoeudInterne* n11);    

  //! Redéfinition de l'allocation mémoire.
  void* operator new(size_t size);
  //! Redéfinition de la libération mémoire.
  void operator delete(void* ptr);

  // Type de polyedre
  virtual Polyedre::PolyedreType type()=0;

  //! Nombre de noeud.
  size_type             nb_noeuds() const;
  //! Nombre de bras.
  size_type             nb_bras() const;
  //! Nombre de polygones.
  size_type             nb_polygones() const;

  //! Noeud numéro n.
  _NoeudInterne*        noeud(size_type n) const;
  //! Bras numéro n.
  _BrasInterne*         bras(size_type n) const;
  //! Polygone numéro n.
  _PolygoneInterne*     polygone(size_type n) const;

  //! Nombre potentiel d'arêtes.
  virtual size_type     nb_aretes() const = 0;
  //! Nombre potentiel de faces.
  virtual size_type     nb_faces() const = 0;

  //! Reserve la place pour les bras.
  void                  reserver_bras();
  //! Reserve la place pour les polygones.
  void                  reserver_polygones();
  //! Connecte le bras à la position pos.
  void                  connecter(_BrasInterne* br, size_type pos);
  //! Connecte le polygone à la position pos.
  void                  connecter(_PolygoneInterne* pg, size_type pos);

  //! Crée l'arête n.
  virtual _BrasInterne* extraire_arete(size_type n) const = 0;
  //! Crée l'arête n.
  virtual _PolygoneInterne* extraire_face(size_type n) const = 0;

  //! R.A.Z. des connexions.
  void                  invalider_bras();
  //! R.A.Z. des connexions.
  void                  invalider_polygones();

  //! Volume orienté du polyèdre.
  virtual double        volume() const = 0;

  //! Inverse le sens de parcours du polyèdre.
  virtual void          retourner() = 0;

  //! Recopie des noeuds seuls.
  void                  copier(const _PolyedreInterne* br);

  //! Retourne l'indice local du bras, -1 sinon.
  virtual int           contenir(const _BrasInterne* br) const = 0;
  //! Retourne l'indice local du polygone, -1 sinon.
  virtual int           contenir(const _PolygoneInterne* pg) const = 0;

  //! Iterator premier noeud.
  iterator_noeud        premier_noeud() 
    { return m_noeuds.begin(); }
  const_iterator_noeud  premier_noeud() const 
    { return m_noeuds.begin(); }

  //! Iterator dernier noeud.
  iterator_noeud        dernier_noeud() 
    { return m_noeuds.end(); }
  const_iterator_noeud  dernier_noeud() const 
    { return m_noeuds.end(); }

  //! Iterator premier bras.
  iterator_bras         premier_bras() 
    { return m_bras.begin(); }
  const_iterator_bras   premier_bras() const 
    { return m_bras.begin(); }

  //! Iterator dernier bras.
  iterator_bras         dernier_bras() 
    { return m_bras.end(); }
  const_iterator_bras dernier_bras() const 
    { return m_bras.end(); }

  //! Iterator premier polygone.
  iterator_polygone     premier_polygone() 
    { return m_polygones.begin(); }
  const_iterator_polygone premier_polygone() const 
    { return m_polygones.begin(); }

  //! Iterator dernier polygone.
  iterator_polygone     dernier_polygone() 
    { return m_polygones.end(); }
  const_iterator_polygone dernier_polygone() const 
    { return m_polygones.end(); }

  //! Comparaison des noeuds seuls.
  static bool comparer(const _PolyedreInterne* p1, const _PolyedreInterne* p2);  

  //! Remplace le noeud n1 par le noeud n2 pour chaque occurence existante de n1.
  void remplacer_noeud (_NoeudInterne* n1, _NoeudInterne* n2);

protected:
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3);
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4);
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5);
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
		   _NoeudInterne* n6, _NoeudInterne* n7);
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
		   _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
		   _NoeudInterne* n9);
  _PolyedreInterne(_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5,
		   _NoeudInterne* n6, _NoeudInterne* n7, _NoeudInterne* n8,
		   _NoeudInterne* n9, _NoeudInterne* n10, _NoeudInterne* n11);

  _PolyedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3);    
  _PolyedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3,  _NoeudInterne* n4);
  _PolyedreInterne(size_type n,
		   _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
		   _NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5);
  _PolyedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4, 
		   _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7);
  _PolyedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4, 
		   _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7,
		   _NoeudInterne* n8, _NoeudInterne* n9);
  _PolyedreInterne(size_type n, _NoeudInterne* n0, _NoeudInterne* n1, 
		   _NoeudInterne* n2, _NoeudInterne* n3, _NoeudInterne* n4, 
		   _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7,
		   _NoeudInterne* n8, _NoeudInterne* n9, _NoeudInterne* n10,
		   _NoeudInterne* n11);

  virtual bool comparer(const _PolyedreInterne* p) const = 0;
  
protected:
#ifndef LIMA_MT
  static size_type              m_max_id;
#endif	// LIMA_MT

  Tableau<_NoeudInterne*>       m_noeuds;
  Tableau<_BrasInterne*>        m_bras;
  Tableau<_PolygoneInterne*>    m_polygones;
};

inline _PolyedreInterne::_PolyedreInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (4)
#else
	: _ElementFini (0), m_noeuds (4)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
}	// _PolyedreInterne::_PolyedreInterne

inline _PolyedreInterne::_PolyedreInterne(
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3,
	_NoeudInterne* n4)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (5)
#else
	: _ElementFini (0), m_noeuds (5)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
}    	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3, 
	_NoeudInterne* n4, _NoeudInterne* n5)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (6)
#else
	: _ElementFini (0), m_noeuds (6)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3,
	_NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (8)
#else
	: _ElementFini (0), m_noeuds (8)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3,
	_NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7,
	_NoeudInterne* n8, _NoeudInterne* n9)
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (10)
#else
	: _ElementFini (0), m_noeuds (10)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
  m_noeuds[8] = n8;
  m_noeuds[9] = n9;
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	_NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2, _NoeudInterne* n3,
	_NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6, _NoeudInterne* n7,
	_NoeudInterne* n8, _NoeudInterne* n9, _NoeudInterne* n10,
	_NoeudInterne* n11) 
#ifndef LIMA_MT
	: _ElementFini (++m_max_id), m_noeuds (12)
#else
	: _ElementFini (0), m_noeuds (12)
#endif	// LIMA_MT
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
  m_noeuds[8] = n8;
  m_noeuds[9] = n9;
  m_noeuds[10] = n10;
  m_noeuds[11] = n11;
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3)
	: _ElementFini (n), m_noeuds (4)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3,  _NoeudInterne* n4)
	: _ElementFini (n), m_noeuds (5)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}    	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5)
	: _ElementFini (n), m_noeuds (6)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6,
	_NoeudInterne* n7)
	: _ElementFini (n), m_noeuds (8)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6,
	_NoeudInterne* n7, _NoeudInterne* n8, _NoeudInterne* n9)
	: _ElementFini (n), m_noeuds (10)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
  m_noeuds[8] = n8;
  m_noeuds[9] = n9;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}        	// _PolyedreInterne::_PolyedreInterne


inline _PolyedreInterne::_PolyedreInterne (
	size_type n, _NoeudInterne* n0, _NoeudInterne* n1, _NoeudInterne* n2,
	_NoeudInterne* n3, _NoeudInterne* n4, _NoeudInterne* n5, _NoeudInterne* n6,
	_NoeudInterne* n7, _NoeudInterne* n8, _NoeudInterne* n9, _NoeudInterne* n10,
	_NoeudInterne* n11): 
	 _ElementFini (n), m_noeuds (12)
{
  m_noeuds[0] = n0;
  m_noeuds[1] = n1;
  m_noeuds[2] = n2;
  m_noeuds[3] = n3;
  m_noeuds[4] = n4;
  m_noeuds[5] = n5;
  m_noeuds[6] = n6;
  m_noeuds[7] = n7;
  m_noeuds[8] = n8;
  m_noeuds[9] = n9;
  m_noeuds[10] = n10;
  m_noeuds[11] = n11;
#ifndef LIMA_MT
  m_max_id = (m_max_id<n)? n:m_max_id;
#endif	// LIMA_MT
}        	// _PolyedreInterne::_PolyedreInterne

inline
void* _PolyedreInterne::operator new(size_t)
{
  return ClassAllocateur<_PolyedreInterne>::allouer();
}
  
inline
void _PolyedreInterne::operator delete(void* ptr)
{
  ClassAllocateur<_PolyedreInterne>::liberer(ptr);    
}

inline
size_type _PolyedreInterne::nb_noeuds() const 
{ 
  return m_noeuds.size(); 
}

inline
size_type _PolyedreInterne::nb_bras() const 
{ 
  return m_bras.size(); 
}

inline
size_type _PolyedreInterne::nb_polygones() const 
{ 
  return m_polygones.size(); 
}

inline
_NoeudInterne* _PolyedreInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Polyèdre " + to_str(id()) + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds[n]; 
}

inline
_BrasInterne* _PolyedreInterne::bras(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Polyèdre " + to_str(id()) + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
  return m_bras[n]; 
}

inline
_PolygoneInterne* _PolyedreInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Polyèdre " + to_str(id()) + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones[n]; 
}

inline
void _PolyedreInterne::reserver_bras()
{
  m_bras.resize(nb_aretes());
}

inline
void _PolyedreInterne::reserver_polygones()
{
  m_polygones.resize(nb_faces());
}

inline
void _PolyedreInterne::connecter(_BrasInterne* br, size_type pos)
{
  m_bras[pos] = br;
}

inline
void _PolyedreInterne::connecter(_PolygoneInterne* pg, size_type pos)
{
  m_polygones[pos] = pg;
}

inline
void _PolyedreInterne::invalider_bras()
{
  m_bras.resize(0);
}

inline
void _PolyedreInterne::invalider_polygones()
{
  m_polygones.resize(0);
}

inline
void _PolyedreInterne::copier(const _PolyedreInterne* pd)
{
  m_noeuds = pd->m_noeuds;
}

inline
bool _PolyedreInterne::comparer(const _PolyedreInterne* p1, 
				const _PolyedreInterne* p2)
{
  return p1->comparer(p2);
}


END_NAMESPACE_LIMA

#endif
