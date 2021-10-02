/*************************************************************************
	> File Name: jd_hash_multimap.h
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 21:50:01 2021
 ************************************************************************/

#ifndef _JD_HASH_MULTIMAP_H
#define _JD_HASH_MULTIMAP_H

#include "default_alloc_template.h"
#include "jd_macro.h"
#include "jd_hash_fun.h"
#include "jd_hashtable.h"
#include "jd_function.h"
#include <functional>
#include <utility>

JD_SPACE_BEGIN
template<class Key,
        class T,
        class hashFunc = JD::my_hash<Key>,
        class EqualKey = std::equal_to<Key>,
		class Alloc = JD::alloc>
class hash_multimap {
private:
	typedef JD::hashtable<std::pair<const Key, T>, Key, hashFunc, JD::select1st<std::pair<const Key, T> >, EqualKey, Alloc> ht;
	ht rep;
public:
	typedef typename ht::key_type key_type;
	typedef T data_type;
	typedef T mapped_type;
	typedef typename ht::value_type value_type;
	typedef typename ht::hasher hasher;
	typedef typename ht::key_equal key_equal;

	typedef typename ht::size_type size_type;
    typedef typename ht::pointer pointer;
    typedef typename ht::const_pointer const_pointer;
    typedef typename ht::iterator iterator;
    typedef typename ht::reference reference;
    typedef typename ht::const_reference const_reference;
    typedef typename ht::difference_type difference_type;

	hasher hash_funct() const { return rep.hash_funct(); }
	key_equal key_eq() const { return rep.key_eq(); }

public:
	hash_multimap(): rep(100, hasher(), key_equal()) {}
	explicit hash_multimap(size_type n): rep(n, hasher(), key_equal()) {}
	hash_multimap(size_type n, const hasher &hf): rep(n, hf, key_equal()) {}
	hash_multimap(size_type n, const hasher &hf, const key_equal &eq): rep(n, hf, eq) {}

	// 使用 insert_unique
	template<class InputIterator>
	hash_multimap(InputIterator f, InputIterator l, size_type n, const hasher &hf): rep(n, hf, key_equal()) {
		for (; f != l; f++) {
			rep.insert_unique(*f);
		}
	}

public:
	size_type size() const { return rep.size(); }
	bool empty() const { return size() == 0; }
	void swap(hash_multimap& hs) { rep.swap(hs.rep); }
	friend bool operator==(const hash_multimap &hs1, const hash_multimap &hs2) {
		return hs1.rep == hs2.rep;
	}
	iterator begin() { return rep.begin(); }
	iterator end() { return rep.end(); }
public:
	std::pair<iterator, bool> insert(const value_type &obj) {
		std::pair<iterator, bool> p = rep.insert_equal(obj);
		return std::pair<iterator, bool>(p.first, p.second);
	}
	std::pair<iterator, bool> insert_noresize(const value_type &obj) {
		std::pair<iterator, bool> p = rep.insert_equal_noresize(obj);
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

	T& operator[](const key_type &key) {
		return rep.find_or_insert(value_type(key, T())).second;
	}

public:
	void resize(size_type n) { rep.resize(n); }
	size_type bucket_count() const { return rep.bucket_count(); }
	size_type max_bucket_count() const { return rep.max_bucket_count(); }
	size_type elems_in_bucket(size_type n) const { return rep.elems_in_bucket(n); }
};

JD_SPACE_END

#endif
