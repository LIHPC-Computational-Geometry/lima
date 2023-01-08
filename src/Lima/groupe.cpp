#include <string.h>
#include <ctype.h>

#include "LimaP/groupe.h"
#include <Lima/erreur.h>

#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

void _Groupe::formater_nom()
{
  char          *nom;
    
  nom = strdup(m_nom.c_str());
  if(nom == NULL)
    throw alloc_erreur("Erreur d'allocation d'un string\n");
    
  formater_nom(nom);
    
  m_nom = string(nom);
  free(nom);
}

// Fonction static pour formater un string de la même manière qu'un groupe.
void _Groupe::formater_nom(char* nom)
{
  char          *pt1, *pt2;
  int           flag = 0;
  
  pt1 = nom;
  pt2 = nom;

  /* On saute les caracteres nom graphique du debut */
  while(isspace((unsigned char)(*pt1)) && *pt1!='\0')
    pt1++;
  
  while(*pt1!='\0'){
    /* si ce n'est pas un blanc, on copie */
    if(!isspace((unsigned char)(*pt1))){
      /* le flag sert a changer tous les blancs intermediaires en 1 seul _ */
      if(flag){
        *pt2++ = '_';
        flag = 0;
      }
      *pt2++ = toupper((unsigned char)(*pt1++));
    }
    /* on saute les blancs */
    else{
      flag = 1;
      pt1++;
    }
  }
  *pt2 = '\0';
}


END_NAMESPACE_LIMA
