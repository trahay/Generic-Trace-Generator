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
