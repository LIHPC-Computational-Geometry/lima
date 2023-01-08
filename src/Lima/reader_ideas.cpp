#ifdef __INTERNE_IDEAS
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_ideas.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Derniere modification le : 18/12/07 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_ideas.C
  \brief Interface de lecture / ecriture pour les fichiers ideas. 
	Compatibilité jusqu'à la version 11 incluse.
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include <ctype.h>

#include "LimaP/maillage_it.h"
#include "LimaP/algorithme.h"
#include "LimaP/reader_ideas.h" 
#include "LimaP/MutableString.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

//////////////////////////////////////////////////////////////////////////////
_ReaderIdeas::_ReaderIdeas(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format) {
  
  m_file.ascii = true;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderIdeas::~_ReaderIdeas(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderIdeas::readAll ( )
{
	try
	{
		m_mai->debut_lecture (m_file.full_name, 0);
		lire ( );
		m_mai->fin_lecture (m_file.full_name, 0);
	}
	catch (const _Reader::LectureAnnulee&)
	{
		m_mai->vider ( );
		throw read_erreur ("Lecture annulée par l'application.");
	}
	catch (...)
	{
		throw;
	}
}

//////////////////////////////////////////////////////////////////////////////
void _ReaderIdeas::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Ideas.");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static const int TailleBuf = 128;
static size_t n_ligne;

//////////////////////////////////////////////////////////////////////////////
/*! \brief Recherche de toutes les occurances de D et remplace par E 
  En Fortran, l'exposant d'un nombre reel peut etre precede par D, 
  ce qui est incompatible avec le scan du C.
*/
void _ReaderIdeas::remplace_D_par_E(char* str)
{
  while((str=strchr(str, 'D'))!=NULL){
    *str = 'E';
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture d'une ligne du fichier et stockage dans le buffer.
void _ReaderIdeas::lire_ligne(FILE* fp    // pointeur de fichier
		       , char* buf // buffer
		       )
{
	// V 6.2.1 (CP) : un memset semble indispensable car le fgets ne remplit pas
	// forcément le tampon. Or certaines fonctions travaillent sur des offsets
	// du tampon (ex : sscanf(buf+4, "%d", &marqueur)) sans se soucier d'un
	// éventuel '\0' en début du chaîne et peuvent ainsi se voir perturber
	// par une lecture antérieure.
	memset (buf, '\0', TailleBuf * sizeof(char));	// V 6.2.1
  char* res = fgets(buf, TailleBuf, fp);
  if(res==NULL && !feof(fp))
    throw read_erreur("Flot de donnees interrompu ligne " +
		      to_str(n_ligne) + "\n");
    
  if(res==NULL && feof(fp))
    throw eof_erreur("Fin de fichier rencontree prematurement ligne " +
		     to_str(n_ligne) + "\n");
    
  // elimination du retour chariot
  char* fin_ligne = strchr(buf, '\n');
  if(fin_ligne!=NULL)
    *fin_ligne = '\0';
  ++n_ligne;    
}  
//////////////////////////////////////////////////////////////////////////////
//! Recherche le debut d'un nouveau dataset et retourne son numero
int _ReaderIdeas::debut_dataset(FILE* fp)
{
  int            marqueur = 0;
  int            dataset;
  char           buf[TailleBuf];
    
  try{
    // un dataset commence par '-1' en 4eme colonne
    do{
      lire_ligne(fp, buf);
		// CP, v 6.27.1, coverity : on ne teste pas sscanf, ça arrive qu'aucun
		// entier ne soit lu, on passe en ligne suivante ...
      sscanf(buf+4, "%d", &marqueur);
    }while(marqueur != -1);
  }
  catch(eof_erreur& err){
    /* on intercepte la fin de fichier car il n'y a peut etre plus 
     *  de dataset a lire. */
    return 0;
  }
    
  // lecture du numero de dataset
  lire_ligne(fp, buf);
  if(sscanf(buf, "%d", &dataset)!=1)
    throw read_erreur("Il manque le type de dataset ligne "+to_str(n_ligne)+"\n");
  
  return dataset;
}

//////////////////////////////////////////////////////////////////////////////
//! Recherche la fin du dataset
void _ReaderIdeas::fin_dataset(FILE* fp)
{
  char           buf[TailleBuf];
  int            marqueur = 0;
    
  // un dataset fini par '-1' en 4eme colonne
  do{
    lire_ligne(fp, buf);
		// CP, v 6.27.1, coverity : on ne teste pas sscanf, ça arrive qu'aucun
		// entier ne soit lu, on passe en ligne suivante ...
    sscanf(buf+4, "%d", &marqueur);
  }while(marqueur != -1);
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture d'un fichier ideas
void _ReaderIdeas::lire ( )
{
  FILE* fp = m_file.fp;
  // Dimension 3 pour les fichiers ideas
  // dimension(D3);
    
  // Compteur de ligne
  n_ligne = 1;
    
  vector<Typ> type_element;    // tableau des types des elements
    
    // boucle sur les datasets
  while(1){
    int dataset = debut_dataset(fp);
    if(feof(fp))
      break;
    switch(dataset){
    case 151:
      /* dataset du titre */
      lire_dataset_titre(fp);
      break;
    case 164:
      /* dataset des unites */
      lire_dataset_unite(fp);
      break;
    case 781:	// obsolete
    case 2411:
      /* dataset des noeuds */
      lire_dataset_noeuds(fp);
      break;
    case 780:	// obsolete
    case 2412:
      /* dataset des elements */
      lire_dataset_elements(fp, dataset, type_element);      
      break;
    case 752:	// V10, obsolete
    case 2417:	// V10, obsolete
    case 2429:	// V10 (obsolete ?)
    case 2432:	// obsolete
    case 2435:	// V8, obsolete
    case 2452:	// V9, obsolete
	case 2467:	// V10, obsolete
	case 2477:	// V11
      /* dataset des groupes */
      lire_dataset_groupes(fp, dataset, type_element);      
      break;     
    default:
      // dataset sans interet pour LIMA, on cherche la fin
      fin_dataset(fp);
    }
  }
}
  
//////////////////////////////////////////////////////////////////////////////
//! Lecture titre et date
void _ReaderIdeas::lire_dataset_titre(FILE* fp)
{	 
  char           buf[TailleBuf];
  int            l;
    
  for(l=0; l<2; ++l)
    lire_ligne(fp, buf);
    
  /* lecture titre, mise en forme */
  m_mai->titre(buf);
    
  for(l=0; l<5; ++l)
    lire_ligne(fp, buf);
    
  /* lecture date */
  m_mai->date(buf);
    
  /* saut jusqu'en fin de dataset */
  fin_dataset(fp);
}
  
//////////////////////////////////////////////////////////////////////////////
//! Lecture unite de longueur
void _ReaderIdeas::lire_dataset_unite(FILE* fp)
{	 
  char           buf[TailleBuf];
  double         unilo;
    
  /* lecture unite longueur en 2e ligne */
  for(int l=0; l<2; ++l)
    lire_ligne(fp, buf);
  // formatage des doubles
  remplace_D_par_E(buf);
    
  if(sscanf(buf, "%lf", &unilo)!=1)
    throw read_erreur("Il manque l'unite de longueur ligne "+to_str(n_ligne)+"\n");
    
  if(unilo!=0)
    m_mai->unite_longueur(1/unilo);
  else
    m_mai->unite_longueur(0);
    
  /* saut jusqu'en fin de dataset */
  fin_dataset(fp);
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des coordonnees des noeuds
void 
_ReaderIdeas::lire_dataset_noeuds(FILE* fp) 
{	 
  double         x, y, z;
  long           indice=0;
  char           buf[TailleBuf];
    
  // boucle sur les noeuds
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  while(1){
    lire_ligne(fp, buf);
    if(sscanf(buf, "%ld", &indice)!=1)
      throw read_erreur("Indice de noeud manqant ligne "+to_str(n_ligne)+"\n");

    // fin de dataset
    if(indice==-1)
      break;
      
    lire_ligne(fp, buf);
    // formatage des doubles
    remplace_D_par_E(buf);
    if(sscanf(buf, "%lf%lf%lf", &x, &y, &z)!=3)
      throw read_erreur("Coordonnees du noeuds "+to_str(indice)+" incompletes "
		       "ligne "+to_str(n_ligne)+"\n");
    if(m_mai->dimension()==D2) z=0;
    // ajout du noeud
    m_mai->ajouter(_NoeudInterne::create(indice, x, y, z));
  }
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 100., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}
  
//////////////////////////////////////////////////////////////////////////////
//! Lecture des elements finis
void 
_ReaderIdeas::lire_dataset_elements(FILE* fp, // pointeur de fichier
				      int dataset, // version du dataset
				      // types des elements
				      vector<Typ>& type)
{	 
  long           indice=0;
  int            type_element;
  int            nb_noeuds;
  char           buf[TailleBuf];
  // Maximum 8 noeuds par elements
  size_t         noeuds[8];
    
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  // boucle sur les elements
  while(1){
    // Lecture indice
    lire_ligne(fp, buf);
    if(sscanf(buf, "%ld", &indice)!=1)
      throw read_erreur("Indice de l'element manquant ligne "+to_str(n_ligne)+"\n");

    // fin de dataset
    if(indice==-1)
      break;
      
    /* les bras, les polygones et les polyedres partage la meme numerotation
     * non contigue. On stocke donc le type */
    type.resize(max(size_t(indice+1), type.size()));
      
    // Lecture nombre de noeuds et type d'element
    switch(dataset){
    case 780:
      if(sscanf(buf, "%*d%d%*d%*d%*d%*d%*d%d", &type_element, &nb_noeuds)!=2)
	throw read_erreur("Caracteristiques de l'element "+to_str(indice)+" incomplet "
			 "ligne "+to_str(n_ligne)+"\n");
      break;
    default:      
      if(sscanf(buf, "%*d%d%*d%*d%*d%d", &type_element, &nb_noeuds)!=2)
		throw read_erreur("Caracteristiques de l'element "+to_str(indice)+" incomplet "
			 "ligne "+to_str(n_ligne)+"\n");
      break;
    }
      
    // Lecture des noeuds
    switch(type_element){
    case 21:
      {
	/* bras, saut d'une ligne */
	lire_ligne(fp, buf);
	lire_ligne(fp, buf);  
	if(sscanf(buf, "%ld%ld%ld%ld%ld%ld%ld%ld", noeuds, noeuds+1, noeuds+2,
		  noeuds+3, noeuds+4, noeuds+5, noeuds+6, noeuds+7)!=nb_noeuds)
	  throw read_erreur("Caracteristiques du bras "+to_str(indice)+" incomplet "
			   "ligne "+to_str(n_ligne)+"\n");

	ajouter_bras(m_mai, noeuds, noeuds+nb_noeuds, size_type(indice));
	type[indice] = BRA;
      }
      break;
    case 41:
    case 44:
    case 91:
    case 94:
      {
	/* polygone */
	lire_ligne(fp, buf);
	if(sscanf(buf, "%ld%ld%ld%ld%ld%ld%ld%ld", noeuds, noeuds+1, noeuds+2,
		  noeuds+3, noeuds+4, noeuds+5, noeuds+6, noeuds+7)!=nb_noeuds)
	  throw read_erreur("Caracteristiques du polygone "+to_str(indice)+" incomplet "
			   "ligne "+to_str(n_ligne)+"\n");

	ajouter_polygone(m_mai, noeuds, noeuds+nb_noeuds, size_type(indice));
	type[indice] = PGN;
      }
      break;      
    case 111:
    case 112:
    case 115:
      {
	/* polyedre */
	lire_ligne(fp, buf);
	if(sscanf(buf, "%ld%ld%ld%ld%ld%ld%ld%ld", noeuds, noeuds+1, noeuds+2,
		  noeuds+3, noeuds+4, noeuds+5, noeuds+6, noeuds+7)!=nb_noeuds)
	  throw read_erreur("Caracteristiques du polyedre "+to_str(indice)+" incomplet "
			    "ligne "+to_str(n_ligne)+"\n");
	
	if(m_mai->dimension()==D3)
	  ajouter_polyedre(m_mai, noeuds, noeuds+nb_noeuds, size_type(indice));
	type[indice] = PDR;
      }
      break;
    default:
      throw read_erreur("Type d'element "+to_str(type_element)+" inconnu ligne "+ 
		       to_str(n_ligne)+"\n");
    }
  }
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}
  
//////////////////////////////////////////////////////////////////////////////
//! Lecture des groupes
void 
_ReaderIdeas::lire_dataset_groupes(FILE* fp, // pointeur de fichier
				     int dataset, // version du dataset
				     // types des elements
				     vector<Typ>& type)
{
  long           indice=0;
  int            nb_elements;
  char           buf[TailleBuf];
  char           nom[TailleBuf];
  int            te[4], elem[4];
    
  // boucle dur les groupes
  while(1){
    // Lecture indice
    lire_ligne(fp, buf);
    if(sscanf(buf, "%ld", &indice)!=1)
      throw read_erreur("Indice de l'element manqant ligne "+to_str(n_ligne)+"\n");

    // fin de dataset
    if(indice==-1)
      break;
      
    // Lecture nombre d'elements
    switch(dataset){
    case 752:
      if(sscanf(buf, "%*d%*d%*d%*d%*d%d", &nb_elements)!=1)
	throw read_erreur("Il manque le nombre d'element du groupe "+to_str(indice)+
			 " ligne "+to_str(n_ligne)+"\n");
      break;
    case 2417:
      if(sscanf(buf, "%*d%*d%*d%*d%*d%*d%d", &nb_elements)!=1)
	throw read_erreur("Il manque le nombre d'element du groupe "+to_str(indice)+
			 " ligne "+to_str(n_ligne)+"\n");
      break;
    default:
      if(sscanf(buf, "%*d%*d%*d%*d%*d%*d%*d%d", &nb_elements)!=1)
	throw read_erreur("Il manque le nombre d'element du groupe "+to_str(indice)+
			 " ligne "+to_str(n_ligne)+"\n");
      break;
    }
      
    /* nom du groupe */
//    lire_ligne(fp, nom);
	// CP, version 5.8.1 : elimination des espaces
	char	tmp [TailleBuf];
	lire_ligne (fp, tmp);
	size_t	blanks	= strspn (tmp, " \t");
	strcpy (nom, tmp + blanks);
	for (int i = strlen (nom); i > 0; i--)
		if ((0 != isspace (nom [i])) || ('\0' == nom [i]))
			nom [i]	= '\0';
		else
			break;
	
    /* lecture entites du groupe */
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, nom, 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
    switch(dataset){
    case 752:
    case 2417:
    case 2429:
    case 2432:
      {
	/* Les groupes dans IDEAS sont heterogene, ils peuvent contenir
	 * simultanement des noeuds, des bras, des polygones et des polyedres. 
	 * On les eclates pour LIMA */
	_NuageInterne* le_nuage = new _NuageInterne(nom);
	_LigneInterne* la_ligne =  new _LigneInterne(nom);
	_SurfaceInterne* la_surface = new _SurfaceInterne(nom);
	_VolumeInterne* le_volume = new _VolumeInterne(nom);
	m_mai->ajouter(le_nuage);
	m_mai->ajouter(la_ligne);
	m_mai->ajouter(la_surface);
	m_mai->ajouter(le_volume);

	/* boucle sur les entites du groupe, 4 par ligne */
	for(int l=0; l<(nb_elements+3)/4; l++){
	  lire_ligne(fp, buf);
	  // Nombre d'elements sur la ligne
	  int nb = min(4, nb_elements-4*l);
	  if(sscanf(buf, "%d%d%d%d%d%d%d%d", te, elem, te+1, elem+1,
		    te+2, elem+2, te+3, elem+3)!=nb*2)
	    throw read_erreur("Il manque un ou plusieurs elements du groupe "+
			     to_str(indice)+" ligne "+to_str(n_ligne)+"\n");

	  // Ajout de l'element au groupe correspondant
	  for(int e=0; e<nb; e++){
	    if(te[e]==7){
	      /* noeud */
	      le_nuage->ajouter(m_mai->noeud_id(elem[e]));
	    }
	    else{
	      /* element fini */
	      switch(type[elem[e]]){
	      case BRA:
		la_ligne->ajouter(m_mai->bras_id(elem[e]));
		break;
	      case PGN:
		la_surface->ajouter(m_mai->polygone_id(elem[e]));
		break;
	      case PDR:
		if(m_mai->dimension()==D3)
		  le_volume->ajouter(m_mai->polyedre_id(elem[e]));
		break;
	      }
	    }
	  }
	}
	  
	// Ajout des groupes non vide au maillage
	if(le_nuage->nb_noeuds()==0)
	  m_mai->retirer(le_nuage);
	if(la_ligne->nb_bras()==0)
	  m_mai->retirer(la_ligne);
	if(la_surface->nb_polygones()==0)
	  m_mai->retirer(la_surface);
	if(le_volume->nb_polyedres()==0)
	  m_mai->retirer(le_volume);
      }
      break;
    default:
      {
	_NuageInterne* le_nuage = new _NuageInterne(nom);
	_LigneInterne* la_ligne =  new _LigneInterne(nom);
	_SurfaceInterne* la_surface = new _SurfaceInterne(nom);
	_VolumeInterne* le_volume = new _VolumeInterne(nom);
	m_mai->ajouter(le_nuage);
	m_mai->ajouter(la_ligne);
	m_mai->ajouter(la_surface);
	m_mai->ajouter(le_volume);
	  
	/* boucle sur les entites du groupe, 2 par ligne */
	for(int l=0; l<(nb_elements+1)/2; l++){
	  lire_ligne(fp, buf);
	  // Nombre d'elements sur la ligne
	  int nb = min(2, nb_elements-2*l);
	  if(sscanf(buf, "%d%d%*d%*d%d%d%*d%*d", te, elem, te+1, elem+1)!=nb*2)
	    throw read_erreur("Il manque un ou plusieurs elements du groupe "+
			     to_str(indice)+" ligne "+to_str(n_ligne)+"\n");

	  // Ajout de l'element au groupe correspondant
	  for(int e=0; e<nb; e++){
	    if(te[e]==7){
	      /* noeud */
	      le_nuage->ajouter(m_mai->noeud_id(elem[e]));
	    }
	    else{
	      /* element fini */
	      switch(type[elem[e]]){
	      case BRA:
		la_ligne->ajouter(m_mai->bras_id(elem[e]));
		break;
	      case PGN:
		la_surface->ajouter(m_mai->polygone_id(elem[e]));
		break;
	      case PDR:
		if(m_mai->dimension()==D3)
		  le_volume->ajouter(m_mai->polyedre_id(elem[e]));
		break;
	      }
	    }
	  }
	}
	  
	// Ajout des groupes non vide au maillage
	if(le_nuage->nb_noeuds()==0)
	  m_mai->retirer(le_nuage);
	if(la_ligne->nb_bras()==0)
	  m_mai->retirer(la_ligne);
	if(la_surface->nb_polygones()==0)
	  m_mai->retirer(la_surface);
	if(le_volume->nb_polyedres()==0)
	  m_mai->retirer(le_volume);
      }
      break;
    }
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, nom, 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }
}
 

//////////////////////////////////////////////////////////////////////////////
//! Ecriture d'une ligne du fichier
void _WriterIdeas::ecrire_ligne(FILE* fp, const char* buf)
{  
  if(fputs(buf, fp)==EOF)
    throw write_erreur("Flot de donnees interrompu ligne "+to_str(n_ligne)+"\n");
	n_ligne++;	// CP Version 5.5.0
}
  
//////////////////////////////////////////////////////////////////////////////
//! Ecriture d'un fichier ideas
void _WriterIdeas::ecrire(File* fd)
{
  FILE* fp = fd->fp;
  char           buf[TailleBuf];
    
  // Compteur de ligne
  n_ligne = 1;
    
  /* ecriture en tete */
  ecrire_ligne(fp, "    -1\n   151\n");
  ecrire_ligne(fp, "\n");
  sprintf(buf, "%s\n", m_mai->titre().c_str());
  ecrire_ligne(fp, buf);
  ecrire_ligne(fp, "\n");
  sprintf(buf, "%s         6         0\n", m_mai->date().c_str());
  ecrire_ligne(fp, buf);
  sprintf(buf, "%s\n", m_mai->date().c_str());
  ecrire_ligne(fp, buf);
  string	limaVersion ("Fichier ecrit par LIMA (");
  limaVersion	+= string (lima_version ( )) + ")\n";
  ecrire_ligne (fp, limaVersion.c_str ( ));
//  ecrire_ligne(fp, "Fichier ecrit par LIMA\n");
  ecrire_ligne(fp, buf);
  ecrire_ligne(fp, "    -1\n");
    
  /* ecriture unites */
  ecrire_ligne(fp, "    -1\n   164\n");
  sprintf(buf, "         9UL=%-8.3G\n", m_mai->unite_longueur());
  ecrire_ligne(fp, buf);   
  sprintf(buf, "% 25.17E% 25.17E% 25.17E\n% 25.17E\n",
	  1/m_mai->unite_longueur(), 1.0, 1.0, 273.15);
  ecrire_ligne(fp, buf);
  ecrire_ligne(fp, "    -1\n");
    
  /* ecriture tables de proprietes physiques */
  ecrire_ligne(fp, "    -1\n   788\n");
    
  ecrire_ligne(fp, "         1        21         4\nLINEAR BEAM1\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
    
  ecrire_ligne(fp,"         2        90         7\nTHIN SHELL2\n");
  ecrire_ligne(fp, " 1.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00\n");
    
  ecrire_ligne(fp, "         3       110         4\nSOLID3\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
    
  ecrire_ligne(fp, "    -1\n");
    
  /* ecriture table de proprietes materiau */
  ecrire_ligne(fp, "    -1\n   773\n");
  ecrire_ligne(fp, "         1         1        27\nISOTROPIC1\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 1.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
    
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, "    -1\n"); 
    
  /* ecriture table des caracterisitiques de section poutre */
  ecrire_ligne(fp, "    -1\n   775\n");
  ecrire_ligne(fp, "         1         0         0\nKEY_IN SECTION1\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp, " 0.000000E+00 0.000000E+00 0.000000E+00" 
	       " 0.000000E+00 0.000000E+00 0.000000E+00\n");
  ecrire_ligne(fp,"        11         7         8        14"
	       "         1        10\n");
  ecrire_ligne(fp, "         0        45         1        11 1.000000E+00\n");
  ecrire_ligne(fp, "    -1\n");
    
  /* ecriture noeuds */
  ecrire_ligne(fp, "    -1\n   781\n");
  for(size_t n=0; n<m_mai->nb_noeuds(); n++){
    sprintf(buf,"%10ld         0         0        11\n", n+1);
    ecrire_ligne(fp, buf);
    sprintf(buf,"% 25.16E% 25.16E% 25.16E\n", 
	    m_mai->noeud(n)->x(), m_mai->noeud(n)->y(), m_mai->noeud(n)->z());
    ecrire_ligne(fp, buf);
  }
  ecrire_ligne(fp, "    -1\n");
    
  /* ecriture elements, concatenation des numeros des elements finis
   * pour avoir une numerotation commune */
  ecrire_ligne(fp, "    -1\n   780\n");
  for(size_t b=0; b<m_mai->nb_bras(); b++){
    _BrasInterne*  le_bras = m_mai->bras(b);
    sprintf(buf, "%10ld        21         1         1         1"
	    "         1         7         2\n", b+1);
    ecrire_ligne(fp, buf);
    ecrire_ligne(fp, "         0         1         1         1         1\n");
    sprintf(buf, "%10ld%10ld\n", le_bras->noeud(0)->id(), 
	    le_bras->noeud(1)->id());
    ecrire_ligne(fp, buf);
  }
  for(size_t p=0; p<m_mai->nb_polygones(); p++){
    _PolygoneInterne* le_polygone = m_mai->polygone(p);
	switch (le_polygone->nb_noeuds ( ))
	{
		case	3 :
      sprintf(buf, "%10ld        91         1         2         1"
	      "         1         8%10ld\n", p+m_mai->nb_bras()+1, 
	      le_polygone->nb_noeuds());
		break;
		case	4 :
      sprintf(buf, "%10ld        94         1         2         1"
	      "         1         8%10ld\n", p+m_mai->nb_bras()+1, 
	      le_polygone->nb_noeuds());
		break;
		default	:
		// CP Version 5.5.0 : levee d'exception.
		MutableString	message;
		message << "Type de polygone non supporte en ligne " << n_ligne
		        << " (" << le_polygone->nb_noeuds ( ) << " noeuds).";
		throw write_erreur (message.str ( ));
	}	// switch (le_polygone->nb_noeuds ( ))
    ecrire_ligne(fp, buf);
    for(size_t n=0; n<le_polygone->nb_noeuds(); n++)
      sprintf(buf+10*n, "%10ld", le_polygone->noeud(n)->id());
    sprintf(buf+10*le_polygone->nb_noeuds(), "\n");
    ecrire_ligne(fp, buf);
  }
  for(size_t d=0; d<m_mai->nb_polyedres(); d++){
    _PolyedreInterne* le_polyedre = m_mai->polyedre(d);
	switch (le_polyedre->nb_noeuds ( ))
	{
		case	4 :
      sprintf(buf, "%10ld       111         1         3         1         1"
	      "         9%10d\n", d+m_mai->nb_bras()+m_mai->nb_polygones()+1, 4);
			break;
		case	6 :
      sprintf(buf, "%10ld       112         1         3         1         1"
	      "         9%10d\n", d+m_mai->nb_bras()+m_mai->nb_polygones()+1, 6);
			break;
		case	8 :
      sprintf(buf, "%10ld       115         1         3         1         1"
	      "         9%10d\n", d+m_mai->nb_bras()+m_mai->nb_polygones()+1, 8);
			break;
		default	:
		// CP Version 5.5.0 : levee d'exception.
		MutableString	message;
		message << "Type de polyedre non supporte en ligne " << n_ligne
		        << " (" << le_polyedre->nb_noeuds ( ) << " noeuds).";
		throw write_erreur (message.str ( ));
	}	// switch (le_polyedre->nb_noeuds ( ))
    ecrire_ligne(fp, buf);
    for(size_t n=0; n<le_polyedre->nb_noeuds(); n++)
      sprintf(buf+10*n, "%10ld", le_polyedre->noeud(n)->id());
    sprintf(buf+10*le_polyedre->nb_noeuds(), "\n");
    ecrire_ligne(fp, buf);
  }
  ecrire_ligne(fp, "    -1\n");
    
  /* Ecriture des groupes, les noms des groupes sont prefixes afin d'eviter
     d'avoir des noms identiques pour des groupes d'elements differents */
  ecrire_ligne(fp, "    -1\n   752\n");
    
  size_type id;

  // boucle sur les nuages
  for(id=0; id<m_mai->nb_nuages(); ++id){
    _NuageInterne* le_nuage = m_mai->nuage(id);
    sprintf(buf, "%10ld         0         0         0         0%10ld\n", 
	    id+1, le_nuage->nb_noeuds());
    ecrire_ligne(fp, buf);
    //    ecrire_ligne(fp, "NUAGE_");
    ecrire_ligne(fp, le_nuage->nom().c_str());
    ecrire_ligne(fp, "\n");     
    /* boucle sur les entites du groupe, 4 par ligne */
    for(int l=0; l<(le_nuage->nb_noeuds()+3)/4; l++){
      // Nombre d'elements sur la ligne
      int nb = min(size_t(4), le_nuage->nb_noeuds()-4*l);
      for(int n=0; n<nb; n++)
	sprintf(buf+20*n, "         7%10ld", le_nuage->noeud(4*l+n)->id());
      sprintf(buf+20*nb, "\n");
      ecrire_ligne(fp, buf);
    }
  }
    
  // boucle sur les lignes
  for(id=0; id<m_mai->nb_lignes(); ++id){
    _LigneInterne* la_ligne = m_mai->ligne(id);
    sprintf(buf, "%10ld         0         0         0         0%10ld\n", 
	    id+m_mai->nb_nuages()+1, la_ligne->nb_bras());
    ecrire_ligne(fp, buf);
    //    ecrire_ligne(fp, "LIGNE_");
    ecrire_ligne(fp, la_ligne->nom().c_str());
    ecrire_ligne(fp, "\n");     
    /* boucle sur les entites du groupe, 4 par ligne */
    for(int l=0; l<(la_ligne->nb_bras()+3)/4; l++){
      // Nombre d'elements sur la ligne
      int nb = min(size_t(4), la_ligne->nb_bras()-4*l);
      for(int n=0; n<nb; n++)
	sprintf(buf+20*n, "         8%10ld", la_ligne->bras(4*l+n)->id());
      sprintf(buf+20*nb, "\n");
      ecrire_ligne(fp, buf);
    }
  }
    
  // boucle sur les surfaces
  for(id=0; id<m_mai->nb_surfaces(); ++id){
    _SurfaceInterne* la_surface = m_mai->surface(id);
    sprintf(buf, "%10ld         0         0         0         0%10ld\n", 
	    id+m_mai->nb_nuages()+m_mai->nb_lignes()+1, la_surface->nb_polygones());
    ecrire_ligne(fp, buf);
    //    ecrire_ligne(fp, "SURFACE_");
    ecrire_ligne(fp, la_surface->nom().c_str());
    ecrire_ligne(fp, "\n");     
    /* boucle sur les entites du groupe, 4 par ligne */
    for(int l=0; l<(la_surface->nb_polygones()+3)/4; l++){
      // Nombre d'elements sur la ligne
      int nb = min(size_t(4), la_surface->nb_polygones()-4*l);
      for(int n=0; n<nb; n++)
	sprintf(buf+20*n, "         8%10ld", 
		m_mai->nb_bras()+la_surface->polygone(4*l+n)->id());
      sprintf(buf+20*nb, "\n");
      ecrire_ligne(fp, buf);
    }
  }
    
  // boucle sur les volumes
  for(id=0; id<m_mai->nb_volumes(); ++id){
    _VolumeInterne* le_volume = m_mai->volume(id);
    sprintf(buf, "%10ld         0         0         0         0%10ld\n", 
	    id+m_mai->nb_nuages()+m_mai->nb_lignes()+m_mai->nb_surfaces()+1, 
	    le_volume->nb_polyedres());
    ecrire_ligne(fp, buf);
    //    ecrire_ligne(fp, "VOLUME_");
    ecrire_ligne(fp, le_volume->nom().c_str());
    ecrire_ligne(fp, "\n");     
    /* boucle sur les entites du groupe, 4 par ligne */
    for(int l=0; l<(le_volume->nb_polyedres()+3)/4; l++){
      // Nombre d'elements sur la ligne
      int nb = min(size_t(4), le_volume->nb_polyedres()-4*l);
      for(int n=0; n<nb; n++)
	sprintf(buf+20*n, "         8%10ld", 
		m_mai->nb_bras()+m_mai->nb_polygones()+le_volume->polyedre(4*l+n)->id());
      sprintf(buf+20*nb, "\n");
      ecrire_ligne(fp, buf);
    }
  }
    
  ecrire_ligne(fp, "    -1\n");
}


//////////////////////////////////////////////////////////////////////////////
_WriterIdeas::_WriterIdeas(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Writer(mesh, nom_fichier, format) {
//  m_file.open("wt");
  m_file.open("w");
}
//////////////////////////////////////////////////////////////////////////////
_WriterIdeas::~_WriterIdeas() {
  m_file.close();
}

//////////////////////////////////////////////////////////////////////////////
void _WriterIdeas::write(format_t ){
  ecrire(&m_file);
}

END_NAMESPACE_LIMA
#endif
