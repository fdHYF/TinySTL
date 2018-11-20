#ifndef CONSTRUCT_H_INCLUDED
#define CONSTRUCT_H_INCLUDED

#include <new>
#include "type_traits.h"
#include "iterator.h"

namespace TinySTL
{
    template<class T1,class T2>
    inline void construct(T1* p,const T2& value) {
        new(p) T1(value);
    }

    template<class T>
    inline void destroy(T* ptr) {
        ptr->~T();
    }

    //如果元素的数值型别(value type)有non-trivial destructor...
    template<class ForwardIterator>
    inline void
    _destroy_aux(ForwardIterator first,ForwardIterator last, _false_type) {
        for(;first != last;++first)
            destroy(&*first);
    }

    //如果元素型别(value type)具有trival destructor...
    template<class ForwardIterator>
    inline void
    _destroy_aux(ForwardIterator first,ForwardIterator last,_true_type) {}

    template<class ForwardIterator,class T>
    inline void _destroy(ForwardIterator first,ForwardIterator last,T*) {
        using trivial_destructor = typename _type_traits<T>::has_trivial_destructor;
        _destroy_aux(first,last,trivial_destructor());
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first,ForwardIterator last) {
        _destroy(first,last,value_type(first));
    }
}

#endif // CONSTRUCT_H_INCLUDED
