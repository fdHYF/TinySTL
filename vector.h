#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "alloc.h"
#include "uninitialized.h"

namespace TinySTL
{
    template <class T,class Alloc = alloc>
    class vector {
    public:
        using value_type        = T;
        using pointer           = T*;
        using iterator          = T*;
        using const_iterator    = const T*;
        using reference         = T&;
        using const_reference   = const T&;
        using size_type         = size_t;
        using difference_type   = ptrdiff_t;

    public:
        //构造、复制、析构相关函数
        vector() : start(0),finish(0),end_of_storage(0) {}
        vector(size_type n,const T& value) { fill_initialize(n,value); }
        vector(int n,const T& value) { fill_initialize(n,value); }
        vector(long n,const T& value) { fill_initialize(n,value); }
        vector(const size_type n) { fill_initialize(n,value_type()); }
        template <class Iterator>
        vector(Iterator first,Iterator last);
        vector(const vector& rhs);
        vector(vector&& rhs) noexcept;
        vector& operator = (const vector& rhs);
        vector& operator = (vector&&rhs) noexcept;
        ~vector();

        //比较操作
        bool operator == (const vector& rhs) const;
        bool operator != (const vector& rhs) const;

        //迭代器相关操作
        iterator begin() { return start; }
        iterator end() { return finish; }
        const_iterator begin() const { return start; }
        const_iterator end() const { return finish; }
        const_iterator cbegin() const { return start; }
        const_iterator cend() const { return finish; }

        //容器容量相关操作
        size_type size() const { return size_type(finish - start); }
        size_type capacity () const {return size_type(end_of_storage - start); }
        bool empty() const { return start == finish; }
        void resize(size_type new_sz,const T& x);
        void resize(size_type new_sz);
        void shrink_to_fit();

        //对容器元素的访问
        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }
        reference operator[](const difference_type i) { return *(begin() + i); }
        const_reference operator[](const difference_type i) const { return *(cbegin() + i); }

        //修改容器相关的操作
        void clear() { erase(begin(), end()); }
        void swap(vector& rhs);
        void push_back(const T& value);
        void pop_back();
        void insert(iterator position,size_type n,const T& value);
        void insert(iterator position,const T& value) { return insert_aux(position, value); }
        template <class InputIterator>
        void insert(iterator position,InputIterator first,InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first,iterator last);

    private:
        //辅助函数
        void fill_initialize(size_type n,const T& value);
        void deallocate();
        iterator allocate_and_fill(size_type n,const T& value);
        template<class InputIterator>
        void allocate_and_copy(InputIterator first,InputIterator last);
        void insert_aux(iterator position,const T& value);

    private:
        using data_allocator = simple_alloc<value_type,Alloc>;

        iterator start;             //目前使用空间的头
        iterator finish;            //目前使用空间的尾
        iterator end_of_storage;    //表示目前可用空间的尾

    public:
        template<class T1, class Alloc1>
		friend bool operator == (const vector<T1, Alloc1>& lhs, const vector<T1, Alloc1>& rhs);
		template<class T1, class Alloc1>
        friend bool operator != (const vector<T1, Alloc1>& lhs, const vector<T1, Alloc1>& rhs);
    };  //end of class vector
}
#include "impl/vector.impl.h"
#endif // VECTOR_H_INCLUDED
