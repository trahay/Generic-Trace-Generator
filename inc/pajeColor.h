/**
 *  \file pajeColor.h
 *  \version 0.1
 *  \brief
 *
 *  pajeColor is a file that defines some usefull color to use in entity value for Paje. They are define in a RGB style.
 *
 *  \authors
 *    Developpers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - kevin.coulomb@gmail.com \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 *
 */

#ifndef _PAJECOLOR_
#define _PAJECOLOR_

#define _GNU_SOURCE
#include <stdio.h>


/**
 * \defgroup PajeColor Defined colors for Paje
 */

/**
 * \ingroup color
 * \brief Red color
 */
#define P_RED        "1.0 0.0 0.0"
/**
 * \ingroup color
 * \brief Green color
 */
#define P_GREEN      "0.0 1.0 0.0"
/**
 * \ingroup color
 * \brief Blue color
 */
#define P_BLUE       "0.0 0.0 1.0"
/**
 * \ingroup color
 * \brief Black color
 */
#define P_BLACK      "0.0 0.0 0.0"
/**
 * \ingroup color
 * \brief White color
 */
#define P_WHITE      "1.0 1.0 1.0"
/**
 * \ingroup color
 * \brief Teal color
 */
#define P_TEAL       "0.0 1.0 1.0"
/**
 * \ingroup color
 * \brief Dark grey color
 */
#define P_DARKGREY   "0.3 0.3 0.3"
/**
 * \ingroup color
 * \brief Yellow color
 */
#define P_YELLOW     "0.7 0.7 0.0"
/**
 * \ingroup color
 * \brief Purple color
 */
#define P_PURPLE     "0.6 0.1 0.9"   
/**
 * \ingroup color
 * \brief Light brown color
 */
#define P_LIGHTBROWN "0.7 0.5 0.5"
/**
 * \ingroup color
 * \brief Light grey color
 */
#define P_LIGHTGREY  "0.8 0.8 0.8"
/**
 * \ingroup color
 * \brief Dark blue color
 */
#define P_DARKBLUE   "0.0 0.0 0.3"
/**
 * \ingroup color
 * \brief Pink color
 */
#define P_PINK       "1.0 0.0 1.0"
/**
 * \ingroup color
 * \brief Dark pink color
 */
#define P_DARKPINK   "0.8 0.3 0.8"
/**
 * \ingroup color
 * \brief Sea blue color
 */
#define P_SEABLUE    "0.1 0.5 0.8"
/**
 * \ingroup color
 * \brief Kaki color
 */
#define P_KAKI       "0.3 0.4 0.1"
/**
 * \ingroup color
 * \brief Red blood color
 */
#define P_REDBLOOD   "0.8 0.1 0.1"
/**
 * \ingroup color
 * \brief Brown color
 */
#define P_BROWN      "0.4 0.1 0.1"
/**
 * \ingroup color
 * \brief Grenat color
 */
#define P_GRENAT     "0.4 0.0 0.3"
/**
 * \ingroup color
 * \brief Orange color
 */
#define P_ORANGE     "1.0 0.6 0.0"
/**
 * \ingroup color
 * \brief Mauve color
 */
#define P_MAUVE      "0.5 0.0 1.0"
/**
 * \ingroup color
 * \brief Light pink color
 */
#define P_LIGHTPINK  "1.0 0.5 1.0"

typedef char* paje_color_t;

static inline
const paje_color_t Paje_get_color(gtg_color_t p_color) {
	/* todo */
	paje_color_t res = NULL;
	asprintf(&res, "%1.1f %1.1f %1.1f", 
		 (float)GTG_COLOR_GET_RED(p_color->rgb)/256,
		 (float)GTG_COLOR_GET_GREEN(p_color->rgb)/256,
		 (float)GTG_COLOR_GET_BLUE(p_color->rgb)/256);
	return res;
}

#endif
