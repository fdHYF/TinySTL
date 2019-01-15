#ifndef VECTORTEST_CPP_INCLUDED
#define VECTORTEST_CPP_INCLUDED

#include "VectorTest.h"
#include <sys/time.h>
#include <cstdlib>
namespace TinySTL
{
    namespace VectorTest
    {
        //基本的push_back和pop_back测试
        void test1() {
            TinySTL::vector<int> tinyVector;
            std::vector<int> stdVector;
            for(int i = 0; i < 10000; ++i) {
                tinyVector.push_back(i);
                stdVector.push_back(i);
            }
            std::cout << "The tinyVector's size is " << tinyVector.size() << std::endl;
            std::cout << "The stdVcetor's size is " << stdVector.size() << std::endl;
            if(Test::CompareContainer(tinyVector, stdVector))
                std::cout << "The test of push_back() work well" << std::endl;
            else
                std::cerr << "There is some error of vector's push_back()" << std::endl;

            size_t size = tinyVector.size();
            while(size > 0) {
                tinyVector.pop_back();
                stdVector.pop_back();
                --size;
            }
            std::cout << "The tinyVector's size is " << tinyVector.size() << std::endl;
            std::cout << "The stdVcetor's size is " << stdVector.size() << std::endl;
            if(tinyVector.empty() && stdVector.empty())
                std::cout << "The test of pop_back() work well" << std::endl;
            else
                std::cerr << "There is some error of vector's pop_back()" << std::endl;
        }

        //性能测试，TinySTL::vector<int> push_back 10000000(1千万)times
        void test2() {
            TinySTL::vector<int> tinyVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                tinyVector.push_back(i);
            gettimeofday(&now, nullptr);
            long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long used = expire2 - expire1;
            std::cout << "TinySTL::vector used " << used << "ms to push_back 1000000 times" << std::endl;
            std::cout << "tinyVector's size is " << tinyVector.size() << " capacity is " << tinyVector.capacity() << std::endl;
        }

        //性能测试，std::vector<int> push_back 10000000(1千万)times
        void test3() {
            std::vector<int> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long  expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                stdVector.push_back(i);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "std::vector used " << used << "ms to push_back 1000000 times" << std::endl;
            std::cout << "stdVcetor's size is " << stdVector.size() << " capacity is " << stdVector.capacity() << std::endl;
        }

        //性能测试，TinySTL::vector<int> push_back 1000000(1百万)times
        void test4() {
            TinySTL::vector<int> tinyVcetor;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                tinyVcetor.push_back(i);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "TinySTL::vector used " << used << "ms to push_back 1000000 times" << std::endl;
            std::cout << "tinyVector's size is" << tinyVcetor.size() << " capacity is" << tinyVcetor.capacity() << std::endl;
        }

        //性能测试，std::vector<int> push_back 1000000(1百万)times
        void test5() {
            std::vector<int> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long  expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                stdVector.push_back(i);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "std::vector used " << used << "ms to push_back 1000000 times" << std::endl;
            std::cout << "stdVcetor's size is " << stdVector.size() << " capacity is " << stdVector.capacity() << std::endl;
        }

        //性能测试，TinySTL::vector<int> push_back 100000(10万)times
        void test6() {
            TinySTL::vector<int> tinyVcetor;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long expire1 = now.tv_sec * 10000 + now.tv_usec / 100;
            for(int i = 0; i < 100000; ++i)
                tinyVcetor.push_back(i);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 10000 + now.tv_usec / 100;
            long long used = expire2 - expire1;
            std::cout << "TinySTL::vector used " << used << "ms to push_back 100000 times" << std::endl;
            std::cout << "tinyVector's size is" << tinyVcetor.size() << " capacity is" << tinyVcetor.capacity() << std::endl;
        }

        //性能测试，std::vector<int> push_back 100000(10万)times
        void test7() {
            std::vector<int> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            long long  expire1 = now.tv_sec * 1000 + now.tv_usec / 100;
            for(int i = 0; i < 100000; ++i)
                stdVector.push_back(i);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 100;
            long long used = expire2 - expire1;
            std::cout << "std::vector used " << used << "ms to push_back 100000 times" << std::endl;
            std::cout << "stdVcetor's size is " << stdVector.size() << " capacity is " << stdVector.capacity() << std::endl;
        }

        //性能测试，TinySTL::vector<std::string> push_back 100000(10万)times
        void test8() {
            TinySTL::vector<std::string> tinyVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";   //“望天”，是一首非常动听的纯音乐
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 100000; ++i)
                tinyVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "TinySTL::vector<std::string> used " << used << "ms to push_back 100000 times" << std::endl;
        }

        //性能测试，std::vector<std::string> push_back 100000(10万)times
        void test9() {
            std::vector<std::string> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 100000; ++i)
                stdVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "std::vector<std::string> used " << used << "ms to push_back 100000 times" << std::endl;
        }

        //性能测试，TinySTL::vector<std::string> push_back 1000000(100万)times
        void test10() {
            TinySTL::vector<std::string> tinyVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";   //“望天”，是一首非常动听的纯音乐
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                tinyVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "TinySTL::vector<std::string> used " << used << "ms to push_back 1000000 times" << std::endl;
        }

        //性能测试，std::vector<std::string> push_back 1000000(100万)times
        void test11() {
            std::vector<std::string> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 1000000; ++i)
                stdVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "std::vector<std::string> used " << used << "ms to push_back 1000000 times" << std::endl;
        }

        //性能测试，TinySTL::vector<std::string> push_back 10000000(1000万)times
        void test12() {
            TinySTL::vector<std::string> tinyVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";   //“望天”，是一首非常动听的纯音乐
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                tinyVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "TinySTL::vector<std::string> used " << used << "ms to push_back 10000000 times" << std::endl;
        }

        //性能测试，std::vector<std::string> push_back 10000000(1000万)times
        void test13() {
            std::vector<std::string> stdVector;
            struct timeval now;
            gettimeofday(&now, nullptr);
            std::string str = "WangTian";
            long long expire1 = now.tv_sec * 1000 + now.tv_usec / 1000;
            for(int i = 0; i < 10000000; ++i)
                stdVector.push_back(str);
            gettimeofday(&now, nullptr);
            long long expire2 = now.tv_sec * 1000 + now.tv_usec / 1000;
            long long used = expire2 - expire1;
            std::cout << "std::vector<std::string> used " << used << "ms to push_back 10000000 times" << std::endl;
        }
        void runAll() {
            //test1();
            //test3();
            //test2();
            //test4();
            //test5();
            //test6();
            //test7();
            test8();
            test9();
            test10();
            test11();
            test12();
            test13();
        }
    }
}

#endif // VECTORTEST_CPP_INCLUDED
