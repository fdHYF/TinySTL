#ifndef RB_TREE_IMPL_H_INCLUDED
#define RB_TREE_IMPL_H_INCLUDED

namespace TinySTL
{
    namespace Base
    {
        //rb-tree node
        template<class T>
        typename _rb_tree_node<T>::node_ptr
        _rb_tree_node<T>::minimum(node_ptr x) {
            while(x != nullptr) {
                x = x->left;
            }
            return x;
        }

        template<class T>
        typename _rb_tree_node<T>::node_ptr
        _rb_tree_node<T>::maximum(node_ptr x) {
            while(x != nullptr) {
                x = x->right;
            }
            return x;
        }


        //rb-tree iterator
        template<class T, class Ref, class Ptr>
        void _rb_tree_iterator<T, Ref, Ptr>::increment() {
            if(node->right != nullptr) {
                node = node->right;
                while(node->left != nullptr)
                    node = node->left;
            }
            else {
                node_ptr tmp = node->parent;
                while(node == tmp->right) {
                    node = tmp;
                    tmp = tmp->parent;
                }
                if(node->right != tmp)
                    node = tmp;
            }
        }

        template<class T, class Ref, class Ptr>
        void _rb_tree_iterator<T, Ref, Ptr>::decrement() {
            if(node->color == _rb_tree_red && node->parent->parent == node)
                node = node->right;
            else if(node->left != nullptr) {
                node_ptr tmp = node->left;
                while(tmp->right != nullptr)
                    tmp = tmp->right;
                node = tmp;
            }
            else {
                node_ptr tmp = node->parent;
                while(node == tmp->left) {
                    node = tmp;
                    tmp = tmp->parent;
                }
                node = tmp;
            }
        }

        template<class T, class Ref, class Ptr>
        bool operator==(const _rb_tree_iterator<T, Ref, Ptr>& lhs, const _rb_tree_iterator<T, Ref, Ptr>& rhs) {
            return lhs.node == rhs.node;
        }
        template<class T, class Ref, class Ptr>
        bool operator!=(const _rb_tree_iterator<T, Ref, Ptr>& lhs, const _rb_tree_iterator<T, Ref, Ptr>& rhs) {
            return !(lhs == rhs);
        }
    }//namespace Base end

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree& rhs) {
        rb_tree();
        if(!rhs.empty()) {
            root() = _copy(rhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = rhs.node_count;
        }
    }


    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree& rhs) {
        if(rhs != *this) {
            rb_tree tmp(rhs);
            swap(rhs, *this);
        }
        return *this;
    }




    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::~rb_tree() {
        clear();
        put_node(header);
    }

    //rb-tree
    //public member function
    //插入的节点键值允许重复
    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& val) {
        node_ptr tmp = header;
        node_ptr cur = root();
        while(cur != nullptr) {
            tmp = cur;
            cur = key_compare(KeyOfValue()(val), key(cur)) ? left(cur) : right(cur);
        }
        return _insert(cur, tmp, val);
    }

    //插入的节点键值不允许重复
    //返回一个pair,第一个元素是迭代器指向新增节点，第二个元素表示插入与否
    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& val) {
        node_ptr y = header;
        node_ptr cur = root();
        bool comp = true;
        while(cur != nullptr) {
            y = cur;
            comp = key_compare(KeyOfValue()(val), key(cur));
            cur = comp ? left(cur) : right(cur);
        }

        iterator ptr = iterator(y);
        if(comp) {
            if(ptr == begin())
                return pair<iterator, bool>(_insert(cur, y, val), true);
            else
                --ptr;
        }
        if(key_compare(key(ptr.node), KeyOfValue()(val)))
            return pair<iterator, bool>(_insert(cur, y, val), true);

        //新值与树中键值重复
        return pair<iterator, bool>(ptr, false);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    template<class InputIterator>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
        for(; first != last; ++first)
            insert_unique(*first);
    }


    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() {
        _erase(root());
    }

    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
        node_ptr y = header;
        node_ptr x = root();
        while(x != nullptr) {
            if(!key_compare(key(x), k)) {
                //x值大于k值，向左
                y = x;
                x = left(x);
            }
            else
                x = right(x);
        }
        iterator j = iterator(y);
        return (j == end() || key_compare(k, key(j.node))) ? end() : j;
    }


    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
        node_ptr y = header;
        node_ptr x = root();
        while(x != nullptr) {
            if(!key_compare(key(x), k)) {
                //x处对应节点键值大于等于k
                y = x;
                x = left(x);
            }
            else
                x = right(x);
        }
        return iterator(y);
    }


    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
        node_ptr y = header;
        node_ptr x = root();
        while(x != nullptr) {
            if(key_compare(k, key(x))) {
                y = x;
                x = left(x);
            }
            else
                x = right(x);
        }
        return iterator(y);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) {
        return size_type(distance(lower_bound(k), upper_bound(k)));
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
                    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }

    template<class Key,class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& k) {
        size_type number = count(k);
        if(number == 0)
            return 0;
        iterator first = lower_bound(k);
        iterator last = upper_bound(k);
        while(first != last) {
            node_ptr cur_node = first.node;
            ++first;
            _Rb_tree_rebalance_for_erase(cur_node, leftmost(), rightmost(), root());
        }
        return number;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position) {
        node_ptr cur_node = position.node;
        ++position;
        _Rb_tree_rebalance_for_erase(cur_node, leftmost(), rightmost(), root());
        --node_count;
        return position;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
        if(first == begin() && last == end()) {
            clear();
            return end();
        }
        while(first != last)
            first = erase(first);
        return last;
    }

    //private member function
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::init() {
        header = get_node();
        color(header) = _rb_tree_red;

        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

    //复制以x为根节点的树，复制所得x_copy父节点为p
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(node_ptr x, node_ptr p) {
        //x与p必须是non-null
        node_ptr top = clone_node(x);
        top->parent = p;
        try {
            if(x->right != nullptr)
                top->right = _copy(right(x), top);
            p = top;
            x = left(x);
            while(x != nullptr) {
                node_ptr y = clone_node(x);
                p ->left = y;
                y->parent = p;
                if(x->right != nullptr)
                    y->right = _copy(right(x), y);
                p = y;
                x = left(x);
            }
        }
        catch(...) {
            _erase(top);
        }
        return top;
    }

    //递归删除
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(node_ptr x) {
        while(x != nullptr) {
            _erase(right(x));
            node_ptr y = x->left;
            destroy(x);
            x = y;
        }

    }

    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::creat_node(const value_type& val) {
        node_ptr tmp = get_node();
        try {
            construct(&tmp->value, val);
        }
        catch(...) {
            put_node(tmp);
            return nullptr;
        }
        return tmp;
    }

    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clone_node(node_ptr x) {
        node_ptr tmp = creat_node(x->value);
        tmp->color = x->color;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = nullptr;
        return tmp;
    }

    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc>
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_node(node_ptr x) {
        destroy(&x->value);
        put_node(x);
    }

    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc >
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(node_ptr _x, node_ptr _y, const value_type& value) {
        //x_为新插入节点，y_为插入点的父节点，value为插入值
        node_ptr x = _x;
        node_ptr y = _y;
        node_ptr z;
        if(y == header || x != nullptr || key_compare(KeyOfValue()(value), key(y))) {
            z = creat_node(value);
            left(y) = z;
            if(y == header) {
                root() = z;
                rightmost() = z;
            }
            else if(y == leftmost())
                leftmost() = z;
        }
        else {
            z = creat_node(value);
            right(y) = z;
            if(y == rightmost())
                rightmost() = z;
        }

        parent(z) = y;
        left(z) = nullptr;
        right(z) = nullptr;

        _rb_tree_rebalance(z, root());
        ++node_count;
        return iterator(z);
    }

    //调整插入节点颜色，旋转保持平衡
    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rebalance(node_ptr x,node_ptr& root) {
        x->color = _rb_tree_red;        //新增节点必为红
        while(x != root && x->parent->color == _rb_tree_red) {
            if(x->parent == x->parent->parent->left) {  //父节点是祖父节点的左子节点
                node_ptr y = x->parent->parent->right;
                if(y != nullptr && y->color == _rb_tree_red) {
                    x->parent->color = _rb_tree_black;
                    y->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    x = x->parent->parent;
                }
                else {
                    if(x == x->parent->right) {
                        x = x->parent;
                        _rb_tree_rotate_left(x, root);
                    }
                    x->parent->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    _rb_tree_rotate_right(x->parent->parent, root);
                }
            }
            else { //父节点是祖父节点的右子节点
                node_ptr y = x->parent->parent->left;
                if(y && y->color == _rb_tree_red) {
                    x->parent->color = _rb_tree_black;
                    y->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    x=x->parent->parent;
                }
                else {
                    if(x == x->parent->left) {
                        x = x->parent;
                        _rb_tree_rotate_right(x, root);
                    }
                    x->parent->color = _rb_tree_black;
                    x->parent->parent->color = _rb_tree_red;
                    _rb_tree_rotate_left(x->parent->parent, root);
                }
            }
        }
        root->color = _rb_tree_black;
    }

    //左旋操作
    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rotate_left(node_ptr x, node_ptr& root) {
        node_ptr y = x->right;
        x->right = y->left;
        if(y->left != nullptr)
            y->left->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if(x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    //右旋操作
    template<class Key, class Value, class KeyOfValue,
            class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_rb_tree_rotate_right(node_ptr x, node_ptr& root) {
        node_ptr y = x->left;
        x->left = y->right;
        if(y->right != nullptr)
            y->right->parent = x;
        y->parent = x->parent;
        if(x == root)
            root = y;
        else if(x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    //删除节点并保持平衡
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc >
    void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::
        _Rb_tree_rebalance_for_erase(node_ptr z,node_ptr& _root,
                                     node_ptr& _leftmost,node_ptr& _rightmost) {
        node_ptr y = z;
        node_ptr x = nullptr;
        node_ptr x_parent = nullptr;

        if(y->left == nullptr)          //z含有最多一个非空子节点,y==z
            x = y->right;               //x可能为空也可能不为空
        else {  //y->left != nullptr
            if(y->right == nullptr)
                x = y->left;            //x不为nullptr,y==z
            else {
                //y含有两个非空子节点
                y = y->right;
                while(y->left != nullptr)
                    y = y->left;        //y是z的后继节点,y!=z
                x = y->right;           //x可能为空
            }
        }
        //z同时有两个非空子节点
        if(y != z) {
            z->left->parent = y;
            y->left = z->left;
            if(y != z->right) {
                x_parent = y->parent;
                y->parent->left = x;
                y->right = z->right;
                z->right->parent = y;
            }
            else
                x_parent = y;
            if(_root == z)
                _root = y;
            else if(z->parent->left == z)
                z->parent->left = y;
            else
                z->parent->right = y;
            y->parent = z->parent;
            swap(z->color, y->color);
            y = z;                      //y现在实际指向被删除的节点
        }
        else {  //y == z
            x_parent = y->parent;
            if(x != nullptr)
                x->parent = y->parent;
            if(_root == z)
                _root = x;
            else {
                if(z->parent->left == z)
                    z->parent->left = x;
                else
                    z->parent->right = x;
            }
            if(_leftmost == z) {
                if(z->right == nullptr)
                    _leftmost = z->parent;
                else
                    _leftmost = minimum(x);
            }

            if(_rightmost == z) {
                if(z->left == nullptr)
                    _rightmost = z->parent;
                else
                    _rightmost = maximum(x);
            }
        }
        //如果删除节点是红色，直接删除，否则需要调整颜色
        if(y->color != _rb_tree_red) {
            while(x != root() && (x == nullptr || x->color == _rb_tree_black)) {
                if(x == x->parent->left) {               //x是其父节点的左子节点
                    node_ptr w = x->parent->right;
                    if(w->color == _rb_tree_red) {      //case1:w是红色
                        w->color = _rb_tree_black;
                        x_parent->color = _rb_tree_red;
                        _rb_tree_rotate_left(x_parent, _root);
                        w = x_parent->right;
                    }
                    //case1将转化为case2、case3、case4
                    //case2:w左右节点均为黑色(NULL自视为黑色节点)
                    if((w->left == nullptr || w->left->color == _rb_tree_black) &&
                       (w->right == nullptr || w->right->color == _rb_tree_black)) {
                        w->color = _rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else {
                        //case3:w为黑色，w左节点是红色，w的右节点是黑色
                        if(w->right == nullptr || w->right->color == _rb_tree_black) {
                            if(w->left != nullptr)
                                w->left->color = _rb_tree_black;
                            w->color = _rb_tree_red;
                            _rb_tree_rotate_right(w, _root);
                            w = x_parent->right;
                        }
                        //case4:w为黑色，w的右节点为红色
                        w->color = x_parent->color;
                        x_parent->color = _rb_tree_black;
                        if(w->right != nullptr)
                            w->right->color = _rb_tree_black;
                        _rb_tree_rotate_left(x_parent, _root);
                        break;
                    }
                }
                else {
                    //x是其父节点的右子节点，操作同上
                    node_ptr w = x->parent->left;
                    if(w->color == _rb_tree_red) {
                        w->color = _rb_tree_black;
                        x_parent->color = _rb_tree_red;
                        _rb_tree_rotate_left(x_parent, _root);
                        w = x_parent->left;
                    }
                    if((w->left == nullptr || w->left->color == _rb_tree_black) &&
                       (w->right == nullptr || w->right->color == _rb_tree_black)) {
                        w->color = _rb_tree_red;
                        x = x_parent;
                        x_parent = x_parent->parent;
                    }
                    else {
                        if(w->left == nullptr || w->left->color == _rb_tree_black) {
                            w->color = _rb_tree_red;
                            _rb_tree_rotate_left(w, _root);
                            w = x_parent->left;
                        }
                        w->color = x_parent->color;
                        x_parent->color = _rb_tree_black;
                        if(w->left != nullptr)
                            w->left->color = _rb_tree_black;
                        _rb_tree_rotate_right(x_parent, _root);
                        break;
                    }
                }
            }
            if(x != nullptr)
                x->color = _rb_tree_black;
        }
        destroy_node(y);
    }

    //friend function
    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
         rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
        using std::swap;
        swap(lhs.key_compare, rhs.key_compare);
        swap(lhs.node_count, rhs.node_count);
        swap(lhs.header, rhs.header);
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    bool operator == (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
        if(lhs.size() == rhs.size()) {
            auto ptr1 = lhs.begin();
            auto ptr2 = rhs.begin();
            while(ptr1 != lhs.end() && ptr2 != rhs.end()) {
                if(*ptr1 != *ptr2)
                    return false;
            }
            return true;
        }
        return false;
    }

    template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
    bool operator != (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
                      const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }
}

#endif
