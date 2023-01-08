/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : writer.C
 Cree le : Fri Nov 10 16:37:43 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!   
  \file writer.C
  \brief Interface pour les modules d'ecriture
*/

#include "LimaP/reader.h"
#include "LimaP/writer.h"


#ifdef __INTERNE_IDEAS
#include "LimaP/reader_ideas.h"
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
#ifdef __INTERNE_MODULEF
#include "LimaP/reader_modulef.h"
#endif

#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA

const char* _liste_format_ecriture[]={
#ifdef __INTERNE_MALI
  (char*) "unf, format de fichier MALI",
#endif
#ifdef __INTERNE_MALIPP
  (char*) "mli, format (désuet) de fichier MALI++",
  (char*) "mliz, format (désuet) de fichier MALI++",
#endif
#ifdef __INTERNE_MALIPP2
  (char*) "mli2, format de fichier MALI++",
#endif 
#ifdef __INTERNE_IDEAS
  (char*) "unv, format de fichier IDEAS",
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
  NULL
};

const char** liste_format_ecriture()
{
  return _liste_format_ecriture;
};

//////////////////////////////////////////////////////////////////////////////
_Writer::_Writer(_MaillageInterne* mesh, 
                 const string& nom_fichier, 
                 format_t format) : m_file(nom_fichier, format) {
  m_mai = mesh;
  // Modif Olivier, on ouvre uniquement dans le writer_xxx
  // (ce open fait doublon)
  //  m_file.open("wt");

}
//////////////////////////////////////////////////////////////////////////////
_Writer::~_Writer(){
  // Modif Olivier, idem ci-dessus
  //  m_file.close();
}

//////////////////////////////////////////////////////////////////////////////
/*!
  C'est ici qu'il faut ajouter les nouveaux writer
*/
_Writer* _Writer::create(_MaillageInterne* mesh, 
                         const string& nom_fichier, 
                         format_t format,
                         size_type num) {

  
  /* Detection du format par le suffixe du nom du fichier. */
  if (format==SUFFIXE)
    format = _Reader::detectFormat(nom_fichier);
  if (format==SUFFIXE)
    throw argument_erreur("Extension de fichier inconnu\n");

  _Writer* writer = NULL;

  switch(format){
#ifdef __INTERNE_MALI
  case MALI:
    writer = new _WriterMali(mesh, nom_fichier, format);
    break;
#endif
#ifdef __INTERNE_MALIPP
  case MALIPP:
    writer = new _WriterMalipp(mesh, nom_fichier, format, num, false);
    break;
  case MALIPP_Z:
    writer = new _WriterMalipp(mesh, nom_fichier, format, num, true);
    break;
#endif
#ifdef __INTERNE_MALIPP2
  case MALIPP2:
    writer = createMalipp2Writer (mesh, nom_fichier, format, num);
    break;
#endif
#ifdef __INTERNE_IDEAS
  case IDEAS:
    //((_MaillageIdeas*)(this))->ecrire(&m_file);
    writer = new _WriterIdeas(mesh, nom_fichier, format);
    break;
#endif
#ifdef __INTERNE_GIBI
  case GIBI:
    //((_MaillageGibi*)(this))->ecrire(&m_file, GIBI);
    writer = new _WriterK2KGibi(mesh, nom_fichier, format);
    break;
  case GIBI_I8:
    //((_MaillageGibi*)(this))->ecrire(&m_file, GIBI_I8);
    writer = new _WriterK2KGibi(mesh, nom_fichier, format);
    break;
  case CASTEM:
    //((_MaillageCastem*)(this))->ecrire(&m_file);
    writer = new _WriterK2KCastem(mesh, nom_fichier, format);
    break;
#endif
#ifdef __INTERNE_MODULEF
  case MODULEF:
    //((_MaillageModulef*)(this))->ecrire(&m_file);
    writer = new _WriterModulef(mesh, nom_fichier, format);
    break;
#endif
#ifdef __INTERNE_MAZE
  case MAZE:
    throw argument_erreur("Ecriture au format MAZE non disponible\n");
#endif
#ifdef __INTERNE_INGRID
  case INGRID:
    throw argument_erreur("Ecriture au format INGRID non disponible\n");
#endif
#ifdef __INTERNE_ICEMCFD
  case ICEMCFD:
    throw argument_erreur("Ecriture au format ICEMCFD non disponible\n");
#endif
  default:
    throw argument_erreur("Type de format inconnu\n");
  }

  return writer;
}

END_NAMESPACE_LIMA
