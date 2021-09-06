#ifndef _JD_SET_
#define _JD_SET_
#include "rb_tree.h"

JD_SPACE_BEGIN

template<class K, class COMP = less<K>>
class set {

public:
    typedef K value_type;
    typedef rb_tree<K, value_type, SelectRow<K>, COMP> BASE;
    typedef typename BASE::iterator iterator;
    typedef typename BASE::node_t node_t;
private:
    BASE tree;
public:
    void insert(K data ) {
        tree.insert(data);
    }
    iterator begin() {
        return tree.begin();
    }
    iterator end() {
        return tree.end();
    }
    void output() {
        tree.output();
    }
};

JD_SPACE_END

#endif