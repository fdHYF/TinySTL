#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

#include "algorithm.h"

namespace TinySTL
{
    template<class T, class Sequence = vector<T>,
                class Compare = std::less<typename Sequence::value_type>>
    class priority_queue {
    public:
        using value_type      = typename Sequence::value_type;
        using size_type       = typename Sequence::size_type;
        using reference       = typename Sequence::reference;
        using const_reference = typename Sequence::const_reference;

        priority_queue() : c () {}
        explicit priority_queue(const Compare& comp) : c (), cmp(comp) {}
        template<class InputIterator>
        priority_queue(InputIterator first, InputIterator last) :
            c(first, last) { make_heap(first, last); }

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        const_reference top() const { return c.front(); }
        reference top() { return c.front(); }
        size_type capacity()const { return c.capacity(); }
        void push(const value_type& value) {
            try {
                c.push_back(value);
                push_heap(c.begin(), c.end(), cmp);
            }
            catch(...) {
                c.clear();
            }
        }

        void pop() {
            try {
                pop_heap(c.begin(), c.end(), cmp);
                c.pop_back();
            }
            catch(...) {
                c.clear();
            }
        }

    private:
        Sequence c;         //底层容器
        Compare cmp;        //元素大小比较标准
    };
}


#endif // PRIORITY_QUEUE_H_INCLUDED
