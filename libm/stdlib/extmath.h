#pragma once

#include <stdint.h>
#include <stddef.h>

#define FORCE_EVAL(x) do {									\
	if (sizeof(x) == sizeof(float)) {						\
		volatile float __x __attribute__((unused));			\
		__x = (x);											\
	} else if (sizeof(x) == sizeof(double)) {				\
		volatile double __x __attribute__((unused));		\
		__x = (x);											\
	} else {												\
		volatile long double __x __attribute__((unused));	\
		__x = (x);											\
	}														\
} while(0)

/* Get two 32 bit ints from a double */
#define EXTRACT_WORDS(hi, lo, d)							\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.f = (d);											\
	(hi) = __u.i >> 32;										\
	(lo) = (uint32_t)__u.i;									\
} while (0)

/* Get the more significant 32 bit int from a double */
#define GET_HIGH_WORD(hi, d)								\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.f = (d);											\
	(hi) = __u.i >> 32;										\
} while (0)

/* Get the less significant 32 bit int from a double */
#define GET_LOW_WORD(lo, d)									\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.f = (d);											\
	(lo) = (uint32_t)__u.i;									\
} while (0)

/* Set a double from two 32 bit ints */
#define INSERT_WORDS(d, hi, lo)								\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.i = ((uint64_t)(hi)<<32) | (uint32_t)(lo);			\
	(d) = __u.f;											\
} while (0)

/* Set the more significant 32 bits of a double from an int */
#define SET_HIGH_WORD(d, hi)								\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.f = (d);											\
	__u.i &= 0xffffffff;									\
	__u.i |= (uint64_t)(hi) << 32;							\
	(d) = __u.f;											\
} while (0)

/* Set the less significant 32 bits of a double from an int */
#define SET_LOW_WORD(d, lo)									\
do {														\
	union {double f; uint64_t i;} __u;						\
	__u.f = (d);											\
	__u.i &= 0xffffffff00000000ull;							\
	__u.i |= (uint32_t)(lo);								\
	(d) = __u.f;											\
} while (0)

/* Get a 32 bit int from a float */
#define GET_FLOAT_WORD(w, d)								\
do {														\
	union {float f; uint32_t i;} __u;						\
	__u.f = (d);											\
	(w) = __u.i;											\
} while (0)

/* Set a float from a 32 bit int */
#define SET_FLOAT_WORD(d, w)								\
do {														\
	union {float f; uint32_t i;} __u;						\
	__u.i = (w);											\
	(d) = __u.f;											\
} while (0)

#define FP_NAN					0
#define FP_INFINITE				1
#define FP_ZERO					2
#define FP_SUBNORMAL			3
#define FP_NORMAL				4

#define NAN						__builtin_nan("")
#define INFINITY				__builtin_inf()
#define	HUGE_VALF				__builtin_huge_valf()
#define	HUGE_VAL				__builtin_huge_val()
#define	HUGE_VALL				__builtin_huge_vall()

#define	isgreater(x, y)			__builtin_isgreater((x), (y))
#define	isgreaterequal(x, y)	__builtin_isgreaterequal((x), (y))
#define	isless(x, y)			__builtin_isless((x), (y))
#define	islessequal(x, y)		__builtin_islessequal((x), (y))
#define	islessgreater(x, y)		__builtin_islessgreater((x), (y))
#define	isunordered(x, y)		__builtin_isunordered((x), (y))

struct ieee_double {
	unsigned int	dbl_fracl;
	unsigned int	dbl_frach:20;
	unsigned int	dbl_exp:11;
	unsigned int	dbl_sign:1;
};

struct ieee_single {
	unsigned int	sng_frac:23;
	unsigned int	sng_exp:8;
	unsigned int	sng_sign:1;
};

#define	SNG_EXP_INFNAN	255
#define	DBL_EXP_INFNAN	2047
#define	EXT_EXP_INFNAN	32767

int __fpclassifyf(float f);
int __fpclassify(double d);


static __inline unsigned __FLOAT_BITS(float __f)
{
	union {float __f; unsigned __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

static __inline unsigned long long __DOUBLE_BITS(double __f)
{
	union {double __f; unsigned long long __i;} __u;
	__u.__f = __f;
	return __u.__i;
}

#define fpclassify(x) ( \
	sizeof(x) == sizeof(float) ? __fpclassifyf(x) : \
	sizeof(x) == sizeof(double) ? __fpclassify(x) : \
	__fpclassify(x) )

#define isinf(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) == 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) == 0x7ffULL<<52 : \
	__fpclassify(x) == FP_INFINITE)

#define isnan(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) > 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) > 0x7ffULL<<52 : \
	__fpclassify(x) == FP_NAN)

#define isnormal(x) ( \
	sizeof(x) == sizeof(float) ? ((__FLOAT_BITS(x)+0x00800000) & 0x7fffffff) >= 0x01000000 : \
	sizeof(x) == sizeof(double) ? ((__DOUBLE_BITS(x)+(1ULL<<52)) & -1ULL>>1) >= 1ULL<<53 : \
	__fpclassify(x) == FP_NORMAL)

#define isfinite(x) ( \
	sizeof(x) == sizeof(float) ? (__FLOAT_BITS(x) & 0x7fffffff) < 0x7f800000 : \
	sizeof(x) == sizeof(double) ? (__DOUBLE_BITS(x) & -1ULL>>1) < 0x7ffULL<<52 : \
	__fpclassify(x) > FP_INFINITE)

#define M_E						2.7182818284590452354	/* e */
#define M_LOG2E					1.4426950408889634074	/* log_2 e */
#define M_LOG10E				0.43429448190325182765	/* log_10 e */
#define M_LN2					0.69314718055994530942	/* log_e 2 */
#define M_LN10					2.30258509299404568402	/* log_e 10 */
#define M_PI					3.14159265358979323846	/* pi */
#define M_PI_2					1.57079632679489661923	/* pi/2 */
#define M_PI_4					0.78539816339744830962	/* pi/4 */
#define M_1_PI					0.31830988618379067154	/* 1/pi */
#define M_2_PI					0.63661977236758134308	/* 2/pi */
#define M_2_SQRTPI				1.12837916709551257390	/* 2/sqrt(pi) */
#define M_SQRT2					1.41421356237309504880	/* sqrt(2) */
#define M_SQRT1_2				0.70710678118654752440	/* 1/sqrt(2) */
#define asuint(f) \
  ((union {       \
    float _f;     \
    uint32_t _i;  \
  }){f})          \
      ._i
#define asfloat(i) \
  ((union {        \
    uint32_t _i;   \
    float _f;      \
  }){i})           \
      ._f
#define asuint64(f) \
  ((union {         \
    double _f;      \
    uint64_t _i;    \
  }){f})            \
      ._i
#define asdouble(i) \
  ((union {         \
    uint64_t _i;    \
    double _f;      \
  }){i})            \
      ._f
#define predict_true(x) __builtin_expect(!!(x), 1)
#define predict_false(x) __builtin_expect(x, 0)

float Ln(float x);
double Modf(double x, double *iptr);
double Floor(double x);
double Scalbn(double x, int n);
float Cosf(float x); 
double Fabs(double x);
double Sqrt(double x);
double Atan2(double y, double x);
float Sinf(float x);
