#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

#include <cstddef>
namespace TinySTL
{
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};


    template <class Category,class T,class Distance = ptrdiff_t,
                class Pointer = T*, class Reference = T&>
    struct iterator {
        using iterator_category         = Category;
        using value_type                = T;
        using difference_type           = Distance;
        using pointer                   = Pointer;
        using reference                 = Reference;
    };

    template <class Iterator>
    struct iterator_traits {
        using iterator_category         = typename Iterator::iterator_category;
        using value_type                = typename Iterator::value_type;
        using pointer                   = typename Iterator::pointer;
        using reference                 = typename Iterator::reference;
        using difference_type           = typename Iterator::difference_type;
    };

    //根据原生指针设计的traits偏特化版本
    template <class T>
    struct iterator_traits<T*> {
        using iterator_category         = random_access_iterator_tag;
        using value_type                = T;
        using difference_type           = ptrdiff_t;
        using pointer                   = T*;
        using reference                 = T&;
    };

    template<class T>
    struct iterator_traits<const T*> {
        using iterator_category         = random_access_iterator_tag;
        using value_type                = T;
        using difference_type           = ptrdiff_t;
        using pointer                   = const T*;
        using reference                 = const T&;
    };

    //此函数用于决定某个迭代器的类型
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&) {
        using category = typename iterator_traits<Iterator>::iterator_category;
        return category();
    }

    //用于决定某个迭代器的distance type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    distance_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    //用于决定某个迭代器的value type
    template <class Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator&) {
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    //distance函数，用以计算两迭代器之间的距离
    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last,
              input_iterator_tag) {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first != last) {
            ++first;
            ++n;
        }
        return n;
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    _distance(InputIterator first, InputIterator last,
              random_access_iterator_tag) {
        return last - first;
    }

    template<class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last) {
        using category = typename iterator_traits<InputIterator>::iterator_category;
        return _distance(first, last,category());
    }
}

#endif // ITERATOR_H_INCLUDED
