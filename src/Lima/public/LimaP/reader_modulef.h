#ifdef __INTERNE_MODULEF
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_modulef.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_modulef.h
  \brief Interface de lecture / ecriture pour les fichiers modulef
*/

#ifndef _READER_MODULEF_H
#define _READER_MODULEF_H


#if !defined(INT_4)
#define INT_4 int
#endif     

#if !defined(int_4)
#define int_4 int
#endif     

#if !defined(FLOAT_4)
#define FLOAT_4 float
#endif     

// pour la convertion big endian <-> little endian
//#if defined(__alpha) || defined(__ultrix) || defined(__i386) || defined(__ia64__)
#if defined(__alpha) || defined(__ultrix) || defined(__i386) || defined(__ia64__) || defined(__x86_64__) || defined (__arm__) || defined (__aarch64__)
#define CONVERT_ENDIAN 1
#else 
#define CONVERT_ENDIAN 0
#endif

static const int TailleBuf = 128;


#include "reader.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA

class _ReaderModulef : public _Reader {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderModulef(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderModulef();

public:



  //
  // voir la documentation Modulef pour les details sur la structure du fichier
  //
  // en gros, une SD NOPO (Structure de Donnees NOeud POint) contient
  // 6 tableaux (ou plutot enregistrements) NOP0, NOP1, ... NOP5
  // la structure des tableaux est definie ici dans les typedef SD_NOPX_t
  // les coordonnees des points sont dans NOP4
  // la connectivite dans NOP5
  //

//! tableau 0 : information generale
  typedef struct SD_NOP0_t {
	SD_NOP0_t ( );	// CP, v 6.27.1, coverity
    char TITRE[80];
    char DATE[8];
    char NOMCRE[24];
    char TYPE[4];
    INT_4 NIVEAU;
    INT_4 ETAT;
    INT_4 NTACM;
  } SD_NOPO_t;


  //! descripteur des eventuels tableaux supplementaires
  typedef struct SD_B1_t {
	SD_B1_t ( );	// CP, v 6.27.1, coverity
    char NAME[4];
    INT_4 ADRESS;
    INT_4 NB_WORD;
    INT_4 TYPE; // 1 entier, 2 float, 4 char, 5 double...
    char COMMENT[72];
  } SD_B1_t;

  //! descripteur des eventuels tableaux supplementaires
  typedef struct SD_NOP1_t {
	SD_NOP1_t ( );	// CP, v 6.27.1, coverity
    INT_4 nbr;
    INT_4 max;
    SD_B1_t *elm;
  } SD_NOP1_t;

  //! description generale du maillage
  enum {
    NDIM = 0,
    NDSR, NDSD,
    NCOPNP,
    NE, 
    NEPO, NSEG,NTRI, NQUA, NTET, NPEN, NHEX,
    NSUP, NEF, NOE, N1,ISET, ISEQ, ISETE, ISEPE, ISEHE,
    NP,  // nombre de points
    NTYCOO, // type : 1 entier, 2 float, 4 char, 5 double...
    LPGDN, NBEGM, LNOP5,
    NTACOO // syteme d'axe : 1 x,y,z.  2 r, teta, z.  3 r, teta, phi
  };
  typedef struct SD_NOP2_t {
	SD_NOP2_t ( );	// CP, v 6.27.1, coverity
    INT_4 elm[27];
  } SD_NOP2_t;

  //! pointeur eventuel
  typedef struct SD_NOP3_t {
	SD_NOP3_t ( );	// CP, v 6.27.1, coverity
    INT_4* elm;
  } SD_NOP3_t;

  //! tableau des coordonnees
  typedef struct SD_NOP4_t {
	SD_NOP4_t ( );	// CP, v 6.27.1, coverity
    FLOAT_4* coo;
  } SD_NOP4_t;

  //! description sequentielle des elements
  typedef struct SD_NOP5_t {
	SD_NOP5_t ( );	// CP, v 6.27.1, coverity
    INT_4* elm;
  } SD_NOP5_t;


  //! structure de donnees SD NOPO
  typedef struct SD_NOP_t {
	SD_NOP_t ( );	// CP, v 6.27.1, coverity
    INT_4 header[13];
    SD_NOPO_t NOP0;
    SD_NOP1_t NOP1;
    SD_NOP2_t NOP2;
    SD_NOP3_t NOP3;
    SD_NOP4_t NOP4;
    SD_NOP5_t NOP5;
  } SD_NOP_t;

  //! description d'un enregistrement 
  typedef struct enr_size_t {
	enr_size_t ( );	// CP, v 6.27.1, coverity
    INT_4 reserved; // enregitrement reserve. cela provient de l'ecriture en fortran. Attention pas compatible avec le binaire Cray 
    INT_4 nbo; // taille en d'octets
    INT_4 nbi; // taille en nombre d'entiers I4 (4 octets)
  }enr_size_t;


#ifdef CONVERT_ENDIAN
  /*!
    \brief Convertie un tableau d'entier (ou de float) (sur 4 octets) 
    bigendian <-> littleendian
  */
  static void swap_endian(INT_4* tab, INT_4 nbr);
#endif //CONVERT_ENDIAN

private:


  /*!
    \brief comme "fread" avec exception et affichage du message
    \exception read_erreur "Flot de données interrompu\n"
    \exception eof_erreur "Fin de fichier rencontrée prématurement\n"
  */
  size_t fread2(void *ptr, size_t size, size_t nitems, File *fd, IN_STD string msg);


  /*!
    \brief interface decriture d'un tableau d'entiers en fortran
    \exception write_erreur "Flot de données interrompu\n"
  */
  void fwriteInt4(INT_4 *ptr, INT_4 nitems, File* fd);

  void lire();
  
  void addElements();



  //! SD = Structure de Donnees
  SD_NOP_t SD;

  //! tailles des tableaux
  size_type coo_nbr;
  size_type elm_nbr;


};


//////////////////////////////////////////////////////////////////////////////
class _WriterModulef : public _Writer {
public :
  _WriterModulef(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE);

  virtual ~_WriterModulef();
  void write(format_t format);
private:
  int ulf; // unite logique fortran. correspondant au fichier fd->fp
  int err; // erreur fortran
  void ecrire(File* fd);
  void ecrire_ligne(FILE* fp, const char* buf);
  void fwriteInt4(INT_4 *ptr, INT_4 nitems, File* fd);
#ifdef CONVERT_ENDIAN
  void swap_endian(INT_4* tab, INT_4 nbr);
#endif
};


END_NAMESPACE_LIMA

#endif // _READER_MODULEF_H
#endif
