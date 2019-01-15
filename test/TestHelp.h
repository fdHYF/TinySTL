#ifndef TESTHELP_H_INCLUDED
#define TESTHELP_H_INCLUDED
#include <string>
#include <iterator>
#include <iostream>

namespace TinySTL
{
    namespace Test
    {
        template<class Container>
        void PrintContainer(Container& container, const std::string& name) {
            std::cout << "Now printf the value of Container " << name << ":" << std::endl;
            for(auto ptr = container.begin(); ptr != container.end(); ++ptr) {
                std::cout << *ptr << " ";
            }
            std::cout << std::endl;
        }

        template<class Container1, class Container2>
        bool CompareContainer(Container1& container1, Container2& container2) {
            auto first1 = std::begin(container1);
            auto first2 = std::begin(container2);
            auto last1 = std::end(container1);
            auto last2 = std::end(container2);

            while(first1 != last1 && first2 != last2) {
                if(*first1 != *first2)
                    return false;
                ++first1;
                ++first2;
            }
            return true;
        }
    }
}

#endif // TESTHELP_H_INCLUDED
