/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : validateur.C
 Cree le : Fri Jun 14 13:29:50 MEST 2002
 Derniere modification le : Fri Nov 15 13:08:01 MET 2002
 Auteur  : C Pignerol
******************************************************************************/

/*!   
  \file validateur.C
  \brief Utilitaire de verification de maillage.
*/

#include "LimaP/validateur.h"
#include "LimaP/MutableString.h"
#include <Lima/erreur.h>
#include "LimaP/config_it.h"

BEGIN_NAMESPACE_LIMA


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verification_parametrable (const _MaillageInterne& mail, 
                                             LM_V_TYPEMASQUE masque)
{
	MutableString	message;
	bool		ok	= true;

	if ((LM_V_BRAS & masque) &&
	    (!((LM_V_BRAS_NON_NULS & masque) || (LM_V_ORIENTATION_BRAS & masque) ||
	       (LM_V_CONNEXIONS_POLYGONES_BRAS & masque)))
	   )
	{
		try
		{
			verifier_bras (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_BRAS_NON_NULS & masque)
	{
		try
		{
			verifier_bras_non_nuls (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_ORIENTATION_BRAS & masque)
	{
		try
		{
			verifier_orientations_bras (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_CONNEXIONS_POLYGONES_BRAS & masque)
	{
		try
		{
			verifier_connexions_polygones_bras (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_STRUCTURER_LIGNES & masque)
	{
		try
		{
			verifier_structuration_lignes (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if ((LM_V_CONTOURS_SURFACES & masque) &&
	    (!((LM_V_STRUCT_CONTOURS_SURF & masque) || 
	       (LM_V_ORIENT_CONTOURS_SURF & masque))))
	{
		try
		{
			verifier_extraction_contours_surfaces (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if ((LM_V_STRUCT_CONTOURS_SURF & masque) &&
	    (!(LM_V_ORIENT_CONTOURS_SURF & masque)))
	{
		try
		{
			verifier_structuration_contours_surfaces (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_ORIENT_CONTOURS_SURF & masque)
	{
		try
		{
			verifier_orientation_contours_surfaces (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_ORIENTATION_POLYGONES & masque)
	{
		try
		{
			verifier_orientation_polygones (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}
	if (LM_V_COMPACTE & masque)
	{
		try
		{
			verifier_compactage_numerotation (mail);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	} 

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verification_parametrable


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_compactage_numerotation_noeuds (
		const _MaillageInterne& mail)
{
	_MaillageInterne::const_iterator_noeud	noeuds	= mail.premier_noeud ( );
	for (size_type ref = 0; 
	     (ref < mail.nb_noeuds ( )) && (mail.dernier_noeud ( ) != noeuds); 
	     ref++, noeuds++)
	{
		if ((ref + 1) != (*noeuds)->id ( ))
		{
			MutableString	message;
			message << "La numerotation des noeuds n'a pas ete "
			        << "compactee. Erreur au " << (ref + 1) 
			        << "-ieme noeud dont l'identifiant est " 
			        << (*noeuds)->id ( );
			throw erreur (message.str ( ));
		}	// if (ref != (*noeuds)->id ( ))
	}	// for (size_type ref = 0; (ref < mail.nb_noeuds ( )) && ...
}	// _Validateur::verifier_compactage_numerotation_noeuds


//////////////////////////////////////////////////////////////////////////////
const _PolygoneInterne* _Validateur::polygone_gauche (const _BrasInterne& bras)
{
	verifier_bras (bras);

	const _NoeudInterne*	n1	= bras.noeud (0);
	const _NoeudInterne*	n2	= bras.noeud (1);
	for (size_type p = 0; p < bras.nb_polygones ( ); p++)
	{
		const _PolygoneInterne*	polygone	= bras.polygone (p);

		// Le bras est-il oriente dans le meme sens que l'arrete correspondante
		// de la maille ?
		for (size_type n = 0; n < polygone->nb_noeuds ( ); n++)
		{
			if ((n1 == polygone->noeud (n)) && 
			    (n2 == polygone->noeud ((n + 1) % polygone->nb_noeuds ( ))))
				return polygone;
		}	// for (size_type n = 0; n < polygone->nb_noeuds ( ); n++)
	}	// for (size_type p = 0; p < bras->nb_polygones ( ); p++)

	MutableString	message;
	message << "Le bras " << bras.id ( ) 
	        << " n'a pas de polygone a sa gauche.";
	throw erreur (message.str ( ));
}	// _Validateur::polygone_gauche


//////////////////////////////////////////////////////////////////////////////
const _PolygoneInterne* _Validateur::polygone_droit (const _BrasInterne& bras)
{
	verifier_bras (bras);

	const _NoeudInterne*	n1	= bras.noeud (0);
	const _NoeudInterne*	n2	= bras.noeud (1);
	for (size_type p = 0; p < bras.nb_polygones ( ); p++)
	{
		const _PolygoneInterne*	polygone	= bras.polygone (p);

		// Le bras est-il oriente dans en sens inverse que l'arrete 
		// correspondante de la maille ?
		for (size_type n = 0; n < polygone->nb_noeuds ( ); n++)
		{
			if ((n1 == polygone->noeud (n)) && 
			    (n2 == polygone->noeud (0 == n ? polygone->nb_noeuds ( ) - 1 : 
			                                    n - 1)))
				return polygone;
		}	// for (size_type n = 0; n < polygone->nb_noeuds ( ); n++)
	}	// for (size_type p = 0; p < bras.nb_polygones ( ); p++)

	MutableString	message;
	message << "Le bras " << bras.id ( ) 
	        << " n'a pas de polygone a sa droite.";
	throw erreur (message.str ( ));
}	// _Validateur::polygone_droit


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_bras (const _BrasInterne& bras)
{
	if (2 != bras.nb_noeuds ( ))
	{
		MutableString	message;
		message << "Le bras " << bras.id ( ) << " a " << bras.nb_noeuds ( )
		        << " noeuds.";
		throw erreur (message.str ( ));
	}	// if (2 != bras.nb_noeuds ( ))
}	// _Validateur::verifier_bras (const _BrasInterne& bras)


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_bras_non_nul (const _BrasInterne& bras)
{
	verifier_bras (bras); 

	if (bras.noeud (0) == bras.noeud (1))
	{
		const _NoeudInterne*	noeud	= bras.noeud (0);
		MutableString	message;
		message << "Le bras " << bras.id ( ) 
		        << " est constitue de noeuds identiques (" << noeud->x ( )
		        << ", " << noeud->y ( ) << ").";
		throw erreur (message.str ( ));
	}	// if (bras.noeuds (0) == bras.noeuds (1))
}	// _Validateur::verifier_bras_non_nul (const _BrasInterne& bras)


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_bras (const _BrasInterne& bras)
{
	verifier_bras (bras);
	verifier_connexions_polygones_bras (bras);

	if (2 == bras.nb_polygones ( ))
	{
		const _PolygoneInterne&	polygoneGauche	= *polygone_gauche (bras);
		const _PolygoneInterne&	polygoneDroit	= *polygone_droit (bras);

		if (polygoneGauche.id ( ) >= polygoneDroit.id ( ))
		{
			MutableString	message;
			message << "Le polygone " << polygoneGauche.id ( ) 
			        << " situe a gauche du bras " << bras.id ( ) 
			        << " a un identifiant superieur ou egal au polygone "
			        << " localise a droite de ce bras.";
			throw erreur (message.str ( ));
		}	// if (polygoneGauche.id ( ) >= polygoneDroit.id ( ))
	}	// if (2 == bras.nb_polygones ( ))
}	// _Validateur::verifier_orientation_bras


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_connexions_polygones_bras (const _BrasInterne& bras)
{
	// Un bras est connecte a un ou deux polygones. Si deux, l'un est a sa
	// droite, l'autre a sa gauche.
	size_type	nb_polygones	= bras.nb_polygones ( );
	if ((1 != nb_polygones) && (2 != nb_polygones))
	{
		MutableString	message;
		message << "Le bras " << bras.id ( );
		if (0 == nb_polygones)
			message << " est connecte a aucun polygone.";
		else
		{
			message << " est connecte a " << nb_polygones<< " polygones (";
			for (size_t pol = 0; pol < nb_polygones; pol++)
			{
				message << bras.polygone (pol)->id ( );
				if (nb_polygones - 1 != pol)
					message << ", ";
			}
			message << ").";
		}
		throw erreur (message.str ( ));
	}	// if ((1 != nb_polygones) && (2 != nb_polygones))

	// Si 2 polygones alors 1 de chaque cote :
	if (2 == nb_polygones)
	{
		try
		{
			polygone_gauche (bras);
			polygone_droit (bras);
		}
		catch (const erreur&/* err*/)
		{
			MutableString	message;
			message << "Le bras " << bras.id ( ) 
			        << " est connecte a deux polygones mais ceux-ci "
			        << "sont situes du meme cote.";
			throw erreur (message.str ( ));
		}
	}	// if (2 == nb_polygones)
}	// _Validateur::verifier_connexions_polygones_bras


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_compactage_numerotation_bras (
		const _MaillageInterne& mail)
{
	_MaillageInterne::const_iterator_bras	bras	= mail.premier_bras ( );
	for (size_type ref = 0; 
	     (ref < mail.nb_bras ( )) && (mail.dernier_bras ( ) != bras); 
	     ref++, bras++)
	{
		if ((ref + 1) != (*bras)->id ( ))
		{
			MutableString	message;
			message << "La numerotation des bras n'a pas ete "
			        << "compactee. Erreur au " << (ref + 1) 
			        << "-ieme bras dont l'identifiant est " 
			        << (*bras)->id ( );
			throw erreur (message.str ( ));
		}	// if (ref != (*bras)->id ( ))
	}	// for (size_type ref = 0; (ref < mail.nb_bras ( )) && ...
}	// _Validateur::verifier_compactage_numerotation_bras


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_bras (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_bras bras = mail.premier_bras ( );
	     mail.dernier_bras ( ) != bras; bras++)
	{
		try
		{
			verifier_bras (**bras);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}	
	}	// for (_MaillageInterne::const_iterator_bras bras = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_bras (const _MaillageInterne& mail)


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_bras_non_nuls (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_bras bras = mail.premier_bras ( );
	     mail.dernier_bras ( ) != bras; bras++)
	{
		try
		{
			verifier_bras_non_nul (**bras);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_bras bras = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_bras_non_nuls (const _MaillageInterne& mail)


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientations_bras (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

size_type	nb_bras	= mail.nb_bras ( );
size_type	count	= 0;
	for (_MaillageInterne::const_iterator_bras bras = mail.premier_bras ( );
	     mail.dernier_bras ( ) != bras; bras++, count++)
	{
		try
		{
			verifier_orientation_bras (**bras);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_bras bras = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_orientations_bras (const _MaillageInterne& mail)


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_connexions_polygones_bras (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_bras bras = mail.premier_bras ( );
	     mail.dernier_bras ( ) != bras; bras++)
	{
		try
		{
			verifier_connexions_polygones_bras (**bras);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_bras bras = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_connexions_polygones_bras


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_structuration_ligne (const _LigneInterne& ligne)
{
	size_type	nb_noeuds	= ligne.nb_noeuds ( );
	size_type	nb_bras		= ligne.nb_bras ( );

	if (((nb_noeuds != nb_bras) && ((nb_noeuds - 1) != nb_bras)) ||
	    (0 == nb_bras))
	{
		MutableString	message;
		message << "La ligne " << ligne.nom ( ) << " est constituee de "
		        << nb_bras << " bras et " << nb_noeuds << " noeuds.";
		throw erreur (message.str ( ));
	}	// if ((nb_noeuds != nb_bras) && ((nb_noeuds - 1) != nb_bras))

	for (size_type b = 0; b < nb_bras; b++)
	{
		// Si la ligne est ouverte on ne teste pas le dernier bras :
		if ((nb_noeuds != nb_bras) && (nb_bras - 1 == b))
			break;

		const _BrasInterne&	bras1	= *(ligne.bras (b));
		const _BrasInterne&	bras2	= *(ligne.bras ((b + 1) % nb_bras));
		try
		{
			verifier_bras (bras1);
			verifier_bras (bras2);
		}
		catch (const erreur& err)
		{
			MutableString	message;
			message << "Invalidite d'un bras de la ligne " << ligne.nom ( )
			        << " : " << err.what ( );
			throw erreur (message.str ( ));
		}

		// Ces deux bras doivent avoir un noeud en commun :
		const _NoeudInterne*	noeud11	= bras1.noeud (0);
		const _NoeudInterne*	noeud12 = bras1.noeud (1);
		const _NoeudInterne*	noeud21	= bras2.noeud (0);
		const _NoeudInterne*	noeud22 = bras2.noeud (1);
		if ((noeud11 != noeud21) && (noeud11 != noeud22) && 
		    (noeud12 != noeud21) && (noeud12 != noeud22))
		{
			MutableString	message;
			message << "Les bras " << bras1.id ( ) << " et " << bras2.id ( )
			        << " de la ligne " << ligne.nom ( ) << " sont disjoints." ;
			throw erreur (message.str ( ));
		}	// if ((noeud11 != noeud21) && (noeud11 != noeud22) && ...
	}	// for (size_type b = 0; b < nb_bras; b++)
}	// _Validateur::verifier_structuration_ligne


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_ligne (const _LigneInterne& ligne)
{
	// La ligne doit etre structuree :
	verifier_structuration_ligne (ligne);

	double	surface	= 0.;
	for (size_type n = 0; n < ligne.nb_noeuds ( ); n++)
	{
		const _NoeudInterne&	n1	= *(ligne.noeud (n));
		const _NoeudInterne&	n2	= 
							*(ligne.noeud ((n + 1) % ligne.nb_noeuds ( )));
		surface	+= n1.x ( ) * n2.y ( ) - n1.y ( ) * n2.x ( );
	}	// for (size_type n = 0; n < ligne.nb_noeuds ( ); n++)

	if (surface < 0)
	{
		MutableString	message;	
		message << "La ligne " << ligne.nom ( ) 
		        << " n'est pas orientee dans le sens trigonometrique." ;
		throw erreur (message.str ( ));
	}	// if (surface < 0)
}	// _Validateur::verifier_orientation_ligne


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_structuration_lignes (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_ligne ligne = mail.premier_ligne( );
	     mail.dernier_ligne ( ) != ligne; ligne++)
	{
		try
		{
			verifier_structuration_ligne (**ligne);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_ligne ligne = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_structuration_lignes


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_lignes_fermees (
		const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_ligne ligne = mail.premier_ligne( );
	     mail.dernier_ligne ( ) != ligne; ligne++)
	{
		if ((*ligne)->nb_noeuds ( ) != (*ligne)->nb_bras ( ))
			continue;		// Ligne non fermee (ouverte, ou non structuree)

		try
		{
			verifier_orientation_ligne (**ligne);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_ligne ligne = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_orientation_lignes_fermees


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_polygone (
		const _PolygoneInterne& polygone)
{
	// Test effectue a l'aide de la formule de Green :
	double	surface	= 0.;

	for (size_type n = 0; n < polygone.nb_noeuds ( ); n++)
	{
		const _NoeudInterne&	n1	= *(polygone.noeud (n));
		const _NoeudInterne&	n2	= 
						*(polygone.noeud ((n + 1) % polygone.nb_noeuds ( )));
		surface	+= n1.x ( )*n2.y ( ) - n1.y ( ) * n2.x ( );
	}	// for (size_type n = 0; n < polygone.nb_noeuds ( ); n++)

	if (surface < 0)
	{
		MutableString	message;
		message << "La maille " << polygone.id ( ) 
		        << " n'est pas orientee dans le sens trigonometrique.";
		throw erreur (message.str ( ));
	}	// if (surface < 0)
}	// _Validateur::verifier_orientation_polygone


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_polygones (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_polygone polygone = 
				mail.premier_polygone ( );
	     mail.dernier_polygone ( ) != polygone; polygone++)
	{
		try
		{
			verifier_orientation_polygone (**polygone);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_polygone polygone = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_orientation_polygones


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_compactage_numerotation_polygones (
	const _MaillageInterne& mail)
{
	_MaillageInterne::const_iterator_polygone polygone = 
				mail.premier_polygone ( );
	for (size_type ref = 0; 
	     (ref < mail.nb_polygones ( )) && (mail.dernier_polygone() != polygone);
	      ref++, polygone++)
	{
		if ((ref + 1) != (*polygone)->id ( ))
		{
			MutableString	message;
			message << "La numerotation des polygones n'a pas ete "
			        << "compactee. Erreur au " << (ref + 1) 
			        << "-ieme polygone dont l'identifiant est " 
			        << (*polygone)->id ( );
			throw erreur (message.str ( ));
		}	// if ((ref + 1) != (*polygone)->id ( ))
	}	// for (size_type ref = 0; ...
}	// _Validateur::verifier_compactage_numerotation_polygones


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_extraction_contours_surfaces (const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_surface surface = 
				mail.premier_surface ( );
	     mail.dernier_surface ( ) != surface; surface++)
	{
		try
		{
			const _LigneInterne*	ligne	= (*surface)->contour ( );
			if (0 == ligne)
				throw erreur ("contour non extrait");
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << "Le contour de la surface " << (*surface)->nom ( )
			        << " est invalide : " << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_surface surface = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_extraction_contours_surfaces


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_structuration_contours_surfaces (
		const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_surface surface = 
				mail.premier_surface ( );
	     mail.dernier_surface ( ) != surface; surface++)
	{
		try
		{
			const _LigneInterne*	ligne	= (*surface)->contour ( );
			if (0 == ligne)
				throw erreur ("contour non extrait.");
			verifier_structuration_ligne (*ligne);

			// Un contour est une ligne fermee :
			if (ligne->nb_noeuds ( ) != ligne->nb_bras ( ))
				throw erreur ("ligne non fermee.");
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << "Le contour de la surface " << (*surface)->nom ( )
			        << " n'est pas structure : " << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_surface surface = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_structuration_contours_surfaces


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_orientation_contours_surfaces (
		const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	for (_MaillageInterne::const_iterator_surface surface = 
				mail.premier_surface ( );
	     mail.dernier_surface ( ) != surface; surface++)
	{
		try
		{
			const _LigneInterne*	ligne	= (*surface)->contour ( );
			if (0 == ligne)
				throw erreur ("contour non extrait.");
			verifier_structuration_ligne (*ligne);
			verifier_orientation_ligne (*ligne);
		}
		catch (const erreur& erreur)
		{
			if (false == ok)
				message << "\n";
			message << "Le contour de la surface " << (*surface)->nom ( )
			        <<" n'est pas structure ou oriente dans le sens "
			        << "trigonometrique : " << erreur.what ( );
			ok	= false;
		}
	}	// for (_MaillageInterne::const_iterator_surface surface = ...

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_orientation_contours_surfaces


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_compactage_numerotation_polyedres (
	const _MaillageInterne& mail)
{
	_MaillageInterne::const_iterator_polyedre polyedre = 
				mail.premier_polyedre ( );
	for (size_type ref = 0; 
	     (ref < mail.nb_polyedres( )) && (mail.dernier_polyedre( ) != polyedre);
	     ref++, polyedre++)
	{
		if ((ref + 1) != (*polyedre)->id ( ))
		{
			MutableString	message;
			message << "La numerotation des polyedres n'a pas ete "
			        << "compactee. Erreur au " << (ref + 1) 
			        << "-ieme polyedres dont l'identifiant est " 
			        << (*polyedre)->id ( );
			throw erreur (message.str ( ));
		}	// if (ref != (*polyedre)->id ( ))
	}	// for (size_type ref = 0; ...
}	// _Validateur::verifier_compactage_numerotation_polyedres


//////////////////////////////////////////////////////////////////////////////
void _Validateur::verifier_compactage_numerotation (
		const _MaillageInterne& mail)
{
	MutableString	message;
	bool		ok	= true;

	try
	{
		verifier_compactage_numerotation_noeuds (mail);
	}
	catch (const erreur& erreur)
	{
//		if (false == ok)
//			message << "\n";
		message << erreur.what ( );
		ok	= false;
	}
	try
	{
		verifier_compactage_numerotation_bras (mail);
	}
	catch (const erreur& erreur)
	{
		if (false == ok)
			message << "\n";
		message << erreur.what ( );
		ok	= false;
	}
	try
	{
		verifier_compactage_numerotation_polygones (mail);
	}
	catch (const erreur& erreur)
	{
		if (false == ok)
			message << "\n";
		message << erreur.what ( );
		ok	= false;
	}
	try
	{
		verifier_compactage_numerotation_polyedres (mail);
	}
	catch (const erreur& erreur)
	{
		if (false == ok)
			message << "\n";
		message << erreur.what ( );
		ok	= false;
	}

	if (false == ok)
	{
		throw erreur (message.str ( ));
	}	// if (false == ok)
}	// _Validateur::verifier_compactage_numerotation


END_NAMESPACE_LIMA
