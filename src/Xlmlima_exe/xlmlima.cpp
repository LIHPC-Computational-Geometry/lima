/**
 * Point d'entrée du programme xlmlima.
 */

#include "XlmLima/XlmLimaFactory.h"
#include "XlmLima/XlmLimaException.h"

#ifdef __hpux
#include <iostream.h>
#else
#include <iostream>
#endif	// __hpux
#include <memory>
#include <stdexcept>


USING_STD
USING_XLMLIMA



int main (int argc, char* argv [])
{
	try
	{
		auto_ptr<XlmLimaAction>	action;
		action.reset (XlmLimaFactory::createAction (argc - 1, argv + 1));
		if (0 == action.get ( ))
			throw XlmLimaException ("xlmlima. Erreur interne : absence d'action.");
		action->run ( );
	}
	catch (const XlmLimaException& xlmExc)
	{
		cout << xlmExc.what ( ) << endl;
		return -1;
	}
	catch (const exception& exc)
	{
		cout << exc.what ( ) << endl;
		return -1;
	}	
	catch (...)
	{
		cout << "xlmlima. Erreur non documentée." << endl;
		return -1;
	}

	return 0;
}	// main
