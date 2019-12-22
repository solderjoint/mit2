
#ifndef __DIAGNOSTIC_H
#define __DIAGNOSTIC_H

/* **************************************************** *
 *                  SYSTEM-WIDE MACROS
 * **************************************************** */

/* **************************************************** *
 *                MISC BUILD INFORMATION
 * **************************************************** */
#if !defined(BUILD_TIME) && !defined(BUILD_DATE)
#	define BUILD_DATE     __DATE__
#	define BUILD_TIME     __TIME__
#endif

#if !defined(BUILD_VERSION)
#	define VERSION_MAJOR  0
#	define VERSION_MINOR  1
#	define VERSION_PATCH  1
#	define BUILD_VERSION  \
		(10000*(VERSION_MAJOR) + 100*(VERSION_MINOR) + (VERSION_PATCH))
#endif

/* **************************************************** *
 *              USED COMPILER INFORMATION
 * **************************************************** */
#if defined(__GNUC__)
#	define COMPILER_NAME     "ARMGCC"
#	define COMPILER_VERSION  \
		(10000*__GNUC__ + 100*__GNUC_MINOR__ + __GNUC_PATCHLEVEL__)
#elif defined(__ARMCC_VERSION)
#	define COMPILER_NAME     "KEILCC"
#	define COMPILER_VERSION  (__ARMCC_VERSION)
#elif defined(__IAR_SYSTEMS_ICC__) && defined(__VER__)
#	define COMPILER_NAME     "IARCC"
#	define COMPILER_VERSION  (__VER__)
#else
#	define COMPILER_NAME     "UNKNWN"
#	if defined(__VERSION__)
#		define COMPILER_VERSION  (__VERSION__)
#	else
#		define COMPILER_VERSION  (10101)
#	endif
#endif

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__DIAGNOSTIC_H
