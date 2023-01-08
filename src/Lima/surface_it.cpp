#include "LimaP/surface_it.h"
#include "LimaP/ligne_it.h"
#include "LimaP/fonction.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA
  
_SurfaceInterne::~_SurfaceInterne()
{
  if(m_contour){
    m_contour->ref_moins();
    if(m_contour->orphelin()) 
      delete m_contour;
  }
}


_SurfaceInterne::_SurfaceInterne(const IN_STD string& str):
  _Groupe(str), m_contour(0), m_i(0), m_j(0),
  m_liste_att_noeuds(),
  m_liste_att_bras(),
  m_liste_att_polygones()
{
}


_LigneInterne* _SurfaceInterne::contour() const
{
  return m_contour;
}


size_type _SurfaceInterne::nb_noeuds() const
{
  return m_noeuds.nb_elements();
}


size_type _SurfaceInterne::nb_bras() const
{
  return m_bras.nb_elements();
}


size_type _SurfaceInterne::nb_polygones() const
{
  return m_polygones.nb_elements();
}


size_type _SurfaceInterne::nb_elm() const
{
  return m_polygones.nb_elements();
}


size_type _SurfaceInterne::nb_noeuds_grille() const
{
  return m_grille.nb_elements();
}


_NoeudInterne* _SurfaceInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Surface " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}


_BrasInterne* _SurfaceInterne::bras(size_type n) const
{
 #ifdef RANGE_CHECKING
  if(n>=nb_bras())
    throw range_erreur("Surface " + nom() + 
		       ", Indice local de bras " +
		       to_str(n) + " non valide\n");
#endif
 return m_bras.element(n);
}


_PolygoneInterne* _SurfaceInterne::polygone(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Surface " + nom() + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones.element(n);
}


_PolygoneInterne* _SurfaceInterne::elm(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_polygones())
    throw range_erreur("Surface " + nom() + 
		       ", Indice local de polygone " +
		       to_str(n) + " non valide\n");
#endif
  return m_polygones.element(n);
}


_NoeudInterne* _SurfaceInterne::noeud_grille(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds_grille())
    throw range_erreur("Surface " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_grille.element(n);
}


_NoeudInterne* _SurfaceInterne::noeud_id(size_type n) const
{
  return m_noeuds.element_id(n);
}


_BrasInterne* _SurfaceInterne::bras_id(size_type n) const
{
  return m_bras.element_id(n);
}


_PolygoneInterne* _SurfaceInterne::polygone_id(size_type n) const
{
  return m_polygones.element_id(n);
}


void _SurfaceInterne::reserver_noeuds(size_type n)
{
  m_noeuds.reserver(n);
}


void _SurfaceInterne::reserver_bras(size_type n)
{
  m_bras.reserver(n);
}


void _SurfaceInterne::reserver(size_type n)
{
  m_polygones.reserver(n);
}
  

Liste_attribut&    _SurfaceInterne::att_polygones()
{
  return m_liste_att_polygones;
}

/*! 
  Le polygone et la surface doivent au préalable avoir été attachés au
  même maillage (_ElementMaillage::attacher).
*/

void _SurfaceInterne::ajouter(_PolygoneInterne* pg)
{
 if(maillage()!=pg->maillage())
    throw range_erreur("Surface " + nom() + ", et Polygone " + to_str(pg->id()) + 
		       " n'appartiennent pas au meme maillage\n");    
  att_polygones().augmenter();
  m_polygones.ajouter(pg);
  m_composition.nb_elements++;
  m_composition.detail [pg->nb_noeuds ( )]++;
}


void _SurfaceInterne::retirer(_PolygoneInterne* pg)
{
  att_polygones().retirer(m_polygones.indice(pg));
  m_polygones.retirer(pg);
  m_composition.nb_elements--;
  m_composition.detail [pg->nb_noeuds ( )]--;
}
 

bool _SurfaceInterne::contenir(_PolygoneInterne* pg) const
{
  return m_polygones.contenir(pg);
}


bool _SurfaceInterne::contenir(_BrasInterne* br) const
{
  return m_bras.contenir(br);
}


bool _SurfaceInterne::contenir(_NoeudInterne* nd) const
{
  return m_noeuds.contenir(nd);
}


size_type _SurfaceInterne::longueur_i() const
{
  return m_i;
}


size_type _SurfaceInterne::longueur_j() const
{
  return m_j;
}


Liste_attribut&    _SurfaceInterne::att_noeuds()
{
  return m_liste_att_noeuds;
}


Liste_attribut&    _SurfaceInterne::att_bras()
{
  return m_liste_att_bras;
}


void               _SurfaceInterne::init_att_noeuds()
{
  att_noeuds().set_max_val(nb_noeuds());
}


void               _SurfaceInterne::init_att_bras()
{
  att_bras().set_max_val(nb_bras());
}


void               _SurfaceInterne::init_att_polygones()
{
  att_polygones().set_max_val(nb_polygones());
}


void               _SurfaceInterne::init_att()
{
  init_att_noeuds();
  init_att_bras();
  init_att_polygones();
}


void _SurfaceInterne::extraire_noeuds()
{
	if (0 == nb_polygones ( ))		// CP, version 5.8.0
		return;	

  extraire_elements(premier_polygone(), dernier_polygone(),
		    _les_noeuds<_PolygoneInterne>(), m_noeuds);
}

void _SurfaceInterne::extraire_bras()
{
	if (0 == nb_polygones ( ))		// CP, version 5.8.0
		return;	

  extraire_elements(premier_polygone(), dernier_polygone(),
		    _les_bras<_PolygoneInterne>(), m_bras);
}

void _SurfaceInterne::extraire_contour()
{
  if(m_contour && m_contour->orphelin()) delete m_contour;
  m_contour = new _LigneInterne("CONTOUR_"+nom());
  m_contour->attacher(maillage());
  m_contour->ref_plus();

	if (0 == nb_polygones ( ))		// CP, version 5.8.0
		return;	
  _extraire_contour(premier_polygone(), dernier_polygone(),
		    _les_bras<_PolygoneInterne>(), m_contour);
}

void _SurfaceInterne::chainer(Liste<_PolygoneInterne>& les_polygones, 
			      Liste<_BrasInterne>& les_bras)
{
  int retourne = 0;
  int nb = 0;
  
  if(les_bras.front() == NULL ||
     les_bras.front()->element_oppose(les_polygones.front()) == NULL)
    retourne = 1;

  while(nb < nb_polygones()){
    _PolygoneInterne* le_polygone = NULL;
    _BrasInterne* le_bras = les_bras.back();
    if(le_bras!=NULL){
      le_polygone = le_bras->element_oppose(les_polygones.back());
      if(le_polygone == NULL){
	if(retourne==1) return;
	else{
	  reverse(les_polygones.premier(), les_polygones.dernier());
	  reverse(les_bras.premier(), les_bras.dernier());
	  retourne = 1;	
	}
      }
      else if(le_polygone == les_polygones.front()){
	return;
      }
      else{
	nb++;
	les_polygones.push_back(le_polygone);      
	le_bras = le_polygone->element_oppose(le_bras);
	les_bras.push_back(le_bras);
      }
    }
    else{
      if(retourne==1) return;
      else{
	reverse(les_polygones.premier(), les_polygones.dernier());
	reverse(les_bras.premier(), les_bras.dernier());
	retourne = 1;	
      }
    }
  }
}

void _SurfaceInterne::structurer()
{
	if (0 == nb_polygones ( ))		// CP, version 5.8.0
		return;	

  int nb_quad = 0;
  int nb_tr = 0;

  // Verification prealable : il y a des quads
  iterator_polygone pg = premier_polygone();
  while(pg != dernier_polygone()){
    nb_quad += ((*pg)->nb_noeuds() == 4);
    nb_tr += ((*pg)->nb_noeuds() == 3);
    ++pg;
  }
  if((nb_quad == 0) || ((nb_quad+nb_tr)!=nb_polygones())){
    m_i = m_j = 0;
    return;
  }

  // table bras du groupe -> polygone du groupe
  iterator_bras br;
  for(br=premier_bras(); br!=dernier_bras(); ++br)
    (*br)->invalider_polygones();
  connexions_inverses(premier_polygone(), dernier_polygone(),
		      _les_bras<_PolygoneInterne>());
  // Un bras est connecte a au plus deux polygones
  for(br=premier_bras(); br!=dernier_bras(); ++br){
    if((*br)->nb_polygones()>2) return;
  }

  // polygone de depart
  _PolygoneInterne* le_polygone = polygone(0);

  // Une ligne
  Liste<_PolygoneInterne> ligne_polygones;
  ligne_polygones.push_back(le_polygone);

  Liste<_BrasInterne> ligne_bras;
  ligne_bras.push_back(le_polygone->element_oppose(le_polygone->bras(0)));
  ligne_bras.push_back(le_polygone->bras(0));

  chainer(ligne_polygones, ligne_bras);
  m_i = ligne_polygones.nb_elements();

  // La premiere colonne
  le_polygone = ligne_polygones.front();
  Liste<_PolygoneInterne>  colonne_polygones;
  colonne_polygones.push_back(le_polygone);

  Liste<_BrasInterne> colonne_bras;
  _BrasInterne* le_bras = le_polygone->bras_suivant(ligne_bras.element(1));
  if(le_polygone->element_oppose(le_bras) == ligne_bras.element(1))
    le_bras = le_polygone->bras_precedent(ligne_bras.element(1));
  colonne_bras.push_back(le_polygone->element_oppose(le_bras));
  colonne_bras.push_back(le_bras);

  chainer(colonne_polygones, colonne_bras);
  m_j = colonne_polygones.nb_elements();

  if(longueur_i()*longueur_j()!=nb_polygones()){
    m_i = m_j = 0;
    return;
  }

  // Les grilles
  Liste<_PolygoneInterne> grille_polygones;
  grille_polygones.redimensionner(nb_polygones());
  Liste<_BrasInterne> grille_bras;
  grille_bras.redimensionner(m_i*(m_j+1)+m_j*(m_i+1));

  for(size_t c=longueur_j(); c>0; --c){
    ligne_polygones.vider();
    ligne_bras.vider();

    le_polygone = colonne_polygones.element(c-1);
    ligne_polygones.push_back(le_polygone);

    if(!(c-1)){
      _BrasInterne* le_bras = le_polygone->bras_precedent(colonne_bras.element(1));
      if(le_polygone->element_oppose(le_bras) == colonne_bras.element(1))
	le_bras = le_polygone->bras_suivant(colonne_bras.element(1));
      ligne_bras.push_back(le_polygone->element_oppose(le_bras));
      ligne_bras.push_back(le_bras);
    }
    else{
      _BrasInterne* le_bras = le_polygone->bras_precedent(colonne_bras.element(c-1));
      if(le_polygone->element_oppose(le_bras) == colonne_bras.element(c-1))
	le_bras = le_polygone->bras_suivant(colonne_bras.element(c-1));
      ligne_bras.push_back(le_polygone->element_oppose(le_bras));
      ligne_bras.push_back(le_bras);
    }

    chainer(ligne_polygones, ligne_bras);

    if((ligne_polygones.nb_elements() != longueur_i()) |
       (ligne_polygones.front()!= le_polygone)){
      m_i = m_j = 0;
      return;
    }

    copy(ligne_polygones.premier(), ligne_polygones.dernier(),
	 grille_polygones.premier()+(c-1)*longueur_i());
    for(size_t l=0; l<=longueur_i(); ++l)
      grille_bras.element(m_i*(m_j+1)+l*m_j+(c-1)) = ligne_bras.element(l);
  }

  for(size_t l=0; l<longueur_i(); ++l){
    colonne_polygones.vider();
    colonne_bras.vider();

    le_polygone = ligne_polygones.element(l);
    colonne_polygones.push_back(le_polygone);

    if(!l){
      _BrasInterne* le_bras = le_polygone->bras_precedent(ligne_bras.element(1));
      if(le_polygone->element_oppose(le_bras) == ligne_bras.element(1))
	le_bras = le_polygone->bras_suivant(ligne_bras.element(1));
      colonne_bras.push_back(le_polygone->element_oppose(le_bras));
      colonne_bras.push_back(le_bras);
    }
    else{
      _BrasInterne* le_bras = le_polygone->bras_suivant(ligne_bras.element(l));
      if(le_polygone->element_oppose(le_bras) == ligne_bras.element(l))
	le_bras = le_polygone->bras_precedent(ligne_bras.element(l));
      colonne_bras.push_back(le_polygone->element_oppose(le_bras));
      colonne_bras.push_back(le_bras);
    }
    
    chainer(colonne_polygones, colonne_bras);

    if((colonne_polygones.nb_elements() != longueur_j()) |
       (colonne_polygones.front()!= le_polygone)){
      m_i = m_j = 0;
      return;
    }

    for(size_type c=0; c<=longueur_j(); ++c)
      grille_bras.element(c*m_i+l) = colonne_bras.element(c);
  }

  // derniere verification
  if(nb_tr != 0){
    // pole
    if( (nb_tr!=m_i) && (nb_tr!=2*m_i) && (nb_tr!=m_j) && (nb_tr!=2*m_j) ){
      m_i = m_j = 0;
      return;
    }
  }
      
  m_polygones = grille_polygones;

  iterator_bras gr = grille_bras.premier();
  br = m_bras.premier();

  while(gr!=grille_bras.dernier() && br!=m_bras.dernier()){
    if(*gr != NULL){
      *br = *gr;
      ++br;
    }
    ++gr;
  }

  m_grille.redimensionner((m_i+1)*(m_j+1));
  for(size_type j=0; j<=m_j; j++){
    size_type l = min(j, m_j-1);
    for(size_type i=0; i<=m_i; i++){
      size_type c = min(i, m_i-1);
      if(grille_bras.element(j*m_i+c) == NULL){
	c = min(i+1, m_i);
	m_grille.element(j*(m_i+1)+i) =
	  _BrasInterne::frontiere(grille_bras.element(m_i*(m_j+1)+(c-1)*m_j+l), 
				  grille_bras.element(m_i*(m_j+1)+c*m_j+l));
      }
      else if(grille_bras.element(m_i*(m_j+1)+i*m_j+l) == NULL){
	m_grille.element(j*(m_i+1)+i) =
	  _BrasInterne::frontiere(grille_bras.element((min(j+1, m_j)-1)*m_i+c),
				  grille_bras.element(min(j+1, m_j)*m_i+c));
      }
      else
	m_grille.element(j*(m_i+1)+i) =
	  _BrasInterne::frontiere(grille_bras.element(j*m_i+c), 
				  grille_bras.element(m_i*(m_j+1)+i*m_j+l));
    }
  }

  // m_noeuds = m_grille;
  iterator_noeud nd = m_noeuds.premier();
  while(nd!=m_noeuds.dernier())
    (*nd++)->set_udata(this);    

  nd = m_noeuds.premier();
  iterator_noeud gn = m_grille.premier();
  while(gn!=m_grille.dernier()){
    if ((0 != *gn) && ((*gn)->udata ( ) == this)){ // Correction CP (Lima 5.4.0)
//    if((*gn)->udata() == this){
      *nd = *gn;    
      (*nd++)->set_udata(0);    
    }
    ++gn;
  }
}

END_NAMESPACE_LIMA
