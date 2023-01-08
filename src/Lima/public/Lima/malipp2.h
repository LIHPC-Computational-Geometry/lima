#if defined(__INTERNE_MALIPP2)

#ifndef _MALIPP2_H
#define _MALIPP2_H

#ifdef _MALIPP_H
#	error Les fichiers malipp.h et malipp2.h ne peuvent pas etre inclus dans le meme fichier.
#endif	/* _MALIPP_H */

#include "config.h"
#include "size.h"
#include "enum.h"
#include "attribut.h"
#include "version.h"
#include "composition.h"
#include "polyedre.h"

#include <memory>
#include <vector>
#include <map>

//#include <H5Ipublic.h>	// => hid_t not provided by H5Cpp 1.10.5 ...
#include <H5Cpp.h>


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif



#ifndef MLI_TYPES
#define MLI_TYPES
/** Le type numérique utilisé pour sauvegarder les listes d'indices dans
 * les fichiers. Au niveau HDF, il est utilisé avec le type NATIVE_UINT32.
 * Cela correspond à 2^32 indices possibles, ce qui correspond à un peu
 * plus de 4 milliards d'indices. 
*/
typedef u_int_4 id_type;

/** Le type numérique utilisé pour sauvegarder des types (type de
  * polygone ou polyèdre, ...). Au niveau HDF, il est utilisé avec le
  * type NATIVE_INT32. 
*/
typedef	int_4	object_type;
#endif	// MLI_TYPES

/** Taille max des noms d'attributs. */
const int MLI2_ATTRIBUT_SIZE   = 128;


/**
 * Structure décrivant les attributs.
 */
struct HDFAttributeDescriptor2
{
	/**
	 * Constructeur. Initialisations à 0.
	 */
	HDFAttributeDescriptor2 ( );

	/**
	 * Recopie de la structure reçue en argument.
	 */
	HDFAttributeDescriptor2 (const HDFAttributeDescriptor2&);
	HDFAttributeDescriptor2& operator = (const HDFAttributeDescriptor2&);

	/** Nom de l'attribut.  */
	char	m_name [MLI2_ATTRIBUT_SIZE];

	/** Type de l'attribut (cf. enuméré Attribut::TYPE). */
	int_2	m_type;

	/** Nombre de valeurs de l'attribut. */
	id_type	m_size;
};	// HDFAttributeDescriptor2


/** Classe de lecture de fichiers au format mali++ (extensions mli, mliz).
 * L'usage des diverses fonctions membres de cette classe requiert que le
 * constructeur n'ait levé aucune exception.
 * ATTENTION : contrairement à la classe MaliPPReader celle-ci n'est pas
 * thread safe.
 */
class MaliPPReader2
{
	public :

	/**
	 * Permet de désigner à quelle entité sont rattachés les
	 * attributs.
	 */
	enum ENTITE { MAILLAGE, NOEUDS, BRAS, POLYGONES, POLYEDRES,
	              NUAGES, LIGNES, SURFACES, VOLUMES };

	/**
	 * Constructeur. Ouvre le fichier en lecture et charge les données
	 * générales du maillage.
	 * @param		Nom du fichier
	 * @param		Numéro du maillage dans le fichier
	 * @exception	Une exception est levée en cas d'erreur
	 */
	MaliPPReader2 (const IN_STD string& nom_fichier, size_type num);

	/**
	 * Destructeur : ferme le fichier.	
	 */
	~MaliPPReader2 ( );

	/**
	 * @return	La version de HDF5 utilisée par cette version de lima++.
	 */
	static Version lireVersionHDF ( );

	/**
	 * Lecture de la composition d'un groupe de données.
	 * @param	Groupe de données contenant la composition à lire
	 * @param	Nom du sous-groupe contenant la composition du groupe de 
	 * 			données (il faut format mali++ >= 1.1.0).
	 * @param	Version du format mali++ du maillage
	 * @param	En retour, la composition du groupe
	 */
	static void lire_composition (
						H5::Group& root, const IN_STD string& nom, 
						const Version& version, Composition& composition);

	/**
	 * Lit la liste des attributs (nom + type) du groupe transmis en argument.
	 * @param	Groupe de données contenant la liste des attributs.
	 * @param	En retour, la liste des attributs
	 */
	static void lire_liste_attributs (
						H5::Group& group, 
						IN_STD vector<HDFAttributeDescriptor2>& liste);

	/**
	 * Lit les valeurs entières de l'attribut dont le descripteur est
	 * transmis en argument.
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	static void lire_attr_entier (
						H5::Group& group, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, int_4*& tableau);

	/**
	 * Lit les valeurs réelles de l'attribut dont le descripteur est
	 * transmis en argument.
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	static void lire_attr_reel (
						H5::Group& group, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, double*& tableau);

	/**
	 * Lit les valeurs vecteur 2D de réel de l'attribut dont le descripteur 
	 * est transmis en argument.
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour 2*nombre) où ranger les valeurs 
	 *				lues
	 */
	static void lire_attr_vect_2d (
						H5::Group& group, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, double*& tableau);

	/**
	 * Lit les valeurs vecteur 3D de réel de l'attribut dont le descripteur 
	 * est transmis en argument.
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour 3*nombre) où ranger les valeurs 
	 *				lues
	 */
	static void lire_attr_vect_3d (
						H5::Group& group, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, double*& tableau);

	/**
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @return		La longueur des chaines de caractères de l'attribut.
	 */
	static size_type longueur_chaines (
						H5::Group& group, const HDFAttributeDescriptor2& desc);

	/**
	 * Lit les valeurs chaine de caractères de l'attribut dont le descripteur
	 * est transmis en argument.
	 * @param		groupe de données contenant l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		vecteur (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	static void lire_attr_chaine (
						H5::Group& group, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, 
						IN_STD vector<IN_STD string>& chaines);

	/**
	 * @return		La version de lima++ utilisée pour l'entête du fichier
	 */
	const Version& version_lima_fichier ( ) const
	{ return m_lima_version_fichier; }

	/**
	 * @return		La version du format mali++ utilisée pour 
	 *				l'entête du fichier
	 */
	const Version& version_malipp_fichier ( ) const
	{ return m_mli_version_fichier; }

	/**
	 * @return		La version de lima++ utilisée pour l'entête du 
	 *				maillage
	 */
	const Version& version_lima_maillage ( ) const
	{ return m_lima_version_maillage; }

	/**
	 * @return		La version du format mali++ utilisée pour 
	 *				l'entête du maillage
	 */
	const Version& version_malipp_maillage ( ) const
	{ return m_mli_version_maillage; }

	/**
	 * @return		Le nom du fichier d'où est issu le maillage.
	 * @see			numero_maillage
	 */	
	const IN_STD string& fichier ( ) const
	{ return m_nom_fichier; }

	/**
	 * @return		Le numéro du maillage dans le fichier.
	 * @see			fichier
	 */	
	size_type numero_maillage ( ) const
	{ return m_numero_maillage; }

	/**
	 * @return		Le titre du maillage
	 */	
	const IN_STD string& titre ( ) const
	{ return m_titre; }

	/**
	 * @return		La date du maillage
	 */	
	const IN_STD string& date ( ) const
	{ return m_date; }

	/**
	 * @return		La dimension du maillage
	 */
	dim_t dimension ( ) const
	{ return m_dimension; }

	/**
	 * @return		La géométrie du maillage
	 */
	geometrie_t geometrie ( ) const
	{ return m_geometrie; }

	/**
	 * @return		Le système de coordonnées du maillage
	 */
	coordonnee_t systeme_coordonnees ( ) const
	{ return m_systeme_coords; }

	/**
	 * @return		le oefficient multiplicatif à appliquer sur les angles
	 *				pour obtenir des radians. 
	 */
	double unite_angle ( ) const
	{ return m_unite_angle; }

	/**
	 * @return		le oefficient multiplicatif à appliquer sur les 
	 *				coordonnées pour obtenir des mètres. 
	 */
	double unite_longueur ( ) const
	{ return m_unite_longueur; }

	/**
	 * @return		la composition en noeuds du maillage
	 */
	Composition composition_noeuds ( ) const
	{ return m_composition_noeuds; }

	/**
	 * @return		la composition en bras du maillage
	 */
	Composition composition_bras ( ) const
	{ return m_composition_bras; }

	/**
	 * @return		la composition en polygônes du maillage
	 */
	Composition composition_polygones ( ) const
	{ return m_composition_polygones; }

	/**
	 * @return		la composition en polyèdres du maillage
	 */
	Composition composition_polyedres ( ) const
	{ return m_composition_polyedres; }

	/**
	 * Alloue un tampon pour la lecture des identifiants par passes de
	 * nb_elements. Ce tableau est à libérer par delete [].
	 * @param		nombre d'identifiants lus par passe
	 * @return		le tableau alloué
	 * @see			lire_noeuds_ids
	 * @see			lire_noeuds
	 */
	size_type* allouer_tampon_ids (size_type nb_elements);

	/**
	 * Alloue un tampon pour la lecture des coordonnées par passes de
	 * nb_elements. Cette fonction prend en compte la dimension du maillage.
	 * Ce tableau est à libérer par delete [].
	 * @param		nombre de coordonnées lues par passe
	 * @return		le tableau alloué
	 * @see			lire_noeuds
	 */
	double* allouer_tampon_coords (size_type nb_elements);

	/**
	 * Lit les noeuds et place les identifiants dans le premier tableau et
	 * les coordonnées dans le second.
	 * Les coordoonées ne contiennent que les abscisses en cas de
	 * maillage de dimension 1, des séries abscisses-ordonnées dans
	 * le cas d'un maillage de dimension 2, et des séries abscisses-
	 * ordonnées-altitudes dans un maillage de dimension 3.
	 * @param		indice de la première donnée à lire
	 * @param		nombre de données à lire
	 * @param		dimension du maillage
	 * @param		tableau (alloué) où ranger les identifiants lus
	 * @param		tableau (alloué) où ranger les coordonnées lues
	 * @return		nombre de données effectivement lues
	 * @see			allouer_tampon_ids
	 * @see			allouer_tampon_coords
	 */
	size_type lire_noeuds (size_type premier, size_type nombre,
	                       size_type*& ids, double*& coords);

	/**
	 * Alloue un tampon pour la lecture des bras par passes de
	 * nb_elements. Ce tableau est à libérer par delete [].
	 * @param		nombre de bras lus par passe
	 * @return		le tableau alloué
	 * @see			lire_bras
	 */
	size_type* allouer_tampon_bras (size_type nb_elements);

	/**
	 * Lit les bras et les place dans le tableau. La structure du
	 * tableau, en sortie, est un suite d'informations caractérisant
	 * chaque bras où les informations concernant un bras sont
	 * définies comme suit :
	 * identifiant id_noeud_0 id_noeud_1
	 * @param		indice du premier bras à lire
	 * @param		nombre de bras à lire
	 * @param		tableau (alloué) où ranger les bras lus
	 * @return		nombre de bras effectivement lus
	 * @see			allouer_tampon_bras
	 */
	size_type lire_bras (size_type premier, size_type nombre,
	                     size_type*& tableau);

	/**
	 * @return		Le nombre de noeuds contribuant a la description de tous les
	 *				polygônes du maillage. Ce nombre est égal à la 
	 *				somme du nombre de noeuds nécessaire à la description
	 *				de chaque polygône.
	 */
	size_type nb_polygones_noeuds ( );

	/**
	 * Alloue un tampon pour la lecture des polygônes par passes de
	 * nb_elements. Ce tableau est à libérer par delete [].
	 * @param		nombre de polygônes lus par passe
	 * @return		le tableau alloué
	 * @see			lire_polygones
	 */
	size_type* allouer_tampon_polygones (size_type nb_elements);

	/**
	 * Lit les polygônes et les place dans le tableau. La structure du
	 * tableau, en sortie, est un suite d'informations caractérisant
	 * chaque polygônes où les informations concernant un polygône
	 * sont définies comme suit :
	 * identifiant nombre_noeuds id_noeud_0 id_noeud_1 ... id_noeud_n
	 * @param		indice du premier polygône à lire
	 * @param		nombre de polygône à lire
	 * @param		tableau (alloué) où ranger les polygône lus
	 * @return		nombre de polygône effectivement lus
	 * @see			allouer_tampon_polygones
	 */
	size_type lire_polygones (size_type premier, size_type nombre,
	                          size_type*& tableau);

	/**
	 * @return		Le nombre de noeuds contribuant a la description de tous les
	 *				polyèders du maillage. Ce nombre est égal à la 
	 *				somme du nombre de noeuds nécessaire à la description
	 *				de chaque polyèdre.
	 */
	size_type nb_polyedres_noeuds ( );
	
	/**
	 * Alloue un tampon pour la lecture des polyèdres par passes de
	 * nb_elements. Ce tableau est à libérer par delete [].
	 * @param		nombre de polyèdres lus par passe
	 * @return		le tableau alloué
	 * @see			lire_polyedres
	 */
	size_type* allouer_tampon_polyedres (size_type nb_elements);

	/**
	 * Lit les polyèdres et les place dans le tableau. La structure du
	 * tableau, en sortie, est un suite d'informations caractérisant
	 * chaque polyèdres où les informations concernant un polyèdre
	 * sont définies comme suit :
	 * identifiant nombre_noeuds id_noeud_0 id_noeud_1 ... id_noeud_n
	 * @param		indice du premier polyèdres à lire
	 * @param		nombre de polyèdres à lire
	 * @param		tableau (alloué) où ranger les polyèdres lus
	 * @return		nombre de polyèdres effectivement lus
	 * @see			allouer_tampon
	 */
	size_type lire_polyedres (size_type premier, size_type nombre,
	                          size_type*& tableau);

	/**
	 * @param		en retour, la liste des noms des nuages du maillage
	 */
	void liste_nuages (IN_STD vector<IN_STD string>& liste);

	/**
	 * @param		en retour, la liste des noms des lignes du maillage
	 */
	void liste_lignes (IN_STD vector<IN_STD string>& liste);

	/**
	 * @param		en retour, la liste des noms des surfaces du maillage
	 */
	void liste_surfaces (IN_STD vector<IN_STD string>& liste);

	/**
	 * @param		en retour, la liste des noms des volumes du maillage
	 */
	void liste_volumes (IN_STD vector<IN_STD string>& liste);

	/**
	 * Lit la liste des attributs (nom + type).
	 * @param	Entité de rattachement
	 * @param	En retour, la liste des attributs
	 */
	void lire_liste_attributs (ENTITE entite,
						IN_STD vector<HDFAttributeDescriptor2>& liste);

	/**
	 * Lit les valeurs entières de l'attribut dont le descripteur est
	 * transmis en argument.
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	void lire_attr_entier (
						ENTITE entite, const HDFAttributeDescriptor2& desc,
						size_type premier, size_type nombre, int_4*& tableau);

	/**
	 * Lit les valeurs réelles de l'attribut dont le descripteur est
	 * transmis en argument.
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	void lire_attr_reel (ENTITE entite, const HDFAttributeDescriptor2& desc,
						 size_type premier, size_type nombre, double*& tableau);

	/**
	 * Lit les valeurs vecteur 2D de réel de l'attribut dont le descripteur 
	 * transmis en argument.
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour 2*nombre) où ranger les valeurs 
	 *				lues
	 */
	void lire_attr_vect_2d (ENTITE entite, const HDFAttributeDescriptor2& desc,
						 size_type premier, size_type nombre, double*& tableau);

	/**
	 * Lit les valeurs vecteur 3D de réel de l'attribut dont le descripteur 
	 * transmis en argument.
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		tableau (alloué pour 3*nombre) où ranger les valeurs 
	 *				lues
	 */
	void lire_attr_vect_3d (ENTITE entite, const HDFAttributeDescriptor2& desc,
						 size_type premier, size_type nombre, double*& tableau);

	/**
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @return		La longueur des chaines de caractères de l'attribut.
	 */
	size_type longueur_chaines (
						ENTITE entite, const HDFAttributeDescriptor2& desc);

	/**
	 * Lit les valeurs chaine de caractères de l'attribut dont le descripteur
	 * est transmis en argument.
	 * @param		entité de rattachement de l'attribut
	 * @param		descripteur de l'attribut
	 * @param		indice de la première valeur à lire
	 * @param		nombre de valeurs à lire
	 * @param		vecteur (alloué pour nombre) où ranger les valeurs 
	 *				lues
	 */
	void lire_attr_chaine (ENTITE entite, const HDFAttributeDescriptor2& desc,
						 size_type premier, size_type nombre, 
						 IN_STD vector<IN_STD string>& chaines);

	/**
	 * Classe représentant un groupe de données (nuage/ligne/surface/
	 * volume).
	 */
	class GroupeReader
	{
		public :

		/**
		 * Destructeur.
		 */
		virtual ~GroupeReader ( );

		/**
		 * @return		le nom du groupe.
		 */
		const IN_STD string& nom ( ) const;

		/**
		 * @return		La composition du groupe
		 */
		const Composition& composition ( ) const;

		/**
		 * Lit la liste des attributs (nom + type).
		 * @param	Entité de rattachement
		 * @param	En retour, la liste des attributs
		 */
		void lire_liste_attributs (MaliPPReader2::ENTITE entite,
						IN_STD vector<HDFAttributeDescriptor2>& liste);

		/**
		 * Lit les valeurs entières de l'attribut dont le descripteur est
		 * transmis en argument.
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @param		indice de la première valeur à lire
		 * @param		nombre de valeurs à lire
		 * @param		tableau (alloué pour nombre) où ranger les valeurs
		 *				lues
		 */
		virtual void lire_attr_entier (
				MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc,
				size_type premier, size_type nombre, int_4*& tableau);

		/**
		 * Lit les valeurs réelles de l'attribut dont le descripteur est
		 * transmis en argument.
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @param		indice de la première valeur à lire
		 * @param		nombre de valeurs à lire
		 * @param		tableau (alloué pour nombre) où ranger les valeurs
		 *				lues
		 */
		virtual void lire_attr_reel (
				MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc,
				size_type premier, size_type nombre, double*& tableau);

		/**
		 * Lit les valeurs vecteur 2D de réel de l'attribut dont le 
		 * descripteur transmis en argument.
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @param		indice de la première valeur à lire
		 * @param		nombre de valeurs à lire
		 * @param		tableau (alloué pour 2*nombre) où ranger les 
		 *				valeurs lues
		 */
		void lire_attr_vect_2d (
			MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc,
			size_type premier, size_type nombre, double*& tableau);

		/**
		 * Lit les valeurs vecteur 3D de réel de l'attribut dont le
		 * descripteur transmis en argument.
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @param		indice de la première valeur à lire
		 * @param		nombre de valeurs à lire
		 * @param		tableau (alloué pour 3*nombre) où ranger les 
		 *				valeurs *lues
		 */
		void lire_attr_vect_3d (
			MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc,
			size_type premier, size_type nombre, double*& tableau);

		/**
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @return		La longueur des chaines de caractères de l'attribut.
		 */
		size_type longueur_chaines (
			MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc);

		/**
		 * Lit les valeurs chaine de caractères de l'attribut dont le
		 * descripteur est transmis en argument.
		 * @param		entité de rattachement de l'attribut
		 * @param		descripteur de l'attribut
		 * @param		indice de la première valeur à lire
		 * @param		nombre de valeurs à lire
		 * @param		vecteur (alloué pour nombre) où ranger les valeurs
		 *				lues
		 */
		void lire_attr_chaine (
				MaliPPReader2::ENTITE entite, const HDFAttributeDescriptor2& desc,
				size_type premier, size_type nombre, 
						IN_STD vector<IN_STD string>& chaines);

		protected :

		/**
		 * Constructeur. Lecture de la composition du groupe.
		 * @param		Lecteur du maillage. Ne doit pas être nul.
		 * @param		Nom du groupe
		 * @param		Groupe HDF associé
		 * @param		Nom du groupe HDF contenant la composition
		 */
		GroupeReader (MaliPPReader2* reader, const IN_STD string& nom, 
		              const H5::Group& group, const IN_STD string& composition);

		/**
		 * Constructeur de copie => copie membre à membre.
		 */
		GroupeReader (const GroupeReader& reader);

		/**
		 * Opérateur = :  => copie membre à membre.
		 */
		GroupeReader& operator = (const GroupeReader& reader);

		/**
		 * @return		Le lecteur du maillgae
		 */
		MaliPPReader2* reader ( )
		{ return m_reader; }
		MaliPPReader2* reader ( ) const
		{ return m_reader; }

		/**
		 * @return		le groupe HDF associé
		 */
		H5::Group& groupe_hdf ( );
		const H5::Group& groupe_hdf ( ) const;

		/** Lève une exception si la classe ne gère pas d'attributs
		 * associé à ce type d'entité.
		 */
		virtual void evaluer_entite (MaliPPReader2::ENTITE entite) = 0;

		/**
		 * @return		Le groupe HDF correspondant à l'entité transmise 
		 *				en argument.
		 */
		H5::Group groupe_attribut (ENTITE entite);


		private :

		/** Le lecteur du maillage. */
		MaliPPReader2*			m_reader;

		/** Le nom du groupe. */
		IN_STD string			m_nom;

		/** Le groupe HDF associé. */
		H5::Group				m_groupe_hdf;

		/** La composition du groupe. */
		Composition				m_composition;
	};	// class GroupeReader


	/**
	 * Classe représentant un nuage de noeuds.
	 */
	class NuageReader : public GroupeReader
	{
		public :

		/**
		 * Constructeur. Lecture de la composition du groupe.
		 * @param		Lecteur du maillage. Ne doit pas être nul.
		 * @param		Nom du groupe
		 * @param		Groupe HDF associé
		 */
		NuageReader (MaliPPReader2* reader, const IN_STD string& nom, 
		             const H5::Group& group);

		/**
		 * Constructeur de copie : copie membre à membre.
		 */
		NuageReader (const NuageReader&);

		/**
		 * Opérateur = : copie membre à membre.
		 */
		NuageReader& operator = (const NuageReader&);

		/**
		 * Destructeur.
		 */
		virtual ~NuageReader ( );

		/**
		 * Alloue un tampon pour la lecture des identifiants par passes de
		 * nb_elements. Ce tableau est à libérer par delete [].
		 * @param		nombre d'identifiants lus par passe
		 * @return		le tableau alloué
		 * @see			lire_noeuds_ids
		 */
		size_type* allouer_tampon_ids (size_type nb_elements);

		/**
		 * Lit les identifiants des noeuds et les place dans le tableau.
		 * @param		indice du premier identifiant à lire
		 * @param		nombre d'identifiants à lire
		 * @param		tableau (alloué) où ranger les identifiants lus
		 * @return		nombre d'identifiants effectivement lus
		 * @see			allouer_tampon_ids
		 */
		size_type lire_noeuds_ids (size_type premier, size_type nombre,
		                           size_type*& tableau);


		protected :

		/** Lève une exception si la classe ne gère pas d'attributs
		 * associé à ce type d'entité.
		 */
		virtual void evaluer_entite (MaliPPReader2::ENTITE entite);
	};	// class NuageReader


	/**
	 * Classe représentant une ligne.
	 */
	class LigneReader : public GroupeReader
	{
		public :

		/**
		 * Constructeur. Lecture de la composition du groupe.
		 * @param		Lecteur du maillage. Ne doit pas être nul.
		 * @param		Nom du groupe
		 * @param		Groupe HDF associé
		 */
		LigneReader (MaliPPReader2* reader, const IN_STD string& nom, 
		             const H5::Group& group);
		
		/**
		 * Constructeur de copie : copie membre à membre.
		 */
		LigneReader (const LigneReader&);

		/**
		 * Opérateur = : copie membre à membre.
		 */
		LigneReader& operator = (const LigneReader&);

		/**
		 * Destructeur.
		 */
		virtual ~LigneReader ( );

		/**
		 * Alloue un tampon pour la lecture des identifiants par passes de
		 * nb_elements. Ce tableau est à libérer par delete [].
		 * @param		nombre d'identifiants lus par passe
		 * @return		le tableau alloué
		 * @see			lire_bras_ids
		 */
		size_type* allouer_tampon_ids (size_type nb_elements);

		/**
		 * Lit les identifiants des bras et les place dans le tableau.
		 * @param		indice du premier identifiant à lire
		 * @param		nombre d'identifiants à lire
		 * @param		tableau (alloué) où ranger les identifiants lus
		 * @return		nombre d'identifiants effectivement lus
		 * @see			allouer_tampon_ids
		 */
		size_type lire_bras_ids (size_type premier, size_type nombre,
		                         size_type*& tableau);


		protected :

		/** Lève une exception si la classe ne gère pas d'attributs
		 * associé à ce type d'entité.
		 */
		virtual void evaluer_entite (MaliPPReader2::ENTITE entite);
	};	// class LigneReader


	/**
	 * Classe représentant une surface.
	 */
	class SurfaceReader : public GroupeReader
	{
		public :

		/**
		 * Constructeur. Lecture de la composition du groupe.
		 * @param		Lecteur du maillage. Ne doit pas être nul.
		 * @param		Nom du groupe
		 * @param		Groupe HDF associé
		 */
		SurfaceReader (MaliPPReader2* reader, const IN_STD string& nom, 
		               const H5::Group& group);

		/**
		 * Constructeur de copie : copie membre à membre.
		 */
		SurfaceReader (const SurfaceReader&);

		/**
		 * Opérateur = : copie membre à membre.
		 */
		SurfaceReader& operator = (const SurfaceReader&);

		/**
		 * Destructeur.
		 */
		virtual ~SurfaceReader ( );

		/**
		 * Alloue un tampon pour la lecture des identifiants par passes de
		 * nb_elements. Ce tableau est à libérer par delete [].
		 * @param		nombre d'identifiants lus par passe
		 * @return		le tableau alloué
		 * @see			lire_mailles_ids
		 */
		size_type* allouer_tampon_ids (size_type nb_elements);

		/**
		 * Lit les identifiants des mailles et les place dans le tableau.
		 * @param		indice du premier identifiant à lire
		 * @param		nombre d'identifiants à lire
		 * @param		tableau (alloué) où ranger les identifiants lus
		 * @return		nombre d'identifiants effectivement lus
		 * @see			allouer_tampon_ids
		 */
		size_type lire_mailles_ids (size_type premier, size_type nombre,
		                            size_type*& tableau);


		protected :

		/** Lève une exception si la classe ne gère pas d'attributs
		 * associé à ce type d'entité.
		 */
		virtual void evaluer_entite (MaliPPReader2::ENTITE entite);
	};	// class SurfaceReader


	/**
	 * Classe représentant un volume.
	 */
	class VolumeReader : public GroupeReader
	{
		public :

		/**
		 * Constructeur. Lecture de la composition du groupe.
		 * @param		Lecteur du maillage. Ne doit pas être nul.
		 * @param		Nom du groupe
		 * @param		Volume HDF associé
		 */
		VolumeReader (MaliPPReader2* reader, const IN_STD string& nom, 
		              const H5::Group& group);

		/**
		 * Constructeur de copie : copie membre à membre.
		 */
		VolumeReader (const VolumeReader&);

		/**
		 * Opérateur = : copie membre à membre.
		 */
		VolumeReader& operator = (const VolumeReader&);

		/**
		 * Destructeur.
		 */
		virtual ~VolumeReader ( );

		/**
		 * Alloue un tampon pour la lecture des identifiants par passes de
		 * nb_elements. Ce tableau est à libérer par delete [].
		 * @param		nombre d'identifiants lus par passe
		 * @return		le tableau alloué
		 * @see			lire_mailles_ids
		 */
		size_type* allouer_tampon_ids (size_type nb_elements);

		/**
		 * Lit les identifiants des mailles et les place dans le tableau.
		 * @param		indice du premier identifiant à lire
		 * @param		nombre d'identifiants à lire
		 * @param		tableau (alloué) où ranger les identifiants lus
		 * @return		nombre d'identifiants effectivement lus
		 * @see			allouer_tampon_ids
		 */
		size_type lire_mailles_ids (size_type premier, size_type nombre,
		                            size_type*& tableau);


		protected :

		/** Lève une exception si la classe ne gère pas d'attributs
		 * associé à ce type d'entité.
		 */
		virtual void evaluer_entite (MaliPPReader2::ENTITE entite);
	};	// class VolumeReader

	/**
	 * @param		Nom du nuage à retourner
	 * @return		Le lecteur associé au nuage dont le nom est renseigné
	 *				en argument
	 */
	NuageReader nuage (const IN_STD string& nom);

	/**
	 * @param		Nom de la ligne à retourner
	 * @return		Le lecteur associé à la ligne dont le nom est 
	 *				renseigné en argument
	 */
	LigneReader ligne (const IN_STD string& nom);

	/**
	 * @param		Nom de la surface à retourner
	 * @return		Le lecteur associé à la surface dont le nom est 
	 *				renseigné en argument
	 */
	SurfaceReader surface (const IN_STD string& nom);

	/**
	 * @param		Nom du volume à retourner
	 * @return		Le lecteur associé au volume dont le nom est renseigné
	 *				en argument
	 */
	VolumeReader volume (const IN_STD string& nom);


	protected :

	/**
	 * @param		Indice du polygône
	 * @param		Tampon utilisé pour la lecture des types de polygônes
	 * @param		Taille du tampon
	 * @return		La position du premier identifiant des noeuds du
	 *				polygône dont l'indice est transmis en argument.
	 */
	size_type position_ids_noeuds_polygone (
					size_type indice, object_type* tampon, size_type taille);

	/**
	 * @param		Indice du polyèdre
	 * @param		Tampon utilisé pour la lecture des types de polyèdres
	 * @param		Taille du tampon
	 * @return		La position du premier identifiant des noeuds du
	 *				polyèdre dont l'indice est transmis en argument.
	 */
	size_type position_ids_noeuds_polyedre (
					size_type indice, object_type* tampon, size_type taille);

	/**
	 * @return		Le nom du groupe HDF correspondant à l'entité transmise 
	 *				en argument.
	 */
	static const IN_STD string& nom_groupe_attribut (ENTITE entite);

	/**
	 * @return		Le groupe HDF correspondant à l'entité transmise 
	 *				en argument.
	 */
	H5::Group groupe_attribut (ENTITE entite);


	private:

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	MaliPPReader2 (const MaliPPReader2&);
	MaliPPReader2& operator = (const MaliPPReader2&);

	/**
	 * Lecture de l'entête du maillage.
	 */
	void lire_entete_maillage ( );

	/**
	 * Lecture de la composition du maillage (nombre de chaque type de 
	 * données et distribution selon le nombre de noeuds qui les
	 * définissent).
	 */
	void lire_composition_maillage ( );

	/** Le nom du fichier. */
	IN_STD string						m_nom_fichier;

	/** Le numéro du maillage. */
	size_type							m_numero_maillage;

	/** La version de Lima++ lors de l'écriture du fichier. */
	Version								m_lima_version_fichier;

	/** La version du format lima++ décriture du fichier. */
	Version								m_mli_version_fichier;

	/** La version de Lima++ lors de l'écriture du maillage. */
	Version								m_lima_version_maillage;

	/** La version du format lima++ décriture du maillage. */
	Version								m_mli_version_maillage;

	/** Le titre du maillage. */
	IN_STD string						m_titre;

	/** La date de création. */
	IN_STD string						m_date;

	/** La dimension du maillage. */
	dim_t								m_dimension;

	/** Le type de géométrie du maillage. */
	geometrie_t							m_geometrie;

	/** Le système de coordonnées. */
	coordonnee_t						m_systeme_coords;

	/** L'unité d'angle. */
	double								m_unite_angle;

	/** L'unité de longueur. */
	double								m_unite_longueur;

	/** La composition en noeuds.*/
	Composition							m_composition_noeuds;

	/** La composition en bras.*/
	Composition							m_composition_bras;

	/** La composition en polygones.*/
	Composition							m_composition_polygones;

	/** La composition en polyedres.*/
	Composition							m_composition_polyedres;

	/** Le fichier HDF. */
	IN_STD auto_ptr<H5::H5File>		m_fichier_hdf;

	/** Le maillage. */
	H5::Group							m_maillage_hdf;

	/** Pour optimisation, dernier indice et dernière valeur retournée
	 * par position_ids_noeuds_polygone. */
	size_type							m_indice_ids_noeuds_polygone,
										m_pos_ids_noeuds_polygone;

	/** Pour optimisation, dernier indice et dernière valeur retournée
	 * par position_ids_noeuds_polyedre. */
	size_type							m_indice_ids_noeuds_polyedre,
										m_pos_ids_noeuds_polyedre;
};	// class MaliPPReader2


/**
 * Classe décrivant la structure HDFAttributeDescriptor2 pour le stockage
 * sous HDF.
 */
class HDFAttributeDescriptor2Type : public H5::CompType
{
	public :

	HDFAttributeDescriptor2Type ( );

	static const IN_STD string			m_name;
	static const IN_STD string			m_type;
	static const IN_STD string			m_size;


	private :

	HDFAttributeDescriptor2Type (const HDFAttributeDescriptor2Type&);
	HDFAttributeDescriptor2Type& operator = (const HDFAttributeDescriptor2Type&);
};	// class HDFAttributeDescriptor2Type


/**
 *
 */
class MaliPPWriter2
{
	public :

	/**
	 * Constructeur. Ouvre le fichier en écriture et écrit les données
	 * générales du maillage.
	 * @param		Nom du fichier
	 * @param		Numéro du maillage dans le fichier
	 * @exception	Une exception est levée en cas d'erreur
	 */
	MaliPPWriter2 (const IN_STD string& nom_fichier, size_type num=1);

	/**
	 * Destructeur : ferme le fichier.
	 */
	~MaliPPWriter2 ( );

	void close ( );

	//! Dimension du maillage.
	void          dimension(dim_t dim);
	//! Unité de longueur du maillage.
	void          unite_longueur(double unilo);
	//! Unité d'angle du maillage.
	void          unite_angle(double unia);
	//! Type de géométrie du maillage.
	void          type_geometrie(geometrie_t geom);
	//! Type de coordonnée du maillage.
	void          type_coordonnee(coordonnee_t tycoo);
	//! Titre du maillage.
	void          titre(const IN_STD string& titre);
	//! Date du maillage.
	void          date(const IN_STD string& date);

	void		 beginWrite();

	void 		 writeNodesInfo (bool isContiguous, id_type nbNodes, id_type firstID=0);
	void 		 writeNodes (id_type nodesChunkSize, double* coordsX, double* coordsY, double* coordsZ, id_type* ids=NULL);
	void 		 writeNodeSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes);
	void 		 writeNodeSetData (std::string name, id_type nodesChunkSize, id_type* ids);
	void 		 writeNodeAttributes ( );
	void 		 writeNodeSetsAttributes ( );

	void 		 writeEdgesInfo (bool isContiguous, id_type nbEdges, id_type firstID=0);
	void 		 writeEdges (id_type edgesChunkSize, id_type* edge2nodeIDs, id_type* ids=NULL);
	void 		 writeEdgeSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes);
	void 		 writeEdgeSetData (std::string name, id_type nodesChunkSize, id_type* ids);
	void 		 writeEdgeAttributes ( );
	void 		 writeEdgeSetsAttributes ( );

	void 		 writeFacesInfo (bool isContiguous, id_type nbFaces, id_type firstID=0);
	void 		 writeFaces (id_type facesChunkSize, id_type* face2nodeIDs, id_type* nbNodesPerFace, id_type* ids=NULL);
	void 		 writeFaceSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes);
	void 		 writeFaceSetData (std::string name, id_type nodesChunkSize, id_type* ids);
	void 		 writeFaceSetData (std::string name, id_type nodesChunkSize, id_type* ids, id_type* nbNodesPerFace);
	void 		 writeFaceAttributes ( );
	void 		 writeFaceSetsAttributes ( );

	void 		 writeRegionsInfo (bool isContiguous, id_type nbRegions, id_type firstID=0);
	void 		 writeRegions (id_type regionsChunkSize, id_type* region2nodeIDs, Lima::Polyedre::PolyedreType* regionTypes, id_type* ids=NULL);
	void 		 writeRegionSetInfo (id_type nbSet, std::vector<std::string> setNames, std::vector<id_type> setSizes);
	void 		 writeRegionSetData (std::string name, id_type nodesChunkSize, id_type* ids);
	void 		 writeRegionSetData (std::string name, id_type nodesChunkSize, id_type* ids, Lima::Polyedre::PolyedreType* regionTypes);
	void 		 writeRegionAttributes ( );
	void 		 writeRegionSetsAttributes ( );


	private :

	//! Creation du header d'un fichier
	void 		 createMainHeader ( );
	//! Ecriture des informations relatives aux versions (Lima/Format mli)
	void         writeVersionsInfos (H5::H5Object&);
	//! incremente le nombre de maillage du fichier.
	// Rem : les identifiants ne sont pas forcement successifs.
	void         nbMeshInc ( );
	//! Creation du header d'un maillage
	void         writeMeshHeader ( );
	//! Creation d'un maillage
	void         createMesh ( );

	//! Creation du squelette d'un maillage .mli
	void         createMeshDataGroup ( );

	void		 writeMeshAttributes ( );

	void writeComposition (const Composition& composition, const std::string& name,
	                                      H5::Group& compositionGroup);

	/**
	 * Constructeurs et destructeur : interdits.
	 */
	MaliPPWriter2 ( );
	MaliPPWriter2 (const MaliPPWriter2&);
	MaliPPWriter2& operator = (const MaliPPWriter2&);

	/** Le nom du fichier. */
	IN_STD string						m_fileName;

	/** Le numéro du maillage. */
	size_type							m_meshNum;

	//! Le groupe contenant le maillage.
	H5::Group							m_meshGroup;

	//! Le fichier HDF contenant le maillage.
	IN_STD auto_ptr<H5::H5File>		m_hdfFile;

	//! Dimension du maillage.
	dim_t m_dim;
	//! Unité de longueur du maillage.
	double m_unilo;
	//! Unité d'angle du maillage.
	double m_unia;
	//! Type de géométrie du maillage.
	geometrie_t m_geom;
	//! Type de coordonnée du maillage.
	coordonnee_t m_tycoo;
	//! Titre du maillage.
	IN_STD string m_titre;
	//! Date du maillage.
	IN_STD string m_date;

	id_type m_nbMeshAttributes;
	H5::Group m_meshAttributesGroup;

	bool m_nodesIsContiguous;
	id_type m_nodeIndexCurrent;
	id_type m_nbNodes;
	Composition m_compoNode;
	H5::Group m_nodeGroup;
	H5::DataSet	m_nodeZCoordDataSet;
	H5::DataSet	m_nodeYCoordDataSet;
	H5::DataSet	m_nodeXCoordDataSet;
	H5::DataSet m_nodeIdsDataSet;
	H5::Group m_nodeSetGroup;
	id_type m_nbNodeSet;
	std::vector<H5::Group> m_nodeSetGroups;
	std::map<std::string,id_type> m_nodeSetNames;
	std::vector<id_type> m_nodeSetSizes;
	std::vector<id_type> m_nodeSetIndexCurrent;
	std::vector<H5::DataSet> m_nodeSetIdsDataSet;
	id_type m_nbNodeAttributes;
	H5::Group m_nodeAttributesGroup;
	id_type m_nbNodeSetsAttributes;
	H5::Group m_nodeSetsAttributesGroup;

	bool m_edgesIsContiguous;
	id_type m_edgeIndexCurrent;
	id_type m_nbEdges;
	Composition m_compoEdge;
	H5::Group m_edgeGroup;
	H5::DataSet m_edge2nodeIdsDataSet;
	H5::DataSet m_edgeIdsDataSet;
	H5::Group m_edgeSetGroup;
	id_type m_nbEdgeSet;
	std::vector<H5::Group> m_edgeSetGroups;
	std::map<std::string,id_type> m_edgeSetNames;
	std::vector<id_type> m_edgeSetSizes;
	std::vector<id_type> m_edgeSetIndexCurrent;
	std::vector<H5::DataSet> m_edgeSetIdsDataSet;
	id_type m_nbEdgeAttributes;
	H5::Group m_edgeAttributesGroup;
	id_type m_nbEdgeSetsAttributes;
	H5::Group m_edgeSetsAttributesGroup;

	bool m_facesIsContiguous;
	id_type m_faceIndexCurrent;
	id_type m_nbFaces;
	size_t m_nbNodesPerFaceIndexCurrent;
	Composition m_compoFace;
	H5::Group m_faceGroup;
	H5::DataSet m_face2nodeIdsDataSet;
	H5::DataSet m_nbNodesPerFaceDataSet;
	H5::DataSet m_faceIdsDataSet;
	H5::Group m_faceSetGroup;
	id_type m_nbFaceSet;
	std::vector<H5::Group> m_faceSetGroups;
	std::map<std::string,id_type> m_faceSetNames;
	std::vector<id_type> m_faceSetSizes;
	std::vector<id_type> m_faceSetIndexCurrent;
	std::vector<H5::DataSet> m_faceSetIdsDataSet;
	std::vector<Composition> m_faceSetCompo;
	id_type m_nbFaceAttributes;
	H5::Group m_faceAttributesGroup;
	id_type m_nbFaceSetsAttributes;
	H5::Group m_faceSetsAttributesGroup;

	bool m_regionsIsContiguous;
	id_type m_regionIndexCurrent;
	id_type m_nbRegions;
	size_t m_nbNodesPerRegionIndexCurrent;
	Composition m_compoRegion;
	H5::Group m_regionGroup;
	H5::DataSet m_region2nodeIdsDataSet;
	H5::DataSet m_nbNodesPerRegionDataSet;
	H5::DataSet m_regionIdsDataSet;
	H5::Group m_regionSetGroup;
	id_type m_nbRegionSet;
	std::vector<H5::Group> m_regionSetGroups;
	std::map<std::string,id_type> m_regionSetNames;
	std::vector<id_type> m_regionSetSizes;
	std::vector<id_type> m_regionSetIndexCurrent;
	std::vector<H5::DataSet> m_regionSetIdsDataSet;
	std::vector<Composition> m_regionSetCompo;
	id_type m_nbRegionAttributes;
	H5::Group m_regionAttributesGroup;
	id_type m_nbRegionSetsAttributes;
	H5::Group m_regionSetsAttributesGroup;

};	// class MaliPPWriter2


#ifndef SWIG
END_NAMESPACE_LIMA
#endif

#endif	// _MALIPP2_H

#endif // __INTERNE_MALIPP
