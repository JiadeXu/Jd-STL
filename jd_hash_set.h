/*************************************************************************
	> File Name: jd_hash_set.h
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 19:16:06 2021
 ************************************************************************/

#ifndef _JD_HASH_SET_H
#define _JD_HASH_SET_H

#include "jd_macro.h"
#include "jd_iterator.h"
#include "default_alloc_template.h"
#include "jd_hashtable.h"
#include "jd_hash_fun.h"
#include "jd_function.h"
#include <cstddef>
#include <utility>

JD_SPACE_BEGIN

template<class Value,
	class HashFunc = JD::my_hash<Value>,
	class EqualKey = std::equal_to<Value>,
	class Alloc = JD::alloc>
class hash_set {
private:
	typedef JD::hashtable<Value, Value, HashFunc, JD::identity<Value>, EqualKey, Alloc> ht;
	ht rep;
public:
	typedef typename ht::key_type key_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;

    typedef typename ht::pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::iterator iterator;
    typedef typename ht::reference reference;
    typedef typename ht::const_reference const_reference;
    typedef typename ht::size_type size_type;
    typedef typename ht::difference_type difference_type;

	hasher hash_funct() const { return rep.hash_funct(); }
	key_equal key_eq() const { return rep.key_eq(); }

public:
	hash_set(): rep(100, hasher(), key_equal()) {}
	explicit hash_set(size_type n): rep(n, hasher(), key_equal()) {}
	hash_set(size_type n, const hasher &hf): rep(n, hf, key_equal()) {}
	hash_set(size_type n, const hasher &hf, const key_equal &eq): rep(n, hf, eq) {}

	// 使用 insert_unique
	template<class InputIterator>
	hash_set(InputIterator f, InputIterator l, size_type n, const hasher &hf): rep(n, hf, key_equal()) {
		for (; f != l; f++) {
			rep.insert_unique(*f);
		}
	}
public:
	size_type size() const { return rep.size(); }
	bool empty() const { return size() == 0; }
	void swap(hash_set& hs) { rep.swap(hs.rep); }
	friend bool operator==(const hash_set &hs1, const hash_set &hs2) {
		return hs1.rep == hs2.rep;
	}
	iterator begin() { return rep.begin(); }
	iterator end() { return rep.end(); }
public:
	std::pair<iterator, bool> insert(const value_type &obj) {
		std::pair<iterator, bool> p = rep.insert_unique(obj);
		return std::pair<iterator, bool>(p.first, p.second);
	}
	std::pair<iterator, bool> insert_noresize(const value_type &obj) {
		std::pair<iterator, bool> p =rep.insert_unique_noresize(obj);
		return std::pair<iterator, bool>(p.first, p.second);
	}
	iterator find(const key_type &key) { return rep.find(key); }
	size_type count(const key_type &key) const { return rep.count(key); }
	std::pair<iterator, iterator> equal_range(const key_type &key) const {
		return rep.equal_range(key);
	}

	size_type erase(const key_type &key) { return rep.erase(key); }
	void erase(const iterator &itr) { rep.erase(itr); }
	void clear() { rep.clear(); }

public:
	void resize(size_type n) { rep.resize(n); }
	size_type bucket_count() const { return rep.bucket_count(); }
	size_type max_bucket_count() const { return rep.max_bucket_count(); }
	size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

JD_SPACE_END

#endif
