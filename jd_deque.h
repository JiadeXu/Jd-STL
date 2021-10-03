/*************************************************************************
	> File Name: jd_deque.h
	> Author: 
	> Mail: 
	> Created Time: Mon Sep 20 19:20:08 2021
 ************************************************************************/

#ifndef _JD_DEQUE_H
#define _JD_DEQUE_H

#include "jd_construct.h"
#include "jd_macro.h"
#include "jd_iterator.h"
#include "jd_alloc.h"
#include "jd_uninitialized.h"
#include <algorithm>
#include <cstddef>
#include <iostream>

JD_SPACE_BEGIN

// n 不为 0 表示用户自定义缓冲区大小
// 如果 n 为0，表示buffer suze 使用默认值 那么
// 如果 sz (元素大小, sizeof(value_type)) 小于512 传回 512 / sz
// 如果 不小于 512 传回 1
inline size_t __deque_buf_size(size_t n, size_t sz) {
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template<class T, class Ref, class Ptr, size_t BufSize>
struct __deque_iterator {
	
	typedef __deque_iterator<T, T&, T*, BufSize> iterator;
	typedef __deque_iterator<const T, const T&, const T*, BufSize> const_iterator;
	static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

	// 没有继承 JD::iterator
	typedef JD::random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;
	typedef __deque_iterator self;

	// 保持与容器的联结
	T *cur; // 此迭代器所指向的缓存区中的现行(current)元素
	T *first; // 此迭代器所指的缓冲区的头
	T *last; // 此迭代器所指的缓冲区的尾 含备用空间
	map_pointer node; // 指向管控中心

	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}

	reference operator*() const { return *cur; }
	pointer operator->() const { return &(operator*()); }

	difference_type operator-(const self &x) const {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}
	// 前++
	self& operator++() {
		++cur;
		if (cur == last) {
			// 如果已到达缓冲区的尾端 就切换至下一节点（也是缓冲区）的第一个元素
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	// 后加加
	self operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}
	// 前--
	self& operator--() {
		if (cur == first) {
			// 如果前一个是上一个缓冲区
			set_node(node - 1);
			cur = last;
		}
		--cur; // 切换至前一个元素
		return *this;
	}
	// 后--
	self operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}
	// 以下实现随机存取、迭代器可以直接跳跃 n 个距离
	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size())) {
			// 在同一个缓冲区内偏移
			cur += n;
		} else {
			// 目标的位置不在同一个缓冲区
			difference_type node_offset = 
				offset > 0 ? offset / difference_type(buffer_size())
							: -difference_type((-offset - 1) / buffer_size()) - 1;
			// 切换至正确的节点
			set_node(node + node_offset);
			// 切换至正确的元素
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	// 参考 More efffective C++ Consider using op= instead of
	// stand-alone op
	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n; // 调用+=
	}

	// 利用 += 完成 -=
	self& operator-=(difference_type n) { return *this += -n; }
	self operator-(difference_type n) const {
		self tmp = *this;
		return tmp -= n;
	}

	// 随机存取
	reference operator[](difference_type n) { return *(*this + n); }

	bool operator==(const self &x) const { return cur == x.cur; }
	bool operator!=(const self &x) const { return !(*this == x); }
	bool operator<(const self &x) const {
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
	
};

// BufSize为0表示使用 512 bytes 的缓存
template<class T, class Alloc = JD::alloc, size_t BufSize = 0>
class deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
public:
	typedef JD::__deque_iterator<value_type, reference, pointer, BufSize> iterator;
protected:
	typedef pointer* map_pointer; // 指针的指针 用于调度 几块 连续的用于存储T类型数据的空间
	typedef JD::simple_alloc<value_type, Alloc> data_allocator;
	// 每次配置一个指针大小
	typedef JD::simple_alloc<pointer, Alloc> map_allocator;
protected:
	iterator start;
	iterator finish;

	map_pointer map; // 管控中心， map是块连续空间 其内的每个元素都是一个指针（称为节点）指向一块缓冲区 他指向的指针称为node
	
	size_type map_size; // map 中有多少指针
protected:
	static size_type buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

	static size_type initial_map_size() {
		return BufSize;
	}

	value_type* allocate_node() { return data_allocator::allocate(buffer_size()); }
	void deallocate_node(pointer p) {
		data_allocator::deallocate(p, buffer_size());
	} 

	void fill_initialize(size_type n, const value_type &value) {
		create_map_and_nodes(n);
		map_pointer cur;
		for(cur = start.node; cur < finish.node; cur++) {
			JD::uninitialized_fill(*cur, *cur + buffer_size(), value);
		}
		JD::uninitialized_fill(finish.first, finish.cur, value);
	}
	// 负责产生并安排好deque的结构
	void create_map_and_nodes(size_type num_elements) {
		// 需要的节点数=(元素个数/缓冲区可容纳的元素个数) + 1
		// 刚好整除会多分配一个节点
		size_type num_nodes = num_elements / buffer_size() + 1;
		// 一个map要管理几个节点 最少 BufSize 个，最多是 ”所需节点数加2“
		// （前后各预留一个，扩充时可用）
		map_size = std::max(initial_map_size(), num_nodes + 2);
		// std::cout << "num_elements " << num_elements << " num_nodes " << num_nodes;
		// std::cout << " map_size " << map_size;
		// std::cout << std::endl;
		// 配置出具有 map_size个节点的map
		map = map_allocator::allocate(map_size);
		
		// 令nstart和nfinish指向map所拥有的全部节点的中央区段
		// 保持在中央区段 可使头尾两端的扩充能量一样大，每个节点的对应一个缓冲区
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		__JD_TRY {
			// 为map的每个现用节点配置缓冲区,所有缓冲区加起来就是deque的可用空间（最后的缓冲区可能会留有一些余裕）
			for (cur = nstart; cur <= nfinish; cur++) {
				*cur = allocate_node();
			}
		}
		#ifdef __JD_USE_EXCEPTIONS
			catch(...) {
				// 回滚
			}
		#endif
		// 为deque的两个内部迭代器设定值
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % (buffer_size());
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		std::cout << "reverse_map  reallocate_map" << std::endl;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) {
			// 本身有足够的空间 只是某一端的空间不足 就移动一下
			new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node) {
				// 向前移动
				JD::copy(start.node, finish.node + 1, new_nstart);
			} else {
				// 向后移动
				JD::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
			}
		} else {
			// 空间不足 重新添加空间
			size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
			// 配置一块新空间
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			// 拷贝原内容
			JD::copy(start.node, finish.node + 1, new_nstart);
			// 释放原map
			map_allocator::deallocate(map, map_size);
			// 
			map = new_map;
			map_size = new_map_size;
		}

		// 重新设定 start  finish 迭代器
		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	void reserve_map_at_back(size_type nodes_to_add = 1) {
		// map中剩余的node的数量不够了
		if (nodes_to_add + 1 > map_size - (finish.node - map)) {
			// std::cout << "reserve_map_at_back " << std::endl;
			reallocate_map(nodes_to_add, false);
		}
	}

	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > start.node - map) {
			// 前端的备用节点不足
			// std::cout << "reserve_map_at_front " << std::endl;
			reallocate_map(nodes_to_add, true);
		}
	}

	void push_back_aux(const value_type &x) {
		// 只有当finish.cur == finish.last - 1时才会调用
		value_type t_copy = x;
		reserve_map_at_back(); // 若符合某种条件则必须重换一个map
		*(finish.node + 1) = allocate_node(); // 配置一个新节点
		__JD_TRY {
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		__JD_UNWIND(deallocate_node(*(finish.node + 1)));
	}

	// start.cur == start.first时才会被调用 push_front_aux
	void push_front_aux(const value_type &x) {
		value_type t_copy = x;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		__JD_TRY {
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, t_copy);
		}
		#ifdef __JD_USE_EXCEPTIONS
		catch(...) {
			start.set_node(start.node + 1);
			start.cur = start.first;
			deallocate_node(*(start.node - 1));
			throw;
		}
		#endif
	}
public:
	deque(int n, const value_type &value): start(), finish(), map(0), map_size(0) {
		fill_initialize(n, value);
	}
	template<class InputIterator>
	deque(InputIterator first, InputIterator last): start(), finish(), map(0), map_size(0) {
		typedef typename JD::iterator_traits<InputIterator>::value_type v_t;
		fill_initialize(0, v_t());
		for (; first != last; ++first) {
			push_back(*first);
		}
	}

	iterator begin() { return start; }
	iterator end() { return finish; }

	reference operator[](size_t n) {
		return start[difference_type(n)];
	}

	reference front() { return *start; }
	reference back() {
		iterator tmp = finish;
		--tmp;
		// 因为__deque_iterator<>没有为finish-1定义运算自 ?!
		return *tmp;
	}
	size_type size() const { return finish - start;; } // 调用 iterator 的 operation-
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return finish == start; }

	void push_back(const value_type &t) {
		if (finish.cur != finish.last - 1) {
			// 缓冲区尚有两个以上备用空间
			JD::construct(finish.cur, t);
			++finish.cur;
		} else {
			// 扩容
			push_back_aux(t);
		}
	}

	void push_front(const value_type &t) {
		if (start.cur != start.first) {
			JD::construct(start.cur - 1, t);
			// 直接在备用空间上构造元素
			--start.cur;
		} else {
			// 第一缓冲区已无备用空间
			push_front_aux(t);
		}
	}

	void pop_back_aux() {
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		JD::destory(finish.cur);
	}

	void pop_back() {
		if (finish.cur != finish.first) {
			--finish.cur;
			JD::destory(finish.cur);
		} else {
			// 最后的缓冲区没有任何元素
			pop_back_aux(); // 释放缓冲区
		}
	}

	void pop_front_aux() {
		destory(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

	void pop_front() {
		if (start.cur != start.last - 1) {
			// 第一个缓冲区有两个或更多的元素
			JD::destory(start.cur);
			start.cur++;
		} else {
			pop_front_aux();
		}
	}

	void erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start; // 清除点之前的元素
		if (index < (size() >> 1)) {
			// 如果清除点之前的元素较少
			JD::copy_backward(start, pos, next); // 就移动之前的元素
			pop_front();
		} else {
			JD::copy(next, finish, pos); // 否则就移动后面的元素到前面
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last) {
		if (first == start && last == finish) {
			clear();
			return first;
		}

		difference_type n = last - first;
		difference_type elems_before = first - start;
		if (elems_before < (size() - n) / 2) {
			// 区间前面的元素较少
			JD::copy_backward(start, first, last);
			iterator new_nstart = start + n;
			JD::destory(start, new_nstart);
			// 以下将冗余的缓冲区释放
			for (map_pointer cur = start.node; cur < new_nstart.node; cur++) {
				deallocate_node(*cur);
			}
			start = new_nstart;
		} else {
			// 后面的元素较少
			JD::copy(last, finish, first);
			iterator new_finish = finish - n;
			JD::destory(new_finish, finish); // 析构冗余元素
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; cur++) {
				deallocate_node(*cur);
			}
			finish = new_finish;
		}
		return first + elems_before;
	}

	iterator insert(iterator position, const value_type &x) {
		if (position.cur == start.cur) {
			push_front(x);
			return start;
		} else if (position.cur == finish.cur) {
			push_back(x);
			iterator tmp = finish;
			tmp--;
			return tmp;
		} else {
			return insert_aux(position, x);
		}
	}

	iterator insert_aux(iterator pos, const value_type &x) {
		difference_type index = pos - start;
		value_type x_copy = x;
		if (index < size() / 2) { // 前面的元素少
			push_front(front()); // 在最前端加入与第一个元素同值的元素 然后标识记号然后移动
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			JD::copy(front2, pos1, front1);
		} else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			JD::copy_backward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}

	void clear() {
		for (map_pointer node = start.node + 1; node < finish.node; node++) {
			JD::destory(*node, *node + buffer_size());
			deallocate_node(*node);
		}
		if (start.node != finish.node) {
			JD::destory(start.cur, start.last);
			JD::destory(finish.first, finish.cur);
			// 释放缓冲区 头缓冲区保留
			deallocate_node(finish.first);
		} else {
			// 只有一个缓冲区
			JD::destory(start.cur, finish.cur);
		}
		finish = start;
	}
	~deque() {
		// map_pointer cur;
		map_pointer cur;
		for(cur = start.node; cur < finish.node; cur++) {
			JD::destory(*cur, *cur + buffer_size());
			deallocate_node(*cur);
		}
		JD::destory(*finish.node, finish.cur);
		deallocate_node(*finish.node);
		map_allocator::deallocate(map, map_size);
	}
};
JD_SPACE_END

#endif
