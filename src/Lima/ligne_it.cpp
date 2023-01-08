#include "LimaP/ligne_it.h"
#include "LimaP/nuage_it.h"
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA
  
_LigneInterne::~_LigneInterne()
{
  if(m_contour){
    m_contour->ref_moins();
    if(m_contour->orphelin()) 
      delete m_contour;
  }
}


_LigneInterne::_LigneInterne(const IN_STD string& str):
  _Groupe(str), m_contour(0), m_i(0),
  m_liste_att_noeuds(), m_liste_att_bras()
{
}


_NuageInterne* _LigneInterne::contour() const
{
  return m_contour;
}


size_type _LigneInterne::nb_noeuds() const
{
  return m_noeuds.nb_elements();
}


size_type _LigneInterne::nb_elm() const
{
  return m_bras.nb_elements();
}


size_type _LigneInterne::nb_bras() const
{
  return m_bras.nb_elements();
}


_NoeudInterne* _LigneInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Ligne " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}


_BrasInterne* _LigneInterne::elm(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Ligne " + nom() + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
 return m_bras.element(n);
}


_BrasInterne* _LigneInterne::bras(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Ligne " + nom() + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
 return m_bras.element(n);
}


_NoeudInterne* _LigneInterne::noeud_id(size_type n) const
{
  return m_noeuds.element_id(n);
}


_BrasInterne* _LigneInterne::bras_id(size_type n) const
{
  return m_bras.element_id(n);
}


void _LigneInterne::reserver_noeuds(size_type n)
{
  m_noeuds.reserver(n);
}


void _LigneInterne::reserver(size_type n)
{
  m_bras.reserver(n);
}
  

Liste_attribut&    _LigneInterne::att_bras()
{
  return m_liste_att_bras;
}

/*! 
  Le bras et la ligne doivent au préalable avoir été attachés au
  même maillage (_ElementMaillage::attacher).
*/

void _LigneInterne::ajouter(_BrasInterne* br)
{
 if(maillage()!=br->maillage())
    throw range_erreur("Ligne " + nom() + ", et Bras " + to_str(br->id()) + 
		       " n'appartiennent pas au meme maillage\n");    
 att_bras().augmenter();
 m_bras.ajouter(br);
  m_composition.nb_elements++;
  m_composition.detail [br->nb_noeuds ( )]++;
}


void _LigneInterne::retirer(_BrasInterne* br)
{
  att_bras().retirer(m_bras.indice(br));
  m_bras.retirer(br);
  m_composition.nb_elements--;
  m_composition.detail [br->nb_noeuds ( )]--;
}


bool _LigneInterne::contenir(_BrasInterne* br) const
{
  return m_bras.contenir(br);
}


bool _LigneInterne::contenir(_NoeudInterne* nd) const
{
  return m_noeuds.contenir(nd);
}


size_type _LigneInterne::longueur_i() const
{
  return m_i;
}


Liste_attribut&    _LigneInterne::att_noeuds()
{
  return m_liste_att_noeuds;
}


void               _LigneInterne::init_att_noeuds()
{
  att_noeuds().set_max_val(nb_noeuds());
}


void               _LigneInterne::init_att_bras()
{
  att_bras().set_max_val(nb_bras());
}


void               _LigneInterne::init_att()
{
  init_att_noeuds();
  init_att_bras();
}


void _LigneInterne::extraire_noeuds()
{
	if (0 == nb_bras ( ))	// CP version 5.8.0
		return;

  extraire_elements(premier_bras(), dernier_bras(),
		    _les_noeuds<_BrasInterne>(), m_noeuds);
}

void _LigneInterne::extraire_contour()
{
  if(m_contour && m_contour->orphelin()) delete m_contour;
  m_contour = new _NuageInterne("CONTOUR_"+nom());
  m_contour->attacher(maillage());
  m_contour->ref_plus();

	if (0 == nb_bras ( ))	// CP version 5.8.0
		return;
  _extraire_contour(premier_bras(), dernier_bras(),
		    _les_noeuds<_BrasInterne>(), m_contour);
}

void _LigneInterne::chainer(Liste<_BrasInterne>& les_bras, 
			    Liste<_NoeudInterne>& les_noeuds)
{
  int retourne = 0;

  if(les_noeuds.front()->element_oppose(les_bras.front()) == NULL){
    retourne = 1;
  }
  else if(les_noeuds.back()->element_oppose(les_bras.front()) == NULL){
    reverse(les_noeuds.premier(), les_noeuds.dernier());
    retourne = 1;
  }
  while(1){
    _NoeudInterne* le_noeud = les_noeuds.back();
    _BrasInterne* le_bras = le_noeud->element_oppose(les_bras.back());
    if(le_bras == NULL){
      if(retourne==1) return;
      else{
	reverse(les_bras.premier(), les_bras.dernier());
	reverse(les_noeuds.premier(), les_noeuds.dernier());
	retourne = 1;	
      }
    }
    else if(le_bras == les_bras.front()){
      return;
    }
    else{
      les_bras.push_back(le_bras);
      le_noeud = le_bras->element_oppose(le_noeud);
      les_noeuds.push_back(le_noeud);
    }
  }
}

void _LigneInterne::structurer()
{
  if(nb_noeuds()!=nb_bras()+1 && nb_noeuds()!=nb_bras())
    return;
	if (0 == nb_bras ( ))	// CP version 5.8.0
		return;
  
  // table noeud du groupe -> bras du groupe
  iterator_noeud nd;
  for(nd=premier_noeud(); nd!=dernier_noeud(); ++nd)
    (*nd)->invalider_bras();
  connexions_inverses(premier_bras(), dernier_bras(),
		      _les_noeuds<_BrasInterne>());
  // Un noeud est connecte a au plus deux bras
  for(nd=premier_noeud(); nd!=dernier_noeud(); ++nd){
    if((*nd)->nb_bras()>2) return;
  }
  
#if 0
  /* bras de depart : numero mini du groupe */
  iterator_bras br = premier_bras();
  _BrasInterne* le_bras = *br;
  for(++br; br!=dernier_bras(); ++br){
    if((*br)->id()<le_bras->id())
      le_bras = *br;
  }
#endif
  _BrasInterne* le_bras = bras(0);
  
  Liste<_BrasInterne> grille_bras;
  grille_bras.push_back(le_bras);
  
  Liste<_NoeudInterne>  grille_noeuds;
  grille_noeuds.push_back(le_bras->noeud(0));
  grille_noeuds.push_back(le_bras->element_oppose(grille_noeuds.front()));
  
  chainer(grille_bras, grille_noeuds);
  
  if(grille_bras.nb_elements()==nb_bras()){
    // Compatibilite numerotation ancienne version.
    // reverse(grille_bras.premier(), grille_bras.dernier());
    // reverse(grille_noeuds.premier(), grille_noeuds.dernier());
    m_i = nb_bras();
    m_bras = grille_bras;
    copy(grille_noeuds.premier(), grille_noeuds.premier()+nb_noeuds(),
	 m_noeuds.premier());
  }
  else 
    m_i = 0;
}


void _LigneInterne::retourner ( )
{
	if (2 > nb_bras ( ))	// CP version 5.8.0
		return;

	reverse (premier_bras ( ), dernier_bras ( ));
	reverse (premier_noeud ( ), dernier_noeud ( ));
}	// _LigneInterne::retourner


double _LigneInterne::calcul_sens_rotation ( ) const
{
	double	surface	= 0.;

	for (size_type n = 0; n < nb_noeuds ( ); n++)
	{
		_NoeudInterne*	n1 (noeud (n));
		_NoeudInterne*	n2 (noeud ((n + 1) % nb_noeuds ( )));
		surface	+= n1->x ( ) * n2->y ( ) - n1->y ( ) * n2->x ( );
	}	// for (size_type n = 0; n < nb_noeuds ( ); n++)

	return surface;
}	// _LigneInterne::calcul_sens_rotation

END_NAMESPACE_LIMA
