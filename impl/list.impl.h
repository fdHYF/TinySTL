#ifndef LIST_IMPL_H_INCLUDED
#define LIST_IMPL_H_INCLUDED

namespace TinySTL
{
    namespace Base
    {
        template<class T,class Ref, class Ptr>
        bool operator == (const _list_iterator<T, Ref, Ptr>& lhs,
                          const _list_iterator<T, Ref, Ptr>& rhs) {
            return lhs.p == rhs.p;
        }

        template<class T,class Ref, class Ptr>
        bool operator != (const _list_iterator<T, Ref, Ptr>& lhs,
                          const _list_iterator<T, Ref, Ptr>& rhs) {
            return !(lhs == rhs);
        }
    }//end of namespace Base

    //list constructor
    template<class T, class Alloc>
    list<T, Alloc>::list(size_type n, const value_type& val) {
        empty_initialize();
        for(int i = 0; i < n; ++i)
            push_back(val);
    }

    template<class T, class Alloc>
    template<class InputIterator>
    list<T, Alloc>::list(InputIterator first, InputIterator last) {
        empty_initialize();
        auto ptr = first;
        while(ptr != last) {
            push_back(*ptr);
        }
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(const list& rhs) {
        empty_initialize();
        auto ptr = rhs.begin();
        for(; ptr != rhs.end(); ++ptr)
            push_back(*ptr);
    }

    template<class T, class Alloc>
    list<T, Alloc>::list(list&& rhs) noexcept :
        node(rhs.node) {
        rhs.node = nullptr;
    }

    template<class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator = (const list& rhs) {
        if(this != &rhs) {
            list tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    template<class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator = (list&& rhs) noexcept {
        if(this != &rhs) {
            clear();
            put_node(node);
            node = rhs.node;
            rhs.node = nullptr;
        }
        return *this;
    }

    template<class T, class Alloc>
    list<T, Alloc>::~list() {
        clear();
        put_node(node);
    }


    //list的接口
    template<class T, class Alloc>
    typename list<T, Alloc>::size_type
    list<T, Alloc>::size() const {
        if(empty())
            return 0;
        size_type count = 0;
        link_type tmp = link_type(node->next);
        while(tmp != node) {
            count++;
            tmp = tmp->next;
        }
        return count;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::operator == (const list& rhs) const {
        if(size() != rhs.size())
            return false;
        auto ptr1 = begin();
        auto ptr2 = rhs.begin();
        while(ptr1 != end()) {
            if(*ptr1 == *ptr2) {
                ++ptr1;
                ++ptr2;
            }
            else
                return false;
        }
        return true;
    }

    template<class T, class Alloc>
    bool list<T, Alloc>::operator != (const list& rhs) const {
        return !(*this == rhs);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::insert(iterator position, const value_type& val) {
        link_type tmp = creat_node(val);
        tmp->next = position.p;
        tmp->prev = position.p->prev;
        (link_type(position.p->prev))->next = tmp;
        position.p->prev = tmp;
        return iterator(tmp);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::insert(iterator position, size_type n, const value_type& val) {
        if(n <= 0)
            return;
        while(n != 0) {
            position = insert(position, val);
            n--;
        }
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        while(first != last) {
            auto tmp = first++;
            position = insert(position, *tmp);
        }
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator position) {
        link_type prev_node = link_type(position.p->prev);
        link_type next_node = link_type(position.p->next);
        prev_node->next = next_node;
        next_node->prev = prev_node;
        destroy_node(position.p);
        return iterator(next_node);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator
    list<T, Alloc>::erase(iterator first, iterator last) {
        while(first != last)
            first = erase(first);
        return last;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::clear() {
        link_type cur = (link_type)node->next;
        while(cur != node) {
            link_type tmp = cur;
            cur = (link_type) cur->next;
            destroy_node(tmp);
        }

        node->next = node;
        node->prev = node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::remove(const value_type& val) {
        iterator first = begin();
        while(first != end()) {
            iterator next = first;
            next++;
            if(*first == val)
                erase(first);
            first = next;
        }
    }

    //移除连续相同元素
    template<class T, class Alloc>
    void list<T, Alloc>::unique() {
        iterator first = begin();
        iterator last = end();

        if(first == last)
            return;

        iterator next = first;
        while(++next != last) {
            if(*next == *first)
                erase(next);
            else
                first = next;
            next = first;
        }
    }


    template<class T, class Alloc>
    void list<T, Alloc>::transfer(iterator position, iterator first, iterator last) {
        if(position != last) {
            (link_type(last.p->prev))->next = position.p;
            (link_type(first.p->prev))->next = last.p;
            (link_type(position.p->prev))->next = first.p;
            link_type tmp = (link_type(position.p))->prev;
            link_type(position.p->prev) = link_type(last.p->prev);
            link_type(last.p->prev) = link_type(first.p->prev);
            link_type(first.p->prev) = tmp;
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list& l) {
        if(!l.empty())
            transfer(position, l.begin(), l.end());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list&, iterator i) {
        iterator j = i;
        ++j;
        if(position == i || position == j)
            return;
        transfer(position, i, j);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list&, iterator first, iterator last) {
        if(first != last)
            transfer(position, first, last);
    }


    template<class T, class Alloc>
    void list<T, Alloc>::merge(list<T, Alloc>& x) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        //两个链表已经经过递增排序
        while(first1 != last1 && first2 != last2) {
            if(*first2 < *first1) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if(first2 != last2)
            transfer(last1, first2, last2);
     }

    template<class T, class Alloc>
    template<class Compare>
    void list<T, Alloc>::merge(list& x, Compare comp) {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = x.begin();
        iterator last2 = x.end();

        //两个链表已经经过递增排序
        while(first1 != last1 && first2 != last2) {
            if(comp(*first1, *first2)) {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
                ++first1;
        }
        if(first2 != last2)
            transfer(last1, first2, last2);
    }


    template<class T, class Alloc>
    void list<T, Alloc>::reverse() {
        if(node->next == node || link_type(node->next)->next == node)
            return;
        iterator first = begin();
        while(first != end()) {
            iterator last = first;
            ++first;
            transfer(begin(), last, first);
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::swap(list& rhs) {
        TinySTL::swap(node, rhs.node);
    }

    template<class T, class Alloc>
    void swap(list<T, Alloc>& lhs, list<T, Alloc>& rhs) {
        lhs.swap(rhs);
    }
    //private函数
    template<class T, class Alloc>
    void list<T, Alloc>::destroy_node(link_type p) {
        destroy(&p->data);
        put_node(p);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::link_type
    list<T, Alloc>::creat_node(const value_type& val) {
        link_type p = get_node();
        construct(&p->data, val);
        return p;
    }


    template<class T, class Alloc>
    void list<T, Alloc>::empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }
}

#endif // LIST_IMPL_H_INCLUDED
