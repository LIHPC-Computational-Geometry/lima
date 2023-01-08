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

#ifndef _LISTE_FICHIER_H
#define _LISTE_FICHIER_H

#include "config_it.h"
#include <Lima/enum.h>
#include <Lima/size.h>


#ifdef LIMA_MT
#include "lima_mt.h"
#endif	// LIMA_MT


BEGIN_NAMESPACE_LIMA

// Classe _ListeFichier
class _ListeFichier
{

protected:
  //! Constructeur
  _ListeFichier();
  //! Destructeur
  virtual ~_ListeFichier();

  //! Teste si le numero de maillage d'un fichier est deja ouvert
  bool    estOuvert(IN_STD string nom_fichier,size_type num, 
                    IN_STD string mode);
  //! Ajoute un numero de maillage et le nom d'un fichier a la liste
  void    ajouter(IN_STD string nom_fichier,size_type num, IN_STD string mode);
  //! Retire un numero de maillage et le nom d'un fichier de la liste
  void    retirer(IN_STD string nom_fichier,size_type num, IN_STD string mode);
  //! Teste si la liste est vide
  bool    estVide();

public:
  //! Gestion de la liste unique
  /*! Gere l'instanciation d'une unique liste de fichier
      Teste si le numero de maillage d'un fichier est deja ouvert
      Si ce n'est pas le cas, il est ajoute a la liste */
  static bool donneInstance(IN_STD string nom_fichier,size_type num, 
                    IN_STD string mode);
  //! Gestion de la liste unique
  /*! Retire un numero de maillage et le nom d'un fichier de la liste
      Si la liste est vide, elle est liberee */
  static void libereInstance(IN_STD string nom_fichier,size_type num, 
                    IN_STD string mode);

  //! Descripteur (nom de fichier, numero de maillage)
  typedef struct fichier_id{
    IN_STD string    nom_fic;
    size_type num;
    IN_STD string    mode;
  } fichier_id;

private:
  //! Pointeur sur l'instance unique de _ListeFichier
  static _ListeFichier* instanceUnique;
  //! Liste des fichiers et numeros de mallage ouverts
  IN_STD vector<fichier_id> m_liste;

#ifdef LIMA_MT
	static	LimaMutex	mutex;
#endif	// LIMA_MT
};

END_NAMESPACE_LIMA

#endif // _LISTE_FICHIER_H
