#ifndef _GTGCOLOR_
#define _GTGCOLOR_

#include <stdint.h>

/**
 * \defgroup color Defined colors for GTG
 */

typedef uint32_t gtg_rgb_color_t;

typedef struct gtg_color {
	const char* color_name;
	gtg_rgb_color_t rgb;
} gtg_color_t;

/* In a 4-byte value, the first byte corresponds to blue,
 * the second to green, the third to red. The 4th byte is 
 * unused (for now). ie. a color is represented as follows:
 * 0x00rrggbb
 */
#define GTG_COLOR_BLUE_POS  0
#define GTG_COLOR_GREEN_POS 1
#define GTG_COLOR_RED_POS   2

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
 * \ingroup color
 * \brief Black color
 */
#define GTG_BLACK      0
#define GTG_BLACK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 0)
#define GTG_COLOR_BLACK(var) \
	gtg_color_t var = {.color_name = "BLACK"; rgb = GTG_BLACK_RGB}

/**
 * \ingroup color
 * \brief Red color
 */
#define GTG_RED        1
#define GTG_RED_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 0, 0)
#define GTG_COLOR_RED(var) \
	gtg_color_t var = {.color_name = "RED"; rgb = GTG_RED_RGB}

/**
 * \ingroup color
 * \brief Green color
 */
#define GTG_GREEN      2
#define GTG_GREEN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 255, 0)
#define GTG_COLOR_GREEN(var) \
	gtg_color_t var = {.color_name = "GREEN"; rgb = GTG_GREEN_RGB}

/**
 * \ingroup color
 * \brief Blue color
 */
#define GTG_BLUE       3
#define GTG_BLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 255)
#define GTG_COLOR_BLUE(var) \
	gtg_color_t var = {.color_name = "BLUE"; rgb = GTG_BLUE_RGB}

/**
 * \ingroup color
 * \brief White color
 */
#define GTG_WHITE      5
/* todo: get the RGB value for the other colors */
#define GTG_WHITE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 255, 255)
#define GTG_COLOR_WHITE(var) \
	gtg_color_t var = {.color_name = "WHITE"; rgb = GTG_WHITE_RGB}

/**
 * \ingroup color
 * \brief Teal color
 */
#define GTG_TEAL       6
/**
 * \ingroup color
 * \brief Dark grey color
 */
#define GTG_DARKGREY   7
/**
 * \ingroup color
 * \brief Yellow color
 */
#define GTG_YELLOW     8
/**
 * \ingroup color
 * \brief Purple color
 */
#define GTG_PURPLE     9
/**
 * \ingroup color
 * \brief Light brown color
 */
#define GTG_LIGHTBROWN 10
/**
 * \ingroup color
 * \brief Light grey color
 */
#define GTG_LIGHTGREY  11
/**
 * \ingroup color
 * \brief Dark blue color
 */
#define GTG_DARKBLUE   12
/**
 * \ingroup color
 * \brief Pink color
 */
#define GTG_PINK       13
/**
 * \ingroup color
 * \brief Dark pink color
 */
#define GTG_DARKPINK   14
/**
 * \ingroup color
 * \brief Sea blue color
 */
#define GTG_SEABLUE    15
/**
 * \ingroup color
 * \brief Kaki color
 */
#define GTG_KAKI       16
/**
 * \ingroup color
 * \brief Red blood color
 */
#define GTG_REDBLOOD   17
/**
 * \ingroup color
 * \brief Brown color
 */
#define GTG_BROWN      18
/**
 * \ingroup color
 * \brief Grenat color
 */
#define GTG_GRENAT     19
/**
 * \ingroup color
 * \brief Orange color
 */
#define GTG_ORANGE     20
/**
 * \ingroup color
 * \brief Mauve color
 */
#define GTG_MAUVE      21
/**
 * \ingroup color
 * \brief Light pink color
 */
#define GTG_LIGHTPINK  22



#endif
