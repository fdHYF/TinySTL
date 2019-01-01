#ifndef MULTIMAP_H_INCLUDED
#define MULTIMAP_H_INCLUDED

namespace TinySTL
{
    template<class Key, class T, class Compare = less<Key>, class Alloc = alloc>
    class multimap {
    public:
        using key_type              = Key;
        using data_type             = T;
        using value_type            = pair<const Key, T>;
        using key_compare           = Compare;

        //value_typeµÄ±È½Ïº¯Êý
        class value_compare : public binary_function<value_type, value_type, bool> {
        friend class multimap<Key, T, Compare, Alloc>;
        protected:
            Compare comp;
            value_compare(Compare c) : comp(c) {}
        public:
            bool operator()(const value_type& x, const value_type& y) const {
                return comp(x.first, y.first);
            }
        };

    private:
        using rep_type              = rb_tree<key_type, value_type, select1st<value_type>, Compare, Alloc>;
        rep_type tree;

    public:
        using pointer               = typename rep_type::pointer;
        using const_pointer         = typename rep_type::const_pointer;
        using reference             = typename rep_type::reference;
        using const_reference       = typename rep_type::const_reference;
        using size_type             = typename rep_type::size_type;
        using difference_type       = typename rep_type::difference_type;
        using iterator              = typename rep_type::iterator;
        using const_iterator        = typename rep_type::const_iterator;

        multimap() : tree(Compare()) {}
        explicit multimap(const Compare& comp) : tree(comp) {}
        template<class InputIterator>
        multimap(InputIterator first, InputIterator last) : tree(Compare())
                { tree.insert_equal(first, last); }
        template<class InputIterator>
        multimap(InputIterator first, InputIterator last, const Compare& comp) : tree(comp)
                { tree.insert_equal(first, last); }
        multimap(const multimap& rhs) : tree(rhs.tree) {}
        multimap& operator=(const multimap& rhs) { tree = rhs.tree; return *this; }

    public:
        key_compare key_comp() const { return tree.key_comp(); }
        value_compare value_comp() const { return value_compare(tree.key_comp()); }
        iterator begin() { return tree.begin(); }
        iterator end() { return tree.end(); }
        const_iterator begin() const { return tree.cbegin(); }
        const_iterator end() const { return tree.cend(); }
        const_iterator cbegin() const { return tree.cbegin(); }
        const_iterator cend() const { return tree.cend(); }
        bool empty() const { return tree.empty(); }
        size_type size() const { return tree.size(); }
        size_type max_size() const { return tree.max_size(); }
        T& operator[](const key_type& k) {
            return (*((insert(value_type(k, T()))).first)).second;
        }

        pair<iterator, bool> insert(const value_type& value) {
            return tree.insert_equal(value);
        }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            tree.insert_equal(first, last);
        }

        iterator erase(iterator position) { return tree.erase(position); }
        iterator erase(iterator first, iterator last) { return tree.erase(first, last); }
        size_type erase(const key_type& k) { return tree.erase(k); }
        void clear() { tree.clear(); }

        iterator find(const key_type& k) { return tree.find(k); }
        size_type count(const key_type& k) const { return tree.find(k); }
        const_iterator find(const key_type& k) const { return tree.fin(k); }
        iterator lower_bound(const key_type& k) { return tree.lower_bound(k); }
        iterator upper_bound(const key_type& k) { return tree.upper_bound(k); }
        pair<iterator, iterator> equal_range(const key_type& k) {return tree.equal_range(k); }
        const_iterator lower_bound(const key_type& k) const
                { return tree.lower_bound(k); }
        const_iterator upper_bound(const key_type& k) const
                { return tree.upper_bound(k); }
        pair<const iterator, const iterator> equal_range(const key_type& k) const
                { return tree.equal_range(k); }

    private:
        template<class Key_, class T_, class Compare_, class Alloc_>
        friend bool operator == (const multimap<Key_, T_, Compare_, Alloc_>& lhs,
                                 const multimap<Key_, T_, Compare_, Alloc_>& rhs);
        template<class Key_, class T_, class Compare_, class Alloc_>
        friend bool operator != (const multimap<Key_, T_, Compare_, Alloc_>& lhs,
                                 const multimap<Key_, T_, Compare_, Alloc_>& rhs);
    };

    template<class Key, class T, class Compare, class Alloc>
    bool operator == (const multimap<Key, T, Compare, Alloc>& lhs,
                      const multimap<Key, T, Compare, Alloc>& rhs) {
        return lhs.tree == rhs.tree;
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator != (const multimap<Key, T, Compare, Alloc>& lhs,
                      const multimap<Key, T, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }
}



#endif // MULTIMAP_H_INCLUDED
