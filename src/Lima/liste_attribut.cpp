#include <Lima/liste_attribut.h>
#include "LimaP/attribut_it.h"
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

void Liste_attribut::attacher(const Attribut& a)
{ 
  if (!contenir(a)) {
    push_back(a);
    ((_Attribut*)a)->set_max_elt(max_val());
  } 
  else 
    throw att_erreur("Erreur attachement : " + a.nom() + ", element deja existant");
}

void Liste_attribut::vider()
{ 
  clear();
}

void Liste_attribut::retirer(const Attribut& a)
{ 
  iterator it,it2;
  for (it=premier(); it!=dernier(); ++it ) {
    if (*it == a)  {
      it2=it--;
      erase(it2);
    }
  }  
}

void Liste_attribut::retirer(const string& s)
{ 
  iterator it,it2;
  for (it=premier(); it!=dernier(); ++it ) {
    if (it->nom()==s)  {
      it2=it--;
      erase(it2);
    }
  }  
}

void Liste_attribut::retirer(size_type indice)
{ 
  if (indice!=-1) {
    iterator it;
    set_max_val(max_val()-1);
    for (it=premier(); it!=dernier(); ++it ){
      it->retirer(indice);
       ((_Attribut*)*it)->set_max_elt(max_val());
    }
  }
}

void Liste_attribut::augmenter()
{ 
  iterator it;
  set_max_val(max_val()+1);
  for (it=premier(); it!=dernier(); ++it ){
    it->augmenter();
    ((_Attribut*)*it)->set_max_elt(max_val());
  }
}

Attribut Liste_attribut::detacher(const string& s){
  Attribut a;
  a=element(s);
  retirer(a);
  return a;
}

bool Liste_attribut::contenir(const Attribut& a) const 
{
  return contenir(a.nom());
}

bool Liste_attribut::contenir(const string& s) const 
{
  for (const_iterator it=premier(); it!=dernier(); ++it ) {
    if (it->nom()==s) return true;
  }  
  return false;
}

Attribut Liste_attribut::element(const string& s)
{
	if (contenir(s))
	{
		iterator it;
		for ( it=premier(); it!=dernier(); ++it )
		{
			if (it->nom() == s)
			{
				return (*it);
			}
		}
	}

	// CP, v 6.27.1, coverity :
	throw att_erreur("Erreur : Element inexistant dans la liste");
}

vector<string>  Liste_attribut::liste() const
{
  const_iterator it;
  vector<string> v;
  if (nb_elements()!=0) {
    for ( it=premier(); it!=dernier(); ++it ) {
      v.push_back(it->nom());
    }  
  }
  return v;
}

END_NAMESPACE_LIMA
