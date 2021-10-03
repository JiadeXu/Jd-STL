#ifndef _JD_SET_
#define _JD_SET_

#include "jd_macro.h"
#include "jd_algorithm.h"
#include "default_alloc_template.h"
#include "jd_function.h"
#include "jd_rb_tree_sgi.h"
#include "jd_rb_tree.h"

JD_SPACE_BEGIN

// 默认以递增
template<class Key, class Compare = JDLess<Key>, class Alloc = JD::alloc>
class set {
public:
    typedef Key key_type;
    typedef Key value_type;
    // 以下 key_compare 和 value_compare 使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    // typedef JD::rb_tree<Key, value_type, identity<value_type>, key_compare, Alloc> rep_type;
    typedef JD::my_rb_tree<Key, value_type, identity<value_type>, key_compare, Alloc> rep_type;

    rep_type tree;
public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    // set的迭代器无法执行写入操作，set的元素有一定的次序安排
    // 不允许用户在任意处进行写入操作
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocate/deallocate
    // set一定使用rb-tree的insert_unique 
    // multiset 才使用 insert_equal
    // set的键值是唯一的
    set(): tree(Compare()) {}
    explicit set(const Compare &comp): tree(comp) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last): tree(Compare()) {
        for (; first != last; first++) {
            tree.insert_unique(*first);
        }
    }

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const Compare& comp): tree(comp) {
        for (; first != last; first++) {
            tree.insert_unique(*first);
        }
    }

    set(const set &x) : tree(x.tree) {}

    set& operator=(const set &x) {
        tree = x.tree;
        return *this;
    }

    key_compare key_comp() const { return tree.key_comp(); }
    // value_comp 就是 key_comp
    value_compare value_comp() const { return tree.key_comp(); }
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }
    size_type max_size() const { return tree.max_size(); }
    void swap(set &x) { tree.swap(x.tree, tree); }

    // insert / erase
    typedef std::pair<iterator, bool> pair_iterator_bool;
    std::pair<iterator, bool> insert(const value_type &x) {
        std::pair<typename rep_type::iterator, bool> p = tree.insert_unique(x);
        return std::pair<iterator, bool>(p.first, p.second);
    }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for (; first != last; first++) {
            tree.insert_unique(*first);
        }
    }

    void show() { tree.preorder(); }
    
    void erase(iterator position) {
        // typedef typename rep_type::iterator rep_iterator;
        tree.erase(position);
    }
    size_type erase(const key_type &x) {
        return tree.erase(x);
    }
    void erase(iterator first, iterator last) {
        tree.erase(first, last);
    }
    void clear() { tree.clear(); }

    // set operations
    iterator find(const key_type &key) const { return tree.find(key); }
    size_type count(const key_type &x) { return tree.count(x); }
    iterator lower_bound(const key_type &x) const { return tree.lower_bound(x); }
    iterator upper_bound(const key_type &x) const { return tree.upper_bound(x); }
    std::pair<iterator, iterator> equal_range(const key_type &k) { return tree.equal_range(k); }
};

// 默认以递增
template<class Key, class Compare = JDLess<Key>, class Alloc = JD::alloc>
class multiset {
public:
    typedef Key key_type;
    typedef Key value_type;
    // 以下 key_compare 和 value_compare 使用同一个比较函数
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    // typedef JD::rb_tree<Key, value_type, identity<value_type>, key_compare, Alloc> rep_type;
    typedef JD::my_rb_tree<Key, value_type, identity<value_type>, key_compare, Alloc> rep_type;

    rep_type tree;
public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::const_iterator iterator;
    // multiset的迭代器无法执行写入操作，multiset的元素有一定的次序安排
    // 不允许用户在任意处进行写入操作
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    // allocate/deallocate
    // multiset一定使用rb-tree的insert_unique 
    // multimultiset 才使用 insert_equal
    // multiset的键值是唯一的
    multiset(): tree(Compare()) {}
    explicit multiset(const Compare &comp): tree(comp) {}

    template<class InputIterator>
    multiset(InputIterator first, InputIterator last): tree(Compare()) {
        for (; first != last; first++) {
            tree.insert_equal(*first);
        }
    }

    template<class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare& comp): tree(comp) {
        for (; first != last; first++) {
            tree.insert_equal(*first);
        }
    }

    multiset(const multiset &x) : tree(x.tree) {}

    multiset& operator=(const multiset &x) {
        tree = x.tree;
        return *this;
    }

    key_compare key_comp() const { return tree.key_comp(); }
    // value_comp 就是 key_comp
    value_compare value_comp() const { return tree.key_comp(); }
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }
    size_type max_size() const { return tree.max_size(); }
    void swap(multiset &x) { tree.swap(x.tree, tree); }

    // insert / erase
    typedef std::pair<iterator, bool> pair_iterator_bool;
    std::pair<iterator, bool> insert(const value_type &x) {
        std::pair<typename rep_type::iterator, bool> p = tree.insert_equal(x);
        return std::pair<iterator, bool>(p.first, p.second);
    }
    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        for (; first != last; first++) {
            tree.insert_equal(*first);
        }
    }

    void show() { tree.preorder(); }
    
    void erase(iterator position) {
        // typedef typename rep_type::iterator rep_iterator;
        tree.erase(position);
    }
    size_type erase(const key_type &x) {
        return tree.erase(x);
    }
    void erase(iterator first, iterator last) {
        tree.erase(first, last);
    }
    void clear() { tree.clear(); }

    // multiset operations
    iterator find(const key_type &key) const { return tree.find(key); }
    size_type count(const key_type &x) { return tree.count(x); }
    iterator lower_bound(const key_type &x) const { return tree.lower_bound(x); }
    iterator upper_bound(const key_type &x) const { return tree.upper_bound(x); }
    std::pair<iterator, iterator> equal_range(const key_type &k) { return tree.equal_range(k); }
};

JD_SPACE_END

#endif