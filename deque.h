#ifndef DEQUE_H_INCLUDED
#define DEQUE_H_INCLUDED
#define MIN_NODE_SIZE 8
#include "alloc.h"
#include "construct.h"
#include "iterator.h"

namespace TinySTL
{
    namespace Base
    {
        template<class T, size_t BufSize>
        class _deque_iterator {//未采用继承
            template<class T1, class Alloc, size_t Buf>
            friend class deque;
        public:
            static size_t buffer_size() { return _deque_buf_size(BufSize, sizeof(T)); }
            static inline size_t _deque_buf_size(size_t n, size_t sz) {
                return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
            }

            using value_type            = T;
            using pointer               = T*;
            using reference             = T&;
            using const_pointer         = const T*;
            using const_reference       = const T&;
            using difference_type       = ptrdiff_t;
            using size_type             = size_t;
            using iterator_category     = random_access_iterator_tag;
            using map_pointer           = T**;

            using self                  = _deque_iterator;

            reference operator*() const { return *cur; }
            pointer operator->() const { return&(operator*()); }
            difference_type operator-(const self& rhs) const;
            self& operator++();
            self operator++(int);
            self& operator--();
            self operator--(int);
            self& operator+=(difference_type n);
            self operator+(difference_type n) const;
            self& operator-=(difference_type n) { return *this += -n; }
            self operator-(difference_type n) const;

            reference operator[](difference_type n) const { return *(*this + n); }

            bool operator==(const self& rhs) const { return cur == rhs.cur; }
            bool operator!=(const self& rhs) const { return !(*this == rhs); }

        private:
            void set_node(map_pointer new_node);        //到达缓冲区边缘，实现缓冲区跳跃(前进或后退)

            T* cur;                                     //此迭代器所指缓冲区的当前元素
            T* first;                                   //此迭代器所指缓冲区的头
            T* last;                                    //此迭代器所指缓冲区的尾
            map_pointer node;
        };
    }//end of namespace Base

    template<class T, class Alloc = alloc, size_t BufSize = 0>
    class deque {
        template<class T1, class BufSize1>
        friend class _deque_iterator;
    public:
        using value_type        = T;
        using size_type         = size_t;
        using pointer           = T*;
        using map_pointer       = T**;
        using reference         = T&;
        using const_reference   = const T&;
        using difference_type   = ptrdiff_t;
        using iterator          = Base::_deque_iterator<T, BufSize>;
        using const_iterator    = Base::_deque_iterator<const T, BufSize>;
        static size_t buffer_size() {return _deque_buf_size(BufSize,sizeof(T));}
        static size_t _deque_buf_size(size_t n,size_t sz){
            return n!=0?n:(sz<512?size_t(512/sz):size_t(1));
        }

    private:
        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size;

    public:
        deque();
        explicit deque(size_type n, const value_type& value);
        explicit deque(size_type n);
        template<class InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque& rhs);
        deque& operator=(const deque& rhs);
        deque& operator=(deque&& rhs);
        ~deque();

        iterator begin() { return start; }
        iterator end() { return finish; }
        reference operator[](size_type n) { return start[difference_type(n)]; }
        reference front() { return *start; }
        reference back();
        size_type size() const { return (finish - start); }
        bool empty() const { return finish == start; }
        size_type max_size() const { return size_type(-1); }

        void push_back(const value_type& value);
        void push_front(const value_type& value);
        void pop_back();
        void pop_front();
        void clear();
        void shrink_to_fit();
        void resize(size_type new_size);
        void swap(deque& rhs);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        iterator insert(iterator position, const value_type& value);
        void insert(iterator position, size_type n, const value_type& value);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);

    private:
        size_t initial_map_size() { return MIN_NODE_SIZE; }

        void fill_initialize(size_type n, const value_type& value);
        void creat_map_and_nodes(size_type n);
        void push_back_aux(const value_type& value);
        void push_front_aux(const value_type& value);
        void pop_back_aux();
        void pop_front_aux();
        void reverse_map_at_back(size_type nodes_to_add = 1);
        void reverse_map_at_front(size_type nodes_to_add = 1);
        void reallocate_map(size_type nodes_to_add, bool add_at_front);
        iterator insert_aux(iterator position, const value_type& value);
        pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
        void deallocate_node(pointer p) { data_allocator::deallocate(p, buffer_size()); }

    private:
        using data_allocator = simple_alloc<T, Alloc>;
        using map_allocator = simple_alloc<pointer, Alloc>;
    };
}
#include "impl/deque.impl.h"
#endif // DEQUE_H_INCLUDED
