#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "alloc.h"
#include "iterator.h"
#include "construct.h"
#include "uninitialized.h"

namespace TinySTL
{
    /***********************heap算法*********************/
    //***************make heap(O(n))***************
    template<class RandomAccessIterator, class Compare>
    //上溯算法
    void heap_up(RandomAccessIterator first, RandomAccessIterator last,
                 RandomAccessIterator head, Compare cmp) {
        if(first != last) {
            int index = last - head;
            auto parentIndex = (index - 1) / 2;
            for(auto cur = last; parentIndex >= 0 && cur != head; parentIndex = (index - 1) / 2) {
                auto parent = head + parentIndex;
                if(cmp(*parent, *cur))
                    TinySTL::swap(*parent, *cur);
                cur = parent;
                index = cur - head;
            }
        }
    }

    template<class RandomAccessIterator, class Compare>
    //下溯算法
    void heap_down(RandomAccessIterator first, RandomAccessIterator last,
                   RandomAccessIterator head, Compare cmp) {
        if(first != last) {
            int index = first - head;
            int leftchildIndex = 2 * index + 1;
            for(auto cur = first; leftchildIndex < last - head + 1 && cur < last; leftchildIndex = 2 * index + 1) {
                auto child = head + leftchildIndex;
                if((child + 1) <= last && *child < *(child + 1))
                    child += 1;
                if(cmp(*cur, *child))
                    TinySTL::swap(*cur, *child);
                cur = child;
                index = cur - head;
            }
        }
    }

    template<class RandomAccessIterator, class Compare>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
        if(last - first < 2)
            return;
        auto len = last - first;
        for(auto cur = first + len / 2 - 1; cur >= first; --cur) {
            heap_down(cur, last, first, cmp);
            if(cur == first)
                return;
        }
    }

    //默认make_heap,大根堆
    template<class RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
        make_heap(first, last,
                  std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }

    //********************push_heap(O(logN))*****************
    template<class RandomAccessIterator, class Compare>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
        heap_up(first, last - 1, first, cmp);
    }

    template<class RandomAccessIterator>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
        push_heap(first, last,
                  std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }

    //********************pop_heap(O(logN))********************
    template<class RandomAccessIterator, class Compare>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
        swap(*first, *(last - 1));
        if(last - first >= 2)
        heap_down(first, last - 2, first, cmp);
    }

    template<class RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
        pop_heap(first, last,
                 std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }

    //*********************sort_heap(O(N*logN))
    template<class RandomAccessIterator, class Compare>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
        for(auto cur = last; cur != first; --cur)
            pop_heap(first, cur, cmp);
    }

    template<class RandomAccessIterator>
    void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
        sort_heap(first, last,
                  std::less<typename iterator_traits<RandomAccessIterator>::value_type>());
    }
}

#endif // ALGORITHM_H_INCLUDED
