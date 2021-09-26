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
#include <utility>

JD_SPACE_BEGIN

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false; // 红色为0
const __rb_tree_color_type __rb_tree_black = true; // 黑色为1

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
	link_type& leftmost() const { return (link_type &) header->left; }
	link_type& rightmost() const { return (link_type &) header->right; }

	// 以下用来访问指定节点的成员
	static link_type& left(link_type x) { return (link_type &)x->left; }
	static link_type& right(link_type x) { return (link_type &)x->right; }
	static link_type& parent(link_type x) { return (link_type &)x->parent; }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(x); }
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
		return ((link_type)x)->value_field;
	}
	static const Key& key(base_ptr x) {
		return KeyOfValue()(value((link_type)x));
	}
	static color_type& color(base_ptr x) { return (color_type&)(((link_type)x)->color); }

	static link_type minimum(link_type x) {
		return (link_type) __rb_tree_node_base::minimum(x);
	}
	static link_type maximum(link_type x) {
		return (link_type) __rb_tree_node_base::maximum(x);
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
			destory_node((link_type)p);
		}
	}
public:
	// 后序遍历清空
	void clear() {}
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
	// 将x插入到RB-tree中 保持节点独一无二
	std::pair<iterator, bool> insert_unique(const value_type &x);
	// 将x插入到 rb_tree 允许节点值重复
	iterator insert_equal(const value_type &x);
};

JD_SPACE_END

#endif
