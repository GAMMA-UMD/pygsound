/***
* zmminitrin.h - Meta Header file for Intel(R) Architecture intrinsic functions.
*
* Copyright (C) 2007-2016 Intel Corporation.  All rights reserved.
*
* The information and source code contained herein is the exclusive
* property of Intel Corporation and may not be disclosed, examined
* or reproduced in whole or in part without explicit written authorization
* from the company.
*
*
*******************************************************************************/

#ifndef _ZMMINTRIN_H_INCLUDED
#define _ZMMINTRIN_H_INCLUDED

//#ifndef _INCLUDED_IMM
//#error "Header should only be included from <immintrin.h>."
//#endif

//
// Definitions and declarations for use with 512-bit compiler intrinsics.
//

//
// A word about intrinsic function naming conventions.  Most 512-bit
// vector instructions have names such as v<operation><type>.  For
// example "vaddps" is an addition operation (add) on packed single-
// precision (ps) values.  The corresponding intrinsic is usually
// (not always) named _mm512_<operation>_<type>. For example, the
// "_mm512_add_ps" function generates VADDPS.  The corresponding
// masked flavor adds "_mask" to the name, e.g. "_mm512_mask_add_ps".
//
// The types include:
//
//    ps    -- packed single precision
//    pd    -- packed double precision
//    epi32 -- packed 32-bit integers
//    epu32 -- packed 32-bit unsigned integers
//    epi64 -- packed 64-bit integers
//

typedef unsigned char       __mmask8;
typedef unsigned short      __mmask16;
typedef unsigned int        __mmask32;
typedef unsigned __int64    __mmask64;

typedef union __declspec(intrin_type) /* __declspec(align(64)) */ __m512 {
    float m512_f32[16];
} __m512;

typedef struct __declspec(intrin_type) /* __declspec(align(64)) */ __m512d {
    double m512d_f64[8];
} __m512d;

typedef union  __declspec(intrin_type) /* __declspec(align(64)) */ __m512i {
    __int8              m512i_i8[64];
    __int16             m512i_i16[32];
    __int32             m512i_i32[16];
    __int64             m512i_i64[8];
    unsigned __int8     m512i_u8[64];
    unsigned __int16    m512i_u16[32];
    unsigned __int32    m512i_u32[16];
    unsigned __int64    m512i_u64[8];
} __m512i;


#ifdef __cplusplus
extern "C" {
// Intrinsic functions use C name-mangling.
#endif /* __cplusplus */

/* Conversion from one type to another, no change in value. */
extern __m256  __cdecl _mm512_castps512_ps256(__m512);
extern __m512  __cdecl _mm512_castpd_ps(__m512d);
extern __m512  __cdecl _mm512_castps256_ps512(__m256);
extern __m512  __cdecl _mm512_castsi512_ps(__m512i);
extern __m512  __cdecl _mm512_castps128_ps512(__m128);

extern __m256d __cdecl _mm512_castpd512_pd256(__m512d);
extern __m512d __cdecl _mm512_castpd256_pd512(__m256d);
extern __m512d __cdecl _mm512_castps_pd(__m512);
extern __m512d __cdecl _mm512_castsi512_pd(__m512i);
extern __m512d __cdecl _mm512_castpd128_pd512(__m128d);

extern __m256i __cdecl _mm512_castsi512_si256(__m512i);
extern __m512i __cdecl _mm512_castpd_si512(__m512d);
extern __m512i __cdecl _mm512_castps_si512(__m512);
extern __m512i __cdecl _mm512_castsi256_si512(__m256i);

/* Constant for special read-only mask register 'k0'. */
#define _MM_K0_REG (0xffff)
#define _MM_K0_REG64 (0xffffffffffffffff)

/* Constants for broadcasts to vectors with 32-bit elements. */
typedef enum {
    _MM_BROADCAST32_NONE,   /* identity swizzle/convert */
#define _MM_BROADCAST_16X16 _MM_BROADCAST32_NONE
    _MM_BROADCAST_1X16,     /* broadcast x 16 ( aaaa aaaa aaaa aaaa ) */
    _MM_BROADCAST_4X16      /* broadcast x 4  ( dcba dcba dcba dcba ) */
} _MM_BROADCAST32_ENUM;

/* Constants for broadcasts to vectors with 64-bit elements. */
typedef enum {
    _MM_BROADCAST64_NONE,   /* identity swizzle/convert */
#define _MM_BROADCAST_8X8 _MM_BROADCAST64_NONE
    _MM_BROADCAST_1X8,      /* broadcast x 8 ( aaaa aaaa ) */
    _MM_BROADCAST_4X8       /* broadcast x 2 ( dcba dcba ) */
} _MM_BROADCAST64_ENUM;

/*
 * Constants for rounding mode.
 * These names beginnig with "_MM_ROUND" are deprecated.
 * Use the names beginning with "_MM_FROUND" going forward.
 */
typedef enum {
    _MM_ROUND_MODE_NEAREST,             /* round to nearest (even) */
    _MM_ROUND_MODE_DOWN,                /* round toward negative infinity */
    _MM_ROUND_MODE_UP,                  /* round toward positive infinity */
    _MM_ROUND_MODE_TOWARD_ZERO,         /* round toward zero */
    _MM_ROUND_MODE_DEFAULT              /* round mode from MXCSR */
} _MM_ROUND_MODE_ENUM;

/* Constants for exponent adjustment. */
typedef enum {
    _MM_EXPADJ_NONE,               /* 2**0  (32.0 - no exp adjustment) */
    _MM_EXPADJ_4,                  /* 2**4  (28.4)  */
    _MM_EXPADJ_5,                  /* 2**5  (27.5)  */
    _MM_EXPADJ_8,                  /* 2**8  (24.8)  */
    _MM_EXPADJ_16,                 /* 2**16 (16.16) */
    _MM_EXPADJ_24,                 /* 2**24 (8.24)  */
    _MM_EXPADJ_31,                 /* 2**31 (1.31)  */
    _MM_EXPADJ_32                  /* 2**32 (0.32)  */
} _MM_EXP_ADJ_ENUM;

/* Constants for index scale (vgather/vscatter). */
typedef enum {
    _MM_SCALE_1 = 1,
    _MM_SCALE_2 = 2,
    _MM_SCALE_4 = 4,
    _MM_SCALE_8 = 8
} _MM_INDEX_SCALE_ENUM;

/*
 * Helper type and macro for computing the values of the immediate
 * used in mm512_fixup_ps.
 */
typedef enum {
    _MM_FIXUP_NO_CHANGE,
    _MM_FIXUP_NEG_INF,
    _MM_FIXUP_NEG_ZERO,
    _MM_FIXUP_POS_ZERO,
    _MM_FIXUP_POS_INF,
    _MM_FIXUP_NAN,
    _MM_FIXUP_MAX_FLOAT,
    _MM_FIXUP_MIN_FLOAT
} _MM_FIXUPRESULT_ENUM;

#define _MM_FIXUP(_NegInf, \
                  _Neg, \
                  _NegZero, \
                  _PosZero, \
                  _Pos, \
                  _PosInf, \
                  _Nan) \
   ((int) (_NegInf) | \
   ((int) (_Neg) << 3) | \
   ((int) (_NegZero) << 6) | \
   ((int) (_PosZero) << 9) | \
   ((int) (_Pos) << 12) | \
   ((int) (_PosInf) << 15) | \
   ((int) (_Nan) << 18))

/*
* Extract float32 or float64 normalized mantissas.
*/

/* Constants for mantissa extraction */
typedef enum {
    _MM_MANT_NORM_1_2,      /* interval [1, 2)      */
    _MM_MANT_NORM_p5_2,     /* interval [1.5, 2)    */
    _MM_MANT_NORM_p5_1,     /* interval [1.5, 1)    */
    _MM_MANT_NORM_p75_1p5   /* interval [0.75, 1.5) */
} _MM_MANTISSA_NORM_ENUM;

typedef enum {
    _MM_MANT_SIGN_src,      /* sign = sign(SRC)     */
    _MM_MANT_SIGN_zero,     /* sign = 0             */
    _MM_MANT_SIGN_nan       /* DEST = NaN if sign(SRC) = 1 */
} _MM_MANTISSA_SIGN_ENUM;

/*
* Compare float32, float64 or int32 vectors and set mask.
*/

/* Constants for integer comparison predicates */
typedef enum {
    _MM_CMPINT_EQ,      /* Equal */
    _MM_CMPINT_LT,      /* Less than */
    _MM_CMPINT_LE,      /* Less than or Equal */
    _MM_CMPINT_UNUSED,
    _MM_CMPINT_NE,      /* Not Equal */
    _MM_CMPINT_NLT,     /* Not Less than */
#define _MM_CMPINT_GE   _MM_CMPINT_NLT  /* Greater than or Equal */
    _MM_CMPINT_NLE      /* Not Less than or Equal */
#define _MM_CMPINT_GT   _MM_CMPINT_NLE  /* Greater than */
} _MM_CMPINT_ENUM;


/*
* Intel(R) AVX-512 intrinsic functions
*/
extern __m512  __cdecl _mm512_setzero_ps(void);
extern __m512d __cdecl _mm512_setzero_pd(void);

extern __m512  __cdecl _mm512_set_ps(float /* e15 */, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float /* e0 */);
extern __m512d __cdecl _mm512_set_pd(double /* e7 */, double, double, double, double, double, double, double /* e0 */);

extern __m512  __cdecl _mm512_set1_ps(float);
extern __m512d __cdecl _mm512_set1_pd(double);

extern __m512  __cdecl _mm512_maskz_load_ps(__mmask16, void const*);
extern __m512d __cdecl _mm512_maskz_load_pd(__mmask8, void const*);
extern __m512  __cdecl _mm512_mask_load_ps(__m512, __mmask16, void const*);
extern __m512d __cdecl _mm512_mask_load_pd(__m512d, __mmask8, void const*);
extern __m512  __cdecl _mm512_maskz_loadu_ps(__mmask16, void const*);
extern __m512d __cdecl _mm512_maskz_loadu_pd(__mmask8, void const*);
extern __m512  __cdecl _mm512_mask_loadu_ps(__m512, __mmask16, void const*);
extern __m512d __cdecl _mm512_mask_loadu_pd(__m512d, __mmask8, void const*);

extern void    __cdecl _mm512_storeu_ps(void*, __m512);
extern void    __cdecl _mm512_storeu_pd(void*, __m512d);
extern void    __cdecl _mm512_mask_store_ps(void*, __mmask16, __m512);
extern void    __cdecl _mm512_mask_store_pd(void*, __mmask8, __m512d);
extern void    __cdecl _mm512_mask_storeu_ps(void*, __mmask16, __m512);
extern void    __cdecl _mm512_mask_storeu_pd(void*, __mmask8, __m512d);

extern __m512  __cdecl _mm512_maskz_add_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_add_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_add_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_add_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_sub_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_sub_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_sub_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_sub_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_mul_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_mul_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_mul_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_mul_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512d __cdecl _mm512_maskz_div_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_div_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_div_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_div_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_fmadd_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fmadd_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fmadd_round_ps(__mmask16, __m512, __m512, __m512, const int);
extern __m512d __cdecl _mm512_mask_fmadd_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fmadd_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fmadd_round_pd(__mmask8, __m512d, __m512d, __m512d, const int);

extern __m512  __cdecl _mm512_mask_fmsub_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fmsub_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fmsub_round_ps(__mmask16, __m512, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_fmsub_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fmsub_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fmsub_round_pd(__mmask8, __m512d, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_fmaddsub_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fmaddsub_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fmaddsub_round_ps(__mmask16, __m512, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_fmaddsub_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fmaddsub_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fmaddsub_round_pd(__mmask8, __m512d, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_fmsubadd_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fmsubadd_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fmsubadd_round_ps(__mmask16, __m512, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_fmsubadd_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fmsubadd_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fmsubadd_round_pd(__mmask8, __m512d, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_fnmadd_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fnmadd_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fnmadd_round_ps(__mmask16, __m512, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_fnmadd_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fnmadd_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fnmadd_round_pd(__mmask8, __m512d, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_fnmsub_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512  __cdecl _mm512_mask3_fnmsub_round_ps(__m512, __m512, __m512, __mmask16, const int /* rounding */);
extern __m512  __cdecl _mm512_maskz_fnmsub_round_ps(__mmask16, __m512, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_fnmsub_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask3_fnmsub_round_pd(__m512d, __m512d, __m512d, __mmask8, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_fnmsub_round_pd(__mmask8, __m512d, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_sqrt_round_ps(__mmask16, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_sqrt_round_pd(__mmask8, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_sqrt_round_ps(__m512, __mmask16, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_sqrt_round_pd(__m512d, __mmask8, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_abs_ps(__mmask16, __m512);
extern __m512  __cdecl _mm512_mask_abs_ps(__m512, __mmask16, __m512);
extern __m512d __cdecl _mm512_maskz_abs_pd(__mmask8, __m512d);
extern __m512d __cdecl _mm512_mask_abs_pd(__m512d, __mmask8, __m512d);

extern __m512  __cdecl _mm512_maskz_max_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_max_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_max_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_max_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_min_round_ps(__mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_maskz_min_round_pd(__mmask8, __m512d, __m512d, const int /* rounding */);
extern __m512  __cdecl _mm512_mask_min_round_ps(__m512, __mmask16, __m512, __m512, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_min_round_pd(__m512d, __mmask8, __m512d, __m512d, const int /* rounding */);

extern __m512  __cdecl _mm512_maskz_rcp14_ps(__mmask16, __m512);
extern __m512d __cdecl _mm512_maskz_rcp14_pd(__mmask8, __m512d);
extern __m512  __cdecl _mm512_mask_rcp14_ps(__m512, __mmask16, __m512);
extern __m512d __cdecl _mm512_mask_rcp14_pd(__m512d, __mmask8, __m512d);

extern __m512  __cdecl _mm512_maskz_rsqrt14_ps(__mmask16, __m512);
extern __m512d __cdecl _mm512_maskz_rsqrt14_pd(__mmask8, __m512d);
extern __m512  __cdecl _mm512_mask_rsqrt14_ps(__m512, __mmask16, __m512);
extern __m512d __cdecl _mm512_mask_rsqrt14_pd(__m512d, __mmask8, __m512d);

extern __m512  __cdecl _mm512_mask_rcp28_round_ps(__m512, __mmask16, __m512, const int);
extern __m512  __cdecl _mm512_maskz_rcp28_round_ps(__mmask16, __m512, const int);
extern __m512d __cdecl _mm512_mask_rcp28_round_pd(__m512d, __mmask8, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_rcp28_round_pd(__mmask8, __m512d, const int);
extern __m512  __cdecl _mm512_mask_rsqrt28_round_ps(__m512, __mmask16, __m512, const int);
extern __m512  __cdecl _mm512_maskz_rsqrt28_round_ps(__mmask16, __m512, const int);
extern __m512d __cdecl _mm512_mask_rsqrt28_round_pd(__m512d, __mmask8, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_rsqrt28_round_pd(__mmask8, __m512d, const int);

extern __m512d __cdecl _mm512_maskz_cvt_roundps_pd(__mmask8, __m256, const int /* rounding */);
extern __m256  __cdecl _mm512_maskz_cvt_roundpd_ps(__mmask8, __m512d, const int /* rounding */);
extern __m512d __cdecl _mm512_mask_cvt_roundps_pd(__m512d, __mmask8, __m256, const int /* rounding */);
extern __m256  __cdecl _mm512_mask_cvt_roundpd_ps(__m256, __mmask8, __m512d, const int /* rounding */);

extern __mmask16 __cdecl _mm512_mask_cmp_round_ps_mask(__mmask16, __m512, __m512, const int, const int /* rounding */);
extern __mmask8  __cdecl _mm512_mask_cmp_round_pd_mask(__mmask8, __m512d, __m512d, const int, const int /* rounding */);

extern __m512  __cdecl _mm512_mask_broadcast_f32x2(__m512, __mmask16, __m128);
extern __m512  __cdecl _mm512_maskz_broadcast_f32x2(__mmask16, __m128);
extern __m512  __cdecl _mm512_mask_broadcast_f32x4(__m512, __mmask16, __m128);
extern __m512  __cdecl _mm512_maskz_broadcast_f32x4(__mmask16, __m128);
extern __m512  __cdecl _mm512_mask_broadcast_f32x8(__m512, __mmask16, __m256);
extern __m512  __cdecl _mm512_maskz_broadcast_f32x8(__mmask16, __m256);
extern __m512d __cdecl _mm512_mask_broadcast_f64x2(__m512d, __mmask8, __m128d);
extern __m512d __cdecl _mm512_maskz_broadcast_f64x2(__mmask8, __m128d);
extern __m512d __cdecl _mm512_mask_broadcast_f64x4(__m512d, __mmask8, __m256d);
extern __m512d __cdecl _mm512_maskz_broadcast_f64x4(__mmask8, __m256d);
extern __m512d __cdecl _mm512_mask_broadcastsd_pd(__m512d, __mmask8, __m128d);
extern __m512d __cdecl _mm512_maskz_broadcastsd_pd(__mmask8, __m128d);
extern __m512  __cdecl _mm512_mask_broadcastss_ps(__m512, __mmask16, __m128);
extern __m512  __cdecl _mm512_maskz_broadcastss_ps(__mmask16, __m128);

extern __m128  __cdecl _mm512_mask_extractf32x4_ps(__m128, __mmask8, __m512, const int);
extern __m128  __cdecl _mm512_maskz_extractf32x4_ps(__mmask8, __m512, int);
extern __m256  __cdecl _mm512_mask_extractf32x8_ps(__m256, __mmask8, __m512, const int);
extern __m256  __cdecl _mm512_maskz_extractf32x8_ps(__mmask8, __m512, int);
extern __m128d __cdecl _mm512_mask_extractf64x2_pd(__m128d, __mmask8, __m512d, const int);
extern __m128d __cdecl _mm512_maskz_extractf64x2_pd(__mmask8, __m512d, int);
extern __m256d __cdecl _mm512_mask_extractf64x4_pd(__m256d, __mmask8, __m512d, const int);
extern __m256d __cdecl _mm512_maskz_extractf64x4_pd(__mmask8, __m512d, int);

extern __m512  __cdecl _mm512_mask_insertf32x4(__m512, __mmask16, __m512, __m128, const int);
extern __m512  __cdecl _mm512_maskz_insertf32x4(__mmask16, __m512, __m128, int);
extern __m512  __cdecl _mm512_mask_insertf32x8(__m512, __mmask16, __m512, __m256, const int);
extern __m512  __cdecl _mm512_maskz_insertf32x8(__mmask16, __m512, __m256, int);
extern __m512d __cdecl _mm512_mask_insertf64x2(__m512d, __mmask8, __m512d, __m128d, const int);
extern __m512d __cdecl _mm512_maskz_insertf64x2(__mmask8, __m512d, __m128d, int);
extern __m512d __cdecl _mm512_mask_insertf64x4(__m512d, __mmask8, __m512d, __m256d, const int);
extern __m512d __cdecl _mm512_maskz_insertf64x4(__mmask8, __m512d, __m256d, int);

extern __m512  __cdecl _mm512_mask_shuffle_f32x4(__m512, __mmask16, __m512, __m512, const int);
extern __m512  __cdecl _mm512_maskz_shuffle_f32x4(__mmask16, __m512, __m512, const int);
extern __m512d __cdecl _mm512_mask_shuffle_f64x2(__m512d, __mmask8, __m512d, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_shuffle_f64x2(__mmask8, __m512d, __m512d, const int);
extern __m512d __cdecl _mm512_mask_shuffle_pd(__m512d, __mmask8, __m512d, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_shuffle_pd(__mmask8, __m512d, __m512d, const int);
extern __m512  __cdecl _mm512_mask_shuffle_ps(__m512, __mmask16, __m512, __m512, const int);
extern __m512  __cdecl _mm512_maskz_shuffle_ps(__mmask16, __m512, __m512, const int);


#define _mm512_setzero() _mm512_setzero_ps()
#define _mm512_undefined() _mm512_setzero()
#define _mm512_undefined_pd() _mm512_setzero_pd()
#define _mm512_undefined_ps() _mm512_undefined()

#define _mm512_set4_ps(a,b,c,d) \
    _mm512_set_ps((a), (b), (c), (d), (a), (b), (c), (d), \
    (a), (b), (c), (d), (a), (b), (c), (d))

#define _mm512_set4_pd(a,b,c,d) \
    _mm512_set_pd((a), (b), (c), (d), (a), (b), (c), (d))

#define _mm512_setr_ps(e0,e1,e2,e3,e4,e5,e6,e7,e8, \
    e9, e10, e11, e12, e13, e14, e15) \
    _mm512_set_ps((e15), (e14), (e13), (e12), (e11), (e10), \
    (e9), (e8), (e7), (e6), (e5), (e4), (e3), (e2), (e1), (e0))

#define _mm512_set_16to16_ps(e0,e1,e2,e3,e4,e5,e6,e7,e8, \
    e9, e10, e11, e12, e13, e14, e15) \
    _mm512_set_ps((e0), (e1), (e2), (e3), (e4), (e5), (e6), (e7), \
    (e8), (e9), (e10), (e11), (e12), (e13), (e14), (e15))

#define _mm512_setr_pd(e0,e1,e2,e3,e4,e5,e6,e7) \
    _mm512_set_pd((e7), (e6), (e5), (e4), (e3), (e2), (e1), (e0))

#define _mm512_set_8to8_pd(e0,e1,e2,e3,e4,e5,e6,e7) \
    _mm512_set_pd((e0), (e1), (e2), (e3), (e4), (e5), (e6), (e7))

#define _mm512_setr4_ps(a,b,c,d) \
    _mm512_set4_ps((d), (c), (b), (a))

#define _mm512_set_4to16_ps(a,b,c,d) \
    _mm512_set4_ps((d), (c), (b), (a))

#define _mm512_setr4_pd(a,b,c,d) \
    _mm512_set4_pd((d), (c), (b), (a))

#define _mm512_set_4to8_pd(a,b,c,d) \
    _mm512_set4_pd((d), (c), (b), (a))

#define _mm512_set_1to16_ps(x) _mm512_set1_ps((x))
#define _mm512_set_1to8_pd(x) _mm512_set1_pd((x))

#define _mm512_load_ps(x) _mm512_maskz_load_ps(_MM_K0_REG, (x))
#define _mm512_load_pd(x) _mm512_maskz_load_pd((__mmask8)_MM_K0_REG, (x))
#define _mm512_loadu_ps(x) _mm512_maskz_loadu_ps(_MM_K0_REG, (x))
#define _mm512_loadu_pd(x) _mm512_maskz_loadu_pd((__mmask8)_MM_K0_REG, (x))

#define _mm512_store_ps(x, v) _mm512_mask_store_ps((x), _MM_K0_REG, (v))
#define _mm512_store_pd(x, v) _mm512_mask_store_pd((x), (__mmask8)_MM_K0_REG, (v))

#define _mm512_add_ps(v1, v2) \
    _mm512_maskz_add_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_add_round_ps(v1, v2, e3) \
    _mm512_maskz_add_round_ps(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_add_ps(k1, v2, v3) \
    _mm512_maskz_add_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_add_ps(v1, k2, v3, v4) \
    _mm512_mask_add_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_add_pd(v1, v2) \
    _mm512_maskz_add_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_add_round_pd(v1, v2, e3) \
    _mm512_maskz_add_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_add_pd(k1, v2, v3) \
    _mm512_maskz_add_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_add_pd(v1, k2, v3, v4) \
    _mm512_mask_add_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)

#define _mm512_sub_ps(v1, v2) \
    _mm512_maskz_sub_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sub_round_ps(v1, v2, e3) \
    _mm512_maskz_sub_round_ps(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_sub_ps(k1, v2, v3) \
    _mm512_maskz_sub_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_sub_ps(v1, k2, v3, v4) \
    _mm512_mask_sub_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sub_pd(v1, v2) \
    _mm512_maskz_sub_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sub_round_pd(v1, v2, e3) \
    _mm512_maskz_sub_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_sub_pd(k1, v2, v3) \
    _mm512_maskz_sub_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_sub_pd(v1, k2, v3, v4) \
    _mm512_mask_sub_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)

#define _mm512_mul_ps(v1, v2) \
    _mm512_maskz_mul_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mul_round_ps(v1, v2, e3) \
    _mm512_maskz_mul_round_ps(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_mul_ps(k1, v2, v3) \
    _mm512_maskz_mul_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_mul_ps(v1, k2, v3, v4) \
    _mm512_mask_mul_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mul_pd(v1, v2) \
    _mm512_maskz_mul_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mul_round_pd(v1, v2, e3) \
    _mm512_maskz_mul_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_mul_pd(k1, v2, v3) \
    _mm512_maskz_mul_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_mul_pd(v1, k2, v3, v4) \
    _mm512_mask_mul_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)

#define _mm512_div_ps(v1, v2) \
    _mm512_maskz_div_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_div_round_ps(v1, v2, e3) \
    _mm512_maskz_div_round_ps(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_div_ps(k1, v2, v3) \
    _mm512_maskz_div_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_div_ps(v1, k2, v3, v4) \
    _mm512_mask_div_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_div_pd(v1, v2) \
    _mm512_maskz_div_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_div_round_pd(v1, v2, e3) \
    _mm512_maskz_div_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_maskz_div_pd(k1, v2, v3) \
    _mm512_maskz_div_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_div_pd(v1, k2, v3, v4) \
    _mm512_mask_div_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)

#define _mm512_fmadd_round_ps(v1, v2, v3, e4) \
    _mm512_maskz_fmadd_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fmadd_ps(v1, v2, v3) \
    _mm512_fmadd_round_ps((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fmadd_ps(v1, k1, v2, v3) \
    _mm512_mask_fmadd_round_ps((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmadd_ps(k1, v1, v2, v3) \
    _mm512_maskz_fmadd_round_ps((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmadd_round_pd(v1, v2, v3, e4) \
    _mm512_maskz_fmadd_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fmadd_pd(v1, v2, v3) \
    _mm512_fmadd_round_pd((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fmadd_pd(v1, k1, v2, v3) \
    _mm512_mask_fmadd_round_pd((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmadd_pd(k1, v1, v2, v3) \
    _mm512_maskz_fmadd_round_pd((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)

#define _mm512_fmsub_round_ps(v1, v2, v3, e4) \
    _mm512_maskz_fmsub_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fmsub_ps(v1, v2, v3) \
    _mm512_fmsub_round_ps((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fmsub_ps(v1, k1, v2, v3) \
    _mm512_mask_fmsub_round_ps((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmsub_ps(k1, v1, v2, v3) \
    _mm512_maskz_fmsub_round_ps((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmsub_round_pd(v1, v2, v3, e4) \
    _mm512_maskz_fmsub_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fmsub_pd(v1, v2, v3) \
    _mm512_fmsub_round_pd((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fmsub_pd(v1, k1, v2, v3) \
    _mm512_mask_fmsub_round_pd((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmsub_pd(k1, v1, v2, v3) \
    _mm512_maskz_fmsub_round_pd((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)

#define _mm512_fnmadd_round_ps(v1, v2, v3, e4) \
    _mm512_maskz_fnmadd_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fnmadd_ps(v1, v2, v3) \
    _mm512_fnmadd_round_ps((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fnmadd_ps(v1, k1, v2, v3) \
    _mm512_mask_fnmadd_round_ps((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fnmadd_ps(k1, v1, v2, v3) \
    _mm512_maskz_fnmadd_round_ps((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_fnmadd_round_pd(v1, v2, v3, e4) \
    _mm512_maskz_fnmadd_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fnmadd_pd(v1, v2, v3) \
    _mm512_fnmadd_round_pd((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fnmadd_pd(v1, k1, v2, v3) \
    _mm512_mask_fnmadd_round_pd((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fnmadd_pd(k1, v1, v2, v3) \
    _mm512_maskz_fnmadd_round_pd((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)

#define _mm512_fnmsub_round_ps(v1, v2, v3, e4) \
    _mm512_maskz_fnmsub_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fnmsub_ps(v1, v2, v3) \
    _mm512_fnmsub_round_ps((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fnmsub_ps(v1, k1, v2, v3) \
    _mm512_mask_fnmsub_round_ps((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fnmsub_ps(k1, v1, v2, v3) \
    _mm512_maskz_fnmsub_round_ps((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_fnmsub_round_pd(v1, v2, v3, e4) \
    _mm512_maskz_fnmsub_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e4))
#define _mm512_fnmsub_pd(v1, v2, v3) \
    _mm512_fnmsub_round_pd((v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_fnmsub_pd(v1, k1, v2, v3) \
    _mm512_mask_fnmsub_round_pd((v1), (k1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fnmsub_pd(k1, v1, v2, v3) \
    _mm512_maskz_fnmsub_round_pd((k1), (v1), (v2), (v3), \
    _MM_FROUND_CUR_DIRECTION)

#define _mm512_sqrt_ps(v1) \
    _mm512_maskz_sqrt_round_ps(_MM_K0_REG, (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sqrt_round_ps(v1, e2) \
    _mm512_maskz_sqrt_round_ps(_MM_K0_REG, (v1), e2)
#define _mm512_mask_sqrt_ps(v1, k2, v2) \
    _mm512_mask_sqrt_round_ps(v1, k2, v2, _MM_FROUND_CUR_DIRECTION);
#define _mm512_maskz_sqrt_ps(k1, v1) \
    _mm512_maskz_sqrt_round_ps((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sqrt_pd(v1) \
    _mm512_maskz_sqrt_round_pd((__mmask8)_MM_K0_REG, (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_sqrt_round_pd(v1, e2) \
    _mm512_maskz_sqrt_round_pd((__mmask8)_MM_K0_REG, (v1), e2)
#define _mm512_mask_sqrt_pd(v1, k2, v2) \
    _mm512_mask_sqrt_round_pd(v1, k2, v2, _MM_FROUND_CUR_DIRECTION);
#define _mm512_maskz_sqrt_pd(k1, v1) \
    _mm512_maskz_sqrt_round_pd((k1), (v1), _MM_FROUND_CUR_DIRECTION)

#define _mm512_abs_ps(v1) \
    _mm512_maskz_abs_ps(_MM_K0_REG, (v1))
#define _mm512_abs_pd(v1) \
    _mm512_maskz_abs_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_max_ps(v1, v2) \
    _mm512_maskz_max_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_max_round_ps(v1, v2, e3) \
    _mm512_maskz_max_round_ps(_MM_K0_REG, (v1), (v2), e3)
#define _mm512_mask_max_ps(v1, k2, v3, v4) \
    _mm512_mask_max_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_max_ps(k1, v2, v3) \
    _mm512_maskz_max_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_max_pd(v1, v2) \
    _mm512_maskz_max_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_max_round_pd(v1, v2, e3) \
    _mm512_maskz_max_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), e3)
#define _mm512_mask_max_pd(v1, k2, v3, v4) \
    _mm512_mask_max_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_max_pd(k1, v2, v3) \
    _mm512_maskz_max_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)

#define _mm512_min_ps(v1, v2) \
    _mm512_maskz_min_round_ps(_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_min_round_ps(v1, v2, e3) \
    _mm512_maskz_min_round_ps(_MM_K0_REG, (v1), (v2), e3)
#define _mm512_mask_min_ps(v1, k2, v3, v4) \
    _mm512_mask_min_round_ps((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_min_ps(k1, v2, v3) \
    _mm512_maskz_min_round_ps((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_min_pd(v1, v2) \
    _mm512_maskz_min_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_min_round_pd(v1, v2, e3) \
    _mm512_maskz_min_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), e3)
#define _mm512_mask_min_pd(v1, k2, v3, v4) \
    _mm512_mask_min_round_pd((v1), (k2), (v3), (v4), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_min_pd(k1, v2, v3) \
    _mm512_maskz_min_round_pd((k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)

#define _mm512_rcp14_ps(v1) \
    _mm512_maskz_rcp14_ps(_MM_K0_REG, v1);
#define _mm512_rcp14_pd(v1) \
    _mm512_maskz_rcp14_pd((__mmask8)_MM_K0_REG, v1);

#define _mm512_rsqrt14_ps(v1) \
    _mm512_maskz_rsqrt14_ps(_MM_K0_REG, v1);
#define _mm512_rsqrt14_pd(v1) \
    _mm512_maskz_rsqrt14_pd((__mmask8)_MM_K0_REG, v1);

#define _mm512_cvt_roundps_pd(v1, e2) \
    _mm512_maskz_cvt_roundps_pd((__mmask8)_MM_K0_REG, (v1), e2)
#define _mm512_cvtps_pd(v1) \
    _mm512_cvt_roundps_pd((v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtps_pd(k1, v2) \
    _mm512_maskz_cvt_roundps_pd((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtps_pd(v1, k2, v3) \
    _mm512_mask_cvt_roundps_pd((v1), (k2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvt_roundpd_ps(v1, e2) \
    _mm512_maskz_cvt_roundpd_ps((__mmask8)_MM_K0_REG, (v1), e2)
#define _mm512_cvtpd_ps(v1) \
    _mm512_cvt_roundpd_ps((v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtpd_ps(k1, v2) \
    _mm512_maskz_cvt_roundpd_ps((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtpd_ps(v1_old, k1, v2) \
    _mm512_mask_cvt_roundpd_ps((v1_old), (k1), (v2), _MM_FROUND_CUR_DIRECTION)

#define _mm512_cmp_ps_mask(v1, v2, i3) \
    _mm512_mask_cmp_round_ps_mask(_MM_K0_REG, v1, v2, i3, _MM_FROUND_CUR_DIRECTION)
#define _mm512_cmp_round_ps_mask(v1, v2, i3, e4) \
    _mm512_mask_cmp_round_ps_mask(_MM_K0_REG, v1, v2, i3, e4)
#define _mm512_mask_cmp_ps_mask(k1, v2, v3, i4) \
    _mm512_mask_cmp_round_ps_mask(k1, v2, v3, i4, _MM_FROUND_CUR_DIRECTION)
#define _mm512_cmp_pd_mask(v1, v2, i3) \
    _mm512_mask_cmp_round_pd_mask((__mmask8)_MM_K0_REG, v1, v2, i3, _MM_FROUND_CUR_DIRECTION)
#define _mm512_cmp_round_pd_mask(v1, v2, i3, e4) \
    _mm512_mask_cmp_round_pd_mask((__mmask8)_MM_K0_REG, v1, v2, i3, e4)
#define _mm512_mask_cmp_pd_mask(k1, v2, v3, i4) \
    _mm512_mask_cmp_round_pd_mask(k1, v2, v3, i4, _MM_FROUND_CUR_DIRECTION)

#define _mm512_cmpeq_ps_mask(v1, v2) _mm512_cmp_ps_mask((v1), (v2), _CMP_EQ_OQ)
#define _mm512_mask_cmpeq_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_EQ_OQ)
#define _mm512_cmplt_ps_mask(v1, v2) _mm512_cmp_ps_mask((v1), (v2), _CMP_LT_OS)
#define _mm512_mask_cmplt_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_LT_OS)
#define _mm512_cmple_ps_mask(v1, v2) _mm512_cmp_ps_mask((v1), (v2), _CMP_LE_OS)
#define _mm512_mask_cmple_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_LE_OS)
#define _mm512_cmpunord_ps_mask(v1, v2) \
    _mm512_cmp_ps_mask((v1), (v2), _CMP_UNORD_Q)
#define _mm512_mask_cmpunord_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_UNORD_Q)
#define _mm512_cmpneq_ps_mask(v1, v2) \
    _mm512_cmp_ps_mask((v1), (v2), _CMP_NEQ_UQ)
#define _mm512_mask_cmpneq_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_NEQ_UQ)
#define _mm512_cmpnlt_ps_mask(v1, v2) \
    _mm512_cmp_ps_mask((v1), (v2), _CMP_NLT_US)
#define _mm512_mask_cmpnlt_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_NLT_US)
#define _mm512_cmpnle_ps_mask(v1, v2) \
    _mm512_cmp_ps_mask((v1), (v2), _CMP_NLE_US)
#define _mm512_mask_cmpnle_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_NLE_US)
#define _mm512_cmpord_ps_mask(v1, v2) \
    _mm512_cmp_ps_mask((v1), (v2), _CMP_ORD_Q)
#define _mm512_mask_cmpord_ps_mask(k1, v1, v2) \
    _mm512_mask_cmp_ps_mask((k1), (v1), (v2), _CMP_ORD_Q)

#define _mm512_cmpeq_pd_mask(v1, v2) _mm512_cmp_pd_mask((v1), (v2), _CMP_EQ_OQ)
#define _mm512_mask_cmpeq_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_EQ_OQ)
#define _mm512_cmplt_pd_mask(v1, v2) _mm512_cmp_pd_mask((v1), (v2), _CMP_LT_OS)
#define _mm512_mask_cmplt_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_LT_OS)
#define _mm512_cmple_pd_mask(v1, v2) _mm512_cmp_pd_mask((v1), (v2), _CMP_LE_OS)
#define _mm512_mask_cmple_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_LE_OS)
#define _mm512_cmpunord_pd_mask(v1, v2) \
    _mm512_cmp_pd_mask((v1), (v2), _CMP_UNORD_Q)
#define _mm512_mask_cmpunord_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_UNORD_Q)
#define _mm512_cmpneq_pd_mask(v1, v2) \
    _mm512_cmp_pd_mask((v1), (v2), _CMP_NEQ_UQ)
#define _mm512_mask_cmpneq_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_NEQ_UQ)
#define _mm512_cmpnlt_pd_mask(v1, v2) \
    _mm512_cmp_pd_mask((v1), (v2), _CMP_NLT_US)
#define _mm512_mask_cmpnlt_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_NLT_US)
#define _mm512_cmpnle_pd_mask(v1, v2) \
    _mm512_cmp_pd_mask((v1), (v2), _CMP_NLE_US)
#define _mm512_mask_cmpnle_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_NLE_US)
#define _mm512_cmpord_pd_mask(v1, v2) \
    _mm512_cmp_pd_mask((v1), (v2), _CMP_ORD_Q)
#define _mm512_mask_cmpord_pd_mask(k1, v1, v2) \
    _mm512_mask_cmp_pd_mask((k1), (v1), (v2), _CMP_ORD_Q)

#define _mm512_broadcast_f32x2(v1) \
    _mm512_maskz_broadcast_f32x2(_MM_K0_REG, (v1))
#define _mm512_broadcast_f32x4(v1) \
    _mm512_maskz_broadcast_f32x4(_MM_K0_REG, (v1))
#define _mm512_broadcast_f32x8(v1) \
    _mm512_maskz_broadcast_f32x8(_MM_K0_REG, (v1))
#define _mm512_broadcast_f64x2(v1) \
    _mm512_maskz_broadcast_f64x2((__mmask8)_MM_K0_REG, (v1))
#define _mm512_broadcast_f64x4(v1) \
    _mm512_maskz_broadcast_f64x4((__mmask8)_MM_K0_REG, (v1))
#define _mm512_broadcastsd_pd(v1) \
    _mm512_maskz_broadcastsd_pd((__mmask8)_MM_K0_REG, (v1))
#define _mm512_broadcastss_ps(v1) \
    _mm512_maskz_broadcastss_ps(_MM_K0_REG, (v1))

#define _mm512_extractf32x4_ps(v1, e2) \
    _mm512_maskz_extractf32x4_ps((__mmask8)_MM_K0_REG, (v1), (e2))
#define _mm512_extractf32x8_ps(v1, e2) \
    _mm512_maskz_extractf32x8_ps((__mmask8)_MM_K0_REG, (v1), (e2))
#define _mm512_extractf64x2_pd(v1, e2) \
    _mm512_maskz_extractf64x2_pd((__mmask8)_MM_K0_REG, (v1), (e2))
#define _mm512_extractf64x4_pd(v1, e2) \
    _mm512_maskz_extractf64x4_pd((__mmask8)_MM_K0_REG, (v1), (e2))

#define _mm512_insertf32x4(v1, v2, e3) \
    _mm512_maskz_insertf32x4(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_insertf32x8(v1, v2, e3) \
    _mm512_maskz_insertf32x8(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_insertf64x2(v1, v2, e3) \
    _mm512_maskz_insertf64x2((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_insertf64x4(v1, v2, e3) \
    _mm512_maskz_insertf64x4((__mmask8)_MM_K0_REG, (v1), (v2), (e3))

#define _mm512_shuffle_f32x4(v1, v2, e3) \
    _mm512_maskz_shuffle_f32x4(_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_shuffle_f64x2(v1, v2, e3) \
    _mm512_maskz_shuffle_f64x2((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_shuffle_pd(v1, v2, e3) \
    _mm512_maskz_shuffle_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e3))
#define _mm512_shuffle_ps(v1, v2, e3) \
    _mm512_maskz_shuffle_ps(_MM_K0_REG, (v1), (v2), (e3))

#define _mm512_kand(k1, k2) \
    ((__mmask16) ((k1) & (k2)))
#define _mm512_kandn(k1, k2) \
    ((__mmask16) (~(k1) & (k2)))
#define _mm512_kor(k1, k2) \
    ((__mmask16) ((k1) | (k2)))
#define _mm512_kxor(k1, k2) \
    ((__mmask16) ((k1) ^ (k2)))
#define _mm512_kxnor(k1, k2) \
    ((__mmask16) (~((k1) ^ (k2))))
#define _mm512_knot(k1) \
    ((__mmask16) (~(k1)))

extern __m512i __cdecl _mm512_setzero_si512(void);

extern __m512i __cdecl _mm512_set_epi8(char /* e63 */, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char, char /* e0 */);
extern __m512i __cdecl _mm512_set_epi16(short /* e31 */, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short, short /* e0 */);
extern __m512i __cdecl _mm512_set_epi32(int /* e15 */, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int /* e0 */);
extern __m512i __cdecl _mm512_set_epi64(__int64 /* e7 */, __int64, __int64, __int64, __int64, __int64, __int64, __int64 /* e0 */);

extern __m512i __cdecl _mm512_mask_set1_epi8(__m512i, __mmask64, char);
extern __m512i __cdecl _mm512_maskz_set1_epi8(__mmask64, char);
extern __m512i __cdecl _mm512_mask_set1_epi16(__m512i, __mmask32, short);
extern __m512i __cdecl _mm512_maskz_set1_epi16(__mmask32, short);
extern __m512i __cdecl _mm512_mask_set1_epi32(__m512i, __mmask16, int);
extern __m512i __cdecl _mm512_maskz_set1_epi32(__mmask16, int);
extern __m512i __cdecl _mm512_mask_set1_epi64(__m512i, __mmask8, __int64);
extern __m512i __cdecl _mm512_maskz_set1_epi64(__mmask8, __int64);

extern __m512i __cdecl _mm512_mask_add_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_add_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_add_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_add_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_add_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_add_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_add_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_add_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_adds_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_adds_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_adds_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_adds_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_adds_epu8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_adds_epu8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_adds_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_adds_epu16(__mmask32, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_abs_epi8(__m512i, __mmask64, __m512i);
extern __m512i __cdecl _mm512_maskz_abs_epi8(__mmask64, __m512i);
extern __m512i __cdecl _mm512_mask_abs_epi16(__m512i, __mmask32, __m512i);
extern __m512i __cdecl _mm512_maskz_abs_epi16(__mmask32, __m512i);
extern __m512i __cdecl _mm512_mask_abs_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_abs_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_abs_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_abs_epi64(__mmask8, __m512i);

extern __m512i  __cdecl _mm512_mask_broadcast_i32x2(__m512i, __mmask16, __m128i);
extern __m512i  __cdecl _mm512_maskz_broadcast_i32x2(__mmask16, __m128i);
extern __m512i  __cdecl _mm512_mask_broadcast_i32x4(__m512i, __mmask16, __m128i);
extern __m512i  __cdecl _mm512_maskz_broadcast_i32x4(__mmask16, __m128i);
extern __m512i  __cdecl _mm512_mask_broadcast_i32x8(__m512i, __mmask16, __m256i);
extern __m512i  __cdecl _mm512_maskz_broadcast_i32x8(__mmask16, __m256i);
extern __m512i  __cdecl _mm512_mask_broadcast_i64x2(__m512i, __mmask8, __m128i);
extern __m512i  __cdecl _mm512_maskz_broadcast_i64x2(__mmask8, __m128i);
extern __m512i  __cdecl _mm512_mask_broadcast_i64x4(__m512i, __mmask8, __m256i);
extern __m512i  __cdecl _mm512_maskz_broadcast_i64x4(__mmask8, __m256i);
extern __m512i __cdecl _mm512_mask_broadcastb_epi8(__m512i, __mmask64, __m128i);
extern __m512i __cdecl _mm512_maskz_broadcastb_epi8(__mmask64, __m128i);
extern __m512i __cdecl _mm512_mask_broadcastw_epi16(__m512i, __mmask32, __m128i);
extern __m512i __cdecl _mm512_maskz_broadcastw_epi16(__mmask32, __m128i);
extern __m512i __cdecl _mm512_mask_broadcastd_epi32(__m512i, __mmask16, __m128i);
extern __m512i __cdecl _mm512_maskz_broadcastd_epi32(__mmask16, __m128i);
extern __m512i __cdecl _mm512_mask_broadcastq_epi64(__m512i, __mmask8, __m128i);
extern __m512i __cdecl _mm512_maskz_broadcastq_epi64(__mmask8, __m128i);
extern __m512i __cdecl _mm512_broadcastmw_epi32(__mmask16);
extern __m512i __cdecl _mm512_broadcastmb_epi64(__mmask8);

extern __m512i __cdecl _mm512_mask_sub_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sub_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_sub_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sub_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_sub_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sub_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_sub_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sub_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_subs_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_subs_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_subs_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_subs_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_subs_epu8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_subs_epu8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_subs_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_subs_epu16(__mmask32, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_max_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epu8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epu8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epu16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epu32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epu32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_max_epu64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_max_epu64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_min_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epu8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epu8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epu16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epu32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epu32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_min_epu64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_min_epu64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_mul_epi32(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mul_epi32(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mul_epu32(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mul_epu32(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mulhi_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mulhi_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mulhi_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mulhi_epu16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mullo_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mullo_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mullo_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mullo_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mullo_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mullo_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mullox_epi64(__m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mullox_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_mulhrs_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_mulhrs_epi16(__mmask32, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_load_epi32(__m512i, __mmask16, void const*);
extern __m512i __cdecl _mm512_maskz_load_epi32(__mmask16, void const*);
extern __m512i __cdecl _mm512_mask_load_epi64(__m512i, __mmask8, void const*);
extern __m512i __cdecl _mm512_maskz_load_epi64(__mmask8, void const*);
extern __m512i __cdecl _mm512_mask_loadu_epi8(__m512i, __mmask64, void const*);
extern __m512i __cdecl _mm512_maskz_loadu_epi8(__mmask64, void const*);
extern __m512i __cdecl _mm512_mask_loadu_epi16(__m512i, __mmask32, void const*);
extern __m512i __cdecl _mm512_maskz_loadu_epi16(__mmask32, void const*);
extern __m512i __cdecl _mm512_mask_loadu_epi32(__m512i, __mmask16, void const*);
extern __m512i __cdecl _mm512_maskz_loadu_epi32(__mmask16, void const*);
extern __m512i __cdecl _mm512_mask_loadu_epi64(__m512i, __mmask8, void const*);
extern __m512i __cdecl _mm512_maskz_loadu_epi64(__mmask8, void const*);

extern void    __cdecl _mm512_mask_store_epi32(void*, __mmask16, __m512i);
extern void    __cdecl _mm512_mask_store_epi64(void*, __mmask8, __m512i);
extern void    __cdecl _mm512_mask_storeu_epi8(void*, __mmask64, __m512i);
extern void    __cdecl _mm512_mask_storeu_epi16(void*, __mmask32, __m512i);
extern void    __cdecl _mm512_mask_storeu_epi32(void*, __mmask16, __m512i);
extern void    __cdecl _mm512_mask_storeu_epi64(void*, __mmask8, __m512i);

extern __m128i __cdecl _mm512_mask_extracti32x4_epi32(__m128i, __mmask8, __m512i, int);
extern __m128i __cdecl _mm512_maskz_extracti32x4_epi32(__mmask8, __m512i, int);
extern __m256i __cdecl _mm512_mask_extracti32x8_epi32(__m256i, __mmask8, __m512i, int);
extern __m256i __cdecl _mm512_maskz_extracti32x8_epi32(__mmask8, __m512i, int);
extern __m128i __cdecl _mm512_mask_extracti64x2_epi64(__m128i, __mmask8, __m512i, int);
extern __m128i __cdecl _mm512_maskz_extracti64x2_epi64(__mmask8, __m512i, int);
extern __m256i __cdecl _mm512_mask_extracti64x4_epi64(__m256i, __mmask8, __m512i, int);
extern __m256i __cdecl _mm512_maskz_extracti64x4_epi64(__mmask8, __m512i, int);

extern __m512i __cdecl _mm512_mask_inserti32x4(__m512i, __mmask16, __m512i, __m128i, int);
extern __m512i __cdecl _mm512_maskz_inserti32x4(__mmask16, __m512i, __m128i, int);
extern __m512i __cdecl _mm512_mask_inserti32x8(__m512i, __mmask16, __m512i, __m256i, int);
extern __m512i __cdecl _mm512_maskz_inserti32x8(__mmask16, __m512i, __m256i, int);
extern __m512i __cdecl _mm512_mask_inserti64x2(__m512i, __mmask8, __m512i, __m128i, int);
extern __m512i __cdecl _mm512_maskz_inserti64x2(__mmask8, __m512i, __m128i, int);
extern __m512i __cdecl _mm512_mask_inserti64x4(__m512i, __mmask8, __m512i, __m256i, int);
extern __m512i __cdecl _mm512_maskz_inserti64x4(__mmask8, __m512i, __m256i, int);

extern __m512i __cdecl _mm512_mask_shuffle_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_shuffle_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_shuffle_epi32(__m512i, __mmask16, __m512i, int);
extern __m512i __cdecl _mm512_maskz_shuffle_epi32(__mmask16, __m512i, int);
extern __m512i __cdecl _mm512_mask_shuffle_i32x4(__m512i, __mmask16, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_shuffle_i32x4(__mmask16, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_mask_shuffle_i64x2(__m512i, __mmask8, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_shuffle_i64x2(__mmask8, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_mask_shufflehi_epi16(__m512i, __mmask32, __m512i, int);
extern __m512i __cdecl _mm512_maskz_shufflehi_epi16(__mmask32, __m512i, int);
extern __m512i __cdecl _mm512_mask_shufflelo_epi16(__m512i, __mmask32, __m512i, int);
extern __m512i __cdecl _mm512_maskz_shufflelo_epi16(__mmask32, __m512i, int);

extern __m512  __cdecl _mm512_mask_mov_ps(__m512, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_mov_ps(__mmask16, __m512);
extern __m512d __cdecl _mm512_mask_mov_pd(__m512d, __mmask8, __m512d);
extern __m512d __cdecl _mm512_maskz_mov_pd(__mmask8, __m512d);
extern __m512i __cdecl _mm512_mask_mov_epi8(__m512i, __mmask64, __m512i);
extern __m512i __cdecl _mm512_maskz_mov_epi8(__mmask64, __m512i);
extern __m512i __cdecl _mm512_mask_mov_epi16(__m512i, __mmask32, __m512i);
extern __m512i __cdecl _mm512_maskz_mov_epi16(__mmask32, __m512i);
extern __m512i __cdecl _mm512_mask_mov_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_mov_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_mov_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_mov_epi64(__mmask8, __m512i);
extern __m512d __cdecl _mm512_mask_movedup_pd(__m512d, __mmask8, __m512d);
extern __m512d __cdecl _mm512_maskz_movedup_pd(__mmask8, __m512d);
extern __m512  __cdecl _mm512_mask_movehdup_ps(__m512, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_movehdup_ps(__mmask16, __m512);
extern __m512  __cdecl _mm512_mask_moveldup_ps(__m512, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_moveldup_ps(__mmask16, __m512);

extern __m512i __cdecl _mm512_movm_epi8(__mmask64);
extern __m512i __cdecl _mm512_movm_epi16(__mmask32);
extern __m512i __cdecl _mm512_movm_epi32(__mmask16);
extern __m512i __cdecl _mm512_movm_epi64(__mmask8);
extern __mmask64 __cdecl _mm512_movepi8_mask(__m512i);
extern __mmask32 __cdecl _mm512_movepi16_mask(__m512i);
extern __mmask16 __cdecl _mm512_movepi32_mask(__m512i);
extern __mmask8  __cdecl _mm512_movepi64_mask(__m512i);

extern __m512i __cdecl _mm512_mask_alignr_epi8(__m512i, __mmask64, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_alignr_epi8(__mmask64, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_mask_alignr_epi32(__m512i, __mmask16, __m512i, __m512i, const int /* count */);
extern __m512i __cdecl _mm512_maskz_alignr_epi32(__mmask16, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_mask_alignr_epi64(__m512i, __mmask8, __m512i, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_alignr_epi64(__mmask8, __m512i, __m512i, const int);

extern __m512d __cdecl _mm512_mask_and_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_and_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_and_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_and_ps(__mmask16, __m512, __m512);
extern __m512i __cdecl _mm512_mask_and_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_and_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_and_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_and_epi64(__mmask8, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_andnot_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_andnot_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_andnot_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_andnot_ps(__mmask16, __m512, __m512);
extern __m512i __cdecl _mm512_mask_andnot_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_andnot_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_andnot_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_andnot_epi64(__mmask8, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_or_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_or_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_or_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_or_ps(__mmask16, __m512, __m512);
extern __m512i __cdecl _mm512_mask_or_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_or_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_or_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_or_epi64(__mmask8, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_xor_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_xor_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_xor_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_xor_ps(__mmask16, __m512, __m512);
extern __m512i __cdecl _mm512_mask_xor_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_xor_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_xor_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_xor_epi64(__mmask8, __m512i, __m512i);

extern __m512  __cdecl _mm512_mask_blend_ps(__mmask16, __m512, __m512);
extern __m512d __cdecl _mm512_mask_blend_pd(__mmask8, __m512d, __m512d);
extern __m512i __cdecl _mm512_mask_blend_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_blend_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_blend_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_blend_epi64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_sll_epi16(__m512i, __mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sll_epi16(__mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_sll_epi32(__m512i, __mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sll_epi32(__mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_sll_epi64(__m512i, __mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sll_epi64(__mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_slli_epi16(__m512i, __mmask32, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_slli_epi16(__mmask32, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_slli_epi32(__m512i, __mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_slli_epi32(__mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_slli_epi64(__m512i, __mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_slli_epi64(__mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_sllv_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sllv_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_sllv_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sllv_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_sllv_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_sllv_epi64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_srl_epi16(__m512i, __mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_srl_epi16(__mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_srl_epi32(__m512i, __mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_srl_epi32(__mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_srl_epi64(__m512i, __mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_srl_epi64(__mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_srli_epi16(__m512i, __mmask32, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srli_epi16(__mmask32, __m512i, int);
extern __m512i __cdecl _mm512_mask_srli_epi32(__m512i, __mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srli_epi32(__mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_srli_epi64(__m512i, __mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srli_epi64(__mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_srlv_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srlv_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_srlv_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srlv_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_srlv_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srlv_epi64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_sra_epi16(__m512i, __mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sra_epi16(__mmask32, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_sra_epi32(__m512i, __mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sra_epi32(__mmask16, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_sra_epi64(__m512i, __mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_maskz_sra_epi64(__mmask8, __m512i, __m128i);
extern __m512i __cdecl _mm512_mask_srai_epi16(__m512i, __mmask32, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srai_epi16(__mmask32, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_srai_epi32(__m512i, __mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srai_epi32(__mmask16, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_srai_epi64(__m512i, __mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_maskz_srai_epi64(__mmask8, __m512i, unsigned int);
extern __m512i __cdecl _mm512_mask_srav_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srav_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_srav_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srav_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_srav_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_srav_epi64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_bslli_epi128(__m512i, int);
extern __m512i __cdecl _mm512_bsrli_epi128(__m512i, int);

extern __m512i __cdecl _mm512_mask_rol_epi32(__m512i, __mmask16, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_rol_epi32(__mmask16, __m512i, const int);
extern __m512i __cdecl _mm512_mask_rol_epi64(__m512i, __mmask8, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_rol_epi64(__mmask8, __m512i, const int);
extern __m512i __cdecl _mm512_mask_rolv_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_rolv_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_rolv_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_rolv_epi64(__mmask8, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_ror_epi32(__m512i, __mmask16, __m512i, int);
extern __m512i __cdecl _mm512_maskz_ror_epi32(__mmask16, __m512i, int);
extern __m512i __cdecl _mm512_mask_ror_epi64(__m512i, __mmask8, __m512i, int);
extern __m512i __cdecl _mm512_maskz_ror_epi64(__mmask8, __m512i, int);
extern __m512i __cdecl _mm512_mask_rorv_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_rorv_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_rorv_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_rorv_epi64(__mmask8, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_unpackhi_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_unpackhi_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_unpackhi_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_unpackhi_ps(__mmask16, __m512, __m512);
extern __m512d __cdecl _mm512_mask_unpacklo_pd(__m512d, __mmask8, __m512d, __m512d);
extern __m512d __cdecl _mm512_maskz_unpacklo_pd(__mmask8, __m512d, __m512d);
extern __m512  __cdecl _mm512_mask_unpacklo_ps(__m512, __mmask16, __m512, __m512);
extern __m512  __cdecl _mm512_maskz_unpacklo_ps(__mmask16, __m512, __m512);
extern __m512i __cdecl _mm512_mask_unpackhi_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpackhi_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpackhi_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpackhi_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpackhi_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpackhi_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpackhi_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpackhi_epi64(__mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpacklo_epi8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpacklo_epi8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpacklo_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpacklo_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpacklo_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpacklo_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_unpacklo_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_unpacklo_epi64(__mmask8, __m512i, __m512i);

extern __m512  __cdecl _mm512_mask_getexp_round_ps(__m512, __mmask16, __m512, int);
extern __m512  __cdecl _mm512_maskz_getexp_round_ps(__mmask16, __m512, int);
extern __m512d __cdecl _mm512_mask_getexp_round_pd(__m512d, __mmask8, __m512d, int);
extern __m512d __cdecl _mm512_maskz_getexp_round_pd(__mmask8, __m512d, int);

extern __m512  __cdecl _mm512_mask_getmant_round_ps(__m512, __mmask16, __m512, int, int, int);
extern __m512  __cdecl _mm512_maskz_getmant_round_ps(__mmask16, __m512, int, int, int);
extern __m512d __cdecl _mm512_mask_getmant_round_pd(__m512d, __mmask8, __m512d, int, int, int);
extern __m512d __cdecl _mm512_maskz_getmant_round_pd(__mmask8, __m512d, int, int, int);

extern __m512d __cdecl _mm512_mask_permute_pd(__m512d, __mmask8, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_permute_pd(__mmask8, __m512d, const int);
extern __m512  __cdecl _mm512_mask_permute_ps(__m512, __mmask16, __m512, const int);
extern __m512  __cdecl _mm512_maskz_permute_ps(__mmask16, __m512, const int);
extern __m512d __cdecl _mm512_mask_permutevar_pd(__m512d, __mmask8, __m512d, __m512i);
extern __m512d __cdecl _mm512_maskz_permutevar_pd(__mmask8, __m512d, __m512i);
extern __m512  __cdecl _mm512_mask_permutevar_ps(__m512, __mmask16, __m512, __m512i);
extern __m512  __cdecl _mm512_maskz_permutevar_ps(__mmask16, __m512, __m512i);
extern __m512i __cdecl _mm512_permutevar_epi32(__m512i, __m512i);
extern __m512i __cdecl _mm512_mask_permutevar_epi32(__m512i, __mmask16, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_permutex_pd(__m512d, __mmask8, __m512d, const int);
extern __m512d __cdecl _mm512_maskz_permutex_pd(__mmask8, __m512d, const int);
extern __m512i __cdecl _mm512_mask_permutex_epi64(__m512i, __mmask8, __m512i, const int);
extern __m512i __cdecl _mm512_maskz_permutex_epi64(__mmask8, __m512i, const int);
extern __m512d __cdecl _mm512_mask_permutexvar_pd(__m512d, __mmask8, __m512i, __m512d);
extern __m512d __cdecl _mm512_maskz_permutexvar_pd(__mmask8, __m512i, __m512d);
extern __m512  __cdecl _mm512_mask_permutexvar_ps(__m512, __mmask16, __m512i, __m512);
extern __m512  __cdecl _mm512_maskz_permutexvar_ps(__mmask16, __m512i, __m512);
extern __m512i __cdecl _mm512_mask_permutexvar_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_permutexvar_epi16(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_permutexvar_epi32(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_permutexvar_epi32(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_permutexvar_epi64(__m512i, __mmask8, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_permutexvar_epi64(__mmask8, __m512i, __m512i);

extern __m512d __cdecl _mm512_mask_permutex2var_pd(__m512d, __mmask8, __m512i /* index */, __m512d);
extern __m512d __cdecl _mm512_mask2_permutex2var_pd(__m512d, __m512i /* index */, __mmask8, __m512d);
extern __m512d __cdecl _mm512_maskz_permutex2var_pd(__mmask8, __m512d, __m512i /* index */, __m512d);
extern __m512  __cdecl _mm512_mask_permutex2var_ps(__m512, __mmask16, __m512i /* index */, __m512);
extern __m512  __cdecl _mm512_mask2_permutex2var_ps(__m512, __m512i /* index */, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_permutex2var_ps(__mmask16, __m512, __m512i /* index */, __m512);
extern __m512i __cdecl _mm512_mask_permutex2var_epi16(__m512i, __mmask32, __m512i /* idx */, __m512i);
extern __m512i __cdecl _mm512_mask2_permutex2var_epi16(__m512i, __m512i /* idx */, __mmask32, __m512i);
extern __m512i __cdecl _mm512_maskz_permutex2var_epi16(__mmask32, __m512i, __m512i /* idx */, __m512i);
extern __m512i __cdecl _mm512_mask_permutex2var_epi32(__m512i, __mmask16, __m512i /* idx */, __m512i);
extern __m512i __cdecl _mm512_mask2_permutex2var_epi32(__m512i, __m512i /* idx */, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_permutex2var_epi32(__mmask16, __m512i, __m512i /* idx */, __m512i);
extern __m512i __cdecl _mm512_mask_permutex2var_epi64(__m512i, __mmask8, __m512i /* idx */, __m512i);
extern __m512i __cdecl _mm512_mask2_permutex2var_epi64(__m512i, __m512i /* idx */, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_permutex2var_epi64(__mmask8, __m512i, __m512i /* idx */, __m512i);

extern __m512d __cdecl _mm512_mask_compress_pd(__m512d, __mmask8, __m512d);
extern __m512d __cdecl _mm512_maskz_compress_pd(__mmask8, __m512d);
extern __m512  __cdecl _mm512_mask_compress_ps(__m512, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_compress_ps(__mmask16, __m512);
extern __m512i __cdecl _mm512_mask_compress_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_compress_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_compress_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_compress_epi64(__mmask8, __m512i);

extern void    __cdecl _mm512_mask_compressstoreu_pd(void*, __mmask8, __m512d);
extern void    __cdecl _mm512_mask_compressstoreu_ps(void*, __mmask16, __m512);
extern void    __cdecl _mm512_mask_compressstoreu_epi32(void*, __mmask16, __m512i);
extern void    __cdecl _mm512_mask_compressstoreu_epi64(void*, __mmask8, __m512i);

extern __m512d __cdecl _mm512_mask_expand_pd(__m512d, __mmask8, __m512d);
extern __m512d __cdecl _mm512_maskz_expand_pd(__mmask8, __m512d);
extern __m512  __cdecl _mm512_mask_expand_ps(__m512, __mmask16, __m512);
extern __m512  __cdecl _mm512_maskz_expand_ps(__mmask16, __m512);
extern __m512i __cdecl _mm512_mask_expand_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_expand_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_expand_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_expand_epi64(__mmask8, __m512i);
extern __m512d __cdecl _mm512_mask_expandloadu_pd(__m512d, __mmask8, void const*);
extern __m512d __cdecl _mm512_maskz_expandloadu_pd(__mmask8, void const*);
extern __m512  __cdecl _mm512_mask_expandloadu_ps(__m512, __mmask16, void const*);
extern __m512  __cdecl _mm512_maskz_expandloadu_ps(__mmask16, void const*);
extern __m512i __cdecl _mm512_mask_expandloadu_epi32(__m512i, __mmask16, void const*);
extern __m512i __cdecl _mm512_maskz_expandloadu_epi32(__mmask16, void const*);
extern __m512i __cdecl _mm512_mask_expandloadu_epi64(__m512i, __mmask8, void const*);
extern __m512i __cdecl _mm512_maskz_expandloadu_epi64(__mmask8, void const*);

extern __m512i __cdecl _mm512_mask_ternarylogic_epi32(__m512i, __mmask16, __m512i, __m512i, int);
extern __m512i __cdecl _mm512_maskz_ternarylogic_epi32(__mmask16, __m512i, __m512i, __m512i, int);
extern __m512i __cdecl _mm512_mask_ternarylogic_epi64(__m512i, __mmask8, __m512i, __m512i, int);
extern __m512i __cdecl _mm512_maskz_ternarylogic_epi64(__mmask8, __m512i, __m512i, __m512i, int);

extern __m512i __cdecl _mm512_mask_conflict_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_conflict_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_conflict_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_conflict_epi64(__mmask8, __m512i);

extern __m512i __cdecl _mm512_mask_lzcnt_epi32(__m512i, __mmask16, __m512i);
extern __m512i __cdecl _mm512_maskz_lzcnt_epi32(__mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_lzcnt_epi64(__m512i, __mmask8, __m512i);
extern __m512i __cdecl _mm512_maskz_lzcnt_epi64(__mmask8, __m512i);

extern __m512i __cdecl _mm512_mask_avg_epu8(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_avg_epu8(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_avg_epu16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_avg_epu16(__mmask32, __m512i, __m512i);

extern __m512i __cdecl _mm512_sad_epu8(__m512i, __m512i);
extern __m512i __cdecl _mm512_mask_dbsad_epu8(__m512i, __mmask32, __m512i, __m512i, int);
extern __m512i __cdecl _mm512_maskz_dbsad_epu8(__mmask32, __m512i, __m512i, int);

extern float   __cdecl _mm512_reduce_add_ps(__m512);
extern float   __cdecl _mm512_mask_reduce_add_ps(__mmask16, __m512);
extern double  __cdecl _mm512_reduce_add_pd(__m512d);
extern double  __cdecl _mm512_mask_reduce_add_pd(__mmask8, __m512d);
extern int     __cdecl _mm512_reduce_add_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_add_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_add_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_add_epi64(__mmask8, __m512i);

extern float   __cdecl _mm512_reduce_mul_ps(__m512);
extern float   __cdecl _mm512_mask_reduce_mul_ps(__mmask16, __m512);
extern double  __cdecl _mm512_reduce_mul_pd(__m512d);
extern double  __cdecl _mm512_mask_reduce_mul_pd(__mmask8, __m512d);
extern int     __cdecl _mm512_reduce_mul_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_mul_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_mul_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_mul_epi64(__mmask8, __m512i);

extern float   __cdecl _mm512_reduce_min_ps(__m512);
extern float   __cdecl _mm512_mask_reduce_min_ps(__mmask16, __m512);
extern double  __cdecl _mm512_reduce_min_pd(__m512d);
extern double  __cdecl _mm512_mask_reduce_min_pd(__mmask8, __m512d);
extern int     __cdecl _mm512_reduce_min_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_min_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_min_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_min_epi64(__mmask8, __m512i);
extern unsigned int     __cdecl _mm512_reduce_min_epu32(__m512i);
extern unsigned int     __cdecl _mm512_mask_reduce_min_epu32(__mmask16, __m512i);
extern unsigned __int64 __cdecl _mm512_reduce_min_epu64(__m512i);
extern unsigned __int64 __cdecl _mm512_mask_reduce_min_epu64(__mmask8, __m512i);

extern float   __cdecl _mm512_reduce_max_ps(__m512);
extern float   __cdecl _mm512_mask_reduce_max_ps(__mmask16, __m512);
extern double  __cdecl _mm512_reduce_max_pd(__m512d);
extern double  __cdecl _mm512_mask_reduce_max_pd(__mmask8, __m512d);
extern int     __cdecl _mm512_reduce_max_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_max_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_max_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_max_epi64(__mmask8, __m512i);
extern unsigned int     __cdecl _mm512_reduce_max_epu32(__m512i);
extern unsigned int     __cdecl _mm512_mask_reduce_max_epu32(__mmask16, __m512i);
extern unsigned __int64 __cdecl _mm512_reduce_max_epu64(__m512i);
extern unsigned __int64 __cdecl _mm512_mask_reduce_max_epu64(__mmask8, __m512i);

extern int     __cdecl _mm512_reduce_and_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_and_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_and_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_and_epi64(__mmask8, __m512i);

extern int     __cdecl _mm512_reduce_or_epi32(__m512i);
extern int     __cdecl _mm512_mask_reduce_or_epi32(__mmask16, __m512i);
extern __int64 __cdecl _mm512_reduce_or_epi64(__m512i);
extern __int64 __cdecl _mm512_mask_reduce_or_epi64(__mmask8, __m512i);

extern __m512d __cdecl _mm512_mask_reduce_round_pd(__m512d, __mmask8, __m512d, int, int);
extern __m512d __cdecl _mm512_maskz_reduce_round_pd(__mmask8, __m512d, int, int);
extern __m512  __cdecl _mm512_mask_reduce_round_ps(__m512, __mmask16, __m512, int, int);
extern __m512  __cdecl _mm512_maskz_reduce_round_ps(__mmask16, __m512, int, int);

extern __m512d __cdecl _mm512_mask_roundscale_round_pd(__m512d, __mmask8, __m512d, int, int);
extern __m512d __cdecl _mm512_maskz_roundscale_round_pd(__mmask8, __m512d, int, int);
extern __m512  __cdecl _mm512_mask_roundscale_round_ps(__m512, __mmask16, __m512, int, int);
extern __m512  __cdecl _mm512_maskz_roundscale_round_ps(__mmask16, __m512, int, int);

extern __m512d __cdecl _mm512_mask_scalef_round_pd(__m512d, __mmask8, __m512d, __m512d, int);
extern __m512d __cdecl _mm512_maskz_scalef_round_pd(__mmask8, __m512d, __m512d, int);
extern __m512  __cdecl _mm512_mask_scalef_round_ps(__m512, __mmask16, __m512, __m512, int);
extern __m512  __cdecl _mm512_maskz_scalef_round_ps(__mmask16, __m512, __m512, int);

extern __m512d __cdecl _mm512_mask_fixupimm_round_pd(__m512d, __mmask8, __m512d, __m512i, const int, const int);
extern __m512d __cdecl _mm512_maskz_fixupimm_round_pd(__mmask8, __m512d, __m512d, __m512i, const int, const int);
extern __m512  __cdecl _mm512_mask_fixupimm_round_ps(__m512, __mmask16, __m512, __m512i, const int, const int);
extern __m512  __cdecl _mm512_maskz_fixupimm_round_ps(__mmask16, __m512, __m512, __m512i, const int, const int);

extern void    __cdecl _mm512_stream_pd(void*, __m512d);
extern void    __cdecl _mm512_stream_ps(void*, __m512);
extern void    __cdecl _mm512_stream_si512(void*, __m512i);
extern __m512i __cdecl _mm512_stream_load_si512(void const*);

extern __m128d __cdecl _mm512_castpd512_pd128(__m512d);
extern __m128  __cdecl _mm512_castps512_ps128(__m512);
extern __m128i __cdecl _mm512_castsi512_si128(__m512i);
extern __m512i __cdecl _mm512_castsi128_si512(__m128i);

extern __m512  __cdecl _mm512_mask_exp2a23_round_ps(__m512, __mmask16, __m512, int);
extern __m512  __cdecl _mm512_maskz_exp2a23_round_ps(__mmask16, __m512, int);
extern __m512d __cdecl _mm512_mask_exp2a23_round_pd(__m512d, __mmask8, __m512d, int);
extern __m512d __cdecl _mm512_maskz_exp2a23_round_pd(__mmask8, __m512d, int);

extern __mmask16 __cdecl _mm512_mask_fpclass_ps_mask(__mmask16, __m512, int);
extern __mmask8  __cdecl _mm512_mask_fpclass_pd_mask(__mmask8, __m512d, int);

extern __m512d __cdecl _mm512_mask_range_round_pd(__m512d, __mmask8, __m512d, __m512d, int, int);
extern __m512d __cdecl _mm512_maskz_range_round_pd(__mmask8, __m512d, __m512d, int, int);
extern __m512  __cdecl _mm512_mask_range_round_ps(__m512, __mmask16, __m512, __m512, int, int);
extern __m512  __cdecl _mm512_maskz_range_round_ps(__mmask16, __m512, __m512, int, int);

extern __m512i __cdecl _mm512_mask_madd_epi16(__m512i, __mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_madd_epi16(__mmask16, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_maddubs_epi16(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_maddubs_epi16(__mmask32, __m512i, __m512i);

extern __m512i __cdecl _mm512_mask_packs_epi16(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_packs_epi16(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_packs_epi32(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_packs_epi32(__mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_packus_epi16(__m512i, __mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_packus_epi16(__mmask64, __m512i, __m512i);
extern __m512i __cdecl _mm512_mask_packus_epi32(__m512i, __mmask32, __m512i, __m512i);
extern __m512i __cdecl _mm512_maskz_packus_epi32(__mmask32, __m512i, __m512i);

extern __mmask64 __cdecl _mm512_mask_cmp_epi8_mask(__mmask64, __m512i, __m512i, const int);
extern __mmask32 __cdecl _mm512_mask_cmp_epi16_mask(__mmask32, __m512i, __m512i, const int);
extern __mmask16 __cdecl _mm512_mask_cmp_epi32_mask(__mmask16, __m512i, __m512i, const int);
extern __mmask8  __cdecl _mm512_mask_cmp_epi64_mask(__mmask8, __m512i, __m512i, const int);
extern __mmask64 __cdecl _mm512_mask_cmp_epu8_mask(__mmask64, __m512i, __m512i, const int);
extern __mmask32 __cdecl _mm512_mask_cmp_epu16_mask(__mmask32, __m512i, __m512i, const int);
extern __mmask16 __cdecl _mm512_mask_cmp_epu32_mask(__mmask16, __m512i, __m512i, const int);
extern __mmask8  __cdecl _mm512_mask_cmp_epu64_mask(__mmask8, __m512i, __m512i, const int);

extern __mmask64 __cdecl _mm512_mask_test_epi8_mask(__mmask64, __m512i, __m512i);
extern __mmask32 __cdecl _mm512_mask_test_epi16_mask(__mmask32, __m512i, __m512i);
extern __mmask64 __cdecl _mm512_mask_testn_epi8_mask(__mmask64, __m512i, __m512i);
extern __mmask32 __cdecl _mm512_mask_testn_epi16_mask(__mmask32, __m512i, __m512i);
extern __mmask16 __cdecl _mm512_mask_test_epi32_mask(__mmask16, __m512i, __m512i);
extern __mmask8  __cdecl _mm512_mask_test_epi64_mask(__mmask8, __m512i, __m512i);
extern __mmask16 __cdecl _mm512_mask_testn_epi32_mask(__mmask16, __m512i, __m512i);
extern __mmask8  __cdecl _mm512_mask_testn_epi64_mask(__mmask8, __m512i, __m512i);

extern __mmask16 __cdecl _mm512_kunpackb(__mmask16, __mmask16);
extern __mmask32 __cdecl _mm512_kunpackw(__mmask32, __mmask32);
extern __mmask64 __cdecl _mm512_kunpackd(__mmask64, __mmask64);

extern unsigned char __cdecl _mm512_testz_and_mask8(__mmask8, __mmask8);
extern unsigned char __cdecl _mm512_testz_and_mask16(__mmask16, __mmask16);
extern unsigned char __cdecl _mm512_testz_and_mask32(__mmask32, __mmask32);
extern unsigned char __cdecl _mm512_testz_and_mask64(__mmask64, __mmask64);
extern unsigned char __cdecl _mm512_testz_andn_mask8(__mmask8, __mmask8);
extern unsigned char __cdecl _mm512_testz_andn_mask16(__mmask16, __mmask16);
extern unsigned char __cdecl _mm512_testz_andn_mask32(__mmask32, __mmask32);
extern unsigned char __cdecl _mm512_testz_andn_mask64(__mmask64, __mmask64);
extern unsigned char __cdecl _mm512_testz_or_mask8(__mmask8, __mmask8);
extern unsigned char __cdecl _mm512_testz_or_mask16(__mmask16, __mmask16);
extern unsigned char __cdecl _mm512_testz_or_mask32(__mmask32, __mmask32);
extern unsigned char __cdecl _mm512_testz_or_mask64(__mmask64, __mmask64);
extern unsigned char __cdecl _mm512_testz_nor_mask8(__mmask8, __mmask8);
extern unsigned char __cdecl _mm512_testz_nor_mask16(__mmask16, __mmask16);
extern unsigned char __cdecl _mm512_testz_nor_mask32(__mmask32, __mmask32);
extern unsigned char __cdecl _mm512_testz_nor_mask64(__mmask64, __mmask64);

extern __m512  __cdecl _mm512_i32gather_ps(__m512i, void const*, int);
extern __m512  __cdecl _mm512_mask_i32gather_ps(__m512, __mmask16, __m512i, void const*, int);
extern void    __cdecl _mm512_i32scatter_ps(void*, __m512i, __m512, int);
extern void    __cdecl _mm512_mask_i32scatter_ps(void*, __mmask16, __m512i, __m512, int);
extern __m512d __cdecl _mm512_i64gather_pd(__m512i, void const*, int);
extern __m512d __cdecl _mm512_mask_i64gather_pd(__m512d, __mmask8, __m512i, void const*, int);
extern void    __cdecl _mm512_i64scatter_pd(void*, __m512i, __m512d, int);
extern void    __cdecl _mm512_mask_i64scatter_pd(void*, __mmask8, __m512i, __m512d, int);
extern __m512d __cdecl _mm512_i32gather_pd(__m256i, void const*, int);
extern __m512d __cdecl _mm512_mask_i32gather_pd(__m512d, __mmask8, __m256i, void const*, int);
extern void    __cdecl _mm512_i32scatter_pd(void*, __m256i, __m512d, int);
extern void    __cdecl _mm512_mask_i32scatter_pd(void*, __mmask8, __m256i, __m512d, int);
extern __m512i __cdecl _mm512_i32gather_epi32(__m512i, void const*, int);
extern __m512i __cdecl _mm512_mask_i32gather_epi32(__m512i, __mmask16, __m512i, void const*, int);
extern void    __cdecl _mm512_i32scatter_epi32(void*, __m512i, __m512i, int);
extern void    __cdecl _mm512_mask_i32scatter_epi32(void*, __mmask16, __m512i, __m512i, int);
extern __m512i __cdecl _mm512_i32gather_epi64(__m256i, void const*, int);
extern __m512i __cdecl _mm512_mask_i32gather_epi64(__m512i, __mmask8, __m256i, void const*, int);
extern __m512i __cdecl _mm512_i64gather_epi64(__m512i, void const*, int);
extern __m512i __cdecl _mm512_mask_i64gather_epi64(__m512i, __mmask8, __m512i, void const*, int);
extern void    __cdecl _mm512_i32scatter_epi64(void*, __m256i, __m512i, int);
extern void    __cdecl _mm512_mask_i32scatter_epi64(void*, __mmask8, __m256i, __m512i, int);
extern void    __cdecl _mm512_i64scatter_epi64(void*, __m512i, __m512i, int);
extern void    __cdecl _mm512_mask_i64scatter_epi64(void*, __mmask8, __m512i, __m512i, int);
extern __m256  __cdecl _mm512_i64gather_ps(__m512i, void const*, int);
extern __m256  __cdecl _mm512_mask_i64gather_ps(__m256, __mmask8, __m512i, void const*, int);
extern void    __cdecl _mm512_i64scatter_ps(void*, __m512i, __m256, int);
extern void    __cdecl _mm512_mask_i64scatter_ps(void*, __mmask8, __m512i, __m256, int);
extern __m256i __cdecl _mm512_i64gather_epi32(__m512i, void const*, int);
extern __m256i __cdecl _mm512_mask_i64gather_epi32(__m256i, __mmask8, __m512i, void const*, int);
extern void    __cdecl _mm512_i64scatter_epi32(void*, __m512i, __m256i, int);
extern void    __cdecl _mm512_mask_i64scatter_epi32(void*, __mmask8, __m512i, __m256i, int);

extern void __cdecl _mm512_prefetch_i32gather_pd(__m256i vindex, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i32gather_ps(__m512i index, void const* mv, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i32scatter_pd(void* base_addr, __m256i vindex, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i32scatter_ps(void* mv, __m512i index, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i64gather_pd(__m512i vindex, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i64gather_ps(__m512i vindex, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i64scatter_pd(void* base_addr, __m512i vindex, int scale, const int hint);
extern void __cdecl _mm512_prefetch_i64scatter_ps(void* base_addr, __m512i vindex, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i32gather_pd(__m256i vindex, __mmask8 mask, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i32gather_ps(__m512i vindex, __mmask16 mask, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i32scatter_pd(void* base_addr, __mmask8 mask, __m256i vinde, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i32scatter_ps(void* mv, __mmask16 k, __m512i index, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i64gather_pd(__m512i vindex, __mmask8 mask, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i64gather_ps(__m512i vindex, __mmask8 mask, void const* base_addr, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i64scatter_pd(void* base_addr, __mmask8 mask, __m512i vindex, int scale, const int hint);
extern void __cdecl _mm512_mask_prefetch_i64scatter_ps(void* base_addr, __mmask8 mask, __m512i vindex, int scale, const int hint);

extern __m512d __cdecl _mm512_cvtpslo_pd(__m512);
extern __m512d __cdecl _mm512_mask_cvtpslo_pd(__m512d, __mmask8, __m512);
extern __m512d __cdecl _mm512_cvtepi32lo_pd(__m512i);
extern __m512d __cdecl _mm512_mask_cvtepi32lo_pd(__m512d, __mmask8, __m512i);
extern __m512d __cdecl _mm512_cvtepu32lo_pd(__m512i);
extern __m512d __cdecl _mm512_mask_cvtepu32lo_pd(__m512d, __mmask8, __m512i);
extern __m512d __cdecl _mm512_mask_cvtepi32_pd(__m512d, __mmask8, __m256i);
extern __m512d __cdecl _mm512_maskz_cvtepi32_pd(__mmask8, __m256i);
extern __m512d __cdecl _mm512_mask_cvtepu32_pd(__m512d, __mmask8, __m256i);
extern __m512d __cdecl _mm512_maskz_cvtepu32_pd(__mmask8, __m256i);

extern __m512  __cdecl _mm512_mask_cvt_roundepi32_ps(__m512, __mmask16, __m512i, int);
extern __m512  __cdecl _mm512_maskz_cvt_roundepi32_ps(__mmask16, __m512i, int);
extern __m512  __cdecl _mm512_mask_cvt_roundepu32_ps(__m512, __mmask16, __m512i, int);
extern __m512  __cdecl _mm512_maskz_cvt_roundepu32_ps(__mmask16, __m512i, int);
extern __m512  __cdecl _mm512_mask_cvt_roundph_ps(__m512, __mmask16, __m256i, int);
extern __m512  __cdecl _mm512_maskz_cvt_roundph_ps(__mmask16, __m256i, int);
extern __m256i __cdecl _mm512_mask_cvt_roundps_ph(__m256i, __mmask16, __m512, int);
extern __m256i __cdecl _mm512_maskz_cvt_roundps_ph(__mmask16, __m512, int);
extern __m256  __cdecl _mm512_mask_cvt_roundepi64_ps(__m256, __mmask8, __m512i, int);
extern __m256  __cdecl _mm512_maskz_cvt_roundepi64_ps(__mmask8, __m512i, int);
extern __m256  __cdecl _mm512_mask_cvt_roundepu64_ps(__m256, __mmask8, __m512i, int);
extern __m256  __cdecl _mm512_maskz_cvt_roundepu64_ps(__mmask8, __m512i, int);

extern __m512i __cdecl _mm512_mask_cvtepi8_epi32(__m512i, __mmask16, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepi8_epi32(__mmask16, __m128i);
extern __m512i __cdecl _mm512_mask_cvtepi8_epi64(__m512i, __mmask8, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepi8_epi64(__mmask8, __m128i);
extern __m512i __cdecl _mm512_mask_cvtepi16_epi32(__m512i, __mmask16, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepi16_epi32(__mmask16, __m256i);
extern __m512i __cdecl _mm512_mask_cvtepi16_epi64(__m512i, __mmask8, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepi16_epi64(__mmask8, __m128i);
extern __m128i __cdecl _mm512_mask_cvtepi32_epi8(__m128i, __mmask16, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtepi32_epi8(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtepi32_storeu_epi8(void*, __mmask16, __m512i);
extern __m128i __cdecl _mm512_mask_cvtsepi32_epi8(__m128i, __mmask16, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtsepi32_epi8(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi32_storeu_epi8(void*, __mmask16, __m512i);
extern __m128i __cdecl _mm512_mask_cvtusepi32_epi8(__m128i, __mmask16, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtusepi32_epi8(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi32_storeu_epi8(void*, __mmask16, __m512i);
extern __m256i __cdecl _mm512_mask_cvtepi32_epi16(__m256i, __mmask16, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtepi32_epi16(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtepi32_storeu_epi16(void*, __mmask16, __m512i);
extern __m256i __cdecl _mm512_mask_cvtsepi32_epi16(__m256i, __mmask16, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtsepi32_epi16(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi32_storeu_epi16(void*, __mmask16, __m512i);
extern __m256i __cdecl _mm512_mask_cvtusepi32_epi16(__m256i, __mmask16, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtusepi32_epi16(__mmask16, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi32_storeu_epi16(void*, __mmask16, __m512i);
extern __m512i __cdecl _mm512_mask_cvtepi32_epi64(__m512i, __mmask8, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepi32_epi64(__mmask8, __m256i);
extern __m128i __cdecl _mm512_mask_cvtepi64_epi8(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtepi64_epi8(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtepi64_storeu_epi8(void*, __mmask8, __m512i);
extern __m128i __cdecl _mm512_mask_cvtsepi64_epi8(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtsepi64_epi8(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi64_storeu_epi8(void*, __mmask8, __m512i);
extern __m128i __cdecl _mm512_mask_cvtusepi64_epi8(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtusepi64_epi8(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi64_storeu_epi8(void*, __mmask8, __m512i);
extern __m128i __cdecl _mm512_mask_cvtepi64_epi16(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtepi64_epi16(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtepi64_storeu_epi16(void*, __mmask8, __m512i);
extern __m128i __cdecl _mm512_mask_cvtsepi64_epi16(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtsepi64_epi16(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi64_storeu_epi16(void*, __mmask8, __m512i);
extern __m128i __cdecl _mm512_mask_cvtusepi64_epi16(__m128i, __mmask8, __m512i);
extern __m128i __cdecl _mm512_maskz_cvtusepi64_epi16(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi64_storeu_epi16(void*, __mmask8, __m512i);
extern __m256i __cdecl _mm512_mask_cvtepi64_epi32(__m256i, __mmask8, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtepi64_epi32(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtepi64_storeu_epi32(void*, __mmask8, __m512i);
extern __m256i __cdecl _mm512_mask_cvtsepi64_epi32(__m256i, __mmask8, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtsepi64_epi32(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi64_storeu_epi32(void*, __mmask8, __m512i);
extern __m256i __cdecl _mm512_mask_cvtusepi64_epi32(__m256i, __mmask8, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtusepi64_epi32(__mmask8, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi64_storeu_epi32(void*, __mmask8, __m512i);
extern __m512i __cdecl _mm512_mask_cvtepu8_epi32(__m512i, __mmask16, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepu8_epi32(__mmask16, __m128i);
extern __m512i __cdecl _mm512_mask_cvtepu8_epi64(__m512i, __mmask8, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepu8_epi64(__mmask8, __m128i);
extern __m512i __cdecl _mm512_mask_cvtepu16_epi32(__m512i, __mmask16, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepu16_epi32(__mmask16, __m256i);
extern __m512i __cdecl _mm512_mask_cvtepu16_epi64(__m512i, __mmask8, __m128i);
extern __m512i __cdecl _mm512_maskz_cvtepu16_epi64(__mmask8, __m128i);
extern __m512i __cdecl _mm512_mask_cvtepu32_epi64(__m512i, __mmask8, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepu32_epi64(__mmask8, __m256i);

extern __m512i __cdecl _mm512_mask_cvt_roundps_epi32(__m512i, __mmask16, __m512, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundps_epi32(__mmask16, __m512, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundps_epi32(__m512i, __mmask16, __m512, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundps_epi32(__mmask16, __m512, int);
extern __m512i __cdecl _mm512_mask_cvt_roundps_epu32(__m512i, __mmask16, __m512, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundps_epu32(__mmask16, __m512, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundps_epu32(__m512i, __mmask16, __m512, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundps_epu32(__mmask16, __m512, int);
extern __m256i __cdecl _mm512_mask_cvt_roundpd_epi32(__m256i, __mmask8, __m512d, int);
extern __m256i __cdecl _mm512_maskz_cvt_roundpd_epi32(__mmask8, __m512d, int);
extern __m256i __cdecl _mm512_mask_cvtt_roundpd_epi32(__m256i, __mmask8, __m512d, int);
extern __m256i __cdecl _mm512_maskz_cvtt_roundpd_epi32(__mmask8, __m512d, int);
extern __m256i __cdecl _mm512_mask_cvt_roundpd_epu32(__m256i, __mmask8, __m512d, int);
extern __m256i __cdecl _mm512_maskz_cvt_roundpd_epu32(__mmask8, __m512d, int);
extern __m256i __cdecl _mm512_mask_cvtt_roundpd_epu32(__m256i, __mmask8, __m512d, int);
extern __m256i __cdecl _mm512_maskz_cvtt_roundpd_epu32(__mmask8, __m512d, int);

extern __m512i __cdecl _mm512_mask_cvtepi8_epi16(__m512i, __mmask32, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepi8_epi16(__mmask32, __m256i);
extern __m512i __cdecl _mm512_mask_cvtepu8_epi16(__m512i, __mmask32, __m256i);
extern __m512i __cdecl _mm512_maskz_cvtepu8_epi16(__mmask32, __m256i);
extern __m256i __cdecl _mm512_mask_cvtepi16_epi8(__m256i, __mmask32, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtepi16_epi8(__mmask32, __m512i);
extern void    __cdecl _mm512_mask_cvtepi16_storeu_epi8(void*, __mmask32, __m512i);
extern __m256i __cdecl _mm512_mask_cvtsepi16_epi8(__m256i, __mmask32, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtsepi16_epi8(__mmask32, __m512i);
extern void    __cdecl _mm512_mask_cvtsepi16_storeu_epi8(void*, __mmask32, __m512i);
extern __m256i __cdecl _mm512_mask_cvtusepi16_epi8(__m256i, __mmask32, __m512i);
extern __m256i __cdecl _mm512_maskz_cvtusepi16_epi8(__mmask32, __m512i);
extern void    __cdecl _mm512_mask_cvtusepi16_storeu_epi8(void*, __mmask32, __m512i);

extern __m512d __cdecl _mm512_mask_cvt_roundepi64_pd(__m512d, __mmask8, __m512i, int);
extern __m512d __cdecl _mm512_maskz_cvt_roundepi64_pd(__mmask8, __m512i, int);
extern __m512d __cdecl _mm512_mask_cvt_roundepu64_pd(__m512d, __mmask8, __m512i, int);
extern __m512d __cdecl _mm512_maskz_cvt_roundepu64_pd(__mmask8, __m512i, int);
extern __m512i __cdecl _mm512_mask_cvt_roundpd_epi64(__m512i, __mmask8, __m512d, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundpd_epi64(__mmask8, __m512d, int);
extern __m512i __cdecl _mm512_mask_cvt_roundpd_epu64(__m512i, __mmask8, __m512d, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundpd_epu64(__mmask8, __m512d, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundpd_epi64(__m512i, __mmask8, __m512d, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundpd_epi64(__mmask8, __m512d, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundpd_epu64(__m512i, __mmask8, __m512d, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundpd_epu64(__mmask8, __m512d, int);
extern __m512i __cdecl _mm512_mask_cvt_roundps_epi64(__m512i, __mmask8, __m256, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundps_epi64(__mmask8, __m256, int);
extern __m512i __cdecl _mm512_mask_cvt_roundps_epu64(__m512i, __mmask8, __m256, int);
extern __m512i __cdecl _mm512_maskz_cvt_roundps_epu64(__mmask8, __m256, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundps_epi64(__m512i, __mmask8, __m256, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundps_epi64(__mmask8, __m256, int);
extern __m512i __cdecl _mm512_mask_cvtt_roundps_epu64(__m512i, __mmask8, __m256, int);
extern __m512i __cdecl _mm512_maskz_cvtt_roundps_epu64(__mmask8, __m256, int);

#define _mm512_setzero_epi32 _mm512_setzero_si512
#define _mm512_set1_epi8(e1) \
    _mm512_maskz_set1_epi8(_MM_K0_REG64, (e1))
#define _mm512_set1_epi16(e1) \
    _mm512_maskz_set1_epi16((__mmask32)_MM_K0_REG64, (e1))
#define _mm512_set1_epi32(e1) \
    _mm512_maskz_set1_epi32(_MM_K0_REG, (e1))
#define _mm512_set1_epi64(e1) \
    _mm512_maskz_set1_epi64((__mmask8)_MM_K0_REG, (e1))
#define _mm512_set_1to8_pq(x) _mm512_set1_epi64((x))
#define _mm512_set_1to8_epi64(x) _mm512_set1_epi64((x))
#define _mm512_set_1to16_pi(x) _mm512_set1_epi32((x))
#define _mm512_set_1to16_epi32(x) _mm512_set1_epi32((x))
#define _mm512_set4_epi64(a,b,c,d) \
    _mm512_set_epi64((a), (b), (c), (d), (a), (b), (c), (d))
#define _mm512_set4_epi32(a,b,c,d) \
    _mm512_set_epi32((a), (b), (c), (d), (a), (b), (c), (d), \
                    (a), (b), (c), (d), (a), (b), (c), (d))
#define _mm512_setr4_epi64(a,b,c,d) \
    _mm512_set4_epi64((d),(c),(b),(a))
#define _mm512_set_4to8_pq(a,b,c,d) \
    _mm512_set4_epi64((d),(c),(b),(a))
#define _mm512_set_4to8_epi64(a,b,c,d) \
    _mm512_set4_epi64((d),(c),(b),(a))
#define _mm512_setr4_epi32(a,b,c,d) \
    _mm512_set4_epi32((d),(c),(b),(a))
#define _mm512_set_4to16_pi(a,b,c,d) \
    _mm512_set4_epi32((d),(c),(b),(a))
#define _mm512_set_4to16_epi32(a,b,c,d) \
    _mm512_set4_epi32((d),(c),(b),(a))
#define _mm512_setr_epi32(e0,e1,e2,e3,e4,e5,e6,e7,e8, \
                            e9,e10,e11,e12,e13,e14,e15) \
    _mm512_set_epi32((e15),(e14),(e13),(e12),(e11),(e10), \
                        (e9),(e8),(e7),(e6),(e5),(e4),(e3),(e2),(e1),(e0))
#define _mm512_set_16to16_pi(e0,e1,e2,e3,e4,e5,e6,e7,e8, \
                                e9,e10,e11,e12,e13,e14,e15) \
    _mm512_set_epi32((e0),(e1),(e2),(e3),(e4),(e5),(e6),(e7), \
                        (e8),(e9),(e10),(e11),(e12),(e13),(e14),(e15))
#define _mm512_set_16to16_epi32(e0,e1,e2,e3,e4,e5,e6,e7,e8, \
                                e9,e10,e11,e12,e13,e14,e15) \
    _mm512_set_epi32((e0),(e1),(e2),(e3),(e4),(e5),(e6),(e7), \
                        (e8),(e9),(e10),(e11),(e12),(e13),(e14),(e15))
#define _mm512_setr_epi64(e0,e1,e2,e3,e4,e5,e6,e7) \
    _mm512_set_epi64((e7),(e6),(e5),(e4),(e3),(e2),(e1),(e0))
#define _mm512_set_8to8_pq(e0,e1,e2,e3,e4,e5,e6,e7) \
    _mm512_set_epi64((e0),(e1),(e2),(e3),(e4),(e5),(e6),(e7))
#define _mm512_set_8to8_epi64(e0,e1,e2,e3,e4,e5,e6,e7) \
    _mm512_set_epi64((e0),(e1),(e2),(e3),(e4),(e5),(e6),(e7))

#define _mm512_add_epi8(v1, v2) \
    _mm512_maskz_add_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_add_epi16(v1, v2) \
    _mm512_maskz_add_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_add_epi32(v1, v2) \
    _mm512_maskz_add_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_add_epi64(v1, v2) \
    _mm512_maskz_add_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_adds_epi8(v1, v2) \
    _mm512_maskz_adds_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_adds_epi16(v1, v2) \
    _mm512_maskz_adds_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_adds_epu8(v1, v2) \
    _mm512_maskz_adds_epu8(_MM_K0_REG64, (v1), (v2))
#define _mm512_adds_epu16(v1, v2) \
    _mm512_maskz_adds_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_abs_epi8(v1) \
    _mm512_maskz_abs_epi8(_MM_K0_REG64, (v1))
#define _mm512_abs_epi16(v1) \
    _mm512_maskz_abs_epi16((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_abs_epi32(v1) \
    _mm512_maskz_abs_epi32(_MM_K0_REG, (v1))
#define _mm512_abs_epi64(v1) \
    _mm512_maskz_abs_epi64((__mmask8)_MM_K0_REG, (v1))

#define _mm512_broadcast_i32x2(v1) \
    _mm512_maskz_broadcast_i32x2(_MM_K0_REG, (v1))
#define _mm512_broadcast_i32x4(v1) \
    _mm512_maskz_broadcast_i32x4(_MM_K0_REG, (v1))
#define _mm512_broadcast_i32x8(v1) \
    _mm512_maskz_broadcast_i32x8(_MM_K0_REG, (v1))
#define _mm512_broadcast_i64x2(v1) \
    _mm512_maskz_broadcast_i64x2(_MM_K0_REG, (v1))
#define _mm512_broadcast_i64x4(v1) \
    _mm512_maskz_broadcast_i64x4((__mmask8)_MM_K0_REG, (v1))
#define _mm512_broadcastb_epi8(v1) \
    _mm512_maskz_broadcastb_epi8(_MM_K0_REG64, (v1))
#define _mm512_broadcastw_epi16(v1) \
    _mm512_maskz_broadcastw_epi16((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_broadcastd_epi32(v1) \
    _mm512_maskz_broadcastd_epi32(_MM_K0_REG, (v1))
#define _mm512_broadcastq_epi64(v1) \
    _mm512_maskz_broadcastq_epi64((__mmask8)_MM_K0_REG, (v1))

#define _mm512_sub_epi8(v1, v2) \
    _mm512_maskz_sub_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_sub_epi16(v1, v2) \
    _mm512_maskz_sub_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_sub_epi32(v1, v2) \
    _mm512_maskz_sub_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_sub_epi64(v1, v2) \
    _mm512_maskz_sub_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_subs_epi8(v1, v2) \
    _mm512_maskz_subs_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_subs_epi16(v1, v2) \
    _mm512_maskz_subs_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_subs_epu8(v1, v2) \
    _mm512_maskz_subs_epu8(_MM_K0_REG64, (v1), (v2))
#define _mm512_subs_epu16(v1, v2) \
    _mm512_maskz_subs_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_max_epi8(v1, v2) \
    _mm512_maskz_max_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_max_epi16(v1, v2) \
    _mm512_maskz_max_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_max_epi32(v1, v2) \
    _mm512_maskz_max_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_max_epi64(v1, v2) \
    _mm512_maskz_max_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_max_epu8(v1, v2) \
    _mm512_maskz_max_epu8(_MM_K0_REG64, (v1), (v2))
#define _mm512_max_epu16(v1, v2) \
    _mm512_maskz_max_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_max_epu32(v1, v2) \
    _mm512_maskz_max_epu32(_MM_K0_REG, (v1), (v2))
#define _mm512_max_epu64(v1, v2) \
    _mm512_maskz_max_epu64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_min_epi8(v1, v2) \
    _mm512_maskz_min_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_min_epi16(v1, v2) \
    _mm512_maskz_min_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_min_epi32(v1, v2) \
    _mm512_maskz_min_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_min_epi64(v1, v2) \
    _mm512_maskz_min_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_min_epu8(v1, v2) \
    _mm512_maskz_min_epu8(_MM_K0_REG64, (v1), (v2))
#define _mm512_min_epu16(v1, v2) \
    _mm512_maskz_min_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_min_epu32(v1, v2) \
    _mm512_maskz_min_epu32(_MM_K0_REG, (v1), (v2))
#define _mm512_min_epu64(v1, v2) \
    _mm512_maskz_min_epu64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_mul_epi32(v1, v2) \
    _mm512_maskz_mul_epi32((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_mul_epu32(v1, v2) \
    _mm512_maskz_mul_epu32((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_mulhi_epi16(v1, v2) \
    _mm512_maskz_mulhi_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_mulhi_epu16(v1, v2) \
    _mm512_maskz_mulhi_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_mullo_epi16(v1, v2) \
    _mm512_maskz_mullo_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_mullo_epi32(v1, v2) \
    _mm512_maskz_mullo_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_mullo_epi64(v1, v2) \
    _mm512_maskz_mullo_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_mulhrs_epi16(v1, v2) \
    _mm512_maskz_mulhrs_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_load_epi32(p1) \
    _mm512_maskz_load_epi32(_MM_K0_REG, (p1))
#define _mm512_load_si512   _mm512_load_epi32
#define _mm512_load_epi64(p1) \
    _mm512_maskz_load_epi64((__mmask8)_MM_K0_REG, (p1))
#define _mm512_loadu_epi8(p1) \
    _mm512_maskz_loadu_epi8(_MM_K0_REG64, (p1))
#define _mm512_loadu_epi16(p1) \
    _mm512_maskz_loadu_epi16((__mmask32)_MM_K0_REG64, (p1))
#define _mm512_loadu_epi32(p1) \
    _mm512_maskz_loadu_epi32(_MM_K0_REG, (p1))
#define _mm512_loadu_si512  _mm512_loadu_epi32
#define _mm512_loadu_epi64(p1) \
    _mm512_maskz_loadu_epi64((__mmask8)_MM_K0_REG, (p1))
#define _mm512_store_epi32(p1, v1) \
    _mm512_mask_store_epi32((p1), _MM_K0_REG, (v1))
#define _mm512_store_si512  _mm512_store_epi32
#define _mm512_store_epi64(p1, v1) \
    _mm512_mask_store_epi64((p1), (__mmask8)_MM_K0_REG, (v1))
#define _mm512_storeu_epi8(p1, v1) \
    _mm512_mask_storeu_epi8((p1), _MM_K0_REG64, (v1))
#define _mm512_storeu_epi16(p1, v1) \
    _mm512_mask_storeu_epi16((p1), (__mmask32)_MM_K0_REG64, (v1))
#define _mm512_storeu_epi32(p1, v1) \
    _mm512_mask_storeu_epi32((p1), _MM_K0_REG, (v1))
#define _mm512_storeu_si512 _mm512_storeu_epi32
#define _mm512_storeu_epi64(p1, v1) \
    _mm512_mask_storeu_epi64((p1), (__mmask8)_MM_K0_REG, (v1))

#define _mm512_extracti32x4_epi32(v1, e1) \
    _mm512_maskz_extracti32x4_epi32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_extracti32x8_epi32(v1, e1) \
    _mm512_maskz_extracti32x8_epi32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_extracti64x2_epi64(v1, e1) \
    _mm512_maskz_extracti64x2_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_extracti64x4_epi64(v1, e1) \
    _mm512_maskz_extracti64x4_epi64((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_inserti32x4(v1, v2, e1) \
    _mm512_maskz_inserti32x4(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_inserti32x8(v1, v2, e1) \
    _mm512_maskz_inserti32x8(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_inserti64x2(v1, v2, e1) \
    _mm512_maskz_inserti64x2((__mmask8)_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_inserti64x4(v1, v2, e1) \
    _mm512_maskz_inserti64x4((__mmask8)_MM_K0_REG, (v1), (v2), (e1))

#define _mm512_shuffle_epi8(v1, v2) \
    _mm512_maskz_shuffle_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_shuffle_epi32(v1, e1) \
    _mm512_maskz_shuffle_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_shuffle_i32x4(v1, v2, e1) \
    _mm512_maskz_shuffle_i32x4(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_shuffle_i64x2(v1, v2, e1) \
    _mm512_maskz_shuffle_i64x2((__mmask8)_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_shufflehi_epi16(v1, e1) \
    _mm512_maskz_shufflehi_epi16((__mmask32)_MM_K0_REG64, (v1), (e1))
#define _mm512_shufflelo_epi16(v1, e1) \
    _mm512_maskz_shufflelo_epi16((__mmask32)_MM_K0_REG64, (v1), (e1))

#define _mm512_movedup_pd(v1) \
    _mm512_maskz_movedup_pd((__mmask8)_MM_K0_REG, (v1))
#define _mm512_movehdup_ps(v1) \
    _mm512_maskz_movehdup_ps(_MM_K0_REG, (v1))
#define _mm512_moveldup_ps(v1) \
    _mm512_maskz_moveldup_ps(_MM_K0_REG, (v1))

#define _mm512_alignr_epi8(v1, v2, e1) \
    _mm512_maskz_alignr_epi8(_MM_K0_REG64, (v1), (v2), (e1))
#define _mm512_alignr_epi32(v1, v2, e1) \
    _mm512_maskz_alignr_epi32(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_alignr_epi64(v1, v2, e1) \
    _mm512_maskz_alignr_epi64((__mmask8)_MM_K0_REG, (v1), (v2), (e1))

#define _mm512_and_pd(v1, v2) \
    _mm512_maskz_and_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_and_ps(v1, v2) \
    _mm512_maskz_and_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_and_epi32(v1, v2) \
    _mm512_maskz_and_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_and_si512      _mm512_and_epi32
#define _mm512_and_epi64(v1, v2) \
    _mm512_maskz_and_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_andnot_pd(v1, v2) \
    _mm512_maskz_andnot_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_andnot_ps(v1, v2) \
    _mm512_maskz_andnot_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_andnot_epi32(v1, v2) \
    _mm512_maskz_andnot_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_andnot_si512   _mm512_andnot_epi32
#define _mm512_andnot_epi64(v1, v2) \
    _mm512_maskz_andnot_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_or_pd(v1, v2) \
    _mm512_maskz_or_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_or_ps(v1, v2) \
    _mm512_maskz_or_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_or_epi32(v1, v2) \
    _mm512_maskz_or_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_or_si512       _mm512_or_epi32
#define _mm512_or_epi64(v1, v2) \
    _mm512_maskz_or_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_xor_pd(v1, v2) \
    _mm512_maskz_xor_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_xor_ps(v1, v2) \
    _mm512_maskz_xor_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_xor_epi32(v1, v2) \
    _mm512_maskz_xor_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_xor_si512      _mm512_xor_epi32
#define _mm512_xor_epi64(v1, v2) \
    _mm512_maskz_xor_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_sll_epi16(v1, v2) \
    _mm512_maskz_sll_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_sll_epi32(v1, v2) \
    _mm512_maskz_sll_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_sll_epi64(v1, v2) \
    _mm512_maskz_sll_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_slli_epi16(v1, e1) \
    _mm512_maskz_slli_epi16((__mmask32)_MM_K0_REG64, (v1), (e1))
#define _mm512_slli_epi32(v1, e1) \
    _mm512_maskz_slli_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_slli_epi64(v1, e1) \
    _mm512_maskz_slli_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_sllv_epi16(v1, v2) \
    _mm512_maskz_sllv_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_sllv_epi32(v1, v2) \
    _mm512_maskz_sllv_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_sllv_epi64(v1, v2) \
    _mm512_maskz_sllv_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_srl_epi16(v1, v2) \
    _mm512_maskz_srl_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_srl_epi32(v1, v2) \
    _mm512_maskz_srl_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_srl_epi64(v1, v2) \
    _mm512_maskz_srl_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_srli_epi16(v1, e1) \
    _mm512_maskz_srli_epi16((__mmask32)_MM_K0_REG64, (v1), (e1))
#define _mm512_srli_epi32(v1, e1) \
    _mm512_maskz_srli_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_srli_epi64(v1, e1) \
    _mm512_maskz_srli_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_srlv_epi16(v1, v2) \
    _mm512_maskz_srlv_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_srlv_epi32(v1, v2) \
    _mm512_maskz_srlv_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_srlv_epi64(v1, v2) \
    _mm512_maskz_srlv_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_sra_epi16(v1, v2) \
    _mm512_maskz_sra_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_sra_epi32(v1, v2) \
    _mm512_maskz_sra_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_sra_epi64(v1, v2) \
    _mm512_maskz_sra_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_srai_epi16(v1, e1) \
    _mm512_maskz_srai_epi16((__mmask32)_MM_K0_REG64, (v1), (e1))
#define _mm512_srai_epi32(v1, e1) \
    _mm512_maskz_srai_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_srai_epi64(v1, e1) \
    _mm512_maskz_srai_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_srav_epi16(v1, v2) \
    _mm512_maskz_srav_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_srav_epi32(v1, v2) \
    _mm512_maskz_srav_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_srav_epi64(v1, v2) \
    _mm512_maskz_srav_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_rol_epi32(v1, e1) \
    _mm512_maskz_rol_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_rol_epi64(v1, e1) \
    _mm512_maskz_rol_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_rolv_epi32(v1, v2) \
    _mm512_maskz_rolv_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_rolv_epi64(v1, v2) \
    _mm512_maskz_rolv_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_ror_epi32(v1, e1) \
    _mm512_maskz_ror_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_ror_epi64(v1, e1) \
    _mm512_maskz_ror_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_rorv_epi32(v1, v2) \
    _mm512_maskz_rorv_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_rorv_epi64(v1, v2) \
    _mm512_maskz_rorv_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_unpackhi_pd(v1, v2) \
    _mm512_maskz_unpackhi_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_unpackhi_ps(v1, v2) \
    _mm512_maskz_unpackhi_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_unpacklo_pd(v1, v2) \
    _mm512_maskz_unpacklo_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_unpacklo_ps(v1, v2) \
    _mm512_maskz_unpacklo_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_unpackhi_epi8(v1, v2) \
    _mm512_maskz_unpackhi_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_unpackhi_epi16(v1, v2) \
    _mm512_maskz_unpackhi_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_unpackhi_epi32(v1, v2) \
    _mm512_maskz_unpackhi_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_unpackhi_epi64(v1, v2) \
    _mm512_maskz_unpackhi_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_unpacklo_epi8(v1, v2) \
    _mm512_maskz_unpacklo_epi8(_MM_K0_REG64, (v1), (v2))
#define _mm512_unpacklo_epi16(v1, v2) \
    _mm512_maskz_unpacklo_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_unpacklo_epi32(v1, v2) \
    _mm512_maskz_unpacklo_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_unpacklo_epi64(v1, v2) \
    _mm512_maskz_unpacklo_epi64((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_mask_getexp_ps(v1, k1, v2) \
    _mm512_mask_getexp_round_ps((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_getexp_ps(k1, v1) \
    _mm512_maskz_getexp_round_ps((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_getexp_round_ps(v1, e1) \
    _mm512_maskz_getexp_round_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_getexp_ps(v1) \
    _mm512_maskz_getexp_ps(_MM_K0_REG, (v1))
#define _mm512_mask_getexp_pd(v1, k1, v2) \
    _mm512_mask_getexp_round_pd((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_getexp_pd(k1, v1) \
    _mm512_maskz_getexp_round_pd((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_getexp_round_pd(v1, e1) \
    _mm512_maskz_getexp_round_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_getexp_pd(v1) \
    _mm512_maskz_getexp_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_mask_getmant_ps(v1, k1, v2, e1, e2) \
    _mm512_mask_getmant_round_ps((v1), (k1), (v2), (e1), (e2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_getmant_ps(k1, v1, e1, e2) \
    _mm512_maskz_getmant_round_ps((k1), (v1), (e1), (e2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_getmant_round_ps(v1, e1, e2, e3) \
    _mm512_maskz_getmant_round_ps(_MM_K0_REG, (v1), (e1), (e2), (e3))
#define _mm512_getmant_ps(v1, e1, e2) \
    _mm512_maskz_getmant_ps(_MM_K0_REG, (v1), (e1), (e2))
#define _mm512_mask_getmant_pd(v1, k1, v2, e1, e2) \
    _mm512_mask_getmant_round_pd((v1), (k1), (v2), (e1), (e2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_getmant_pd(k1, v1, e1, e2) \
    _mm512_maskz_getmant_round_pd((k1), (v1), (e1), (e2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_getmant_round_pd(v1, e1, e2, e3) \
    _mm512_maskz_getmant_round_pd((__mmask8)_MM_K0_REG, (v1), (e1), (e2), (e3))
#define _mm512_getmant_pd(v1, e1, e2) \
    _mm512_maskz_getmant_pd((__mmask8)_MM_K0_REG, (v1), (e1), (e2))

#define _mm512_permute_pd(v1, e1) \
    _mm512_maskz_permute_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_permute_ps(v1, e1) \
    _mm512_maskz_permute_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_permutevar_pd(v1, v2) \
    _mm512_maskz_permutevar_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_permutevar_ps(v1, v2) \
    _mm512_maskz_permutevar_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_permutex_pd(v1, e1) \
    _mm512_maskz_permutex_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_permutex_epi64(v1, e1) \
    _mm512_maskz_permutex_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_permutexvar_pd(v1, v2) \
    _mm512_maskz_permutexvar_pd((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_permutexvar_ps(v1, v2) \
    _mm512_maskz_permutexvar_ps(_MM_K0_REG, (v1), (v2))
#define _mm512_permutexvar_epi16(v1, v2) \
    _mm512_maskz_permutexvar_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_permutexvar_epi32(v1, v2) \
    _mm512_maskz_permutexvar_epi32(_MM_K0_REG, (v1), (v2))
#define _mm512_permutexvar_epi64(v1, v2) \
    _mm512_maskz_permutexvar_epi64((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_permutex2var_pd(v1, v2, v3) \
    _mm512_maskz_permutex2var_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_permutex2var_ps(v1, v2, v3) \
    _mm512_maskz_permutex2var_ps(_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_permutex2var_epi16(v1, v2, v3) \
    _mm512_maskz_permutex2var_epi16((__mmask32)_MM_K0_REG64, (v1), (v2), (v3))
#define _mm512_permutex2var_epi32(v1, v2, v3) \
    _mm512_maskz_permutex2var_epi32(_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_permutex2var_epi64(v1, v2, v3) \
    _mm512_maskz_permutex2var_epi64((__mmask8)_MM_K0_REG, (v1), (v2), (v3))

#define _mm512_ternarylogic_epi32(v1, v2, v3, e1) \
    _mm512_maskz_ternarylogic_epi32(_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_ternarylogic_epi64(v1, v2, v3, e1) \
    _mm512_maskz_ternarylogic_epi64((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e1))

#define _mm512_conflict_epi32(v1) \
    _mm512_maskz_conflict_epi32(_MM_K0_REG, (v1))
#define _mm512_conflict_epi64(v1) \
    _mm512_maskz_conflict_epi64((__mmask8)_MM_K0_REG, (v1))

#define _mm512_lzcnt_epi32(v1) \
    _mm512_maskz_lzcnt_epi32(_MM_K0_REG, (v1))
#define _mm512_lzcnt_epi64(v1) \
    _mm512_maskz_lzcnt_epi64((__mmask8)_MM_K0_REG, (v1))

#define _mm512_avg_epu8(v1, v2) \
    _mm512_maskz_avg_epu8(_MM_K0_REG64, (v1), (v2))
#define _mm512_avg_epu16(v1, v2) \
    _mm512_maskz_avg_epu16((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_dbsad_epu8(v1, v2, e1) \
    _mm512_maskz_dbsad_epu8((__mmask32)_MM_K0_REG64, (v1), (v2), (e1))

#define _mm512_reduce_round_pd(v1, e1, e2) \
    _mm512_maskz_reduce_round_pd((__mmask8)_MM_K0_REG, (v1), (e1), (e2))
#define _mm512_mask_reduce_pd(v1, k1, v2, e1) \
    _mm512_mask_reduce_round_pd((v1), (k1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_reduce_pd(k1, v1, e1) \
    _mm512_maskz_reduce_round_pd((k1), (v1), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_reduce_pd(v1, e1) \
    _mm512_maskz_reduce_pd((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_reduce_round_ps(v1, e1, e2) \
    _mm512_maskz_reduce_round_ps(_MM_K0_REG, (v1), (e1), (e2))
#define _mm512_mask_reduce_ps(v1, k1, v2, e1) \
    _mm512_mask_reduce_round_ps((v1), (k1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_reduce_ps(k1, v1, e1) \
    _mm512_maskz_reduce_round_ps((k1), (v1), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_reduce_ps(v1, e1) \
    _mm512_maskz_reduce_ps(_MM_K0_REG, (v1), (e1))

#define _mm512_roundscale_round_pd(v1, e1, e2) \
    _mm512_maskz_roundscale_round_pd((__mmask8)_MM_K0_REG, (v1), (e1), (e2))
#define _mm512_mask_roundscale_pd(v1, k1, v2, e1) \
    _mm512_mask_roundscale_round_pd((v1), (k1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_roundscale_pd(k1, v1, e1) \
    _mm512_maskz_roundscale_round_pd((k1), (v1), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_roundscale_pd(v1, e1) \
    _mm512_maskz_roundscale_pd((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_roundscale_round_ps(v1, e1, e2) \
    _mm512_maskz_roundscale_round_ps(_MM_K0_REG, (v1), (e1), (e2))
#define _mm512_mask_roundscale_ps(v1, k1, v2, e1) \
    _mm512_mask_roundscale_round_ps((v1), (k1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_roundscale_ps(k1, v1, e1) \
    _mm512_maskz_roundscale_round_ps((k1), (v1), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_roundscale_ps(v1, e1) \
    _mm512_maskz_roundscale_ps(_MM_K0_REG, (v1), (e1))

#define _mm512_scalef_round_pd(v1, v2, e1) \
    _mm512_maskz_scalef_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_mask_scalef_pd(v1, k1, v2, v3) \
    _mm512_mask_scalef_round_pd((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_scalef_pd(k1, v1, v2) \
    _mm512_maskz_scalef_round_pd((k1), (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_scalef_pd(v1, v2) \
    _mm512_maskz_scalef_pd((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_scalef_round_ps(v1, v2, e1) \
    _mm512_maskz_scalef_round_ps(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_mask_scalef_ps(v1, k1, v2, v3) \
    _mm512_mask_scalef_round_ps((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_scalef_ps(k1, v1, v2) \
    _mm512_maskz_scalef_round_ps((k1), (v1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_scalef_ps(v1, v2) \
    _mm512_maskz_scalef_ps(_MM_K0_REG, (v1), (v2))

#define _mm512_fixupimm_round_pd(v1, v2, v3, e1, e2) \
    _mm512_maskz_fixupimm_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e1), (e2))
#define _mm512_mask_fixupimm_pd(v1, k1, v2, v3, e1) \
    _mm512_mask_fixupimm_round_pd((v1), (k1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fixupimm_pd(k1, v1, v2, v3, e1) \
    _mm512_maskz_fixupimm_round_pd((k1), (v1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fixupimm_pd(v1, v2, v3, e1) \
    _mm512_maskz_fixupimm_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_fixupimm_round_ps(v1, v2, v3, e1, e2) \
    _mm512_maskz_fixupimm_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e1), (e2))
#define _mm512_mask_fixupimm_ps(v1, k1, v2, v3, e1) \
    _mm512_mask_fixupimm_round_ps((v1), (k1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fixupimm_ps(k1, v1, v2, v3, e1) \
    _mm512_maskz_fixupimm_round_ps((k1), (v1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fixupimm_ps(v1, v2, v3, e1) \
    _mm512_maskz_fixupimm_ps(_MM_K0_REG, (v1), (v2), (v3), (e1))

#define _mm512_exp2a23_round_ps(v1, e1) \
    _mm512_maskz_exp2a23_round_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_mask_exp2a23_ps(v1, k1, v2) \
    _mm512_mask_exp2a23_round_ps((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_exp2a23_ps(k1, v1) \
    _mm512_maskz_exp2a23_round_ps((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_exp2a23_ps(v1) \
    _mm512_maskz_exp2a23_ps(_MM_K0_REG, (v1))
#define _mm512_exp2a23_round_pd(v1, e1) \
    _mm512_maskz_exp2a23_round_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_mask_exp2a23_pd(v1, k1, v2) \
    _mm512_mask_exp2a23_round_pd((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_exp2a23_pd(k1, v1) \
    _mm512_maskz_exp2a23_round_pd((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_exp2a23_pd(v1) \
    _mm512_maskz_exp2a23_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_fpclass_ps_mask(v1, e1) \
    _mm512_mask_fpclass_ps_mask(_MM_K0_REG, (v1), (e1))
#define _mm512_fpclass_pd_mask(v1, e1) \
    _mm512_mask_fpclass_pd_mask((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_range_round_pd(v1, v2, e1, e2) \
    _mm512_maskz_range_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e1), (e2))
#define _mm512_mask_range_pd(v1, k1, v2, v3, e1) \
    _mm512_mask_range_round_pd((v1), (k1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_range_pd(k1, v1, v2, e1) \
    _mm512_maskz_range_round_pd((k1), (v1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_range_pd(v1, v2, e1) \
    _mm512_maskz_range_pd((__mmask8)_MM_K0_REG, (v1), (v2), (e1))

#define _mm512_range_round_ps(v1, v2, e1, e2) \
    _mm512_maskz_range_round_ps(_MM_K0_REG, (v1), (v2), (e1), (e2))
#define _mm512_mask_range_ps(v1, k1, v2, v3, e1) \
    _mm512_mask_range_round_ps((v1), (k1), (v2), (v3), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_range_ps(k1, v1, v2, e1) \
    _mm512_maskz_range_round_ps((k1), (v1), (v2), (e1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_range_ps(v1, v2, e1) \
    _mm512_maskz_range_ps(_MM_K0_REG, (v1), (v2), (e1))

#define _mm512_madd_epi16(v1, v2) \
    _mm512_maskz_madd_epi16(_MM_K0_REG, (v1), (v2))
#define _mm512_maddubs_epi16(v1, v2) \
    _mm512_maskz_maddubs_epi16((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_packs_epi16(v1, v2) \
    _mm512_maskz_packs_epi16(_MM_K0_REG64, (v1), (v2))
#define _mm512_packs_epi32(v1, v2) \
    _mm512_maskz_packs_epi32((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_packus_epi16(v1, v2) \
    _mm512_maskz_packus_epi16(_MM_K0_REG64, (v1), (v2))
#define _mm512_packus_epi32(v1, v2) \
    _mm512_maskz_packus_epi32((__mmask32)_MM_K0_REG64, (v1), (v2))

#define _mm512_mask_rcp28_ps(v1, k1, v2) \
    _mm512_mask_rcp28_round_ps((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_rcp28_ps(k1, v1) \
    _mm512_maskz_rcp28_round_ps((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_rcp28_round_ps(v1, e1) \
    _mm512_maskz_rcp28_round_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_rcp28_ps(v1) \
    _mm512_maskz_rcp28_ps(_MM_K0_REG, (v1))

#define _mm512_mask_rcp28_pd(v1, k1, v2) \
    _mm512_mask_rcp28_round_pd((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_rcp28_pd(k1, v1) \
    _mm512_maskz_rcp28_round_pd((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_rcp28_round_pd(v1, e1) \
    _mm512_maskz_rcp28_round_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_rcp28_pd(v1) \
    _mm512_maskz_rcp28_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_mask_rsqrt28_ps(v1, k1, v2) \
    _mm512_mask_rsqrt28_round_ps((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_rsqrt28_ps(k1, v1) \
    _mm512_maskz_rsqrt28_round_ps((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_rsqrt28_round_ps(v1, e1) \
    _mm512_maskz_rsqrt28_round_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_rsqrt28_ps(v1) \
    _mm512_maskz_rsqrt28_ps(_MM_K0_REG, (v1))

#define _mm512_mask_rsqrt28_pd(v1, k1, v2) \
    _mm512_mask_rsqrt28_round_pd((v1), (k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_rsqrt28_pd(k1, v1) \
    _mm512_maskz_rsqrt28_round_pd((k1), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_rsqrt28_round_pd(v1, e1) \
    _mm512_maskz_rsqrt28_round_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_rsqrt28_pd(v1) \
    _mm512_maskz_rsqrt28_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_mask3_fmadd_ps(v1, v2, v3, k3) \
    _mm512_mask3_fmadd_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmadd_pd(v1, v2, v3, k3) \
    _mm512_mask3_fmadd_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmsub_ps(v1, v2, v3, k3) \
    _mm512_mask3_fmsub_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmsub_pd(v1, v2, v3, k3) \
    _mm512_mask3_fmsub_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fnmadd_ps(v1, v2, v3, k3) \
    _mm512_mask3_fnmadd_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fnmadd_pd(v1, v2, v3, k3) \
    _mm512_mask3_fnmadd_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fnmsub_ps(v1, v2, v3, k3) \
    _mm512_mask3_fnmsub_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fnmsub_pd(v1, v2, v3, k3) \
    _mm512_mask3_fnmsub_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmaddsub_ps(v1, v2, v3, k3) \
    _mm512_mask3_fmaddsub_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmaddsub_pd(v1, v2, v3, k3) \
    _mm512_mask3_fmaddsub_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmsubadd_ps(v1, v2, v3, k3) \
    _mm512_mask3_fmsubadd_round_ps((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask3_fmsubadd_pd(v1, v2, v3, k3) \
    _mm512_mask3_fmsubadd_round_pd((v1), (v2), (v3), (k3), _MM_FROUND_CUR_DIRECTION)

#define _mm512_mask_fmaddsub_ps(v1, k1, v2, v3) \
    _mm512_mask_fmaddsub_round_ps((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmaddsub_ps(k1, v1, v2, v3) \
    _mm512_maskz_fmaddsub_round_ps((k1), (v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmaddsub_round_ps(v1, v2, v3, e1) \
    _mm512_maskz_fmaddsub_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_fmaddsub_ps(v1, v2, v3) \
    _mm512_maskz_fmaddsub_ps(_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_mask_fmaddsub_pd(v1, k1, v2, v3) \
    _mm512_mask_fmaddsub_round_pd((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmaddsub_pd(k1, v1, v2, v3) \
    _mm512_maskz_fmaddsub_round_pd((k1), (v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmaddsub_round_pd(v1, v2, v3, e1) \
    _mm512_maskz_fmaddsub_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_fmaddsub_pd(v1, v2, v3) \
    _mm512_maskz_fmaddsub_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_mask_fmsubadd_ps(v1, k1, v2, v3) \
    _mm512_mask_fmsubadd_round_ps((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmsubadd_ps(k1, v1, v2, v3) \
    _mm512_maskz_fmsubadd_round_ps((k1), (v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmsubadd_round_ps(v1, v2, v3, e1) \
    _mm512_maskz_fmsubadd_round_ps(_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_fmsubadd_ps(v1, v2, v3) \
    _mm512_maskz_fmsubadd_ps(_MM_K0_REG, (v1), (v2), (v3))
#define _mm512_mask_fmsubadd_pd(v1, k1, v2, v3) \
    _mm512_mask_fmsubadd_round_pd((v1), (k1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_fmsubadd_pd(k1, v1, v2, v3) \
    _mm512_maskz_fmsubadd_round_pd((k1), (v1), (v2), (v3), _MM_FROUND_CUR_DIRECTION)
#define _mm512_fmsubadd_round_pd(v1, v2, v3, e1) \
    _mm512_maskz_fmsubadd_round_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3), (e1))
#define _mm512_fmsubadd_pd(v1, v2, v3) \
    _mm512_maskz_fmsubadd_pd((__mmask8)_MM_K0_REG, (v1), (v2), (v3))

#define _mm512_cmp_epi8_mask(v1, v2, e1) \
    _mm512_mask_cmp_epi8_mask(_MM_K0_REG64, (v1), (v2), (e1))
#define _mm512_cmp_epi16_mask(v1, v2, e1) \
    _mm512_mask_cmp_epi16_mask((__mmask32)_MM_K0_REG64, (v1), (v2), (e1))
#define _mm512_cmp_epi32_mask(v1, v2, e1) \
    _mm512_mask_cmp_epi32_mask(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_cmp_epi64_mask(v1, v2, e1) \
    _mm512_mask_cmp_epi64_mask((__mmask8)_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_cmp_epu8_mask(v1, v2, e1) \
    _mm512_mask_cmp_epu8_mask(_MM_K0_REG64, (v1), (v2), (e1))
#define _mm512_cmp_epu16_mask(v1, v2, e1) \
    _mm512_mask_cmp_epu16_mask((__mmask32)_MM_K0_REG64, (v1), (v2), (e1))
#define _mm512_cmp_epu32_mask(v1, v2, e1) \
    _mm512_mask_cmp_epu32_mask(_MM_K0_REG, (v1), (v2), (e1))
#define _mm512_cmp_epu64_mask(v1, v2, e1) \
    _mm512_mask_cmp_epu64_mask((__mmask8)_MM_K0_REG, (v1), (v2), (e1))

#define _mm512_cmpeq_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpneq_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpge_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpgt_epi32_mask(v1, v2) \
    _mm512_cmp_epi32_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epi32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epi32_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpeq_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpneq_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpge_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpgt_epu32_mask(v1, v2) \
    _mm512_cmp_epu32_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epu32_mask(k1, v1, v2) \
    _mm512_mask_cmp_epu32_mask((k1), (v1), (v2), _MM_CMPINT_GT)

#define _mm512_cmpeq_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epi64_mask(v1, v2) \
        _mm512_cmp_epi64_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epi64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi64_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpeq_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epu64_mask(v1, v2) \
        _mm512_cmp_epu64_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epu64_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu64_mask((k1), (v1), (v2), _MM_CMPINT_NE)

#define _mm512_cmpeq_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epi8_mask(v1, v2) \
        _mm512_cmp_epi8_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epi8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi8_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpeq_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epu8_mask(v1, v2) \
        _mm512_cmp_epu8_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epu8_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu8_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpeq_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epi16_mask(v1, v2) \
        _mm512_cmp_epi16_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epi16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epi16_mask((k1), (v1), (v2), _MM_CMPINT_NE)
#define _mm512_cmpeq_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_EQ)
#define _mm512_mask_cmpeq_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_EQ)
#define _mm512_cmplt_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_LT)
#define _mm512_mask_cmplt_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_LT)
#define _mm512_cmple_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_LE)
#define _mm512_mask_cmple_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_LE)
#define _mm512_cmpgt_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_GT)
#define _mm512_mask_cmpgt_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_GT)
#define _mm512_cmpge_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_GE)
#define _mm512_mask_cmpge_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_GE)
#define _mm512_cmpneq_epu16_mask(v1, v2) \
        _mm512_cmp_epu16_mask((v1), (v2), _MM_CMPINT_NE)
#define _mm512_mask_cmpneq_epu16_mask(k1, v1, v2) \
        _mm512_mask_cmp_epu16_mask((k1), (v1), (v2), _MM_CMPINT_NE)

#define _mm512_test_epi8_mask(v1, v2) \
    _mm512_mask_test_epi8_mask(_MM_K0_REG64, (v1), (v2))
#define _mm512_test_epi16_mask(v1, v2) \
    _mm512_mask_test_epi16_mask((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_testn_epi8_mask(v1, v2) \
    _mm512_mask_testn_epi8_mask(_MM_K0_REG64, (v1), (v2))
#define _mm512_testn_epi16_mask(v1, v2) \
    _mm512_mask_testn_epi16_mask((__mmask32)_MM_K0_REG64, (v1), (v2))
#define _mm512_test_epi32_mask(v1, v2) \
    _mm512_mask_test_epi32_mask(_MM_K0_REG, (v1), (v2))
#define _mm512_test_epi64_mask(v1, v2) \
    _mm512_mask_test_epi64_mask((__mmask8)_MM_K0_REG, (v1), (v2))
#define _mm512_testn_epi32_mask(v1, v2) \
    _mm512_mask_testn_epi32_mask(_MM_K0_REG, (v1), (v2))
#define _mm512_testn_epi64_mask(v1, v2) \
    _mm512_mask_testn_epi64_mask((__mmask8)_MM_K0_REG, (v1), (v2))

#define _mm512_kmov(m) ((__mmask16)(m))
#define _mm512_mask2int(m) ((int)(__mmask16)(m))
#define _mm512_int2mask(i) ((__mmask16)(i))
#define _mm512_kortestz(m1, m2) ((int)_mm512_testz_or_mask16((m1), (m2)))
#define _mm512_kortestc(m1, m2) ((int)_mm512_testz_nor_mask16((m1), (m2)))
#define _mm512_undefined_epi32() _mm512_castps_si512(_mm512_undefined())

#define _mm512_i32logather_epi64(index, addr, scale) \
    _mm512_i32gather_epi64(_mm512_castsi512_si256(index), (addr), (scale))
#define _mm512_mask_i32logather_epi64(v1, k1, index, addr, scale) \
    _mm512_mask_i32gather_epi64((v1), (k1), _mm512_castsi512_si256(index), (addr), (scale))
#define _mm512_i32logather_pd(index, addr, scale) \
    _mm512_i32gather_pd(_mm512_castsi512_si256(index), (addr), (scale))
#define _mm512_mask_i32logather_pd(v1, k1, index, addr, scale) \
    _mm512_mask_i32gather_pd((v1), (k1), _mm512_castsi512_si256(index), (addr), (scale))
#define _mm512_i32loscatter_pd(addr, index, v1, scale) \
    _mm512_i32scatter_pd((addr), _mm512_castsi512_si256(index), (v1), (scale))
#define _mm512_mask_i32loscatter_pd(addr, k1, index, v1, scale) \
    _mm512_mask_i32scatter_pd((addr), (k1), _mm512_castsi512_si256(index), (v1), (scale))

#define _mm512_cvtepi32_pd(v1) \
    _mm512_maskz_cvtepi32_pd((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepu32_pd(v1) \
    _mm512_maskz_cvtepu32_pd((__mmask8)_MM_K0_REG, (v1))

#define _mm512_cvt_roundepi32_ps(v1, e1) \
    _mm512_maskz_cvt_roundepi32_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundepu32_ps(v1, e1) \
    _mm512_maskz_cvt_roundepu32_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundph_ps(v1, e1) \
    _mm512_maskz_cvt_roundph_ps(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundps_ph(v1, e1) \
    _mm512_maskz_cvt_roundps_ph(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundepi64_ps(v1, e1) \
    _mm512_maskz_cvt_roundepi64_ps((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundepu64_ps(v1, e1) \
    _mm512_maskz_cvt_roundepu64_ps((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_cvtepi8_epi32(v1) \
    _mm512_maskz_cvtepi8_epi32(_MM_K0_REG, (v1))
#define _mm512_cvtepi8_epi64(v1) \
    _mm512_maskz_cvtepi8_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepi16_epi32(v1) \
    _mm512_maskz_cvtepi16_epi32(_MM_K0_REG, (v1))
#define _mm512_cvtepi16_epi64(v1) \
    _mm512_maskz_cvtepi16_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepi32_epi8(v1) \
    _mm512_maskz_cvtepi32_epi8(_MM_K0_REG, (v1))
#define _mm512_cvtsepi32_epi8(v1) \
    _mm512_maskz_cvtsepi32_epi8(_MM_K0_REG, (v1))
#define _mm512_cvtusepi32_epi8(v1) \
    _mm512_maskz_cvtusepi32_epi8(_MM_K0_REG, (v1))
#define _mm512_cvtepi32_epi16(v1) \
    _mm512_maskz_cvtepi32_epi16(_MM_K0_REG, (v1))
#define _mm512_cvtsepi32_epi16(v1) \
    _mm512_maskz_cvtsepi32_epi16(_MM_K0_REG, (v1))
#define _mm512_cvtusepi32_epi16(v1) \
    _mm512_maskz_cvtusepi32_epi16(_MM_K0_REG, (v1))
#define _mm512_cvtepi32_epi64(v1) \
    _mm512_maskz_cvtepi32_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepi64_epi8(v1) \
    _mm512_maskz_cvtepi64_epi8((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtsepi64_epi8(v1) \
    _mm512_maskz_cvtsepi64_epi8((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtusepi64_epi8(v1) \
    _mm512_maskz_cvtusepi64_epi8((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepi64_epi16(v1) \
    _mm512_maskz_cvtepi64_epi16((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtsepi64_epi16(v1) \
    _mm512_maskz_cvtsepi64_epi16((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtusepi64_epi16(v1) \
    _mm512_maskz_cvtusepi64_epi16((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepi64_epi32(v1) \
    _mm512_maskz_cvtepi64_epi32((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtsepi64_epi32(v1) \
    _mm512_maskz_cvtsepi64_epi32((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtusepi64_epi32(v1) \
    _mm512_maskz_cvtusepi64_epi32((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepu8_epi32(v1) \
    _mm512_maskz_cvtepu8_epi32(_MM_K0_REG, (v1))
#define _mm512_cvtepu8_epi64(v1) \
    _mm512_maskz_cvtepu8_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepu16_epi32(v1) \
    _mm512_maskz_cvtepu16_epi32(_MM_K0_REG, (v1))
#define _mm512_cvtepu16_epi64(v1) \
    _mm512_maskz_cvtepu16_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvtepu32_epi64(v1) \
    _mm512_maskz_cvtepu32_epi64((__mmask8)_MM_K0_REG, (v1))
#define _mm512_cvt_roundps_epi32(v1, e1) \
    _mm512_maskz_cvt_roundps_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundps_epi32(v1, e1) \
    _mm512_maskz_cvtt_roundps_epi32(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundps_epu32(v1, e1) \
    _mm512_maskz_cvt_roundps_epu32(_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundps_epu32(v1, e1) \
    _mm512_maskz_cvtt_roundps_epu32(_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundpd_epi32(v1, e1) \
    _mm512_maskz_cvt_roundpd_epi32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundpd_epi32(v1, e1) \
    _mm512_maskz_cvtt_roundpd_epi32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundpd_epu32(v1, e1) \
    _mm512_maskz_cvt_roundpd_epu32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundpd_epu32(v1, e1) \
    _mm512_maskz_cvtt_roundpd_epu32((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtepi8_epi16(v1) \
    _mm512_maskz_cvtepi8_epi16((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_cvtepu8_epi16(v1) \
    _mm512_maskz_cvtepu8_epi16((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_cvtepi16_epi8(v1) \
    _mm512_maskz_cvtepi16_epi8((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_cvtsepi16_epi8(v1) \
    _mm512_maskz_cvtsepi16_epi8((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_cvtusepi16_epi8(v1) \
    _mm512_maskz_cvtusepi16_epi8((__mmask32)_MM_K0_REG64, (v1))
#define _mm512_cvt_roundepi64_pd(v1, e1) \
    _mm512_maskz_cvt_roundepi64_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundepu64_pd(v1, e1) \
    _mm512_maskz_cvt_roundepu64_pd((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundpd_epi64(v1, e1) \
    _mm512_maskz_cvt_roundpd_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundpd_epu64(v1, e1) \
    _mm512_maskz_cvt_roundpd_epu64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundpd_epi64(v1, e1) \
    _mm512_maskz_cvtt_roundpd_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundpd_epu64(v1, e1) \
    _mm512_maskz_cvtt_roundpd_epu64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundps_epi64(v1, e1) \
    _mm512_maskz_cvt_roundps_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvt_roundps_epu64(v1, e1) \
    _mm512_maskz_cvt_roundps_epu64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundps_epi64(v1, e1) \
    _mm512_maskz_cvtt_roundps_epi64((__mmask8)_MM_K0_REG, (v1), (e1))
#define _mm512_cvtt_roundps_epu64(v1, e1) \
    _mm512_maskz_cvtt_roundps_epu64((__mmask8)_MM_K0_REG, (v1), (e1))

#define _mm512_cvtepi32_ps(v) \
        _mm512_cvt_roundepi32_ps((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepi32_ps(v1, k1, v2) \
        _mm512_mask_cvt_roundepi32_ps((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepi32_ps(k1, v2) \
        _mm512_maskz_cvt_roundepi32_ps((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtepu32_ps(v) \
        _mm512_cvt_roundepu32_ps((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepu32_ps(v1, k1, v2) \
        _mm512_mask_cvt_roundepu32_ps((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepu32_ps(k1, v2) \
        _mm512_maskz_cvt_roundepu32_ps((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtps_epi32(v) \
        _mm512_cvt_roundps_epi32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtps_epi32(v1, k1, v2) \
        _mm512_mask_cvt_roundps_epi32((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtps_epi32(k1, v2) \
        _mm512_maskz_cvt_roundps_epi32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttps_epi32(v) \
        _mm512_cvtt_roundps_epi32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttps_epi32(v1, k1, v2) \
        _mm512_mask_cvtt_roundps_epi32((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttps_epi32(k1, v2) \
        _mm512_maskz_cvtt_roundps_epi32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtps_epu32(v) \
        _mm512_cvt_roundps_epu32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtps_epu32(v1, k1, v2) \
        _mm512_mask_cvt_roundps_epu32((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtps_epu32(k1, v2) \
        _mm512_maskz_cvt_roundps_epu32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttps_epu32(v) \
        _mm512_cvtt_roundps_epu32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttps_epu32(v1, k1, v2) \
        _mm512_mask_cvtt_roundps_epu32((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttps_epu32(k1, v2) \
        _mm512_maskz_cvtt_roundps_epu32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtpd_epi32(v) \
        _mm512_cvt_roundpd_epi32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtpd_epi32(v1, k1, v2) \
        _mm512_mask_cvt_roundpd_epi32((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtpd_epi32(k1, v2) \
        _mm512_maskz_cvt_roundpd_epi32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttpd_epi32(v) \
        _mm512_cvtt_roundpd_epi32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttpd_epi32(v1, k1, v2) \
        _mm512_mask_cvtt_roundpd_epi32((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttpd_epi32(k1, v2) \
        _mm512_maskz_cvtt_roundpd_epi32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtpd_epu32(v) \
        _mm512_cvt_roundpd_epu32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtpd_epu32(v1, k1, v2) \
        _mm512_mask_cvt_roundpd_epu32((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtpd_epu32(k1, v2) \
        _mm512_maskz_cvt_roundpd_epu32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttpd_epu32(v) \
        _mm512_cvtt_roundpd_epu32((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttpd_epu32(v1, k1, v2) \
        _mm512_mask_cvtt_roundpd_epu32((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttpd_epu32(k1, v2) \
        _mm512_maskz_cvtt_roundpd_epu32((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtph_ps(v1) \
        _mm512_cvt_roundph_ps((v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtph_ps(v1, k, v2) \
        _mm512_mask_cvt_roundph_ps((v1), (k), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtph_ps(k, v1) \
        _mm512_maskz_cvt_roundph_ps((k), (v1), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtps_ph(v1, a) \
        _mm512_cvt_roundps_ph((v1), (a))
#define _mm512_mask_cvtps_ph(v1, k, v2, a) \
        _mm512_mask_cvt_roundps_ph((v1), (k), (v2), (a))
#define _mm512_maskz_cvtps_ph(k, v2, a) \
        _mm512_maskz_cvt_roundps_ph((k), (v2), (a))
#define _mm512_cvtepi64_pd(v) \
        _mm512_cvt_roundepi64_pd((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepi64_pd(v1, k1, v2) \
        _mm512_mask_cvt_roundepi64_pd((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepi64_pd(k1, v2) \
        _mm512_maskz_cvt_roundepi64_pd((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtepu64_pd(v) \
        _mm512_cvt_roundepu64_pd((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepu64_pd(v1, k1, v2) \
        _mm512_mask_cvt_roundepu64_pd((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepu64_pd(k1, v2) \
        _mm512_maskz_cvt_roundepu64_pd((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtpd_epi64(v) \
        _mm512_cvt_roundpd_epi64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtpd_epi64(v1, k1, v2) \
        _mm512_mask_cvt_roundpd_epi64((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtpd_epi64(k1, v2) \
        _mm512_maskz_cvt_roundpd_epi64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtpd_epu64(v) \
        _mm512_cvt_roundpd_epu64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtpd_epu64(v1, k1, v2) \
        _mm512_mask_cvt_roundpd_epu64((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtpd_epu64(k1, v2) \
        _mm512_maskz_cvt_roundpd_epu64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttpd_epi64(v) \
        _mm512_cvtt_roundpd_epi64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttpd_epi64(v1, k1, v2) \
        _mm512_mask_cvtt_roundpd_epi64((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttpd_epi64(k1, v2) \
        _mm512_maskz_cvtt_roundpd_epi64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttpd_epu64(v) \
        _mm512_cvtt_roundpd_epu64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttpd_epu64(v1, k1, v2) \
        _mm512_mask_cvtt_roundpd_epu64((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttpd_epu64(k1, v2) \
        _mm512_maskz_cvtt_roundpd_epu64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtps_epi64(v) \
        _mm512_cvt_roundps_epi64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtps_epi64(v1, k1, v2) \
        _mm512_mask_cvt_roundps_epi64((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtps_epi64(k1, v2) \
        _mm512_maskz_cvt_roundps_epi64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtps_epu64(v) \
        _mm512_cvt_roundps_epu64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtps_epu64(v1, k1, v2) \
        _mm512_mask_cvt_roundps_epu64((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtps_epu64(k1, v2) \
        _mm512_maskz_cvt_roundps_epu64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtepi64_ps(v) \
        _mm512_cvt_roundepi64_ps((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepi64_ps(v1, k1, v2) \
        _mm512_mask_cvt_roundepi64_ps((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepi64_ps(k1, v2) \
        _mm512_maskz_cvt_roundepi64_ps((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvtepu64_ps(v) \
        _mm512_cvt_roundepu64_ps((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvtepu64_ps(v1, k1, v2) \
        _mm512_mask_cvt_roundepu64_ps((v1), (k1), (v2), \
                                      _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvtepu64_ps(k1, v2) \
        _mm512_maskz_cvt_roundepu64_ps((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttps_epi64(v) \
        _mm512_cvtt_roundps_epi64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttps_epi64(v1, k1, v2) \
        _mm512_mask_cvtt_roundps_epi64((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttps_epi64(k1, v2) \
        _mm512_maskz_cvtt_roundps_epi64((k1), (v2), _MM_FROUND_CUR_DIRECTION)
#define _mm512_cvttps_epu64(v) \
        _mm512_cvtt_roundps_epu64((v), _MM_FROUND_CUR_DIRECTION)
#define _mm512_mask_cvttps_epu64(v1, k1, v2) \
        _mm512_mask_cvtt_roundps_epu64((v1), (k1), (v2), \
                                       _MM_FROUND_CUR_DIRECTION)
#define _mm512_maskz_cvttps_epu64(k1, v2) \
        _mm512_maskz_cvtt_roundps_epu64((k1), (v2), _MM_FROUND_CUR_DIRECTION)

#ifdef __cplusplus
};
#endif /* __cplusplus */


#endif /* _ZMMINTRIN_H_INCLUDED */
