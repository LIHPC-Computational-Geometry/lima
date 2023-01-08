#ifndef XLMLIMA_EXCEPTION_H
#define XLMLIMA_EXCEPTION_H

#include "XlmLima/xlmlima_config.h"

#include <string>


BEGIN_NAMESPACE_XLMLIMA


/** Classe ne derivant pas de std::exception afin d'eviter des conflits avec
la structure exception de math.h, vtk sous OSF, ... */
class XlmLimaException
{
	public :

	XlmLimaException (const IN_STD string& what)
		: _what (what)
	{ }

	XlmLimaException (const XlmLimaException& exc)
		: _what (exc._what)
	{ }

	XlmLimaException& operator = (const XlmLimaException& exc)
	{
		if (&exc != this)
			_what	= exc._what;

		return *this;
	}

	virtual ~XlmLimaException ( )
	{ }

	virtual const IN_STD string& what ( ) const
	{ return _what; }


	protected :

	virtual void setMessage (IN_STD string& message)
	{ _what = message; }


	private :

	IN_STD string		_what;
};	// class XlmLimaException


END_NAMESPACE_XLMLIMA


#endif	// XLMLIMA_EXCEPTION_H
