#ifdef __INTERNE_MAZE
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_maze.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_maze.C
  \brief Interface de lecture / ecriture pour les fichiers maze
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include "LimaP/maillage_it.h"
#include "LimaP/reader_maze.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA

//////////////////////////////////////////////////////////////////////////////
_ReaderMaze::_ReaderMaze(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format) {
  
  m_file.ascii = true;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderMaze::~_ReaderMaze(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderMaze::readAll(){

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
void _ReaderMaze::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Maze.");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


  
static const int TailleBuf = 128;
static size_t n_ligne;

//! Lecture d'une ligne du fichier et stockage dans le buffer.
void _ReaderMaze::lire_ligne(FILE* fp    // pointeur de fichier
		       , char* buf // buffer
		       )
{
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
//! Lecture des caracteristiques du maillage
void _ReaderMaze::lire_control_cards(FILE* fp,      // pointeur de fichier
				       size_type* nb_mat,// nombre de materiau
				       size_type* nb_pt, // nombre de noeuds
				       size_type* nb_el, // nombre de mailles
				       size_type* nb_lg, // nombre de lignes
				       size_type* nb_nc // nombre de contraintes
				       )
{
  char           buf[TailleBuf];
  char           format[3]; // Input format

  // le titre
  lire_ligne(fp, buf);
  if(!strcmp(buf,"")) lire_ligne(fp, buf);
  m_mai->titre(buf);

  // les caracteristiques
  lire_ligne(fp, buf);
  if(sscanf(buf, "%2s%3ld%5ld%5ld", format, nb_mat, nb_pt, nb_el)!=4)
    throw read_erreur("Il manque une ou plusieurs donnees lors de la "
		      "lecture des caracteristiques du maillage ligne " + 
		      to_str(n_ligne) + "\n");

  lire_ligne(fp, buf);
  if(sscanf(buf, "%5ld%*5d%*5d%*5d%*5d%*5d%*5d%*5d%*5d%*5d%*15f%5ld", 
	    nb_lg, nb_nc)!=2)
    throw read_erreur("Il manque une ou plusieurs donnees lors de la "
		      "lecture des caracteristiques du maillage ligne" + 
		      to_str(n_ligne) + "\n");

  // saut card 4 lignes pas utiles pour LIMA
  for(int l=0; l<3; ++l)
    lire_ligne(fp, buf);

  if(!strcmp(format, "91"))
    lire_ligne(fp, buf);   
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des materiaux
void _ReaderMaze::lire_material_cards(FILE* fp, char* nom)
{
  char           buf[TailleBuf];
  int            mat_type = 0;
  int            eos_type = 0;
  
  // Type du materiau
  lire_ligne(fp, buf);
  buf[25] = '\0';
  if(sscanf(buf, "%*5d%*5d%*10f%5d", &eos_type)!=1)
    throw read_erreur("Il manque le type d'equation d'etat "
		      "du materiau ligne "+ to_str(n_ligne) + "\n");

  buf[10] = '\0';  
  if(sscanf(buf, "%*5d%5d", &mat_type)!=1)
    throw read_erreur("Il manque le type du materiau ligne " +
		      to_str(n_ligne) + "\n");

  lire_ligne(fp, buf);
  _Groupe::formater_nom(buf);
  if(nom && strcmp(buf, ""))
    strncpy(nom, buf, 40);

  // Type 101, on saute 12 lignes
  if(mat_type==101){
    for(int l=0; l<12; l++)
      lire_ligne(fp, buf);
  }
  // Autre type, on saute 6 lignes
  else{
    for(int l=0; l<6; l++)
      lire_ligne(fp, buf);
    if(eos_type!=0){
      // On passe l'equation d'etat
      for(int l=0; l<2; l++)
	lire_ligne(fp, buf);      
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des coordonnees des noeuds
void _ReaderMaze::lire_point_cards(FILE* fp,    // pointeur de fichier, 
					size_type nb_pt // nombre de noeuds
					)
{
  char           buf[TailleBuf];
  double         x, y;
  size_type      ind=0;

	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "",
	                     0., cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  
  // Les noeuds peuvent etre donne dans le desordre.
  
  // On lit les coordonnees du noeud 'ind'
  // Nombre de % a faire pour callback sur lecture :
  const double	p_cible		= .1;
  double		pourcentage	= 0., p_partiel = 0.;
  // % effectue a chaque lecture :
  const double	p_unitaire  = 0 == nb_pt ? 0. : 1. / nb_pt;
  while(ind!=nb_pt){
    lire_ligne(fp, buf);
    if(sscanf(buf, "%5ld%*5lf%10lf%10lf", &ind, &x, &y)!=3)
      throw read_erreur("Caracteristiques du noeuds " + to_str(ind) +  
			"incompletes ligne " + to_str(n_ligne) + "\n");

    if(ind<=0 || ind>nb_pt)
      throw read_erreur("Indice de noeud " + to_str(ind) + "incompatible avec le "
			"nombre de noeuds annonce " + to_str(nb_pt) +
			"ligne "+to_str(n_ligne)+"\n");

    // ajout du noeud
    m_mai->ajouter(_NoeudInterne::create(ind, x, y, 0.0));
	p_partiel	+= p_unitaire;
	if ((p_partiel >= p_cible) || (nb_pt == ind + 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "",
		                     pourcentage * 100., cancel);
		p_partiel	= 0;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des mailles
void _ReaderMaze::lire_element_cards(FILE* fp,     // pointeur de fichier
				       size_type nb_el, // nombre de mailles
				       size_type nb_mat, // nombre de materiaux
				       nom_t* noms // les noms
				       )
{
  char           buf[TailleBuf];
  size_type         ind=0;
  size_type         noeuds[4];
  size_type         nb_noeuds;
  size_type         mat = 0;
  _SurfaceInterne** sur = new _SurfaceInterne*[nb_mat];
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::POLYGONE, "",
	                     0., cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  
  // creation d'un groupe de maille par materiau
  for(size_type g=0; g<nb_mat; g++){
    char nom[41];
    sur[g] =  new _SurfaceInterne(noms[g]);
    m_mai->ajouter(sur[g]);
  }

  // Lecture des mailles
  // Nombre de % a faire pour callback sur lecture :
  const double	p_cible		= .1;
  double		pourcentage	= 0., p_partiel = 0.;
  // % effectue a chaque lecture :
  const double	p_unitaire  = 0 == nb_el ? 0. : 1. / nb_el;
  while(ind!=nb_el){
    lire_ligne(fp, buf);
    if(sscanf(buf, "%5ld%5ld%5ld%5ld%5ld%5ld", &ind, noeuds, noeuds+1, 
	      noeuds+2, noeuds+3, &mat)!=6)
      throw read_erreur("Caracteristiques du polygone " + to_str(ind) + 
			"incompletes ligne "+to_str(n_ligne)+"\n");

    // Si les deux derniers noeuds sont identiques, c'est un triangle
    if(noeuds[2]==noeuds[3])
      nb_noeuds = 3;
    // Sinon c'est un quadrilatere
    else
      nb_noeuds = 4;
    ajouter_polygone(m_mai, noeuds, noeuds+nb_noeuds, size_type(ind));
	if ((mat < 1) || (mat > nb_mat))
		throw read_erreur("Indice de matériau invalide en ligne " + to_str (n_ligne) + "\n");	// CP, v 6.27.1, coverity
    sur[mat-1]->ajouter(m_mai->polygone_id(ind));
	p_partiel	+= p_unitaire;
	if ((p_partiel >= p_cible) || (nb_el == ind + 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::POLYGONE, "",
		                     pourcentage * 100., cancel);
		p_partiel	= 0;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }
  delete[] sur;
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des points d'amorcage
void _ReaderMaze::lire_boundary_condition_card(FILE* fp, size_type nb_lg)
{
  char           buf[TailleBuf];
  size_type      lg, n1, n2, bdr, old_bdr = 0;
  _LigneInterne* ligne	= 0;
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::BRAS, "",
	                     0., cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  
  // Nombre de % a faire pour callback sur lecture :
  const double	p_cible		= .1;
  double		pourcentage	= 0., p_partiel = 0.;
  // % effectue a chaque lecture :
  const double	p_unitaire  = 0 == nb_lg ? 0. : 1. / nb_lg;
  for(lg=0; lg<nb_lg; ++lg){
    lire_ligne(fp, buf);
    if(sscanf(buf, "%*5d%5ld%5ld%5ld", &bdr, &n1, &n2)!=3)
      throw read_erreur("Caracteristiques des lignes de chargement "
			"incompletes ligne "+to_str(n_ligne)+"\n");
    
    if(bdr!=old_bdr){
      char nom[41];
      sprintf(nom, "Contour%ld", bdr);
      ligne = new _LigneInterne(nom);
      m_mai->ajouter(ligne);
      old_bdr = bdr;    
    }
 
	if (0 == ligne)
		throw erreur ("Erreur interne : ligne nulle.");// CP, v 6.27.1, coverity

    _BrasInterne* bras = 
      _BrasInterne::create(m_mai->noeud_id(n1), m_mai->noeud_id(n2));
    m_mai->ajouter(bras);
    ligne->ajouter(bras);

	p_partiel	+= p_unitaire;
	if ((p_partiel >= p_cible) || (nb_lg == lg + 1))
	{
		pourcentage	+= p_partiel;
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::BRAS, "",
		                     pourcentage * 100., cancel);
		p_partiel	= 0;
	}	// if (p_partiel >= p_cible)
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture des contraintes nodales
void _ReaderMaze::lire_nodal_constraint_card(FILE* fp, size_type nb_nc)
{
  char           buf[TailleBuf];
  size_type         nc, nbn, n[16], nu = 1;
  
  for(nc=0; nc<nb_nc; ++nc){
    lire_ligne(fp, buf);
    if(sscanf(buf, "%5ld", &nbn)!=1)
      throw read_erreur("Caracteristiques des lignes de chargement "
			"incompletes ligne "+to_str(n_ligne)+"\n");
    lire_ligne(fp, buf);
    if(sscanf(buf, "%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld", 
	      n,n+1,n+2,n+3,n+4,n+5,n+6,n+7,n+8,n+9,n+10,n+11,n+12,n+13,n+14,n+15)!=nbn)
      throw read_erreur("Caracteristiques des contraintes nodale "
			"incompletes ligne "+to_str(n_ligne)+"\n");

    for(size_type i=0; i<nbn; ++i){
      if(n[i]>=1 && n[i]<=m_mai->nb_noeuds()){
	char nom[41];
	sprintf(nom, "Noeud%ld", nu++);
	_NuageInterne* nuage = new _NuageInterne(nom);
	m_mai->ajouter(nuage);
	nuage->ajouter(m_mai->noeud_id(n[i]));
      }
      else
	throw read_erreur("Le noeud "+to_str(n[i])+"n'appartient pas au maillage "
			  "ligne "+to_str(n_ligne)+"\n");
    }  
  }
}

//////////////////////////////////////////////////////////////////////////////
//! Lecture d'un fichier maze
void _ReaderMaze::lire ( )
{
  FILE* fp = m_file.fp;
  size_type         nb_mat = 0; // nombre de materiaux
  size_type         nb_pt = 0;  // nombre de noeuds
  size_type         nb_el = 0;  // nombre de mailles
  size_type         nb_lg = 0;  // nombre de lignes
  size_type         nb_nc = 0;  // nombre de contraintes
  nom_t*         noms = 0;
 
  // Dimension 2 pour les fichiers maze
  m_mai->dimension(D2);

  // Compteur de ligne
  n_ligne = 1;

  lire_control_cards(fp, &nb_mat, &nb_pt, &nb_el, &nb_lg, &nb_nc);
  noms = new nom_t[nb_mat];
  for(size_t g=0; g<nb_mat; g++)
    sprintf(noms[g], "Objet%ld", g+1);

  for(size_type m=0; m<nb_mat; m++)
    lire_material_cards(fp, noms[m]);
  lire_point_cards(fp, nb_pt);
  lire_element_cards(fp, nb_el, nb_mat, noms);
  
  if(nb_lg!=0)
    lire_boundary_condition_card(fp, nb_lg);
  if(nb_nc!=0)
    lire_nodal_constraint_card(fp, nb_nc);

  delete[] noms;
}




END_NAMESPACE_LIMA
#endif
