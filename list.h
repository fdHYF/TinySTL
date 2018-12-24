#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "iterator.h"
#include "alloc.h"

namespace TinySTL
{
    namespace Base {
        //list节点设计
        template<class T>
        struct _list_node {
            T data;
            _list_node<T>* prev;
            _list_node<T>* next;

            _list_node(const T& d, _list_node<T>* p, _list_node<T>* n) :
                data(d), prev(p), next(n) {}
            bool operator == (const _list_node& rhs) {
                return data == rhs.data && prev == rhs.prev &&next == rhs.next;
            }
        };

        //list迭代器设计
        template<class T,class Ref, class Ptr>
        struct _list_iterator {
        public:
            using iterator                  = _list_iterator<T, T&, T*>;
            using self                      = _list_iterator<T, Ref, Ptr>;

            using iterator_category         = bidirectional_iterator_tag;
            using value_type                = T;
            using reference                 = Ref;
            using pointer                   = Ptr;
            using size_type                 = size_t;
            using difference_type           = ptrdiff_t;

            using link_type = _list_node<T>*;
            link_type p;

            //constructor
            _list_iterator(const link_type x) : p(x) {}
            _list_iterator() {}
            _list_iterator(const _list_iterator& x) : p(x.p) {}

            //迭代器操作
            _list_iterator& operator++() {
                p = p->next;
                return *this;
            }
            _list_iterator operator++(int) {
                auto tmp = *this;
                ++*this;
                return tmp;
            }
            _list_iterator& operator--() {
                p = p->prev;
                return p;
            }
            _list_iterator operator--(int) {
                auto tmp = *this;
                ++*this;
                return tmp;
            }

            T& operator*() const { return p->data; }
            T* operator->() const { return &(operator*()); }

            template<class T_, class Ref_, class Ptr_>
            friend bool operator == (const _list_iterator<T_, Ref_, Ptr_>& lhs,
                                     const _list_iterator<T_, Ref_, Ptr_>& rhs);
            template<class T_, class Ref_, class Ptr_>
            friend bool operator != (const _list_iterator<T_, Ref_, Ptr_>& lhs,
                                     const _list_iterator<T_, Ref_, Ptr_>& rhs);
        };
    }//end of namespace Base

    template<class T, class Alloc = alloc>
    class list {
    public:
        using value_type            = T;
        using size_type             = size_t;
        using reference             = T&;
        using iterator              = Base::_list_iterator<T, T&, T*>;
        using const_iterator        = Base::_list_iterator<T, const T&, const T*>;

    private:
        using list_node             = Base::_list_node<T>;
        using link_type             = list_node*;
        using list_node_allocator   = simple_alloc<list_node, Alloc>;
        link_type node;

    public:
        list() { empty_initialize(); }
        list(size_type n, const T& value);
        template<class InputIterator>
        list(InputIterator first, InputIterator last);
        list(const list& rhs);
        list(list&& rhs) noexcept;
        list& operator = (const list& rhs);
        list& operator = (list&& rhs) noexcept;
        ~list();

        bool empty() const { return node->next == node; }
        size_type size() const;
        reference front() { return *begin(); }
        reference back() { return *(--end()); }

        void push_back(const value_type& value) { insert(end(), value); }
        void push_front(const value_type& value) { insert(begin(), value); }
        void pop_front() { erase(begin()); }
        void pop_back() { iterator tmp = end(); erase(--tmp); }

        iterator begin() { return (link_type)(node->next); }
        iterator end() { return node; }
        const_iterator begin() const { return (link_type)(node->next); }
        const_iterator end() const { return node; }

        bool operator == (const list& rhs) const;
        bool operator != (const list& rhs) const;

        iterator insert(iterator position, const value_type& val);
        void insert(iterator position, size_type n, const value_type& val);
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void clear();
        void remove(const value_type& val);
        void unique();
        void splice(iterator position, list& x);
        void splice(iterator position, list&, iterator i);
        void splice(iterator position, list&, iterator first, iterator last);
        void merge(list& x);
        template<class Compare>
        void merge(list& x, Compare comp);
        void reverse();
        void swap(list& rhs);
        void sort();
        template<class Compare>
        void sort(Compare comp);

    private:
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type p) { list_node_allocator::deallocate(p); }
        link_type creat_node(const value_type& val);
        void destroy_node(link_type p);
        void empty_initialize();
        void transfer(iterator positiopn, iterator first, iterator last);
        const_iterator changeIteratorToConstIterator(iterator& ptr);

    public:
        template<class T1, class Alloc1>
        friend void swap(list<T1, Alloc1>& lhs, list<T1, Alloc1>& rhs);
        template<class T1, class Alloc1>
        friend bool operator==(const list<T1, Alloc1>& lhs, const list<T1, Alloc1>& rhs);
        template<class T1, class Alloc1>
        friend bool operator!=(const list<T1, Alloc1>& lhs, const list<T1, Alloc1>& rhs);
    };
}

#include "impl/list.impl.h"
#endif // LIST_H_INCLUDED
