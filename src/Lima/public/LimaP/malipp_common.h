#if defined(__INTERNE_MALIPP) || defined(__INTERNE_MALIPP2)
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : malipp_common.h
 Cree le : 06/06/2019
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/

/*!
  \file malipp_common.h
  \brief Interface de lecture / ecriture pour les fichiers MALI++ (.mli/.mliz)
*/

#ifndef _MALIPP_COMMON_H
#define _MALIPP_COMMON_H

#include <Lima/config.h>

#include <string>


BEGIN_NAMESPACE_LIMA

// ! Retourne le nom de l'utilisateur si celui-ci a pu être déterminé.
IN_STD string getUserName ( );

// ! Le nom du groupe "header" du fichier.
extern const IN_STD string	FILE_HEADER_GROUP_NAME;

// ! Le nom du champ "title".
extern const IN_STD string	TITLE_FIELD_NAME;

// ! La version du format de fichier.
extern const IN_STD string	FILE_VERSION;
extern const IN_STD string	MLI2_FILE_VERSION;

// ! Le nom de l'attribut "file_version".
extern const IN_STD string	FILE_VERSION_FIELD_NAME;

// ! La taille maximale de l'attribut "file_version".
extern const int	FILE_VERSION_MAX_LENGTH;

// ! La version de lima++.
extern const IN_STD string	LIMA_VERSION;

// ! Le nom de l'attribut "lima_version".
extern const IN_STD string	LIMA_VERSION_FIELD_NAME;

// ! La taille maximale de l'attribut "lima_version".
extern const int	LIMA_VERSION_MAX_LENGTH;

// ! Le nom du champ "author".
extern const IN_STD string	AUTHOR_FIELD_NAME;

// ! Le nom du champ "comment".
extern const IN_STD string	COMMENT_FIELD_NAME;

// ! Le nom du champ "nb_mesh".
extern const IN_STD string	NB_MESHES_FIELD_NAME;

// ! Le prefix d'un groupe racine de maillage.
extern const IN_STD string	MESH_GROUP_PREFIX;

// ! Le nom du groupe "header" du maillage.
extern const IN_STD string	MESH_HEADER_GROUP_NAME;

// ! Le nom du groupe "data_mesh".
extern const IN_STD string	DATA_MESH_GROUP_NAME;

// ! Le nom du groupe "node".
extern const IN_STD string	NODE_GROUP_NAME;

// ! Le nom du groupe "data_node".
extern const IN_STD string	DATA_NODE_GROUP_NAME;

// ! Le nom du groupe "edge".
extern const IN_STD string	EDGE_GROUP_NAME;

// ! Le nom du groupe "data_edge".
extern const IN_STD string	DATA_EDGE_GROUP_NAME;

// ! Le nom du groupe "cell2D".
extern const IN_STD string	CELL_2D_GROUP_NAME;

// ! Le nom du groupe "data_cell2D".
extern const IN_STD string	DATA_CELL_2D_GROUP_NAME;

// ! Le nom du groupe "cell3D".
extern const IN_STD string	CELL_3D_GROUP_NAME;

// ! Le nom du groupe "data_cell3D".
extern const IN_STD string	DATA_CELL_3D_GROUP_NAME;

// ! Le nom du groupe "nodeset".
extern const IN_STD string	NODESET_GROUP_NAME;

// ! Le nom du groupe "data_nodeset".
extern const IN_STD string	DATA_NODESET_GROUP_NAME;

// ! Le nom du groupe "line".
extern const IN_STD string	LINE_GROUP_NAME;

// ! Le nom du groupe "data_line".
extern const IN_STD string	DATA_LINE_GROUP_NAME;

// ! Le nom du groupe "surface".
extern const IN_STD string	SURFACE_GROUP_NAME;

// ! Le nom du groupe "data_surface".
extern const IN_STD string	DATA_SURFACE_GROUP_NAME;

// ! Le nom du groupe "volume".
extern const IN_STD string	VOLUME_GROUP_NAME;

// ! Le nom du groupe "data_volume".
extern const IN_STD string	DATA_VOLUME_GROUP_NAME;

// ! Le prefix des sous-groupe.
extern const IN_STD string	SUBGROUP_PREFIX;

// ! Le nom de l'attribut "title" du maillage.
extern const IN_STD string	MESH_TITLE_FIELD_NAME;

// ! Le nom de l'attribut "date" du maillage.
extern const IN_STD string	MESH_DATE_FIELD_NAME;

// ! Le nom de l'attribut "dimension" du maillage.
extern const IN_STD string	MESH_DIMENSION_FIELD_NAME;

// ! Le nom de l'attribut "geometry" du maillage.
extern const IN_STD string	MESH_GEOMETRY_FIELD_NAME;

// ! Le nom de l'attribut "coordinate_system" du maillage.
extern const IN_STD string	MESH_COORD_SYSTEM_FIELD_NAME;

// ! Le nom de l'attribut "angle_unit" du maillage.
extern const IN_STD string	MESH_ANGLE_UNIT_FIELD_NAME;

// ! Le nom de l'attribut "lenght_unit" du maillage.
extern const IN_STD string	MESH_LENGTH_UNIT_FIELD_NAME;

// ! Le nombre d'éléments d'un ensemble (dataset, groupe ou autre).
extern const IN_STD string	SIZE_ATTR_NAME;

// ! Le nom d'un élément (nuage, ligne, surface, ...).
extern const IN_STD string	NAME_ATTR_NAME;

// ! Le nom du dataset "coordinate_x" d'un maillage.
extern const IN_STD string	DATASET_COORD_X_NAME;

// ! Le nom du dataset "coordinate_y" d'un maillage.
extern const IN_STD string	DATASET_COORD_Y_NAME;

// ! Le nom du dataset "coordinate_z" d'un maillage.
extern const IN_STD string	DATASET_COORD_Z_NAME;

// ! Le nom des datasets contenant les identifiants d'un groupe :
extern const IN_STD string	DATASET_IDS_NAME;

// ! Le nom des datasets contenant une liste d'identifiants :
extern const IN_STD string	DATASET_IDS_LIST_NAME;

// ! Le caractère contigue des identifiants d'un groupe :
extern const IN_STD string	CONTIGUOUS_IDS_ATTR_NAME;

// ! Le nom des datasets contenant les types d'objets d'un groupe :
extern const IN_STD string	DATASET_TYPES_NAME;

// ! Le nom des datasets contenant la liste des attributs (Lima) d'un groupe :
extern const IN_STD string	DATASET_ATTR_LIST_NAME;

// ! La composition en noeuds d'un groupe.
extern const IN_STD string	NODE_COMPOSITION_DATASET_NAME;

// ! La composition en bras d'un groupe.
extern const IN_STD string	EDGE_COMPOSITION_DATASET_NAME;

// ! La composition en polygones d'un groupe.
extern const IN_STD string	CELL2D_COMPOSITION_DATASET_NAME;

// ! La composition en polyèdres d'un groupe.
extern const IN_STD string	CELL3D_COMPOSITION_DATASET_NAME;

// ! Le nom de l'attribut indiquant si une composition est détaillée ou non.
extern const IN_STD string	DETAILED_ATTR_NAME;

//! L'attribut contenant le détail de la composition d'un groupe de données.
extern const IN_STD string	COMPOSITION_ATTR_NAME;


END_NAMESPACE_LIMA

#endif	// _MALIPP_COMMON_H
#endif	// #if defined(__INTERNE_MALIPP) || defined(__INTERNE_MALIPP2)
