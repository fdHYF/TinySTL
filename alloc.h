#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED

#include "construct.h"
#include<cstdlib>
namespace TinySTL
{
    /**
     * SGI STL的第一级配置器
     * template<int inst>
     * class _malloc_alloc_template {...};
     * 其中：
     * 1.allocate()直接使用malloc(),
     * deallocate()直接使用free()。
     * 2.模拟C++的set_new_handler()处理内存不足的情况。
     */
    template<int inst>
    class _malloc_alloc_template {
    private:
        //oom:out of memory.
        static void* oom_malloc(size_t);
        static void* oom_realloc(void*,size_t);
        static void (* _malloc_alloc_oom_handler) ();

    public:
        static void* allocate(size_t n);
        static void deallocate(void* ptr,size_t /* n*/);
        static void* reallocate(void* ptr,size_t /* old_sz */,size_t new_sz);
        static void (* set_malloc_handler(void (*f)())) (){
            void (* old) () = _malloc_alloc_oom_handler;
            _malloc_alloc_oom_handler = f;
            return(old);
        }
    };

    using malloc_alloc = _malloc_alloc_template<0>;


    /**
     *  SGI STL二级配置器
     *  template<bool threads,int inst>
     *  class _default_alloc_template {...};
     *  其中：
     *  1.维护16个自由链表，负责16种小型区块的次配置能力。
     *  2.如果需求大于128bytes,转调用第一级配置器。
     *
     */
    enum {_ALLGN = 8};                         //小型区块的上调边界
    enum {_MAX_BYTES = 128};                   //小型区块的上限
    enum {_NFREELISTS = _MAX_BYTES / _ALLGN};  //free-lists个数

    template<bool threads,int inst>
    class _default_alloc_template {
    public:
        static void* allocate(size_t n);
        static void deallocate(void* ptr,size_t n);
        static void* reallocate(void* ptr,size_t old_sz,size_t new_sz);
    private:
        union obj {     //free-lists节点构造
            union obj* free_list_link;
            char client_data[1];
        };

        //将bytes上调至8的倍数
        static size_t ROUND_UP(size_t bytes) {return (((bytes)+_ALLGN-1) & ~(_ALLGN-1));}

        static obj* volatile free_list[_NFREELISTS];
        //更据区块大小确定使用第n号free-list
        static size_t FREELIST_INDEX(size_t bytes) {
            return (((bytes) + _ALLGN - 1)/_ALLGN - 1);
        }
        static void* refill(size_t n);
        //配置一大块空间，可容纳nobjs个大小为size的区块
        //如果配置不方便，nobjs会有所下降
        static char* chunk_alloc(size_t size,int& nobjs);

        static char* start_free;
        static char* end_free;
        static size_t heap_size;
    };
    template<bool threads,int inst>
    char* _default_alloc_template<threads,inst>::start_free = nullptr;

    template<bool threads,int inst>
    char* _default_alloc_template<threads,inst>::end_free = nullptr;

    template<bool threads,int inst>
    size_t _default_alloc_template<threads,inst>::heap_size = 0;

    template<bool threads,int inst>
    typename _default_alloc_template<threads,inst>::obj* volatile
    _default_alloc_template<threads,inst>::free_list[_NFREELISTS] =
    {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,
     nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,};



    typedef _default_alloc_template<0,0> alloc;


    //包装，使Alloc具有标准接口
    template<class T,class Alloc = alloc>
    class simple_alloc{
    public:
        static T *allocate(size_t n)
                {return 0==n?0:(T*)Alloc::allocate(n*sizeof(T));}
        static T *allocate(void)
                {return (T*)Alloc::allocate(sizeof(T));}
        static void deallocate(T *p,size_t n)
                {if(0!=n) Alloc::deallocate(p,n*sizeof(T));}
        static void deallocate(T *p)
                {Alloc::deallocate(p,sizeof(T));}
    };
}

#include "impl/alloc.impl.h"
#endif // ALLOC_H_INCLUDED
