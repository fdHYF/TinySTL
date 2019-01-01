#ifndef RB_TREE_H_INCLUDED
#define RB_TREE_H_INCLUDED

#include "alloc.h"
#include "iterator.h"
#include "construct.h"
#include "uninitialized.h"
#include "function.h"
#include "pair.h"

namespace TinySTL
{
    //树节点颜色
    using _rb_tree_color_type                = bool;
    const _rb_tree_color_type _rb_tree_red   = false;  //红色为0
    const _rb_tree_color_type _rb_tree_black = true;   //黑色为1

    namespace Base
    {
        //红黑树节点设计
        template<class T>
        struct _rb_tree_node {
            using color_type = _rb_tree_color_type;
            using node_ptr   = _rb_tree_node<T>*;

            _rb_tree_node() = default;
            _rb_tree_node(const T& val) : value(val), parent(nullptr), left(nullptr), right(nullptr) {}

            static node_ptr minimum(node_ptr x);
            static node_ptr maximum(node_ptr x);

            bool operator==(const _rb_tree_node<T>& rhs) {
                return (*this.color == rhs.color) && (*this.parent == rhs.parent) &&
                        (*this.left == rhs.left) && (*this.right == rhs.right) && (*this.value == rhs.value);
            }

            bool operator!=(const _rb_tree_node<T>& rhs) { return !(this == &rhs); }

            color_type color;
            node_ptr parent;    //指向父节点
            node_ptr left;      //指向左节点
            node_ptr right;     //指向右节点
            T value;
        };

        //迭代器的设计
        template<class T, class Ref, class Ptr>
        struct _rb_tree_iterator {
            typedef typename _rb_tree_node<T>::node_ptr node_ptr;
            using iterator_category   = bidirectional_iterator_tag;
            using difference_type     = ptrdiff_t;
            using value_type          = T;
            using reference           = Ref;
            using pointer             = Ptr;

            using iterator            = _rb_tree_iterator<T, T&, T*>;
            using const_iterator      = _rb_tree_iterator<T, const T&, const T*>;
            using self                = _rb_tree_iterator<T, Ref, Ptr>;

        public:
            _rb_tree_iterator()  = default;
            _rb_tree_iterator(node_ptr x) : node(x) {}
            _rb_tree_iterator(const iterator& rhs) : node(rhs.node) {}
            node_ptr& get_node() { return node; }
            reference operator*() const { return node->value; }
            pointer operator->() const { return &(operator*()); }
            self& operator++() { increment(); return *this; }
            self& operator++(int) {
                self tmp = *this;
                increment();
                return tmp;
            }
            self& operator--() { decrement(); return *this; }
            self operator--(int) {
                self tmp = *this;
                decrement();
                return tmp;
            }

            bool operator == (const self& rhs) { return node == rhs.node; }
            bool operator != (const self& rhs) { return !(node == rhs.node); }

        public:
            node_ptr node;

        private:
            void increment();       //operator++内部操作
            void decrement();       //operator--内部操作
            template<class T_, class Ref_, class Ptr_>
            friend bool operator==(const _rb_tree_iterator<T_, Ref_, Ptr_>& lhs,
                                   const _rb_tree_iterator<T_, Ref_, Ptr_>& rhs);
            template<class T_, class Ref_, class Ptr_>
            friend bool operator!=(const _rb_tree_iterator<T_, Ref_, Ptr_>& lhs,
                                   const _rb_tree_iterator<T_, Ref_, Ptr_>& rhs);
        };
    }//namespace Base end


    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc = alloc>
    class rb_tree {
    public:
        using node_ptr              = typename Base::_rb_tree_node<Value>::node_ptr;
        using rb_tree_node          = Base::_rb_tree_node<Value>;
        using rb_tree_node_alocator = simple_alloc<rb_tree_node, Alloc>;
        using color_type            = _rb_tree_color_type;

        using key_type              = Key;
        using value_type            = Value;
        using pointer               = value_type*;
        using const_pointer         = const value_type*;
        using reference             = value_type&;
        using const_reference       = const value_type&;
        using size_type             = size_t;
        using difference_type       = ptrdiff_t;
        using iterator              = Base::_rb_tree_iterator<value_type, reference, pointer>;
        using const_iterator        = Base::_rb_tree_iterator<value_type, const_reference, const_pointer>;

    public:
        rb_tree() : node_count(0), key_compare() { init(); }
        rb_tree(const Compare& comp) : node_count(0), key_compare(comp) { init(); }
        rb_tree(const rb_tree& rhs);
        rb_tree(rb_tree&& rhs);
        rb_tree& operator = (const rb_tree& rhs);
        rb_tree& operator = (rb_tree&& rhs);
        ~rb_tree();

    public:
        Compare key_comp() const { return key_compare; }
        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        const_iterator cbegin() const { return leftmost(); }
        const_iterator cend() const { return header; }
        bool empty() const { return node_count == 0; }
        size_type size() const { return node_count; }
        size_type max_size() const { return size_type(-1); }
        void clear();

        pair<iterator, bool> insert_unique(const value_type& value);
        template<class InputIterator>
        void insert_unique(InputIterator first, InputIterator last);
        iterator insert_equal(const value_type& value);
        template<class InputIterator>
        void insert_equal(InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        size_type erase(const key_type& k);             //删除键值等于k的节点，并返回删除个数
        iterator find(const key_type& k);
        iterator lower_bound(const key_type& k);        //返回一个迭代器，指向第一个关键字大于k的元素
        iterator upper_bound(const key_type& k);        //返回一个迭代器，指向第一个关键字不小于k的元素
        size_type count(const key_type& k);             //返回key值与k相等的元素个数
        //equal_range返回一个pair表示关键字等于k的元素的范围
        pair<iterator, iterator> equal_range(const key_type& k);
    private:
        void init();            //用于初始化rb_tree
        node_ptr get_node() { return rb_tree_node_alocator::allocate(); }
        void put_node(node_ptr p) { rb_tree_node_alocator::deallocate(p); }
        node_ptr creat_node(const value_type& value);       //分配并构造一个节点
        void destroy_node(node_ptr x);                      //析构并释放一个节点
        node_ptr clone_node(node_ptr x);                    //复制一个节点
        iterator _insert(node_ptr x, node_ptr y, const value_type& value);
        node_ptr _copy(node_ptr x, node_ptr p);
        void _erase(node_ptr x);
        void _rb_tree_rebalance(node_ptr x, node_ptr& root);
        void _rb_tree_rotate_left(node_ptr x, node_ptr& root);
        void _rb_tree_rotate_right(node_ptr x, node_ptr& root);
        void _Rb_tree_rebalance_for_erase(node_ptr z, node_ptr& root, node_ptr& _leftmost, node_ptr& _rightmost);

        //header成员的存取
        node_ptr& root() const { return (node_ptr&)header->parent; }
        node_ptr& leftmost() const { return (node_ptr&)header->left; }
        node_ptr& rightmost() const { return (node_ptr&)header->right; }

        //节点成员存取
        static node_ptr& left(node_ptr& x)
            { return (node_ptr&)(x->left); }
        static node_ptr& right(node_ptr& x)
            { return (node_ptr&)(x->right); }
        static node_ptr& parent(node_ptr& x)
            { return (node_ptr&)(x->parent); }
        static reference value(node_ptr x)
            { return x->value; }
        static color_type& color(node_ptr x)
            { return (color_type&)x->color; }
        static const Key& key(node_ptr x)
            { return KeyOfValue()(value(x)); }

        //求取极大值和极小值
        static node_ptr minimum(node_ptr x) { return rb_tree_node::minimum(x); }
        static node_ptr maximum(node_ptr x) { return rb_tree_node::maximum(x); }

    private:
        size_type node_count;
        node_ptr header;
        Compare key_compare;
    private:
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend swap(rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& lsh,
                    rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator==(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& rhs);
        template<class Key_, class Value_, class KeyOfValue_, class Compare_, class Alloc_>
        friend bool operator!=(const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& lhs,
                               const rb_tree<Key_, Value_, KeyOfValue_, Compare_, Alloc_>& rhs);
    };
}
#include "impl/rb-tree.impl.h"
#endif // RB-TREE_H_INCLUDED
