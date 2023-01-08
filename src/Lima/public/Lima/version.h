#ifndef _VERSION_H
#define _VERSION_H

#include "config.h"


#ifndef SWIG
BEGIN_NAMESPACE_LIMA
#endif

  
/** Classe décrivant la version d'un maillage.
 */
class Version
{
	public :

	/** Version sous forme majeur.mineur.release */
	Version (const IN_STD string& version);

	/** Constructeur de copie. RAS. */
	Version (const Version&);

	/** Opérrateur =. RAS. */
	Version& operator = (const Version&);

	/** Divers accesseurs. */
	unsigned short getMajor ( ) const;
	unsigned short getMinor ( ) const;
	unsigned short getRelease ( ) const;
	IN_STD string getVersion ( ) const;

	/** Opérateurs de comparaison. */
	bool operator == (const Version&) const;
	bool operator != (const Version&) const;
	bool operator > (const Version&) const;
	bool operator < (const Version&) const;
	bool operator >= (const Version&) const;
	bool operator <= (const Version&) const;

	/** Conversion chaine -> majeur/mineur/release. */
	static void stringToValues (const IN_STD string& v, unsigned short& major,
								unsigned short& minor, unsigned short& release);


	private :

	unsigned short		m_major, m_minor, m_release;
};	// class Version


#ifndef SWIG
END_NAMESPACE_LIMA
#endif


#endif	// _VERSION_H

