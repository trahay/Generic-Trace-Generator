/**
 *  \file GTGColor.h
 *  \version 0.1
 *  \brief
 *
 *  GTG is a file that defines some usefull color to use in entity value for GTG.
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

#ifndef _GTGCOLOR_
#define _GTGCOLOR_

#include <stdint.h>

/**
 * \defgroup GTGColor Defined colors for GTG
 */

typedef uint32_t gtg_rgb_color_t;

struct gtg_color {
	char* color_name;
	gtg_rgb_color_t rgb;
};
typedef struct gtg_color* gtg_color_t;

/* In a 4-byte value, the first byte corresponds to blue,
 * the second to green, the third to red. The 4th byte is 
 * unused (for now). ie. a color is represented as follows:
 * 0x00rrggbb
 */
#define GTG_COLOR_BLUE_POS  0
#define GTG_COLOR_GREEN_POS 8
#define GTG_COLOR_RED_POS   16

#define GTG_COLOR_BLUE_MASK  (0x000000ff << GTG_COLOR_BLUE_POS)
#define GTG_COLOR_GREEN_MASK (0x000000ff << GTG_COLOR_GREEN_POS)
#define GTG_COLOR_RED_MASK   (0x000000ff << GTG_COLOR_RED_POS)

/* returns the 1-byte value of the blue component of a rgb color */
#define GTG_COLOR_GET_BLUE(rgb)  (((rgb) & GTG_COLOR_BLUE_MASK)  >> GTG_COLOR_BLUE_POS )
/* returns the 1-byte value of the green component of a rgb color */
#define GTG_COLOR_GET_GREEN(rgb) (((rgb) & GTG_COLOR_GREEN_MASK) >> GTG_COLOR_GREEN_POS)
/* returns the 1-byte value of the red component of a rgb color */
#define GTG_COLOR_GET_RED(rgb)   (((rgb) & GTG_COLOR_RED_MASK)   >> GTG_COLOR_RED_POS  )

/* returns the 4-bytes RGB color from 3 1-byte components */
#define GTG_COLOR_SET_COLOR(r,g,b)				\
	((r << GTG_COLOR_RED_POS) |				\
	 (g << GTG_COLOR_GREEN_POS) |				\
	 (b << GTG_COLOR_BLUE_POS))

/**
 * declare a default gtg_color_t variables (GTG_BLACK, GTG_RED, etc.)
 */
extern gtg_color_t GTG_BLACK;
extern gtg_color_t GTG_RED;
extern gtg_color_t GTG_GREEN;
extern gtg_color_t GTG_BLUE;
extern gtg_color_t GTG_WHITE;
extern gtg_color_t GTG_TEAL;
extern gtg_color_t GTG_DARKGREY;
extern gtg_color_t GTG_YELLOW;
extern gtg_color_t GTG_PURPLE;
extern gtg_color_t GTG_LIGHTBROWN;
extern gtg_color_t GTG_LIGHTGREY;
extern gtg_color_t GTG_DARKBLUE;
extern gtg_color_t GTG_PINK;
extern gtg_color_t GTG_DARKPINK;
extern gtg_color_t GTG_SEABLUE;
extern gtg_color_t GTG_KAKI;
extern gtg_color_t GTG_REDBLOOD;
extern gtg_color_t GTG_BROWN;
extern gtg_color_t GTG_GRENAT;
extern gtg_color_t GTG_ORANGE;
extern gtg_color_t GTG_MAUVE;
extern gtg_color_t GTG_LIGHTPINK;


void gtg_color_init();

#endif
