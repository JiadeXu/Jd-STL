#ifndef _JD_MACRO_
#define _JD_MACRO_

#define JD_SPACE_BEGIN namespace JD {
#define JD_SPACE_END }
#define USING_JD_SPACE using namespace JD
#define JD_SIZE_T unsigned long

#define __JD_STL_TEMPLATE_NULL template<>
#define __JD_STL_TEMPLATE_ARGS <>

#ifdef __JD_USE_EXCEPTIONS
#define __JD_TRY   try
#define __JD_UNWIND(action)   catch(...) { action; throw; }
#else
#define __JD_TRY
#define __JD_UNWIND(action)
#endif

JD_SPACE_BEGIN
struct __true_type {};
struct __false_type {};
JD_SPACE_END
#endif