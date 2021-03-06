/*************************************************************************
	> File Name: jd_hashtable.h
	> Author: 
	> Mail: 
	> Created Time: Fri Oct  1 15:12:08 2021
 ************************************************************************/

#ifndef _JD_HASHTABLE_H
#define _JD_HASHTABLE_H

#include "jd_construct.h"
#include "jd_iterator.h"
#include "jd_macro.h"
#include "default_alloc_template.h"
#include "jd_alloc.h"
#include "jd_vector.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>

JD_SPACE_BEGIN

template<class Value>
struct __hashtable_node {
	__hashtable_node* next;
	Value val;
};

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
class hashtable;

// hashtable的迭代器必须永远维系着与整个 “buckets vector”的关系，并记录目前所指的节点。其前进操作
// 是首先尝试从目前所指的节点出发，前进一个位置（节点），由于节点被安置于list内，所以利用节点的next指针即可
template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> _hashtable;
	typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> iterator;
	// typedef __hashtable_const_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef __hashtable_node<Value> node;

	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node *cur;
	_hashtable* ht; // 保持对容器的连洁关系

	__hashtable_iterator(node *n, _hashtable *tab): cur(n), ht(tab) {}
	__hashtable_iterator() {}
	reference operator*() const { return cur->val; }
	pointer operator->() const { return &(operator*()); }
	// ++i
	iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			// 找到下一个bucket的起点
			// 根据元素值，定位出下一个buckets
			size_type bucket = ht->bkt_num(old->val);
			while(!cur && ++bucket < ht->buckets.size()) {
				cur = ht->buckets[bucket];
			}
		}
		return *this;
	}
	// i++
	iterator operator++(int) {
		iterator tmp = *this;
		++(*this);
		return tmp;
	}
	bool operator==(const iterator &itr) const { return cur == itr.cur; }
	bool operator!=(const iterator &itr) const { return cur != itr.cur; }
};

static const int __num_primes = 28;
static const unsigned long __prime_list[__num_primes] = {
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul, 
  1610612741ul, 3221225473ul, 4294967291ul
};

// 找出与n最接近的质数
inline unsigned long __next_prime(unsigned long n) {
	const unsigned long* first = __prime_list;
	const unsigned long* last = __prime_list + __num_primes;
	const unsigned long* pos = std::lower_bound(first, last, n);
	return pos == last ? *(last - 1) : *pos;
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc = JD::alloc>
class hashtable {
public:
	typedef HashFunc hasher;
	typedef EqualKey key_equal;
	typedef size_t size_type;
	typedef Value value_type;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;
   	typedef Key key_type;

	typedef ptrdiff_t         difference_type;
	typedef value_type&       reference;
	typedef const value_type& const_reference;
	typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> iterator;

  friend struct
  __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>;
private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	typedef __hashtable_node<Value> node;
	typedef simple_alloc<node, Alloc> node_allocator;

	JD::vector<node*, Alloc> buckets;
	size_type num_elements;

private:
	node* new_node(const value_type &obj) {
		node* n = node_allocator::allocate();
		n->next = 0;
		JD::construct(&n->val, obj);
		return n;
	}

	void delete_node(node *n) {
		JD::destory(&n->val);
		node_allocator::deallocate(n);
	}

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node *) 0);
        num_elements = 0;
	}

	size_type next_size(size_type n) const { return __next_prime(n); }
public:
	hashtable(
		size_type n,
		const HashFunc &hf,
		const EqualKey &eql
	): hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
		initialize_buckets(n);
	}

	hasher hash_funct() { return hash; }
  	key_equal key_eq() const { return equals; }

	// bucket 个数即 buckets vector 的大小
	size_type bucket_count() const { return buckets.size(); }
	// ...

	// 总共有多少个buckets
	size_type max_bucket_count() const {
		return __prime_list[__num_primes - 1];
	}

	// 接受实值（value）和buckets 个数
	size_type bkt_num(const Value &obj, size_t n) const {
		return bkt_num_key(ExtractKey()(obj), n);
	}

	// 接受实值
	size_type bkt_num(const Value &obj) const {
		return bkt_num_key(ExtractKey()(obj));
	}

	// 只接受键值
	size_type bkt_num_key(const key_type &key) const {
		return bkt_num_key(key, buckets.size());
	}

	// 接受键值和buckets个数
	size_type bkt_num_key(const key_type &key, size_type n) const {
		return hash(key) % n;
	}

	size_type elems_in_bucket(size_type __bucket) const {
		size_type __result = 0;
		for (node* __cur = buckets[__bucket]; __cur != 0; __cur = __cur->next)
			__result += 1;
		return __result;
	}

	// 从小至大排序
	iterator begin() {
		for (size_type i = 0 ; i < buckets.size(); i++) {
			if (buckets[i] != 0) {
				return iterator(buckets[i], this);
			}
		}
		return end();
	}
	iterator end() { return iterator(0, this); }

	void swap(hashtable &h) {
		std::swap(hash, h.hash);
		std::swap(equals, h.equals);
		std::swap(get_key, h.get_key);
		std::swap(num_elements, h.num_elements);
		buckets.swap(h.buckets);
	}

	void resize(size_type num_elements_hint);
	// 键值不可重复
	std::pair<iterator, bool> insert_unique_noresize(const value_type &obj);
	std::pair<iterator, bool> insert_unique(const value_type &obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}
	iterator insert_equal_noresize(const value_type &obj);
	// 插入元素，允许重复
	iterator insert_equal(const value_type &obj) {
		// 判断需不需要扩容
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}

	size_type erase(const key_type &key);
	void erase(const iterator &itr);

	void clear();
	void copy_from(const hashtable &ht);
  	size_type size() const { return num_elements; }
	iterator find(const key_type &key) {
		size_type bucket_pos = bkt_num(key);
		node *first = buckets[bucket_pos];
		for (; first != 0; first = first->next) {
			if (equals(first->val, key)) {
				break;
			}
		}
		return iterator(first, this);
	}

	reference find_or_insert(const value_type &obj) {
		resize(num_elements + 1);
		size_type n = bkt_num(obj);
		node *first = buckets[n];
		for (node *cur = first; cur; cur = cur->next) {
			if (equals(get_key(cur->val), get_key(obj))) {
				return cur->val;
			}
		}
		node *tmp = new_node(obj);
		tmp->next = first;
		buckets[n] = tmp;
		++num_elements;
		return tmp->val;
	}

	size_type count(const key_type &key) {
		size_type rs = 0;
		const size_type bucket_pos = bkt_num_key(key);
		for (const node *cur = buckets[bucket_pos]; cur; cur = cur->next) {
			if (equals(get_key(cur->val), key)) {
				rs++;
			}
		}
		return rs;
	}

	std::pair<iterator, iterator> equal_range(const key_type &key) {
		typedef std::pair<iterator, iterator> pii;
		const size_type n = bkt_num_key(key);

		for (node *first = buckets[n]; first; first = first->next) {
			if (equal(get_key(first->val), key)) {
				for (node *cur = first->next; cur; cur = cur->next) {
					if (!equal(get_key(cur->val), key)) {
						return pii(iterator(first, this), iterator(cur, this));
					}
				}
				for (size_type m = n + 1; m < buckets.size(); m++) {
					if (buckets[m]) {
						return pii(iterator(first, this), iterator(buckets[m], this));
					}
				}
			}
		}

		return pii(end(), end());
	}

	friend bool operator==(const hashtable &h1, const hashtable &h2) {
		typedef hashtable::node _Node;
		if (h1.buckets.size() != h2.buckets.size())
			return false;
		for (int __n = 0; __n < h1.buckets.size(); ++__n) {
			_Node* __cur1 = h1.buckets[__n];
			_Node* __cur2 = h2.buckets[__n];
			for ( ; __cur1 && __cur2 && __cur1->val == __cur2->val;
				__cur1 = __cur1->next, __cur2 = __cur2->next)
			{}
			if (__cur1 || __cur2)
			return false;
		}
		return true;
	}
};

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint) {
	const size_type old_n = buckets.size();
	if (num_elements_hint > old_n) {
		// 1
		const size_type n = next_size(num_elements_hint);
		if (n > old_n) {
			JD::vector<node*, Alloc> new_buckets(n, (node *)0); // 新buckets
			// 2
			for (size_type old_pos = 0; old_pos < buckets.size(); old_pos++) {
				node *first = buckets[old_pos];
				while(first) {
					size_type new_pos = bkt_num(first->val, n);
					// 旧bucket指向其所对应的下一个节点
					buckets[old_pos] = first->next;
					// 将当前节点插入到新 bucket 内，成为其对应bucket的头节点
					first->next = new_buckets[new_pos];
					new_buckets[new_pos] = first;
					// 回到旧buckets所指的待处理节点的下一点
					first = buckets[old_pos];
				}
			}
			// 2
			buckets.swap(new_buckets);
		}
		// 1
	}
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
std::pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool> 
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type &obj) 
{
	const size_type n = bkt_num(obj); // 决定obj应位于 n 号bucket
	node *first = buckets[n];

	// 如果 buckets[n] 已被占用，此时first 将不为0，于是进入以下循环
	// 走过 bucket 所对应的整个链表
	for (node *cur = first; cur != 0; cur = cur->next) {
		if (equals(get_key(cur->val), get_key(obj))) {
			// 如果发现链表中已经有键值相同
			return std::pair<iterator, bool>(iterator(cur, this), false);
		}
	}

	node *tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	num_elements++;
	return std::pair<iterator, bool>(iterator(tmp, this), true);
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type &obj) 
{
	const size_type n = bkt_num(obj);
	node *first = buckets[n];
	// 如果buckets[n] 已被占用，此时first将不为0
	for (node *cur = first; cur; cur = cur->next) {
		if (equals(get_key(cur->val), get_key(obj))) {
			// 如果发现与链表中的某键值相同，就马上插入，然后返回
			node *tmp = new_node(obj);
			// 保证顺序
			tmp->next = cur->next;
			cur->next = tmp;
			++num_elements;
			return iterator(tmp, this);
		}
	}

	// 没有重复的键值 头插
	node *tmp = new_node(obj);
	tmp->next = first;
	buckets[n] = tmp;
	++num_elements;
	return iterator(tmp, this);
}


template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::size_type 
	hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::erase(const key_type &key) {
	const size_type n = bkt_num_key(key);
	node *first = buckets[n];
	size_type erased = 0;

	if (first) {
		node *cur = first;
		node *next = cur->next;
		while(next) {
			if (equals(key, get_key(next->val))) {
				cur->next = next->next;
				delete_node(next);
				next = cur->next;
				++erased;
				--num_elements;
			} else {
				cur = next;
				next = cur->next;
			}
		}
		if (equals(get_key(first->val), key)) {
			buckets[n] = first->next;
			delete_node(first);
			++erased;
			--num_elements;
		}
	}
	return erased;
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::erase(const typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator &itr) {
	node *p = itr.cur;
	if (p) {
		const size_type n = bkt_num_key(p->val);
		node *cur = buckets[n];

		if (cur == p) {
			// 头结点就是
			buckets[n] = cur->next;
			delete_node(p);
			--num_elements;
		} else {
			node *next = cur->next;
			while (next) {
				if (next == p) {
					cur->next = next->next;
					delete_node(next);
					--num_elements;
					break;
				} else {
					cur = next;
					next = next->next;
				}
			}
		}
	}
}


template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
	for(size_type i = 0; i < buckets.size(); i++) {
		node *cur = buckets[i];
		while(cur != 0) {
			node *next = cur->next;
			delete_node(cur);
			cur = next;
		}
		buckets[i] = 0;
	}
	num_elements = 0;
}

template<class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> &ht) {
	// 先清除
	buckets.clear();
	// 为己方的buckets vector 保留空间 使与对方相同
	// 如果己方大于对方就不动，小就增大
	buckets.reserve(ht.buckets.size());
	// 从及覅的buckets vector 尾端开始 插入n 个元素，值为null
	buckets.insert(buckets.end(), ht.buckets.size(), (node *)0);

	for (size_type i = 0; i < ht.buckets.size(); i++) {
		const node *cur = ht.buckets[i];
		if (cur != 0) {
			node *copy = new_node(cur->val);
			buckets[i] = copy;

			for (node *next = cur->next; next; cur = next, next = cur->next) {
				copy->next = new_node(next->val);
				copy = copy->next;
			}
		}
	}
	num_elements = ht.num_elements;
}

JD_SPACE_END

#endif
