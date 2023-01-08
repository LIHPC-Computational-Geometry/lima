#ifdef __INTERNE_INGRID
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_ingrid.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Derniere modification le : Wed Aug  4 08:09:41 METDST 2004 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_ingrid.C
  \brief Interface de lecture / ecriture pour les fichiers ingrid
*/

#include "LimaP/reader_ingrid.h"
#include "LimaP/algorithme.h"
#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include <ctype.h>

#include "LimaP/maillage_it.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA



//////////////////////////////////////////////////////////////////////////////
_ReaderIngrid::_ReaderIngrid (_MaillageInterne* mesh, const string& nom_fichier,
                              format_t format) 
	: _Reader(mesh, nom_fichier, format) , _n_ligne (0), _dernierMatNum (0)
{
	m_file.ascii = true;
	m_file.open ("r");
}	// _ReaderIngrid::_ReaderIngrid


//////////////////////////////////////////////////////////////////////////////
_ReaderIngrid::~_ReaderIngrid(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderIngrid::readAll()
{

  //((_MaillageIngrid*)(m_mai))->lire(&m_file);
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
void _ReaderIngrid::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Ingrid.");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

static const size_t TailleBuf = 128;

//! Lecture d'une ligne du fichier et stockage dans le buffer.
void _ReaderIngrid::lire_ligne(FILE* fp,   // descripteur de fichier
                                 char* buf   // buffer
                                 )
{  
  do{
    char* res = fgets(buf, TailleBuf, fp);
    if(res==NULL && !feof(fp))
      throw read_erreur("Flot de donnees interrompu ligne " +
			to_str(_n_ligne) + "\n");
    if(res==NULL && feof(fp))
      throw eof_erreur("Fin de fichier rencontree prematurement ligne " +
		       to_str(_n_ligne) + "\n");

    if((res=strchr(buf, '\n'))!=NULL)
      *res = '\0';
    ++_n_ligne;
    if(!strncmp(buf, "** INTERFACE NAME:", 18))
      return;
  } while(*buf=='*' || !strncmp(buf, "##", 2));
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des caracteristiques du maillage
void 
_ReaderIngrid::lire_control_cards(FILE* fp,       // pointeur de fichier
				    size_t* nb_mat, // nombre de materiau
				    size_t* nb_pt,  // nombre de noeuds
				    size_t* nb_el,  // nombre de mailles
				    size_t* nb_dt,  // nombre de points de detonations
				    size_t* nb_sur, // nombre de surface
				    int* format     // format
				    )
{
  char           buf[TailleBuf];
  
  // le format
  lire_ligne(fp, buf);
  buf[74] = '\0';  
  if(sscanf(buf+72, "%d", format)!=1)
    throw read_erreur("Il manque le numero de version du format "
		      "de fichier ligne " + to_str(_n_ligne) + "\n");
    
  buf[72] = '\0';   
  m_mai->titre(buf);  

  // les caracteristiques
  lire_ligne(fp, buf);
//  if(sscanf(buf+2, "%3ld%10ld%10ld", nb_mat, nb_pt, nb_el)!=3)	// CP 16/12/02
  if(sscanf(buf+2, "%lu %lu %lu", nb_mat, nb_pt, nb_el)!=3)
    throw read_erreur("Il manque une ou plusieurs donnees lors de la "
		      "lecture des caracteristiques du maillage ligne " +
		      to_str(_n_ligne) + "\n");

  lire_ligne(fp, buf);
  lire_ligne(fp, buf);
  // les points
/* CP le 03/12/02 ===========================================================
  if(sscanf(buf, "%*5d%*5d%*5d%*5d%*5d%5ld", nb_dt)!=1)
    throw read_erreur("Il manque le nombre de points de detonations "
		      "ligne " + to_str(_n_ligne) + "\n");
   Pb : le premier entier peut etre sous forme de caracteres blancs car genere
        par du code fortran. On le considere alors comme etant nul si les
        autres informations sont presentes.
   ============== Remplace par : =================================== */
//   int	nb	= sscanf (buf, "%*5d%*5d%*5d%*5d%*5d%ld", nb_dt);	// CP 16/12/02
   int	nb	= sscanf (buf, "%*lu %*lu %*lu %*lu %*lu %lu %*lu %*lf", nb_dt);
   if (0 == nb)	
   { // On ne lit pas le premier entier. Tentative de lecture des suivants :
//     nb	= sscanf (buf + 5, "%*5d%*5d%*5d%*5d%ld", nb_dt);	// CP 16/12/02
       nb	= sscanf (buf, "%*lu %*lu %*lu %*lu %lu %*lu %*lf", nb_dt);
     if (1 != nb)
        throw read_erreur("Il manque le nombre de points de detonations "
		                  "ligne " + to_str(_n_ligne) + "\n");
     *nb_dt	= 0;
   }	// if (0 == nb)
/* Fin remplacement */
  lire_ligne(fp, buf);
  // les surfaces
//  if(sscanf(buf, "%*5d%5ld", nb_sur)!=1)	// CP 16/12/02
  if(sscanf(buf, "%*d %lu", nb_sur)!=1)
    throw read_erreur("Il manque le nombre de surfaces de glissement "
		      "ligne " + to_str(_n_ligne) + "\n");
  
  switch(*format){
  case 88:
    {
      // 4 lignes pas utiles pour LIMA
      for(int l=0; l<4; ++l)
	lire_ligne(fp, buf);
    }
    break;
  default:
    {
      // 20 lignes pas utiles pour LIMA
      for(int l=0; l<20; ++l)
	lire_ligne(fp, buf);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des materiaux
void _ReaderIngrid::lire_material_cards(FILE* fp, char* nom)
{
  char           buf[TailleBuf];
  size_t         mat_type = 0;
  
  lire_ligne(fp, buf);
  buf[10] = '\0';
//  if(sscanf(buf, "%*5d%5ld", &mat_type)!=1)	// CP 16/12/02
  if(sscanf(buf, "%*d %lu", &mat_type)!=1)
    throw read_erreur("Il manque le type du materiau ligne " +
		      to_str(_n_ligne) + "\n");

  // Lecture du nom du materiau.  Le but est ici de recuperer le nom du materiau
  // depourvu d'eventuels espaces et/ou tabulations situes au debut car le nom 
  // sera tronque a 40 caracteres.
  lire_ligne(fp, buf);
  size_t	blanks		= strspn (buf, " \t");

  // Le nom est-il compose uniquement de caracteres blancs et/ou tabulations ?
  // Un probable oubli de la part de l'operateur.
  // Dans ce cas on donne un nom genere automatiquement au materiau.
  if (blanks < strlen (buf))
    strncpy (nom, buf + blanks, 40);
  else
    strncpy (nom, creer_nom_materiau ( ).c_str ( ), 40);
	// CP, version 5.8.1 :
	for (int i = strlen (nom); i > 0; i--)
		if ((0 != isspace (nom [i])) || ('\0' == nom [i]))
			nom [i]	= '\0';
		else
			break;

  // Type 101, on saute 12 lignes
  if(mat_type==101){
    for(int l=0; l<12; l++)
      lire_ligne(fp, buf);
  }
  // Autre type, on saute 6 lignes
  else{
    for(int l=0; l<6; l++)
      lire_ligne(fp, buf);
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des coordonnees des noeuds
void _ReaderIngrid::lire_point_cards(FILE* fp,    // pointeur de fichier, 
				       size_t nb_pt // nombre de noeuds
				       )
{
  char           buf[TailleBuf];
  double         x, y, z;
  size_t         ind=0;
 
  // Les noeuds peuvent etre donne dans le desordre.

  // On lit les coordonnees du noeud 'ind'
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  while(ind!=nb_pt){
    lire_ligne(fp, buf);
//    if(sscanf(buf, "%8ld%*5lf%20lf%20lf%20lf", &ind, &x, &y, &z)!=4)	// CP 16/12/02
    if(sscanf(buf, "%lu %*lf %lf %lf %lf", &ind, &x, &y, &z)!=4)
      throw read_erreur("Caracteristiques du noeuds " + to_str(ind) +  
			"incompletes ligne " + to_str(_n_ligne) + "\n");

    if(ind<=0 || ind>nb_pt)
      throw read_erreur("Indice de noeud " + to_str(ind) + "incompatible avec le "
			"nombre de noeuds annonce " + to_str(nb_pt) +
			"ligne "+to_str(_n_ligne)+"\n");

    // ajout du noeud
    m_mai->ajouter(_NoeudInterne::create(ind, x, y, z));
  }
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 100., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des mailles
void 
_ReaderIngrid::lire_element_cards(FILE* fp,   // pointeur de fichier
				    size_t nb_el, // nombre de mailles
				    size_t nb_mat, // nombre de materiaux
				    nom_t* noms // les noms
				    )
{
  char           buf[TailleBuf];
  size_t         ind=0;
  size_t         noeuds[8];
  size_t         mat = 0;
  _VolumeInterne**    vol = new _VolumeInterne*[nb_mat];

  // creation d'un groupe de maille par materiau
  for(size_t g=0; g<nb_mat; g++){
    vol[g] = new _VolumeInterne(noms[g]);
    m_mai->ajouter(vol[g]);
  }
 
  // Lecture des mailles
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  while(ind!=nb_el){
    lire_ligne(fp, buf);
//    if(sscanf(buf, "%8ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld", &ind, &mat, 
    if(sscanf(buf, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &ind, &mat, 
	      noeuds, noeuds+1, noeuds+2, noeuds+3, noeuds+4, 
	      noeuds+5, noeuds+6, noeuds+7)!=10)
      throw read_erreur("Caracteristiques du polyedre " + to_str(ind) + 
			"incompletes ligne "+to_str(_n_ligne)+"\n");

    // Ajout de la maille au groupe correspondant
    ajouter_polyedre(m_mai, noeuds, noeuds+8, size_type(ind));
    vol[mat-1]->ajouter(m_mai->polyedre_id(ind));
  }

  delete[] vol;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des surfaces.
void _ReaderIngrid::lire_sliding_interface(FILE* fp, size_t nb_sur)
{
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0., 
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  char           buf[TailleBuf];
  size_t*        nb_lg;
  size_t         s, lg;
  size_t         noeuds[4];
  
  nb_lg = new size_t[nb_sur];
  _SurfaceInterne** sur = new _SurfaceInterne*[nb_sur];

  for(s=0; s<nb_sur; ++s){
    lire_ligne(fp, buf);

    char nom[41];
//    sprintf(nom, "Surface%ld", s+1);	// CP 16/12/02
    sprintf(nom, "Surface%lu", s+1);

    if(!strncmp(buf, "** INTERFACE NAME:", 18)){
      buf[29] = '\0';
		// CP version 5.8.1 : elimination des blancs
		size_t	blanks		= strspn (buf + 18, " \t");
		strcpy (nom, buf + 18 + blanks);
		for (int i = strlen (nom); i > 0; i--)
			if ((0 != isspace (nom [i])) || ('\0' == nom [i]))
				nom [i]	= '\0';
			else
				break;
//      strcpy(nom, buf+18);     
      lire_ligne(fp, buf);      
    }

//    if(sscanf(buf, "%5ld", nb_lg+s)!=1)	// CP 16/12/02
    if(sscanf(buf, "%lu", nb_lg+s)!=1)
      throw read_erreur("Il manque le nombre d'elements de "
			"la surface de chargement ligne " +
			to_str(_n_ligne) + "\n");

    lire_ligne(fp, buf);    
    lire_ligne(fp, buf);    
    sur[s] = new _SurfaceInterne(nom);
    m_mai->ajouter(sur[s]);
  }

  for(s=0; s<nb_sur; ++s){
    for(lg=0; lg<nb_lg[s]; ++lg){
      lire_ligne(fp, buf);
//      if(sscanf(buf, "%*8d%8ld%8ld%8ld%8ld", noeuds, noeuds+1, 	// CP 16/12/02
      if(sscanf(buf, "%*d %lu %lu %lu %lu", noeuds, noeuds+1, 
		noeuds+2, noeuds+3)!=4)
	throw read_erreur("Caracteristiques du polygone incompletes "
			  "ligne " + to_str(_n_ligne) + "\n");

      _PolygoneInterne* polygone = 
	_PolygoneInterne::create(m_mai->noeud_id(noeuds[0]),
                                 m_mai->noeud_id(noeuds[1]),
				 m_mai->noeud_id(noeuds[2]),
                                 m_mai->noeud_id(noeuds[3]));
      m_mai->ajouter(polygone);
      // Ajout de la maille au groupe correspondant
      sur[s]->ajouter(polygone);
    }
  }

  delete[] nb_lg;
  delete[] sur;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des points.
void _ReaderIngrid::lire_detonation_points(FILE* fp, size_t nb_dt)
{
  char           buf[TailleBuf];
  double         x, y, z;
 
//  for(int n=0; n<nb_dt; ++n){	// CP 16/12/02
  for(size_t n=0; n<nb_dt; ++n){
    lire_ligne(fp, buf);
    
//    if(sscanf(buf, "%*10lf%*5d%10lf%10lf%10lf", &x, &y, &z)!=3)	// CP 16/12/02
    if(sscanf(buf, "%*lf %*d %lf %lf %lf", &x, &y, &z)!=3)
      throw read_erreur("Caracteristiques du point de detonation incompletes "
	      "ligne " + to_str(_n_ligne) + "\n");

    _NoeudInterne* nd = _NoeudInterne::create(x, y, z);
    m_mai->ajouter(nd);
    
    char nom[41];
//    sprintf(nom, "Noeud%ld", n+1);	// CP 16/12/02
    sprintf(nom, "Noeud%lu", n+1);
    _NuageInterne* nuage = new _NuageInterne(nom);
    m_mai->ajouter(nuage);
    nuage->ajouter(nd);
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture d'un fichier ingrid
void _ReaderIngrid::lire( )
{
  FILE*fp = m_file.fp;
  size_t         nb_mat = 0; // nombre de materiaux
  size_t         nb_pt = 0;  // nombre de noeuds
  size_t         nb_el = 0;  // nombre de mailles
  size_t         nb_dt = 0;  // nombre de points de detonation
  size_t         nb_sur = 0; // nombre de surfaces
  int            format = 0;
  nom_t*         noms = 0;
  
  // Dimension 3 pour les fichiers ingrid
  m_mai->dimension(D3);

  // Compteur de ligne
  _n_ligne			= 0;
  _dernierMatNum	= 0;

  lire_control_cards(fp, &nb_mat, &nb_pt, &nb_el, &nb_dt, &nb_sur, &format);
  noms = new nom_t[nb_mat];
  memset(noms, 0, nb_mat*sizeof(nom_t));

  if(format != 88){
    for(size_t g=0; g<nb_mat; g++)
       strncpy (noms[g], creer_nom_materiau ( ).c_str ( ), 40);
//      sprintf(noms[g], "Materiau%ld", g+1);
  }
 
  for(size_t m=0; m<nb_mat; m++)
    lire_material_cards(fp, noms[m]);
  lire_point_cards(fp, nb_pt);
  lire_element_cards(fp, nb_el, nb_mat, noms);

  if(nb_sur!=0)
    lire_sliding_interface(fp, nb_sur);

  if(nb_dt!=0)
    lire_detonation_points(fp, nb_dt);

  delete[] noms;
}


string _ReaderIngrid::creer_nom_materiau ( )
{
	char	buffer [1024];
	sprintf (buffer, "Materiau_%lu", _dernierMatNum++);

	return string (buffer);
}	// _ReaderIngrid::creer_nom_materiau


END_NAMESPACE_LIMA
#endif
