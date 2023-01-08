#ifdef __INTERNE_MALI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_mali.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Derniere modification le : Wed Apr 30 08:44:42 MEST 2003(C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_mali.C
  \brief Interface de lecture / ecriture pour les fichiers mali
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include "LimaP/fonction.h"
#include "LimaP/maillage_it.h"
#include "LimaP/reader_mali.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

#include <ctype.h>


BEGIN_NAMESPACE_LIMA

//////////////////////////////////////////////////////////////////////////////
_ReaderMali::_ReaderMali(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format) {
  
  m_file.ascii = true;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderMali::~_ReaderMali(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderMali::readAll ( )
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
void _ReaderMali::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Mali.");
}


static const int TailleBuf = 80; // c'est en fait le nombre de caractères utiles par ligne écrite ou lue
static size_t n_ligne;

//////////////////////////////////////////////////////////////////////////////
/*! \brief Recherche de toutes les occurances de D et remplace par E 
  En Fortran, l'exposant d'un nombre reel peut etre precede par D, 
  ce qui est incompatible avec le scan du C.
*/
void _ReaderMali::remplace_D_par_E(char* str)
{
  while((str=strchr(str, 'D'))!=NULL){
    *str = 'E';
  }
}

//////////////////////////////////////////////////////////////////////////////
//! recherche du premier caractere non blanc
char* _ReaderMali::passer_blanc(char* buf){
  while(*buf!='\0' && isspace(*buf))
    ++buf;
  return buf;
}

 
//////////////////////////////////////////////////////////////////////////////
/*! Lecture d'une ligne du fichier et stockage dans le buffer.
  Les lignes de commentaires sont eliminees */
void _ReaderMali::lire_ligne(FILE* fp    // pointeur de fichier
		       , char* buf // buffer
			   , int taille // longueur du buffer à lire (sa taille est de taille+1)
		       )
{
  do{
    char* res = fgets(buf, taille+1, fp); // lecture de taille caractères
    if(res==NULL && !feof(fp)){
      throw read_erreur("Flot de donnees interrompu ligne " +
			to_str(n_ligne) + "\n");
    }
    if(res==NULL && feof(fp)){
      throw eof_erreur("Fin de fichier rencontree prematurement ligne " +
		       to_str(n_ligne) + "\n");
    }
    // elimination du retour chariot
    char* fin_ligne = strchr(buf, '\n');
    if(fin_ligne!=NULL)
      *fin_ligne = '\0';
    ++n_ligne;
      
  }while(*(passer_blanc(buf)) == '!'); // commentaire commence par !
  //std::cout << "lire_ligne => "<<buf<<std::endl;
}
  
//////////////////////////////////////////////////////////////////////////////
//! Recherche le debut d'un nouveau bloc et retourne sont nom
const char* _ReaderMali::debut_bloc(FILE* fp, // pointeur de fichier
			      char* buf // buffer
			      )
{
  // recherche du premier caractere nom blanc
  char* bloc = passer_blanc(buf);
    
    // un bloc commence par '##' en 1ere colonne
  while(strncmp("##", bloc, 2) != 0){
    lire_ligne(fp, buf, TailleBuf+1);
    bloc = passer_blanc(buf);
  }
    
  // recherche du premier caractere nom blanc
  bloc += 2;
  bloc = passer_blanc(bloc);
    
  return bloc;
}

//////////////////////////////////////////////////////////////////////////////
//! Recherche le debut d'un nouvel attribut et retourne sont nom
const char* _ReaderMali::debut_attribut(FILE* fp, // pointeur de fichier
                                         char* buf // buffer
				  )
{
  // recherche du premier caractere nom blanc
  char* attrib = passer_blanc(buf);
    
    // un attribut commence par '#' en 1ere colonne
  while(*attrib != '#'){
    lire_ligne(fp, buf, TailleBuf+1);
    attrib = passer_blanc(buf);
  }
    
  // recherche du premier caractere nom blanc
  attrib += 1;
  attrib = passer_blanc(attrib);
    
  //std::cout<<"debut_attribut => "<<attrib<<std::endl;
  return attrib;
}
  
//////////////////////////////////////////////////////////////////////////////
void _ReaderMali::lire ( )
{

  FILE* fp = m_file.fp;
  size_type         eta[QTYP+1][QTYP];
  forma_t       format;
  char           buf[TailleBuf+1+1]; // +1 pour \n et +1 pour \0
  *buf = '\0';
    
  try{
    while(strncmp(debut_bloc(fp, buf), "DEBUT MAILLAGE", 
		  strlen("DEBUT MAILLAGE"))!=0)
      lire_ligne(fp, buf, TailleBuf+1);
  }
  catch(erreur& err){
    throw read_erreur(string("Bloc DEBUT MAILLAGE absent ou bloc "
			     "dans le desordre\n") + err.what());
  }
  lire_bloc_maillage(fp, buf);
    
  try{
    while(strncmp(debut_bloc(fp, buf), "TABLEAU ETAT", 
		  strlen("TABLEAU ETAT"))!=0)
      lire_ligne(fp, buf, TailleBuf+1);
  }
  catch(erreur& err){
    throw read_erreur(string("Bloc TABLEAU ETAT absent ou bloc "
			     "dans le desordre\n") + err.what());
  }
  lire_bloc_etat(fp, buf, eta);
  m_mai->dimension(dim_t(eta[GRO][GRO]));
    
  try{
    while(strncmp(debut_bloc(fp, buf), "FORMATS", strlen("FORMATS"))!=0)
      lire_ligne(fp, buf, TailleBuf+1);
  }
  catch(erreur& err){
    throw read_erreur(string("Bloc FORMAT absent ou bloc "
				 "dans le desordre\n") + err.what());
  }
  lire_bloc_format(fp, buf, &format);
    
  try{
    while(strncmp(debut_bloc(fp, buf), "COORDONNEES", 
		  strlen("COORDONNEES"))!=0)
      lire_ligne(fp, buf, TailleBuf+1);
  }
  catch(erreur& err){
    throw read_erreur(string("Bloc COORDONNEES absent ou bloc "
			     "dans le desordre\n") + err.what());
  }
  lire_bloc_coordonnees(fp, buf, eta, &format);
    
  try{
    while(strncmp(debut_bloc(fp, buf), "TABLES", strlen("TABLES"))!=0)
      lire_ligne(fp, buf, TailleBuf+1);
  }
  catch(erreur& err){
    throw read_erreur(string("Bloc TABLES absent ou bloc "
			     "dans le desordre\n") + err.what());
  }
  lire_bloc_tables(fp, buf, eta, &format);
    
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Parcours bloc DEBUT MAILLAGE */
void _ReaderMali::lire_bloc_maillage(FILE* fp, char* buf)
{
  lire_attr_titre(fp, buf);
    
  const char* attribut;
  do{
    attribut = debut_attribut(fp, buf);
    if(strncmp(attribut, "UNITE LONGUEUR", strlen("UNITE LONGUEUR"))==0)
      lire_attr_longueur(fp, buf);
    else if(strncmp(attribut, "DATE", strlen("DATE"))==0)
      lire_attr_date(fp, buf);
    else if(strncmp(attribut, "UNITE ANGLE", strlen("UNITE ANGLE"))==0)
      lire_attr_angle(fp, buf);
    else if(strncmp(attribut, "TYPE COORDONNEES", 
		    strlen("TYPE COORDONNEES"))==0)
      lire_attr_coordonnees(fp, buf);
    else if(strncmp(attribut, "TYPE GEOMETRIE",
		    strlen("TYPE GEOMETRIE"))==0)
      lire_attr_geometrie(fp, buf);
  }while(*attribut != '#');
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture UNITE LONGUEUR */
void _ReaderMali::lire_attr_longueur(FILE* fp, char* buf)
{
  double         unilo;
    
  lire_ligne(fp, buf, TailleBuf+1);
  remplace_D_par_E(buf);
  if(sscanf(buf, "%lf", &unilo)!=1){
    throw read_erreur("Il manque l'unite de longueur ligne" + 
		      to_str(n_ligne) + "\n");
  }
  m_mai->unite_longueur(unilo);
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture DATE */
void _ReaderMali::lire_attr_date(FILE* fp, char* buf)
{
  lire_ligne(fp, buf, TailleBuf+1);
  char* la_date = passer_blanc(buf);
  m_mai->date(la_date);
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture UNITE ANGLE */
void _ReaderMali::lire_attr_angle(FILE* fp, char* buf)
{
  double         unia;
    
  lire_ligne(fp, buf, TailleBuf+1);
  remplace_D_par_E(buf);
  if(sscanf(buf, "%lf", &unia)!=1){
    throw read_erreur("Il manque l'unite d'angle ligne " +
		      to_str(n_ligne) + "\n");
  }
  m_mai->unite_angle(unia);
}
  
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture TYPE COORDONNEES */
void _ReaderMali::lire_attr_coordonnees(FILE* fp, char* buf)
{
  lire_ligne(fp, buf, TailleBuf+1);
  char* coor = passer_blanc(buf);
    
  if(!strncmp(coor, "CARTESIEN", strlen("CARTESIEN")))
    m_mai->type_coordonnee(CARTESIEN);
  else if(!strncmp(coor, "CYLINDRIQUE", strlen("CYLINDRIQUE")))
    m_mai->type_coordonnee(CYLINDRIQUE);
  else{
    throw read_erreur(string("TYPE COORDONNEES inconnue ")+ coor + 
		      "ligne " + to_str(n_ligne) + "\n");
  }
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture TYPE GEOMETRIE */
void _ReaderMali::lire_attr_geometrie(FILE* fp, char* buf)
{
  lire_ligne(fp, buf, TailleBuf+1);
  char* geom = passer_blanc(buf);
    
  if(!strncmp(geom, "NORMAL", strlen("NORMAL")))
    m_mai->type_geometrie(NORMAL);
  else if(!strncmp(geom, "AXISYMETRIQUE", strlen("AXISYMETRIQUE")))
    m_mai->type_geometrie(AXISYMETRIQUE);
  else if(!strncmp(geom, "PERIODIQUE", strlen("PERIODIQUE")))
    m_mai->type_geometrie(PERIODIQUE);
  else{
    throw read_erreur(string("TYPE GEOMETRIE inconnue ")+ geom + 
		      "ligne " + to_str(n_ligne) + "\n");
  }
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture titre */
void _ReaderMali::lire_attr_titre(FILE* fp, char* buf)
{
  lire_ligne(fp, buf, TailleBuf+1);
  char* le_titre = passer_blanc(buf);
  if(*le_titre != '#')
    m_mai->titre(le_titre);
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture TABLEAU ETAT */
void _ReaderMali::lire_bloc_etat(FILE* fp, char* buf,
				   size_type eta[][_ReaderMali::QTYP])
{
  for(int l=0; l<=QTYP; ++l){
    lire_ligne(fp, buf, TailleBuf+1);
    if(sscanf(buf, "%10ld%10ld%10ld%10ld%10ld", eta[l], eta[l]+1, eta[l]+2,
	      eta[l]+3, eta[l]+4) != QTYP){
      throw read_erreur("Tableau d'etat incomplet ligne " +
			to_str(n_ligne) + "\n");
    }
  }
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture FORMATS */
void _ReaderMali::lire_bloc_format(FILE* fp, char* buf,
				     _ReaderMali::forma_t* format)
{
  lire_ligne(fp, buf, TailleBuf+1);
  if(sscanf(buf, "%10d%10d", &(format->nb_int), &(format->nc_int)) != 2)
    throw read_erreur("Format incomplet ligne " + to_str(n_ligne) + "\n");

  lire_ligne(fp, buf, TailleBuf+1);
  if(sscanf(buf, "%10d%10d", &(format->nb_float), &(format->nc_float)) != 2)
    throw read_erreur("Format incomplet ligne " + to_str(n_ligne) + "\n");

  lire_ligne(fp, buf, TailleBuf+1);
  if(sscanf(buf, "%10d%10d",
	    &(format->nb_string), &(format->nc_string)) != 2)
    throw read_erreur("Format incomplet ligne " + to_str(n_ligne) + "\n");
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture des COORDONNEES */
void
_ReaderMali::lire_bloc_coordonnees(FILE* fp, char* buf,
				     const size_type eta[][_ReaderMali::QTYP],
				     const _ReaderMali::forma_t* format)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  char str[TailleBuf+1+1];

  // On cree d'abord les noeuds
  for(size_t n=1; n<=eta[NOE][NOE]; n++)
    m_mai->ajouter(_NoeudInterne::create(n));

  size_type nb_lignes = (eta[NOE][NOE]*eta[GRO][GRO]+
			 format->nb_float-1)/format->nb_float;
    
  double coor;
  size_type c		= 0;
  // Nombre de % a faire pour callback sur lecture :
  const double	p_cible		= .1;
  double		pourcentage	= 0., p_partiel = 0.;
  // % effectue a chaque lecture :
  const double	p_unitaire	= 0 == nb_lignes ? 0. : 1. / nb_lignes;
  for(size_type l=0; l<nb_lignes; ++l){
    lire_ligne(fp, buf, TailleBuf+1);
    remplace_D_par_E(buf);
    size_type nb = min(size_type(format->nb_float),
		       eta[NOE][NOE]*eta[GRO][GRO] -l*format->nb_float);
    for(int f=0; f<nb; ++f){
      memcpy(str, buf+f*format->nc_float, format->nc_float);
      str[format->nc_float] = '\0';

      if(strlen(buf)<f*format->nc_float ||
	 sscanf(str, "%lf", &coor)!=1)
	throw read_erreur("Coordonnees incorrect ligne "+to_str(n_ligne)+"\n");
	
      switch(c%eta[GRO][GRO]){
      case 0:
	m_mai->noeud(c/eta[GRO][GRO])->set_x(coor);
	break;
      case 1:
	m_mai->noeud(c/eta[GRO][GRO])->set_y(coor);
	break;
      case 2:
	m_mai->noeud(c/eta[GRO][GRO])->set_z(coor);
	break;
      }
      ++c;
    }	// for(int f=0; f<nb; ++f)

	p_partiel	+= p_unitaire;
	if ((p_partiel >= p_cible) || (l == nb_lignes - 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "",
		                     pourcentage * 100., cancel);
		p_partiel	= 0.;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }	// for(size_type l=0; l<nb_lignes; ++l)
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Parcours bloc TABLES */
void _ReaderMali::lire_bloc_tables(FILE* fp, char* buf,
				     const size_type eta[][_ReaderMali::QTYP],
				     const _ReaderMali::forma_t* format)
{
  lire_ligne(fp, buf, TailleBuf+1);
    
  const char* attribut;
  do{
    attribut = debut_attribut(fp, buf);
    if(strncmp(attribut, "TABLE", strlen("TABLE"))==0)
      lire_attr_table(fp, buf, eta, format);
    if(strncmp(attribut, "ATTRIBUT NOM", strlen("ATTRIBUT NOM"))==0)
      lire_attr_nom(fp, buf, eta, format);
  }while(*attribut != '#');
}
  
//////////////////////////////////////////////////////////////////////////////
//! Lecture de la table de connectivite.
void _ReaderMali::lire_attr_table(FILE* fp, char* buf,
				    const size_type eta[][_ReaderMali::QTYP],
				    const _ReaderMali::forma_t* format)
{
//	std::cout<<"lire_attr_table..."<<std::endl;
//	std::cout<<"format->nb_int = "<<format->nb_int<<std::endl;
//	std::cout<<"format->nc_int = "<<format->nc_int<<std::endl;
//	std::cout<<" TailleBuf = "<<TailleBuf<<std::endl;

	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  char str[TailleBuf+1+1];

  lire_ligne(fp, buf, TailleBuf+1);
  int i, j;

  if(sscanf(buf, "%10d%10d", &i, &j) != 2)
    throw read_erreur("Indice I et J de la table incomplet ligne" +  
		      to_str(n_ligne) + "\n");

  i--;
  j--;
  if(i<NOE || i>GRO || j<NOE || j>=GRO)
    throw read_erreur("Indice I et J de la table incorrect ligne " +
		      to_str(n_ligne) + "\n");
    
  vector<size_type> pointage;
  vector<size_type> liste;
    
  if(i!=GRO){
    if(j!=NOE)
      return;
#if 0
      throw read_erreur("Les elements ne peuvent etre defini que par leurs "
			"noeuds ligne " + to_str(n_ligne) + "\n");
#endif
    pointage.reserve(eta[i][i]+1);
    liste.reserve(eta[i][j]);
    pointage.resize(eta[i][i]+1);
    liste.resize(eta[i][j]);
  }
  else{
    pointage.reserve(eta[GRO+1][j]+1);
    liste.reserve(eta[i][j]);
    pointage.resize(eta[GRO+1][j]+1);
    liste.resize(eta[i][j]);
  }
    
  size_type nb_lignes, l;
  vector<size_type>::iterator ptr;
    
  // Lecture vecteur de pointage
  nb_lignes = (pointage.size()+format->nb_int-1)/format->nb_int;
  ptr = pointage.begin();
    
  // Nombre de % a faire pour callback sur lecture :
  const double	p_cible		= .1;
  double		pourcentage	= 0., p_partiel = 0.;
  // % effectue a chaque lecture :
  const double	p_unitaire_1	= 0 == nb_lignes ? 0. : 1. / nb_lignes;
  for(l=0; l<nb_lignes; ++l){
    lire_ligne(fp, buf, TailleBuf+1);
    int nb = min(size_type(format->nb_int), 
		 pointage.size()-l*format->nb_int);
    for(int i=0; i<nb; ++i){
      memcpy(str, buf+i*format->nc_int, format->nc_int);
      str[format->nc_int] = '\0';

 if(strlen(buf)<i*format->nc_int ||
	 sscanf(str, "%ld", &*ptr)!=1)	// MODIF CP : ptr -> &*ptr
	throw read_erreur("Indice incorrect ou manquant ligne " +
			  to_str(n_ligne) + "\n");
      *ptr -= 1;
      ++ptr;
    }

	p_partiel	+= p_unitaire_1;
	if ((p_partiel >= p_cible) || (l == nb_lignes - 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "",
			                 pourcentage * 100., cancel);
		p_partiel	= 0.;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }	// for(l=0; l<nb_lignes; ++l)
    
  // Lecture vecteur liste d'elements
  nb_lignes = (liste.size()+format->nb_int-1)/format->nb_int;
  ptr = liste.begin();
    
  pourcentage	= 0.;
  p_partiel		= 0.;
  const double	p_unitaire_2	= 0 == nb_lignes ? 0. : 1. / nb_lignes;
  for(l=0; l<nb_lignes; ++l){
    lire_ligne(fp, buf, TailleBuf+1);
    int nb = min(size_type(format->nb_int), liste.size()-l*format->nb_int);
    for(int i=0; i<nb; ++i){
      memcpy(str, buf+i*format->nc_int, format->nc_int);
      str[format->nc_int] = '\0';
  if(strlen(buf)<i*format->nc_int ||
	 sscanf(str, "%ld", &*ptr)!=1)	// MODIF CP : ptr -> &*ptr
	throw read_erreur("Indice incorrect ou manquant ligne  " +
			  to_str(n_ligne) + "\n");
      ++ptr;
    }

	p_partiel	+= p_unitaire_2;
	if ((p_partiel >= p_cible) || (l == nb_lignes - 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "",
	                         pourcentage * 100., cancel);
		p_partiel	= 0.;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }
    
  for(size_type ind=1; ind<pointage.size(); ++ind){
    vector<size_type>::const_iterator debut = liste.begin()+pointage[ind-1];
    vector<size_type>::const_iterator fin = liste.begin()+pointage[ind];
      
    if(i==GRO){
      switch(j){
      case NOE:
	{
	  _NuageInterne* nu = new _NuageInterne("Nuage" + to_str(ind));
	  m_mai->ajouter(nu);
	  for( ; debut!=fin; ++debut)
	    nu->ajouter(m_mai->noeud_id(*debut));
	}
	break;
      case BRA:
	{
	  _LigneInterne* li = new _LigneInterne("Ligne" + to_str(ind));
	  m_mai->ajouter(li);
	  for( ; debut!=fin; ++debut)
	    li->ajouter(m_mai->bras_id(*debut));
	}
	break;
      case PGN:
	{
	  _SurfaceInterne* su = new _SurfaceInterne("Surface" + to_str(ind));
	  m_mai->ajouter(su);
	  for( ; debut!=fin; ++debut)
	    su->ajouter(m_mai->polygone_id(*debut));
	}
	break;
      case PDR:
	{
	  _VolumeInterne* vo = new _VolumeInterne("Volume" + to_str(ind));
	  m_mai->ajouter(vo);
	  for( ; debut!=fin; ++debut)
	    vo->ajouter(m_mai->polyedre_id(*debut));
	}
	break;
      }
    }
    else{
      switch(i){
      case BRA:
	ajouter_bras(m_mai, debut, fin, ind);
	break;
      case PGN:
	ajouter_polygone(m_mai, debut, fin, ind);
	break;
      case PDR:
	ajouter_polyedre(m_mai, debut, fin, ind);
	break;
      }
    }
  }
}
  
//////////////////////////////////////////////////////////////////////////////
/*! Lecture ATTRIBUT NOM de la table */
void _ReaderMali::lire_attr_nom(FILE* fp, char* glob_buf,
				  const size_type eta[][_ReaderMali::QTYP],
				  const _ReaderMali::forma_t* format)
{
	glob_buf[0] = '\0';
//	std::cout<<"lire_attr_nom avec format->nc_string = "<<format->nc_string<<std::endl;
//	std::cout<<"lire_attr_nom avec format->nb_string = "<<format->nb_string<<std::endl;

	int LocTailleBuf = TailleBuf;
	if (LocTailleBuf < format->nc_string) // [EB] pour le cas des noms très longs
		LocTailleBuf = format->nc_string;

	char	buf [LocTailleBuf+1+ 1]; // allocation d'un buffer local

  lire_ligne(fp, buf, LocTailleBuf+1);
  int i, j;
    
  if(sscanf(buf, "%10d%10d", &i, &j) != 2)
    throw read_erreur("Indice I et J de la table incomplet ligne " + 
		      to_str(n_ligne) + "\n");
  i--;
  j--;
  if(i<NOE || i>GRO || j<NOE || j>=GRO)
    throw read_erreur("Indice I et J de la table incorrect ligne " +
		      to_str(n_ligne) + "\n");
  if(i!=GRO)
    throw read_erreur ("On ne peut nommer que les groupes " + 
		      to_str(n_ligne) + "\n");
    
  size_type nb_lignes = (eta[GRO+1][j]+format->nb_string-1)/format->nb_string;
  char nom[LocTailleBuf+1+1];
    
  size_type ind=0;
  for(size_type l=0; l<nb_lignes; ++l){
    lire_ligne(fp, buf, LocTailleBuf+1);
    size_type nb = min(size_type(format->nb_string),
		       eta[GRO+1][j]-l*format->nb_string);

    for(size_type i=0; i<nb; ++i){
      if(strlen(buf)<i*format->nc_string ||
	 sscanf(buf+i*format->nc_string, "%s", nom)!=1)
	throw read_erreur("Il manque un nom ligne " + to_str(n_ligne) + "\n");
      switch(j){
      case NOE:
	m_mai->nuage(ind)->nommer(nom);
	break;
      case BRA:
	m_mai->ligne(ind)->nommer(nom);
	break;
      case PGN:
	m_mai->surface(ind)->nommer(nom);
	break;
      case PDR:
	m_mai->volume(ind)->nommer(nom);
	break;
      }
      ++ind;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
template <class Iter, class Func> void
print_connexions(FILE* fp, int nc, int nb,
		 Iter debut, Iter fin, Func extracteur)
{
  size_type pointage = 1;
  fprintf(fp, "%*ld", nc, pointage);
    
  size_type m = 0;
  for(Iter it1 = debut; it1 != fin; ++it1, ++m){
    pointage += extracteur.dernier(*it1) - extracteur.premier(*it1);
    if(((m+1)%nb)==0)
      fprintf(fp, "\n");
    fprintf(fp, "%*ld", nc, pointage);
  }
  fprintf(fp, "\n");
    
  size_type nb_int = 0;
  for(Iter it2 = debut; it2 != fin; ++it2){
    for(typename Func::iterator element = extracteur.premier(*it2);
	element != extracteur.dernier(*it2); ++element){
      if(nb_int && (nb_int%nb)==0)
	fprintf(fp, "\n");
      fprintf(fp, "%*ld", nc,  (*element)->id());
      ++nb_int;
    }
  }
  fprintf(fp, "\n");
}
  
//////////////////////////////////////////////////////////////////////////////
template <class Iter> void
print_noms(FILE* fp, int nc, int nb, Iter debut, Iter fin)
{
	int LocTailleBuf = TailleBuf;
	if (LocTailleBuf < nc) // [EB] pour le cas des noms très longs
		LocTailleBuf = nc;

	char	buf [LocTailleBuf+1 + 1];	// CP, v 6.27.1, coverity // [EB] on se permet d'avoir parfois un buffer plus long, v 6.34.0
	if (0 == nb)
		throw write_erreur("Division par 0, nombre de noms de groupes invalide.");
  size_type g = 0;
  for(Iter it = debut; it != fin; ++it, ++g){
    if(g && (g%nb)==0)
      fprintf(fp, "\n");
	if ((*it)->nom ( ).length ( ) > LocTailleBuf)	// CP, v 6.27.1, coverity
		throw erreur ("Nom " + (*it)->nom ( ) + " trop long.\n");
    strcpy(buf, (*it)->nom().c_str());	// Rem : nom.length( ) <= TailleBuf

    // remplacement de " (commentaire) par N
    char* ptr = buf;
    while(*ptr != '\0'){
      if(*ptr == '#')
	*ptr = 'N';
      ptr ++;
    }
    fprintf(fp, "%-*s ", nc-1, buf);
  }
  fprintf(fp, "\n");
}


//////////////////////////////////////////////////////////////////////////////
//! Ecriture d'une ligne du fichier
void _WriterMali::ecrire_ligne(FILE* fp, const char* buf)
{
  if(fputs(buf, fp)==EOF)
    throw write_erreur("Flot de donnees interrompu ligne " +
		       to_str(n_ligne) + "\n");
}
  
  
//////////////////////////////////////////////////////////////////////////////
//! Ecriture d'un fichier mali
void _WriterMali::ecrire(File* fd)
{
  FILE* fp = fd->fp;
  char           buf[TailleBuf+1+1];
    
  // Compteur de ligne
  n_ligne = 1;
    
  /* ecriture en tete */
//  ecrire_ligne(fp, "! Fichier MALI ecrit par LIMA\n");
  string	limaVersion ("! Fichier MALI ecrit par LIMA (");
  limaVersion	 += string (lima_version ( )) + ")\n";
  ecrire_ligne (fp, limaVersion.c_str ( ));
  ecrire_ligne(fp, "## DEBUT MAILLAGE\n");
  sprintf(buf, "%s\n", m_mai->titre().c_str());
  ecrire_ligne(fp, buf);
  ecrire_ligne(fp, "# UNITE LONGUEUR\n");
  sprintf(buf, "%23.16E\n", m_mai->unite_longueur());
  ecrire_ligne(fp, buf);
  ecrire_ligne(fp, "# TYPE COORDONNEES\n");
  switch(m_mai->type_coordonnee()){
  case CARTESIEN:
    ecrire_ligne(fp, "CARTESIEN\n");
    break;
  case CYLINDRIQUE:
    ecrire_ligne(fp, "CYLINDRIQUE\n");
    break;
  }
  ecrire_ligne(fp, "# TYPE GEOMETRIE\n");
  switch(m_mai->type_geometrie()){
  case NORMAL:
    ecrire_ligne(fp, "NORMAL\n");
    break;
  case AXISYMETRIQUE:
    ecrire_ligne(fp, "AXISYMETRIQUE\n");
    break;
  case PERIODIQUE:
    ecrire_ligne(fp, "PERIODIQUE\n");
    break;
  }
    
  size_type eta[QTYP+1][QTYP];
  memset(eta, 0, sizeof(size_t)*QTYP*(QTYP+1));
    
  eta[GRO][GRO] = m_mai->dimension();
  eta[NOE][NOE] = m_mai->nb_noeuds();
  eta[BRA][BRA] = m_mai->nb_bras();
  eta[PGN][PGN] = m_mai->nb_polygones();
  eta[PDR][PDR] = m_mai->nb_polyedres();
  eta[GRO+1][NOE] = m_mai->nb_nuages();
  eta[GRO+1][BRA] = m_mai->nb_lignes();
  eta[GRO+1][PGN] = m_mai->nb_surfaces();
  eta[GRO+1][PDR] = m_mai->nb_volumes();
    
  size_type nb_tables = 0;
  if(eta[BRA][BRA] != 0) ++nb_tables;
  if(eta[PGN][PGN] != 0) ++nb_tables;
  if(eta[PDR][PDR] != 0) ++nb_tables;
  if(eta[GRO+1][NOE] != 0) ++nb_tables;
  if(eta[GRO+1][BRA] != 0) ++nb_tables;
  if(eta[GRO+1][PGN] != 0) ++nb_tables;
  if(eta[GRO+1][PDR] != 0) ++nb_tables;
  eta[GRO+1][GRO] = nb_tables;
    
  eta[BRA][NOE] =
    nombre_connexions(m_mai->premier_bras(), m_mai->dernier_bras(), 
		      _les_noeuds<_BrasInterne>());
  eta[PGN][NOE] =
    nombre_connexions(m_mai->premier_polygone(), m_mai->dernier_polygone(), 
		      _les_noeuds<_PolygoneInterne>());
  eta[PDR][NOE] =
    nombre_connexions(m_mai->premier_polyedre(), m_mai->dernier_polyedre(),
		      _les_noeuds<_PolyedreInterne>());
  eta[GRO][NOE] =
    nombre_connexions(m_mai->premier_nuage(), m_mai->dernier_nuage(),
		      _les_noeuds<_NuageInterne>());
  eta[GRO][BRA] =
    nombre_connexions(m_mai->premier_ligne(), m_mai->dernier_ligne(),
		      _les_bras<_LigneInterne>());
  eta[GRO][PGN] =
    nombre_connexions(m_mai->premier_surface(), m_mai->dernier_surface(),
		      _les_polygones<_SurfaceInterne>());
  eta[GRO][PDR] =
    nombre_connexions(m_mai->premier_volume(), m_mai->dernier_volume(),
		      _les_polyedres<_VolumeInterne>());
    
  ecrire_ligne(fp, "## TABLEAU ETAT\n");
  for(int i=0; i<=QTYP; ++i){
    sprintf(buf, "%10ld%10ld%10ld%10ld%10ld\n", eta[i][NOE],
	    eta[i][BRA], eta[i][PGN], eta[i][PDR], eta[i][GRO]);
    ecrire_ligne(fp, buf);
  }
    
  /* calcul des formats : nombre de chiffres pour les entiers, nombre de
   * caracteres pour les chaines, de maniere a avoir au moins un blanc de
   * separation */
  size_type max_indice = *max_element((size_t*)eta,
				      (size_t*)eta+(QTYP+1)*QTYP);
    
  int max_string = 0;
  for(_MaillageInterne::iterator_nuage nu = m_mai->premier_nuage();
      nu != m_mai->dernier_nuage(); ++nu)
    max_string = max(max_string, (int)(*nu)->nom().size());
  for(_MaillageInterne::iterator_ligne li = m_mai->premier_ligne();
      li != m_mai->dernier_ligne(); ++li)
    max_string = max(max_string, (int)(*li)->nom().size());
  for(_MaillageInterne::iterator_surface su = m_mai->premier_surface();
      su != m_mai->dernier_surface(); ++su)
    max_string = max(max_string, (int)(*su)->nom().size());
  for(_MaillageInterne::iterator_volume vo = m_mai->premier_volume(); 
      vo != m_mai->dernier_volume(); ++vo)
    max_string = max(max_string, (int)(*vo)->nom().size());


  forma_t format;
    
  format.nc_int = 1;
  while(max_indice != 0){
    format.nc_int += 1;
    max_indice /= 10;
  }
  format.nb_int = TailleBuf / format.nc_int;
    
//	std::cout<<"format.nb_int = "<<format.nb_int<<std::endl;
//	std::cout<<"format.nc_int = "<<format.nc_int<<std::endl;
//	std::cout<<" TailleBuf = "<<TailleBuf<<std::endl;

  format.nc_string = max_string+1;
  format.nb_string = TailleBuf / format.nc_string;
  if (format.nb_string==0)
	  format.nb_string = 1; // [EB] au moins un nom par ligne

  ecrire_ligne(fp, "## FORMATS\n");
  sprintf(buf, "%10d%10d\n%10d%10d%10d\n%10d%10d\n",
	  format.nb_int, format.nc_int,
	  m_mai->dimension(), 24, 16,
	  format.nb_string, format.nc_string);
  ecrire_ligne(fp, buf);
    
  ecrire_ligne(fp, "## COORDONNEES\n");
  for(_MaillageInterne::iterator_noeud nd = m_mai->premier_noeud();
      nd != m_mai->dernier_noeud(); ++nd){
    switch(m_mai->dimension()){
    case D1:
      sprintf(buf,"% *.*E\n", 24, 16, (*nd)->x());
      break;
    case D2:
      sprintf(buf,"% *.*E% *.*E\n", 24, 16, (*nd)->x(),
	      24, 16, (*nd)->y());
      break;
    case D3:
      sprintf(buf,"% *.*E% *.*E% *.*E\n", 24, 16, (*nd)->x(),
	      24, 16, (*nd)->y(), 24, 16, (*nd)->z());
      break;
    }
    ecrire_ligne(fp, buf);
  }
    
  ecrire_ligne(fp, "## TABLES\n");
  if(m_mai->nb_bras()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", BRA+1, NOE+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int,
                     m_mai->premier_bras(), 
		     m_mai->dernier_bras(), _les_noeuds<_BrasInterne>());
  }
  if(m_mai->nb_polygones()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", PGN+1, NOE+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int,
                     m_mai->premier_polygone(),
		     m_mai->dernier_polygone(), _les_noeuds<_PolygoneInterne>());
  }
  if(m_mai->nb_polyedres()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", PDR+1, NOE+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int, 
                     m_mai->premier_polyedre(),
		     m_mai->dernier_polyedre(), _les_noeuds<_PolyedreInterne>());
  }
  if(m_mai->nb_nuages()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", GRO+1, NOE+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int,
                     m_mai->premier_nuage(),
		     m_mai->dernier_nuage(), _les_noeuds<_NuageInterne>());
    ecrire_ligne(fp, "# ATTRIBUT NOM\n");
    sprintf(buf,"%10d%10d\n", GRO+1, NOE+1);
    ecrire_ligne(fp, buf);
    print_noms(fp, format.nc_string, format.nb_string, 
	       m_mai->premier_nuage(), m_mai->dernier_nuage());
  }
  if(m_mai->nb_lignes()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", GRO+1, BRA+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int, m_mai->premier_ligne(),
		     m_mai->dernier_ligne(), _les_bras<_LigneInterne>());
    ecrire_ligne(fp, "# ATTRIBUT NOM\n");
    sprintf(buf,"%10d%10d\n", GRO+1, BRA+1);
    ecrire_ligne(fp, buf);
    print_noms(fp, format.nc_string, format.nb_string, 
	       m_mai->premier_ligne(), m_mai->dernier_ligne());
  }
  if(m_mai->nb_surfaces()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", GRO+1, PGN+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int,
                     m_mai->premier_surface(),
		     m_mai->dernier_surface(), _les_polygones<_SurfaceInterne>());
    ecrire_ligne(fp, "# ATTRIBUT NOM\n");
    sprintf(buf,"%10d%10d\n", GRO+1, PGN+1);
    ecrire_ligne(fp, buf);
    print_noms(fp, format.nc_string, format.nb_string, 
	       m_mai->premier_surface(), m_mai->dernier_surface());
  }
  if(m_mai->nb_volumes()!=0){
    ecrire_ligne(fp, "# TABLE\n");
    sprintf(buf,"%10d%10d\n", GRO+1, PDR+1);
    ecrire_ligne(fp, buf);
    print_connexions(fp, format.nc_int, format.nb_int, 
                     m_mai->premier_volume(),
		     m_mai->dernier_volume(), _les_polyedres<_VolumeInterne>());
    ecrire_ligne(fp, "# ATTRIBUT NOM\n");
    sprintf(buf,"%10d%10d\n", GRO+1, PDR+1);
    ecrire_ligne(fp, buf);
    print_noms(fp, format.nc_string, format.nb_string, 
	       m_mai->premier_volume(), m_mai->dernier_volume());
  }
    
  ecrire_ligne(fp, "## FIN MAILLAGE\n");
}

//////////////////////////////////////////////////////////////////////////////
_WriterMali::_WriterMali(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Writer(mesh, nom_fichier, format) {
//  m_file.open("wt");
  m_file.open("w");
}
//////////////////////////////////////////////////////////////////////////////
_WriterMali::~_WriterMali() {
  m_file.close();
}

//////////////////////////////////////////////////////////////////////////////
void _WriterMali::write(format_t ){
  ecrire(&m_file);
}

END_NAMESPACE_LIMA
#endif
