#ifndef __BASE_DATATYPE_H__
#define __BASE_DATATYPE_H__

#if defined( BASE_INT64 )

typedef int64_t  base_int_t;
typedef uint64_t base_uint_t;
#define BASE_INT_MAX INT64_MAX
#define BASE_INT_MIN INT64_MIN

#elif defined( BASE_INT32 )

typedef int32_t  base_int_t;
typedef uint32_t base_uint_t;
#define BASE_INT_MAX INT32_MAX
#define BASE_INT_MIN INT32_MIN

#elif defined( BASE_LONG )

typedef long          base_int_t;
typedef unsigned long base_uint_t;
#define BASE_INT_MAX LONG_MAX
#define BASE_INT_MIN LONG_MIN

#elif defined( BASE_INT )

typedef int          base_int_t;
typedef unsigned int base_uint_t;
#define BASE_INT_MAX INT_MAX
#define BASE_INT_MIN INT_MIN

#else

typedef int64_t  base_int_t;
typedef uint64_t base_uint_t;
#define BASE_INT_MAX INT64_MAX
#define BASE_INT_MIN INT64_MIN

#endif

#endif  //__BASE_DATATYPE_H__
