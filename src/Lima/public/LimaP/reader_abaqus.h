#ifdef __INTERNE_ABAQUS
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_abaqus.h
 Cree le : 
 Auteur  : S. Chemin
******************************************************************************/

/*!
  \file reader_abaqus.h
  \brief Interface de lecture / ecriture pour les fichiers abaqus
*/

#ifndef _READER_ABAQUS_H
#define _READER_ABAQUS_H

//#include<map>

#include "reader.h"
#include "writer.h"


BEGIN_NAMESPACE_LIMA


class _ReaderAbaqus : public _Reader {

public:

  virtual void readStructure();
 
  virtual void readAll();

  _ReaderAbaqus(_MaillageInterne* mesh, const IN_STD string& nom_fichier, format_t format=SUFFIXE);
  
  virtual ~_ReaderAbaqus();

private:

  enum {ABAQUS_NODE=1,ABAQUS_ELEMENT,ABAQUS_NSET,ABAQUS_ELSET,ABAQUS_SURFACE,ABAQUS_INSTANCE, // Reconnus
        ABAQUS_PART, ABAQUS_SYSTEM,                                                           // Reconnus
        ABAQUS_NGEN,ABAQUS_NCOPY,ABAQUS_ELGEN,ABAQUS_ELCOPY,ABAQUS_NFILL,ABAQUS_ORIENTATION,  // Interdits (non spécifiés)
        ABAQUS_INCLUDE,ABAQUS_PARAMETERS,ABAQUS_PARAMETER,ABAQUS_IMPORT,                                                    
        ABAQUS_END};
  
  //! Correspondances faces Abaqus <-> faces Lima
  static size_type lima_faces[6][6];
                              
  //! Mots clefs Abaqus
  static IN_STD pair<IN_STD string,int> keyword_pairs[];  
       
  //! Types Abaqus
  static IN_STD pair<IN_STD string,int> type_pairs[];             
  
  //! Map des mots clefs Abaqus
  static IN_STD map<IN_STD string,int> abaqus_keywords;   
       
  //! Map des types d'éléments Abaqus
  static IN_STD map<IN_STD string,int> types_elements;           
  
  typedef IN_STD multimap<IN_STD string,int>::iterator mmap_it_int;
  typedef IN_STD multimap<IN_STD string,IN_STD string>::iterator mmap_it_str;


  //! Paramètres de translation/rotations des instances
   IN_STD vector<double> trans_rot;                               
   
   //! Associe le nom Lima d'un groupe avec son type
   IN_STD map<IN_STD string,int> types_groupes;                 
   
   //! Associe le nom Abaqus et le nom Lima d'un groupe
   IN_STD multimap<IN_STD string,IN_STD string> groupe_abaqus;  

   //! Numérotation globale de chaque instance (décalage en nombre de noeuds et d'éléments)
   IN_STD multimap<IN_STD string,int> instance_abaqus;           
                                                               

   //! Type Lima de chaque élément (BRAS,POLYGONE,POLYEDRE)
   IN_STD map<int,int> type_element_lima;                             

   //! Etat de chaque PART : - true si la PART contient le maillage - false si la PART est vide
   IN_STD map<IN_STD string, bool> info_part;                    
		                                                                     

   //! Associe la PART et sa position dans le fichier
   IN_STD map<IN_STD string,IN_STD ifstream::pos_type>  position_part; 

   // dimension du maillage
   size_t maillage_dim;

   // vrai si la dimension du maillage a été lue
   bool dim_lu;

   //! Nombre de noeuds dans une instance donnée
   int nbnodes_inst;

   //! Nombre d'éléments dans une instance donnée                                         
   int nbelems_inst;

   //! Nombre total de noeuds dans le maillage                                         
   int nbnodes_glob;

   //! Nombre total d'éléments dans le maillage                                         
   int nbelems_glob;                                            

   //! Numéro de la ligne en cpours de lecture
   size_type n_ligne;                                            
   
   //! Matrice de changement de repère (bloc SYSTEM)
   double matrice[3][3]; 
                                       
   //! Coordonnées des noeuds dans le nouveau repère 
   IN_STD vector<double> new_system;                            
   
   //! Position courante d'une INSTANCE dans le fichier
   IN_STD ifstream::pos_type pos_instance;   
                          
   //! Position à l'ouverture du fichier
   IN_STD ifstream::pos_type pos_init;                                

  //! Methodes de lecture
   void lire();
   bool lire_ligne(IN_STD ifstream&,IN_STD vector<IN_STD string>&);  
   void lire_noeuds(IN_STD ifstream&,IN_STD vector<IN_STD string>&,IN_STD string nom_instance_appelante ="");
   int lire_elements(IN_STD ifstream&,IN_STD vector<IN_STD string>&,IN_STD string nom_instance_appelante ="");
   void lire_groupe(IN_STD ifstream&,IN_STD vector<IN_STD string>&,IN_STD string nom_instance_appelante ="");
   void lire_surface(IN_STD ifstream&,IN_STD vector<IN_STD string>&,IN_STD string nom_instance_appelante ="", int type = 3/*polyèdre*/);
   void lire_instance(IN_STD ifstream&,IN_STD vector<IN_STD string>&);
   void lire_point(IN_STD ifstream&,IN_STD vector<double>&); 
   void lire_part(IN_STD ifstream&,IN_STD vector<IN_STD string>&);
   void lire_system(IN_STD ifstream& fp);
   void lire_bloc_mots_clefs(IN_STD ifstream&,IN_STD string nom_instance_appelante =""); 

  //! Operation sur les groupes
   void ajouter_groupe(const int&,IN_STD string&,_MaillageInterne*&);
   void ajouter_element(int,const int&,IN_STD string&,_MaillageInterne*&);
   void extraire_peau(long,
                      const int&,
                      IN_STD string& ,
                      IN_STD string& ,
                      _MaillageInterne*& );
	void extraire_face (
				long, int, const IN_STD string&, int, _MaillageInterne*&);

   bool contient_groupe(_MaillageInterne*,IN_STD string,int);

   
   //! Utilitaires
   int getElemType(int,int);
   bool num_string(IN_STD string&);
   bool empty_line(IN_STD string&);
   void split_string(IN_STD string&,const IN_STD string&,IN_STD vector<IN_STD string>&);
   void transform_coordinates(double& x,double& y, double& z,IN_STD vector<double>& vec);
   void system_coordinates(double& x,double& y, double& z);
   void merge_groupe(const IN_STD string& nom_groupe_in,const int& type,const IN_STD string& nom_groupe);
   int check_bloc_end(IN_STD ifstream&);
   void read_lines(IN_STD ifstream&, IN_STD string&);  
   void convert_string_to_long(IN_STD string,const int, long&); 
};

END_NAMESPACE_LIMA

#endif // _READER_ABAQUS_H
#endif
