#ifndef ALLOC_IMPL_H_INCLUDED
#define ALLOC_IMPL_H_INCLUDED

#include <iostream>

namespace TinySTL
{
    /************************第一级配置器*******************/
    template<int inst>
    void* _malloc_alloc_template<inst>::allocate(size_t n){
        void* result = malloc(n);
        if(result == nullptr)
            result = oom_malloc(n);
        return result;
    }

    template<int inst>
    void _malloc_alloc_template<inst>::deallocate(void* ptr,size_t){
        free(ptr);                //直接调用free()
    }

    template<int inst>
    void* _malloc_alloc_template<inst>::reallocate(void* ptr,size_t,size_t new_sz){
        void* result = reallocate(ptr,new_sz);
        if(result == nullptr)
            result = oom_realloc(ptr,new_sz);
        return result;
    }

    template<int inst>
    void (* _malloc_alloc_template<inst>::_malloc_alloc_oom_handler) () = 0;

    template<int inst>
    void* _malloc_alloc_template<inst>::oom_malloc(size_t n) {
        void (* my_malloc_handler) ();
        void* result;

        for(;;) {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(0 == my_malloc_handler) { //客端没有设置内存不足处理例程
                std::cerr << "out of memory" << std::endl;
                exit(0);
            }
            (*my_malloc_handler) ();
            result = malloc(n);
            if(result)
                return result;
        }
    }

    template<int inst>
    void* _malloc_alloc_template<inst>::oom_realloc(void* ptr,size_t n) {
        void (* my_malloc_handler) ();
        void* result;

        for(;;) {
            my_malloc_handler = _malloc_alloc_oom_handler;
            if(0 == my_malloc_handler) {//没有设置内存处理例程
                std::cerr << "out of memory" << std::endl;
                exit(0);
            }
            (*my_malloc_handler) ();
            result = realloc(ptr,n);
            if(result)
                return result;
        }
    }


    /*******************第二级配置器*************************/
    template<bool threads,int inst>
    void* _default_alloc_template<threads,inst>::allocate(size_t bytes){
        if (bytes > (size_t)_MAX_BYTES) {
            return malloc_alloc::allocate(bytes);
        }

        //选择freelist编号
        size_t index = FREELIST_INDEX(bytes);

        obj * my_list = free_list[index];

        if (my_list == 0) {
        //没有可用的freelist,准备重新填充freelist
        void *r = refill(ROUND_UP(bytes));
        return r;
        }
        //调整freelist,将list后面的空间前移，返回list所指的空间
        free_list[index] = my_list->free_list_link;
        return my_list;
    }

    template<bool threads,int inst>
    void _default_alloc_template<threads,inst>::deallocate(void* ptr,size_t n){
        obj* q = static_cast<obj*>(ptr);
        obj* volatile * my_free_list;

        //大于128bytes调用一级配置器
        if(n > static_cast<size_t>(_MAX_BYTES)){
            malloc_alloc::deallocate(ptr,n);
            return;
        }
        my_free_list = free_list + FREELIST_INDEX(n);
        //调整free list,回收区块
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    //当free list中没有对应的区块时，调用refill(),为free list填充空间
    //新的空间取自内存池，缺省取得20个新节点，但可能小于20
    template<bool threads,int inst>
    void* _default_alloc_template<threads,inst>::refill(size_t n){
        int nobjs = 20;
        char* chunk = chunk_alloc(n,nobjs);
        obj* volatile * my_free_list;
        obj* result;
        obj* current_obj;
        obj* next_obj;
        int i;
        //如果只取得一个区块，分配与调用者
        if(nobjs == 1)
            return chunk;
        my_free_list = free_list + FREELIST_INDEX(n);

        result = (obj*)chunk;   //此块返回与客户端
        *my_free_list = next_obj = (obj*)(chunk+n);
        for(i = 1;;++i) {
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj + n);
            if(nobjs - 1 == i) {
                current_obj->free_list_link = nullptr;
                break;
            }
            else
                current_obj->free_list_link = next_obj;
        }
        return result;
    }

    template<bool threads,int inst>
    char* _default_alloc_template<threads,inst>::chunk_alloc(size_t size,int& nobjs){
        char* result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;      //内存池剩余空间

        if(bytes_left >= total_bytes){
            result = start_free;
            start_free += total_bytes;
            return result;
        } else if(bytes_left >= size) {
            //内存池空间不能完全满足需求量，但足够供应一个以上的区块
            nobjs = bytes_left/size;
            total_bytes = size * nobjs;
            result = start_free;
            start_free += total_bytes;
            return result;
        } else {
            //内存池剩余空间一个区块也无法提供
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >>4);

            if(bytes_left > 0) {
                //将内存池中的残余分配与适当的free list
                obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ((obj*)start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj*)start_free;
            }

            //配置heap空间，补充内存池
            start_free = (char*)malloc(bytes_to_get);
            if(start_free == nullptr) {
                //heap空间不足
                int i;
                obj* volatile * my_free_list,*p;
                for(i = size;i <= _MAX_BYTES; i += _ALLGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(p != nullptr) {
                        //调整free list释放未用区块
                        *my_free_list = p->free_list_link;
                        start_free = (char*)p;
                        end_free = start_free + i;
                        return chunk_alloc(size,nobjs);
                    }
                }
                end_free = nullptr;
                //调用一级配置器,尝试out_of_memory
                start_free = (char*)malloc_alloc::allocate(bytes_to_get);
            }
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size,nobjs);     //修正nobjs
        }
    }
}

#endif // ALLOC_IMPL_H_INCLUDED
