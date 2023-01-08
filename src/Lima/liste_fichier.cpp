/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : liste_fichier.h
 Cree le : 06/06/01
 Auteur  : O. Bertrand
******************************************************************************/

/*!
  \file liste_fichier.h
  \brief Gestion des fichiers de maillage deja ouverts
*/

#include "LimaP/liste_fichier.h"
#include "LimaP/using_it.h"

#include <iostream>
using namespace std;

BEGIN_NAMESPACE_LIMA

#ifdef LIMA_MT
	LimaMutex		_ListeFichier::mutex;
#endif	// LIMA_MT


_ListeFichier* _ListeFichier::instanceUnique=NULL;

//////////////////////////////////////////////////////////////////////////////
_ListeFichier::_ListeFichier(){
}

//////////////////////////////////////////////////////////////////////////////
_ListeFichier::~_ListeFichier(){
}

//////////////////////////////////////////////////////////////////////////////
bool _ListeFichier::estOuvert(string nom_fichier,size_type num, string mode){
  vector<fichier_id>::iterator it;

  for (it=m_liste.begin();it!=m_liste.end();++it) {
    // Teste si un numero de maillage pour un fichier donne est deja ouvert
    if ((*it).nom_fic==nom_fichier&&(*it).num==num&&(*it).mode==mode) return true;
    // Si num=-1, teste uniquement si le fichier est ouvert
    if ( num==-1 && (*it).nom_fic==nom_fichier ) return true;
  }  
  return false;
}

//////////////////////////////////////////////////////////////////////////////
void _ListeFichier::ajouter(string nom_fichier,size_type num, string mode){
  fichier_id fic;

  fic.nom_fic=nom_fichier;
  fic.num=num;
  fic.mode=mode;
  m_liste.push_back(fic);
}

//////////////////////////////////////////////////////////////////////////////
void _ListeFichier::retirer(string nom_fichier,size_type num, string mode){
  vector<fichier_id>::iterator it1,it2;

  for (it1=m_liste.begin();it1!=m_liste.end();++it1) {
    if ((*it1).nom_fic==nom_fichier&&(*it1).num==num&&(*it1).mode==mode){
      it2=it1--;
      m_liste.erase(it2);
    }
  }  
}

//////////////////////////////////////////////////////////////////////////////
bool _ListeFichier::estVide(){
return (m_liste.size()==0);
}

//////////////////////////////////////////////////////////////////////////////
bool _ListeFichier::donneInstance(string nom_fichier,size_type num, string mode){

#ifdef LIMA_MT
	AutoMutex	autoMutex (mutex);
#endif	// LIMA_MT
  // On cree une instance si il n'y en a pas encore
  if (instanceUnique==NULL) instanceUnique = new _ListeFichier();

  // On retourne FAUX si le maillage est deja ouvert
  // Sinon, on l'ajoute a la liste des fichiers ouverts
  if (instanceUnique->estOuvert(nom_fichier,num,mode)) return false;
  instanceUnique->ajouter(nom_fichier,num,mode);
  return true;
}

//////////////////////////////////////////////////////////////////////////////
void _ListeFichier::libereInstance(string nom_fichier,size_type num, string mode){
#ifdef LIMA_MT
	AutoMutex	autoMutex (mutex);
#endif	// LIMA_MT
  if (instanceUnique!=NULL) {
    instanceUnique->retirer(nom_fichier,num,mode);
    if (instanceUnique->estVide()) {
      delete instanceUnique;
      instanceUnique=NULL;
    }
  }
}

END_NAMESPACE_LIMA
