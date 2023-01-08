#ifdef __INTERNE_GIBI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_k2kcastem.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Derniere modification le : Tue Aug  3 14:05:28 MEST 2004 (C. Pignerol)
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_k2kcastem.C
  \brief Interface de lecture / ecriture pour les fichiers castem
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include <ctype.h>

#include "LimaP/reader_k2kcastem.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"

BEGIN_NAMESPACE_LIMA
 
//! \return L'identifiant k2kcastem des differents types d'elements
static int  elm_type_id[_ReaderK2K::NB_MAX_ELM_TYPE] = { 0, 1, 2, 4, 8, 23, 16, 14};


//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_info_mesh_t::k_info_mesh_t ( )	// CP, v 6.27.1, coverity
	: NIVEAU (-1), IERR (-1), JDIM (-1), DENSITE (-1.)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_info_pile_t::k_info_pile_t ( )	// CP, v 6.27.1, coverity
	: IPIL (-1), NOBJN (-1), NOBJ (-1)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_objn_t::k_objn_t ( )	// CP, v 6.27.1, coverity
	: nbr (-1), NOM (0), IP (0)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_lnode_t::k_lnode_t ( )	// CP, v 6.27.1, coverity
	: nbr (-1), id (0), coo ( )
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_meleme_t::k_meleme_t ( )	// CP, v 6.27.1, coverity
	: ITYPEL (-1), NBSOUS (-1), NBREF (-1), NBNN (-1), NBELEM (-1),
	  LISOUS (0), LISREF (0), ICOLOR (0), NUM (0)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_pile1_t::k_pile1_t ( )	// CP, v 6.27.1, coverity
	: nbr (-1), elm (0)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::k_pile32_t::k_pile32_t ( )	// CP, v 6.27.1, coverity
	: nbr (-1), pt2nod (0)
{
}
//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::_ReaderK2KCastem(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _ReaderK2K(mesh, nom_fichier, format) {
  
  m_file.ascii = true;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderK2KCastem::~_ReaderK2KCastem(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderK2KCastem::readAll ( )
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
void _ReaderK2KCastem::readStructure() 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Castem.");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
static const int TailleBuf = 128;
#define PILE    2
#define INFOMSH 4
#define END     5

#define PILE_POINT  0
#define PILE_MELEME 1
#define PILE_POINT2 32
#define PILE_CONFIG 33

#ifndef READ_ERREUR
#define READ_ERREUR(_msg) read_erreur(string(_msg) + \
                      "Erreur detectee a la ligne " + to_str(fd->line_num) + \
                      " du fichier.\n")

#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/*!
  \brief lit un tableau d'entiers ecrit en ascii
  \return 0 si ok, 1 si echec
*/
int _ReaderK2KCastem::readInt
(
 File* fd,   //!< fichier 
 int nbval,  //!< nombre d'entiers a lire
 int nbchar, //!< nombre de caracteres par entier
 int bynb,   //!< nombre d'entier par ligne
 int*tab     //!< tableau a remplir
 ) {
  char buf[TailleBuf+1];
  // int nbvalread = 0;
  for (int ival = 0; ival < nbval; ival++) {
    if (ival % bynb == 0)
      fgets2(buf, TailleBuf, fd);
    tab[ival] = atoin(&buf[(ival % bynb) * nbchar], nbchar);
  }
  return 0;
}

int _ReaderK2KCastem::readInt
(
 File* fd,   //!< fichier 
 int nbval,  //!< nombre d'entiers a lire
 int nbchar, //!< nombre de caracteres par entier
 int bynb,   //!< nombre d'entier par ligne
 size_type*tab     //!< tableau a remplir
 ) {
  char buf[TailleBuf+1];
  // int nbvalread = 0;
  for (int ival = 0; ival < nbval; ival++) {
    if (ival % bynb == 0)
      fgets2(buf, TailleBuf, fd);
    tab[ival] = atoin(&buf[(ival % bynb) * nbchar], nbchar);
  }
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
void _ReaderK2KCastem::lire( ){

  bool end = false;

  k_info_mesh_t info_mesh;
  k_info_pile_t info_pile;

	// CP, v 6.27.1, coverity :
	info_mesh.NIVEAU = info_mesh.IERR = info_mesh.JDIM = info_mesh.DENSITE = -1;
	info_pile.IPIL = info_pile.NOBJN = info_pile.NOBJ = -1;

  k2k_lname_t node_name;
  node_name.nbr = 0;
  k2k_lnode_t lnode;
  lnode.nbr = 0;

  k2k_lname_t object_name;
  object_name.nbr = object_name.max = 0;
  k2k_lobject_t object;
  object.nbr = object.max = 0;

  k2k_lname_t pile32_name;
  pile32_name.nbr = pile32_name.max = 0;
  k_pile32_t pile32;
  pile32.nbr = 0;

  int enr_typ;

  m_file.ascii = true;
  m_file.line_num = 0;

  k2k_data_t data;

  bool node_on = false;

  //fgets2(buf, 30, fd);
  enr_typ = readNextEnr(&m_file);
 
  // boucle sur les enregistrements
	bool	cancel	= false;
  while (!end) {

    switch (enr_typ) {
    case -1:
      throw read_erreur("Enregistrement non reconnu\n");
      
    case INFOMSH: 
      readInfoMesh(&m_file, info_mesh);
      data.dim = info_mesh.JDIM;
      m_mai->dimension(dim_t(data.dim));
      enr_typ = readNextEnr(&m_file);
      break;
    case PILE:
      readInfoPile(&m_file, info_mesh.NIVEAU, info_pile);

      switch (info_pile.IPIL) {
      case PILE_POINT: // pile de points

        if (!node_on) {
			m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "",
			                     0., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
          // lecture des noms des noeuds
		if (info_pile.NOBJN < 0)	// CP, v 6.27.1, coverity
			throw range_erreur ("Nombre négatif d'éléments dans la pile.\n");
          node_name.nbr = info_pile.NOBJN;
          readPileObjectNamed(&m_file, node_name);
          
          // lecture des coordonnees
          lnode.nbr = info_pile.NOBJ;
          readNode(&m_file, info_mesh.JDIM, lnode);
          
          node_on = true;
			m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "",
			                     100., cancel);
			if (true == cancel) throw _Reader::LectureAnnulee ( );
        } else {
          perr("Les coordonnees des points ont deja ete lues, la seconde description est ignoree.\n");
        }
        break;
      case PILE_CONFIG: // 2nde pile de points 
        if (!node_on) {
          // lecture des noms des noeuds
			if (info_pile.NOBJN < 0)	// CP, v 6.27.1, coverity
				throw range_erreur ("Nombre négatif d'éléments dans la pile.\n");
          node_name.nbr = info_pile.NOBJN;
          readPileObjectNamed(&m_file, node_name);
          
          // lecture des coordonnees
          lnode.nbr = info_pile.NOBJ;
          readPile33(&m_file, info_mesh.JDIM, lnode);
          
          node_on = true;
        } else {
          perr("Les coordonnees des points ont deja ete lues, la seconde description est ignoree.\n");
        }

         break;
      case PILE_MELEME: // pile d'elements
	{
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "",
		                     0., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	  // lecture des nom des elements
		if (info_pile.NOBJN < 0)	// CP, v 6.27.1, coverity
			throw range_erreur ("Nombre négatif d'éléments dans la pile.\n");
	  object_name.nbr = info_pile.NOBJN;
	  readPileObjectNamed(&m_file, object_name);
	  
	  // lecture des elements
	  object.nbr = info_pile.NOBJ;
	  readPile1(&m_file, object);
	  
	  for (int iobj = 0; iobj < object_name.nbr; iobj++){
	    size_type objn = object_name.elm[iobj].obj - 1;
	    object.elm[objn].name = object_name.elm[iobj].name;
	  }
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 
		                     100., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	}
        break;

      case PILE_POINT2: // tableau d'indirection
	{
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "",
		                     0., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	  if(info_mesh.NIVEAU>=11){	  // lecture des noms
		if (info_pile.NOBJN < 0)	// CP, v 6.27.1, coverity
			throw range_erreur ("Nombre négatif d'éléments dans la pile.\n");
	    pile32_name.nbr = info_pile.NOBJN;
	    readPileObjectNamed(&m_file, pile32_name);
	    
	    // lecture des elements
	    pile32.nbr = info_pile.NOBJ;
	    readPile32(&m_file, pile32);
	  }
		m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "",
		                     100., cancel);
		if (true == cancel) throw _Reader::LectureAnnulee ( );
	}
	break;
      }	// switch (info_pile.IPIL)

      enr_typ = readNextEnr(&m_file);
      break;
    case END:
      end = true;
      break;
    default:
      enr_typ = readNextEnr(&m_file);
      break;
    }	// switch (enr_typ)
  }	// while (!end)

  addNode(lnode, node_name, pile32, pile32_name);

  if (node_name.max > 0)
    delete [] node_name.elm;

  if (pile32_name.max > 0)
    delete [] pile32_name.elm;

  if (pile32.nbr && info_mesh.NIVEAU>=11) {
    prepareObject(object, pile32);
    delete [] pile32.pt2nod;
  }
  addObject(object, object_name);

  if (object_name.max > 0)
    delete [] object_name.elm;

}




//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Recherche le prochain enregistrement dans le fichier
  \return Le type d'enregistrement.
*/
int _ReaderK2KCastem::readNextEnr(File* fd){
  int type = -1;;
  if (fd->ascii){
    char buf[TailleBuf+1];

    do {
      fgets2(buf, TailleBuf, fd);
    } while (strncmp(buf, " ENREGISTREMENT DE TYPE", 23));

    int nb1 = sscanf(buf, "%*23c %4d ", &type);
    if (nb1 != 1)
      throw read_erreur("Impossible d'identifier le type d'enregitrement.\n");

  } else { // binaire


  }
  return type;
}



//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit les informations generales sur le maillage
*/
void _ReaderK2KCastem::readInfoMesh(File* fd, k_info_mesh_t& info){
  if (fd->ascii){
    char buf[TailleBuf+1];
    char keyword[24];
    fgets2(buf, TailleBuf, fd);
    int nb1 = sscanf(buf, "%7c %4d %*14c %4d %*10c %4d",
                         keyword, &info.NIVEAU, &info.IERR, &info.JDIM);
    fgets2(buf, TailleBuf, fd);
    int nb2 = sscanf(buf, "%*8c %E", &info.DENSITE);
    if (nb1 != 4 || nb2 != 1 || strncmp(keyword, " NIVEAU", 7))
      throw READ_ERREUR("Description generale sur le maillage erronee\n");
    
    printf("Lima, NIVEAU FICHIER GIBI sauv : %d\n", info.NIVEAU);
    fflush(stdout);
  } else {  // binaire
    
  }

}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit les informations d'une pile (entete seulement)
*/
void _ReaderK2KCastem::readInfoPile(File* fd, int level, k_info_pile_t& pile){
  if (fd->ascii) {
    const char *format[] = {"%12c %4d %*18c %5d %*11c %5d",
                            "%12c %4d %*18c %8d %*11c %8d"};
    char buf[TailleBuf+1];
    char keyword[24];
    fgets2(buf, TailleBuf, fd);
    int nb1 = sscanf(buf, format[(level  <= 7) ? 0 : 1],
                     keyword, &pile.IPIL, &pile.NOBJN, &pile.NOBJ);
    if (nb1 != 4 || strncmp(keyword, " PILE NUMERO", 12))
      throw READ_ERREUR("Description de la pile erronee\n");

  } else { // binaire

  }

	if (pile.NOBJN < 0)	// CP, v 6.27.1, coverity
		throw READ_ERREUR("Description de la pile erronee\n");
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit les objets nommes d'une pile
*/
void _ReaderK2KCastem::readPileObjectNamed(File* fd, k2k_lname_t& lname){
  lname.max = 0;
  if (lname.nbr == 0)
    return;

  // allocation
  lname.max = lname.nbr;
  lname.elm = new k2k_name_t[lname.nbr];
  
  // lecture
  if (fd->ascii){
    if (lname.nbr > 0) {
      char buf[TailleBuf+1];
      // lecture des noms
      for (int iobj = 0; iobj < lname.nbr; iobj++){
        if (iobj % 8 == 0)
          fgets2(buf, TailleBuf, fd);
// CP version 5.8.1 : eviter "NOM     ".
//        sprintf(lname.elm[iobj].name, "%-8.8s", &buf[1+(iobj % 8) * 9]);
		char	tmp [9];
		sprintf (tmp, "%8.8s", &buf[1+(iobj % 8) * 9]);
		for (int i = 0; i < 9; i++)
			if (0 != isspace (tmp [i]))
			{
				tmp [i]	= '\0';
				break;
			}
        sprintf (lname.elm[iobj].name, "%s", tmp);
      }
      // lecture des rangs
#if 1
      int *tab = new int[lname.nbr];
      readInt(fd, lname.nbr, 8, 10, tab);
      {
	for (int iobj = 0; iobj < lname.nbr; iobj++)
	  lname.elm[iobj].obj = tab[iobj];
      }
      delete [] tab;
#else
      for (int iobj = 0; iobj < lname.nbr; iobj++){
        if (iobj % 10 == 0)
          fgets2(buf, TailleBuf, fd);
        lname.elm[iobj].obj = atoin(&buf[8*(iobj % 10)], 8);
      }
#endif       
#if 0
      for (int iobj = 0; iobj < lname.nbr; iobj++){
        pout("Objet nomme %2d : %s rang %d\n",
             iobj, lname.elm[iobj].name, lname.elm[iobj].obj);
      }
#endif        
    }
  } else { // binaire

  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit les coordonnees des points d'une pile de type 0
*/
void _ReaderK2KCastem::readNode(File* fd, int dim, k2k_lnode_t& lnode){
  lnode.max = 0;
  if (lnode.nbr == 0)
    return;

  lnode.max = lnode.nbr + 1;
  lnode.elm = new k2k_node_t[lnode.max];
  if (fd->ascii) {
    char buf[TailleBuf+1];
    int ivalueline = 0;
    for (int in = 0; in < lnode.nbr; in++){
      lnode.elm[in].id = in + 1;
      lnode.elm[in].name = NULL;
      lnode.elm[in].coo.n.y = lnode.elm[in].coo.n.z = 0.;
      for (int idir = 0; idir < dim; idir++){
        if ((ivalueline % 3) == 0) // 3 valeurs par ligne
          fgets2(buf, TailleBuf, fd);
        lnode.elm[in].coo.d[idir] = atofn(&buf[1+(ivalueline % 3) * 22], 21);
        ivalueline++;
      }
      if ((ivalueline % 3) == 0) // 3 valeurs par lignes
        fgets2(buf, TailleBuf, fd);
      ivalueline++; // on ne lit pas la densite du point
    }
  } else { // binaire

  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit une pile de type 33. Coordonnees des points (!= pile 0)
*/
void _ReaderK2KCastem::readPile33(File* fd, int dim, k2k_lnode_t& lnode){
  lnode.max = 0;

  if (lnode.nbr != 1)
    throw read_erreur("Fichier castem : Pile 33 erronnee\n");

  char buf[TailleBuf+1];
  fgets2(buf, TailleBuf, fd);
  int nbval;
  if (sscanf(buf, "%d", &nbval) != 1)
    throw read_erreur("Fichier castem : Pile 33 erronnee\n");

  lnode.nbr = nbval / (int)(dim + 1);
  if ((lnode.nbr * (dim+1)) != nbval)
    throw read_erreur("Fichier castem : Pile 33 erronnee\n");

  readNode(fd, dim, lnode);

}



//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit une pile de type 1
*/
void _ReaderK2KCastem::readPile1(File* fd, k2k_lobject_t& pile){
  pile.max = pile.nbr;
  if (pile.nbr == 0)
    return;

  pile.elm = new k2k_object_t[pile.nbr];

  // bcl sur les elements
  for (int ielm = 0; ielm < pile.nbr; ielm++){
    readObject(fd, &pile.elm[ielm]); 
  }
} 

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Retourne le type d'objet (noeud, bra, quadrangle, prisme...)
*/
int _ReaderK2KCastem::ObjectType
(
 int id, //> identifiant 2k2
 int nbn //> nombre de noeuds
 ){
  if (id == 0)
    return GROUPE;
  int itype;
  for (itype = NOEUD; itype < NB_MAX_ELM_TYPE; itype++)
    if (id == elm_type_id[itype] && nbn == k2k_NbNode(itype))
      break;
  if (itype == NB_MAX_ELM_TYPE)
    throw read_erreur(string("Element non identifie. id= ")+to_str(id)+
                      " nombre de noeuds= " +to_str(nbn));
  return itype;
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit un objet MELEME
*/
#define INT_K2K_SIZE (8)
void _ReaderK2KCastem::readObject(File* fd, k2k_object_t* obj){
  if (fd->ascii) {
    char buf[TailleBuf+1];
    fgets2(buf, TailleBuf, fd);
//    INT_4 ITYPEL, NBSOUS, NBREF, NBNN, NBELEM;
	INT_4 ITYPEL = -1, NBSOUS = -1, NBREF = -1, NBNN = -1, NBELEM = -1;	// v 6.3.2

	// v 6.3.2 : la ligne suivante met ce lecteur en erreur :
	//       14       0       0       812600000
	// Valeurs affectées :
	// ITYPEL = 14, NBSOUS = 0, NBREF = 0, NBNN = 81260000, NBELEM = 0 ...
	// sscanf (..., "%8d ...) : le 8 est un nombre maximum de caractères
	// encodant le nombre. scanf n'a pas d'option pour spécifier le nombre de
	// caractères exact encodant un nombre ...
//    int nb1 = sscanf(buf, "%8d %8d %8d %8d %8d", &ITYPEL, &NBSOUS, // v 6.3.2
//                     &NBREF, &NBNN, &NBELEM);
	const size_t	length	= strlen (buf);			// v 6.3.2
	char	itypel [INT_K2K_SIZE + 1]	= {'\0'},
			nbsous [INT_K2K_SIZE + 1]	= {'\0'},
			nbref [INT_K2K_SIZE + 1]	= {'\0'},
			nbnn [INT_K2K_SIZE + 1]		= {'\0'},
			nbelem [INT_K2K_SIZE + 1]	= {'\0'};	// v 6.3.2
	strncpy (itypel,	buf, INT_K2K_SIZE);
	if (length > INT_K2K_SIZE)
		strncpy (nbsous,	buf + INT_K2K_SIZE, INT_K2K_SIZE);
	if (length > (2 * INT_K2K_SIZE))
		strncpy (nbref,		buf + 2 * INT_K2K_SIZE, INT_K2K_SIZE);
	if (length > (3 * INT_K2K_SIZE))
		strncpy (nbnn,		buf + 3 * INT_K2K_SIZE, INT_K2K_SIZE);
	if (length > (4 * INT_K2K_SIZE))
		strncpy (nbelem,	buf + 4 * INT_K2K_SIZE, INT_K2K_SIZE);
	itypel [INT_K2K_SIZE] = nbsous [INT_K2K_SIZE] = nbref [INT_K2K_SIZE] =
						nbnn [INT_K2K_SIZE] = nbelem [INT_K2K_SIZE]	= '\0';
//    if (nb1 != 5)	//  v 6.3.2
	if ((1 != sscanf (itypel, "%d", &ITYPEL)) ||
		(1 != sscanf (nbsous, "%d", &NBSOUS)) ||
	    (1 != sscanf (nbref, "%d", &NBREF))   ||
	    (1 != sscanf (nbnn, "%d", &NBNN))     ||
	    (1 != sscanf (nbelem, "%d", &NBELEM)))
      throw READ_ERREUR("Impossible de lire les informations sur l'element meleme\n");
    
    // identification du type
    obj->type = ObjectType(ITYPEL, NBNN);

    obj->name = NULL;

    if (obj->type == GROUPE){
      // la liste des noeuds devient la liste des sous groupes
      obj->nbn = 1;
      obj->nbe = NBSOUS;
    } else {
      obj->nbn = NBNN;
      obj->nbe = NBELEM;
    }
    
    obj->elem = new size_type[1+obj->nbe];
    obj->node = new size_type[1+obj->nbn * obj->nbe];

    // lecture / saut des sous objets
    if (NBSOUS){
      int issobj;
      if (obj->type == GROUPE){
        // on remplit le tableau obj->nod avec les numeros des groupes 
#if 0
        //10 numeros par ligne (8 caracteres)
        //int *tab = new int[obj->nbe];
        if (readInt(fd, obj->nbe, 8, 10, obj->node))
          throw read_erreur("Lecture de l'element impossible\n");
        //delete [] tab;
#else
        for (issobj = 0; issobj < NBSOUS; issobj++){
          
          // lecture d'une ligne du fichier. 10 numeros par ligne (8 caracteres)
          if ((issobj % 10) == 0) fgets2(buf, TailleBuf, fd);

          // analyse de la ligne 
          int ssobj_num = 0;
          int fmtobj = 8;
          obj->node[issobj] = atoin(&buf[(issobj % 10) * fmtobj], fmtobj)-1;
        }
#endif  
      } else { 
        // saut 
        for (issobj = 0; issobj < NBSOUS; issobj += 10)
          fgets2(buf, TailleBuf, fd);
      }
    } 

    //// les references ne sont jamais lues
    // au + 10 references par ligne
    for (int iref = 0; iref < NBREF; iref += 10)
      fgets2(buf, TailleBuf, fd);
    
    if (obj->type == GROUPE)
      return; // pour un GROUPE, c'est la fin de l'enregistrement

    // saut des couleurs (1 espace + 4 caracteres par couleur) 
    // au + 16 couleurs par ligne
    int ielem;
    for (ielem = 0; ielem < NBELEM; ielem += 10)
      fgets2(buf, TailleBuf, fd);
    
    //// lecture / saut des elements
#if 1
    if (readInt(fd, obj->nbn * obj->nbe, 8, 10, obj->node))
      throw read_erreur("Lecture de l'element impossible\n");
#else
    int nb_val_read = 0;
    int fmtnod = 8;
    int nodlig = 10;
    for (ielem = 0; ielem < obj->nbe; ielem++){
      //pout("Element %d :\n", ielem);
      int inode;
      for (inode = 0; inode < obj->nbn; inode++) {
        
        // lecture d'une ligne du fichier
        // nodlig valeur par ligne
        if (nb_val_read % nodlig == 0)
          fgets2(buf, TailleBuf, fd);
        int elem_num;
        elem_num = atoin(&buf[(nb_val_read % nodlig)* fmtnod], fmtnod);
        if (elem_num == 0) 
          throw READ_ERREUR("numero de point invalide\n");
        
        obj->nod[nb_val_read] = elem_num;
        nb_val_read++;
        //pout(" %d ", elem_num);
        
      }
      //pout("\n");
    }
#endif    

  } else { // binaire
    
  }

  
}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ajoute au maillage les noeuds lus
*/
void _ReaderK2KCastem::addNode
(
 k2k_lnode_t& lnode,      //!< liste des noeuds
 k2k_lname_t& node_name,  //!< 1ere liste de noms de noeuds
 k_pile32_t&  pile32,     //!< liste des indirections
 k2k_lname_t& pile32_name //!< 2eme liste de noms de noeuds (avec indirection)
 ){
  
 int iobj;

  //  indirection de la pile 32 
  for (iobj = 0; iobj < pile32.nbr; iobj++)
    lnode.elm[iobj].id = pile32.pt2nod[iobj];
  //lnode.elm[pile32.pt2nod[iobj] - 1].id = iobj + 1; // <- faux ? ? ? ?

  // ajout des noeuds
  for ( iobj = 0; iobj < lnode.nbr; iobj++){
    m_mai->ajouter(_NoeudInterne::create(iobj + 1, 
				  lnode.elm[iobj].coo.n.x, 
				  lnode.elm[iobj].coo.n.y,
				  lnode.elm[iobj].coo.n.z));
    /*ajouter(_NoeudInterne::create(lnode.elm[iobj].id, 
				  lnode.elm[iobj].coo.n.x, 
				  lnode.elm[iobj].coo.n.y,
				  lnode.elm[iobj].coo.n.z));*/
  }

  // NOTA : un meme noeud peut apparaitre pls fois dans l'une et (ou) l'autre
  // des 2 listes de points nommes suivantes.

  // nuages : liste de nom 1
  for ( iobj = 0; iobj < node_name.nbr; iobj++) {
    _NuageInterne *grp = new _NuageInterne(node_name.elm[iobj].name);
    m_mai->ajouter(grp);
    grp->ajouter(m_mai->noeud(node_name.elm[iobj].obj - 1));
  }

  // nuages : liste de nom 2
  for ( iobj = 0; iobj < pile32_name.nbr; iobj++) {
    _NuageInterne *grp = new _NuageInterne(pile32_name.elm[iobj].name);
    m_mai->ajouter(grp);

//    grp->ajouter(m_mai->noeud(pile32_name.elm[iobj].obj-1));// 	COMMENT CP
// Correction bogue : indirection supplementaire + noeud_id :
    grp->ajouter(m_mai->noeud_id(pile32.pt2nod[pile32_name.elm[iobj].obj - 1]));
  }

  delete [] lnode.elm;
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Exploite la table d'indirection (Pile 32)
  les indices lu dans object.elm[ielm].node[in] sont remplace par 
  pile32.pt2nod[object.elm[ielm].node[in] - 1]

  \warning Dans certains fichiers de maillage, le nombre d'indirections
  est inferieur au nombre de points (coordonnees). C'est pour cela qu'on ne 
  peut pas exploiter ces numeros comme des identifiants.
*/
void _ReaderK2KCastem::prepareObject
(
 k2k_lobject_t& object, //!< Liste d'objets
 k_pile32_t&  pile32    //!< liste des indirections
) {
  size_type iobj, in;
  
  for (iobj = 0; iobj < object.nbr; iobj++){
    if (object.elm[iobj].type != GROUPE) {
      size_type nb_node = object.elm[iobj].nbe * 
        k2k_NbNode(object.elm[iobj].type);
      for (in = 0; in < nb_node; in++)
        object.elm[iobj].node[in] =
          pile32.pt2nod[object.elm[iobj].node[in] - 1];
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ajoute au maillage le groupe
*/
void _ReaderK2KCastem::addGroup
(
 char *name,       //!< nom du groupe
 k2k_object_t* obj //!< objet gibi
 ){
  if (obj->type == NOEUD) {
    // --> nuage
    _NuageInterne *grp_it = NULL;
    if(name) {
      grp_it = new _NuageInterne(name);
      m_mai->ajouter(grp_it);
      addNuage(grp_it, obj);
    }
  } else if (obj->type == BRAS) {
    // --> ligne
    _LigneInterne *grp_it = NULL;
    if(name) {
      grp_it = new _LigneInterne(name);
      m_mai->ajouter(grp_it);
    }
    addLigne(grp_it, obj);
  } else if (elmTypeDimension(obj->type) == D2) {
    // TRIANGLE ou QUADRANGLE --> surface
    _SurfaceInterne *grp_it = NULL;
    if(name) {
      grp_it = new _SurfaceInterne(name);
      m_mai->ajouter(grp_it);
    }
    addSurface(grp_it, obj);
  } else {
    // PRISME ou TETRAEDRE ou CUBE --> volume
    _VolumeInterne *grp_it = NULL;
    if(name) {
      grp_it = new _VolumeInterne(name);
      m_mai->ajouter(grp_it);
    }
    addVolume(grp_it, obj);
  }
}

void _ReaderK2KCastem::addNuage
(
 _NuageInterne* grp_it,
 k2k_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->type);
  for (ielem = 0; ielem < obj->nbe; ielem++)
    grp_it->ajouter(m_mai->noeud(obj->node[ielem]-1));
}

void _ReaderK2KCastem::addLigne
(
 _LigneInterne* grp_it,
 k2k_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->type);
  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->bras(obj->elem[ielem]));
    else{
      obj->elem[ielem] = m_mai->nb_bras();
      ajouter_bras(m_mai,
		   &obj->node[ielem * nb_nod],
		   &obj->node[ielem * nb_nod] + nb_nod,
		   obj->elem[ielem] + 1);
    }
  } 
}

void _ReaderK2KCastem::addSurface
(
 _SurfaceInterne* grp_it,
 k2k_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->type);
  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->polygone(obj->elem[ielem]));
    else{
      obj->elem[ielem] = m_mai->nb_polygones();
      ajouter_polygone(m_mai, 
		       &obj->node[ielem * nb_nod],
		       &obj->node[ielem * nb_nod] + nb_nod,
		       obj->elem[ielem] + 1);
    }
  }
}

void _ReaderK2KCastem::addVolume
(
 _VolumeInterne* grp_it,
 k2k_object_t* obj //!< objet gibi
 ){
  int ielem;
  int nb_nod = k2k_NbNode(obj->type);
  for (ielem = 0; ielem < obj->nbe; ielem++) {
    if(grp_it)
      grp_it->ajouter(m_mai->polyedre(obj->elem[ielem]));
    else{
      obj->elem[ielem] = m_mai->nb_polyedres();
      ajouter_polyedre(m_mai,
		       &obj->node[ielem * nb_nod],
		       &obj->node[ielem * nb_nod] + nb_nod,
		       obj->elem[ielem] + 1);
    }
  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ajoute au maillage les objets gibi lus
*/
/* c'est la meme fonction que dans _MaillageGibi */
void _ReaderK2KCastem::addObject
(
 k2k_lobject_t object, //!< Liste d'objnets
 k2k_lname_t object_name
 ){


  int iobj, objn;

  // cas des groupes homogenes

  // ajout des elements
  for(iobj = 0; iobj < object.nbr; iobj++){
    if (object.elm[iobj].type != GROUPE) {
      addGroup(NULL, &object.elm[iobj]);
    }
  }
  // ajout des groupes
  for(objn = 0; objn < object_name.nbr; objn++){
    iobj = object_name.elm[objn].obj - 1;
    if (object.elm[iobj].type != GROUPE) {
      addGroup(object_name.elm[objn].name, &object.elm[iobj]);
    }
  }

  // cas des groupes heterogenes
  for(objn = 0; objn < object_name.nbr; objn++){
    iobj = object_name.elm[objn].obj - 1;
//    if (object_name.elm[objn].name && object.elm[iobj].type == GROUPE) {
    if (object.elm[iobj].type == GROUPE) {	// CP, v 6.27.1, coverity
      k2k_object_t* grp = &object.elm[iobj];
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
        size_type obj_num = grp->node[issobj];
        k2k_object_t* obj = &object.elm[obj_num];

	if (obj->type == NOEUD) {
	  for (ielem = 0; ielem < obj->nbn; ielem++)
	    nu_it->ajouter(m_mai->noeud(obj->node[ielem] - 1));
	}
	else if (obj->type == BRAS) {
	  for (ielem = 0; ielem < obj->nbe; ielem++)
	    li_it->ajouter(m_mai->bras(obj->elem[ielem]));
	} 
	else if (elmTypeDimension(obj->type) == D2) {
	  for (ielem = 0; ielem < obj->nbe; ielem++)
	    su_it->ajouter(m_mai->polygone(obj->elem[ielem]));
	} 
	else {
	  for (ielem = 0; ielem < obj->nbe; ielem++)
	    vo_it->ajouter(m_mai->polyedre(obj->elem[ielem]));
	}
      } // fin bcl ss-obj gibi

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
    delete [] object.elm[iobj].elem;
    delete [] object.elm[iobj].node;
  }
  if (object.nbr > 0)
    delete [] object.elm;
 

}


//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Lit une pile de type 32 (table d'indirection)
*/
void _ReaderK2KCastem::readPile32(File* fd, k_pile32_t& pile){
  if (pile.nbr <= 0)
    return;
  pile.pt2nod = new int[pile.nbr];

#if 0
  pout("Pile de type 32\n");
#endif

  if (fd->ascii) {
    char buf[TailleBuf+1];
    fgets2(buf, TailleBuf, fd);
    int nb1 = atoin(buf, 8);
    if (nb1 != pile.nbr)
      throw READ_ERREUR("Pile de type 32 incoherente\n");
    readInt(fd, pile.nbr, 8, 10, pile.pt2nod);


  } else { // binaire

  }
}

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Ecrit les objets nommes d'une pile
*/
void _WriterK2KCastem::writePileObjectNamed(File* fd, k2k_lname_t& lname){
  size_type ielm;

  for (ielm = 0; ielm < lname.nbr; ielm++) { // noms
    if (ielm % 8 == 0) fprintf2(fd, "\n");
    fprintf2(fd, " %-8.8s",  lname.elm[ielm].name);
  }
  for (ielm = 0; ielm < lname.nbr; ielm++) { // indices des noms
    if (ielm % 10 == 0) fprintf2(fd, "\n");
    fprintf2(fd, "%8d",  lname.elm[ielm].obj);
  }


}


//////////////////////////////////////////////////////////////////////////////
void _WriterK2KCastem::ecrire(File* fd){

  size_type ielm, iobj;

  // ecriture en tete
  fprintf2(fd, 
          " ENREGISTREMENT DE TYPE   4\n"
          " NIVEAU   9 NIVEAU ERREUR   0 DIMENSION%4d\n"
          " DENSITE 0.00000E+00\n", (int)m_mai->dimension());

  // preparation des noeuds
  // initialisation des udata
  for (ielm = 0; ielm < m_mai->nb_noeuds(); ielm++)
    m_mai->noeud(ielm)->set_udata((void*)(ielm + 1)); // le num des noeuds est stocke ds udata

  k2k_lname_t node_name;
  node_name.nbr = 0;
  node_name.max = 0;
  for (ielm = 0; ielm < m_mai->nb_nuages(); ielm++)
    if (m_mai->nuage(ielm)->nb_noeuds() == 1)
      node_name.max++;

  if (node_name.max > 0){
    node_name.elm = new k2k_name_t[node_name.max];
    for (ielm = 0; ielm < m_mai->nb_nuages(); ielm++)
      if (m_mai->nuage(ielm)->nb_noeuds() == 1) {
        sprintf(node_name.elm[node_name.nbr].name, "%-8.8s", m_mai->nuage(ielm)->nom().c_str());
        node_name.elm[node_name.nbr].obj = 
          (size_type)(m_mai->nuage(ielm)->noeud(0)->udata());
        node_name.nbr++;
      }
  }
  
  // ecriture noeuds
  if (m_mai->nb_noeuds() > 0) {
    fprintf2(fd,
             " ENREGISTREMENT DE TYPE   2\n"
             " PILE NUMERO   0NBRE OBJETS NOMMES%8dNBRE OBJETS%8d",
             node_name.nbr, m_mai->nb_noeuds());
    
    writePileObjectNamed(fd, node_name);
    
    int nb_val = 0;
    for (ielm = 0; ielm < m_mai->nb_noeuds(); ielm++) { // coordonnees
      for (int idim = D1; idim <= m_mai->dimension(); idim++) {
        if (nb_val % 3 == 0) fprintf2(fd, "\n");
        fprintf2(fd, " %21.14E",  m_mai->noeud(ielm)->coo((dim_t)idim));
        nb_val++;
      }
      if (nb_val % 3 == 0) fprintf2(fd, "\n");
      fprintf2(fd, " %21.14E",  0.);
      nb_val++;
    }

    // tableau d'indirection des noeuds (pile32)
    fprintf2(fd,
             "\n ENREGISTREMENT DE TYPE   2\n"
             " PILE NUMERO  32NBRE OBJETS NOMMES%8dNBRE OBJETS%8d\n",
             0, m_mai->nb_noeuds());
    fprintf2(fd,"%8d", m_mai->nb_noeuds());
    for (ielm = 0; ielm < m_mai->nb_noeuds(); ielm++) {// id des noms
      if (ielm % 10 == 0) fprintf2(fd, "\n");
      fprintf2(fd,"%8d", m_mai->noeud(ielm)->id());
    }
  }

  // preparation objet gibi/castem
  k2k_lname_t lname;
  k2k_lobject_t lobject;
  
  ((_WriterK2KGibi*)(this))->prepareListes(&lobject, &lname, node_name.nbr);
  
  //pout("((_MaillageGibi*)(this))->prepareListes(&lobject, &lname, node_name.nbr);");

  if (lobject.nbr > 0){
    fprintf2(fd,
             "\n ENREGISTREMENT DE TYPE   2\n"
             " PILE NUMERO   1NBRE OBJETS NOMMES%8dNBRE OBJETS%8d",
             lname.nbr, lobject.nbr);
    
    writePileObjectNamed(fd, lname);

    for (iobj = 0; iobj < lobject.nbr; iobj++) {
      if (lobject.elm[iobj].type == _ReaderK2K::GROUPE) {
        fprintf2(fd, "\n       0%8d       0       0       0",
               lobject.elm[iobj].nbe);
        for (int issobj = 0; issobj < lobject.elm[iobj].nbe; issobj++) {
          if (issobj % 10 == 0) fprintf2(fd, "\n");
          fprintf2(fd, "%8d", lobject.elm[iobj].node[issobj]+1);
        }
      } else {
        fprintf2(fd, "\n%8d       0       0%8d%8d",
                 elm_type_id[lobject.elm[iobj].type], 
                 _ReaderK2K::k2k_NbNode(lobject.elm[iobj].type),
                 lobject.elm[iobj].nbe);
        
        /* couleurs toutes a 7 */
        for (ielm = 0; ielm < lobject.elm[iobj].nbe; ielm++) {
          if (ielm % 10 == 0) fprintf2(fd, "\n");
          fprintf2(fd, "       7");
        }
        for (ielm = 0; ielm < lobject.elm[iobj].nbn; ielm++) {
          if (ielm % 10 == 0) fprintf2(fd, "\n");
          fprintf2(fd, "%8d", lobject.elm[iobj].node[ielm]);
        }
      }
    }
  }
  fprintf2(fd, "\n ENREGISTREMENT DE TYPE   5\n");

  // liberation
  if (node_name.max > 0)
    delete [] node_name.elm;

  if (lobject.max > 0) {
    for (ielm = 0; ielm < lobject.max; ielm++)
      delete [] lobject.elm[ielm].node;
    delete [] lobject.elm;
  }
  if (lname.max > 0)
    delete [] lname.elm;

}



//////////////////////////////////////////////////////////////////////////////
_WriterK2KCastem::_WriterK2KCastem(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Writer(mesh, nom_fichier, format) {
//  m_file.open("wt");
  m_file.open("w");
}
//////////////////////////////////////////////////////////////////////////////
_WriterK2KCastem::~_WriterK2KCastem() {
  m_file.close();
}

//////////////////////////////////////////////////////////////////////////////
void _WriterK2KCastem::write(format_t ){
  ecrire(&m_file);
}


END_NAMESPACE_LIMA
#endif
