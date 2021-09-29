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

JD_SPACE_BEGIN

enum {
	RED = 0,
	BLACK = 1,
	DOUBLE_BALCK = 2
};

typedef int NODE_COLOR;

template<class Value>
struct jd_rb_tree_node {
	// typedef NODE_COLOR color_type;
	typedef jd_rb_tree_node* base_ptr;

	base_ptr left;
	base_ptr right;
	base_ptr parent;
	NODE_COLOR color;

	// typedef jd_rb_tree_base_node::base_ptr base_ptr;
	typedef jd_rb_tree_node<Value>* link_type;
	Value value_field;

	jd_rb_tree_node(Value v): value_field(v) {}
	jd_rb_tree_node() {}

	static base_ptr minimum(base_ptr x) {
		while(x->left != Nil()) x = x->left;
		return x;
	}
	
	static base_ptr maximum(base_ptr x) {
		while(x->right != Nil()) x = x->right;
		return x;
	}

	static jd_rb_tree_node<Value> __NIL;
	static link_type Nil() {
		if (__NIL.left != &__NIL) {
			__NIL.color = BLACK;
			__NIL.left = __NIL.right = __NIL.parent = &__NIL;
		}
		return &__NIL;
	}
};

template<class Value>
jd_rb_tree_node<Value> jd_rb_tree_node<Value>::__NIL;

template<class Value, class Ref, class Ptr>
struct jd_rb_tree_node_iterator {

	typedef JD::bidirectional_iterator_tag iterator_category;
	typedef Value value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef jd_rb_tree_node_iterator<value_type, value_type&, value_type*> iterator;
	typedef jd_rb_tree_node_iterator<Value, Ref, Ptr> self;
	typedef jd_rb_tree_node<Value> base;
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
		if (node->right != NIL()) {
			node = node->right;
			while(node->left != NIL()) {
				node = node->left;
			}
		} else {
			while(node->parent != NIL() && node->parent->left != node) {
				node = node->parent;
			}
			node = node->parent;
		}
	}

	void decreament() {
		// 找前驱
		if (node->left != NIL()) {
			node = node->left;
			while (node->right != NIL()) {
				node = node->right;
			}
		} else {
			while(node->parent != NIL() && node->parent->right != node) {
				node = node->parent;
			}
			node = node->parent;
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
class jd_rb_tree {
protected:
	typedef NODE_COLOR color_type;
	typedef jd_rb_tree_node<Value> base;
	typedef JD::simple_alloc<base, Alloc> node_allocator;
public:
	typedef base* link_type;
	typedef Value value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef Key key_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef jd_rb_tree_node_iterator<value_type, reference, pointer> iterator;
protected:
	link_type get_node() { return node_allocator::allocate(); }
	void put_ndoe(link_type p) { node_allocator::deallocate(p); }

	link_type create_node(const value_type &x) {
		link_type tmp = get_node();
		JD::construct(tmp, x);
		return tmp;
	}

	void destory_node(link_type node) {
		JD::destory(node);
		put_ndoe(node);
	}
protected:
	size_type node_count;
	base header;
	Compare key_compare;
	
	link_type& root() { return (link_type &) header.left; }
	static link_type& left(link_type x) { return (link_type &) x->left; }
	static link_type& right(link_type x) { return (link_type &) x->right; }
	static link_type& parent(link_type x) { return (link_type &) x->parent; }
	static reference value(link_type x) { return x->value_field; }
	static const Key& key(link_type x) { return KeyOfValue()(x->value_field); }
	static color_type& color(link_type x) { return (color_type &)x->color; }

	static link_type minimum(link_type x) { return base::minimum(x); }
	static link_type maximum(link_type x) { return base::maximum(x); }

    void clear(link_type root) {
        if(root == NIL()) return ;
        clear(root->left);
        clear(root->right);
        destory_node(root);
    }

	static bool has_red_child(link_type root) {
		return left(root)->color == RED || right(root)->color == RED;
	}

	static link_type left_rotate(link_type root) {
		link_type tmp = root->right;
		tmp->parent = root->parent;
		root->right = tmp->left;
		tmp->left->parent = root;
		tmp->left = root;
		root->parent = tmp;
		return tmp;
	}

	static link_type right_rotate(link_type root) {
		link_type tmp = root->left;
		tmp->parent = root->parent;
		root->left = tmp->right;
		tmp->right->parent = root;
		tmp->right = root;
		root->parent = tmp;
		return tmp;
	}

	static link_type predecessor(link_type root) {
		link_type tmp = root->left;
		while(tmp->right != NIL()) {
			tmp = tmp->right;
		}
		return tmp;
	}

	// 插入时的调整，去除双红的情况；以祖父节点的角度进行调整
	link_type insert_maintain(link_type root);

	// 站在父节点的角度调整 删除某个节点后的树
	link_type erase_maintain(link_type root);

	link_type __insert(link_type root, link_type parent, const value_type &data) {
		if (root == NIL()) {
			node_count++;
			link_type newNode = create_node(data);
			newNode->parent = parent;
			newNode->left = newNode->right = NIL();
			newNode->color = RED;
			// 新节点是红节点
			return newNode;
		}
		if (key_compare(key(root), KeyOfValue()(data))) {
			// 当前根节点的数据比新节点的要小
			right(root) = __insert(root->right, root, data);
		} else {
			// 小于等于当前根节点的数据都将在根节点的左子树
			left(root) = __insert(root->left, root, data);
		}
		return insert_maintain(root);
	}

	void insert(const value_type &data) {
		header.left = __insert(header.left, &header, data);
		header.left->color = BLACK;
	}

	link_type __erase(link_type root, const key_type &tartgetKey) {
		if (root == NIL()) return root;
		if (key_compare(key(root), tartgetKey)) {
			// 要删除的数据键 比 当前节点大
			right(root) = __erase(right(root), tartgetKey);
		} else if (key_compare(tartgetKey, key(root))) {
			// 要删除的数据键 比 当前节点小
			left(root) = __erase(left(root), tartgetKey);
		} else {
			if (left(root) == NIL() || right(root) == NIL()) {
				link_type tmp = left(root) == NIL() ? right(root) : left(root);
				tmp->color += root->color; // 会产生双黑节点
				destory_node(root);
				return tmp;
			} else {
				link_type tmp = predecessor(root);
				// 需要operator= 的重载
				root->value_field = tmp->value_field;
				left(root) = __erase(left(root), KeyOfValue()(tmp->value_field));
			}
		}
		return erase_maintain(root);
	}

protected:
	static link_type NIL() {
		return base::Nil();
	}
public:
	jd_rb_tree() {
		header.left = NIL();
		header.left->parent = &header;
		header.parent = NIL();
		node_count = 0;
		key_compare = Compare();
	}

	void insert_equal(const value_type &v) {
		insert(v);
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

	void erase(const key_type &key) {
		__erase(root(), key);
		node_count--;
		header.left->color = BLACK;
	}

	size_type size() {
		return node_count;
	}

	iterator begin() {
		return iterator(minimum(root()));
	}
	iterator end() {
		return iterator(&header);
	}
	~jd_rb_tree() {
		clear(root());
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

	// 前序遍历并输出左右子节点
	void __preorder(const link_type root) {
		if (root != NIL()) {
			std::cout << root->color << " | " << key(root) << ", ";
			std::cout << key(left(root)) << ", ";
			std::cout << key(right(root)) << ", ";
			std::cout << std::endl;
			__preorder(left(root));
			__preorder(right(root));
		}
	}
	void preorder() {
		__preorder(root());
	}
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename jd_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	jd_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_maintain(link_type root) 
{
	if (!has_red_child(root)) {
		return root;
	}
	if (left(root)->color == RED && right(root)->color == RED) {
		if (!has_red_child(left(root)) && !has_red_child(right(root))) {
			return root;
		}
		// 红色节点上浮
		root->color = RED;
		left(root)->color = right(root)->color = BLACK;
		return root;
	}
	// 叔父节点为黑色节点
	if (left(root)->color == RED && !has_red_child(left(root))) return root;
	if (right(root)->color == RED && !has_red_child(right(root))) return root;

	if (left(root)->color == RED) {
		if (right(left(root))->color == RED) {
			// LR型
			left(root) = left_rotate(left(root));
		}
		root = right_rotate(root);
	} else {
		if (left(right(root))->color == RED) {
			// RL型
			right(root) = right_rotate(right(root));
		}
		root = left_rotate(root);
	}
	root->color = RED;
	left(root)->color = right(root)->color = BLACK;
	return root;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename jd_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type 
	jd_rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_maintain(link_type root) 
{
	if (right(root)->color != DOUBLE_BALCK && left(root)->color != DOUBLE_BALCK) return root;
	// 兄弟节点中有红色节点
	if (has_red_child(root)) {
		int flag; // 标记双重黑节点下沉的方向
		root->color = RED;
		if (left(root)->color == DOUBLE_BALCK) {
			root = left_rotate(root);
			flag = 1;
		} else {
			root = right_rotate(root);
			flag = 0;
		}
		root->color = BLACK;
		if (flag == 1) left(root) = erase_maintain(left(root));
		else right(root) = erase_maintain(right(root));
		return root;
	}
	// 兄弟节点的子节点没有红色节点的 双黑上浮
	if ((left(root)->color == DOUBLE_BALCK && !has_red_child(right(root))) || (right(root)->color == DOUBLE_BALCK && !has_red_child(left(root)))) {
		root->color += 1;
		left(root)->color -= 1;
		right(root)->color -= 1;
		return root;
	}
	// 兄弟节点的子节点有红色节点
	if (left(root)->color == DOUBLE_BALCK) {
		left(root)->color -= 1;
		if (right(right(root))->color != RED) {
			// RL型
			right(root)->color = RED;
			right(root) = right_rotate(right(root));
			right(root)->color = BLACK;
		}
		root = left_rotate(root);
		// 新根改为原根的颜色
		root->color = left(root)->color;
	} else {
		right(root)->color -= 1;
		if (left(left(root))->color != RED) {
			// LR型
			left(root)->color = RED;
			left(root) = left_rotate(left(root));
			left(root)->color = BLACK;
		}
		root = right_rotate(root);
		// 新根改为原根的颜色
		root->color = right(root)->color;
	}
	left(root)->color = right(root)->color = BLACK;
	return root;
}

JD_SPACE_END

#endif
