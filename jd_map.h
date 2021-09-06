#ifndef _JD_MAP_
#define _JD_MAP_

#include "rb_tree.h"
JD_SPACE_BEGIN

template<class K, class V, class COMPK = less<K>>
class map {
public:
    typedef std::pair<K, V> value_type;
    typedef rb_tree<K, value_type, SelectFirst<value_type>, COMPK> BASE;
    typedef typename BASE::iterator iterator;
    typedef typename BASE::node_t node_t;
private:
    BASE tree;
public:
    V& operator[](const K &key) {
        node_t *p = tree.find(key);
        if (p != node_t::Nil()) {
            return p->data.second;
        }
        tree.insert(value_type(key, V()));
        return tree.find(key)->data.second;
    }

    iterator begin() {
        return tree.begin();
    }

    iterator end() {
        return tree.end();
    }

    iterator find(const K &key) {
        node_t *p = tree.find(key);
        if (p == node_t::Nil()) {
            return tree.end();
        }
        return iterator(p);
    }
    void output() {
        std::cout << "map output " << std::endl;
        tree.output();
    }
};

JD_SPACE_END

#endif