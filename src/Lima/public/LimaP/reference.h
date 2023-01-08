#ifndef _REFERENCE_H
#define _REFERENCE_H

#include "config_it.h"
#include <Lima/size.h>


#ifdef LIMA_MT
#include "lima_mt.h"
#ifndef LIMA_USES_ATOMIC_API
#include <pthread.h>
#else	// LIMA_USES_ATOMIC_API

#	if (((defined (__GNUC__)) && ((__GNUC__) >= 4) && ((__GNUC_MINOR__) >= 3)) || (defined (__clang__))) || \
       ((defined (__GNUC__)) && ((__GNUC__) >= 6))
#include <ext/atomicity.h>
#	else
#include <bits/atomicity.h>
#	endif	// if defined __GNUC__ && ...

#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT

#include <assert.h>



BEGIN_NAMESPACE_LIMA

//! Classe qui tient le compte du nombre de ses références.
/*!
  Afin d'éviter à l'utilisateur de la librairie Lima de manipuler des
  pointeurs et de gérer l'allocation mémoire, les objets disponibles
  dans l'interface sont en fait des smart pointeur qui références les
  objets internes.
  Tous les objets internes héritent de cette classe qui tient le compte
  du nombre de ses références afin d'être détruit en temps voulu.
*/
class _ObjetReference
{
public:
  //! Zéro référence.
  _ObjetReference();

	~_ObjetReference ( );

  //! Incrémente le compteur.
  void ref_plus();
  //! Décrémente le compteur.
  void ref_moins();
  //! Retourne true si le compteur est à zéro.
  bool orphelin();

private:

	_ObjetReference (const _ObjetReference&);
	_ObjetReference& operator = (const _ObjetReference&);
  //! Nombre de références.
#ifndef LIMA_USES_ATOMIC_API
	size_type m_ref;
#else	// LIMA_USES_ATOMIC_API
	_Atomic_word    m_ref;
#endif	// LIMA_USES_ATOMIC_API

	void lock ( );		// Lima mt
	void unlock ( );	// Lima mt
#ifdef LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	LimaMutex			m_mutex;
#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT
};

inline _ObjetReference::_ObjetReference ( )
	: m_ref (0)
#ifdef LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	, m_mutex ( )
#endif	// LIMA_USES_ATOMIC_API
#endif// LIMA_MT
{
}	// _ObjetReference::_ObjetReference


inline _ObjetReference::_ObjetReference (const _ObjetReference&)
	: m_ref (0)
#ifdef LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	, m_mutex ( )
#endif	// LIMA_USES_ATOMIC_API
#endif// LIMA_MT
{
	assert (0 && "_ObjetReference copy constructor is not allowed.");
}	// _ObjetReference::_ObjetReference


inline _ObjetReference& _ObjetReference::operator = (const _ObjetReference&)
{
	assert (0 && "_ObjetReference operator = is not allowed.");
	return *this;
}	// _ObjetReference::operator =


inline _ObjetReference::~_ObjetReference ( )
{
}	// _ObjetReference::~_ObjetReference


inline void _ObjetReference::lock ( )
{
#ifdef LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	m_mutex.lock ( );
#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT
}	// _ObjetReference::lock


inline void _ObjetReference::unlock ( )
{
#ifdef LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	m_mutex.unlock ( );
#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT
}	// _ObjetReference::unlock


inline void _ObjetReference::ref_plus ( )
{
#ifndef LIMA_MT
	++m_ref;
#else	// LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	AutoMutex mutex (m_mutex);
	++m_ref;
#else	// LIMA_USES_ATOMIC_API
	__gnu_cxx::__atomic_add (&m_ref, 1);
#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT
}	// _ObjetReference::ref_plus

inline void _ObjetReference::ref_moins ( )
{
#ifndef LIMA_MT
	--m_ref;
#else	// LIMA_MT
#ifndef LIMA_USES_ATOMIC_API
	AutoMutex mutex (m_mutex);
	--m_ref;
#else	// LIMA_USES_ATOMIC_API
	__gnu_cxx::__exchange_and_add (&m_ref, -1);
#endif	// LIMA_USES_ATOMIC_API
#endif	// LIMA_MT
}	// _ObjetReference::ref_moins

inline
bool _ObjetReference::orphelin()
{
  return m_ref==0;
}

END_NAMESPACE_LIMA

#endif
