/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : version.C
 Cree le : 04/01/2005
 Derniere modification le :
 Auteur : C. Pignerol
******************************************************************************/

/**
 * @file	version.C
 * @brief	Classe représentant une version.
 */

#include <Lima/version.h>
#include <Lima/erreur.h>
#include "LimaP/MutableString.h"

#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA


Version::Version (const string& version)
	: m_major (0), m_minor (0), m_release (0)
{
	stringToValues (version, m_major, m_minor, m_release);
}	// Version::Version


Version::Version (const Version& version)
	: m_major (version.getMajor ( )), m_minor (version.getMinor ( )),
	  m_release (version.getRelease ( ))
{
}	// Version::Version


Version& Version::operator = (const Version& version)
{
	if (&version != this)
	{
		m_major		= version.getMajor ( );
		m_minor		= version.getMinor ( );
		m_release	= version.getRelease ( );
	}

	return *this;
}	// Version::operator =


unsigned short Version::getMajor ( ) const
{
	return m_major;
}	// Version::getMajor


unsigned short Version::getMinor ( ) const
{
	return m_minor;
}	// Version::getMinor


unsigned short Version::getRelease ( ) const
{
	return m_release;
}	// Version::getRelease


string Version::getVersion ( ) const
{
	MutableString	version;
	version << getMajor ( ) << "." << getMinor ( ) << "." << getRelease ( );

	return version.str ( );
}	// Version::getVersion


bool Version::operator == (const Version& version) const
{
	if ((version.getMajor ( ) == getMajor ( )) && 
	    (version.getMinor ( ) == getMinor ( )) &&
	    (version.getRelease ( ) == getRelease ( )))
		return true;

	return false;
}	// Version::operator ==


bool Version::operator != (const Version& version) const
{
	return !(*this == version);
}	// Version::operator !=


bool Version::operator > (const Version& version) const
{
	if (getMajor ( ) < version.getMajor ( ))
		return false;
	if (getMajor ( ) > version.getMajor ( ))
		return true;

	// getMajor ( ) == version.getMajor ( )
	if (getMinor ( ) < version.getMinor ( ))
		return false;
	if (getMinor ( ) > version.getMinor ( ))
		return true;

	// getMinor ( ) == version.getMinor ( )
	return getRelease ( ) > version.getRelease ( ) ? true : false;
}	// Version::operator >


bool Version::operator < (const Version& version) const
{
	if (*this >= version)
		return false;

	return true;
}	// Version::operator <


bool Version::operator >= (const Version& v) const
{
	if ((*this > v) || (*this == v))
		return true;

	return false;
}	// Version::operator >=


bool Version::operator <= (const Version& version) const
{
	if ((*this < version) || (*this == version))
		return true;

	return false;
}	// Version::operator <=


void Version::stringToValues (
		const string& version, unsigned short& major, unsigned short& minor,
		unsigned short& release)
{
	char		dot1, dot2;
	istrstream	stream (version.c_str ( ));
	stream >> major >> dot1 >> minor >> dot2 >> release;
	if ((dot1 != '.') || (dot2 != '.') || (true == stream.fail ( )) ||
	    (true == stream.bad ( )))
		throw erreur ("Numero de version invalide.");
}	// Version::stringToValues

END_NAMESPACE_LIMA
