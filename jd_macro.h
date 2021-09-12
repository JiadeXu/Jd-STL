#ifndef _JD_MACRO_
#define _JD_MACRO_

#define JD_SPACE_BEGIN namespace JD {
#define JD_SPACE_END }
#define USING_JD_SPACE using namespace JD
#define JD_SIZE_T unsigned long

#define __JD_STL_TEMPLATE_NULL template<>

JD_SPACE_BEGIN
struct __true_type {};
struct __false_type {};
JD_SPACE_END
#endif