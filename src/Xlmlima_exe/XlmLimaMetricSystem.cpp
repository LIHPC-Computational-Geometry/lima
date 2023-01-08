#include "XlmLima/XlmLimaMetricSystem.h"
#include "XlmLima/XlmLimaException.h"

#include <Lima/maillage.h>
#include <Lima/noeud.h>

#include <functional>	// equal_to


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA

XlmLimaMetricSystem::XlmLimaMetricSystem (
				const string& inputFile, format_t inputFormat,
		        const string& outputFile, format_t outputFormat, UNIT unit)
	: XlmLimaAction (inputFile, inputFormat), 
	  _outputFile (outputFile), _outputFormat (outputFormat), _unit (unit)
{
}	// XlmLimaMetricSystem::XlmLimaMetricSystem


XlmLimaMetricSystem::~XlmLimaMetricSystem ( )
{
}	// XlmLimaMetricSystem::~XlmLimaMetricSystem


void XlmLimaMetricSystem::run ( )
{
	try
	{
		Maillage	mesh;
		mesh.lire (getInputFile ( ), getInputFormat ( ));
		UNIT	currentUnit	= coefToUnit (mesh.unite_longueur ( ));
		if (currentUnit != getUnit ( ))
		{
			double	factor	= multiplier (currentUnit, getUnit ( ));
			for (size_type n = 0; n < mesh.nb_noeuds ( ); n++)
			{
				Noeud	node	= mesh.noeud (n);
				mesh.deplacer (node.id ( ), factor * node.x ( ),
				               factor * node.y ( ), factor * node.z ( ));
			}	// for (size_type n = 0; n < mesh.nb_noeuds ( ); n++)
			mesh.unite_longueur (limaMultiplier (getUnit ( )));
		}	// if (currentUnit != getUnit ( ))
		mesh.ecrire (getOutputFile ( ), getOutputFormat ( ));
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
}	// XlmLimaMetricSystem::raiseLimaError


XlmLimaMetricSystem::UNIT XlmLimaMetricSystem::coefToUnit (double coef)
{
	equal_to<double>	equal;

	if (true == equal (coef, 1.))
		return XlmLimaMetricSystem::METER;
	if (true == equal (coef, 1.E-2))
		return XlmLimaMetricSystem::CENTIMETER;
	if (true == equal (coef, 1.E-3))
		return XlmLimaMetricSystem::MILLIMETER;
	if (true == equal (coef, 1.E-6))
		return XlmLimaMetricSystem::MICROMETER;

	throw XlmLimaException ("Système d'unité non supporté.");
}	// XlmLimaMetricSystem::coefToUnit


double XlmLimaMetricSystem::multiplier (UNIT unit1, UNIT unit2)
{
	switch (unit1)
	{
		case METER		:
			switch (unit2)
			{
				case METER		: return 1.;
				case CENTIMETER	: return 1.E2;
				case MILLIMETER	: return 1.E3;
				case MICROMETER	: return 1.E6;
			}	// switch (unit2)
			break;
		case CENTIMETER	:
			switch (unit2)
			{
				case METER		: return 1.E-2;
				case CENTIMETER	: return 1.;
				case MILLIMETER	: return 1.E1;
				case MICROMETER	: return 1.E4;
			}	// switch (unit2)
			break;
		case MILLIMETER	:
			switch (unit2)
			{
				case METER		: return 1.E-3;
				case CENTIMETER	: return 1.E-1;
				case MILLIMETER	: return 1.;
				case MICROMETER	: return 1.E3;
			}	// switch (unit2)
			break;
		case MICROMETER	:
			switch (unit2)
			{
				case METER		: return 1.E-6;
				case CENTIMETER	: return 1.E-4;
				case MILLIMETER	: return 1.E-3;
				case MICROMETER	: return 1.;
			}	// switch (unit2)
			break;
	}	// switch (unit1)

	throw XlmLimaException ("XlmLimaMetricSystem::multiplier : erreur interne.");
}	// XlmLimaMetricSystem::multiplier


double XlmLimaMetricSystem::limaMultiplier (UNIT unit)
{
	switch (unit)
	{
		case METER		: return 1.;
		case CENTIMETER	: return 1.E-2;
		case MILLIMETER	: return 1.E-3;
		case MICROMETER	: return 1.E-6;
	}	// switch (unit)

	throw XlmLimaException ("XlmLimaMetricSystem::limaMultiplier : erreur interne.");
}	// XlmLimaMetricSystem::limaMultiplier


END_NAMESPACE_XLMLIMA
