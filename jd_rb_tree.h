/*************************************************************************
	> File Name: jd_rb_tree_r.h
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 28 13:46:32 2021
 ************************************************************************/

#ifndef _jd_rb_tree_R_H
#define _jd_rb_tree_R_H

#include "jd_alloc.h"
#include "jd_construct.h"
#include "default_alloc_template.h"
#include "jd_iterator.h"
#include "jd_macro.h"
#include <cstddef>
#include <iostream>
#include <utility>

JD_SPACE_BEGIN

typedef int my_rb_tree_color;

const my_rb_tree_color __red = 0;
const my_rb_tree_color __black = 1;
const my_rb_tree_color __double_black = 2;

template<class Value>
class my_tree_node {
public:
	typedef my_rb_tree_color color_type;
	typedef my_tree_node* node_ptr;

	node_ptr lchild;
	node_ptr rchild;
	node_ptr father;
	color_type color;

	Value value_field;

	static node_ptr minimum(node_ptr x) {
		while (x->lchild != Nil()) {
			x = x->lchild;
		}
		return x;
	}
	static node_ptr maximum(node_ptr x) {
		while (x->rchild != Nil()) {
			x = x->rchild;
		}
		return x;
	}

	static node_ptr Nil() {
		if (__NIL.lchild != &__NIL) {
			__NIL.lchild = __NIL.rchild = &__NIL;
			__NIL.color = __black;
		}
		return &__NIL;
	}

private:
	static my_tree_node __NIL;
};
template<class Value>
my_tree_node<Value> my_tree_node<Value>::__NIL;

template<class Value, class Ref, class Ptr>
struct jd_rb_tree_node_iterator {

	typedef ptrdiff_t difference_type;
	typedef JD::bidirectional_iterator_tag iterator_category;
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef jd_rb_tree_node_iterator<value_type, value_type&, value_type*> iterator;
	typedef jd_rb_tree_node_iterator<Value, Ref, Ptr> self;
	typedef my_tree_node<Value> base;
	typedef base* link_type;

	static link_type NIL() {
		return base::Nil();
	}

	link_type node;

	jd_rb_tree_node_iterator() {}

	jd_rb_tree_node_iterator(link_type x):node(x) {}
	jd_rb_tree_node_iterator(const iterator &x) { node = x.node; }

	void increament() {
		// 找后继
		if (node->rchild != NIL()) {
			node = node->rchild;
			while(node->lchild != NIL()) {
				node = node->lchild;
			}
		} else {
			while(node->father != NIL() && node->father->lchild != node) {
				node = node->father;
			}
			node = node->father;
		}
	}

	void decreament() {
		// 找前驱
		if (node->lchild != NIL()) {
			node = node->lchild;
			while (node->rchild != NIL()) {
				node = node->rchild;
			}
		} else {
			while(node->father != NIL() && node->father->rchild != node) {
				node = node->father;
			}
			node = node->father;
		}
	}

	reference operator*() {
		return node->value_field;
	}
	pointer operator->() {
		return &(operator*());
	}
	bool operator!=(const self &o) {
		return o.node != node;
	}
	bool operator==(const self &o) {
		return o.node == node;
	}
	// i++
	self operator++(int) {
		self tmp = *this;
		increament();
		return tmp;
	}
	// ++i
	self operator++() {
		increament();
		return *this;
	}
	// i--
	self operator--(int) {
		self tmp = *this;
		decreament();
		return tmp;
	}
	// --i
	self operator--() {
		decreament();
		return *this;
	}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = JD::alloc>
class my_rb_tree {
protected:
	typedef my_rb_tree_color color_type;
	typedef my_tree_node<Value> base;
	typedef JD::simple_alloc<base, Alloc> node_allocator;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef base* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef jd_rb_tree_node_iterator<value_type, reference, pointer> iterator;
	typedef const jd_rb_tree_node_iterator<value_type, reference, pointer> const_iterator;
protected:
	static link_type NIL() {
		return base::Nil();
	}
	static link_type& left(link_type root) { return (link_type &) root->lchild; }
	static link_type& right(link_type root) { return (link_type &) root->rchild; }
	static link_type& father(link_type root) { return (link_type &) root->father; }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(x->value_field); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }
private:
	size_type node_count;
	link_type tree_root;
	Compare key_compare;
public:
	my_rb_tree() {
		NIL();
	}
// public:
// 	Compare key_comp() const { return key_compare; }
// 	iterator begin() const { return lchildmost(); } // RB头节点 为最左的节点（最小）
// 	iterator end() const { return header; }
// 	bool empty() const { return node_count == 0; }
// 	size_type size() const { return node_count; }
// 	size_type max_size() const { return size_type(-1); }
// public:
// 	void insert_unique(const_iterator first, const_iterator last);
// 	iterator insert_equal(const value_type &v);
// 	std::pair<iterator, bool> insert_unique(const value_type &v);
// 	void erase(iterator pos);
// 	size_type erase(const Key &k);
// 	void erase(iterator first, iterator last);
// 	void erase(const Key *first, const Key *last);
// 	iterator find(const Key &k);
// 	// 测试函数
// 	前序遍历并输出左右子节点
// 	void __preorder(const base_ptr root) {
// 		if (root) {
// 			std::cout << root->color << " | " << key(root) << ", ";
// 			if (lchild(root) != 0) {
// 				std::cout << key(lchild(root)) << ", ";
// 			}
// 			if (rchild(root) != 0) {
// 				std::cout << key(rchild(root)) << ", ";
// 			}
// 			std::cout << std::endl;
// 			__preorder(lchild(root));
// 			__preorder(rchild(root));
// 		}
// 	}
// 	void preorder() {
// 		__preorder(root());
// 	}
};

JD_SPACE_END

#endif
