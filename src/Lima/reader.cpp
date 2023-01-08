/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader.C
 Cree le : Fri Nov 10 16:37:43 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file reader.C
  \brief Interface pour les modules de lecture
*/

#include "LimaP/reader.h"
#ifdef __INTERNE_IDEAS
#include "LimaP/reader_ideas.h"
#endif
#ifdef __INTERNE_ABAQUS
#include "LimaP/reader_abaqus.h"
#endif
#ifdef __INTERNE_INGRID
#include "LimaP/reader_ingrid.h"
#endif
#ifdef __INTERNE_GIBI
#include "LimaP/reader_k2kcastem.h"
#include "LimaP/reader_k2kgibi.h"
#endif
#ifdef __INTERNE_MALI
#include "LimaP/reader_mali.h"
#endif
#ifdef __INTERNE_MALIPP
#include "LimaP/reader_malipp.h"
#endif
#ifdef __INTERNE_MALIPP2
#include "LimaP/malipp2_factory.h"
#endif
#ifdef __INTERNE_MAZE
#include "LimaP/reader_maze.h"
#endif
#ifdef __INTERNE_MODULEF
#include "LimaP/reader_modulef.h"
#endif
#ifdef __INTERNE_ICEMCFD
#include "LimaP/reader_icemcfd.h"
#endif


#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

const char* _liste_format_lecture[]={
#ifdef __INTERNE_MALI
  (char*) "unf, format de fichier MALI",
#endif
#ifdef __INTERNE_MALIPP
  (char*) "mli, format de fichier MALI++",
  (char*) "mliz, format (obsolète) de fichier MALI++ compressé avec la zlib",
#endif 
#ifdef __INTERNE_MALIPP2
  (char*) "mli2, format de fichier MALI++",
#endif
#ifdef __INTERNE_IDEAS
  (char*) "unv, format de fichier IDEAS",
#endif
#ifdef __INTERNE_ABAQUS
  (char*) "inp, format de fichier ABAQUS",
#endif
#ifdef __INTERNE_GIBI
  (char*) "gib, format de fichier GIBI",
  (char*) "sort, format de fichier GIBI",
  (char*) "cas, format de fichier CASTEM",
  (char*) "sauv, format de fichier CASTEM",
#endif
#ifdef __INTERNE_MODULEF
  (char*) "nop, format de fichier MODULEF",
  (char*) "des, format de fichier MODULEF",
#endif
#ifdef __INTERNE_MAZE
  (char*) "maz, format de fichier DYNA2D",
#endif
#ifdef __INTERNE_INGRID
  (char*) "ing, format de fichier DYNA3D",
#endif
#ifdef __INTERNE_ICEMCFD
  (char*) "ice, format de fichier ICE",
  (char*) "uns, format de fichier ICE",
#endif
  NULL
};

const char** liste_format_lecture()
{
  return _liste_format_lecture;
}


//////////////////////////////////////////////////////////////////////////////
_Reader::_Reader(_MaillageInterne* mesh, 
                 const string& nom_fichier, 
                 format_t format,
                 size_type num) : m_file(nom_fichier, format, num) , m_filecpp(nom_fichier, format, num){
  int io;
  
  m_mai = mesh;
};


//////////////////////////////////////////////////////////////////////////////
/*!
  Detecte le format de fichier a partir de son extension
*/
format_t _Reader::detectFormat(const string& fichier){

  format_t format = SUFFIXE;

  if(strrchr(fichier.c_str(),'.')){
#ifdef __INTERNE_MALI
    if(!strcmp(".unf",strrchr(fichier.c_str(),'.')))
      format = MALI;
#endif
#ifdef __INTERNE_MALIPP
    if(!strcmp(".mli",strrchr(fichier.c_str(),'.')))
      format = MALIPP;
    else if(!strcmp(".mliz",strrchr(fichier.c_str(),'.')))
      format = MALIPP_Z;
#endif 
#ifdef __INTERNE_MALIPP2
	if(!strcmp(".mli2",strrchr(fichier.c_str(),'.')))
		format = MALIPP2;
#endif
#ifdef __INTERNE_IDEAS
   if(!strcmp(".unv",strrchr(fichier.c_str(),'.')))
      format = IDEAS;
#endif
#ifdef __INTERNE_ABAQUS
   if(!strcmp(".inp",strrchr(fichier.c_str(),'.')))
      format = ABAQUS;
#endif
#ifdef __INTERNE_GIBI
    if(!strcmp(".gib",strrchr(fichier.c_str(),'.')) ||
       !strcmp(".gib8",strrchr(fichier.c_str(),'.')) ||
       !strcmp(".sort",strrchr(fichier.c_str(),'.')))
      format = GIBI;
    if(!strcmp(".cas",strrchr(fichier.c_str(),'.')) ||
       !strcmp(".sauv",strrchr(fichier.c_str(),'.')))
      format = CASTEM;
#endif
#ifdef __INTERNE_MODULEF
    if(!strcmp(".nop",strrchr(fichier.c_str(),'.')) ||
       !strcmp(".des",strrchr(fichier.c_str(),'.')))
      format = MODULEF;
#endif
#ifdef __INTERNE_MAZE
    if(!strcmp(".maz",strrchr(fichier.c_str(),'.')))
      format = MAZE;
#endif
#ifdef __INTERNE_INGRID
    if(!strcmp(".ing",strrchr(fichier.c_str(),'.')))
      format = INGRID;
#endif
#ifdef __INTERNE_ICEMCFD
    if(!strcmp(".ice",strrchr(fichier.c_str(),'.')) ||
	   !strcmp(".uns",strrchr(fichier.c_str(),'.')))
      format = ICEMCFD;
#endif
  }

  return format; 
}

//////////////////////////////////////////////////////////////////////////////
/*!
  C'est ici qu'il faut ajouter les nouveaux reader
*/
_Reader* _Reader::create(_MaillageInterne* mesh, 
                 const string& nom_fichier, 
                 format_t format,
                 size_type num) {

  _Reader* reader = NULL;
  
  /* Detection du format par le suffixe du nom du fichier. */
  if (format==SUFFIXE)
    format = _Reader::detectFormat(nom_fichier);

  /* Creation du reader correspondant */
  try{
    switch(format){
#ifdef __INTERNE_MALI
    case MALI:
      reader = new _ReaderMali(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_MALIPP
    case MALIPP:
    case MALIPP_Z:
      reader = new _ReaderMalipp(mesh, nom_fichier, format, num);
      break;
#endif
#ifdef __INTERNE_MALIPP2
    case MALIPP2:
      reader = createMalipp2Reader (mesh, nom_fichier, format, num);
      break;
#endif
#ifdef __INTERNE_IDEAS
    case IDEAS:
      reader = new _ReaderIdeas(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_ABAQUS
    case ABAQUS:
      reader = new _ReaderAbaqus(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_GIBI
    case GIBI:
      reader = new _ReaderK2KGibi(mesh, nom_fichier, format);
      break;
    case CASTEM:
      reader = new _ReaderK2KCastem(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_MODULEF
    case MODULEF:
      reader = new _ReaderModulef(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_MAZE
    case MAZE:
      reader = new _ReaderMaze(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_INGRID
    case INGRID:
      reader = new _ReaderIngrid(mesh, nom_fichier, format);
      break;
#endif
#ifdef __INTERNE_ICEMCFD
    case ICEMCFD:
      reader = new _ReaderIcemcfd(mesh, nom_fichier, format);
      break;
#endif
     /* Format inconnue */
    case SUFFIXE:
      /* La détection automatique a échoué */
      throw argument_erreur("Extension de fichier inconnu\n");
    default:
      throw argument_erreur("Type de format inconnu\n");
    }
  }

  catch(erreur& err){
    throw read_erreur("Erreur initialisation du module de lecture"
                      " pour le fichier :\n" + nom_fichier + "\n" + err.what());
  }

  return reader;
}


_Reader::LectureAnnulee::LectureAnnulee ( )
{
}	// LectureAnnulee::LectureAnnulee


_Reader::LectureAnnulee::LectureAnnulee (const LectureAnnulee&)
{
}	// LectureAnnulee::LectureAnnulee


_Reader::LectureAnnulee& _Reader::LectureAnnulee::operator = (
														const LectureAnnulee&)
{
	return *this;
}	// LectureAnnulee::operator =


_Reader::LectureAnnulee::~LectureAnnulee ( )
{
}	// LectureAnnulee::~LectureAnnulee

END_NAMESPACE_LIMA
