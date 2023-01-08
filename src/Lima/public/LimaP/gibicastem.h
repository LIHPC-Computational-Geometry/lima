#ifdef __INTERNE_GIBI

#ifndef _GIBICASTEM_H
#define _GIBICASTEM_H


#include <string.h>


BEGIN_NAMESPACE_LIMA




///////////////////////////////////////////////////////////////////////////////
// noms dans GIBI et CASTEM 2000
static const int k2k_name_nbc = 8;

typedef struct k2k_name_t{
	k2k_name_t ( )	// CP, v 6.27.1, coverity
		: obj (0)
	{
		memset (name, '\0', (k2k_name_nbc+1) * sizeof (char));
	}
  char name[k2k_name_nbc + 1];   // 8 caracteres utiles + \0
  size_type obj;  
} k2k_name_t;

typedef struct k2k_lname_t {
	k2k_lname_t ( )	// CP, v 6.27.1, coverity
		: nbr (0), max (0), elm (0)
	{ }
  size_type nbr;
  size_type max;
  k2k_name_t* elm;
} k2k_lname_t;


///////////////////////////////////////////////////////////////////////////////
// noeud
typedef struct k2k_node_t {
	k2k_node_t ( )	// CP, v 6.27.1, coverity
		: id (0), name (0), coo ( )
	{ }
  size_type id;
  char *name;   //!< nom eventuel de l'objet. 
  // jamais alloue : soit NULL soit pointe vers une chaine

  // attention un noeud (idem pour les objets) peut etre nomme plusieurs fois
  // on ne peut donc pas avoir de pointeur sur un nom dans k2k_node_t (idem
  // k2k_object_t), (il en faudrait pls...)



  union {
    double d[3];
    struct {
      double x;
      double y;
      double z;
    } n;
  } coo;      
} k2k_node_t;

typedef struct k2k_lnode_t {
	k2k_lnode_t ( )	// CP, v 6.27.1, coverity
		: nbr (0), max (0), elm (0)
	{ }
  size_type nbr;
  size_type max;
  k2k_node_t* elm;
} k2k_lnode_t;


///////////////////////////////////////////////////////////////////////////////

typedef struct k2k_object_t {
	k2k_object_t ( )	// CP, v 6.27.1, coverity
		: name (0), type (0), nbe (0), elem (0), nbn (0), node (0)
	{ }
  char *name;    /*!< nom eventuel de l'objet. 
                   jamais alloue : soit NULL soit pointe vers une chaine */
  size_type  type;  /*!< type des elements du groupe */
  size_type  nbe;   /*!< nombre d'elements dans le groupe */ 
  size_type* elem;  /*!< liste des elements [1..] numerotation lima */
  size_type  nbn;   /*!< nombre de noeuds (=nbe * nb_noeud_par_elm) */
  size_type* node;  /*!< liste des noeuds [1..]  numerotation gibi */
} k2k_object_t;

typedef struct k2k_lobject_t {
	k2k_lobject_t ( )	// CP, v 6.27.1, coverity
		: nbr (0), max (0), elm (0)
	{ }
  size_type nbr;
  size_type max;
  k2k_object_t* elm;
} k2k_lobject_t;

///////////////////////////////////////////////////////////////////////////////
typedef struct k2k_data_t{
	k2k_data_t ( )	// CP, v 6.27.1, coverity
  		: dim (0), level (0), lnode ( ), nodename ( ), lobject ( ), objname ( )
	{
		memset (title, '\0', (128+1) * sizeof (char));
		memset (date, '\0', (10) * sizeof (char));
	}
  size_type dim;
  char title[128 + 1];
  char date[10];
  size_type level;
  

  k2k_lnode_t lnode;
  k2k_lname_t nodename;

  k2k_lobject_t lobject;
  k2k_lname_t objname;

} k2k_data_t;


///////////////////////////////////////////////////////////////////////////////


END_NAMESPACE_LIMA


#endif /* _GIBICASTEM_H */
#endif
