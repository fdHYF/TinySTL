#ifndef LISTTEST_CPP_INCLUDED
#define LISTTEST_CPP_INCLUDED

#include "ListTest.h"
#include <sys/time.h>
#include <list>

namespace TinySTL
{
    namespace ListTest
    {
        //性能测试：TinySTL::list<int> push_back 100000(10万)times
        void test1() {
            TinySTL::list<int> tinyList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 100000; ++i)
                tinyList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "tinyList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "tinyList's size = " << tinyList.size() << std::endl;
        }

        //性能测试：std::list<int> push_back 100000(10万)times
        void test2() {
            std::list<int> stdList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 100000; ++i)
                stdList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "stdList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "stdList's size = " << stdList.size() << std::endl;
        }

        //性能测试：TinySTL::list<int> push_back 1000000(100万)times
        void test3() {
            TinySTL::list<int> tinyList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                tinyList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "tinyList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "tinyList's size = " << tinyList.size() << std::endl;
        }

        //性能测试：std::list<int> push_back 1000000(100万)times
        void test4() {
            std::list<int> stdList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                stdList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "stdList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "stdList's size = " << stdList.size() << std::endl;
        }

        //性能测试：TinySTL::list<int> push_back 10000000(1000万)times
        void test5() {
            TinySTL::list<int> tinyList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                tinyList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "tinyList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "tinyList's size = " << tinyList.size() << std::endl;
        }

        //性能测试：std::list<int> push_back 10000000(1000万)times
        void test6() {
            std::list<int> stdList;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expired1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                stdList.push_back(i);
            gettimeofday(&now, nullptr);
            long long expired2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expired2 - expired1;
            std::cout << "tinyList used " << used << "ms to push_back(int) 100000 times" << std::endl;
            std::cout << "tinyList's size = " << stdList.size() << std::endl;
        }

        void runAll() {
            test1();
            test2();
            test3();
            test4();
            test5();
            test6();
        }
    }
}

#endif // LISTTEST_CPP_INCLUDED
