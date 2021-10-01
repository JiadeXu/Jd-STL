#ifndef _JD_MAP_
#define _JD_MAP_

#include "default_alloc_template.h"
#include "jd_rb_tree.h"
#include "jd_function.h"
#include <__functional_base>
#include <utility>

JD_SPACE_BEGIN
// 不可以通过迭代器改变 map 的元素，元素的键值关系到红黑树的整体结构，但可以改变map的元素的实值
// 所以map的迭代器不是 const iterator 也不是 multable iterators

template<class Key, class T, class Compare = JD::JDLess<Key>, class Alloc = JD::alloc>
class map {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef T mapped_type;
    typedef std::pair<const Key, T> value_type;
    typedef Compare key_compare;

    // 定义functor 元素比较函数
    class value_compare : public std::binary_function<value_type, value_type, bool> {
    protected:
        Compare comp;
        value_compare(Compare c): comp(c) {}
    public:
        bool operator()(const value_type &x, const value_type &y) const {
            return comp(x.first, y.first);
        }
    };
private:
    // 以下定义表述型别
    typedef JD::my_rb_tree<key_type, value_type, JD::select1st<value_type>, key_compare, Alloc> rep_type;
    rep_type tree;
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    map(): tree(Compare()) {}
    explicit map(const Compare &comp): tree(comp) {}

    template<class InputIterator>
    map(InputIterator first, InputIterator last): tree(Compare()) {
        tree.insert_unique(first, last);
    }

    template<class InputIterator>
    map(InputIterator first, InputIterator last, const Compare &comp): tree(comp) {
        tree.insert_unique(first, last);
    }

    map<Key, T, Compare, Alloc>(const map<Key, T, Compare, Alloc> &x): tree(x.tree) {}

    map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc> &x) {
        tree = x.tree;
        return *this;
    }

    key_compare key_comp() const { return tree.key_comp(); }
    value_compare value_comp() const { return value_compare(tree.key_comp()); }
    iterator begin() { return tree.begin(); }
    const_iterator begin() const { return tree.begin(); };
    iterator end() { return tree.end(); }
    const_iterator end() const { return tree.end(); };

    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }
    size_type max_size() const { return tree.max_size(); }
    
    T& operator[](const key_type &k) {
        return (*(insert(std::pair<key_type, T>(k, T())).first)).second;
    }

    // insert / erase
    std::pair<iterator, bool> insert(const value_type &x) {
        return tree.insert_unique(x);
    }

    template<class InputIterator>
    iterator insert(InputIterator first, InputIterator last) {
        tree.insert_unique(first, last);
    }

    iterator find(const key_type &k) { return tree.find(k); }
    size_type count(const key_type &k) const { return tree.count(k); }
    iterator lower_bound(const key_type &k) { return tree.lower_bound(k); }
    iterator upper_bound(const key_type &k) { return tree.upper_bound(k); }

    std::pair<iterator, iterator> equal_range(const key_type &k) {
        return tree.equal_range(k);
    }
    friend bool operator==(const map&, const map&);
    // friend bool operator<(const map&, const map&);
};

template<class Key, class T, class Compare, class Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc> &x, const map<Key, T, Compare, Alloc> &y) {
    return x.tree == y.tree;
}

// template<class Key, class T, class Compare, class Alloc>
// inline bool operator<(const map<Key, T, Compare, Alloc> &x, const map<Key, T, Compare, Alloc> &y) {
//     return x.tree < y.tree;
// }

JD_SPACE_END

#endif