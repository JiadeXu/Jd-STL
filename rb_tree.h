#ifndef _RB_TREE_H
#define _RB_TREE_H

#include "jd_macro.h"

#include <cstdlib>
#include <iostream>

JD_SPACE_BEGIN

enum COLOR {
    RED = 0,
    BLACK = 1,
    DOUBLE_BLACK = 2
};

// #define RED 0
// #define BLACK 1
// #define DOUBLE_BLACK 2

template<class V>
struct RbNode {
    typedef RbNode node_t;

    V data;
    COLOR color;
    node_t *lchild, *rchild, *father;
    RbNode(
        V data = V(),
        node_t *father = nullptr,
        node_t *lchild = nullptr,
        node_t *rchild = nullptr,
        COLOR color = RED
    ): data(data), father(father), lchild(lchild), rchild(rchild), color(color) {}

    static node_t __NIL;

    static node_t* Nil() {
        if (__NIL.lchild != &__NIL) {
            __NIL.color = BLACK;
            __NIL.father = __NIL.lchild = __NIL.rchild = &__NIL;
        }
        return &__NIL;
    }
};

template<class V>
RbNode<V> RbNode<V>::__NIL;

template<class V>
class rb_node_iterator {
public:
    typedef RbNode<V> node_t;
    typedef rb_node_iterator iterator;
private:
    node_t *ptr;
public:
    rb_node_iterator(node_t *ptr): ptr(ptr) {}
    
    bool operator!=(const iterator &other) {
        return ptr != other.ptr;
    }

    // ++i
    iterator& operator++() {
        ptr = next(ptr);
        return *this;
    }
    // i++
    iterator operator++(int) {
        iterator ret(ptr);
        ptr = next(ptr);
        return ret;
    }

    V& operator*() {
        return ptr->data;
    }
private:
    static node_t* next(node_t *p) {
        if (p->rchild != node_t::Nil()) {
            p = p->rchild;
            while(p->lchild != node_t::Nil()) {
                p = p->lchild;
            }
            return p;
        }
        while(p->father != node_t::Nil() && p->father->lchild != p) {
            p = p->father;
        }
        return p->father;
    }
    
};

template<class K, class V, class SKEY, class COMPK>
class rb_tree {
public:
    typedef RbNode<V> node_t;
    typedef rb_node_iterator<V> iterator;
public:
    rb_tree() {
        root.lchild = Nil();
        root.lchild->father = &root;
        root.father = Nil();
    }
    node_t* getRoot() {
        return root.lchild;
    }

    void insert(V data) {
        root.lchild = __insert(root.lchild, &root, data);
        root.lchild->color = BLACK;
    }

    void remove(V data) {
        root.lchild = __remove(root.lchild, data);
        root.lchild->color = BLACK;
    }

    iterator begin() {
        node_t *p = root.lchild;
        while(p->lchild != Nil()) {
            p = p->lchild;
        }
        return iterator(p);
    }

    iterator end() {
        return iterator(&root);
    }

    void output() {
        __output(root.lchild);
    }
    ~rb_tree() {
        clear(root.lchild);
    }

    node_t *find(const K &key) {
        node_t *p = root.lchild;
        while (p != Nil()) {
            if (key < getKey(p->data)) {
                p = p->lchild;
            } else if (getKey(p->data) < key) {
                p = p->rchild;
            } else {
                break;
            }
        }
        return p;
    }
private:
    node_t root;
    void clear(node_t *root) {
        if(root == Nil()) return ;
        clear(root->lchild);
        clear(root->rchild);
        delete root;
        return ;
    }
private:
    static node_t* Nil() {
        return node_t::Nil();
    }

    static node_t* get_new_node(V data, node_t *father) {
        return new node_t(data, father, Nil(), Nil(), RED);
    }

    static node_t* left_rotate(node_t *root) {
        node_t *temp = root->rchild;
        temp->father = root->father;
        root->rchild = temp->lchild;
        temp->lchild->father = root;
        temp->lchild = root;
        root->father = temp;
        return temp;
    }
    static node_t* right_rotate(node_t *root) {
        node_t *temp = root->lchild;
        temp->father = root->father;
        root->lchild = temp->rchild;
        temp->rchild->father = root;
        temp->rchild = root;
        root->father = temp;
        return temp;
    }

    static node_t* predecessor(node_t *root) {
        node_t *temp = root->lchild;
        while(temp->rchild != Nil()) {
            temp = temp->rchild;
        }
        return temp;
    }

    static bool has_red_child(node_t *root) {
        return root->lchild->color == RED || root->rchild->color == RED;
    }

    // 在祖父节点的角度消除双红
    static node_t* insert_maintain(node_t *root) {
        if (!has_red_child(root)) return root;
        // 叔叔节点为红色节点 即 祖父节点的两个子节点都是红色节点
        if (root->lchild->color == RED && root->rchild->color == RED) {
            if (!has_red_child(root->lchild) && !has_red_child(root->rchild)) {
                return root;
            }
            // 红色节点上浮
            root->color = RED;
            root->lchild->color = root->rchild->color = BLACK;
            return root;
        }
        // 判断当叔叔节点不为红色时是否有双红的情况
        if (root->lchild->color == RED && !has_red_child(root->lchild)) return root;
        if (root->rchild->color == RED && !has_red_child(root->rchild)) return root;

        if (root->lchild->color == RED) {
            if (root->lchild->rchild->color == RED) {
                // LR型
                root->lchild = left_rotate(root->lchild);
            }
            root = right_rotate(root);
        } else {
            if (root->rchild->lchild->color == RED) {
                // RL型
                root->rchild = right_rotate(root->rchild);
            }
            root = left_rotate(root);
        }
        root->color = RED;
        root->lchild->color = root->rchild->color = BLACK;
        return root;
    }

    static node_t* __insert(node_t *root, node_t *father, V data) {
        if (root == Nil()) return get_new_node(data, father);
        // if (root->data == key) return root;
        if (comp(getKey(data), getKey(root->data))) {
            root->lchild = __insert(root->lchild, root, data);
        } else if (comp(getKey(root->data), getKey(data))) {
            root->rchild = __insert(root->rchild, root, data);
        } else {
            return root;
        }
        return insert_maintain(root);
    }

    // 在父节点角度删除双重黑节点
    static node_t* remove_maintain(node_t *root) {
        if (root->lchild->color != DOUBLE_BLACK && root->rchild->color != DOUBLE_BLACK) {
            return root;
        }
        // 兄弟节点是红色的 通过节点调整为兄弟节点是红色的情况
        if (has_red_child(root)) {
            int flag; // 标识双重黑节点下沉的方向
            root->color = RED;
            if (root->lchild->color == DOUBLE_BLACK) {
                flag = 1;
                root = left_rotate(root);
            } else {
                flag = 0;
                root = right_rotate(root);
            }
            root->color = BLACK;
            if (flag == 1) root->lchild = remove_maintain(root->lchild);
            else root->rchild = remove_maintain(root->rchild);
            return root;
        }
        // 兄弟节点的子节点没有红色的节点的
        if ((root->lchild->color == DOUBLE_BLACK && !has_red_child(root->rchild)) || (root->rchild->color == DOUBLE_BLACK && !has_red_child(root->lchild))) {
            // 双黑上浮
            root->color += 1;
            root->lchild->color -= 1;
            root->rchild->color -= 1;
            return root;
        }
        // 
        if (root->lchild->color == DOUBLE_BLACK) {
            root->lchild->color -= 1;
            if (root->rchild->rchild->color != RED) {
                root->rchild->color = RED;
                root->rchild = right_rotate(root->rchild);
                root->rchild->color = BLACK;
            }
            root = left_rotate(root);
            root->color = root->lchild->color;
        } else {
            root->rchild->color -= 1;
            if (root->lchild->lchild->color != RED) {
                root->lchild->color = RED;
                root->lchild = left_rotate(root->lchild);
                root->lchild->color = BLACK;
            }
            root = right_rotate(root);
            root->color = root->rchild->color;
        }
        root->lchild->color = root->rchild->color = BLACK;
        return root;
    }

    static node_t* __remove(node_t *root, V data) {
        if (root == Nil()) return root;
        if (comp(getKey(data), getKey(root->data))) {
            root->lchild = __remove(root->lchild, data);
        } else if (comp(getKey(root->data), getKey(data))) {
            root->rchild = __remove(root->rchild, data);
        } else {
            if (root->lchild == Nil() || root->rchild == Nil()) {
                node_t *temp = root->lchild != Nil() ? root->lchild : root->rchild;
                temp->color += root->color; // 这里会产生双重黑
                delete root;
                return temp;
            } else {
                node_t *temp = predecessor(root);
                root->data = temp->data;
                root->lchild = __remove(root->lchild, root->data);
            }
        }
        return remove_maintain(root);
    }

    static void __output(node_t *root) {
        if(root == Nil()) return ;
        std::cout << root->color << " | " << getKey(root->data) << ", " << getKey(root->lchild->data) << ", " << getKey(root->rchild->data) << std::endl;
        __output(root->lchild);
        __output(root->rchild);
        return ;
    }

    static COMPK comp;
    static SKEY getKey;
};

template<class K, class V, class SKEY, class COMPK>
COMPK rb_tree<K, V, SKEY, COMPK>::comp;
template<class K, class V, class SKEY, class COMPK>
SKEY rb_tree<K, V, SKEY, COMPK>::getKey;


template<class V>
struct less {
    bool operator()(const V &a, const V &b) const {
        return a < b;
    }
};

template<class V>
struct SelectFirst {
    auto operator()(const V &a) -> decltype(a.first) {
        return a.first;
    }
};

template<class V>
struct SelectRow {
    auto operator()(const V a) {
        return a;
    }
};

// #undef DOUBLE_BLACK
// #undef BLACK
// #undef RED

JD_SPACE_END
#endif