/**
 *  \file GTGPaje.h
 *  \version 0.1
 *  \brief
 *
 *  pajeColor is a file that defines function that manipulate colors.
 *
 */

#ifndef _GTG_PAJE_H_
#define _GTG_PAJE_H_

typedef char* paje_color_t;

/**
 * \ingroup cpaje
 * \fn const paje_color_t Paje_get_color(gtg_color_t color)
 * \brief Converts a GTG color into a PAJE color.
 * \param color GTG color to convert
 * \return The PAJE color
 */
const paje_color_t Paje_get_color(gtg_color_t p_color);

#include "GTGPaje_Basic.h"

#endif /* _GTG_PAJE_H_ */
