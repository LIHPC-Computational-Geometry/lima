#include "XlmLima/XlmLimaAssistant.h"
#include "XlmLima/XlmLimaException.h"

#include <Lima/maillage.h>


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA


XlmLimaAssistant::XlmLimaAssistant (
	const string& inputFile, format_t inputFormat, preparation_t prepa, 
	const string& outputFile, format_t outputFormat)
	: XlmLimaAction (inputFile, inputFormat), 
	  _outputFile (outputFile), _outputFormat (outputFormat),
	  _prepaType (UNITARY), _prepa (prepa), _prepa1 (0), _prepa2 (0)
{
}	// XlmLimaAssistant::XlmLimaAssistant


XlmLimaAssistant::XlmLimaAssistant (
	const string& inputFile, format_t inputFormat,
	TypeMasque prepa1, TypeMasque prepa2, 
	const string& outputFile, format_t outputFormat)
	: XlmLimaAction (inputFile, inputFormat), 
	  _outputFile (outputFile), _outputFormat (outputFormat),
	  _prepaType (ELEMENTARY),
	  _prepa (MINIMALE), _prepa1 (prepa1), _prepa2 (prepa2)
{
}	// XlmLimaAssistant::XlmLimaAssistant


XlmLimaAssistant::XlmLimaAssistant (
	const string& inputFile, format_t inputFormat,
	preparation_t prepa, TypeMasque prepa1, 
	TypeMasque prepa2, const string& outputFile, format_t outputFormat)
	: XlmLimaAction (inputFile, inputFormat), 
	  _outputFile (outputFile), _outputFormat (outputFormat),
	  _prepaType (UNITARY_THEN_ELEMENTARY),
	  _prepa (prepa), _prepa1 (prepa1), _prepa2 (prepa2)
{
}	// XlmLimaAssistant::XlmLimaAssistant


XlmLimaAssistant::~XlmLimaAssistant ( )
{
}	// XlmLimaAssistant::~XlmLimaAssistant


void XlmLimaAssistant::run ( )
{
	try
	{
		Maillage::precision_distance_noeuds (getPrecision ( ));

		Maillage	mesh;
		mesh.lire (getInputFile ( ), getInputFormat ( ));

		switch (_prepaType)
		{
			case UNITARY					:
				mesh.preparer (_prepa);
				break;
			case ELEMENTARY					:
				if (0 == _prepa1)
					throw XlmLimaException ("Erreur : préparation nulle.");
				mesh.preparation_parametrable (_prepa1);
				if (0 != _prepa2)
					mesh.preparation_parametrable (_prepa2);
				break;
			case UNITARY_THEN_ELEMENTARY	:
				mesh.preparer (_prepa);
				if (0 == _prepa1)
					throw XlmLimaException ("Erreur : préparation nulle.");
				mesh.preparation_parametrable (_prepa1);
				if (0 != _prepa2)
					mesh.preparation_parametrable (_prepa2);
				break;
			default							:
				throw XlmLimaException ("Erreur interne : type de préparation non supporté.");
		}	// switch (_prepaType)

		if (0 != getOutputFile ( ).length ( ))
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
}	// XlmLimaAssistant::raiseLimaError


END_NAMESPACE_XLMLIMA
