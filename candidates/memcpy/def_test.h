/*
 * Copyright (c) 2006, Advanced Micro Devices, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met:
 * 
 *    * Redistributions of source code must retain the above copyright 
 *      notice, this list of conditions and the following disclaimer.
 *    * Neither the name of Advanced Micro Devices, Inc. nor the names
 *      of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written
 *      permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _DEF_TEST_H_
#define _DEF_TEST_H_
/**
 * \file memcpy/def_test.h
 * \brief
 * This file defines the functions types and names for the memcpy performance
 * optimization test. \ref memcpy_test
 */
/**
 * \defgroup memcpy_test Memcpy performance test
 * \brief
 * The performance and optimzation test for memcpy.  Uses the framework
 * provided by \ref test and \ref loop_test
 *
 * \section memcpy_status Status
 *
 * The testing and optimization has been completed
 *
 * \section memcpy_results Results
 *
 *	- Winner: lx_memcpy_movq_r()
 *	- Honorable mention: lx_memcpy_si1() best non-mmx implementation
 *
 * \section memcpy_candidates Candidates
 *
 *	- \ref glibc_memcpy()
 *	- \ref lx_memcpy_movq()
 *	- \ref lx_memcpy_movq_r()
 *	- \ref lx_memcpy_noleal()
 *	- \ref lx_memcpy_noleal_128()
 *	- \ref lx_memcpy_si0()
 *	- \ref lx_memcpy_si1()
 *
 * \section memcpy_notes Notes
 *	While the lx_memcpy_movq_r() is the winner, it may not be acceptable
 *	as a glibc function as the kernel uses only light weight threading
 *	and doesn't protect the FPU state.
 *
 * \section memcpy_todo TODO
 * 	- convert utest to use def_test.h (to keep utest and ptest in sync)
 *
 */
 
#include "def_test_def.h"


/** assembly generated by the 586 inline glibc implementation */
void *glibc_memcpy(void *dest, const void *src, size_t n);
/** unrolled copy using movq and prefetch */
void *lx_memcpy_movq(void *dest, const void *src, size_t n);
/** unrolled copy using movq and prefetch, with optimized preamble. \b Best. */
void *lx_memcpy_movq_r(void *dest, const void *src, size_t n);
/** basic unrolled copy using rep;movsl and prefetch */
void *lx_memcpy_noleal(void *dest, const void *src, size_t n);
/** basic unrolled copy using rep;movsl and prefetch, longer stride */
void *lx_memcpy_noleal_128(void *dest, const void *src, size_t n);
/** 'noleal' optimization pass 0 */
void *lx_memcpy_si0(void *dest, const void *src, size_t n);
/** 'noleal' optimization pass 1, add short buffer escape a preamble
 *  reordering to avoid compounding mov and cld stalls.  \b Best \b non-MMX.
 */
void *lx_memcpy_si1(void *dest, const void *src, size_t n);

/**  The function pointer type for the function under test */
typedef void *(*T_def_Fn)(void *, const void *, size_t );


/**
 * \brief
 * the structure that \ref loop_test uses to define the function under test
 */
typedef	struct {
		T_def_Fn f; /*!<pointer to a function to test */
		char *name; /*!<human readable name for function to test */
} T_def_test;

T_def_test def_tests[] = {
		{memcpy, "local" },
		{glibc_memcpy, "libc586"},
		/*{lx_memcpy_noleal, "lx64" }, */
		/*{lx_memcpy_si0, "lx64si0" }, /* Worse than si1 */
		{lx_memcpy_si1, "lx64si1" },
		/* {lx_memcpy_noleal_128, "lx128" }, */
		{lx_memcpy_movq, "movq" },
		{lx_memcpy_movq_r, "movq_r" },
		/* {lx_memcpy_movq_r2, "movq_r2" } /* Worse than _r */
};

/** The number of entries in def_tests (above) */
#define NUM_TEST (sizeof(def_tests)/sizeof(T_def_test))

#define DEF_NEXT(a,tc,c,s,sp,d,dp) DEF_NEXT_BOTH_DEF(a,tc,c,s,sp,d,dp) 

#define DEF_RUN(f,c,sp,dp) f(dp,sp,c)

#endif