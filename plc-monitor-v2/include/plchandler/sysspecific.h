#ifndef __SYSSPECIFIC__H__
#define __SYSSPECIFIC__H__
/*
 * ---Linux/QNX specific---
 * 
 * This file is included at the end of  CmpStd.h.
 * It may contain system specific include files etc. 
 * 
 */

#ifndef PTHREAD_STACK_MIN
	#define PTHREAD_STACK_MIN		0x4000
#endif

#define HANDLE2FD(h)	(int)(unsigned long)h
#define FD2HANDLE(fd)	(RTS_HANDLE)(unsigned long)(fd)

#define NSEC_PER_SEC	1000000000
#define USEC_PER_SEC	1000000

/* Debugging  ---------------------------------------------------------------------------------------- */
#ifdef RTS_DEBUG

#ifdef RTS_ASSERT
  #undef RTS_ASSERT
#endif
 #define RTS_ASSERT(expr) \
        if (!(expr)) DEBUGP("ASSERT: '%s'\n", #expr)

 #ifdef PLC_HANDLER
  #define DEBUGP(fmt, args...) \
        printf("%s[%d]: " fmt "\n", __FUNCTION__, __LINE__, ## args)
 #endif

#else
 #ifdef PLC_HANDLER
  #define DEBUGP(fmt, args...)
 #endif
#endif

#ifndef PLC_HANDLER

 extern int b_debug;

 #define DEBUGP(fmt, args...) \
        if(b_debug)printf("%s[%d]: " fmt "\n", __FUNCTION__, __LINE__, ## args)
 #define DEBUGP2(fmt, args...) \
        if(b_debug >= 2)printf("%s[%d]: " fmt "\n", __FUNCTION__, __LINE__, ## args)
#else
 #define DEBUGP2(fmt, args...)
#endif
/* --------------------------------------------------------------------------------------------------- */

#if defined(LINUX_SL_PRODUCT) && !defined(TRG_64BIT) 
	#define memcpy(a,b,c) CMUtlMemCpy(a,b,c)
#endif
#if defined (TRG_ARM) && defined (TRG_64BIT)
#define SYSINTERNALLIB_GENERATE_EXCEPTION(ulException)		{\
																RegContext Context;\
																ExceptionCode Exception;\
																Context.IP= (RTS_UINTPTR)__builtin_return_address (0);\
																Context.SP= (RTS_UINTPTR)__builtin_frame_address (1);\
																Context.BP= (RTS_UINTPTR)__builtin_frame_address (1);\
																Exception.ulCode = ulException;\
																Exception.bOSException = 0;\
																CAL_SysExceptGenerateException2(RTS_INVALID_HANDLE, Exception, Context, 1);\
															}
#endif

extern int rts_system(const char *command);

#if defined (TRG_PPC) 
#define NO_CAS64_SUPPORT
#endif

#ifdef NO_CAS_SUPPORT

#ifdef TRG_64BIT
#error 64bit CPUs support CompareAndSwap - remove NO_CAS_SUPPORT compile flag
#endif

#define NO_CAS64_SUPPORT

#if defined(TRG_X86)
	#define SYS_ATOMIC_COMPARE_AND_SWAP(piValue, iExchangeValue, iCompareValue, Result) \
		do { \
			RTS_I32 iPrev = 0; \
			__asm__ __volatile__ ( \
				"mov %3, %%eax\n" \
				"lock cmpxchg %2, (%1)\n" \
				"mov %%eax, %0\n" \
				: "=&r"(iPrev)\
				: "r" (piValue), "r" (iExchangeValue), "r" (iCompareValue) \
				: "cc", "memory", "%eax");	\
			if (iPrev == iCompareValue) \
				Result = ERR_OK; \
			else \
				Result = ERR_FAILED; \
		} while (0)

#elif ((defined(TRG_ARM) && (RTS_ARM_ARCH_VERSION >= 6)) || defined (TRG_CORTEX))
	#define SYS_ATOMIC_COMPARE_AND_SWAP(piValue, iExchangeValue, iCompareValue, Result) \
		do { \
			RTS_I32 iPrev = 0; \
			RTS_I32 iStat = 1; \
			__asm__ __volatile__ ( \
						"ldrex %4, [%1]\n" \
						"cmp %3, %4\n" \
						"bne 1f\n" \
						"strex %0, %2, [%1]\n" \
						"1:\n" \
						: "=&r" (iStat) \
						: "r" (piValue), "r" (iExchangeValue), "r" (iCompareValue), "r" (iPrev) \
						: "cc", "memory"); \
			if (iStat == 0) \
				Result = ERR_OK; \
			else \
				Result = ERR_FAILED; \
		} while(0)

#elif defined(TRG_PPC)
	#define SYS_ATOMIC_COMPARE_AND_SWAP(piValue, iExchangeValue, iCompareValue, Result) \
		do { \
			RTS_I32 iPrev; \
			RTS_I32 iStat; \
			__asm__ __volatile__ ( \
						"li     %0, 1\n" \
						"lwarx	%4, 0, %1\n" \
						"cmpw	%3, %4\n" \
						"bne    1f\n" \
						"stwcx.	%2, 0, %1\n" \
						"bne-	1f\n" \
						"li     %0, 0\n" \
						"1:\n" \
						: "=&r" (iStat) \
						: "r" (piValue), "r" (iExchangeValue), "r" (iCompareValue), "r" (iPrev) \
						: "cc", "memory"); \
			if (iStat == 0) \
				Result = ERR_OK; \
			else \
				Result = ERR_FAILED; \
		} while(0)

#endif


#else //NO_CAS_SUPPORT

#define SYS_ATOMIC_COMPARE_AND_SWAP(piValue, iExchangeValue, iCompareValue, Result) \
	do { \
		if (__sync_bool_compare_and_swap((unsigned int *)piValue, (unsigned int)iCompareValue, (unsigned int)iExchangeValue)) \
			Result = ERR_OK; \
		else \
			Result = ERR_FAILED; \
	} while (0)
#endif

#ifndef NO_CAS64_SUPPORT
#define SYS_ATOMIC_COMPARE_AND_SWAP64(pi64Value, i64ExchangeValue, i64CompareValue, Result) \
			do { \
				if (__sync_bool_compare_and_swap((unsigned long long *)pi64Value, (unsigned long long)i64CompareValue, (unsigned long long)i64ExchangeValue)) \
					Result = ERR_OK; \
				else \
					Result = ERR_FAILED; \
			} while (0)
#endif

#endif	/*__SYSSPECIFIC__H__*/
