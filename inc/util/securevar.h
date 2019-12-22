/*
 * Secure variable manipulation macros
 * (basically triples a single variable)
 */

#ifndef __SECUREVAR_H
#define __SECUREVAR_H

/* **************************************************** *
 *               INCLUDED HEADERS SECTION
 * **************************************************** */

/* **************************************************** *
 *             SECURE VAR GET/SET WRAPPERS
 * **************************************************** */
#define svar_get(VAR, XX) { \
	if (VAR == VAR##1) (XX) = VAR; \
	else if (VAR##1 == VAR##2) (XX) = VAR##1; \
	else if (VAR == VAR##2) (XX) = VAR##2; \
	else (XX) = 0; \
}

#define svar_set(VAR, XX) { \
	/*(VAR) = (XX);*/ \
	VAR = (XX); \
	VAR##1 = (XX); \
	VAR##2 = (XX); \
}

/* **************************************************** *
 *          SECURE VAR CHECK AND CORRECT MACRO
 * **************************************************** */
#define svar_check(VAR) { \
	if ((VAR == VAR##1) && (VAR##1 != VAR##2)) \
		svar_set(VAR, VAR); \
	else if ((VAR##1 == VAR##2) && (VAR != VAR##1)) \
		svar_set(VAR, VAR##1); \
	else if ((VAR == VAR##2) && (VAR##1 != VAR##2)) \
		svar_set(VAR, VAR##2); \
	else svar_set(VAR, 0); \
}

/* **************************************************** *
 *              SECURE VAR INITIALIZATION
 * **************************************************** */
#define svar_init(TYPE, VAR, XX) \
	/*TYPE VAR = (XX);*/ \
	TYPE VAR = (XX); \
	TYPE VAR##1 = (XX); \
	TYPE VAR##2 = (XX)

/* **************************************************** *
 *              END OF HEADER DECLARATION
 * **************************************************** */
#endif //__SECUREVAR_H
