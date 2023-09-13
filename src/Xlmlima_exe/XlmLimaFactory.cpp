#include "XlmLima/XlmLimaFactory.h"
#include "XlmLima/XlmLimaException.h"
#include "Lima/enum.h"

#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <assert.h>
#include <iostream>


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA

XlmLimaAction* XlmLimaFactory::createAction (int argc, char* argv [])
{
	if (0 == argc)
		syntax ( );

	const string	action	= argv [0];

	if (action == "-help")
		syntax ( );

	string		inputFile ("");
	string		outputFile ("");
	format_t	inputFormat	= SUFFIXE;
	format_t	outputFormat	= SUFFIXE;
	if (action == "-cp")
	{
		switch (argc)
		{
			case	3	:
				inputFile	= argv [1];
				outputFile	= argv [2];
				break;
			case	5	:
				inputFormat	= stringToFormat (argv [1]);
				outputFormat= stringToFormat (argv [3]);
				inputFile	= argv [2];
				outputFile	= argv [4];
				break;
			default		:
				syntax ( );
		}	// switch (argc)

		return createCopier (inputFile, inputFormat, outputFile, outputFormat);
	}	// if (action == "-cp")

	if (action == "-qte")
	{
		switch (argc)
		{
			case	2	:
				inputFile	= argv [1];
				break;
			case	3	:
				inputFormat	= stringToFormat (argv [1]);
				inputFile	= argv [2];
				break;
			default		:
				syntax ( );
		}	// switch (argc)

		return createQuantifier (inputFile, inputFormat);
	}	// if (action == "-qte")

	if (action == "-prepa")
	{
		double	precision	= DBL_EPSILON;

		switch (argc)
		{
			case	3	:	// -prepa prepa infile
				inputFile	= argv [2];
				break;
			case	4	:	// -prepa prepa format infile
							// -prepa prepa infile outfile
				try
				{
					inputFormat	= stringToFormat (argv [2]);
					inputFile	= argv [3];
				}
				catch (...)
				{
					inputFile	= argv [2];
					outputFile	= argv [3];
				}
				break;
			case	6	:	// -prepa prepa format infile format outfile
							// -prepa prepa -precision precision format infile
							// -prepa prepa -precision precision infile outfile
				if (0 == strcmp (argv [2], "-precision"))
				{
					precision	= stringToDouble (argv [3]);

					try
					{
						inputFormat	= stringToFormat (argv [4]);
						inputFile	= argv [5];
					}
					catch (...)
					{	// => -prepa prepa infile outfile -precision precision
						inputFile	= argv [4];
						outputFile	= argv [5];
					}
				}	// if (0 == strcmp (argv [2], "-precision"))
				else
				{	// => -prepa prepa format infile format outfile
					inputFormat	= stringToFormat (argv [2]);
					outputFormat= stringToFormat (argv [4]);
					inputFile	= argv [3];
					outputFile	= argv [5];
				}	// else if (0 == strcmp (argv [4], "-precision"))
				break;
			case	8	:
				// -prepa prepa -precision p format infile format outfile
				if (0 != strcmp (argv [2], "-precision"))
					break;
				precision	= stringToDouble (argv [3]);
				inputFormat	= stringToFormat (argv [4]);
				outputFormat= stringToFormat (argv [6]);
				inputFile	= argv [5];
				outputFile	= argv [7];
				break;
			default		:
				syntax ( );
		}	// switch (argc)
		const string	prepa (argv [1]);

		XlmLimaAssistant*	assistant	=
					createAssistant (inputFile, inputFormat, prepa, outputFile,
					                 outputFormat);
		assert (0 != assistant);
		assistant->setPrecision (precision);

		return assistant;
	}	// if (action == "-prepa")

	if ((action == "-m") || (action == "-cm") || (action == "-mm") ||
	    (action == "-um"))
	{
		XlmLimaMetricSystem::UNIT	unit	= 
							stringToMetricSystem (action.substr (1));
		switch (argc)
		{
			case	3	:
				inputFile	= argv [1];
				outputFile	= argv [2];
				break;
			case	5	:
				inputFormat	= stringToFormat (argv [1]);
				outputFormat= stringToFormat (argv [3]);
				inputFile	= argv [2];
				outputFile	= argv [4];
				break;
			default		:
				syntax ( );
		}	// switch (argc)

		return createMetricSystem (inputFile, inputFormat, unit,
		                           outputFile, outputFormat);
	}	// if (action == "-cp")

	if (action == "-cmp")
	{
		string		inputFile2 ("");
		format_t	inputFormat2	= SUFFIXE;

		switch (argc)
		{
			case	3	:
				inputFile	= argv [1];
				inputFile2	= argv [2];
				break;
			case	5	:
				inputFormat	= stringToFormat (argv [1]);
				inputFile	= argv [2];
				inputFormat2= stringToFormat (argv [3]);
				inputFile2	= argv [4];
				break;
			default		:
				syntax ( );
		}	// switch (argc)

		return new XlmLimaComparer (
								inputFile, inputFormat, inputFile2, inputFormat2);
	}	// if (action == "-cmp")



	syntax ( );	// -> Exception
	return 0;
}	// XlmLimaFactory::createAction


const string& XlmLimaFactory::getVersion ( )
{
	static const string	version (XLMLIMA_VERSION);	// v 7.2.0
	return version;
}	// XlmLimaFactory::getVersion


XlmLimaCopier* XlmLimaFactory::createCopier (
	const string& infile, format_t inputFormat, const string& outfile,
	format_t outputFormat)
{
	return new XlmLimaCopier (infile, inputFormat, outfile, outputFormat);
}	// XlmLimaFactory::createCopier


XlmLimaQuantifier* XlmLimaFactory::createQuantifier (
								const string& infile, format_t format)
{
	return new XlmLimaQuantifier (infile, format);
}	// XlmLimaFactory::createQuantifier


XlmLimaAssistant* XlmLimaFactory::createAssistant (
	const string& infile, format_t inputFormat, const string& prepa, 
	const string& outfile, format_t outputFormat)
{
	// Cas particulier : la preparation codea-d3d non supportee par Lima :
	if (prepa == "codea-d3d")
	{
		TypeMasque	mask1	= 0LL,	mask2	= 0LL;
		mask1	|= LM_RAZ_POLYGONES_BRAS;
		mask1	|= LM_RAZ_BRAS_POLYGONES;
		mask1	|= LM_RAZ_POLYEDRES_POLYGONES;
		mask1	|= LM_RAZ_POLYGONES_POLYEDRES;
		mask2	|= LM_SUPPRIMER_DOUBLONS_BRAS;
		mask2	|= LM_SUPPRIMER_DOUBLONS_POLYGONES;
		mask2	|= LM_SUPPRIMER_DOUBLONS_POLYEDRES;
		mask2	|= LM_POLYGONES_BRAS;
		mask2	|= LM_BRAS_POLYGONES;
		mask2	|= LM_POLYEDRES_POLYGONES;
		mask2	|= LM_POLYGONES_POLYEDRES;
		mask2	|= LM_COMPACTE;
		return new XlmLimaAssistant (infile, inputFormat, CODE_A, 
		                             mask1, mask2, outfile, outputFormat);
	}	// if (prepa == "codea-d3d")

	if (prepa == "noeuds")
	{
		TypeMasque	mask	= LM_SUPPRIMER_DOUBLONS_NOEUDS | LM_COMPACTE;
		return new XlmLimaAssistant (
						infile, inputFormat, mask, 0, outfile, outputFormat);
	}	// if (prepa == "noeuds")

	preparation_t	preparation	= stringToPreparation (prepa);
	return new XlmLimaAssistant (infile, inputFormat, preparation, outfile, 
	                             outputFormat);
}	// XlmLimaFactory::createAssistant


XlmLimaMetricSystem*  XlmLimaFactory::createMetricSystem (
						const string& infile, format_t inputFormat, 
						XlmLimaMetricSystem::UNIT unit,
						const string& outfile, format_t outputFormat)
{
	return new XlmLimaMetricSystem (infile, inputFormat, 
	                                outfile, outputFormat, unit);
}	// XlmLimaFactory::createMetricSystem


void XlmLimaFactory::syntax ( )
{	
	string	message ("xlmlima");
	message	+= string (" version ") + getVersion ( ) + string (" (Lima version ");
	message	+= string (lima_version ( )) + string (")\n");
	message += string ("\n\n");
	message	+= string ("Usage :\n");
	message	+= string ("xlmlima");
	message	+= string (" -qte [format] fichier\n");
	message	+= string ("        \tpour consulter les tailles\n");
	message	+= string ("        -prepa preparation [-precision precision] ");
	message	+= string ("[format] fichier_entrée [format] [fichier_sortie]");
	message	+= string ("\n        \tpour préparer\n");
	message	+= string ("        -cp [format] fichier_entrée [format] fichier_sortie");
	message	+= string ("\n        \tpour copier\n");
	message	+= string ("        -cmp [format] fichier1 [format] fichier2");
	message	+= string ("\n        \tpour comparer deux maillages\n");
	message	+= string ("        -m [format] fichier_entrée [format] fichier_sortie");
	message	+= string ("\n        \tpour exprimer les coordonnées en mètres\n");
	message	+= string ("        -cm [format] fichier_entrée [format] fichier_sortie");
	message	+= string ("\n        \tpour exprimer les coordonnées en centimètres\n");
	message	+= string ("        -mm [format] fichier_entrée [format] fichier_sortie");
	message	+= string ("\n        \tpour exprimer les coordonnées en millimètres\n");
	message	+= string ("        -um [format] fichier_entrée [format] fichier_sortie");
	message	+= string ("\n        \tpour exprimer les coordonnées en micromètres\n");
	message	+= string ("\n");
	message	+= string ("Formats en lecture     : ");
	message += string ("auto ") + readableFormats ( );
//	message	+= string ("auto mali mli mliz mli2 ideas gibi castem modulef icemcfd");
//	message	+= string (" dyna2d dyna3d abaqus");
	message	+= string ("\n");
	message	+= string ("Formats en écriture    : ");
	message += string ("auto ") + writableFormats ( );
//	message	+= string (" auto mali mli mliz mli2 ideas gibi gibi_i8 castem modulef");
	message	+= string ("\n");
	message += string ("auto : format déterminé par l'extension du fichier.");
	message	+= string ("\n");
	message	+= string ("Préparations        :");
	message	+= string (" minimale maximale codeb codea mailleur2d codet");
	message	+= string (" partitionnement3d lissage3d codea-d3d noeuds");
	message	+= string ("\n");
	message	+= string ("La préparation noeuds fusionne les noeuds confondus.");
	message	+= string (" On peut y adjoindre une précision sur la distance ");
	message	+= string ("entre les noeuds à fusionner.");
	message	+= string ("\n\n");
	message	+= string ("Remarque : certains formats ne sont accessibles que ");
	message	+= string ("sur stations Sun, Compaq ou stations SGI.");
	message	+= string ("\n\n");
	message	+= string ("Exemple : xlmlima -cp es6.unv es6.unf");
	message	+= string ("\n\n");

	throw XlmLimaException (message);
}	// XlmLimaFactory::syntax


double XlmLimaFactory::stringToDouble (const string& str)
{
	char*	ptr	= (char*)str.c_str ( );
	char*	end	= ptr;
	double	val	= strtod (ptr, &end);

	if (0 != *end)
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en réel double précision.";
		throw XlmLimaException (error);
	}	// if (0 != *end)

	return val;
}	// XlmLimaFactory::stringToDouble


preparation_t XlmLimaFactory::stringToPreparation (const string& prepa)
{
	if (prepa == "minimale")
		return MINIMALE;
	if (prepa == "maximale")
		return MAXIMALE;
	if (prepa == "codeb")
		return CODE_B;
	if (prepa == "codea")
		return CODE_A;
	if (prepa == "mailleur2d")
		return MAILLEUR_2D;
	if (prepa == "codet")
		return CODE_T;
	if (prepa == "partionnement3d")
		return PARTITIONNEMENT_3D;
	if (prepa == "lissage3d")
		return LISSAGE_3D;

	string message ("Préparation inconnue : ");
	message	+= prepa;
	throw XlmLimaException (message);
}	// XlmLimaFactory::stringToPreparation


format_t XlmLimaFactory::stringToFormat (const string& format)
{
	if (format == "auto")
		return SUFFIXE;
#ifdef __INTERNE_MALI
	if (format == "mali")
		return MALI;
#endif	// __INTERNE_MALI
#ifdef __INTERNE_MALIPP
	if (format == "mli")
		return MALIPP;
#endif	// __INTERNE_MALIPP
#ifdef __INTERNE_MALIPP2
	if (format == "mli2")
		return MALIPP2;
#endif	// __INTERNE_MALIPP2
#ifdef __INTERNE_IDEAS
	if (format == "ideas")
		return IDEAS;
//	if (format == "ideas_2d")	// Refuse par Lima !
//		return IDEAS_2D;
//	if (format == "ideas_v6")	// Refuse par Lima !
//		return IDEAS_V6;
#endif	// __INTERNE_IDEAS
#ifdef __INTERNE_GIBI
	if (format == "gibi")
		return GIBI;
	if (format == "castem")
		return CASTEM;
	if (format == "gibi_i8")
		return GIBI_I8;
#endif	// __INTERNE_GIBI
#ifdef __INTERNE_MODULEF
	if (format == "modulef")
		return MODULEF;
#endif	// __INTERNE_MODULEF
#ifdef __INTERNE_ICEMCFD
	if (format == "icemcfd")
		return ICEMCFD;
#endif	// __INTERNE_ICEMCFD
#ifdef __INTERNE_MAZE
	if (format == "dyna2d")
		return MAZE;
#endif	// __INTERNE_MAZE
#ifdef __INTERNE_INGRID
	if (format == "dyna3d")
		return INGRID;
#endif	// __INTERNE_INGRID
#ifdef __INTERNE_ABAQUS
	if (format == "abaqus")
		return ABAQUS;
#endif	// __INTERNE_ABAQUS

	string message ("Format inconnu : ");
	message	+= format;
	throw XlmLimaException (message);
}	// XlmLimaFactory::stringToFormat


XlmLimaMetricSystem::UNIT XlmLimaFactory::stringToMetricSystem (const string& system)
{
	if (system == string ("m"))
		return XlmLimaMetricSystem::METER;
	if (system == string ("cm"))
		return XlmLimaMetricSystem::CENTIMETER;
	if (system == string ("mm"))
		return XlmLimaMetricSystem::MILLIMETER;
	if (system == string ("um"))
		return XlmLimaMetricSystem::MICROMETER;
	string	message	("Unité inconnue : ");
	message	+= system;
	throw XlmLimaException (message);
}	// XlmLimaFactory::stringToMetricSystem


string XlmLimaFactory::readableFormats ( )
{
	string	formats;
	
#ifdef __INTERNE_MALI
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mali (*.unf)";
#endif	// __INTERNE_MALI
#ifdef __INTERNE_MALIPP
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mli (*.mli)";
#endif	// __INTERNE_MALIPP
#ifdef __INTERNE_MALIPP2
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mli2 (*.mli2)";
#endif	// __INTERNE_MALIPP2
#ifdef __INTERNE_IDEAS
	if (0 != formats.length ( )) formats += " ";
	formats	+= "ideas (*.unv)";
#endif	// __INTERNE_IDEAS
#ifdef __INTERNE_GIBI
	if (0 != formats.length ( )) formats += " ";
	formats	+= "gibi (*.gib) castem (*.cas)";
#endif	// __INTERNE_GIBI
#ifdef __INTERNE_MODULEF
	if (0 != formats.length ( )) formats += " ";
	formats	+= "modulef (*.nop)";
#endif	// __INTERNE_MODULEF
#ifdef __INTERNE_ICEMCFD
	if (0 != formats.length ( )) formats += " ";
	formats	+= "icemcfd (*.ice *.uns)";
#endif	// __INTERNE_ICEMCFD
#ifdef __INTERNE_MAZE
	if (0 != formats.length ( )) formats += " ";
	formats	+= "maze (*.maz)";
#endif	// __INTERNE_MAZE
#ifdef __INTERNE_INGRID
	if (0 != formats.length ( )) formats += " ";
	formats	+= "ingrid (*.ing)";
#endif	// __INTERNE_INGRID
#ifdef __INTERNE_ABAQUS
	if (0 != formats.length ( )) formats += " ";
	formats	+= "abaqus (*.inp)";
#endif	// __INTERNE_ABAQUS

	return formats;
}	// XlmLimaFactory::readableFormats


string XlmLimaFactory::writableFormats ( )
{
	string	formats;
	
#ifdef __INTERNE_MALI
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mali (*.unf)";
#endif	// __INTERNE_MALI
#ifdef __INTERNE_MALIPP
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mli (*.mli)";
#endif	// __INTERNE_MALIPP
#ifdef __INTERNE_MALIPP2
	if (0 != formats.length ( )) formats += " ";
	formats	+= "mli2 (*.mli2)";
#endif	// __INTERNE_MALIPP2
#ifdef __INTERNE_IDEAS
	if (0 != formats.length ( )) formats += " ";
	formats	+= "ideas (*.unv)";
#endif	// __INTERNE_IDEAS
#ifdef __INTERNE_GIBI
	if (0 != formats.length ( )) formats += " ";
	formats	+= "gibi (*.gib) castem (*.cas)";
#endif	// __INTERNE_GIBI
#ifdef __INTERNE_MODULEF
	if (0 != formats.length ( )) formats += " ";
	formats	+= "modulef (*.nop)";
#endif	// __INTERNE_MODULEF

	return formats;
}	// XlmLimaFactory::writeableFormats

END_NAMESPACE_XLMLIMA
