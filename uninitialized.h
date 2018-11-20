#ifndef UNINITIALIZED_H_INCLUDED
#define UNINITIALIZED_H_INCLUDED

#include "iterator.h"
#include "construct.h"
#include "type_traits.h"
#include "algobase.h"
#include <cstring>

namespace TinySTL
{
    /*******************uninitialized_fill_n*****************/
    //是POD型别
    template<class ForwardIterator,class Size,class T>
    inline ForwardIterator
    _uninitialized_fill_n_aux(ForwardIterator first,Size n,T& value,_true_type) {
        return std::fill_n(first, n, value);
    }

    //不是POD型别
    template<class ForwardIterator,class Size,class T>
    inline ForwardIterator
    _uninitialized_fill_n_aux(ForwardIterator first,Size n,T& value,_false_type) {
        ForwardIterator cur = first;
        for( ; n > 0 ; --n, ++cur)
            construct(&*cur,value);
        return cur;
    }

    template <class ForwardIterator,class Size,class T,class T1>
    inline ForwardIterator _uninitialized_fill_n(ForwardIterator first,
                                                 Size n,T& value,T1*) {
        using is_POD = typename _type_traits<T1>::is_POD_type;
        return _uninitialized_fill_n_aux(first,n,value,is_POD());
    }

     template <class ForwardIterator,class Size,class T>
     inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
                                                 Size n,const T& value) {
        return _uninitialized_fill_n(first,n,value,value_type(first));
    }
    /********************************************************/

    /********************uninitialized_copy******************/
    //是POD型别
    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first,InputIterator last,
                            ForwardIterator result,_true_type) {
        return std::copy(first,last,result);
    }

    //不是POD型别
    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    _uninitialized_copy_aux(InputIterator first,InputIterator last,
                            ForwardIterator result,_false_type) {
        InputIterator cur = result;
        for( ; first != last; ++first, ++cur) {
            construct(&*cur,*first);
        }
        return cur;
    }

    //针对const char*的特化版本
    inline char* uninitialized_copy(const char* first,const char* last,
                                    char* result) {
        memmove(result,first,last - first);
        return result + (last - first);
    }

    //针对const wchar_t*的特化版本
    inline wchar_t* uninitialized_copy(const wchar_t* first,const wchar_t* last,
                                       wchar_t* result) {
        memmove(result,first,last - first);
        return result + (last - first);
    }

    template <class InputIterator,class ForwardIterator,class T>
    inline ForwardIterator
    _uninitialized_copy(InputIterator first,InputIterator last,
                        ForwardIterator result,T*) {
        using is_POD = typename _type_traits<T>::is_POD_type;
        return _uninitialized_copy_aux(first,last,result,is_POD());
    }

    template <class InputIterator,class ForwardIterator>
    inline ForwardIterator
    uninitialized_copy(InputIterator first,InputIterator last,ForwardIterator result) {
        return _uninitialized_copy(first,last,result,value_type(result));
        //利用value_type()取出first的value type
    }

    /*****************************************************************/

    /************************uninitialized_fill***********************/
    //是POD型别
    template <class ForwardIterator,class T>
    inline void
    _uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,
                            const T& value,_true_type) {
        std::fill(first,last,value);
    }

    //不是POD型别
    template <class ForwardIterator,class T>
    inline void
    _uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,
                            const T& value,_false_type) {
        ForwardIterator cur = first;
        for(; cur != last; ++cur) {
            construct(&*cur,value);
        }
    }

    template <class ForwardIterator,class T,class T1>
    inline void _uninitialized_fill(ForwardIterator first,ForwardIterator last,
                                    const T& value,T1*) {
        using is_POD = typename _type_traits<T1>::is_POD_type;
        _uninitialized_fill_aux(first,last,value,is_POD());
    }

    template <class ForwardIterator,class T>
    inline void uninitialized_fill(ForwardIterator first,ForwardIterator last,const T& value) {
        _uninitialized_fill(first,last,value,value_type(first));
    }
    /***********************************************************/
}

#endif // UNINITIALIZED_H_INCLUDED
