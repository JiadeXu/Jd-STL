/*************************************************************************
	> File Name: jd_queue.h
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 21 23:15:00 2021
 ************************************************************************/

#ifndef _JD_QUEUE_H
#define _JD_QUEUE_H

#include "jd_macro.h"
#include "jd_list.h"

JD_SPACE_BEGIN

template<class T, class Sequence = JD::list<T> >
class queue {
	template<class T2, class Sequence2 >
	friend bool operator== (const queue<T2, Sequence2> &, const queue<T2, Sequence2> &);
	template<class T2, class Sequence2 >
	friend bool operator< (const queue<T2, Sequence2> &, const queue<T2, Sequence2> &);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	// Sequence c; // 底层容器
	Sequence c;
public:
	bool empty() { return c.empty(); }
	size_type size() { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	// queue是先进后出 即 末端进末端出
	void push(const value_type &x) { c.push_back(x); }
	void pop() { c.pop_front(); }
};

template<class T, class Sequence>
bool operator==(const JD::queue<T, Sequence> &x, const JD::queue<T, Sequence> &y) {
	return x.c == y.c;
}

template<class T, class Sequence>
bool operator< (const JD::queue<T, Sequence> &x, const JD::queue<T, Sequence> &y) {
	return x.c < y.c;
}

JD_SPACE_END

#endif
