#include "LimaP/quadrangle_it.h"
#include "LimaP/noeud_it.h"
#include "LimaP/bras_it.h"

BEGIN_NAMESPACE_LIMA


double _QuadrangleInterne::surface2D() const
{
	// ATTENTION, formule vraie en 2D axisyétrique uniquement.
  double x1 = noeud(1)->x()-noeud(0)->x();
  double y1 = noeud(1)->y()-noeud(0)->y();
  double x2 = noeud(2)->x()-noeud(1)->x();
  double y2 = noeud(2)->y()-noeud(1)->y();
  double surface = x1*y2-y1*x2;
    
  x1 = noeud(2)->x()-noeud(0)->x();
  y1 = noeud(2)->y()-noeud(0)->y();
  x2 = noeud(3)->x()-noeud(2)->x();
  y2 = noeud(3)->y()-noeud(2)->y();
  surface += x1*y2-y1*x2;
    
  return surface;
}
  
void _QuadrangleInterne::retourner()
{
  /* inversion du bord 12 */
  IN_STD swap(m_noeuds[0], m_noeuds[1]);
  IN_STD swap(m_noeuds[2], m_noeuds[3]);
}
  
int _QuadrangleInterne::contenir(const _BrasInterne* br) const
{
  if(noeud(0)==br->noeud(0)){
    if(noeud(1)==br->noeud(1))
      return 0;      
    if(noeud(3)==br->noeud(1))
      return 3;
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
    if(noeud(3)==br->noeud(1))
      return 2;      
    if(noeud(1)==br->noeud(1))
      return 1;
    return -1;    
  }
  else if(noeud(3)==br->noeud(0)){
    if(noeud(0)==br->noeud(1))
      return 3;      
    if(noeud(2)==br->noeud(1))
      return 2;
    return -1;    
  }
  return -1;
}

_BrasInterne* _QuadrangleInterne::element_oppose(const _BrasInterne* br) const
{
  const_iterator_bras ptr = premier_bras();
    
  for(; ptr!=dernier_bras(); ++ptr){
    if(*ptr == br){
      return 
	bras(((ptr-premier_bras())+2) % 4);
    }
  }
  return NULL;
}

END_NAMESPACE_LIMA

