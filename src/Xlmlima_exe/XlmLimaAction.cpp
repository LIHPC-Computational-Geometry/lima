#include "XlmLima/XlmLimaAction.h"
#include "XlmLima/XlmLimaException.h"

#include <float.h>

USING_STD
USING_LIMA


BEGIN_NAMESPACE_XLMLIMA

XlmLimaAction::XlmLimaAction (const string& inputFile, format_t format)
	: _inputFile (inputFile), _inputFormat (format), _precision (DBL_EPSILON)
{
}	// XlmLimaAction::XlmLimaAction


XlmLimaAction::~XlmLimaAction ( )
{
}	// XlmLimaAction::~XlmLimaAction


void XlmLimaAction::setPrecision (double precision)
{
	if (0 >= precision)
		throw XlmLimaException ("La précision doit être strictement positive.");

	_precision	= precision;
}	// XlmLimaAction::setPrecision


void XlmLimaAction::raiseLimaError (const IN_LIMA erreur& limaError)
{
	string	message	= limaErrorString ( ) + limaError.what ( );
	throw XlmLimaException (message);
}	// XlmLimaAction::raiseLimaError


string XlmLimaAction::limaErrorString ( )
{
	return "Erreur signalée par Lima++ : ";
}	// XlmLimaAction::limaErrorString


void XlmLimaAction::raiseUnexpectedError (const string& error)
{
	string	message	= string ("Erreur d'origine inconnue : ") + error;
	throw XlmLimaException (message);
}	// XlmLimaAction::raiseUnexpectedError


END_NAMESPACE_XLMLIMA
