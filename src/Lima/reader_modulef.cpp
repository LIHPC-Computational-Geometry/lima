#ifdef __INTERNE_MODULEF
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_modulef.C
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader_modulef.C
  \brief Interface de lecture / ecriture pour les fichiers modulef
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"
#include "LimaP/io_fortran.h"
#include "LimaP/maillage_it.h"
#include "LimaP/reader_modulef.h"
#include "LimaP/algorithme.h"
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP0_t::SD_NOP0_t ( )	// CP, v 6.27.1, coverity
	: NIVEAU (-1), ETAT (-1), NTACM (-1)
{
	memset (TITRE, '\0', 80 * sizeof (char));
	memset (DATE, '\0', 8 * sizeof (char));
	memset (NOMCRE, '\0', 24 * sizeof (char));
	memset (TYPE, '\0', 4 * sizeof (char));
}	// SD_NOP0_t::SD_NOP0_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_B1_t::SD_B1_t ( )	// CP, v 6.27.1, coverity
	: ADRESS (-1), NB_WORD (-1), TYPE (-1)
{
	memset (NAME, '\0', 4 * sizeof (char));
	memset (COMMENT, '\0', 72 * sizeof (char));
}	// SD_B1_t::SD_B1_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP1_t::SD_NOP1_t ( )	// CP, v 6.27.1, coverity
	: nbr (-1), max (-1), elm (0)
{
}	// SD_NOP1_t::SD_NOP1_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP2_t::SD_NOP2_t ( )	// CP, v 6.27.1, coverity
{
	memset (elm, 0, 27 * sizeof (INT_4));
}	// SD_NOP2_t::SD_NOP2_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP3_t::SD_NOP3_t ( )	// CP, v 6.27.1, coverity
	: elm (0)
{
}	// SD_NOP3_t::SD_NOP3_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP4_t::SD_NOP4_t ( )	// CP, v 6.27.1, coverity
	: coo (0)
{
}	// SD_NOP4_t::SD_NOP4_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP5_t::SD_NOP5_t ( )	// CP, v 6.27.1, coverity
	: elm (0)
{
}	// SD_NOP5_t::SD_NOP5_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::SD_NOP_t::SD_NOP_t ( )	// CP, v 6.27.1, coverity
{
	memset (header, 0, 13 * sizeof (INT_4));
}	// SD_NOP_t::SD_NOP_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::enr_size_t::enr_size_t ( )	// CP, v 6.27.1, coverity
	: reserved (-1), nbo (-1), nbi (-1)
{
}	// enr_size_t::enr_size_t
//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::_ReaderModulef(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format),
	SD ( ), coo_nbr (0), elm_nbr (0)	// CP, v 6.27.1, coverity
{
  
  m_file.ascii = false;
  m_file.open("r");
}


//////////////////////////////////////////////////////////////////////////////
_ReaderModulef::~_ReaderModulef(){
  m_file.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderModulef::readAll ( )
{

	try
	{
		m_mai->debut_lecture (m_file.full_name, 0);
		lire();
		addElements();
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
void _ReaderModulef::readStructure ( ) 
{
	throw read_erreur ("Lecture de la structure non supportée avec le format Moduflef.");
}


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
void _ReaderModulef::lire(){

#ifdef CRAY
  pout("Le format Modulef n'est pas disponible sur CRAY\n");
  return;
#endif


  enr_size_t size; // 
  memset(&SD, 0, sizeof(SD_NOP_t));
  int itmp;

  char buf[TailleBuf + 1];

  //// entete
  // enregistrement reserve
  // |  taille (size)
  // |  |    Longueur du tableau 0 (en nb d'entier sur 4 octets)
  // |  |    |
  // |  |    |   Longueur du tableau 2   
  // |  |    |    |
  // 56 13 6 32 0 27 0 IX IN 1 1 1 1 2 1
  //    ou IX = nombre de coordonnees( nombre de points fois la dimension)
  //    et IN la longueur du tableau 5

  // lecture de la taille (=13)

  fread2(&itmp, sizeof(INT_4), 1, &m_file, "Lecture entete");
  fread2(&itmp, sizeof(INT_4), 1, &m_file, "Lecture entete");
#if CONVERT_ENDIAN
  swap_endian(&itmp, 1);
#endif
  if (itmp != 13) {
    perr("Entete non supporte\n");
    return;
  }

  fread2(SD.header, 13*sizeof(INT_4), 1, &m_file, "Lecture taille entete");
#if CONVERT_ENDIAN
  swap_endian(SD.header, 13);
#endif

  if(SD.header[1] != 32) {
    perr("Longueur tableau NOP0 # 32\n");
    return;
  }
  coo_nbr = SD.header[5];
  elm_nbr = SD.header[6];

  //// SD NOP0 generalites sur le travail
  fread2(&size, sizeof(enr_size_t), 1, &m_file,
         "Lecture taille NOP0");
#if CONVERT_ENDIAN
  swap_endian(&size.nbo, 2);
#endif
  if (size.nbi != 32){
    perr("Longueur tableau NOP0 inattendue (%d octets au lieu de 128)\n", size.nbo);
    return;
  }

  fread2(&SD.NOP0, sizeof(SD_NOP0_t), 1, &m_file, "Lecture NOP0");
#if CONVERT_ENDIAN
  swap_endian(&SD.NOP0.NIVEAU, 3);
#endif

  if (strncmp(SD.NOP0.TYPE, "NOPO", 4)) {
    perr("SD pas du type NOPO");
    return;
  }

  sprintf(buf, "%.80s", SD.NOP0.TITRE); // il y a des blancs
  m_mai->titre(buf);
  sprintf(buf, "%.8s", SD.NOP0.DATE);
  m_mai->date(buf);


  //// saut de NOP1 et des tableaux supplementaires
  if (SD.NOP0.NTACM > 0) {
    SD_B1_t sd_b1;
    for (int inop1 = 0;  inop1 < SD.NOP0.NTACM; inop1++){
       fread2(&size, sizeof(enr_size_t), 1, &m_file, "Lecture taille NOP1");
       fread2(&sd_b1, sizeof(SD_B1_t), 1, &m_file, "Lecture NOP1");
    }
    //fseek(fd, SD.NOP0.NTACM * (sizeof(enr_size_t) + sizeof(SD_B1_t)), SEEK_CUR);
  }

  //// lecture de NOP2
	bool	cancel	= false;
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  fread2(&size, sizeof(enr_size_t), 1, &m_file, "Lecture taille NOP2");
#if CONVERT_ENDIAN
  swap_endian(&size.nbo, 2);
#endif
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::GROUPE, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  if (size.nbi != 27)
    throw read_erreur("Longueur tableau NOP2 inattendue \n");

  fread2(&SD.NOP2, sizeof(SD_NOP2_t), 1, &m_file, "Lecture NOP2");
#if CONVERT_ENDIAN
  swap_endian(SD.NOP2.elm, 27);
#endif

  m_mai->dimension(dim_t(SD.NOP2.elm[NDIM]));
  if (SD.NOP2.elm[NTYCOO] != 2) 
    throw read_erreur("Type des coordonnees # simple precision. non supporte\n");
  if (coo_nbr != SD.NOP2.elm[NDIM] * SD.NOP2.elm[NP]) 
    throw read_erreur("Incoherence de taille tableau NOP4\n");
  //if (elm_nbr != SD.NOP2.elm[LNOP5]) 
  //  throw read_erreur("Incoherence de taille tableau NOP5\n");
  if (SD.NOP2.elm[NTACOO] != 1)
    throw read_erreur("Coordonnees pas en cartesien\n");


  //// saut de NOP3
  if (SD.NOP2.elm[NBEGM] > 0 || SD.header[4] > 0) {
    fread2(&size, sizeof(enr_size_t), 1, &m_file, "Lecture taille NOP3");
    INT_4 *tabtmp = new INT_4[size.nbi];
    fread2(tabtmp, sizeof(INT_4), size.nbi, &m_file, "Lecture NOP3");
    delete [] tabtmp;
  }

  //// lecture de NOP4 : coordonnees des sommets
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 0.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  SD.NOP4.coo = new FLOAT_4[coo_nbr];
  fread2(&size, sizeof(enr_size_t), 1, &m_file, "Lecture taille NOP4");
  int nb_read = fread2(SD.NOP4.coo, sizeof(FLOAT_4), coo_nbr, &m_file, 
                       "Lecture NOP4");
#if CONVERT_ENDIAN
  swap_endian((int_4*)SD.NOP4.coo, coo_nbr);
#endif
	m_mai->donnees_lues (m_file.full_name, 0, ReaderCallback::NOEUD, "", 100.,
	                     cancel);
	if (true == cancel) throw _Reader::LectureAnnulee ( );
  if (nb_read != coo_nbr) 
    throw read_erreur("Longueur tableau NOP4 annoncee incorrecte\n");


  //// lecture de NOP5 description des elements
  SD.NOP5.elm = new INT_4[elm_nbr];
  fread2(&size, sizeof(enr_size_t), 1, &m_file, "Lecture taille NOP5");
  nb_read  = fread2(SD.NOP5.elm, sizeof(INT_4), elm_nbr, &m_file,
                    "Lecture NOP5");
#if CONVERT_ENDIAN
  swap_endian(SD.NOP5.elm, elm_nbr);
#endif
  if (nb_read != elm_nbr) 
    throw read_erreur("Erreur dans le nombre des references lors du decodage du fichier modulef\n");

  // fin de la lecture proprement dite


}

//////////////////////////////////////////////////////////////////////////////
void _ReaderModulef::addElements(){

  // ajout des noeuds
  int id = 1;
  switch (SD.NOP2.elm[NDIM]) {
  case 2:
    {
      for (int in = 0; in < coo_nbr; in += 2) {
	//pout("Point[%2d] (%15.7g, %15.7g)\n", in + 1,
	//   SD.NOP4.coo[in], SD.NOP4.coo[in+1]);
	m_mai->ajouter(_NoeudInterne::create(id++, SD.NOP4.coo[in], SD.NOP4.coo[in+1], 0.));
      }
    }
    break;
    
  case 3:
    {
      for (int in = 0; in < coo_nbr; in +=3) {
	//pout("Point[%2d] (%15.7g, %15.7g, %15.7g)\n", in + 1, 
	//   SD.NOP4.coo[in], SD.NOP4.coo[in+1], SD.NOP4.coo[in+2]);
	m_mai->ajouter(_NoeudInterne::create(id++, SD.NOP4.coo[in], SD.NOP4.coo[in+1],SD.NOP4.coo[in+2]));
      }
    }
    break;
  }
  delete [] SD.NOP4.coo;


  /* dimension des elements (1D, 2D, 3D) */
  const int elm_type[8] = {0, 0, 1, 2, 2, 3, 3, 3};
  const INT_4* tab = SD.NOP5.elm;
  int it = 0;

  _Groupe** groupe = new _Groupe* [SD.NOP2.elm[NDSD] + 1];
  for (int ig = 0; ig < SD.NOP2.elm[NDSD] + 1; ig ++)
    groupe[ig] = NULL;

  /////////
  // creation des elements finis et ajout au groupe concerne 
  it = 0;
  for (int ielm = 0; ielm < SD.NOP2.elm[NE]; ielm++){
    INT_4 NCGE = tab[it++];
    INT_4 NMAE = tab[it++];
    INT_4 NDSDE = tab[it++];
    INT_4 NNO = tab[it++];
	if (NDSDE >= (SD.NOP2.elm [NDSD] + 1))
		throw erreur ("Indice de groupe invalide.\n"); // CP, v 6.27.1, coverity
    
#if 0
    pout("type : %d,  %d, %d, %d\nnoeud:", NCGE, NMAE, NDSDE, NNO);
    for (int in = 0; in < NNO; in++)
      pout(" %d", tab[it+in]);
    pout("\n");
#endif
    size_type node[8];
	memset (node, 0, 8 * sizeof (size_type));	// CP, v 6.27.1, coverity
	if (NNO >= 8)
		throw erreur ("Nombre de noeuds invalides.\n");	// CP, v 6.27.1
    {
      for (int in = 0; in < NNO; in++)
	node[in] = tab[it + in];
    }
    
    if (NCGE == 1) { // nuage de points
      if (groupe[NDSDE] == NULL) {
        groupe[NDSDE] = new _NuageInterne(to_str(NDSDE));
        m_mai->ajouter((_NuageInterne*)groupe[NDSDE]);
      }
      for (int in = 0; in < NNO; in++)
        ((_NuageInterne*) groupe[NDSDE])->ajouter(m_mai->noeud(tab[it+in] - 1));
      
    } else if (NCGE == 2) { // ligne
      if (groupe[NDSDE] == NULL){
        groupe[NDSDE] = new _LigneInterne(to_str(NDSDE));
        m_mai->ajouter((_LigneInterne*)groupe[NDSDE]);
      }

      ajouter_bras(m_mai,
                   node,
                   node + NNO,
                   m_mai->nb_bras()+1);
      ((_LigneInterne*) groupe[NDSDE])->
        ajouter(m_mai->bras(m_mai->nb_bras()-1));
      
      
    } else if (elm_type[NCGE] == 2) { // polygone
      if (groupe[NDSDE] == NULL) {
        groupe[NDSDE] = new _SurfaceInterne(to_str(NDSDE));
        m_mai->ajouter((_SurfaceInterne*)groupe[NDSDE]);
        
      }
      ajouter_polygone(m_mai,
                       node,
                       node + NNO,
                       m_mai->nb_polygones()+1);
      
      ((_SurfaceInterne*) groupe[NDSDE])->
        ajouter(m_mai->polygone(m_mai->nb_polygones()-1));
      
    } else if (elm_type[NCGE] == 3) { // polyedre 
      if (groupe[NDSDE] == NULL) {
        groupe[NDSDE] = new _VolumeInterne(to_str(NDSDE));
        m_mai->ajouter((_VolumeInterne*)groupe[NDSDE]);
      }
      ajouter_polyedre(m_mai,
                       node,
                       node + NNO,
                       m_mai->nb_polyedres()+1);
      ((_VolumeInterne*) groupe[NDSDE])->
        ajouter(m_mai->polyedre(m_mai->nb_polyedres()-1));
      
    }
    
    it += NNO;
#if 0
    if (SD.NOP2.elm[NCOPNP] == 0) { // pas tjs coincidence
      INT_4 NPO = tab[it++];
      pout("point :");
      for (int ip = 0; ip < NPO; ip++)
        pout(" %d", tab[it+ip]);
      it += NPO;
      pout("\n");
    }
    if (NMAE) {
      INT_4 INING = tab[it++];
      INT_4 iref;
      switch (INING) {
      case 1 :
        pout("ref face :");
         for (iref = 0; iref < NCGE; iref++)
           pout(" %d", tab[it++]);
         //pout("\n");
      case 2:
        pout("ref arete :");
        for (iref = 0; iref < NCGE; iref++)
          pout(" %d", tab[it++]);
        //pout("\n");
     case 3:
        pout("ref sommet :");
        for (iref = 0; iref < NCGE; iref++)
          pout(" %d", tab[it++]);
      }
      pout("\n");
    }
#else
    if (SD.NOP2.elm[NCOPNP] == 0){ // pas tjs coincidence
      INT_4 NPO = tab[it++];
      it += NPO;
    }
    if (NMAE) {
      INT_4 INING = tab[it++];
      it += NMAE - 1;
    }
#endif
  }
  delete [] SD.NOP5.elm;
  delete [] groupe;

}
//////////////////////////////////////////////////////////////////////////////
/*!
  \brief comme "fread" avec exception et affichage du message
  \exception read_erreur "Flot de données interrompu\n"
  \exception eof_erreur "Fin de fichier rencontrée prématurement\n"
*/
size_t _ReaderModulef::fread2(void *ptr, size_t size, size_t nitems, File *fd,
                     string msg){
  FILE* stream = fd->fp;

  size_t nb = fread(ptr, size, nitems, stream);
  if(nb != nitems && !feof(stream)) {
    perror("LIMA");
    throw read_erreur(msg + " : Flot de données interrompu\n");
  }

  if(nb != nitems && feof(stream)) {
    perror("LIMA");
    throw eof_erreur(msg + " : Fin de fichier rencontrée prématurement\n");
  }
  return nb;
}

#ifdef CONVERT_ENDIAN
//////////////////////////////////////////////////////////////////////////////
/*!
  \brief Convertie un tableau d'entier (ou de float) (sur 4 octets) bigendian <-> littleendian
*/
void _ReaderModulef::swap_endian
(
 INT_4* tab, /*= ptr sur le tableau d'entier (float) */ 
 INT_4 nbr   /*> le nombre d'entier */
 ){
  int i;
  char* adr = (char*) tab;
  char  byte[2]; 

  for (i = 0; i < nbr; i++){
    byte[0] = *adr;
    byte[1] = *(adr+1);
    *adr = *(adr+3);
    *(adr+1) = *(adr+2);
    *(adr+2) = byte[1];
    *(adr+3) = byte[0];
    adr += 4;
  }
}

#endif

//////////////////////////////////////////////////////////////////////////////
/*!
  \brief interface decriture d'un tableau d'entiers en fortran
  \exception write_erreur "Flot de données interrompu\n"
*/
void _WriterModulef::fwriteInt4(INT_4 *ptr, INT_4 nitems, File* fd)
{
#ifdef CRAY
  pout("Le format Modulef n'est pas disponible sur CRAY\n");
  return;
#else

  int _err;
  if (sizeof(int) != sizeof(INT_4)){
    int nitems_tmp = (int)nitems;
    int* ptmp = new int[nitems];
    for (int iv = 0; iv < nitems; iv++)
      ptmp[iv] = (int)ptr[iv];
    writeint(ptmp, &nitems_tmp, &fd->id, &_err);
    delete [] ptmp;
  } else {
    writeint(ptr, &nitems, &fd->id, &_err);
  }
  if (_err != 0) {
    perror("LIMA");
    throw write_erreur("Flot de données interrompu\n");
  }
#endif
}



void _WriterModulef::ecrire(File *) {


#ifdef CRAY
  pout("Le format Modulef n'est pas disponible sur CRAY\n");
  return;
#endif

  int in;
  int ielm;
  int igrp;
  
  // enr_size_t size = {0, 0, 0};
  _ReaderModulef::SD_NOP_t SD;

  INT_4 header[13] = {6, 32, 0, 27, 0, 0, 0, 1, 1, 1, 1, 2, 1};
  header[5] = m_mai->nb_noeuds() * m_mai->dimension();
  
  //// SD NOP0 generalites sur le travail
  INT_4 tab_nop0[32];
  memset(tab_nop0, ' ', 80 + 8 + 24 + 4);
  memcpy(tab_nop0, m_mai->titre().c_str(), m_mai->titre().size());
  memcpy(&tab_nop0[20], m_mai->date().c_str(), m_mai->date().size());
  memcpy(&tab_nop0[28], "NOPO", 4);
  tab_nop0[29] = 3;
  tab_nop0[30] = tab_nop0[31] = 0;


  //// saut de NOP1 et des tableaux supplementaires


  //// preparation de NOP4 (coordonnees de noeuds)
  SD.NOP4.coo = new FLOAT_4[m_mai->dimension()*m_mai->nb_noeuds()];
  switch (m_mai->dimension()) {
  case D1:
    {
      for (int in = 0; in < m_mai->nb_noeuds(); in++)
	SD.NOP4.coo[in] = (FLOAT_4) m_mai->noeud(in)->x();
    }
    break;
  case D2:
    {
      for (int in = 0; in < m_mai->nb_noeuds(); in++) {
	SD.NOP4.coo[2*in]   = (FLOAT_4) m_mai->noeud(in)->x();
	SD.NOP4.coo[2*in+1] = (FLOAT_4) m_mai->noeud(in)->y();
      }
    }
    break;
  case D3:
    {
      for (int in = 0; in < m_mai->nb_noeuds(); in++) {
	SD.NOP4.coo[3*in]   = (FLOAT_4) m_mai->noeud(in)->x();
	SD.NOP4.coo[3*in+1] = (FLOAT_4) m_mai->noeud(in)->y();
	SD.NOP4.coo[3*in+2] = (FLOAT_4) m_mai->noeud(in)->z();
      }
    }
   break;
  }

  //// preparation de NOP5 (description sequentielle des elements)

  //// preparation de NOP2
  memset(SD.NOP2.elm, 0, 27 * sizeof(INT_4));
  SD.NOP2.elm[_ReaderModulef::NDIM] = m_mai->dimension();
  SD.NOP2.elm[_ReaderModulef::NCOPNP] = 1;
  SD.NOP2.elm[_ReaderModulef::NDSR] = 0;
  SD.NOP2.elm[_ReaderModulef::NOE] = m_mai->nb_noeuds();
  SD.NOP2.elm[_ReaderModulef::NP] = m_mai->nb_noeuds();
  SD.NOP2.elm[_ReaderModulef::NTYCOO] = 2;
  SD.NOP2.elm[_ReaderModulef::LPGDN] = 1;
  SD.NOP2.elm[_ReaderModulef::NTACOO] = 1;

  /* numero max de sous domaine = nb groupes + 1.
      (+ 1) == au cas ou il n'a pas de groupe de defini
     Rq : on ne peut pas mettre les id
          car un noeud et une maille peuvent avoir le meme id ce qui serait
          genant pour la relecture d'un tel fichier */
  SD.NOP2.elm[_ReaderModulef::NDSD] =
    1 + m_mai->nb_nuages() + m_mai->nb_lignes() +
    m_mai->nb_surfaces() + m_mai->nb_volumes();
  
  
  for (ielm = 0; ielm < m_mai->nb_nuages(); ielm++)
    SD.NOP2.elm[_ReaderModulef::NEPO] += m_mai->nuage(ielm)->nb_noeuds();


  /* comptage du nombre d'element (segment, tri, quad...) :
     (nota : noeud n'en fait pas partie)
     les elements sont listes en parcourant les groupes (== sous domaine)
     les elements qui n'appatiennent pas a un groupe doivent aussi
     apparaitre dans le fichier donc il faut aussi parcourir la liste des 
     elements sans toute fois ajouter ceux qui ont deja ete comptes
     ==> utilisation du champ udata */
  int UN = 1;
  for (ielm = 0; ielm < m_mai->nb_bras(); ielm++)
    m_mai->bras(ielm)->set_udata(NULL);
  for (ielm = 0; ielm < m_mai->nb_polygones(); ielm++)
    m_mai->polygone(ielm)->set_udata(NULL);
  for (ielm = 0; ielm < m_mai->nb_polyedres(); ielm++)
    m_mai->polyedre(ielm)->set_udata(NULL);
  
  // nombre de segments
  for (igrp = 0; igrp < m_mai->nb_lignes(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->ligne(igrp)->nb_bras(); ielm++) {
      m_mai->ligne(igrp)->bras(ielm)->set_udata((void*)&UN);
      SD.NOP2.elm[_ReaderModulef::NSEG]++;
    }
  for (ielm = 0; ielm < m_mai->nb_bras(); ielm++) // parcours des elements
    if (m_mai->bras(ielm)->udata() == NULL)
      SD.NOP2.elm[_ReaderModulef::NSEG]++;
  

  // nombre de triangles / quadrangles
  for (igrp = 0; igrp < m_mai->nb_surfaces(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->surface(igrp)->nb_polygones(); ielm++) {
      m_mai->surface(igrp)->polygone(ielm)->set_udata((void*)&UN);
      int nb_node = m_mai->surface(igrp)->polygone(ielm)->nb_noeuds();
      if (nb_node == 3)
        SD.NOP2.elm[_ReaderModulef::NTRI]++;
      else if (nb_node == 4)
        SD.NOP2.elm[_ReaderModulef::NQUA]++;
    }
  for (ielm = 0; ielm < m_mai->nb_polygones(); ielm++) // parcours des elements
    if (m_mai->polygone(ielm)->udata() == NULL) {
      if (m_mai->polygone(ielm)->nb_noeuds() == 3)
        SD.NOP2.elm[_ReaderModulef::NTRI]++;
      else if (m_mai->polygone(ielm)->nb_noeuds() == 4)
        SD.NOP2.elm[_ReaderModulef::NQUA]++;
    }

  // nb de tetraedres, ...
  for (igrp = 0; igrp < m_mai->nb_volumes(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->volume(igrp)->nb_polyedres(); ielm++) {
      m_mai->volume(igrp)->polyedre(ielm)->set_udata((void*)&UN);
      int nb_node = m_mai->volume(igrp)->polyedre(ielm)->nb_noeuds();
      if (nb_node == 4)
        SD.NOP2.elm[_ReaderModulef::NTET]++;
      else if (nb_node == 5)
        SD.NOP2.elm[_ReaderModulef::NPEN]++;
      else if (nb_node == 6)
        SD.NOP2.elm[_ReaderModulef::NHEX]++;
    }

  for (ielm = 0; ielm < m_mai->nb_polyedres(); ielm++) // parcours des elements
    if (m_mai->polyedre(ielm)->udata() == NULL) {
      if (m_mai->polyedre(ielm)->nb_noeuds() == 4)
        SD.NOP2.elm[_ReaderModulef::NTET]++;
      else if (m_mai->polygone(ielm)->nb_noeuds() == 5)
        SD.NOP2.elm[_ReaderModulef::NPEN]++;
      else if (m_mai->polygone(ielm)->nb_noeuds() == 6)
        SD.NOP2.elm[_ReaderModulef::NHEX]++;
    }

  /* fin comptage du nombre d'elements */


  // nb d'elements
  SD.NOP2.elm[_ReaderModulef::NE] =
    SD.NOP2.elm[_ReaderModulef::NEPO] + SD.NOP2.elm[_ReaderModulef::NSEG] +
    SD.NOP2.elm[_ReaderModulef::NTRI] + SD.NOP2.elm[_ReaderModulef::NQUA] +
    SD.NOP2.elm[_ReaderModulef::NTET] + SD.NOP2.elm[_ReaderModulef::NPEN] + 
    SD.NOP2.elm[_ReaderModulef::NHEX];

  //// preparation de NOP5
  int taille = 4 * SD.NOP2.elm[_ReaderModulef::NE] + // 4 valeurs par element
    1 * SD.NOP2.elm[_ReaderModulef::NEPO] + // nb noeud des elements
    2 * SD.NOP2.elm[_ReaderModulef::NSEG] +
    3 * SD.NOP2.elm[_ReaderModulef::NTRI] + 
    4 * SD.NOP2.elm[_ReaderModulef::NQUA] +
    4 * SD.NOP2.elm[_ReaderModulef::NTET] +
    5 * SD.NOP2.elm[_ReaderModulef::NPEN] +
    6 * SD.NOP2.elm[_ReaderModulef::NHEX]; // nota : pas de references

  header[6] = taille;

  SD.NOP5.elm = new INT_4[taille];

  // on utilise les udata pour reperer la position des noeuds dans m_noeud
  INT_4 *number = new INT_4[m_mai->nb_noeuds() + 1];
  for (in = 0; in < m_mai->nb_noeuds() + 1; in++)
    number[in] = in;

  for (in = 0; in < m_mai->nb_noeuds(); in++)
    m_mai->noeud(in)->set_udata((void*)(&number[in + 1]) );





  size_type it = 0;
  int nb_sous_dom = 0;

  //// noeuds 
  for (igrp = 0; igrp < m_mai->nb_nuages(); igrp++)
    for (ielm = 0; ielm < m_mai->nuage(igrp)->nb_noeuds(); ielm++){
      SD.NOP5.elm[it++] = 1; // type 
      SD.NOP5.elm[it++] = 0; // nb ref
      SD.NOP5.elm[it++] = igrp; // num sous domaine
      SD.NOP5.elm[it++] = 1; // nb noeuds 
      SD.NOP5.elm[it++] = *(INT_4*)(m_mai->nuage(igrp)->noeud(ielm)->udata() );
    }
  
  nb_sous_dom += m_mai->nb_nuages();


  //// bras :
  for (igrp = 0; igrp < m_mai->nb_lignes(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->ligne(igrp)->nb_bras(); ielm++) {
      SD.NOP5.elm[it++] = 2; // type 
      SD.NOP5.elm[it++] = 0; // nb ref
      SD.NOP5.elm[it++] = nb_sous_dom + igrp; // num sous domaine
      SD.NOP5.elm[it++] = 2; // nb noeuds
      SD.NOP5.elm[it++] = *(INT_4*)(m_mai->ligne(igrp)->bras(ielm)->noeud(0)->udata());
      SD.NOP5.elm[it++] = *(INT_4*)(m_mai->ligne(igrp)->bras(ielm)->noeud(1)->udata());
    }
  nb_sous_dom += m_mai->nb_lignes();

  for (ielm = 0; ielm < m_mai->nb_bras(); ielm++) // parcours des elements
    if (m_mai->bras(ielm)->udata() == NULL) {
      SD.NOP5.elm[it++] = 2; // type 
      SD.NOP5.elm[it++] = 0; // nb ref
      SD.NOP5.elm[it++] = nb_sous_dom; // num sous domaine
      SD.NOP5.elm[it++] = 2; // nb noeuds
      SD.NOP5.elm[it++] = *(INT_4*)(m_mai->bras(ielm)->noeud(0)->udata());
      SD.NOP5.elm[it++] = *(INT_4*)(m_mai->bras(ielm)->noeud(1)->udata());
    }
  nb_sous_dom += 1;

  //// polygones :
  for (igrp = 0; igrp < m_mai->nb_surfaces(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->surface(igrp)->nb_polygones(); ielm++) {
      SD.NOP5.elm[it++] = m_mai->surface(igrp)->polygone(ielm)->nb_noeuds();
      SD.NOP5.elm[it++] = 0; 
      SD.NOP5.elm[it++] = nb_sous_dom + igrp; 
      SD.NOP5.elm[it++] = m_mai->surface(igrp)->polygone(ielm)->nb_noeuds();
      for (int in = 0; in < m_mai->surface(igrp)->polygone(ielm)->nb_noeuds();
           in++)
        SD.NOP5.elm[it++] = *(INT_4*)
	  (m_mai->surface(igrp)->polygone(ielm)->noeud(in)->udata());
    }
  nb_sous_dom += m_mai->nb_surfaces();

  for (ielm = 0; ielm < m_mai->nb_polygones(); ielm++) // parcours des elements
    if (m_mai->polygone(ielm)->udata() == NULL) {
      SD.NOP5.elm[it++] = m_mai->polygone(ielm)->nb_noeuds();
      SD.NOP5.elm[it++] = 0; 
      SD.NOP5.elm[it++] = nb_sous_dom; 
      SD.NOP5.elm[it++] = m_mai->polygone(ielm)->nb_noeuds();
      for (int in = 0; in < m_mai->polygone(ielm)->nb_noeuds(); in++)
        SD.NOP5.elm[it++] = *(INT_4*)
          (m_mai->polygone(ielm)->noeud(in)->udata());
    }
  nb_sous_dom += 1;


  //// polyedres :
  for (igrp = 0; igrp < m_mai->nb_volumes(); igrp++) // parcours des groupes
    for (ielm = 0; ielm < m_mai->volume(igrp)->nb_polyedres(); ielm++) {
      SD.NOP5.elm[it++] = 1+m_mai->volume(igrp)->polyedre(ielm)->nb_noeuds(); 
      SD.NOP5.elm[it++] = 0;
      SD.NOP5.elm[it++] = nb_sous_dom + igrp;
      SD.NOP5.elm[it++] = m_mai->volume(igrp)->polyedre(ielm)->nb_noeuds();
      for (int in = 0; in < m_mai->volume(igrp)->polyedre(ielm)->nb_noeuds();
           in++)
        SD.NOP5.elm[it++] = *(INT_4*)
	  (m_mai->volume(igrp)->polyedre(ielm)->noeud(in)->udata());
    }
  nb_sous_dom += m_mai->nb_volumes();

  for (ielm = 0; ielm < m_mai->nb_polyedres(); ielm++) // parcours des elements
    if (m_mai->polyedre(ielm)->udata() == NULL) {
      SD.NOP5.elm[it++] = 1+m_mai->polyedre(ielm)->nb_noeuds(); 
      SD.NOP5.elm[it++] = 0;
      SD.NOP5.elm[it++] = nb_sous_dom;
      SD.NOP5.elm[it++] = m_mai->polyedre(ielm)->nb_noeuds();
      for (int in = 0; in < m_mai->polyedre(ielm)->nb_noeuds(); in++)
        SD.NOP5.elm[it++] = *(INT_4*)
          (m_mai->polyedre(ielm)->noeud(in)->udata());
    }
  nb_sous_dom += 1;


#if CONVERT_ENDIAN
  _ReaderModulef::swap_endian(header, 13);
  _ReaderModulef::swap_endian(&tab_nop0[29], 3);
  _ReaderModulef::swap_endian(SD.NOP2.elm, 27);
  _ReaderModulef::swap_endian((INT_4*)SD.NOP4.coo, m_mai->dimension() * m_mai->nb_noeuds());
  _ReaderModulef::swap_endian(SD.NOP5.elm, taille);
#endif
 
  //// ecriture des tableaux

  findfreeul(&ulf, &err);
  if (err)
    throw write_erreur("Impossible d'ouvrir le fichier en ecriture\n");

  char* name = (char*)(m_file.full_name.c_str());
  openf(name, &ulf, &err);
  if (err)
    throw write_erreur("Impossible d'ouvrir le fichier en ecriture\n");
  m_file.id = ulf;

  fwriteInt4(header, 13, &m_file);
  fwriteInt4(tab_nop0, 32, &m_file);
  fwriteInt4(SD.NOP2.elm, 27, &m_file);
  fwriteInt4((INT_4*)SD.NOP4.coo, 
             m_mai->dimension() * m_mai->nb_noeuds(), &m_file);
  fwriteInt4(SD.NOP5.elm, taille, &m_file);

  closef(&ulf, &err);

  //// fin ecriture des tableaux

  delete [] SD.NOP4.coo;
  delete [] SD.NOP5.elm;

  delete [] number;

}


//////////////////////////////////////////////////////////////////////////////
_WriterModulef::_WriterModulef(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Writer(mesh, nom_fichier, format),
	ulf (-1), err (-1)	// CP, v 6.27.1, coverity
{
}
//////////////////////////////////////////////////////////////////////////////
_WriterModulef::~_WriterModulef() {
}

//////////////////////////////////////////////////////////////////////////////
void _WriterModulef::write(format_t){
  ecrire(&m_file);
}

END_NAMESPACE_LIMA
#endif
