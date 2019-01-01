#ifndef PAIR_H_INCLUDED
#define PAIR_H_INCLUDED

namespace TinySTL
{
    template<class T1, class T2>
    struct pair {
        using first_type = T1;
        using second_type = T2;

        T1 first;
        T2 second;

        pair() : first(T1()), second(T2()) {}
        pair(const T1& a, const T2& b) : first(a), second(b) {}

        pair& operator=(const pair& rhs) {
            first = rhs.first;
            second = rhs.second;
        }
    };
}
#endif // PAIR_H_INCLUDED
