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
#include <ostream>
#include "jd_vector.h"

JD_SPACE_BEGIN

typedef int my_rb_tree_color;

const my_rb_tree_color RED_NODE = 0;
const my_rb_tree_color BLACK_NODE = 1;
const my_rb_tree_color DOUBLE_BLACK_NODE = 2;

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
			__NIL.color = BLACK_NODE;
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

	link_type get_node() { return node_allocator::allocate(); }
	void put_ndoe(link_type p) { node_allocator::deallocate(p); }
	
	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		__JD_TRY {
			JD::construct(&tmp->value_field, x);
		}
		__JD_UNWIND(put_ndoe(p));
		tmp->lchild = tmp->rchild = NIL();
		return tmp;
	}

	link_type clone_node(link_type x) {
		link_type tmp = create_node(x->value_field);
		tmp->color = x->color;
		tmp->father = tmp->lchild = tmp->rchild = NIL();
		return tmp;
	}

	void destory_node(link_type node) {
		JD::destory(&node->value_field); // 析构内容
		put_ndoe(node); // 释放空间
	}


	static bool has_red_child(link_type root) {
		return color(left(root)) == RED_NODE || color(right(root)) == RED_NODE;
	}
	
	static link_type rotate_left(link_type root) {
		link_type tmp = root->rchild;
		tmp->father = root->father;
		root->rchild = tmp->lchild;
		tmp->lchild->father = root;
		tmp->lchild = root;
		root->father = tmp;
		return tmp;
	}
	
	static link_type rotate_right(link_type root) {
		link_type tmp = root->lchild;
		tmp->father = root->father;
		root->lchild = tmp->rchild;
		tmp->rchild->father = root;
		tmp->rchild = root;
		 root->father = tmp;
		return tmp;
	}

	static link_type predecessor(link_type root) {
		link_type tmp = root->lchild;
		while(tmp->rchild != NIL()) {
			tmp = tmp->rchild;
		}
		return tmp;
	}
protected:
	link_type __insert(link_type root, link_type father, const value_type &data);
	// 以祖父节点的角度维护红黑树的性质
	link_type inseret_maintain(link_type root);
	link_type __erase(link_type root, const key_type &key);
	// 以父节点的角度维护删除了节点后的红黑树的性质
	link_type erase_maintain(link_type root);

	void real_erase(const key_type &key) {
		root() = __erase(root(), key);
		root()->color = BLACK_NODE;
		if (root() != NIL()) {
			root()->father = virtual_node;
		}
	}

	void clear(link_type root) {
        if(root == NIL()) return ;
        clear(root->lchild);
        clear(root->rchild);
        destory_node(root);
    }

	link_type copy(link_type src, link_type father) {
		link_type top = clone_node(src);
		top->father = father;

		__JD_TRY {
			if (right(src) != NIL()) {
				top->rchild = this->copy(right(src), top);
			}
			father = top;
			src = left(src);

			while(src != NIL()) {
				link_type y = this->clone_node(src);
				father->lchild = y;
				y->father = father;
				if (src->rchild != NIL()) {
					y->rchild = this->copy(right(src), y);
				}
				father = y;
				src = left(src);
			}
		} __JD_UNWIND(__erase(top));
		return top;
	}
private:
	size_type node_count;
	link_type virtual_node;
	Compare key_compare;

	link_type& root() const { return (link_type &) virtual_node->lchild; }

	void init() {
		virtual_node = get_node(); // 产生一个节点空间
		color(virtual_node) = RED_NODE; // header 为红色，用来区分header和root在iterator.operator--
		root() = NIL();
	}

	link_type leftmost() const {
		return base::minimum(root());
	}

	// 前序遍历并输出左右子节点
	void __preorder(const link_type root) {
		if (root != NIL()) {
			std::cout << root->color << " | " << key(root) << ", ";
			if (left(root) != NIL()) {
				std::cout << key(left(root)) << ", ";
			}
			if (right(root) != NIL()) {
				std::cout << key(right(root)) << ", ";
			}
			std::cout << std::endl;
			__preorder(left(root));
			__preorder(right(root));
		}
	}
public:
	my_rb_tree(): node_count(0), key_compare(Compare()) {
		init();
	}
	my_rb_tree(const Compare &comp): node_count(0), key_compare(comp) {
		init();
	}
	~my_rb_tree() {
		clear();
		destory_node(virtual_node);
	}
public:
	Compare key_comp() const { return key_compare; }
	iterator begin() const { return base::minimum(root()); } // RB头节点 为最左的节点（最小）
	iterator end() const { return virtual_node; } // 还是需要一个虚拟节点作为尾部
	bool empty() const { return node_count == 0; }
	size_type size() const { return node_count; }
	size_type max_size() const { return size_type(-1); }

	void clear() {
		clear(root());
		init();
		node_count = 0;
	}

	std::pair<iterator, iterator> equal_range(const key_type &k) {
		return std::pair<iterator, iterator>(this->lower_bound(k), this->upper_bound(k));
	}

	// 找到以中序遍历为顺序，最前面的键为tartgetKey的节点
	iterator lower_bound(const key_type &tartgetKey) {
		link_type last = NIL();
		link_type cur = root();
		while(cur != NIL()) {
			// 找大于等于 targetKey
			if (!key_compare(key(cur), tartgetKey)) {
				// 寻找相同的键值的开始节点
				last = cur, cur = left(cur);
			} else {
				cur = right(cur);
			}
		}
		return iterator(last);
	}
	// 找到以中序遍历为顺序，最后一个的键为tartgetKey的节点的下一个节点
	iterator upper_bound(const key_type &tartgetKey) {
		link_type last = NIL();
		link_type cur = root(); // 当前节点
		while(cur != NIL()) {
			// 找小于targetkey的
			if (key_compare(tartgetKey, key(cur))) {
				// 寻找相同的键值的开始节点
				last = cur, cur = left(cur);
			} else {
				cur = right(cur);
			}
		}
		return iterator(last);
	}

	iterator find(const key_type &tartgetKey) {
		link_type last = NIL();
		link_type cur = root();
		while (cur != NIL()) {
			if (key_compare(key(cur), tartgetKey)) {
				cur = right(cur);
			} else {
				last = cur, cur = left(cur);
			}
		}
		iterator j = iterator(last);
		// 最后如果 last 指向的是 targetKey key_compare会返回 false
		return j == NIL() || key_compare(tartgetKey, key(j.node)) ? end() : j;
	}

	size_type count(const key_type &k) {
		std::pair<iterator, iterator> p = equal_range(k);
		return JD::distance(p.first, p.second);
	}

	void insert(const value_type &data) {
		root() = __insert(root(), virtual_node, data);
		root()->color = BLACK_NODE;
	}

	void insert_unique(const_iterator first, const_iterator last) {
		for(;first != last; ++first) {
			insert_unique(*first);
		}
	}

	std::pair<iterator, bool> insert_unique(const value_type &v) {
		auto p = find(KeyOfValue()(v));
		if (p == end()) {
			insert(v);
			p = find(KeyOfValue()(v));
			return std::pair<iterator, bool>(p, true);
		}
		return std::pair<iterator, bool>(p, false);
	}
	void insert_equal(const value_type &v) {
		insert(v);
	}
	void erase(iterator pos) {
		real_erase(*pos);
	}
	size_type erase(const Key &k) {
		std::pair<iterator, iterator> p = equal_range(k);
		size_type n = JD::distance(p.first, p.second);
		erase(p.first, p.second);
		return n;
	}
	void erase(iterator first, iterator last) {
		if (first == begin() && last == end()) {
			clear();
		} else {
			while(first != last) {
				erase(first++);
			}
		}
	}
	void erase(const Key *first, const Key *last) {
		for (;first != last; first++) {
			erase(*first);
		}
	}
	// 测试函数
	void preorder() {
		__preorder(root());
	}

	my_rb_tree& operator=(const my_rb_tree& x) {
		if (this != &x) {
			clear();
			node_count = 0;
			key_compare = x.key_compare;
			if (x.root() == NIL()) {
				root() = NIL();
			} else {
				root() = copy(x.root(), virtual_node);
				node_count = x.node_count;
			}
		}
		return *this;
	}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__insert(link_type root, link_type father, const value_type &data) 
{
	if (root == NIL()) {
		node_count++;
		link_type node = create_node(data);
		color(node) = RED_NODE;
		node->father = father;
		return node;
	}
	if (key_compare(key(root), KeyOfValue()(data))) {
		right(root) = __insert(right(root), root, data);
	} else {
		left(root) = __insert(left(root), root, data);
	}
	return inseret_maintain(root);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::inseret_maintain(link_type root) 
{
	if (!has_red_child(root)) return root;
	// 叔父节点也是红节点
	if (left(root)->color == RED_NODE && right(root)->color == RED_NODE) {
		if (!has_red_child(left(root)) && !has_red_child(right(root))) {
			return root;
		}
		// 红节点上浮
		color(root) = RED_NODE;
		left(root)->color = right(root)->color = BLACK_NODE;
		return root;
	}
	if (left(root)->color == RED_NODE && !has_red_child(left(root))) return root;
	if (right(root)->color == RED_NODE && !has_red_child(right(root))) return root;

	if (left(root)->color == RED_NODE) {
		if (left(root)->rchild->color == RED_NODE) {
			// LR型
			left(root) = rotate_left(left(root));
		}
		root = rotate_right(root);
	} else {
		if (right(root)->lchild->color == RED_NODE) {
			// RL型
			right(root) = rotate_right(right(root));
		}
		root = rotate_left(root);
	}
	root->color = RED_NODE;
	left(root)->color = right(root)->color = BLACK_NODE;
	return root;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::__erase(link_type root, const key_type &tartgetKey) 
{
	if (root == NIL()) return root;
	// 目标key比当前节点key大
	if (key_compare(key(root), tartgetKey)) {
		right(root) = __erase(right(root), tartgetKey);
	} else if (key_compare(tartgetKey, key(root))) {
		// 目标key比当前节点key小
		left(root) = __erase(left(root), tartgetKey);
	} else {
		if (left(root) == NIL() || right(root) == NIL()) {
			link_type tmp = left(root) == NIL() ? right(root) : left(root);
			tmp->color += root->color;
			node_count--;
			destory_node(root);
			return tmp;
		} else {
			link_type tmp = predecessor(root);
			root->value_field = tmp->value_field;
			left(root) = __erase(left(root), KeyOfValue()(tmp->value_field));
		}
	}
	if (right(root) != NIL()) {
		right(root)->father = root;
	}

	if (left(root) != NIL()) {
		left(root)->father = root;
	}
	return erase_maintain(root);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	my_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_maintain(link_type root) 
{
	if (left(root)->color != DOUBLE_BLACK_NODE && right(root)->color != DOUBLE_BLACK_NODE) {
		return root;
	}
	if (has_red_child(root)) {
		int flag; // 标识双重黑色节点下沉方向
		root->color = RED_NODE;
		if (left(root)->color == DOUBLE_BLACK_NODE) {
			flag = 1;
			root = rotate_left(root);
		} else {
			flag = 2;
			root = rotate_right(root);
		}
		root->color = BLACK_NODE;
		if (flag == 1) left(root) = erase_maintain(left(root));
		else right(root) = erase_maintain(right(root));
		return root;
	}
	// 叔父节点为黑色节点 且 叔父节点子节点没有红节点
	if ((left(root)->color == DOUBLE_BLACK_NODE && !has_red_child(right(root))) || (right(root)->color == DOUBLE_BLACK_NODE && !has_red_child(left(root)))) {
		// 黑色上浮
		root->color += 1;
		left(root)->color -= 1;
		right(root)->color -= 1;
		return root;
	}
	// 叔父节点为黑色节点 且 叔父节点有红色节点
	if (left(root)->color == DOUBLE_BLACK_NODE) {
		left(root)->color -= 1;
		if (right(root)->rchild->color != RED_NODE) {
			// RL型
			right(root)->color = RED_NODE;
			right(root) = rotate_right(right(root));
			right(root)->color = BLACK_NODE;
		}
		root = rotate_left(root);
		root->color = left(root)->color;
	} else {
		right(root)->color -= 1;
		if (left(root)->lchild->color != RED_NODE) {
			left(root)->color = RED_NODE;
			left(root) = rotate_right(left(root));
			left(root)->color = BLACK_NODE;
		}
		root = rotate_right(root);
		root->color = right(root)->color;
	}
	left(root)->color = right(root)->color = BLACK_NODE;
	return root;
}

JD_SPACE_END

#endif
