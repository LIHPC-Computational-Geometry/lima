#ifndef _HANDLE_H
#define _HANDLE_H

#include "config.h"
#include "size.h"

BEGIN_NAMESPACE_LIMA

template <class T> class _Handle
{
public:
  operator T* () const { return m_ptr; };
  
protected:
  _Handle(T* ptr);
  _Handle(const _Handle<T>& h);
  ~_Handle();
  
  bool operator == (const _Handle<T>& h);
  const _Handle<T>& operator = (const _Handle<T>& h);

protected:  
  T* m_ptr;
};

END_NAMESPACE_LIMA

#endif
