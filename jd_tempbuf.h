/*************************************************************************
	> File Name: jd_tempbuf.h
	> Author: 
	> Mail: 
	> Created Time: Mon Oct  4 17:52:27 2021
 ************************************************************************/

#ifndef _JD_TEMPBUF_H
#define _JD_TEMPBUF_H

#include <climits>
#include <malloc/_malloc.h>
#include <utility>
#include <cstddef>
#include <cstdlib>
#include "jd_macro.h"
#include "jd_iterator.h"
#include "jd_uninitialized.h"
#include "jd_type_traits.h"
#include "jd_construct.h"

JD_SPACE_BEGIN

template<class T>
std::pair<T*, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T *) {
  if (len > ptrdiff_t(INT_MAX / sizeof(T)))
    len = INT_MAX / sizeof(T);

  while (len > 0) {
    T* __tmp = (T*) malloc((size_t)len * sizeof(T));
    if (__tmp != 0)
      return std::pair<T*, ptrdiff_t>(__tmp, len);
    len /= 2;
  }

  return std::pair<T*, ptrdiff_t>((T*)0, 0);
}

template <class _Tp>
inline std::pair<_Tp*, ptrdiff_t> get_temporary_buffer(ptrdiff_t __len, _Tp*) {
  return __get_temporary_buffer(__len, (_Tp*) 0);
}

template<class T>
void return_temporary_buffer(T *p) {
	free(p);
}

template<class ForwardIterator, class T>
class _temporary_buffer {
private:
	ptrdiff_t original_len;
	ptrdiff_t len;
	T *buffer;

	void allocate_buffer() {
		original_len = len;
		buffer = 0;

		if (len > (ptrdiff_t)(INT_MAX / sizeof(int))) {
			len = INT_MAX / sizeof(int);
		}

		while(len > 0) {
			buffer = (T *) malloc(len * sizeof(T));
			if (buffer) {
				break;
			}
			len /= 2;// 防止申请不到足够的空间
		}
	}

	void initialize_buffer(const T &, JD::__true_type) {}
	void initialize_buffer(const T &val, JD::__false_type) {
		JD::uninitialized_fill_n(buffer, len, val);
	}
public:
	ptrdiff_t size() const { return len; }
	ptrdiff_t requested_size() const { return original_len; }
	T* begin() { return buffer; }
	T* end() { return buffer + len; }

	_temporary_buffer(ForwardIterator first, ForwardIterator last) {
		typedef typename JD::__type_traits<T>::has_tirvial_default_constructor has_trivial;

		__JD_TRY {
			len = JD::distance(first, last);
			allocate_buffer();
			if (len > 0) {
				initialize_buffer(*first, has_trivial());
			}
		} __JD_UNWIND(free(buffer); buffer = 0; len = 0);
	}

	virtual ~_temporary_buffer() {
		JD::destory(buffer, buffer + len);
		free(buffer);
	}
private:
	_temporary_buffer(const _temporary_buffer &) {}
	void operator=(const _temporary_buffer &) {}
};

template<class ForwardIterator, class T>
struct temporary_buffer : public _temporary_buffer<ForwardIterator, T> {
	temporary_buffer(ForwardIterator first, temporary_buffer last): _temporary_buffer<ForwardIterator, T>(first, last) {

	}
	~temporary_buffer() {}
};

JD_SPACE_END

#endif
