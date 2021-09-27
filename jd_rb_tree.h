/*************************************************************************
	> File Name: jd_rb_tree.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 26 22:32:53 2021
 ************************************************************************/

#ifndef _JD_RB_TREE_H
#define _JD_RB_TREE_H

#include "default_alloc_template.h"
#include "jd_alloc.h"
#include "jd_construct.h"
#include "jd_macro.h"
#include "jd_iterator.h"
#include <cstddef>
#include <iostream>
#include <utility>

JD_SPACE_BEGIN

typedef int __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = 0; // 红色为0
const __rb_tree_color_type __rb_tree_black = 1; // 黑色为1
const __rb_tree_color_type __rb_tree_double_black = 2; // 双重黑为2

struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;

	color_type color; // 节点颜色，非红即黑 双重黑
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while(x->left != 0) x= x->left;
		return x;
	}

	static base_ptr maximum(base_ptr x) {
		while (x->right != 0) x = x->right;
		return x;
	}
};

inline void __rb_tree_rotate_left(__rb_tree_node_base *x, __rb_tree_node_base *&root) {
	// x为旋转点
	__rb_tree_node_base *y = x->right;
	x->right = y->left;
	// y->left = x;
	if (y->left != 0) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x == root) {
		root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

inline void __rb_tree_rotate_right(__rb_tree_node_base *x, __rb_tree_node_base *&root) {
	__rb_tree_node_base *y = x->left;
	x->left = y->right;
	if (y->right != 0) {
		y->right->parent = x;
	}
	y->parent = x->parent;
	if (x == root) {
		root = y;
	} else if (x == x->parent->right) {
		x->parent->right = y;
	} else {
		x->parent->left = y;
	}
	y->right = x;
	x->parent = y;
}

inline void __rb_tree_reblance(__rb_tree_node_base *x, __rb_tree_node_base *&root) {
	x->color = __rb_tree_red; // 新节点必为红
	// 去除双红的情况
	while (x != root && x->parent->color == __rb_tree_red) {
		if (x->parent == x->parent->parent->left) { // 父节点为祖父的左节点
			// y指向叔父节点
			__rb_tree_node_base *y = x->parent->parent->right;
			if (y && y->color == __rb_tree_red) {
				// 红色节点上浮
				// 叔父节点存在且为红色节点 祖父变为红节点 父节点和叔父节点 变为黑节点
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				// 跳到路径上的下一个红节点
				x = x->parent->parent;
			} else {
				// 无叔父节点 或叔父节点为黑
				if (x == x->parent->right) {
					// 新节点为父节点的右节点 LR型
					x = x->parent;
					__rb_tree_rotate_left(x, root); // 第一参数为左旋点
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_right(x->parent->parent, root);
			}
		} else {
			// 插入点的父节点为 祖父节点的右节点
			__rb_tree_node_base *y = x->parent->parent->left;
			if (y && y->color == __rb_tree_red) {
				// 红色节点上浮
				// 叔父节点存在且为红色节点 祖父变为红节点 父节点和叔父节点 变为黑节点
				x->parent->color = __rb_tree_black;
				y->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				// 跳到路径上的下一个红节点
				x = x->parent->parent;
			} else {
				// 无叔父节点或者叔父节点为黑色
				if (x == x->parent->left) {
					// RL型
					x = x->parent;
					__rb_tree_rotate_right(x, root);
				}
				x->parent->color = __rb_tree_black;
				x->parent->parent->color = __rb_tree_red;
				__rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = __rb_tree_black; // 根节点永远为黑色节点
}

template<class Value>
struct __rb_tree_node : public __rb_tree_node_base {
	typedef __rb_tree_node<Value>* link_type;
	Value value_field; // 节点值
};

// 基层迭代器
struct __rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr base_ptr;
	typedef JD::bidirectional_iterator_tag iterator_category;
	typedef ptrdiff_t difference_type;

	base_ptr node; // 用来与容器之间产生连接关系

	// 递增与递减 都是已中序遍历为准
	// increament 只会在operator++中调用； 找后继
	void increament() {
		if (node->right != 0) {
			node = node->right;
			while(node->left != 0) {
				node = node->left;
			}
		} else {
			base_ptr y = node->parent;
			// 当前节点是父节点的右节点就一直向上走
			while(node == y->right) {
				node = y;
				y = y->parent;
			}
			if (node->right != y) {
				// 为了应付一种特殊情况：寻找根节点的下一节点，而恰巧根节点无右子节点
				// 当然，以上特殊做法必须配合 rb-tree根节点与特殊节点 header之间的特殊关系
				node = y;
			}
		}
	}

	// 找前驱
	void decreament() {
		if (node->color == __rb_tree_red && node->parent->parent == node) {
			// 当这是一颗空树时 即虚拟节点，即当前红黑树的 end()
			node = node->right;
		} else if (node->left != 0) {
			// 找前驱
			base_ptr y = node->left;
			while(y->right != 0) {
				y = y->right;
			}
			node = y;
		} else {
			// 非根节点 且没有 左子节点
			base_ptr y = node->parent;
			while(node == y->left) {
				node = y;
				y = y->parent;
			}
			node = y;
		}
	}
};

// 正视得带器
template<class Value, class Ref, class Ptr>
struct __rb_tree_iterator : public __rb_tree_base_iterator {
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __rb_tree_iterator<value_type, value_type&, value_type*> iterator;
	typedef __rb_tree_iterator<value_type, const value_type&, const value_type*> const_iterator;
	typedef __rb_tree_iterator<Value, Ref, Ptr> self;
	typedef __rb_tree_node<value_type>* link_type;

	__rb_tree_iterator() {}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator &x) { node = x.node; }

	reference operator*() { return ((link_type) node)->value_field; }
#ifndef __JD_STL_NO_ARROW_OPERATOR
	pointer operator->() const { return &(operator*()); }
#endif
	bool operator!=(const self &oth) {
		return oth.node != node;
	}
	bool operator==(const self &oth) {
		return oth.node == node;
	}
	// ++i
	self operator++() { increament(); return *this; }
	// i++
	self operator++(int) {
		self tmp = *this;
		increament();
		return tmp;
	}

	// --i
	self operator--() { decreament(); return *this; }
	// i--
	self operator--(int) {
		self tmp = *this;
		decreament();
		return tmp;
	}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = JD::alloc>
class rb_tree {
protected:
	typedef void* void_pointer;
	typedef __rb_tree_node_base* base_ptr;
	typedef __rb_tree_node<Value> rb_tree_node;
	typedef JD::simple_alloc<rb_tree_node, Alloc> rb_tree_node_allocator;
	typedef __rb_tree_color_type color_type;
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef rb_tree_node* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_ndoe(link_type p) { rb_tree_node_allocator::deallocate(p); }
	
	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		__JD_TRY {
			JD::construct(&tmp->value_field, x);
		}
		__JD_UNWIND(put_ndoe(p));
		
		return tmp;
	}

	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}

	void destory_node(link_type node) {
		JD::destory(&node->value_field); // 析构内容
		put_ndoe(node); // 释放空间
	}
protected:
	// RB-tree 只以三笔数据表现
	size_type node_count; // 节点数
	link_type header; // 虚拟节点
	Compare key_compare; // 节点比较函数 都是用小于

	// 以下函数用来获取 header的成员
	link_type& root() { return (link_type &) header->parent; }
	link_type& leftmost() { return (link_type &) header->left; }
	link_type& rightmost() { return (link_type &) header->right; }

	// 以下用来访问指定节点的成员
	static link_type& left(link_type x) { return (link_type &)x->left; }
	static link_type& right(link_type x) { return (link_type &)x->right; }
	static link_type& parent(link_type x) { return (link_type &)x->parent; }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(x->value_field); }
	static color_type& color(link_type x) { return (color_type&)(x->color); }

	static link_type& left(base_ptr x) {
		return (link_type &)(x->left);
	}
	static link_type& right(base_ptr x) {
		return (link_type &)(x->right);
	}
	static link_type& parent(base_ptr x) {
		return (link_type &)(x->parent);
	}
	static reference value(base_ptr x) {
		return (static_cast<link_type>(x))->value_field;
	}
	static const Key& key(base_ptr x) {
		return KeyOfValue()(value(static_cast<link_type>(x)));
	}
	static color_type& color(base_ptr x) { return (color_type&)((static_cast<link_type>(x))->color); }

	static link_type minimum(link_type x) {
		return static_cast<link_type>(__rb_tree_node_base::minimum(x)) ;
	}
	static link_type maximum(link_type x) {
		return static_cast<link_type>(__rb_tree_node_base::maximum(x));
	}
public:
	typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
private:
	iterator __insert(base_ptr x, base_ptr y, const value_type &v);
	link_type copy(link_type x, link_type p);
	void __erase(link_type x);
	void init() {
		header = get_node(); // 产生一个节点空间
		color(header) = __rb_tree_red; // header 为红色，用来区分header和root在iterator.operator--
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}
	void clear(base_ptr p) {
		if (p != 0) {
			clear(p->left);
			clear(p->right);
			destory_node(static_cast<link_type>(p));
		}
	}
public:
	// 后序遍历清空
	void clear() {
		clear(root());
	}
	rb_tree(const Compare &comp = Compare()): node_count(0), key_compare(comp) { init(); }
	~rb_tree() {
		clear();
		put_ndoe(header);
	}
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
public:
	Compare key_comp() const { return key_compare; }
	iterator begin() { return leftmost(); } // RB头节点 为最左的节点（最小）
	iterator end() { return header; }
	bool empty() { return node_count == 0; }
	size_type size() const { return node_count; }
	size_type max_size() const { return size_type(-1); }
public:
	// 将x插入到 rb_tree 允许节点值重复
	iterator insert_equal(const value_type &v) {
		link_type y = header;
		link_type x = root();
		while(x != 0) {
			y = x;
			// 新值 v 的键值比x的小
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		}
		return __insert(x, y, v);
	}
	// 将x插入到RB-tree中 保持节点独一无二
	std::pair<iterator, bool> insert_unique(const value_type &v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0) {
			y = x;
			// 新值的键值比较小就往左子树走
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		// y指向的是父节点
		iterator j = iterator(y);
		if (comp) {
			if (j == begin()) {
				// 插入点在最左边 即中序遍历的第一个节点
				// x为插入点 y 为父节点 v为新值；插入点是当前树第一个节点
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			} else {
				--j;
			}
		}
		//  || key_compare(KeyOfValue()(v), key(j.node))
		if (key_compare(key(j.node), KeyOfValue()(v))) {
			// 新值的键值比较大
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		}
		// 新值重复，不插入改节点
		return std::pair<iterator, bool>(j, false);
	}
	iterator find(const Key &k) {
		link_type y = header;
		link_type x = root();
		while(x != 0) {
			if (!key_compare(key(x), k)) {
				// 当前x的键值大于 k 往x的左子树走
				y = x, x = left(x);
			} else {
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return j == end() || key_compare(k, key(j.node)) ? end() : j;
	}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(base_ptr x_, base_ptr y_, const Value& v) {
	// 参数 x_ 插入点 y_是父节点
	link_type x = static_cast<link_type>(x_);
	link_type y = static_cast<link_type>(y_);
	link_type z;

	// key_compare function<bool(const Key &, const Key &)>
	if (header == y || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
		z = create_node(v); // 新节点
		left(y) = z;
		if (y == header) {
			root() = z; // 维护根节点
			rightmost() = z; // 维护leftmost 让它永远指向中序下的开头
		} else if (y == leftmost()) {
			leftmost() = z;
		}
	} else {
		z = create_node(v);
		right(y) = z;
		if (y == rightmost()) {
			rightmost() = z;
		}
	}
	parent(z) = y;
	left(z) = 0;
	right(z) = 0;

	__rb_tree_reblance(z, header->parent);
	++node_count;
	return iterator(z);
}

JD_SPACE_END

#endif
