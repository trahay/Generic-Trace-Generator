/**
 *  \file pajeColor.h
 *  \version 0.1
 *  \brief
 *
 *  pajeColor is a file that defines function that manipulate colors.
 *

 */

#ifndef _PAJECOLOR_
#define _PAJECOLOR_

#define _GNU_SOURCE
#include <stdio.h>
#include "GTGBasic1.h"

typedef char* paje_color_t;

/**
 * \ingroup cpaje
 * \fn const paje_color_t Paje_get_color(gtg_color_t color)
 * \brief Converts a GTG color into a PAJE color.
 * \param color GTG color to convert
 * \return The PAJE color
 */
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
