#ifndef MUTABLE_STRING_H
#define MUTABLE_STRING_H

/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : MutableString.h
 Cree le : Fri Nov 15 13:08:01 MET 2002
 Derniere modification le :
 Auteur  : C. Pignerol
******************************************************************************/


#include <Lima/config.h>
#include <stdio.h>


/*!
  \file MutableString.h
  \brief Classe string modifiable afin de remplacer les classes ostrstream
         et ostringstream dans la réalisation de messages, toutes deux
         inutilisables pour cause d'incompatibilités avec différents
         compilateurs.
*/

/*! La classe MutableString propose les services suivants : operateur <<
    surchargé pour les principaux types de base, opérateur de promotion
    automatique en chaîne de caractères. */
class MutableString
{
	public :

	MutableString ( );
	MutableString (const MutableString&);
	MutableString& operator = (const MutableString&);
	MutableString (const IN_STD string&);
	~MutableString ( )
	{ }

	// Acces a la chaine de caracteres :
	operator IN_STD string ( ) const
	{ return _str; }
	operator const char* ( ) const
	{ return _str.c_str ( ); }
	const IN_STD string& str ( ) const
	{ return _str; }

	// Operateurs << :
	MutableString& operator << (const IN_STD string&);
	MutableString& operator << (char);
	MutableString& operator << (unsigned char);
	MutableString& operator << (short);
	MutableString& operator << (unsigned short);
	MutableString& operator << (int);
	MutableString& operator << (unsigned int);
	MutableString& operator << (long);
	MutableString& operator << (unsigned long);
	MutableString& operator << (float);
	MutableString& operator << (double);
	MutableString& operator << (long double);

	private :

	IN_STD string		_str;
};	// class MutableString


inline MutableString::MutableString ( )
	: _str ( )
{
}	// MutableString::MutableString ( )


inline MutableString::MutableString (const MutableString& mstr)
	: _str (mstr._str)
{
}	// MutableString::MutableString (const MutableString& str)


inline MutableString& MutableString::operator = (const MutableString& mstr)
{
	if (&mstr != this)
		_str	= mstr._str;

	return *this;
}	// MutableString::operator = (const MutableString& str)


inline MutableString::MutableString (const IN_STD string& str)
	: _str (str)
{
}	// MutableString::MutableString (const IN_STD string& str)


// Rem : l'operateur += de la classe string est prefere a sa fonction append.
// En effet, dans certains cas (ex : xlC 3.1), la classe string utilisee n'est
// pas standard mais par exemple derivee d'une classe non standard
// (ex : IString) et ne proposant donc pas les services standards.
// L'operateur += semble convenir.
inline MutableString& MutableString::operator << (const IN_STD string& str)
{
	_str	+= str;

	return *this;
}	// MutableString::operator << (const IN_STD string& str)


inline MutableString& MutableString::operator << (char c)
{
	char	buffer [2];
	sprintf (buffer, "%c", c);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (char c)



inline MutableString& MutableString::operator << (unsigned char c)
{
	return operator << ((int)c);
}	// MutableString::operator << (unsigned char c)


inline MutableString& MutableString::operator << (short s)
{
	char	buffer [1024];
	sprintf (buffer, "%hd", s);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (short s)


inline MutableString& MutableString::operator << (unsigned short s)
{
	char	buffer [1024];
	sprintf (buffer, "%hu", s);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (unsigned short s)


inline MutableString& MutableString::operator << (int i)
{
	char	buffer [1024];
	sprintf (buffer, "%d", i);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (int i)


inline MutableString& MutableString::operator << (unsigned int i)
{
	char	buffer [1024];
	sprintf (buffer, "%u", i);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (unsigned int i)


inline MutableString& MutableString::operator << (long l)
{
	char	buffer [1024];
	sprintf (buffer, "%ld", l);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (long l)


inline MutableString& MutableString::operator << (unsigned long l)
{
	char	buffer [1024];
	sprintf (buffer, "%lu", l);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (unsigned long l)


inline MutableString& MutableString::operator << (float f)
{
	return operator << ((double)f);
}	// MutableString::operator << (float f)


inline MutableString& MutableString::operator << (double d)
{
	char	buffer [1024];
	sprintf (buffer, "%.10g", d);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (double d)


inline MutableString& MutableString::operator << (long double d)
{
	char	buffer [1024];
	sprintf (buffer, "%.10Lg", d);
	_str	+= buffer;

	return *this;
}	// MutableString::operator << (long double d)


#endif	// MUTABLE_STRING_H
