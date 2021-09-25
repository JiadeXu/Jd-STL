/*************************************************************************
	> File Name: jd_slist.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 25 10:19:09 2021
 ************************************************************************/

#ifndef _JD_SLIST_H
#define _JD_SLIST_H

#include "default_alloc_template.h"
#include "jd_alloc.h"
#include "jd_construct.h"
#include "jd_macro.h"
#include "jd_iterator.h"
#include <cstddef>
#include <iostream>

JD_SPACE_BEGIN

struct __slist_node_base {
	__slist_node_base *next;
};

template<class T>
struct __slist_node : public __slist_node_base {
	T data;
};

// 全局函数 已知某一节点，插入新节点于其后
inline __slist_node_base* __slist_make_link(
	__slist_node_base *prev_node,
	__slist_node_base *new_node
) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

inline size_t __slist_size(__slist_node_base *node) {
	size_t result = 0;
	for (; node != 0; node = node->next) {
		result += 1;
	}
	return result;
}

struct __slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t different_type;
	typedef JD::forward_iterator_tag iterator_category;

	__slist_node_base *node;

	__slist_iterator_base(__slist_node_base *x): node(x) {}

	void incr() { node = node->next; } // 前进

	bool operator==(const __slist_iterator_base &x) const {
		return node == x.node;
	}
	bool operator!=(const __slist_iterator_base &x) const {
		return node != x.node;
	}
};

template<class T, class Ref, class Ptr>
struct __slist_iterator : public __slist_iterator_base {
	typedef __slist_iterator<T, T&, T*> iterator;
	typedef __slist_iterator<const T, const T&, const T*> const_iterator;
	typedef __slist_iterator<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __slist_node<T> list_node;

	__slist_iterator(list_node *x): __slist_iterator_base(x) {}
	// 调用 slist<T>::end() 时会造成 __slist_iterator(0)，于是调用上述函数
	__slist_iterator(): __slist_iterator_base(0) {}
	__slist_iterator(const iterator &x): __slist_iterator_base(x.node) {}

	reference operator*() { return static_cast<list_node *>(node)->data; }
	pointer operator->() { return &(operator*()); }

	// ++i
	self &operator++() {
		incr();
		return *this;
	} 
	// i++
	self operator++(int) {
		self tmp = *this;
		incr(); // 前进
		return tmp;
	}

	self operator+(size_type n) {
		self tmp = *this;
		while(n--) {
			tmp.incr(); // 前进
		}
		return tmp;
	}
	// 因为这是一个 forward_iterator_tag 所以没实现 operator--
};

template<class T, class Alloc = JD::alloc>
class slist {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef __slist_iterator<value_type, reference, pointer> iterator;
	typedef const __slist_iterator<value_type, reference, pointer> const_iterator;

private:
	typedef __slist_node<value_type> list_node;
	typedef __slist_node_base list_node_base;
	typedef __slist_iterator_base iterator_base;
	
	typedef JD::simple_alloc<list_node, Alloc> list_node_allocator;

	static list_node* create_node(const value_type &x) {
		list_node *node = list_node_allocator::allocate();
		__JD_TRY {
			JD::construct(&node->data, x);
			node->next = 0;
		}
		__JD_UNWIND(list_ndoe_allocator::deallocate(node));

		return node;
	}

	static void destory_node(list_node *node) {
		JD::destory(&node->data);
		list_node_allocator::deallocate(node);
	}

private:
	list_node_base head; // 虚拟头结点
public:
	slist() {
		head.next = 0;
	}
	~slist() { clear(); }
public:
	iterator begin() { return iterator(static_cast<list_node *>(head.next)); }
	iterator end() { return iterator(0); }
	size_type size() { return __slist_size(head.next); }

	// 两个 slist 交换虚拟头结点即可
	void swap(slist &l) {
		list_node_base* tmp = head.next;
		head.next = l.head.next;
		l.head.next = tmp;
	}

	list_node* get_prev(iterator position) {
		list_node *prev = 0;
		for (iterator ite = begin(); ite != end(); ite++) {
			if (ite.node != 0 && ite.node->next == position.node) {
				prev = static_cast<list_node *>(ite.node);
				break;
			}
		}
		return prev;
	}
public:
	reference front() { return static_cast<list_node *>(head.next)->data; }
	// 从头部插入元素
	void push_front(const value_type &x) {
		__slist_make_link(&head, create_node(x));
	}
	// slist 没有push_back
	// 从头部取走元素
	void pop_front() {
		list_node *node = static_cast<list_node *>(head.next);
		head.next = node->next;
		destory_node(node);
	}

	// 在指定元素后插入
	iterator insert(iterator position, const value_type &x) {
		if (position == begin()) {
			push_front(x);
			return begin();
		}
		list_node *tmp = create_node(x);
		list_node *prev = get_prev(position);
		if (prev == 0) {
			return end();
		}
		__slist_make_link(prev, tmp);
		return iterator(tmp);
	}

	iterator erase(iterator position) {
		if (position == begin()) {
			pop_front();
			return begin();
		}
		list_node *tmp = static_cast<list_node *>(position.node);
		list_node *prev = get_prev(position);
		prev->next = tmp->next;
		tmp->next = 0;
		destory_node(tmp);
		return iterator(static_cast<list_node *>(prev->next));
	}

	void clear() {
		// list_node *node = static_cast<list_node *>(head.next);
		list_node_base *node = head.next;
		for (; node != 0;) {
			list_node_base *tmp = node->next;
			destory_node(static_cast<list_node *>(node));
			node = tmp;
		}
	}
};

JD_SPACE_END

#endif
