#ifndef DEQUE_IMPL_H_INCLUDED
#define DEQUE_IMPL_H_INCLUDED

namespace TinySTL
{
    namespace Base
    {
        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::difference_type
        _deque_iterator<T, BufSize>::operator-(const self& rhs) const {
            return difference_type(buffer_size()) * (node - rhs.node - 1) +
                (cur - first) + (rhs.last - rhs.cur);
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self&
        _deque_iterator<T, BufSize>::operator++() {
            ++cur;
            if(cur == last) {
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self
        _deque_iterator<T, BufSize>::operator++(int) {
            self temp = *this;
            ++*this;
            return temp;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self&
        _deque_iterator<T, BufSize>::operator--() {
            if(cur == first) {
                set_node(node -1);
                cur = last;
            }
            --cur;
            return *this;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self
        _deque_iterator<T, BufSize>::operator--(int) {
            self tmp = *this;
            --*this;
            return tmp;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self&
        _deque_iterator<T, BufSize>::operator+=(difference_type n) {
            difference_type offset = n + (cur - first);
            if(offset >= 0 && offset < difference_type(buffer_size())) {
                //目标位置在同一缓冲区内
                cur += n;
            }
            else {
                difference_type node_offset =
                    offset > 0 ? offset / difference_type(buffer_size())
                                : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + offset - node_offset * difference_type(buffer_size());
            }
            return *this;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self
        _deque_iterator<T, BufSize>::operator+(difference_type n) const {
            self tmp = *this;
            return tmp += n;
        }

        template<class T, size_t BufSize>
        typename _deque_iterator<T, BufSize>::self
        _deque_iterator<T, BufSize>::operator-(difference_type n) const{
            self tmp = *this;
            return tmp -= n;
        }

        template<class T, size_t BufSize>
        void _deque_iterator<T, BufSize>::set_node(map_pointer new_node) {
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }
    }//end of Base

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::deque() : start(), finish(), map(0), map_size(0) {
        creat_map_and_nodes(0);
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::deque(size_type n, const value_type& value) {
        deque();
        fill_initialize(n, value);
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::deque(size_type n) {
        deque();
        fill_initialize(n, value_type());
    }

    template<class T, class Alloc, size_t BufSize>
    template<class InputIterator>
    deque<T, Alloc, BufSize>::deque(InputIterator first, InputIterator last) {
        deque();
        for(auto ptr = first; ptr != last; ++ptr)
            *this.push_buck(*ptr);
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::deque(const deque& rhs) {
        deque();
        auto ptr = rhs.begin();
        for(; ptr != rhs.end(); ++ptr) {
            push_back(*ptr);
        }
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>&
    deque<T, Alloc, BufSize>::operator=(const deque& rhs) {
        if(this != &rhs) {
            clear();
            auto ptr = rhs.begin();
            for(; ptr != rhs.end(); ++ptr) {
                push_back(*ptr);
            }
        }
        return *this;
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>&
    deque<T, Alloc, BufSize>::operator=(deque&& rhs) {
        if(this != &rhs) {
            *this.swap(rhs);
        }
        return *this;
    }

    template<class T, class Alloc, size_t BufSize>
    deque<T, Alloc, BufSize>::~deque() {
        clear();
        delete[] map;
    }

    //public member function
    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::reference
    deque<T, Alloc, BufSize>::back() {
        auto tmp = finish;
        --tmp;
        return *tmp;
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_back(const value_type& value) {
        if(finish.cur != finish.last - 1){
            construct(finish.cur, value);
            ++finish.cur;
        }
        else
            push_back_aux(value);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_front(const value_type& value) {
        if(start.cur != start.first) {
            construct(start.cur - 1, value);
            --start.cur;
        }
        else
            push_front_aux(value);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::pop_back() {
        if(finish.cur != finish.first) {
            --finish.cur;
            destroy(finish.cur);
        }
        else
            pop_back_aux();
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::pop_front() {
        if(start.cur != start.last - 1) {
            destroy(start.cur);
            ++start.cur;
        }
        else
            pop_front_aux();
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::clear() {
        for(auto node = start.node + 1; node < finish.node; ++node) {
            destroy(*node, *node + buffer_size());
            data_allocator::deallocate(*node, buffer_size());
        }

        if(start.node != finish.node) {
            destroy(start.cur, start.last);
            destroy(finish.first, finish.cur);
            data_allocator::deallocate(finish.first, buffer_size());
        }
        else
            destroy(start.cur, finish.cur);

        finish = start;
    }


    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::erase(iterator position) {
        iterator next = position;
        ++position;
        difference_type index = position - start;
        if(index < (size() >> 1)) {
            std::copy_backward(start, position, next);
            pop_front();
        }
        else {
            std::copy(next, finish, position);
            pop_back();
        }

        return start + index;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::erase(iterator first, iterator last) {
        if(first == start && last == finish) {
            clear();
            return finish;
        }
        else {
            difference_type n = first - last;
            difference_type elems_before = first - start;

            if(elems_before < (size() - n) / 2) {
                std::copy_backward(start, first, last);
                iterator new_start = start + n;
                destroy(start, new_start);

                for(map_pointer cur  = start.node; cur < new_start.node; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                start = new_start;
            }
            else {
                copy(last, finish, first);
                iterator new_finish = finish - n;
                destroy(new_finish, finish);
                for(map_pointer cur = new_finish + 1; cur <= finish; ++cur) {
                    data_allocator::deallocate(*cur, buffer_size());
                }
                finish = new_finish;
            }
             return start + elems_before;
        }
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::insert(iterator position, const value_type& value) {
        if(position.cur == start.cur) {
            push_front(value);
            return start;
        }
        else if(position.cur == finish.cur) {
            push_back(value);
            iterator tmp = finish;
            --tmp;
            return tmp;
        }
        else
            insert_aux(position, value);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::insert(iterator position, size_type n, const value_type& value) {
        for(int i = 0; i < n; ++i)
            position = insert(position, value);
    }

    template<class T, class Alloc, size_t BufSize>
    template<class InputIterator>
    void deque<T, Alloc, BufSize>::insert(iterator position, InputIterator first, InputIterator last) {
        auto ptr = first;
        for(; ptr != last; ++ptr) {
            position = insert(position, *ptr);
        }
    }


    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::swap(deque& rhs) {
        TinySTL::swap(map, rhs.map);
        TinySTL::swap(map_size, rhs.map_size);
        TinySTL::swap(start, rhs.start);
        TinySTL::swap(finish, rhs.finish);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::shrink_to_fit() {
        data_allocator::deallocate(finish.cur, finish.last - finish.cur);
        finish.last = finish.cur;
        data_allocator::deallocate(start.first,start.cur - start.first);
        start.first = start.cur;
    }

    //private member function
    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::fill_initialize(size_type n, const value_type& value) {
        creat_map_and_nodes(n);
        map_pointer cur;
        try {
            for(cur = start.node; cur != finish.node; ++cur)
                uninitialized_fill(*cur, *cur + buffer_size(), value);
            uninitialized_fill(finish.first, finish.last, value);
        }
        catch(...) {
            for(cur = start.node; cur <= finish.node; ++cur) {
                data_allocator::deallocate(*cur, buffer_size());
            }
        }
    }


    template<class T,class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::creat_map_and_nodes(size_type n) {
        size_type num_nodes = n / buffer_size() + 1;

        //一个map管理几个节点。最少8个，最多是“所需节点加2”
        map_size = std::max(initial_map_size(), num_nodes + 2);
        map = map_allocator::allocate(map_size);

        map_pointer nstart = map + (map_size - num_nodes) / 2;
        map_pointer nfinish = nstart + num_nodes - 1;
        map_pointer cur;

        try {
            for(cur = nstart; cur <= nfinish; ++cur) {
                *cur = allocate_node();
            }
        }
        catch(...) {
            for(cur = nstart; cur <= nfinish; ++cur) {
                deallocate_node(*cur);
            }
        }

        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur = start.first;
        finish.cur = finish.first + n % buffer_size();
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_front_aux(const value_type& value) {
        reverse_map_at_front();
        *(start.node - 1) = allocate_node();
        try {
            start.set_node(start.node - 1);
            start.cur = start.last - 1;
            construct(start.cur, value);
        }
        catch(...) {
            start.set_node(start.node + 1);
            start.cur = start.first;
            deallocate_node(*(start.node - 1));
        }
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::push_back_aux(const value_type& value) {
        reverse_map_at_back();
        *(finish.node + 1) = allocate_node();
        try {
            construct(finish.cur, value);
            finish.set_node(finish.node + 1);
            finish.cur = finish.first;
        }
        catch(...) {
            finish.set_node(finish.node - 1);
            finish.cur = finish.last;
            destroy(finish.cur);
            deallocate_node(*(finish.node +1));
        }
    }


    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::reverse_map_at_back(size_type nodes_to_add) {
        if(nodes_to_add + 1 > map_size - (finish.node - map))
            reallocate_map(nodes_to_add, false);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::reverse_map_at_front(size_type nodes_to_add) {
        if(nodes_to_add > size_type(start.node - map))
            reallocate_map(nodes_to_add, true);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
        size_type old_num_nodes = finish.node - start.node + 1;
        size_type new_num_nodes = old_num_nodes + nodes_to_add;

        map_pointer new_start;
        if(map_size > 2 * new_num_nodes) {
            new_start = map + (map_size - new_num_nodes) / 2
                        + (add_at_front ? nodes_to_add : 0);
            if(new_start < start.node)
                std::copy(start.node, finish.node + 1, new_start);
            else
                std::copy_backward(start.node, finish.node + 1, new_start + old_num_nodes);
        }
        else {
            size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start = new_map + (new_map_size - new_num_nodes) / 2
                        + (add_at_front ? nodes_to_add : 0);
            std::copy(start.node, finish.node + 1, new_start);
            map_allocator::deallocate(map, map_size);
            map = new_map;
            map_size = new_map_size;
        }

        start.set_node(new_start);
        finish.set_node(new_start + old_num_nodes - 1);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::pop_back_aux() {
        deallocate_node(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destroy(finish.cur);
    }

    template<class T, class Alloc, size_t BufSize>
    void deque<T, Alloc, BufSize>::pop_front_aux() {
        destroy(start.cur);
        deallocate_node(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }

    template<class T, class Alloc, size_t BufSize>
    typename deque<T, Alloc, BufSize>::iterator
    deque<T, Alloc, BufSize>::insert_aux(iterator pos, const value_type& value) {
        difference_type index = pos - start;
        if(index < size() / 2) {
            push_front(front());
            iterator first1 = start;
            ++first1;
            iterator first2 = first1;
            ++first2;
            pos = start + index;
            iterator pos1 = pos;
            ++pos1;
            std::copy(first2, pos1, first1);
        }
        else {
            push_back(back());
            iterator back1 = finish;
            --back1;
            iterator back2 = back1;
            --back2;
            pos = start + index;
            std::copy_backward(pos, back2, back1);
        }
        *pos = value;
        return pos;
    }
}

#endif // DEQUE_IMPL_H_INCLUDED
