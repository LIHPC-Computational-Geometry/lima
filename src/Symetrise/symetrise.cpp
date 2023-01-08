//
// Programme issu des scripts ruby symX0.rb, symY0.rb, symZ0.rb écrits par Eric Brière de l'Isle.
// Effectue une symétrie par rapport à un plan renseigné en argument : yOz (donc x -> -x), 
// xOz (donc y -> -y), xOy (donc z -> -z)
//

// ================================================================================================================================================
// Commentaires du script sym0.rb :
//
// Fait une symétrie par rapport au plan Z=0 du maillage et donne en sortie le maillage complet
//
// Eric Brière de l'Isle, le 31/03/2006
//
// historique: 
// contrairement au script symZ0.rb initial, on donne le maillage complet
// de plus, le maillage n'est pas symétrisé / Y=0, mais les mailles sont crées par symétrie
//
// remarque : les mailles sur le plan de sym sont dupliquées, mais n'apparaissent qu'une fois dans le groupe (les autres ne sont pas référencées)
// ================================================================================================================================================

#include "LimaP/maillage_it.h"
#include "Lima/erreur.h"

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <stdlib.h>
#include <limits>
#include <numeric>

using namespace Lima;
using namespace std;


enum SYM {SYM_X, SYM_Y, SYM_Z};


static bool parseArgs (int argc, char** argv);
static int syntax (const string& pgm);
static string symToString (SYM s);
static _NoeudInterne& noeud (const _MaillageInterne&, map<size_type, size_type>&, size_type id);
static _NoeudInterne& symetrise (const _NoeudInterne& noeud, SYM s);
static _PolyedreInterne& symetrise (const _PolyedreInterne&, map<size_type, size_type>&);
static _PolygoneInterne& symetrise (const _PolygoneInterne&, map<size_type, size_type>&);


static string	entree, sortie, nomPlan;
static double	dist		= 0.;
static bool		verbeux	= false, utiliserDistance	= false;
static SYM		symetrie	= SYM_X;

int main (int argc, char* argv[])
{
	int	retCode	= -1;

	if (false == parseArgs (argc, argv))
		return syntax (argv [0]);
		
	try 
	{
		unique_ptr<_MaillageInterne>	maillage (new _MaillageInterne ( ));
if (true == verbeux) cout << "Chargement du maillage du fichier " << entree << " ..." << endl;
		maillage->lire (entree, SUFFIXE, 1);

		_SurfaceInterne*	plan	= 0;
		if (true == utiliserDistance)
		{
if (true == verbeux) cout << "La distance maximale d'un noeux au plan de symétrie est " << dist << endl;
if (true == verbeux) cout << "Marquage des noeuds de la surface de symétrie ..." << endl;
			const size_type		nbNoeuds	= maillage->nb_noeuds ( );
			for (size_type n = 0; n < nbNoeuds; n++)
			{
				_NoeudInterne*	noeud	= maillage->noeud (n);
				double			val		= std::numeric_limits<double>::max ( );
				switch (symetrie)
				{
					case SYM_X	: val	= noeud->x ( );	break;
					case SYM_Y	: val	= noeud->y ( );	break;
					case SYM_Z	: val	= noeud->z ( );	break;
					default		: throw erreur ("Type de symetrie non géré.");
				}	// switch (symetrie)
				if (fabs (val) < dist)
					noeud->set_udata ((void*)1);	// => A ne pas dupliquer
			}	// for (size_type n = 0; n < nbNoeuds; n++)
		}	// if (true == utiliserDistance)
		else
		{
if (true == verbeux) cout << "Les noeuds du plan de symétrie sont ceux de la surface " << nomPlan << endl;
if (true == verbeux) cout << "Récupération de la surface de symétrie " << nomPlan << " ..." << endl;
			plan	= maillage->surface (nomPlan);
if (true == verbeux) cout << "Marquage des noeuds de la surface de symétrie ..." << endl;
			const size_type		nbPolys	= plan->nb_polygones ( );
			for (size_type p = 0; p < nbPolys; p++)
			{
				_PolygoneInterne*	poly		= plan->polygone (p);
				const size_type		nbNoeuds	= poly->nb_noeuds ( );
				poly->set_udata ((void*)1);
				for (size_type n = 0; n < nbNoeuds; n++)
				{
					_NoeudInterne*	noeud	= poly->noeud (n);
					noeud->set_udata ((void*)1);	// => A ne pas dupliquer
				}	// for (size_type n = 0; n < nbNoeuds; n++)
			}	// for (size_type p = 0; p < nbPolys; p++)		
		}	// else if (true == utiliserDistance)

if (true == verbeux) cout << "Symétrisation " << symToString (symetrie) << " des noeuds du maillage, exception faite ceux du plan de symétrie " << nomPlan << " ..." << endl;
		const size_type		nbNoeuds	= maillage->nb_noeuds ( );
		map<size_type, size_type>	corNoeuds;	// tableau de correspondance entre ancien id et indice des nouveaux noeuds
		for (size_type n = 0; n < nbNoeuds; n++)
		{
			_NoeudInterne*	noeud	= maillage->noeud (n);
			if ((void*)0 == noeud->udata ( ))
			{
				_NoeudInterne*	oppose	= &symetrise (*noeud, symetrie);
				maillage->ajouter (oppose);
				corNoeuds [noeud->id ( )]	= maillage->nb_noeuds ( ) - 1;
			}
			else
				corNoeuds [noeud->id ( )]	= n;
		}	// for (size_type n = 0; n < nbNoeuds; n++)
			
if (true == verbeux) cout << "Symétrisation des polyèdres ..." << endl;
		const size_type		nbPolyedres	= maillage->nb_polyedres ( );
		map<size_type, size_type>	corPolys;	// tableau de correspondance entre ancien id et indice des nouveaux polyèdres
		for (size_type p = 0; p < nbPolyedres; p++)
		{
			_PolyedreInterne*	poly	= maillage->polyedre (p);
			_PolyedreInterne*	oppose	= &symetrise (*poly, corNoeuds);
			maillage->ajouter (oppose);
			corPolys [poly->id ( )]	= maillage->nb_polyedres ( ) - 1;
		}	// for (size_type p = 0; p < nbPolyedres; p++)
if (true == verbeux) cout << "Ajout des polyèdres crées dans leurs volumes ..." << endl;
		const size_type	nbVolumes	= maillage->nb_volumes ( );
		for (size_type v = 0; v < nbVolumes; v++)
		{
			_VolumeInterne*		volume	= maillage->volume (v);
			const size_type		nb		= volume->nb_polyedres ( );
			for (size_type p = 0; p < nb; p++)
			{
				_PolyedreInterne*	poly	= volume->polyedre (p);
				_PolyedreInterne*	oppose	= maillage->polyedre (corPolys [poly->id ( )]);
				volume->ajouter (oppose);
			}	// for (size_type p = 0; p < nb; p++)
		}	// for (size_type v = 0; v < nbVolumes; v++)

if (true == verbeux) cout << "Symétrisation des polygones, exception faite ceux du plan de symétrie " << nomPlan << " ..." << endl;
		const size_type		nbPolygones	= maillage->nb_polygones ( );
		corPolys.clear ( );	// tableau de correspondance entre ancien id et indice des nouveaux polygones
		for (size_type p = 0; p < nbPolygones; p++)
		{
			_PolygoneInterne*	poly	= maillage->polygone (p);
			if (true == utiliserDistance)
			{
				const size_type		nbNoeuds	= poly->nb_noeuds ( );
				void*				doit		= (void*)1;
				for (size_type n = 0; n < nbNoeuds; n++)
				{
					_NoeudInterne*	noeud	= poly->noeud (n);
					if ((void*)0 == noeud->udata ( ))	// Si un noeud n'est pas sur le plan alors le polygone n'y est pas.
					{
						doit	= (void*)0;
						break;
					}	// if ((void*)0 == noeud->udata ( ))
				}	// for (size_type n = 0; n < nbNoeuds; n++)
				poly->set_udata (doit);
			}	// if (true == utiliserDistance)
			if ((void*)0 == poly->udata ( ))
			{
				_PolygoneInterne*	oppose	= &symetrise (*poly, corNoeuds);
				maillage->ajouter (oppose);
				corPolys [poly->id ( )]	= maillage->nb_polygones ( ) - 1;
			}	// if ((void*)0 == poly->udata ( ))
		}	// for (size_type p = 0; p < nbPolygones; p++)
if (true == verbeux) cout << "Ajout des polygones crées dans leurs surfaces ..." << endl;
		const size_type	nbSurfaces	= maillage->nb_surfaces ( );
		for (size_type v = 0; v < nbSurfaces; v++)
		{
			_SurfaceInterne*		surface	= maillage->surface (v);
			if (surface->nom ( ) != nomPlan)
			{
				const size_type		nb		= surface->nb_polygones ( );
				for (size_type p = 0; p < nb; p++)
				{
					_PolygoneInterne*	poly	= surface->polygone (p);
					if ((void*)0 == poly->udata ( ))
					{
						_PolygoneInterne*	oppose	= maillage->polygone (corPolys [poly->id ( )]);
						surface->ajouter (oppose);
					}	// if ((void*)0 == poly->udata ( ))
				}	// for (size_type p = 0; p < nb; p++)
			}	// if (surface->nom ( ) != nomPlan)
		}	// for (size_type v = 0; v < nbSurfaces; v++)

if (true == verbeux) cout << "Enregistrement du maillage symétrisé dans le fichier " << sortie << " ..." << endl;
		maillage->ecrire (sortie, SUFFIXE, 1);
if (true == verbeux) cout << "Enregistrement du maillage symétrisé dans le fichier " << sortie << " effectué." << endl;
		
		retCode	= 0;
	}
	catch (const erreur& exc)
	{
		cerr << "Lima reporte l'erreur suivante : " << exc.what() << endl;
	}
	catch (const exception& e)
	{
		cerr << "Exception : " << e.what ( ) << endl;
	}
	catch (...)
    {
		cerr << "Exception inconnue " << endl;
	}

	return retCode;
}	// main


static bool parseArgs (int argc, char** argv)
{
	int	i	= 1;
	if (argc == 1)
		return false;
	if ((string (argv [i]) == "-help") || (string (argv [i]) == "-h") || (string (argv [i]) == "--h"))
		return false;
	if ((string (argv [i]) == "-v") || (string (argv [i]) == "--v"))
	{
		verbeux	= true;
		i++;
	}	// if ((string (argv [i]) == "-v") || (string (argv [i]) == "--v"))
	if (4 != argc - i)
		return false;

	const string	sym (argv [i++]);
	if (sym == "X")
		symetrie	= SYM_X;
	else if (sym == "Y")
		symetrie	= SYM_Y;
	else if (sym == "Z")
		symetrie	= SYM_Z;
		
	entree	= argv [i++];
	nomPlan	= argv [i++];
	sortie	= argv [i++];

	// Plan ou distance ?
	char*	ptr		= (char*)nomPlan.c_str ( );
	char*	endptr	= ptr;
	dist			= strtod (ptr, &endptr);
	if ('\0' == *endptr)
	{
		utiliserDistance	= true;
		dist				= fabs (dist);
	}	// if ('\0' == *endptr)

	return true;
}	// parseArgs


static int syntax (const string& pgm)
{
	cerr << "Syntaxe : pgm [-v] sym fichier_depart surface_ou_distance fichier_sortie" << endl
	     << "où sym peut prendre les valeurs X, Y, Z selon que la symétrisation est à faire selon l'axe X, l'axe Y ou l'axe Z." << endl
	     << "Surface est la surface dont les noeuds ne sont pas dupliqués, donc a priori le plan de symétrie, ou distance est la distance maximale d'éloignement d'un noeud au plan de symétrie." << endl
	     << "Lima version " << lima_version ( ) << endl;
	
	return -1;
}	// syntax


static string symToString (SYM s)
{
	switch (s)
	{
		case SYM_X	: return "x -> -x";
		case SYM_Y	: return "y -> -y";
		case SYM_Z	: return "z -> -z";
	}	// switch (s)
	
	throw erreur ("Type de symetrie non géré.");
}	// symToString


static _NoeudInterne& noeud (const _MaillageInterne& maillage, map<size_type, size_type>& corNoeuds, size_type id)
{
	return *(maillage.noeud (corNoeuds [id]));
}	// noeud


static _NoeudInterne& symetrise (const _NoeudInterne& noeud, SYM s)
{
	_NoeudInterne*	n	= 0;
	
	switch (s)
	{
		case SYM_X	: n	= _NoeudInterne::create (-noeud.x ( ), noeud.y ( ), noeud.z ( ));	break;
		case SYM_Y	: n	= _NoeudInterne::create (noeud.x ( ), -noeud.y ( ), noeud.z ( ));	break;
		case SYM_Z	: n	= _NoeudInterne::create (noeud.x ( ), noeud.y ( ), -noeud.z ( ));	break;
		default		: throw erreur ("Type de symétrisation non géré.");
	}	// switch (s)
	
	return *n;
}	// symetrise (const _NoeudInterne& noeud, SYM s)


static _PolyedreInterne& symetrise (const _PolyedreInterne& poly, map<size_type, size_type>& corNoeuds)
{
	_PolyedreInterne*		symetrique	= 0;
	const _MaillageInterne*	maillage	= poly.maillage ( );
	switch (poly.nb_noeuds ( ))
	{
		case	4	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )));
			break;
		case	5	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )));
			break;
		case	6	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (5)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )));
			break;
		case	8	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (5)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (6)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (7)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )));
			break;
		case	10	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (5)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (6)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (7)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (8)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (9)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( ))); 										
			break;
			case	12	: symetrique	= _PolyedreInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (6)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (7)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (8)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (9)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (10)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (11)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (5)->id ( )));
			break;
		default	: throw erreur ("Type de polyèdre non géré.");
	}	// switch (poly.nb_noeuds ( ))
	
//	if (symetrique->volume ( ) < 0)
//		symetrique->retourner ( );
		
	return *symetrique;
}	// symetrise (const _PolyedreInterne&, map<size_type, size_type>&)


static _PolygoneInterne& symetrise (const _PolygoneInterne& poly, map<size_type, size_type>& corNoeuds)
{
	_PolygoneInterne*		symetrique	= 0;
	const _MaillageInterne*	maillage	= poly.maillage ( );
	switch (poly.nb_noeuds ( ))
	{
		case	3	: symetrique	= _PolygoneInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )));
			break;
		case	4	: symetrique	= _PolygoneInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )));
			break;
		case	5	: symetrique	= _PolygoneInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )));
			break;
		case	6	: symetrique	= _PolygoneInterne::create (
										&noeud (*maillage, corNoeuds, poly.noeud (2)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (1)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (0)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (5)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (4)->id ( )),
										&noeud (*maillage, corNoeuds, poly.noeud (3)->id ( )));
			break;

		default	: throw erreur ("Type de polygonee non géré.");
	}	// switch (poly.nb_noeuds ( ))
	
//	if (symetrique->surface ( ) < 0)
//		symetrique->retourner ( );
		
	return *symetrique;
}	// symetrise (const _PolygoneInterne&, map<size_type, size_type>&)


