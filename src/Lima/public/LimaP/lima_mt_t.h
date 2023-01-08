#ifndef _LIMA_MT_T_H
#define _LIMA_MT_T_H

#ifdef LIMA_MT

#include <assert.h>


BEGIN_NAMESPACE_LIMA

inline LimaMutex::LimaMutex ( )
{
	pthread_mutexattr_t attr;
	int ok  = pthread_mutexattr_init (&attr);
	pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init (&m_mutex, &attr);
	pthread_mutexattr_destroy (&attr);
}   // LimaMutex::LimaMutex


inline LimaMutex::LimaMutex (const LimaMutex&)
{
	assert (0 && "LimaMutex copy constructor is not allowed.");
}	// LimaMutex::LimaMutex


inline LimaMutex& LimaMutex::operator = (const LimaMutex&)
{
	assert (0 && "LimaMutex operator = is not allowed.");
	return *this;
}	// LimaMutex::::operator =


inline LimaMutex::~LimaMutex ( )
{
	pthread_mutex_destroy (&m_mutex);
}	// LimaMutex::~LimaMutex


inline void LimaMutex::lock ( )
{
	pthread_mutex_lock (&m_mutex);
}	// LimaMutex::lock


inline void LimaMutex::unlock ( )
{
	pthread_mutex_unlock (&m_mutex);
}	// LimaMutex::unlock



END_NAMESPACE_LIMA


#endif	// LIMA_MT

#endif	// _LIMA_MT_T_H
