#ifndef MULTISET_H_INCLUDED
#define MULTISET_H_INCLUDED

namespace TinySTL
{
    template<class Key, class Compare = less<Key>, class Alloc = alloc>
    class multiset {
    public:
        using key_type          = Key;
        using value_type        = Key;
        using key_compare       = Compare;
        using value_compare     = Compare;
        using rep_type          = rb_tree<key_type, value_type, identity<value_type>, Compare, Alloc>;

    private:
        rep_type tree;

    public:
        using pointer           = typename rep_type::pointer;
        using const_pointer     = typename rep_type::const_pointer;
        using reference         = typename rep_type::reference;
        using const_reference   = typename rep_type::const_reference;
        using iterator          = typename rep_type::const_iterator;
        using const_iterator    = typename rep_type::const_iterator;
        using size_type         = typename rep_type::size_type;
        using difference_type   = typename rep_type::difference_type;

        multiset() : tree(Compare()) {}
        explicit multiset(const Compare& comp) : tree(comp) {}
        template<class InputIterator>
        multiset(InputIterator first, InputIterator last, Compare comp) : tree(comp)
                { tree.insert_equal(first, last); }
        template<class InputIterator>
        multiset(InputIterator first, InputIterator last) : tree(Compare())
                { tree.insert_equal(first, last); }
        multiset(const multiset& rhs) : tree(rhs.tree) {}
        multiset& operator=(const multiset& rhs) { tree = rhs.tree; return *this; }

    public:
        key_compare key_comp() const { return tree.key_comp(); }
        value_compare value_comp() const { return tree.key_comp(); }

        iterator begin() const { return tree.cbegin(); }
        iterator end() const { return tree.cend(); }
        const_iterator cbegin() const { return tree.cbegin(); }
        const_iterator cend() const { return tree.cend(); }

        bool empty() const { return tree.empty(); }
        size_type size() const { return tree.size(); }
        size_type max_size() const { return tree.max_size(); }

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
        iterator find(const key_type& k) const { return tree.find(k); }
        size_type count(const key_type& k) const { return tree.count(k); }
        iterator lower_bound(const key_type& k) const { return tree.lower_bound(k); }
        iterator upper_bound(const key_type& k) const { return tree.upper_bound(k); }
        pair<iterator, iterator> equal_range(const key_type& k) const {
            return tree.equal_range(k);
        }

    private:
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator == (const multiset<Key_, Compare_, Alloc_>& lhs,
                                 const multiset<Key_, Compare_, Alloc_>& rhs);
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator != (const multiset<Key_, Compare_, Alloc_>& lhs,
                                 const multiset<Key_, Compare_, Alloc_>& rhs);
    };

    template<class Key, class Compare, class Alloc>
    bool operator == (const multiset<Key, Compare, Alloc>& lhs,
                      const multiset<Key, Compare, Alloc>& rhs) {
        return lhs.tree == rhs.tree;
    }

    template<class Key, class Compare, class Alloc>
    bool operator != (const multiset<Key, Compare, Alloc>& lhs,
                      const multiset<Key, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }
}

#endif // MULTISET_H_INCLUDED
