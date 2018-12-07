#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

namespace TinySTL
{
    template<class T, class Sequence = deque<T>>
    class stack {
        template<class T1, class Sequence1>
        friend bool operator==(const stack<T1, Sequence1>& lhs, const stack<T1, Sequence1>& rhs);
        template<class T1, class Sequence1>
        friend bool operator!=(const stack<T1, Sequence1>& lhs, const stack<T1, Sequence1>& rhs);
    public:
        using value_type        = typename Sequence::value_type;
        using size_type         = typename Sequence::size_type;
        using reference         = typename Sequence::reference;
        using const_reference   = typename Sequence::const_reference;
    private:
        Sequence c;         //底层容器
    public:
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }
        reference top() { return c.back(); }
        const_reference top() const {return c.back(); }
        void push(const value_type& value) { c.push_back(value); }
        void pop() { c.pop_back(); }
    };
    template<class T, class Sequence>
    bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
        return lhs.c == rhs.c;
    }

    template<class T, class Sequence>
    bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
        return lhs.c != rhs.c;
    }
}

#endif // STACK_H_INCLUDED
