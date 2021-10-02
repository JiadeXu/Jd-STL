/*************************************************************************
	> File Name: jd_vector.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 11 19:45:55 2021
 ************************************************************************/

#ifndef _JD_VECTOR_H
#define _JD_VECTOR_H

#include "jd_construct.h"
#include "jd_macro.h"
#include "jd_alloc.h"
#include "jd_uninitialized.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>

JD_SPACE_BEGIN

// class Alloc = std::allocator<T>
template<class T, class Alloc = JD::alloc>
class vector {
public:
	// vector的嵌套型别定义
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef const iterator const_iterator;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	typedef simple_alloc<value_type, Alloc> data_alloc;
	iterator start; // 目标空间的头
	iterator finish; // 目标空间的尾 最后一个元素后面的那一个位置
	iterator end_of_storage; // 目前可用空间的尾
	// 扩展空间
	void insert_aux(iterator position, const T &x);
	T* allocate(size_type n) {
		return data_alloc::allocate(n);
	}
	void deallocate() {
		if (start) {
			data_alloc::deallocate(start, end_of_storage - start);
		}
	}
	void fill_initialize(size_type n, const T &value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
	iterator allocate_and_copy(size_type n, const_iterator first, const_iterator last) {
		iterator result = allocate(n);
		JD::uninitialized_copy(first, last, result);
		return result;
	}
public:
	iterator begin() const { return start; }
	iterator end() const { return finish; }
	size_type size() const { return finish - start; }
	size_type capacity() const {
		return size_type(end_of_storage - begin());
	}
	bool empty() { return begin() == end(); }
	reference operator[](size_type n) const { return *(start + n); }

	vector(): start(0), finish(0), end_of_storage(0) {}
	vector(size_type n, const T &value) { fill_initialize(n, value); }
	vector(int n, const T &value) { fill_initialize(n, value); }
	vector(long n, const T &value) { fill_initialize(n, value); }
	explicit vector(size_type n) { fill_initialize(n, T()); }

	~vector() {
		destory(start, finish);
		this->deallocate();
	}

	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_back(const T &x) {
		if (finish != end_of_storage) {
			construct(finish, x);
			++finish;
		} else {
			insert_aux(end(), x);
		}
	}
	// 取出最尾端的元素
	void pop_back() {
		--finish;
		destory(finish);
	}
	// 从position开始插入 n 个元素
	void insert(iterator position, size_type n, const T &x);
	// 清除某位置上的元素
	iterator erase(iterator position) {
		if (position + 1 != end()) {
			std::copy(position + 1, finish, position); // 后续元素向前移动
		}
		--finish;
		destory(finish);
		return position;
	}
	// 清除[first, last) 内的所有元素
	iterator erase(iterator first, iterator last) {
		iterator i = std::copy(last, finish, first);
		size_type dis = finish - i;
		destory(i, finish);
		finish = finish - dis;
		return first;
	}
	void resize(size_type new_size, const T &x) {
		if (new_size < size()) {
			erase(begin() + new_size, end());
		} else {
			insert(end(), new_size - size(), x);
		}
	}

	void resize(size_type size) { resize(size, T()); }
	void clear() { erase(begin(), end()); }

	void reserve(size_type n) {
		if (capacity() < n) {
			const size_type old_size = size();
			iterator tmp = allocate_and_copy(n, begin(), end());
			JD::destory(begin(), end());
			this->deallocate();
			start = tmp;
			finish = tmp + old_size;
			end_of_storage = start + n;
		}
	}

  void swap(vector<value_type, Alloc>& x) {
    std::swap(start, x.start);
    std::swap(finish, x.finish);
    std::swap(end_of_storage, x.end_of_storage);
  }

public:
	value_type& operator[](size_type idx) {
		return *(begin() + idx);
	}
protected:
	// 配置空间并填满内容
	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_alloc::allocate(n);
		// JD::uninitialized_fill_n(start, n, x);
		iterator cur = result;
		while(n--) {
			*cur = x;
			cur++;
		}
		return result;
	}
};

// 添加时如果空间不足就扩展
template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T &x) {
	if (finish != end_of_storage) {
		// 还有备用空间
		// 在备用空间起始处构造一个元素 并以vector 最后一个元素值为其初值
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;
		std::copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	} else {
		// 无备用空间
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		// 原大小为0 至少分配一个
		// 原大小不为0 则扩大到两倍
		iterator new_start = data_alloc::allocate(len);
		iterator new_finish = new_start;
		try {
			// 将原vector拷贝到 新vector
			new_finish = JD::uninitialized_copy(start, position, new_start);
			// 为新元素设定初值
			construct(new_finish, x);
			++new_finish;
			// 将安插点的原内容也拷贝过来
			new_finish = JD::uninitialized_copy(position, finish, new_finish);

		} catch (...) {
			destory(new_start, new_finish);
			data_alloc::deallocate(new_start, len);
			throw;
		}
		// 析构并释放原vector
		destory(begin(), end());
		deallocate();
		// 调整迭代器
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T &x) {
	if (n != 0) {
		if (size_type(end_of_storage - finish) >= n) {
			// 空间够用
			T x_copy = x;
			// 计算插入点后的现有元素个数
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				// 插入点后现有元素个数大于新增元素个数
				JD::uninitialized_copy(finish - n, finish, finish);
				finish += n; // 尾端后移
				// 将原来位置上的值赋值到各新位置上
				std::copy_backward(position, old_finish - n, old_finish);
				// 从插入点开始填新值
				std::fill(position, position + n, x);
			} else {
				// 有可能在end插入；位置不够 《》p 127
				JD::uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				JD::uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				std::fill(position, old_finish, x_copy);
			}
		} else {
			// 备用空间小于新增元素个数
			// 首先决定新长度：旧长度的两倍，或旧长度+新增元素个数
			const size_type old_size = size();
			const size_type len = old_size + std::max(old_size, n);
			// 
			iterator new_start = data_alloc::allocate(len);
			iterator new_finish = new_start;
			__JD_TRY {
				// 以下首先将插入点之前的数据赋值到新空间中
				new_finish = JD::uninitialized_copy(start, finish, new_start);
				// 将新增元素填入新空间
				new_finish = JD::uninitialized_fill_n(new_finish, n, x);
				// 再将旧的position后的数据赋值
				new_finish = JD::uninitialized_copy(position, finish, new_finish);
			}
		#ifdef __JD_USE_EXCEPTIONS
			catch(...) {
				// 有异常发生
				destory(new_start, new_finish);
				data_alloc::deallocate(new_start, len);
				throw;
			}
		#endif
			// 清除并释放旧的vector
			destory(start, finish);
			deallocate();

			start = new_start;
			finish = new_finish;
			end_of_storage = start + len;
		}
	} 
}

JD_SPACE_END

#endif
