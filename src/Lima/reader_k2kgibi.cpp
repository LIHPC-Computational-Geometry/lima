#ifdef __INTERNE_GIBI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_k2kgibi.C
 Cree le : Tue Oct 24 13:18:32 MET DST 2000
 Fin le  : Wed Jan 28 09:44:00 MET 2004 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_k2kgibi.C
  \brief Interface de lecture / ecriture pour les fichiers gibi

  taille limite du fichier 2147 Go...
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include "LimaP/reader_k2kgibi.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"


/*****************************************************************************/

/* Version possible des fichiers gibi */

//! entete 
static const char* gibi_header_level[3] = {
  "MAILLAGE PROVENANT DE GIBI NIVEAU 002  29/07/1987", 
  "MAILLAGE PROVENANT DE GIBI NIVEAU 002  Septemb 93",
  "MAILLAGE PROVENANT DE GIBI NIVEAU 002  Septemb 95"
};

 //! nombre de caracteres ascii pour une coordonnee
static const int  fmt_pt[2] = {12, 13};
//! nombre de caracteres ascii pour le numero d'objet
static const int fmt_obj[2] = {5, 8}; 
 //! nombre de caracteres ascii pour le numero de noeud
static const int fmt_nod[2] = {5, 8};
  //! nombre maximum de noeuds par ligne du fichier
static const int nod_lig[2] = {16, 10};

// format standard et etendu pour les coordonnees
static const char *cfmtpt[2] = {"%12.5E", "%13.6E"};

// format standard et etendu pour le numero de noeud
static const char *cfmtnod[2] = {"%5d", "%8d"};

// format standard et etendu pour le debut d'objets
static const char *cfmtobj[2] = {
  "\n%.4s SOUS OBJETS%4d REFERENCES%4d NB NOEUDS%4d NBELEM%5d", 
  "\n%.4s SOUS OBJETS%4d REFERENCES%4d NB NOEUDS%4d NBELEM%8d"};

/*****************************************************************************/

BEGIN_NAMESPACE_LIMA

//////////////////////////////////////////////////////////////////////////////
/*!
  \return !(strcmp)
*/
static int stregal
(
 const char *s1, //> chaine
 const char *s2  //> chaine
 ){
  return !strcmp(s1, s2);
}

static int nextInt (char** buf)	// V 6.2.3
{
	// On ne fait pas plus de tests que dans atoin que cette fonction remplace
	// lorsqu'on ne sait pas sur quel nombre de caractères est encodé en entier.
	// Le but du jeu est de retourner buf pointant sur le prochain entier à
	// lire.
	char*		end		= *buf;
	long int	value	= strtol (*buf, &end, 10);

	if (end == *buf)
		return -1;
	*buf	= end;

	return (int)value;
}	// nextInt

int _ReaderK2KGibi::TailleBuf = 128;

//////////////////////////////////////////////////////////////////////////////
_ReaderK2KGibi::_ReaderK2KGibi(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _ReaderK2K(mesh, nom_fichier, format),
	// CP, v 6.21.0, coverity :
	gibi_level (-1), nb_pt_nomme (-1), nb_obj (-1), nb_obj_nomme (-1),
	idx_object_named (-1)
{
  
  m_file.ascii = true;
  m_file.open("r");

  idx_coordinate = 0;
  idx_node_named = 0;

  idx_object = NULL;

  // liste des objets gibi
  lobject.elm = NULL;
  lobject.nbr = lobject.max = 0;

  // liste des noeuds nommes 
  lnode_named.elm = NULL;
  lnode_named.nbr = lnode_named.max = 0;

  // liste des objects nommes 
  lobject_named.elm = NULL;
  lobject_named.nbr = lobject_named.max = 0;

  useful_node = NULL;
  m_nb_noeuds	= 0;

  readHeader(); // obligatoire
}

//////////////////////////////////////////////////////////////////////////////
_ReaderK2KGibi::~_ReaderK2KGibi(){

  if (lobject.max)
    delete [] lobject.elm;

  if (lnode_named.max)
    delete [] lnode_named.elm;

  if (lobject_named.max)
    delete [] lobject_named.elm;

  if (idx_object)
    delete [] idx_object;

  if (useful_node)
    delete [] useful_node;
  
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderK2KGibi::readAll()
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
//! dejà fait par le constructeur
void _ReaderK2KGibi::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Gibi.");
}


//////////////////////////////////////////////////////////////////////////////
/*
  \brief Identifie un objet gibi a partir de la chaine de caractere \a buf
*/
size_type _ReaderK2KGibi::objectType(char* _buf, k2k_object_t* obj){

  char name[5];
  int nb_sous_obj, nb_ref, nb_node, nb_elem;

  sprintf(name, "%4.4s", _buf);
  nb_sous_obj = atoin(&_buf[16], 4);
  nb_ref = atoin(&_buf[31], 4);
  nb_node = atoin(&_buf[45], 4);
  nb_elem = atoin(&_buf[56], fmt_obj[gibi_level]);
  
  // identification du type des elements de l'objet gibi
  int elm_type;
  for (elm_type = 0; elm_type < NB_MAX_ELM_TYPE; elm_type++)
    if (stregal(name, elmTypeName(elm_type)))
      break;
  if (elm_type == NB_MAX_ELM_TYPE || 
      (elm_type > GROUPE &&
       nb_node != k2k_NbNode(elm_type))) {
    throw read_erreur(string("Element non identifie\n") +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + _buf + "\n");
  }

  if (obj) {
    //obj->name[0] = '\0';
    obj->name = NULL;
    obj->type = elm_type;
    
    if (obj->type == GROUPE){
      // la liste des noeuds devient la liste des sous groupes
      obj->nbn = 1;
      obj->nbe = nb_sous_obj;
    } else {
      obj->nbn = nb_node;
      obj->nbe = nb_elem;
    }
  }

  return elm_type;
}



//////////////////////////////////////////////////////////////////////////////
/*
  \brief Retourne l'index idx_node_named ou le recherche s'il n'est pas connu
*/
long _ReaderK2KGibi::idxNodeNamed(){

  if (idx_node_named > 0)
    return idx_node_named;

	// CP, Coverity, v 6.27.1
	if (0 != fseek (m_file.fp, idx_coordinate - ftell (m_file.fp), SEEK_CUR))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//  fseek(m_file.fp, idx_coordinate,SEEK_SET);
  //  fseek(m_file.fp, idx_coordinate - ftell(m_file.fp), SEEK_CUR);

  // saut des lignes des coordonnees des noeuds
  int nb_val_par_ligne = m_nb_noeuds * (1+m_mai->dimension());
  for (int io = 0; io < nb_val_par_ligne; io += 6)
    fgets2(buf, TailleBuf, m_file);

  // lecture du nombre de points nommes
  fgets2(buf, TailleBuf, m_file);
  int nbscan = sscanf(buf, "%*s %*s %*s %*s %d", &nb_pt_nomme);
  if (nbscan != 1)
    throw read_erreur(string("Lecture du nombre de points nommes impossible\n")
                      +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + buf + "\n");

  idx_node_named = ftell(m_file.fp);
  return idx_node_named;
}

//////////////////////////////////////////////////////////////////////////////
/*
  \brief Retourne l'index idx_object[num] ou le recherche s'il n'est pas connu
  recursif
*/
long _ReaderK2KGibi::idxObject(int num){

  if (idx_object && idx_object[num] > 0) 
    return idx_object[num];

  // positionnement au plus pres
  if (num == 0) {
		// CP, Coverity, v 6.27.1
		if (0 !=
			fseek (m_file.fp, idxNodeNamed ( ) - ftell (m_file.fp), SEEK_CUR))
			throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//    fseek(m_file.fp, idxNodeNamed(), SEEK_SET);
    //  fseek(m_file.fp, idxNodeNamed() - ftell(m_file.fp), SEEK_CUR);
    
    // saut des lignes de points nommes
    for (int io = 0; io < nb_pt_nomme; io += 5)
      fgets2(buf, TailleBuf, m_file);
    
    // lecture du nombre d'objets gibi
    fgets2(buf, TailleBuf, m_file);
    int nbscan = sscanf(buf, "%*s %*s %d", &nb_obj);
    if (nbscan != 1 || nb_obj < 0)
      throw read_erreur(string("Nombre d'objets invalide\n") +
                        "Erreur detectee a la ligne " + 
                        to_str(m_file.line_num)+
                        " du fichier.\n" + buf + "\n");

    // pout("NOMBRE D'OBJETS : %d\n", nb_obj);
    lobject.max = nb_obj;
    lobject.elm = new k2k_object_t[nb_obj];
    idx_object = new long[nb_obj];
    for (int idx = 1; idx < nb_obj; idx++)
      idx_object[idx] = 0;
   
    idx_object[0] = ftell(m_file.fp);
    
    fgets2(buf, TailleBuf, m_file);
    objectType(buf, &lobject.elm[0]);
    return idx_object[0];

  } else {
	const long	offset	= idxObject(num - 1);
	// CP, Coverity, v 6.27.1
	if ((0 > offset) || (0 != fseek (m_file.fp, offset, SEEK_SET)))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//    fseek(m_file.fp, idxObject(num - 1), SEEK_SET); // recursif
  }

  // saut de l'objet num-1
  /* comme le nombre de ligne est un peu complique a connaitre, on recherche 
     directement l'occurence "SOUS" de la 1ere ligne de l'objet num */
  fgets2(buf, TailleBuf, m_file); // lecture de la 1ere ligne de l'objet num - 1
  fgets2(buf, TailleBuf, m_file);
  while (strncmp(&buf[5], "SOUS", 4)) {
    idx_object[num] = ftell(m_file.fp); // on enregistre la position precedente
    fgets2(buf, TailleBuf, m_file);
  }

  // on identifie l' objet maintenant pour gagner du temps
  objectType(buf, &lobject.elm[num]);
  return idx_object[num];
}

//////////////////////////////////////////////////////////////////////////////
/*
  \brief Retourne l'index idx_object_named ou le recherche s'il n'est pas connu
*/
long _ReaderK2KGibi::idxObjectNamed(){
  if (!idx_object)
    idxObject(0);
	const long	offset	= idxObject (nb_obj - 1);
	// CP, Coverity, v 6.27.1
	if ((0 > offset) || (0 != fseek (m_file.fp, offset, SEEK_SET)))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//  fseek(m_file.fp, idxObject(nb_obj - 1), SEEK_SET);

  // saut du dernier objet gibi
  fgets2(buf, TailleBuf, m_file); 
  fgets2(buf, TailleBuf, m_file);
  while (strncmp(buf, "NOMBRE", 6)){
    fgets2(buf, TailleBuf, m_file);
  }

  // lecture du nombre d'objets nommes
  int nbscan = sscanf(buf, "%*s %*s %*s %d", &nb_obj_nomme);
  if (nbscan != 1 || nb_obj_nomme < 0)
    throw read_erreur(string("Nombre d'objets invalide\n") +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + buf + "\n");

  idx_object_named = ftell(m_file.fp);
  return idx_object_named;
}

//////////////////////////////////////////////////////////////////////////////
/*
  \brief lecture de l'entete d'un fichier GIBI
  Obligatoirement fait.
*/
void _ReaderK2KGibi::readHeader(){

	// CP, v 6.27.1, coverity :
	if (0 != fseek (m_file.fp, 0,SEEK_SET))
	throw read_erreur ("Erreur de positionnement dans le fichier.\n");

  //// lecture du titre 
  fgets2(buf, TailleBuf, m_file);
  if (strlen(buf) >= 8)
    m_mai->titre(&buf[8]); // le titre commence a partir du 8eme caractere de la ligne
  else
    m_mai->titre(buf); // titre vide : prendre nom objet de base 
  //pout("titre : |%s\n", buf);

  //// lecture du niveau GIBI
  fgets2(buf, TailleBuf, m_file);
  printf("%s\n", buf);
  fflush(stdout);

  if (strstr(buf, "29/07/1987") || strstr(buf, "MAZE"))
    gibi_level = 0;
  else if (strstr(buf, "93") || strstr(buf, "95")) 
    gibi_level = 1;
  else 
    throw read_erreur(string("Niveau de fichier Gibi non reconnu"));

  //// lecture erreur dimension densite
  // exemple d'entree a lire : "ERREUR   0 DIMENSION   2 DENSITE  0.00000E+00"
  int err_tmp, dim_tmp;
  float rho_tmp;
  int nbscan;
  fgets2(buf, TailleBuf, m_file);
  // Rq : le format %*s permet de lire la chaine mais sans l'affecter
  nbscan = sscanf(buf, "%*s %d %*s %d %*s %g",
                  &err_tmp, &dim_tmp, &rho_tmp);

  if (nbscan != 3) {
    m_file.pwhere();
    perr(buf);
    throw read_erreur(string("ERREUR ou DIMENSION ou DENSITE incorrecte\n"));
  }
  m_mai->dimension((dim_t)dim_tmp);

  //// lecture du nombre de points
  fgets2(buf, TailleBuf, m_file);
  int itmp;
  nbscan = sscanf(buf, "%*s %*s %*s %d", &itmp);
  if (nbscan != 1 || itmp < 0) {
    m_file.pwhere();
    perr(buf);
    throw read_erreur(string("Nombre de points invalide\n"));
  }
  idx_coordinate = ftell(m_file.fp);
  m_nb_noeuds	= itmp;
}



//////////////////////////////////////////////////////////////////////////////
void _ReaderK2KGibi::lire ( ) {

  // on continu la lecture a partir de la fin de l'entete
	// CP, Coverity, v 6.27.1
	if ((0 > idx_coordinate) ||
	    (0 != fseek (m_file.fp, idx_coordinate, SEEK_SET)))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//  fseek(m_file.fp, idx_coordinate, SEEK_SET);

  //////////////////////
  //// lecture des coordonnees des points
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

  // int nb_pts_read = 0;
  int ipt, idir;
  // chaque ligne du fichier contient au plus 6 valeurs
  // l'ordre des valeurs est : les coordonnees suivie(s) d'une densite
  // exemple en 2D : x1 y1 d1 x2 y2 d2
  // exemple en 3D : x1 y1 z1 d1 x2 y2 
  //    (la suite) : z2 d2
  // il y a nb_pt * (dim_tmp + 1) valeurs (avec au + 6 valeurs par linge)
  double coor[3];
  int ivalueline = 0; // indice de valeurs de la ligne (varie de 1 a 6)

  const int fmtpt = fmt_pt[gibi_level];
  for (ipt = 0; ipt < m_nb_noeuds; ipt++){
    coor[0] = coor[1] = coor[2] = 0.;
    for (idir = 0; idir < m_mai->dimension(); idir++){
      if ((ivalueline % 6) == 0) // 6 valeurs par lignes
        fgets2(buf, TailleBuf, m_file);

      coor[idir] = atofn(&buf[(ivalueline % 6) * fmtpt], fmtpt);
      ivalueline++;
    }

    //pout("Point[%2d] (%g, %g, %g)\n", ipt + 1, coor[0], coor[1], coor[2]);
    m_mai->ajouter(_NoeudInterne::create(ipt + 1, coor[0], coor[1], coor[2]));
    /* theoriquement, il faudrait mettre les 2 lignes suivantes mais le test
       n'est jamais vrai ici (6 est multiple de 2 et 3), donc on les commente */
    /*if ((ivalueline % 6) == 0)
      fgets2(buf, TailleBuf, m_file);*/
    ivalueline++; // on ne lit pas la densite du point
  }
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );


  //////////////////////
  //// lecture des noms des points
  // chaque point nomme definit un nuage (a un seul element)

  fgets2(buf, TailleBuf, m_file);
  int _nb_pt_nomme;
  int nbscan = sscanf(buf, "%*s %*s %*s %*s %d", &_nb_pt_nomme);
  if (nbscan != 1)
    throw read_erreur(string("Lecture du nombre de points nommes impossible\n")
                      +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + buf + "\n");

  int pt_num;
  char name[16];  // normalement le nom ne depasse pas 8 caracteres
  char buf2[17];
  ivalueline = 0; // pas + de 5 points nommes par ligne
  for (ipt = 0; ipt < _nb_pt_nomme; ipt++){
    if ((ivalueline % 5) == 0) {
      fgets2(buf, TailleBuf, m_file);
    }
    strncpy(buf2, &buf[(ivalueline % 5) * 16], 17);
    buf2[16] = '\0';
    nbscan = sscanf(buf2, "%8s%8d",
                    name, &pt_num); // 16 caracteres pour le nom ET l'indice
    if (nbscan != 2 || pt_num < 1) {
      perr("Impossible de lire le nom du point ou son indice\n"
        "ligne %d  du fichier :\n%s\n", m_file.line_num, buf);
    } else {
      // pout("%d. point[%d] %s\n", ipt, pt_num, name);
      
      _NuageInterne *grp = new _NuageInterne(name);
      m_mai->ajouter(grp);
      grp->ajouter(m_mai->noeud(pt_num - 1));
    }
    ivalueline++;
  }
  

  //////////////////////
  //// lecture des objets
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );

  // Remarque :
  // ici, un objet est defini par un ou des element(s)
  //   un element est ici soit 
  //      un noeud (POI1),
  //      un bras (SEG2)),
  //      une maille 2D (TRI3, QUA4)
  //      une maille 3D (TET4, PRI6,  CUB8)
  //   un element est tjs defini par des points

  int _nb_obj;
  fgets2(buf, TailleBuf, m_file);
  nbscan = sscanf(buf, "%*s %*s %d", &_nb_obj);
  if (nbscan != 1 || _nb_obj < 0)
    throw read_erreur(string("Nombre d'objets invalide\n") +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + buf + "\n");
  // pout("NOMBRE D'OBJETS : %d\n", nb_obj);

  tab_gibi_object_t object;
  object.nbr = 0;
  object.elm = new gibi_object_t[_nb_obj];
  object.max = _nb_obj;

  for (object.nbr = 0; object.nbr < _nb_obj; object.nbr++)
    read_gibi_object(&object.elm[object.nbr], &m_file, _nb_obj, gibi_level);
  

  //////////////////////
  //// lecture des objets nommes
  // a partir du moment ou un objet est nomme alors il definit un groupe
  // comportant autant d'element que l'objet en comporte

  int _nb_obj_nomme;
  char15* obj_name;
  int* obj_nomme_num;
  int iobjn;
  fgets2(buf, TailleBuf, m_file);
  nbscan = sscanf(buf, "%*s %*s %*s %d", &_nb_obj_nomme);
  if (nbscan != 1 || _nb_obj_nomme < 0)
    throw read_erreur(string("Nombre d'objets nommes invalide\n") +
                      "Erreur detectee a la ligne " + to_str(m_file.line_num)+
                      " du fichier.\n" + buf + "\n");
  
  // pout("NOMBRE D'OBJETS NOMMES : %d\n", nb_obj_nomme);

  obj_name = new char15[_nb_obj_nomme];
  obj_nomme_num = new int[_nb_obj_nomme];

  for (iobjn =  0; iobjn < _nb_obj_nomme; iobjn++){
    if ((iobjn % 5) == 0) // au + 5 objets nommes par ligne (16 caracteres)
      fgets2(buf, TailleBuf, m_file);
    strncpy(buf2, &buf[(iobjn % 5)*16], 17);
    buf2[16] = '\0';
     nbscan = sscanf(buf2, "%s %d", obj_name[iobjn], &(obj_nomme_num[iobjn]));
    if (nbscan != 2 ||
        obj_nomme_num[iobjn] < 1 || obj_nomme_num[iobjn] > _nb_obj){
      perr("Impossible de lire le nom de l'objet ou son indice\n"
        "ligne %d du fichier :\n%s\n", m_file.line_num, buf);
    } else {
      
      /* ignorer les noms qui commencent par '#' : objets
       * intermediaires oublies par GIBI ! Attention, les objets
       * concernes ont un autre nom qu'il ne faut pas ecraser */
      
      if (obj_name[iobjn][0] != '#') {
        // pout("%d. objet[%d] : %s\n" , iobjn, obj_nomme_num, obj_name);
        sprintf(object.elm[obj_nomme_num[iobjn] - 1].name, obj_name[iobjn]);
      }
    }
  } // fin de lecture des objets nommes

  //////////////////////
  //// exploitation des resultats

  addObject(object, _nb_obj_nomme, obj_name, obj_nomme_num);
  
  delete[] obj_name;
  delete[] obj_nomme_num;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ajoute au maillage le groupe
*/
void _ReaderK2KGibi::addNuage
(
 _NuageInterne* grp_it,       //!< nom du groupe
 gibi_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->typ);

  for (ielem = 0; ielem < obj->nbe; ielem++)
    grp_it->ajouter(m_mai->noeud(obj->nod[ielem] - 1));
  
} 

void _ReaderK2KGibi::addLigne
(
 _LigneInterne* grp_it,       //!< nom du groupe
 gibi_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->typ);

  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->bras(obj->elm_n[ielem]));
    else{
      obj->elm_n[ielem] = m_mai->nb_bras();
      ajouter_bras(m_mai,
		   &obj->nod[ielem * nb_nod],
		   &obj->nod[ielem * nb_nod] + nb_nod,
		   obj->elm_n[ielem] + 1);
    }
  }
} 

void _ReaderK2KGibi::addSurface
(
 _SurfaceInterne* grp_it,       //!< nom du groupe
 gibi_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->typ);

  // TRIANGLE ou QUADRANGLE --> surface
  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->polygone(obj->elm_n[ielem]));
    else{
      obj->elm_n[ielem] = m_mai->nb_polygones();
      ajouter_polygone(m_mai, 
		       &obj->nod[ielem * nb_nod],
		       &obj->nod[ielem * nb_nod] + nb_nod,
		       obj->elm_n[ielem] + 1);
    }
  }
} 

void _ReaderK2KGibi::addVolume
(
 _VolumeInterne* grp_it,       //!< nom du groupe
 gibi_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->typ);

  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->polyedre(obj->elm_n[ielem]));
    else{
      obj->elm_n[ielem] = m_mai->nb_polyedres();
      ajouter_polyedre(m_mai,
		       &obj->nod[ielem * nb_nod],
		       &obj->nod[ielem * nb_nod] + nb_nod,
		       obj->elm_n[ielem] + 1);
    }
  }
}

void _ReaderK2KGibi::addGroup
(
 const char *name,       //!< nom du groupe
 gibi_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->typ);

  if (obj->typ == _ReaderK2K::NOEUD) {
    // --> nuage
    _NuageInterne *grp_it = NULL;
    if(name[0]) {
       grp_it = new _NuageInterne(name);
      m_mai->ajouter(grp_it);
      addNuage(grp_it, obj);
    }
  } else if (obj->typ == _ReaderK2K::BRAS) {
    // --> ligne
    _LigneInterne *grp_it = NULL;
    if(name[0]) {
      grp_it = new _LigneInterne(name);
      m_mai->ajouter(grp_it);
    }
    addLigne(grp_it, obj);
  } else if (elmTypeDimension(obj->typ) == D2) {
    // TRIANGLE ou QUADRANGLE --> surface
    _SurfaceInterne *grp_it = NULL;
    if(name[0]) {
      grp_it = new _SurfaceInterne(name);
      m_mai->ajouter(grp_it);
    }
    addSurface(grp_it, obj);
  } else {
    // PRISME ou TETRAEDRE ou CUBE --> volume
    _VolumeInterne *grp_it = NULL;
    if(name[0]) {
      grp_it = new _VolumeInterne(name);
      m_mai->ajouter(grp_it);
    }
    addVolume(grp_it, obj);
  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ajoute au maillage les objets gibi lus
*/
void _ReaderK2KGibi::addObject
(
 tab_gibi_object_t object, //!< Liste d'objets
 int _nb_obj_nomme,
 char15 *obj_name,
 int* obj_nomme_num
 ){
  int nobj;
  int iobj;

  // cas des objets homogenes
  for(iobj = 0; iobj<object.nbr; ++iobj){
    if (object.elm[iobj].typ != GROUPE) {
      if (-1 != object.elm[iobj].typ)	// CP
        addGroup("", &object.elm[iobj]);
    }
  }
  for (nobj =  0; nobj < _nb_obj_nomme; nobj++){
    iobj = obj_nomme_num[nobj]-1;
    if (object.elm[iobj].typ != GROUPE) {
      if (-1 != object.elm[iobj].typ)	// CP
        addGroup(obj_name[nobj], &object.elm[iobj]);
    }
  }

  // cas des objets heterogenes
  for (nobj =  0; nobj < _nb_obj_nomme; nobj++){
    iobj = obj_nomme_num[nobj]-1;
    sprintf(object.elm[iobj].name, obj_name[nobj]);
    if (object.elm[iobj].name[0] && object.elm[iobj].typ == GROUPE) {
      gibi_object_t* grp = &object.elm[iobj];
      int ielem;

      _NuageInterne *nu_it = new _NuageInterne(grp->name);
      m_mai->ajouter(nu_it);
      _LigneInterne *li_it = new _LigneInterne(grp->name);
      m_mai->ajouter(li_it);
      _SurfaceInterne *su_it = new _SurfaceInterne(grp->name);
      m_mai->ajouter(su_it);
      _VolumeInterne *vo_it = new _VolumeInterne(grp->name);
      m_mai->ajouter(vo_it);

      // parcours des sous-objets
      for (int issobj = 0; issobj < grp->nbe; issobj++) {
        size_type obj_num = grp->nod[issobj];
        gibi_object_t* obj = &object.elm[obj_num];

        // Si type == -1 objet non traite. cf xlmlima 3.5 fonction GIlecgib.
        if (-1 == obj->typ)	
          continue;// CP

          if (obj->typ == _ReaderK2K::NOEUD) {
	    for (ielem = 0; ielem < obj->nbn; ielem++)
	      nu_it->ajouter(m_mai->noeud(obj->nod[ielem] - 1));
	  }
	  else if (obj->typ == _ReaderK2K::BRAS) {
	    for (ielem = 0; ielem < obj->nbe; ielem++)
	      li_it->ajouter(m_mai->bras(obj->elm_n[ielem]));
          } 
	  else if (elmTypeDimension(obj->typ) == D2) {
	    for (ielem = 0; ielem < obj->nbe; ielem++)
	      su_it->ajouter(m_mai->polygone(obj->elm_n[ielem]));
          } 
	  else {
	    for (ielem = 0; ielem < obj->nbe; ielem++)
	      vo_it->ajouter(m_mai->polyedre(obj->elm_n[ielem]));
          }
      } 
      // fin bcl ss-obj gibi
      if(nu_it->nb_noeuds() == 0)
	m_mai->retirer(nu_it);
      if(li_it->nb_bras() == 0)
	m_mai->retirer(li_it);
      if(su_it->nb_polygones() == 0)
	m_mai->retirer(su_it);
      if(vo_it->nb_polyedres() == 0)
	m_mai->retirer(vo_it);
    } // fin grp
  }

  // liberation
  for (iobj = 0; iobj < object.nbr; iobj++) {
    delete [] object.elm[iobj].elm_n;
    delete [] object.elm[iobj].nod;
  }
  if (object.nbr > 0)
    delete [] object.elm; 
}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief lit un objet au sens GIBI dans le fichier "fp" a partir de la position courante
  \exception read_erreur 
*/
void _ReaderK2KGibi::read_gibi_object
(
 gibi_object_t* obj, //= pointeur sur l'objet a renseigner
 File* fp,         //> fichier
 int nb_max_obj,     //> nombre d'objet (numero a ne pas depasser)
 int _gibi_level      //> version du fichier gibi
 )
{
  // exemple de lignes a lire :
  // "QUA4 SOUS OBJETS   0 REFERENCES   4 NB NOEUDS   4 NBELEM    4"
  // "   2   4   5   6"    -> references (jamais lues)
  // "BLAN BLAN BLAN BLAN" -> couleurs (tjs BLAN ?)
  // "    5    8    6    4    8    9    7    6    4    6    2    1    6    7    3    2" -> numero des noeuds
  
  // nota : dans le fichier, les numerotations commencent a partir de 1

	char name[5];
	int nb_sous_obj = 0, nb_ref = 0, nb_node = 0, nb_elem = 0;
	const int fmtobj = fmt_obj[_gibi_level];
	const int fmtnod = fmt_nod[_gibi_level];
	const int nodlig = nod_lig[_gibi_level];

	fgets2(buf, TailleBuf, fp);
	//sscanf(buf, "%4s", name);
	sprintf(name, "%4.4s", buf);
	nb_sous_obj = atoin(&buf[16], 4);
	nb_ref = atoin(&buf[31], 4);
	nb_node = atoin(&buf[45], 4);
	nb_elem = atoin(&buf[56], fmtobj);
  
	// identification du type des elements de l'objet gibi
	int elm_type	= NB_MAX_ELM_TYPE;
	// Rem : "    " => GROUPE ssi nb_node == 0 && nb_elem == 0
	// On introduit ici le type "-1", correspondant a :
	// 1- un nom "    ", comme le groupe, mais avec (0 != nb_node) || 
	// (0 != nb_elem), contrairement au groupe. 
	// 2- Tout objet ayant des sous-objets.
	// Les informations liees a ce type d'objet ne sont pas traitees par Lima.
	// Source : xlmlima 3.5, fonction GIlecgib.
	if (stregal(name, "    "))
	{
		if ((0 == nb_node) && (0 == nb_elem))
			elm_type	= GROUPE;
		else
			elm_type	= -1;
	}
	else if (0 != nb_sous_obj)
		elm_type	= -1;
	else
	{
		for (elm_type = 0; elm_type < NB_MAX_ELM_TYPE; elm_type++)
			if (stregal(name, elmTypeName(elm_type)))
				break;
		if (elm_type == NB_MAX_ELM_TYPE || 
		    (elm_type > GROUPE && nb_node != k2k_NbNode(elm_type))) 
		{
			throw read_erreur(string("Element non identifie\n") +
				"Erreur detectee a la ligne " + to_str(m_file.line_num)+
				" du fichier.\n" + buf + "\n");
		}
	}	// else if (stregal(name, "    "))
	obj->name[0] = '\0';
	obj->typ = elm_type;
 
	if (obj->typ == GROUPE)
	{ // la liste des noeuds devient la liste des sous groupes
		obj->nbn = 1;
		obj->nbe = nb_sous_obj;
	} 
	else 
	{
		obj->nbn = nb_node;
		obj->nbe = nb_elem;
	}

	// allocation
	obj->elm_n = new size_type[1+obj->nbe];
	obj->nod = new size_type[1+obj->nbn * obj->nbe];
  
	//// lecture / saut des sous objets
	if (nb_sous_obj) 
	{
		int issobj;
		if (elm_type == GROUPE) 
		{
			// on remplit le tableau obj->nod avec les numeros des groupes 
			char*	bufptr	= buf;	// V 6.2.3
			for (issobj = 0; issobj < nb_sous_obj; issobj++)
			{
				// lecture d'une ligne du fichier
				// au + 20 numeros par ligne (4 caracteres)
//				if ((issobj % 20) == 0) fgets2(buf, TailleBuf, fp);
				if (0 == (issobj % 20))	// V 6.2.3
				{
					fgets2 (buf, TailleBuf, fp);
					bufptr	= buf;
				}	// if (0 == (issobj % 20))
        
				// analyse de la ligne 
				int ssobj_num = 0;
// CP V 6.2.3 : le numéro des sous-objets n'est pas forcément codé sur fmtobj.
// Dans un même fichier provenant de MAZE NIVEAU 002 Septemb93 on en trouve
// codés sur 4 ou 5 caractères ...
// => Utilisation de la fonction nextInt qui ne se préoccupe pas du nombre
// de caractères utilisés. Reste à espérer que le % 20 soit justifié.
//				ssobj_num = atoin(&buf[(issobj % 20) * fmtobj], fmtobj) - 1;
				ssobj_num	= nextInt (&bufptr) - 1;	// V 6.2.3
				if (ssobj_num < 0 || ssobj_num >= nb_max_obj)
				{
					throw read_erreur(string("Numero de sous objet invalide\n")+
							"Erreur detectee a la ligne " +
							to_str(m_file.line_num)+
							" du fichier.\n" + buf + "\n");
				} 
				else 
				{
					obj->nod[issobj] = ssobj_num;
					//pout("obj a ajouter : %d\n", ssobj_num);
				}	// else if (ssobj_num < 0 || ssobj_num >= nb_max_obj)
			}	// for (issobj = 0; issobj < nb_sous_obj; issobj++)
		} 	// if (elm_type == GROUPE)
		else 
		{ // saut
			for (issobj = 0; issobj < nb_sous_obj; issobj += 20)
				fgets2(buf, TailleBuf, fp);
		}	// else if (elm_type == GROUPE)
	}	// if (nb_sous_obj)
  
	//// les references ne sont jamais lues
	// au + 20 references par ligne
	for (int iref = 0; iref < nb_ref; iref += 20)
		fgets2(buf, TailleBuf, fp);

	if (elm_type == GROUPE)
		return; // pour un GROUPE, c'est la fin de l'enregistrement

	// saut des couleurs (1 espace + 4 caracteres par couleur) 
	// au + 16 couleurs par ligne
	int ielem;
	for (ielem = 0; ielem < nb_elem; ielem += 16)
		fgets2(buf, TailleBuf, fp);

	//// lecture / saut des elements
	int nb_val_read = 0;
	for (ielem = 0; ielem < nb_elem; ielem++)
	{
		//pout("Element %d :\n", ielem);
		int inode;
		for (inode = 0; inode < nb_node; inode++) 
		{
			// lecture d'une ligne du fichier
			// nodlig valeur par ligne
			if (nb_val_read % nodlig == 0)
				fgets2(buf, TailleBuf, fp);
			int elem_num;
			elem_num = atoin(&buf[(nb_val_read % nodlig)* fmtnod], fmtnod);
			if (elem_num == 0) 
				throw read_erreur(string("Numero de point invalide\n") +
						"Erreur detectee a la ligne " +
						to_str(m_file.line_num)+
						" du fichier.\n" + buf + "\n");
       
			obj->nod[nb_val_read] = elem_num;
			nb_val_read++;
			//pout(" %d ", elem_num);
		}	// for (inode = 0; inode < nb_node; inode++)
	    //pout("\n");
	}	// for (ielem = 0; ielem < nb_elem; ielem++)
/* ==================================================================
// Modif CP (source xlmlima 3.5, gibi.h, fonction GIlecgib) :
	if (nb_sous_obj) 
	{ // saut
		for (ielem = 0; ielem < nb_elem * nb_node; ielem += nodlig)
			fgets2(buf, TailleBuf, fp);
	}	// if (nb_sous_obj)
================================================================== */
}	// read_gibi_object


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief lit un objet au sens GIBI 
  \exception read_erreur 
*/
void _ReaderK2KGibi::readk2kObject
(
 k2k_object_t* obj
 ){

  // exemple de lignes a lire :
  // "QUA4 SOUS OBJETS   0 REFERENCES   4 NB NOEUDS   4 NBELEM    4"
  // "   2   4   5   6"    -> references (jamais lues)
  // "BLAN BLAN BLAN BLAN" -> couleurs (tjs BLAN ?)
  // "    5    8    6    4    8    9    7    6    4    6    2    1    6    7    3    2" -> numero des noeuds
  
  // nota : dans le fichier, les numerotations commencent a partir de 1


  const int fmtobj = fmt_obj[gibi_level];
  const int fmtnod = fmt_nod[gibi_level];
  const int nodlig = nod_lig[gibi_level];

  fgets2(buf, TailleBuf, m_file);
  size_type nb_sous_obj = atoin(&buf[16], 4);
  size_type nb_ref = atoin(&buf[31], 4);

  // allocation
  obj->elem = new size_type[1+obj->nbe];
  obj->node = new size_type[1+obj->nbn * obj->nbe];
  

  //// lecture / saut des sous objets
  if (nb_sous_obj) {
    int issobj;
    if (obj->type == GROUPE) {
      // on remplit le tableau obj->nod avec les numeros des groupes 
      for (issobj = 0; issobj < nb_sous_obj; issobj++){
        
        // lecture d'une ligne du fichier
        // au + 20 numeros par ligne (4 caracteres)
        if ((issobj % 20) == 0) fgets2(buf, TailleBuf, m_file);
        
        // analyse de la ligne 
        int ssobj_num = 0;
        ssobj_num = atoin(&buf[(issobj % 20) * fmtobj], fmtobj) - 1;
        if (ssobj_num < 0 || ssobj_num >= nb_obj){
          throw read_erreur(string("Numero de sous objet invalide\n") +
                            "Erreur detectee a la ligne " +
                            to_str(m_file.line_num)+
                            " du fichier.\n" + buf + "\n");

         } else {
           obj->node[issobj] = ssobj_num;
           //pout("obj a ajouter : %d\n", ssobj_num);
         }
      }
    } else {
      // saut
      for (issobj = 0; issobj < nb_sous_obj; issobj += 20)
        fgets2(buf, TailleBuf, m_file);
      
    }
  }
  
  //// les references ne sont jamais lues
  // au + 20 references par ligne
  for (int iref = 0; iref < nb_ref; iref += 20)
    fgets2(buf, TailleBuf, m_file);

  if (obj->type == GROUPE)
    return; // pour un GROUPE, c'est la fin de l'enregistrement

  // saut des couleurs (1 espace + 4 caracteres par couleur) 
  // au + 16 couleurs par ligne
  int ielem;
  for (ielem = 0; ielem < obj->nbe; ielem += 16)
    fgets2(buf, TailleBuf, m_file);

  //// lecture / saut des elements
  int nb_val_read = 0;
  for (ielem = 0; ielem < obj->nbe; ielem++){
    //pout("Element %d :\n", ielem);
    int inode;
    for (inode = 0; inode < obj->nbn; inode++) {

      // lecture d'une ligne du fichier
      // nodlig valeur par ligne
      if (nb_val_read % nodlig == 0)
        fgets2(buf, TailleBuf, m_file);
      int elem_num;
      elem_num = atoin(&buf[(nb_val_read % nodlig)* fmtnod], fmtnod);
      if (elem_num == 0) 
        throw read_erreur(string("Numero de point invalide\n") +
                          "Erreur detectee a la ligne " +
                          to_str(m_file.line_num)+
                          " du fichier.\n" + buf + "\n");
       
      obj->node[nb_val_read] = elem_num;
      nb_val_read++;
      //pout(" %d ", elem_num);
    
    }
    //pout("\n");
  }

  if (nb_sous_obj) {
    // saut
    for (ielem = 0; ielem < obj->nbe * obj->nbn; ielem += nodlig)
      fgets2(buf, TailleBuf, m_file);
  }


}


//////////////////////////////////////////////////////////////////////////////
/*! Lit les points nommes */
void _ReaderK2KGibi::readNodeNamed(){
//  fseek(m_file.fp, idxNodeNamed(), SEEK_SET);
	const long	offset	= idxObjectNamed ( );
	// CP, Coverity, v 6.27.1
	if ((0 > offset) || (0 != fseek (m_file.fp, offset, SEEK_SET)))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
  lnode_named.nbr = nb_pt_nomme; 
  /* Attention : nb_pt_nomme n'est connu (au pire) qu'apres l'appel de
     idxNodeNamed() */
  readListOfName(lnode_named);
}

//////////////////////////////////////////////////////////////////////////////
/*! Lit les objects gibi nommes */
void _ReaderK2KGibi::readObjectNamed(){
	const long	offset	= idxObjectNamed ( );
	// CP, Coverity, v 6.27.1
	if ((0 > offset) || (0 != fseek (m_file.fp, offset, SEEK_SET)))
		throw read_erreur ("Erreur de positionnement dans le fichier.\n");
//  fseek(m_file.fp, idxObjectNamed(), SEEK_SET);
  lobject_named.nbr = nb_obj_nomme;
  readListOfName(lobject_named);
}

//////////////////////////////////////////////////////////////////////////////
/*! Lit une liste de nom a partir de la position courante.
  lname.nbr est le nombre de nom a lire
  lname.elm est alloue si besoin
*/
void _ReaderK2KGibi::readListOfName(k2k_lname_t& lname){

  lname.max = 0;
  if (lname.nbr == 0)
    return;

  // allocation
  lname.max = lname.nbr;
  lname.elm = new k2k_name_t[lname.nbr];

  // lecture
  int pt_num;
  char name[16];  // normalement le nom ne depasse pas 8 caracteres
  char buf2[17];
  int ivalueline = 0; // pas + de 5 points nommes par ligne

  for (int io = 0; io < lname.nbr; io++){
    if ((ivalueline % 5) == 0) {
      fgets2(buf, TailleBuf, m_file);
    }
    strncpy(buf2, &buf[(ivalueline % 5) * 16], 17);
    buf2[16] = '\0';
    int nbscan = sscanf(buf2, "%7s %d",
                        lname.elm[io].name, &lname.elm[io].obj); // 16 caracteres pour le nom ET l'indice
    if (nbscan != 2 || lname.elm[io].obj < 1) {
      perr("Impossible de lire le nom du point ou son indice\n"
           "ligne %d  du fichier :\n%s\n", m_file.line_num, buf);
    } 
    ivalueline++;
  }
}



//////////////////////////////////////////////////////////////////////////////
void _ReaderK2KGibi::readNb(){
  size_type io;
  
  idxNodeNamed();
  readObjectNamed();
} 
 

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////
_WriterK2KGibi::_WriterK2KGibi(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Writer(mesh, nom_fichier, format) {
//  m_file.open("wt");
  m_file.open("w");
}
//////////////////////////////////////////////////////////////////////////////
_WriterK2KGibi::~_WriterK2KGibi() {
  m_file.close();
}

//////////////////////////////////////////////////////////////////////////////
void _WriterK2KGibi::write(format_t format){
  ecrire(&m_file, format);
}


//////////////////////////////////////////////////////////////////////////////
void _WriterK2KGibi::ecrire(File* fd, format_t format){

  int gibi_level = 0;
 
  //////////////////////
  // test de compatibilite du format par rapport au nombre de noeuds
  switch (format) {
    
  case GIBI : // format standard
    gibi_level = 0;
    if (m_mai->nb_noeuds() > 99999){
      perr("Nombre de noeuds trop grand pour une sortie au format GIBI standard, essai du format etendu\n");
    } else
      break;
    
  case GIBI_I8 : // format etendu
    gibi_level = 1;
    if (m_mai->nb_noeuds() > 99999999)
      throw write_erreur("Nombre de noeuds trop important par rapport aux possibilites du format GIBI etendu\n"); 
    break;
  default:
    throw write_erreur("Valeur format GIBI non valide\n");
  }
  // int nodlig = nod_lig[gibi_level];

  //////////////////////
  // entete
  fprintf2(fd, "S       %.72s\n"
          "%s\n"
          "ERREUR   0 DIMENSION%4d DENSITE  0.00000E+00\n"
          "NOMBRE DE POINTS %8d" ,
          m_mai->titre().c_str(),
          gibi_header_level[gibi_level],
          m_mai->dimension(),
          m_mai->nb_noeuds());

  //////////////////////
  // coordonnees des noeuds
  // c'est detaille parce qu'on n'a pas forcement le tableau de valeurs 
  // contigues : noeud(inod)->m_coo[idim]
  char fmt[30];
  switch(m_mai->dimension()) {
  case D1: // 3 noeuds par ligne
    {
      sprintf(fmt, "%s%s", cfmtpt[gibi_level], cfmtpt[gibi_level]);
      for (int inod = 0; inod < m_mai->nb_noeuds(); inod++) {
	if (inod % 3 == 0)
	  fprintf2(fd, "\n");
	fprintf2(fd, fmt, m_mai->noeud(inod)->x(), 0.); 
      }
    }
    break;
  case D2 : // 2 noeuds par ligne 
    {
      sprintf(fmt, "%s%s%s", cfmtpt[gibi_level], cfmtpt[gibi_level],
	      cfmtpt[gibi_level]);
      for (int inod = 0; inod <m_mai->nb_noeuds(); inod++) {
	if (inod % 2 == 0)
	  fprintf2(fd, "\n");
	fprintf2(fd, fmt, 
                 m_mai->noeud(inod)->x(), 
                 m_mai->noeud(inod)->y(), 0.); 
      }
    }
    break;
  case D3 : // 1.5 noeuds par ligne
    {
      char format1[30];
      char format2[30];
      char format3[30];
      sprintf(format1, "\n%s%s%s%s", cfmtpt[gibi_level], cfmtpt[gibi_level],
	      cfmtpt[gibi_level], cfmtpt[gibi_level]);
      sprintf(format2, "%s%s\n%s%s", cfmtpt[gibi_level], cfmtpt[gibi_level],
	      cfmtpt[gibi_level], cfmtpt[gibi_level]);
      sprintf(format3, "%s%s%s%s", cfmtpt[gibi_level], cfmtpt[gibi_level],
	      cfmtpt[gibi_level], cfmtpt[gibi_level]);
      for (int inod = 0; inod < m_mai->nb_noeuds(); inod++) {
	switch (inod % 3) {
	case 0 :
	  fprintf2(fd, format1,
		   m_mai->noeud(inod)->x(), m_mai->noeud(inod)->y(),
                   m_mai->noeud(inod)->z(), 0.); 
	  break;
	case 1:
	  fprintf2(fd, format2,
		   m_mai->noeud(inod)->x(), m_mai->noeud(inod)->y(),
                   m_mai->noeud(inod)->z(), 0.); 
	  break;
	default :
	  fprintf2(fd, format3,
		   m_mai->noeud(inod)->x(), m_mai->noeud(inod)->y(),
                   m_mai->noeud(inod)->z(), 0.); 
	}
      }
    }
    break;
  }

  //////////////////////
  // points nommes 
  int nb_val = 0;
  int ielm;
  for (ielm = 0; ielm < m_mai->nb_nuages(); ielm++){
    if (m_mai->nuage(ielm)->nb_noeuds() == 1) {
        nb_val++;
    }
  }

  fprintf(fd->fp, "\nNOMBRE DE POINTS NOMMES%8d", nb_val);

  nb_val = 0;
  for (ielm = 0; ielm < m_mai->nb_nuages(); ielm++){
    if (m_mai->nuage(ielm)->nb_noeuds() == 1) {
      if ((nb_val % 5 )== 0)
	fprintf2(fd, "\n");
      nb_val++;
      fprintf2(fd, "%-8.8s%8d", m_mai->nuage(ielm)->nom().c_str(),
	       m_mai->nuage(ielm)->noeud(0)->id() );
    }
  }
  
  //////////////////////
  // les objets
  k2k_lname_t lname;
  k2k_lobject_t lobject;
  
  prepareListes(&lobject, &lname, nb_val);
  
  fprintf2(fd, "\nNOMBRE D'OBJETS %8d", lobject.nbr);
  
  for (int iobj =0; iobj < lobject.nbr; iobj++){
    if (lobject.elm[iobj].type == _ReaderK2K::GROUPE) {
      fprintf2(fd, cfmtobj[gibi_level],
              _ReaderK2K::elmTypeName(lobject.elm[iobj].type),
              lobject.elm[iobj].nbe, 0, 0, 0);

      for (int issobj = 0; issobj < lobject.elm[iobj].nbe; issobj++) {
        if ((issobj % 20) == 0)
          fprintf2(fd, "\n");
        fprintf2(fd, "%4d", lobject.elm[iobj].node[issobj]+1);
      }

      // pas de reference
      // pas de couleur
      // pas de noeud
    } else {
// Correction bogue C. PIGNEROL Lima++ version 5.5.0
// Transmission a fprintf2 de _ReaderK2K::elmTypeName(lobject.elm[iobj].type)
// qui est un pointeur dans la pile -> probablement ecrase.
// => remplacement par typeName
    char typeName [1024];
	strcpy (typeName, _ReaderK2K::elmTypeName(lobject.elm[iobj].type));
    fprintf2(fd, cfmtobj[gibi_level],
              typeName, 0, 0, 
              _ReaderK2K::k2k_NbNode(lobject.elm[iobj].type),
              lobject.elm[iobj].nbe); 
/*    fprintf2(fd, cfmtobj[gibi_level],
              _ReaderK2K::elmTypeName(lobject.elm[iobj].type), 0, 0, 
              _ReaderK2K::k2k_NbNode(lobject.elm[iobj].type),
              lobject.elm[iobj].nbe); */
      
      // pas de sous objets
      // pas de reference
      // couleur (tjs BLAN)
      for (ielm = 0; ielm < lobject.elm[iobj].nbe; ielm++) {
        if ((ielm % 16) == 0)
          fprintf2(fd, "\n");
        fprintf2(fd, " BLAN");
      }

      // noeud
      for (ielm = 0; ielm < lobject.elm[iobj].nbn; ielm++) {
        if ((ielm % nod_lig[gibi_level]) == 0)
          fprintf2(fd, "\n");
        fprintf2(fd, cfmtnod[gibi_level], lobject.elm[iobj].node[ielm]);
      }
  
    }
  }
  
  //////////////////////
  // les objets nommes
  fprintf2(fd, "\nNOMBRE D'OBJETS NOMMES%8d", lname.nbr);
  {
    for (int iobj = 0; iobj < lname.nbr; iobj++){
      if ((iobj % 5) == 0)
	fprintf2(fd, "\n");
      fprintf2(fd, "%-8.8s%8d", lname.elm[iobj].name, lname.elm[iobj].obj);
    }
  }
  
  fprintf2(fd, "\n");

  // liberation
  if (lobject.max > 0) {
    for (ielm = 0; ielm < lobject.max; ielm++)
      delete [] lobject.elm[ielm].node;
    delete [] lobject.elm;
  }
  if (lname.max > 0)
    delete [] lname.elm;

}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief renvoit le type de maille gibi selon nombre de noeuds et la dimension
  \detail pas valable pour les GROUPE
  la dimension ne sert qu'a dissocier QUADRANGLE de TETRAEDRE
*/
size_type typeLima2Gibi
(
 size_type nb_nod, //!< nombre de noeud de l'objet
 dim_t dimension   //!< dimension de l'objet
 ){
  size_type type_gibi = _ReaderK2K::GROUPE;

  if (nb_nod == 1)
    type_gibi = _ReaderK2K::NOEUD;
  else if (nb_nod == 2)
    type_gibi = _ReaderK2K::BRAS;
  else if (nb_nod == 3)
    type_gibi = _ReaderK2K::TRIANGLE;
  else if (nb_nod == 4)
    if (dimension == D2)
      type_gibi = _ReaderK2K::QUADRANGLE;
    else 
      type_gibi = _ReaderK2K::TETRAEDRE;
  else if (nb_nod == 6)
    type_gibi = _ReaderK2K::PRISME;
  else if (nb_nod == 8)
    type_gibi = _ReaderK2K::CUBE;
  else
    throw write_erreur(string("Objet de ") + to_str(nb_nod) +
                       " noeuds -> non supporte par le format gibi\n");
  return type_gibi;
}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Creation de l'objet (gibi) issu du groupe grp (Surface ou volume).
  \detail exemple pour un _VolumeInterne : 
      - s'il n'y a que des polyedres de meme type, il n'y a qu'un objet
      - s'il y a des CUBE et des TETRAEDRE, il y a 2 objets et le volume
        est un objet compose (GROUPE). les sous-groupes seront crees apres

*/
template <class T> size_type countLimaGroupe
(
 T* grp,                    //!< _LigneInterne ou _SurfaceInterne ou _VolumeInterne 
 dim_t dimension,           //!< respectivement D1 ou D2 ou D3 
 size_type& nb_object_count //!< offset : indice des sous-groupes
 ){

  size_type nb_object = 0;
  size_type nb_gibi_object = 0;
  
  if (dimension >= D2 ) { // polygones ou polyedres
    size_type nb_object_of_type[_ReaderK2K::NB_MAX_ELM_TYPE] =
    { 0, 0, 0, 0, 0, 0, 0, 0};
    for (int ielm = 0; ielm < grp->nb_elm(); ielm++) {
      size_type typ = typeLima2Gibi(grp->elm(ielm)->nb_noeuds(), dimension);
      nb_object_of_type[typ]++;
      grp->elm(ielm)->set_udata((int*) 1); // marquage de comptabilisation
    }
    for (int ityp = 0; ityp < _ReaderK2K::NB_MAX_ELM_TYPE; ityp++)
      if (nb_object_of_type[ityp] > 0)
        nb_object++;
  } else { // ligne ou nuage
    nb_object = 1; // forcement
    for (int ielm = 0; ielm < grp->nb_elm(); ielm++)
      grp->elm(ielm)->set_udata((int*) 1); // marquage de comptabilisation
  }

  k2k_object_t * object = new k2k_object_t;
  grp->set_udata((void*) object);

  if (nb_object == 1) { // objet simple
    object->type = 
      typeLima2Gibi(grp->elm(0)->nb_noeuds(), dimension);
    object->nbe = grp->nb_elm();
    object->nbn =
      _ReaderK2K::k2k_NbNode(object->type) * object->nbe;
    object->node = new size_type[object->nbn];
    size_type indn = 0;
    for (int ielm = 0; ielm < object->nbe; ielm++) {
      for (int in = 0; in < _ReaderK2K::k2k_NbNode(object->type); in++){
        object->node[indn++] = 
          (size_type)(grp->elm(ielm)->noeud(in)->udata());
      }
    }
    nb_gibi_object = 1;
  } else {
    object->type = _ReaderK2K::GROUPE;
    object->nbe = (size_type) nb_object;
    object->nbn = 1;
    object->node = new size_type[object->nbe];
    size_type indn = 0;
    for (int ielm = 0; ielm < object->nbe; ielm++) {
      object->node[indn++] = nb_object_count++; // c'est l'indice qu'aura le sous objet de ce _groupe
    }
    nb_gibi_object = 1+nb_object;
  }

  return nb_gibi_object;
}

//////////////////////////////////////////////////////////////////////////////
/*
  \brief Ajoute a la liste des noms celui de l'objet grp
*/
template <class T> void addNameOfGroupe
(
 T* grp, //!< _SurfaceInterne ou _VolumeInterne ou _LigneInter....
 k2k_lname_t* lname //!< liste de noms
 ){
  if (lname->nbr < lname->max) {
    sprintf(lname->elm[lname->nbr].name, "%-8.8s", grp->nom().c_str());
    lname->elm[lname->nbr].obj = lname->nbr + 1;
    lname->nbr++;
  } else
    throw write_erreur("Nombre d'objets nommes plus important que prevu\n");
}

//////////////////////////////////////////////////////////////////////////////
/*
  \brief Cree les sous-objets d'un objet heterogene
*/
template <class T> void createSubObject
(
 T* grp,                //!< _SurfaceInterne ou _VolumeInterne
 dim_t dimension,       //!< D2 ou D3
 k2k_lobject_t* lobject //!< liste des objets
 ){

  size_type nb_object_of_type[_ReaderK2K::NB_MAX_ELM_TYPE] =
  { 0, 0, 0, 0, 0, 0, 0, 0};
  for (int ielm = 0; ielm < grp->nb_elm(); ielm++) {
    size_type typ = typeLima2Gibi(grp->elm(ielm)->nb_noeuds(), dimension);
    nb_object_of_type[typ]++;
  }
  
  for (int ityp = _ReaderK2K::BRAS; ityp <= _ReaderK2K::CUBE; ityp++){
    if (nb_object_of_type[ityp] > 0) {
      lobject->elm[lobject->nbr].type = ityp;
      lobject->elm[lobject->nbr].nbe = nb_object_of_type[ityp];
      lobject->elm[lobject->nbr].nbn = 
        nb_object_of_type[ityp] * _ReaderK2K::k2k_NbNode(ityp);
      lobject->elm[lobject->nbr].node =
        new size_type[lobject->elm[lobject->nbr].nbn];

      size_type indn = 0;
      for (int ielm = 0; ielm < grp->nb_elm(); ielm++) {
        if (ityp == typeLima2Gibi(grp->elm(ielm)->nb_noeuds(), dimension))
          for (int in = 0; in < _ReaderK2K::k2k_NbNode(ityp); in++){
            lobject->elm[lobject->nbr].node[indn++] = 
              (size_type)(grp->elm(ielm)->noeud(in)->udata());
          }
      }
      lobject->nbr++;
    }
  }

}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief prepare la liste des objets gibi et celle des objets nommes
  \detail  
  on parcourt d'abord les _Groupe qui sont des objets nommes,
  puis on recherche les _ElementFini qui n'appartiennent pas a un _Groupe
  
  Seuls les _volumeInterne et le _SurfaceInternes peuvent donner des 
  objets gibi de type GROUPE (objet compose)
*/
void _WriterK2KGibi::prepareListes
(
 k2k_lobject_t* lobject, //!< liste des objets gibi 
 k2k_lname_t *lname,    //!< liste des objets nommes
 int nb_node_named      //!< nombre de noeuds nommes
){

  size_type ielm, igrp;

  lobject->nbr = 0;

  /*
    la liste des objets gibi commence par tous les objets nommes
    (cad _Groupe Lima) avec dans l'ordre :
    volume, surface, ligne, nuages (de + d'1 noeud)

    ensuite suivent les sous-groupes eventuels de ces objets nommes
    
    puis les objets _ElementsFinis non nommes (cad qui n'appartiennent pas
    a un _Groupe)
  */

  // on remplit tous les objets nomme de type simple


  //// liste des noms
  lname->nbr = 0;
  lname->max = m_mai->nb_volumes() + m_mai->nb_surfaces() + m_mai->nb_lignes()+
    (m_mai->nb_nuages() - nb_node_named);
  lname->elm = new k2k_name_t[1+lname->max];

  for (igrp = 0; igrp < m_mai->nb_volumes(); igrp++)
    addNameOfGroupe(m_mai->volume(igrp), lname);
  for (igrp = 0; igrp < m_mai->nb_surfaces(); igrp++)
    addNameOfGroupe(m_mai->surface(igrp), lname);
  for (igrp = 0; igrp < m_mai->nb_lignes(); igrp++)
    addNameOfGroupe(m_mai->ligne(igrp), lname);
  for (igrp = 0; igrp < m_mai->nb_nuages(); igrp++)
    if (m_mai->nuage(igrp)->nb_noeuds() > 1)
      addNameOfGroupe(m_mai->nuage(igrp), lname);
  
  //// fin de preparation de la liste des noms


  //// liste des objets gibi

  // dans un premier temps, on ne compte pas les sous groupes ni les objets
  // non nommes

  // initialisation des udata
  for (ielm = 0; ielm < m_mai->nb_noeuds(); ielm++)
    m_mai->noeud(ielm)->set_udata((void*)(ielm + 1)); // le num des noeuds est stocke ds udata

  for (ielm = 0; ielm < m_mai->nb_bras(); ielm++)
    m_mai->bras(ielm)->set_udata(NULL);
  for (ielm = 0; ielm < m_mai->nb_polygones(); ielm++)
    m_mai->polygone(ielm)->set_udata(NULL);
  for (ielm = 0; ielm < m_mai->nb_polyedres(); ielm++)
    m_mai->polyedre(ielm)->set_udata(NULL);

  lobject->nbr = 0;
  lobject->max = m_mai->nb_volumes() + m_mai->nb_surfaces() + 
    m_mai->nb_lignes() + 
    (m_mai->nb_nuages() - nb_node_named);

  size_type nb_object_count = lobject->max;

  /*
    Marquage, comptabilisation et remplissage des k2k_gibi_object des 
    objets nommes.
    nota les k2k_gibi_object sont momentanement stockes dans les udata
    des _Groupe
  */
  for (igrp = 0; igrp < m_mai->nb_volumes(); igrp++)
    lobject->nbr += countLimaGroupe(m_mai->volume(igrp), D3, nb_object_count);

  for (igrp = 0; igrp < m_mai->nb_surfaces(); igrp++)
    lobject->nbr += countLimaGroupe(m_mai->surface(igrp), D2, nb_object_count);

  for (igrp = 0; igrp < m_mai->nb_lignes(); igrp++)
    lobject->nbr += countLimaGroupe(m_mai->ligne(igrp), D1, nb_object_count);

  for (igrp = 0; igrp < m_mai->nb_nuages(); igrp++)
    if (m_mai->nuage(igrp)->nb_elm() > 1) { // nuages a + d'un noeud 
      _NuageInterne* grp = m_mai->nuage(igrp);
      k2k_object_t * object = new k2k_object_t;
      grp->set_udata((void*) object);

      object->type = _ReaderK2K::NOEUD;
      object->nbe = grp->nb_elm();
      object->nbn = object->nbe;
      object->node = new size_type[1+object->nbn];
      for (ielm = 0; ielm < object->nbe; ielm++) {
        object->node[ielm] =
          (size_type)(grp->noeud(ielm)->udata());
      }
      lobject->nbr += 1;
    }
  // fin du marquage


  //// comptage du nombre d'objets gibi non nommes
  _LigneInterne *li_it = new _LigneInterne("");
  m_mai->ajouter(li_it);
  _SurfaceInterne *su_it = new _SurfaceInterne("");
  m_mai->ajouter(su_it);
  _VolumeInterne *vo_it = new _VolumeInterne("");
  m_mai->ajouter(vo_it);

  size_type nb_unamed_object_of_type[_ReaderK2K::NB_MAX_ELM_TYPE] = {0,0,0,0, 0, 0, 0, 0};

  for (ielm = 0; ielm < m_mai->nb_polyedres(); ielm++) {
    _PolyedreInterne* e = m_mai->polyedre(ielm);
    if (e->udata() == NULL) {
      const size_type typ = typeLima2Gibi(e->nb_noeuds(), D3);
      nb_unamed_object_of_type[typ]++;
      vo_it->ajouter(e);
    }
  }
  for (ielm = 0; ielm < m_mai->nb_polygones(); ielm++) {
    _PolygoneInterne* e = m_mai->polygone(ielm);
    if (e->udata() == NULL) {
      const size_type typ = typeLima2Gibi(e->nb_noeuds(), D2);
      nb_unamed_object_of_type[typ]++;
      su_it->ajouter(e);
    }
  }
  for (ielm = 0; ielm < m_mai->nb_bras(); ielm++) {
    _BrasInterne* e = m_mai->bras(ielm);
    if (e->udata() == NULL) {
      nb_unamed_object_of_type[_ReaderK2K::BRAS]++;
      li_it->ajouter(e);
    }
  }

  // comptage des polyedres et polygones ... non nommes
  for (int ityp = _ReaderK2K::NOEUD; ityp <= _ReaderK2K::CUBE; ityp++)
    if (nb_unamed_object_of_type[ityp] > 0)
      lobject->nbr++;

  //// fin comptage du nombre d'objets gibi non nommes

  lobject->max = lobject->nbr;
  lobject->elm = new k2k_object_t[1+lobject->max];
  lobject->nbr = 0;

  // recopie des k2k_gibi_object stockes dans les udata des _Groupe
  for (igrp = 0; igrp < m_mai->nb_volumes()-1; igrp++) {
    memcpy(&lobject->elm[lobject->nbr++], m_mai->volume(igrp)->udata(),
           sizeof(k2k_object_t));
    delete (k2k_object_t*) m_mai->volume(igrp)->udata();
  }
  for (igrp = 0; igrp < m_mai->nb_surfaces()-1; igrp++) {
    memcpy(&lobject->elm[lobject->nbr++], m_mai->surface(igrp)->udata(),
           sizeof(k2k_object_t));
    delete (k2k_object_t*)m_mai->surface(igrp)->udata();
  }
  for (igrp = 0; igrp < m_mai->nb_lignes()-1; igrp++) {
    memcpy(&lobject->elm[lobject->nbr++], m_mai->ligne(igrp)->udata(),
           sizeof(k2k_object_t));
    delete (k2k_object_t*)m_mai->ligne(igrp)->udata();
  }
  for (igrp = 0; igrp < m_mai->nb_nuages(); igrp++) {
    if (m_mai->nuage(igrp)->nb_noeuds() > 1) {
      memcpy(&lobject->elm[lobject->nbr++], m_mai->nuage(igrp)->udata(),
             sizeof(k2k_object_t));
      delete (k2k_object_t*)m_mai->nuage(igrp)->udata();
    }
  }

  //// creation des sous-objets gibi
  size_type ind_obj = 0;
  for (igrp = 0; igrp < m_mai->nb_volumes()-1; igrp++)
    if (lobject->elm[ind_obj++].type == _ReaderK2K::GROUPE)
      createSubObject(m_mai->volume(igrp), D3, lobject);
  
  for (igrp = 0; igrp < m_mai->nb_surfaces()-1; igrp++)
    if (lobject->elm[ind_obj++].type == _ReaderK2K::GROUPE) 
      createSubObject(m_mai->surface(igrp), D2, lobject);
  
  //// creation des objets gibi pour les _ElementFini non nommes
  createSubObject(m_mai->ligne(m_mai->nb_lignes()-1), D1, lobject);  
  createSubObject(m_mai->surface( m_mai->nb_surfaces()-1), D2, lobject);  
  createSubObject(m_mai->volume(m_mai->nb_volumes()-1), D3, lobject);

  m_mai->retirer(li_it);
  m_mai->retirer(su_it);
  m_mai->retirer(vo_it);
}




END_NAMESPACE_LIMA

#endif
