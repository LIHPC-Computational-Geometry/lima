#include "XlmLima/XlmLimaComparer.h"
#include "XlmLima/XlmLimaException.h"

#include <Lima/noeud.h>
#include <Lima/bras.h>
#include <Lima/polygone.h>
#include <Lima/polyedre.h>
#include <Lima/nuage.h>
#include <Lima/ligne.h>
#include <Lima/surface.h>
#include <Lima/volume.h>
#include <Lima/liste_attribut.h>

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
#include <iomanip>
#include <sstream>
#include <functional>
#endif	// __hpux

#include <float.h>
#include <math.h>


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA


static double epsilon2	= DBL_EPSILON * DBL_EPSILON;


inline bool operator == (const Noeud& n1, const Noeud& n2)
{
	IN_STD less_equal<double>	le;
	double	dx	= n1.x ( ) - n2.x ( );
	double	dy	= n1.y ( ) - n2.y ( );
	double	dz	= n1.z ( ) - n2.z ( );
	double	distance2	= dx * dx + dy * dy + dz * dz;

	return le (distance2, epsilon2);
}	// operator == (const Noeud& n1, const Noeud& n2)

inline bool operator != (const Noeud& n1, const Noeud& n2)
{
	return !(n1 == n2);
}	// operator != (const Noeud& n1, const Noeud& n2)


inline bool isEqual (double d1, double d2)
{
	if (d1 == d2)
		return true;

	double	percent	= fabs ((d1 - d2) / (d1 + d2));
	return (percent < 1E-6 ? true : false);
}	// isEqual


template <class T> string type2str (T t)
{
	ostringstream	stream;
	stream << setprecision (20) << t << ends;

	return stream.str ( );
}	// type2str


static string typeCoord2str (coordonnee_t t)
{
	switch (t)
	{
		CARTESIEN	: return "CARTESIEN";
		CYLINDRIQUE	: return "CYLINDRIQUE";
	}

	return "INCONNU";
}	// typeCoord2str


static string typeGeom2str (geometrie_t g)
{
	switch (g)
	{
		case	NORMAL			: return "NORMAL";
		case	AXISYMETRIQUE	: return "AXISYMETRIQUE";
		case	PERIODIQUE		: return "PERIODIQUE";
	}

	return "INCONNU";
}	// typeGeom2str


static void compareAttribute (const Attribut& a, const Attribut& b)
{
	if (a.nom ( ) != b.nom ( ))
		throw erreur ("Nom d'attribut différent : " + a.nom ( ) + " / " +
		              b.nom ( ));
	if (a.type ( ) != b.type ( ))
		throw erreur ("Nom d'attribut différent : " + type2str (a.type ( )) +
		              " / " + type2str (b.type ( )));
	if(a.nb_elements ( ) != b.nb_elements ( ))
		throw erreur ("Nombre d'éléments d'attribut différent : " +
		              type2str (a.nb_elements ( )) + " / " +
		              type2str (b.nb_elements ( )));
	ostringstream	str;
	if (Attribut::ENTIER == a.type ( ))
	{
		int	i1	= 0, i2	= 0;
		for (size_t i = 0; i < a.nb_elements ( ); i++)
		{
			a.get_val (i, i1);
			b.get_val (i, i2);

			if (i1 != i2)
			{
				str << i << "-ème valeur de l'attribut " << a.nom ( )
				    << " de type " << type2str (a.type ( )) << " différente ("
				    << i1 << " / " << i2 << ").";
				throw erreur (str.str ( ));
			}	// if (i1 != i2)
		}	// for (size_t i = 0; i < a.nb_elements ( ); i++)
	}	// if (Attribut::ENTIER == a.type ( ))
	else if (Attribut::CHAINE == a.type ( ))
	{
		string	str1, str2;
		for (size_t i = 0; i < a.nb_elements ( ); i++)
		{	
			a.get_val (i, str1);
			b.get_val (i, str2);	// CP v 6.38.0, correctif (str1 -> str2)
			if (str1 != str2)
			{
				str << i << "-ème valeur de l'attribut " << a.nom ( )
				    << " de type " << type2str (a.type ( )) << " différente ("
				    << str1 << " / " << str2 << ").";
				throw erreur (str.str ( ));
			}	// if (str1 != str2)
		}	// for (size_t i = 0; i < a.nb_elements ( ); i++)
	}	// else if (Attribut::CHAINE == a.type ( ))
	else if (Attribut::REEL == a.type ( ))
	{
		double	d1	= 0., d2	= 0.;
		for (size_t i = 0; i < a.nb_elements ( ); i++)
		{
			a.get_val (i, d1);
			b.get_val (i, d2);

			if (false == isEqual (d1, d2))
			{
				str << i << "-ème valeur de l'attribut " << a.nom ( )
				    << " de type " << type2str (a.type ( )) << " différente ("
				    << d1 << " / " << d2 << ").";
				throw erreur (str.str ( ));
			}	// if (false == isEqual (d1, d2))
		}	// for (size_t i = 0; i < a.nb_elements ( ); i++)
	}	// else if (Attribut::REEL == a.type ( ))
	else if (Attribut::VECT2D == a.type ( ))
	{
		Vect2d	v1	= 0., v2	= 0.;
		for (size_t i = 0; i < a.nb_elements ( ); i++)
		{
			a.get_val (i, v1);
			b.get_val (i, v2);

			if ((false == isEqual (v1.x ( ), v2.x ( ))) ||
			    (false == isEqual (v1.y ( ), v2.y ( ))))
			{
				str << i << "-ème valeur de l'attribut " << a.nom ( )
				    << " de type " << type2str (a.type ( )) << " différente (("
				    << v1.x ( ) << ", " << v1.y ( ) << ") / ("
				    << v2.x ( ) << ", " << v2.y ( ) << ")).";
				throw erreur (str.str ( ));
			}	// if ((false == isEqual (v1.x ( ), v2.x ( ))) || ...
		}	// for (size_t i = 0; i < a.nb_elements ( ); i++)
	}	// else if (Attribut::VECT2D == a.type ( ))
	else if (Attribut::VECT3D == a.type ( ))
	{
		Vect3d	v1	= 0., v2	= 0.;
		for (size_t i = 0; i < a.nb_elements ( ); i++)
		{
			a.get_val (i, v1);
			b.get_val (i, v2);

			if ((false == isEqual (v1.x ( ), v2.x ( ))) ||
			    (false == isEqual (v1.y ( ), v2.y ( ))) ||
			    (false == isEqual (v1.z ( ), v2.z ( ))))
			{
				str << i << "-ème valeur de l'attribut " << a.nom ( )
				    << " de type " << type2str (a.type ( )) << " différente (("
				    << v1.x( ) << ", " << v1.y( ) << ", " << v1.z( ) << ") / ("
				    << v2.x( ) << ", " << v2.y( ) << ", " << v2.z( ) << ")).";
				throw erreur (str.str ( ));
			}	// if ((false == isEqual (v1.x ( ), v2.x ( ))) || ...
		}	// for (size_t i = 0; i < a.nb_elements ( ); i++)
	}	// else if (Attribut::VECT3D == a.type ( ))
}	// compareAttribute


static void compareAttributes (
							const Liste_attribut& l1, const Liste_attribut& l2)
{	
	if (l1.nb_elements ( ) != l2.nb_elements ( ))
		throw erreur ("Nombre d'éléments différent dans les listes d'attributs.");

	for (size_t i = 0; i < l1.nb_elements ( ); i++)
		compareAttribute (l1.element (i), l2.element (i));
}	// compareAttributes


XlmLimaComparer::XlmLimaComparer (
				const string& inputFile1, IN_LIMA format_t inputFormat1,
				const IN_STD string& inputFile2, IN_LIMA format_t inputFormat2)
	: XlmLimaAction (inputFile1, inputFormat1),
	  _inputFile2 (inputFile2), _inputFormat2 (inputFormat2)
{
}	// XlmLimaComparer::XlmLimaComparer


XlmLimaComparer::~XlmLimaComparer ( )
{
}	// XlmLimaComparer::~XlmLimaComparer


void XlmLimaComparer::run ( )
{
	try
	{
		ostringstream	str;
		Maillage		mesh1, mesh2;
		cout << "Comparaison des maillages " << getInputFile ( ) << " et "
		     << getInputFile2 ( ) << " ..." << endl;
		mesh1.lire (getInputFile ( ), getInputFormat ( ));
		mesh2.lire (getInputFile2 ( ), getInputFormat2 ( ));

		// Infos générales du maillage :
		if (mesh1.unite_longueur ( ) != mesh2.unite_longueur ( ))
		{
			str << "Unités de longueur différentes (" << mesh1.unite_longueur( )
			    << " / " << mesh2.unite_longueur ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.unite_longueur ( ) != mesh2.unite_longueur ( ))
		if (mesh1.unite_angle ( ) != mesh2.unite_angle ( ))
		{
			str << "Unités d'angles différentes (" << mesh1.unite_angle ( )
			    << " / " << mesh2.unite_angle ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.unite_angle ( ) != mesh2.unite_angle ( ))
		if (mesh1.type_coordonnee ( ) != mesh2.type_coordonnee ( ))
		{
			str << "Types de coordonnées différents ("
			    << typeCoord2str (mesh1.type_coordonnee ( )) << " / "
			    << typeCoord2str (mesh2.type_coordonnee ( )) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.type_coordonnee ( ) != mesh2.type_coordonnee ( ))
		if (mesh1.type_geometrie ( ) != mesh2.type_geometrie ( ))
		{
			str << "Types de géométries différents ("
			    << typeGeom2str (mesh1.type_geometrie ( )) << " / "
			    << typeGeom2str (mesh2.type_geometrie ( )) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.type_geometrie ( ) != mesh2.type_geometrie ( ))
		if (mesh1.dimension ( ) != mesh2.dimension ( ))
		{
			str << "Dimensions différentes (" << mesh1.dimension ( ) << " / "
			    << mesh2.dimension ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.dimension ( ) != mesh2.dimension ( ))
		if (mesh1.nb_noeuds ( ) != mesh2.nb_noeuds ( ))
		{
			str << "Nombre de noeuds différent (" << mesh1.nb_noeuds ( )
			    << " / " << mesh2.nb_noeuds ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_noeuds ( ) != mesh2.nb_noeuds ( ))
		if (mesh1.nb_bras ( ) != mesh2.nb_bras ( ))
		{
			str << "Nombre de bras différent (" << mesh1.nb_bras ( )
			    << " / " << mesh2.nb_bras ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_bras ( ) != mesh2.nb_bras ( ))
		if (mesh1.nb_polygones ( ) != mesh2.nb_polygones ( ))
		{
			str << "Nombre de polygones différent (" << mesh1.nb_polygones ( )
			    << " / " << mesh2.nb_polygones ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_polygones ( ) != mesh2.nb_polygones ( ))
		if (mesh1.nb_polyedres ( ) != mesh2.nb_polyedres ( ))
		{
			str << "Nombre de polyèdres différent (" << mesh1.nb_polyedres ( )
			    << " / " << mesh2.nb_polyedres ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_polyedres ( ) != mesh2.nb_polyedres ( ))
		if (mesh1.nb_nuages ( ) != mesh2.nb_nuages ( ))
		{
			str << "Nombre de nuages différent (" << mesh1.nb_nuages ( )
			    << " / " << mesh2.nb_nuages ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_nuages ( ) != mesh2.nb_nuages ( ))
		if (mesh1.nb_lignes ( ) != mesh2.nb_lignes ( ))
		{
			str << "Nombre de lignes différent (" << mesh1.nb_lignes ( )
			    << " / " << mesh2.nb_lignes ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_lignes ( ) != mesh2.nb_lignes ( ))
		if (mesh1.nb_surfaces ( ) != mesh2.nb_surfaces ( ))
		{
			str << "Nombre de surfaces différent (" << mesh1.nb_surfaces ( )
			    << " / " << mesh2.nb_surfaces ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_surfaces ( ) != mesh2.nb_surfaces ( ))
		if (mesh1.nb_volumes ( ) != mesh2.nb_volumes ( ))
		{
			str << "Nombre de volumes différent (" << mesh1.nb_volumes ( )
			    << " / " << mesh2.nb_volumes ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.nb_volumes ( ) != mesh2.nb_volumes ( ))

		// Comparaison attributs maillage :
		compareAttributes (mesh1.att_maillage ( ), mesh2.att_maillage ( ));
		compareAttributes (mesh1.att_noeuds ( ), mesh2.att_noeuds ( ));
		compareAttributes (mesh1.att_bras ( ), mesh2.att_bras ( ));
		compareAttributes (mesh1.att_polygones ( ), mesh2.att_polygones ( ));
		compareAttributes (mesh1.att_polyedres ( ), mesh2.att_polyedres ( ));

		// Comparaison des noeuds :
		epsilon2	= getPrecision ( ) * getPrecision ( );
		size_t	i	= 0, j	= 0;
		for (i = 0; i < mesh1.nb_noeuds ( ); i++)
		{
			Noeud	n1	= mesh1.noeud (i), n2	= mesh2.noeud (i);
			if (n1.id ( ) != n2.id ( ))
			{
				str << "Les " << i << "-ièmes noeuds n'ont pas les mêmes "
				    << "identifiants : " << n1.id( ) << "/" << n2.id( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (n1.id ( ) != n2.id ( ))
			if (n1 != n2)
			{
				str << "Les noeuds " << n1.id ( ) << " n'ont pas les mêmes "
				    << "coordonnées (" << n1.x ( ) << ", " << n1.y ( ) << ", "
				    << n1.z ( ) << ") / (" << n2.x ( ) << ", " << n2.y ( )
				    << ", " << n2.z ( ) << ").";
				throw XlmLimaException (str.str ( ));
			}	// if (n1 != n2)
		}	// for (i = 0; i < mesh1.nb_noeuds ( ); i++)

		// Comparaison des bras :
		for (i = 0; i < mesh1.nb_bras ( ); i++)
		{
			Bras	b1	= mesh1.bras (i), b2	= mesh2.bras (i);
			if (b1.id ( ) != b2.id ( ))
			{
				str << "Les " << i << "-ièmes bras n'ont pas les mêmes "
				    << "identifiants : " << b1.id( ) << "/" << b2.id( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (b1.id ( ) != b2.id ( ))
			if (b1.nb_noeuds ( ) != b2.nb_noeuds ( ))
			{
				str << "Les bras " << b1.id ( )
				    << " n'ont pas le même nombre de noeuds.";
				throw XlmLimaException (str.str ( ));
			}	// if (b1.nb_noeuds ( ) != b2.nb_noeuds ( ))
			for (j = 0; j < b1.nb_noeuds ( ); j++)
			{
				if (b1.noeud (j).id ( ) != b2.noeud (j).id ( ))
				{
					str << "Les bras " << b1.id ( )
					    << " n'ont pas les mêmes noeuds.";
					throw XlmLimaException (str.str ( ));
				}	// if (b1.noeud (j).id ( ) != b2.noeud (j).id ( ))
			}	// for (j = 0; j < b1.nb_noeuds ( ); j++)
		}	// for (i = 0; i < mesh1.nb_bras ( ); i++)

		// Comparaison des polygones :
		for (i = 0; i < mesh1.nb_polygones ( ); i++)
		{
			Polygone	p1	= mesh1.polygone (i), p2	= mesh2.polygone (i);
			if (p1.id ( ) != p2.id ( ))
			{
				str << "Les " << i << "-ièmes polygones n'ont pas les mêmes "
				    << "identifiants : " << p1.id( ) << "/" << p2.id( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (p1.id ( ) != p2.id ( ))
			if (p1.nb_noeuds ( ) != p2.nb_noeuds ( ))
			{
				str << "Les polygones " << p1.id ( )
				    << " n'ont pas le même nombre de noeuds.";
				throw XlmLimaException (str.str ( ));
			}	// if (p1.nb_noeuds ( ) != p2.nb_noeuds ( ))
			for (j = 0; j < p1.nb_noeuds ( ); j++)
			{
				if (p1.noeud (j).id ( ) != p2.noeud (j).id ( ))
				{
					str << "Les polygones " << p1.id ( )
					    << " n'ont pas les mêmes noeuds.";
					throw XlmLimaException (str.str ( ));
				}	// if (p1.noeud (j).id ( ) != p2.noeud (j).id ( ))
			}	// for (j = 0; j < p1.nb_noeuds ( ); j++)
		}	// for (i = 0; i < mesh1.nb_polygones ( ); i++)

		// Comparaison des polyedres :
		for (i = 0; i < mesh1.nb_polyedres ( ); i++)
		{
			Polyedre	p1	= mesh1.polyedre (i), p2	= mesh2.polyedre (i);
			if (p1.id ( ) != p2.id ( ))
			{
				str << "Les " << i << "-ièmes polyèdres n'ont pas les mêmes "
				    << "identifiants : " << p1.id( ) << "/" << p2.id( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (p1.id ( ) != p2.id ( ))
			if (p1.nb_noeuds ( ) != p2.nb_noeuds ( ))
			{
				str << "Les polyèdres " << p1.id ( )
				    << " n'ont pas le même nombre de noeuds.";
				throw XlmLimaException (str.str ( ));
			}	// if (p1.nb_noeuds ( ) != p2.nb_noeuds ( ))
			for (j = 0; j < p1.nb_noeuds ( ); j++)
			{
				if (p1.noeud (j).id ( ) != p2.noeud (j).id ( ))
				{
					str << "Les polyèdres " << p1.id ( )
					    << " n'ont pas les mêmes noeuds.";
					throw XlmLimaException (str.str ( ));
				}	// if (p1.noeud (j).id ( ) != p2.noeud (j).id ( ))
			}	// for (j = 0; j < p1.nb_noeuds ( ); j++)
		}	// for (i = 0; i < mesh1.nb_polyedres ( ); i++)

		// Comparaison des nuages :
		for (i = 0; i < mesh1.nb_nuages ( ); i++)
		{
			Nuage	n1	= mesh1.nuage (i), n2	= mesh2.nuage (i);
			if (n1.nom ( ) != n2.nom ( ))
			{
				str << "Les " << i << "-ièmes nuages n'ont pas les mêmes "
				    << "noms : " << n1.nom( ) << "/" << n2.nom( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (n1.nom ( ) != n2.nom ( ))
			if (n1.nb_noeuds ( ) != n2.nb_noeuds ( ))
			{
				str << "Les nuages " << n1.nom ( )
				    << " n'ont pas le même nombre de noeuds ("
				    << n1.nb_noeuds ( ) << "/" << n2.nb_noeuds ( ) << ").";
				throw XlmLimaException (str.str ( ));
			}	// if (n1.nb_noeuds ( ) != n2.nb_noeuds ( ))
			for (j = 0; j < n1.nb_noeuds ( ); j++)
			{
				if (n1.noeud (j).id ( ) != n2.noeud (j).id ( ))
				{
					str << "Les " << j << "-ièmes noeuds du nuage "
					    << n1.nom ( ) << " n'ont pas les mêmes identifiants ("
					    << n1.noeud (j).id ( ) << " / " << n2.noeud (j).id ( )
					    << ").";
					throw XlmLimaException (str.str ( ));
				}	// if (n1.noeud (j).id ( ) != n2.noeud (j).id ( ))
			}	// for (j = 0; j < n1.nb_noeuds ( ); j++)

			compareAttributes (n1.att_noeuds ( ), n2.att_noeuds ( ));
		}	// for (i = 0; i < mesh1.nb_nuages ( ); i++)

		// Comparaison des lignes :
		for (i = 0; i < mesh1.nb_lignes ( ); i++)
		{
			Ligne	l1	= mesh1.ligne (i), l2	= mesh2.ligne (i);
			if (l1.nom ( ) != l2.nom ( ))
			{
				str << "Les " << i << "-ièmes lignes n'ont pas les mêmes "
				    << "noms : " << l1.nom( ) << "/" << l2.nom( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (l1.nom ( ) != l2.nom ( ))
			if (l1.nb_bras ( ) != l2.nb_bras ( ))
			{
				str << "Les lignes " << l1.nom ( )
				    << " n'ont pas le même nombre de bras ("
				    << l1.nb_bras ( ) << "/" << l2.nb_bras ( ) << ").";
				throw XlmLimaException (str.str ( ));
			}	// if (l1.nb_bras ( ) != l2.nb_bras ( ))
			for (j = 0; j < l1.nb_bras ( ); j++)
			{
				if (l1.bras (j).id ( ) != l2.bras (j).id ( ))
				{
					str << "Les " << j << "-ièmes bras de la ligne "
					    << l1.nom ( ) << " n'ont pas les mêmes identifiants ("
					    << l1.bras (j).id ( ) << " / " << l2.bras (j).id ( )
					    << ").";
					throw XlmLimaException (str.str ( ));
				}	// if (l1.bras (j).id ( ) != l2.bras (j).id ( ))
			}	// for (j = 0; j < l1.nb_bras ( ); j++)

			compareAttributes (l1.att_bras ( ), l2.att_bras ( ));
		}	// for (i = 0; i < mesh1.nb_lignes ( ); i++)

		// Comparaison des surfaces :
		for (i = 0; i < mesh1.nb_surfaces ( ); i++)
		{
			Surface	s1	= mesh1.surface (i), s2	= mesh2.surface (i);
			if (s1.nom ( ) != s2.nom ( ))
			{
				str << "Les " << i << "-ièmes surfaces n'ont pas les mêmes "
				    << "noms : " << s1.nom( ) << "/" << s2.nom( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (s1.nom ( ) != s2.nom ( ))
			if (s1.nb_polygones ( ) != s2.nb_polygones ( ))
			{
				str << "Les surfaces " << s1.nom ( )
				    << " n'ont pas le même nombre de polygones ("
				    << s1.nb_polygones ( ) << "/" << s2.nb_polygones ( ) <<").";
				throw XlmLimaException (str.str ( ));
			}	// if (s1.nb_polygones ( ) != s2.nb_polygones ( ))
			for (j = 0; j < s1.nb_polygones ( ); j++)
			{
				if (s1.polygone (j).id ( ) != s2.polygone (j).id ( ))
				{
					str << "Les " << j << "-ièmes polygones de la surface "
					    << s1.nom ( ) << " n'ont pas les mêmes identifiants ("
					    << s1.polygone (j).id ( ) << " / "
					    << s2.polygone (j).id ( )
					    << ").";
					throw XlmLimaException (str.str ( ));
				}	// if (s1.polygone (j).id ( ) != s2.polygone (j).id ( ))
			}	// for (j = 0; j < s1.nb_polygones ( ); j++)

			compareAttributes (s1.att_polygones ( ), s2.att_polygones ( ));
		}	// for (i = 0; i < mesh1.nb_surfaces ( ); i++)

		// Comparaison des volumes :
		for (i = 0; i < mesh1.nb_volumes ( ); i++)
		{
			Volume	v1	= mesh1.volume (i), v2	= mesh2.volume (i);
			if (v1.nom ( ) != v2.nom ( ))
			{
				str << "Les " << i << "-ièmes volumes n'ont pas les mêmes "
				    << "noms : " << v1.nom( ) << "/" << v2.nom( ) << ".";
				throw XlmLimaException (str.str ( ));
			}	// if (v1.nom ( ) != v2.nom ( ))
			if (v1.nb_polyedres ( ) != v2.nb_polyedres ( ))
			{
				str << "Les volumes " << v1.nom ( )
				    << " n'ont pas le même nombre de polyèdres ("
				    << v1.nb_polyedres ( ) << "/" << v2.nb_polyedres ( ) <<").";
				throw XlmLimaException (str.str ( ));
			}	// if (v1.nb_polyedres ( ) != v2.nb_polyedres ( ))
			for (j = 0; j < v1.nb_polyedres ( ); j++)
			{
				if (v1.polyedre (j).id ( ) != v2.polyedre (j).id ( ))
				{
					str << "Les " << j << "-ièmes polyèdres de la volume "
					    << v1.nom ( ) << " n'ont pas les mêmes identifiants ("
					    << v1.polyedre (j).id ( ) << " / "
					    << v2.polyedre (j).id ( )
					    << ").";
					throw XlmLimaException (str.str ( ));
				}	// if (v1.polyedre (j).id ( ) != v2.polyedre (j).id ( ))
			}	// for (j = 0; j < v1.nb_polyedres ( ); j++)

			compareAttributes (v1.att_polyedres ( ), v2.att_polyedres ( ));
		}	// for (i = 0; i < mesh1.nb_volumes ( ); i++)

		if (mesh1.titre ( ) != mesh2.titre ( ))
		{
			str << "Seuls les titres sont différents (" << mesh1.titre ( )
			    << " / " << mesh2.titre ( ) << ").";
			throw XlmLimaException (str.str ( ));
		}	// if (mesh1.titre ( ) != mesh2.titre ( ))

		cout << "Les maillages " << getInputFile ( ) << " et "
		     << getInputFile2 ( ) << " sont identiques." << endl;
	}
	catch (const XlmLimaException& xlimaError)
	{	
		raiseLimaError (xlimaError.what ( ));
	}
	catch (const erreur& limaError)
	{	
		raiseLimaError (limaError);
	}
	catch (const exception& exc)
	{
		raiseUnexpectedError (exc.what ( ));
	}
	catch (...)
	{
		raiseUnexpectedError ("Erreur non documentée.");
	}
}	// XlmLimaComparer::raiseLimaError


END_NAMESPACE_XLMLIMA
