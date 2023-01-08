#ifdef __INTERNE_MALI
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_mali.h
 Cree le : Wed Dec 20 14:35:58 MET 2000
 Auteur  : JM Bellard
******************************************************************************/

/*!
  \file reader_mali.h
  \brief Interface de lecture / ecriture pour les fichiers mali
*/

#ifndef _READER_MALI_H
#define _READER_MALI_H

#include "reader.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA

class _ReaderMali : public _Reader {

public:
  virtual void readStructure();

  virtual void readAll();


  _ReaderMali(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderMali();

private:

  //! Lecture d'un fichier mali.
  void          lire ( );

  enum Typ { NOE = 0, BRA, PGN, PDR, GRO, QTYP};
    
  struct forma_t
  {
    int nb_int, nc_int;
    int nb_float, nc_float;
    int nb_string, nc_string;
  };

  void remplace_D_par_E(char* str);
  char* passer_blanc(char* buf);
  void lire_ligne(FILE* fp, char* buf, int taille);
  const char* debut_bloc(FILE* fp, char* buf);
  const char* debut_attribut(FILE* fp, char* buf);

  void          lire_bloc_maillage(FILE* fp, char* buf);
  void          lire_attr_longueur(FILE* fp, char* buf);
  void          lire_attr_date(FILE* fp, char* buf);
  void          lire_attr_angle(FILE* fp, char* buf);
  void          lire_attr_coordonnees(FILE* fp, char* buf);
  void          lire_attr_titre(FILE* fp, char* buf);
  void          lire_attr_geometrie(FILE* fp, char* buf);
    
  void          lire_bloc_etat(FILE* fp, char* buf, size_t eta[][QTYP]);
    
  void          lire_bloc_format(FILE* fp, char* buf, _ReaderMali::forma_t* format);
    
  void          lire_bloc_coordonnees(FILE* fp, char* buf, 
				      const size_t eta[][QTYP],
				      const _ReaderMali::forma_t* format);
    
  void          lire_bloc_tables(FILE* fp, char* buf, 
				 const size_t eta[][QTYP],
				 const _ReaderMali::forma_t* format);
  void          lire_attr_table(FILE* fp, char* buf, 
				const size_t eta[][QTYP],
				const forma_t* format);
  void          lire_attr_nom(FILE* fp, char* buf, 
			      const size_t eta[][QTYP],
			      const forma_t* format);

};


//////////////////////////////////////////////////////////////////////////////
class _WriterMali : public _Writer {
public :
  _WriterMali(_MaillageInterne* mesh, 
               const IN_STD string& nom_fichier, 
               format_t format=SUFFIXE);

  virtual ~_WriterMali();
  void write(format_t format=SUFFIXE);
private:
  enum Typ { NOE = 0, BRA, PGN, PDR, GRO, QTYP};
  struct forma_t
  {
    int nb_int, nc_int;
    int nb_float, nc_float;
    int nb_string, nc_string;
  };
  void ecrire(File* fd);
  void ecrire_ligne(FILE* fp, const char* buf);

};


END_NAMESPACE_LIMA

#endif // _READER_MALI_H
#endif

