#include "XlmLima/XlmLimaCopier.h"
#include "XlmLima/XlmLimaException.h"

#include <Lima/maillage.h>


USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA

XlmLimaCopier::XlmLimaCopier (const string& inputFile, format_t inputFormat,
                              const string& outputFile, format_t outputFormat)
	: XlmLimaAction (inputFile, inputFormat), 
	  _outputFile (outputFile), _outputFormat (outputFormat)
{
}	// XlmLimaCopier::XlmLimaCopier


XlmLimaCopier::~XlmLimaCopier ( )
{
}	// XlmLimaCopier::~XlmLimaCopier


void XlmLimaCopier::run ( )
{
	try
	{
		Maillage	mesh;
		mesh.lire (getInputFile ( ), getInputFormat ( ));
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
}	// XlmLimaCopier::raiseLimaError


END_NAMESPACE_XLMLIMA
