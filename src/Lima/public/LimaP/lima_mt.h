#ifndef _LIMA_MT_H
#define _LIMA_MT_H

#include "config_it.h"
#include <stddef.h>


#ifdef LIMA_MT
#include <pthread.h>


BEGIN_NAMESPACE_LIMA


class LimaMutex
{
	public :

	LimaMutex ( );
	~LimaMutex ( );
	void lock ( );
	void unlock ( );

	private :

	LimaMutex (const LimaMutex&);
	LimaMutex& operator = (const LimaMutex&);

	pthread_mutex_t m_mutex;
};  // class LimaMutex


class AutoMutex
{
	public :

	AutoMutex (LimaMutex& mutex)
	: m_mutex (mutex)
	{
		m_mutex.lock ( );
	}   // AutoMutex

	~AutoMutex ( )
	{
		m_mutex.unlock ( );
	}   // ~AutoMutex

	private :

	AutoMutex (const AutoMutex&);
	AutoMutex& operator = (const AutoMutex&);

	LimaMutex&  m_mutex;
};  // class AutoMutex


END_NAMESPACE_LIMA

#if defined(_IBMR2) || defined(__GNUC__) || defined(CRAY) || defined(INCLUDE_TEMPLATE)
#include "lima_mt_t.h"
#endif

#endif	// LIMA_MT


#endif	// _LIMA_MT_H
