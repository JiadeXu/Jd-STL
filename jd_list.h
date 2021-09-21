/*************************************************************************
	> File Name: jd_list.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 19 09:46:31 2021
 ************************************************************************/

#ifndef _JD_LIST_H
#define _JD_LIST_H

#include "jd_construct.h"
#include "jd_macro.h"
#include "jd_iterator.h"
#include "jd_alloc.h"

JD_SPACE_BEGIN

template<class T>
struct __list_node {
	typedef __list_node<T> *void_pointer;
	void_pointer next;
	void_pointer prev;
	T data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator {
	typedef __list_iterator<T, T&, T*> iterator;
	typedef __list_iterator<T, Ref, Ptr> self;
	typedef JD::bidirectional_iterator_tag iterator_category;;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef __list_node<T>* link_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	link_type node; // 普通指针指向 list 的节点
	__list_iterator(link_type x): node(x) {}
	__list_iterator() {}
	__list_iterator(const iterator &x): node(x.node) {}

	bool operator==(const self &x) const { return x.node == node; }
	bool operator!=(const self &x) const { return x.node != node; }
	// 对节点取值 取的节点的值
	reference operator*() const { return node->data; }
	pointer operator->() const { return &(operator*()); }

	self& operator++() {
		node = node->next;
		return *this;
	}

	self operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}
	self& operator--() {
		node = node->prev;
		return *this;
	}

	self operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}

	self operator+(size_type size) {
		link_type tmp = node;
		while (size--) {
			tmp = tmp->next;
		}
		return iterator(tmp);
	}
	self operator-(size_type size) {
		link_type tmp = node;
		while (size--) {
			tmp = tmp->prev;
		}
		return iterator(tmp);
	}
};

template<class T, class Alloc = JD::alloc>
class list {
protected:
	typedef __list_node<T> list_node;
	typedef JD::simple_alloc<list_node, Alloc> list_node_allocator;
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef __list_iterator<T, T&, pointer> iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef list_node* link_type;
protected:
	link_type node; // 只要一个指针即可表示整个环状双向链表
	// 配置一个节点并返回
	link_type get_node() { return list_node_allocator::allocate(); }
	// 释放
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	// 生产构造并配置一个 node 带值
	link_type create_node(const T &x) {
		link_type node = get_node();
		JD::construct(&node->data, x);
		return node;
	}
	// 销毁 析构 并释放一个node
	void destory_node(link_type p) {
		JD::destory(&p->data);
		put_node(p);
	}

	void empty_initialize() {
		node = get_node();
		node->prev = node;
		node->next = node;
	}

	// 将 (first, last] 内的所有元素移动到position之前 左闭右开
	void transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			last.node->prev->next = position.node;
			first.node->prev->next = last.node;
			position.node->prev->next = first.node;
			iterator tmp = position.node->prev;
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = tmp.node;
		}
	}
public:
	~list() {
		clear();
		put_node(node);
	}
	list() { empty_initialize(); }
	iterator begin() { return iterator(node->next); }
	iterator end() { return iterator(node); }
	bool empty() const { return node->next == node; }
	const size_type size() {
		return JD::distance(begin(), end());
	}
	reference front() { return *(begin()); }

	iterator insert(iterator position, const T &x) {
		link_type tmp = create_node(x);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		position.node->prev->next = tmp;
		position.node->prev = tmp;
		return iterator(tmp);
	}

	void push_back(const T &x) {
		insert(end(), x);
	}

	void push_front(const T &x) {
		insert(begin(), x);
	}

	// 移除 迭代器所指向的节点
	iterator erase(iterator position) {
		link_type next_node = position.node->next;
		link_type prev_node = position.node->prev;
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);
		return iterator(next_node);
	}

	void pop_front() {
		erase(begin());
	}

	// 清除整个链表
	void clear() {
		link_type cur = node->next;
		// 遍历每一个节点
		while (cur != node) {
			link_type tmp = cur;
			cur = cur->next;
			destory_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	// 清空所有值为value的元素
	void remove(const T &value) {
		iterator first = begin();
		iterator last = end();

		while(first != last) {
			iterator next = first;
			++next;
			if (*first == value) {
				erase(first);
			}
			first = next;
		}
	}

	void unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first + 1;
		while(next != last) {
			next++;
			if (*next == *first) {
				erase(next);
			} else {
				first = next;
			}
			next = first;
		}
	}

	void splice(iterator position, JD::list<T, Alloc> &x) {
		if (!x.empty()) {
			transfer(position, x.begin(), x.end());
		}
	}
	// 将 i所指向元素结合与 position 所指位置之前
	void splice(iterator position, JD::list<T, Alloc> &, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position, i, j);
	}

	void splice(iterator position, iterator first, iterator last) {
		if (first != last) {
			transfer(position, first, last);
		}
	}

	// 将 x 合并到 *this 上，两个lsit的内容都必须先经过递增排序
	void merge(JD::list<T, Alloc> &x) {
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while(first1 != last1 && first2 != last2) {
			if (*first1 < *first2) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			} else {
				first1++;
			}
		}
		if (first2 != last2) transfer(last1, first2, last2);
	}

	void reverse() {
		// 空链表或者只有一个节点
		if (node->next == node || node->next->next == node) return;
		iterator first = begin();
		++first;
		while(first != end()) {
			iterator old = first;
			++first;
			transfer(begin(), old, first);
		}
	}

	void sort() {
		if (node->next == node || node->next->next == node) return;
		// 归并排序
		JD::list<T, Alloc> carry;
		JD::list<T, Alloc> counter[64];
		int fill = 0;
		while(!empty()) {
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while(i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill) ++fill;
		}
		for (int i = 1; i < fill; i++) {
			counter[i].merge(counter[i - 1]);
		}
		swap(counter[fill - 1]);
	}

	void swap(JD::list<T, Alloc> &x) {
		link_type tmp;
		tmp = this->node;
		this->node = x.node;
		x.node = tmp;
	}
};

JD_SPACE_END

#endif
