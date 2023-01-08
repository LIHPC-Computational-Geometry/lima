#include "LimaP/nuage_it.h"
#include "LimaP/attribut_it.h"


BEGIN_NAMESPACE_LIMA

_NuageInterne::_NuageInterne(const IN_STD string& str):
  _Groupe(str),  m_liste_att_noeuds()
{
}


size_type _NuageInterne::nb_noeuds() const
{
  return m_noeuds.nb_elements();
}


size_type _NuageInterne::nb_elm() const
{
  return m_noeuds.nb_elements();
}


_NoeudInterne* _NuageInterne::noeud(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Nuage " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}

_NoeudInterne* _NuageInterne::elm(size_type n) const
{
#ifdef RANGE_CHECKING
  if(n>=nb_noeuds())
    throw range_erreur("Nuage " + nom() + 
		       ", Indice local de noeud " +
		       to_str(n) + " non valide\n");
#endif
  return m_noeuds.element(n);
}


_NoeudInterne* _NuageInterne::noeud_id(size_type n) const
{
  return m_noeuds.element_id(n);
}


void _NuageInterne::reserver(size_type n)
{
  m_noeuds.reserver(n);
}


Liste_attribut&    _NuageInterne::att_noeuds()
{
  return m_liste_att_noeuds;
}

/*! 
  Le noeud et le nuage doivent au préalable avoir été attachés au
  même maillage (_ElementMaillage::attacher).
*/
  
void _NuageInterne::ajouter(_NoeudInterne* nd)
{
  if(maillage()!=nd->maillage())
    throw range_erreur("Nuage " + nom() + ", et Noeud " + to_str(nd->id()) + 
		       " n'appartiennent pas au meme maillage\n");    
  att_noeuds().augmenter();
  m_noeuds.ajouter(nd);
  m_composition.nb_elements++;
  m_composition.detail [0]++;
}


void _NuageInterne::retirer(_NoeudInterne* nd)
{
  att_noeuds().retirer(m_noeuds.indice(nd));
  m_noeuds.retirer(nd);
  m_composition.nb_elements--;
  m_composition.detail [0]--;
}


bool _NuageInterne::contenir(_NoeudInterne* nd) const
{
  return m_noeuds.contenir(nd);
}


void               _NuageInterne::init_att_noeuds()
{
  att_noeuds().set_max_val(nb_noeuds());
}


void               _NuageInterne::init_att()
{
  init_att_noeuds();
}


END_NAMESPACE_LIMA
