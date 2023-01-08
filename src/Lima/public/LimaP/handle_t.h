#ifndef _HANDLE_T_H
#define _HANDLE_T_H

#include <Lima/handle.h>
#include "attribut_it.h"
#include "noeud_it.h"
#include "bras_it.h"
#include "polygone_it.h"
#include "polyedre_it.h"

BEGIN_NAMESPACE_LIMA

template <class T>
//inline
_Handle<T>::_Handle(T* ptr):
  m_ptr(ptr)
{
  if(ptr)
    ptr->ref_plus();
}
  
template <class T>
//inline
_Handle<T>::~_Handle()
{
  if(m_ptr){
    m_ptr->ref_moins();
    if(m_ptr->orphelin())
      delete m_ptr;
  }
}
  
template <class T>
//inline
_Handle<T>::_Handle(const _Handle<T>& h):
  m_ptr(h.m_ptr)
{
  if(m_ptr)
    m_ptr->ref_plus();
}
  
template <class T>
//inline
const _Handle<T>&  _Handle<T>::operator = (const _Handle<T>& h)
{
  if(h.m_ptr)
    h.m_ptr->ref_plus();
  if(m_ptr){
    m_ptr->ref_moins();
    if(m_ptr->orphelin())
      delete m_ptr;
  }
  m_ptr = h.m_ptr;

  return *this;
}

template <class T>
//inline
bool _Handle<T>::operator == (const _Handle<T>& h)
{
  return m_ptr == h.m_ptr;
}

END_NAMESPACE_LIMA

#endif
