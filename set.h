#ifndef SET_H_INCLUDED
#define SET_H_INCLUDED

namespace TinySTL
{
    template<class Key, class Compare = less<Key>, class Alloc = alloc>
    class set {
    public:
        using key_type              = Key;
        using value_type            = Key;
        using key_compare           = Compare;
        using value_compare         = Compare;
        using rep_type              = rb_tree<key_type, value_type, identity<value_type>, Compare, Alloc>;

    public:
        using pointer               = typename rep_type::pointer;
        using const_pointer         = typename rep_type::const_pointer;
        using reference             = typename rep_type::reference;
        using const_reference       = typename rep_type::const_reference;
        using difference_type       = typename rep_type::difference_type;
        using size_type             = typename rep_type::size_type;
        using iterator              = typename rep_type::const_iterator;
        using const_iterator        = typename rep_type::const_iterator;

    public:
        set() : tree(Compare()) {}
        explicit set(const Compare& comp) : tree(comp) {}
        template<class InputIterator>
        set(InputIterator first, InputIterator last) : tree(Compare())
            { tree.insert_unique(first, last); }
        template<class InputIterator>
        set(InputIterator first, InputIterator last, const Compare* comp) : tree(comp)
            { tree.insert_unique(first, last); }
        set(const set<Key, Compare, Alloc>& x) : tree(x.tree) {}
        set& operator=(const set& rhs) { tree = rhs.tree; return *this; }

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

        //insert/erase
        pair<iterator, bool> insert(const value_type& value) {
            pair<iterator, bool> tmp = tree.insert_unique(value);
            return pair<iterator, bool>(tmp.first, tmp.second);
        }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            tree.insert_unique(first, last);
        }

        size_type erase(const value_type& value) { return tree.erase(value); }
        iterator erase(iterator first, iterator last) { return tree.erase(first, last); }
        iterator erase(iterator position) { return tree.erase(position); }
        void clear() { tree.clear(); }

        iterator find(const key_type& k) const { return tree.find(k); }
        size_type count(const key_type& k) const { return tree.count(k); }
        iterator lower_bound(const key_type& k) const { return tree.lower_bound(k); }
        iterator upper_bound(const key_type& k) const { return tree.upper_bound(k); }
        pair<iterator, iterator> equal_range(const key_type& k) const {
            return tree.equal_range(k);
        }
    private:
        rep_type tree;
    private:
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator == (const set<Key_, Compare_, Alloc_>& lhs,
                                 const set<Key_, Compare_, Alloc_>& rhs);
        template<class Key_, class Compare_, class Alloc_>
        friend bool operator != (const set<Key_, Compare_, Alloc_>& lhs,
                                 const set<Key_, Compare_, Alloc_>& rhs);
    };

    template<class Key, class Compare, class Alloc>
    bool operator == (const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
        return lhs.tree == rhs.tree;
    }

    template<class Key, class Compare, class Alloc>
    bool operator != (const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
        return !(lhs == rhs);
    }
}

#endif // SET_H_INCLUDED
