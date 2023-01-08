#include "LimaP/config_it.h"
#include "LimaP/triangle_it.h"
#include "LimaP/bras_it.h"
#include "LimaP/noeud_it.h"


BEGIN_NAMESPACE_LIMA


double _TriangleInterne::surface2D() const
{
	// ATTENTION, formule vraie en 2D axisymétrique uniquement.
  double x1 = noeud(1)->x()-noeud(0)->x();
  double y1 = noeud(1)->y()-noeud(0)->y();
  double x2 = noeud(2)->x()-noeud(1)->x();
  double y2 = noeud(2)->y()-noeud(1)->y();
    
  return x1*y2-y1*x2;
}

void _TriangleInterne::retourner()
{ 
  /* inversion du bord 12 */
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
}
  
int _TriangleInterne::contenir(const _BrasInterne* br) const
{
  if(noeud(0)==br->noeud(0)){
    if(noeud(1)==br->noeud(1))
      return 0;      
    if(noeud(2)==br->noeud(1))
      return 2;
    return -1;
  }
  else if(noeud(1)==br->noeud(0)){
    if(noeud(2)==br->noeud(1))
      return 1;      
    if(noeud(0)==br->noeud(1))
      return 0;
    return -1;    
  }
  else if(noeud(2)==br->noeud(0)){
    if(noeud(0)==br->noeud(1))
      return 2;      
    if(noeud(1)==br->noeud(1))
      return 1;
    return -1;    
  }
  return -1;
}

_BrasInterne* _TriangleInterne::element_oppose(const _BrasInterne* br) const
{
  for(const_iterator_bras ptr = premier_bras(); 
      ptr != dernier_bras(); ++ptr){
    if(*ptr == br){
      _PolygoneInterne* voisin = (*ptr)->element_oppose(this);
      if(voisin == NULL || voisin->nb_noeuds() == 3){
	_BrasInterne* res = bras_suivant(br);
	voisin = res->element_oppose(this);
	if(voisin == NULL || voisin->nb_noeuds() == 3)
	  return res;
	else
	  return bras_precedent(br);
      }
      else{
	return NULL;
      }
    }
  }

  return NULL;
}

END_NAMESPACE_LIMA
