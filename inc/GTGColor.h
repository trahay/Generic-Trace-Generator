/**
 *  \file GTGColor.h
 *  \version 0.1
 *  \brief
 *
 *  This file defines some useful colors to use in entity values for GTG.
 *
 */
#ifndef _GTGCOLOR_H_
#define _GTGCOLOR_H_

#include <stdint.h>

/**
 * \defgroup GTGColor Defined colors for GTG
 */

typedef uint32_t gtg_rgb_color_t;

/**
 * \ingroup GTGColor
 * \struct gtg_color
 * \brief This structure defines a color that can be used by GTG.
 */
struct gtg_color {
/*@{*/
	char* color_name; /**< The name of the color (ie. "RED" or "Black",...) */
	gtg_rgb_color_t rgb; /**< RGB code of the color. It should be obtained by calling GTG_COLOR_SET_COLOR(r, g, b). */
/*@{*/
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

/**
 * \ingroup GTGColor
 * \fn GTG_COLOR_GET_BLUE(gtg_rgb_color_t rgb)
 * \brief Return the 1-byte value of the blue component of a rgb color.
 */
static inline uint8_t
GTG_COLOR_GET_BLUE(gtg_rgb_color_t rgb)
{
	return (((rgb) & GTG_COLOR_BLUE_MASK)  >> GTG_COLOR_BLUE_POS );
}

/**
 * \ingroup GTGColor
 * \fn GTG_COLOR_GET_GREEN(gtg_rgb_color_t rgb)
 * \brief Return the 1-byte value of the green component of a rgb color.
 */
static inline uint8_t
GTG_COLOR_GET_GREEN(gtg_rgb_color_t rgb)
{
	return (((rgb) & GTG_COLOR_GREEN_MASK) >> GTG_COLOR_GREEN_POS);
}

/**
 * \ingroup GTGColor
 * \fn GTG_COLOR_GET_RED(gtg_rgb_color_t rgb)
 * \brief Return the 1-byte value of the red component of a rgb color.
 */
static inline uint8_t
GTG_COLOR_GET_RED(gtg_rgb_color_t rgb)
{
	return (((rgb) & GTG_COLOR_RED_MASK)   >> GTG_COLOR_RED_POS  );
}


/**
 * \ingroup GTGColor
 * \fn GTG_COLOR_SET_COLOR(uint8_t r, uint8_t g, uint8_t b)
 * \brief Return the 4-bytes RGB color from 3 1-byte components.
 */
static inline gtg_rgb_color_t
GTG_COLOR_SET_COLOR(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r << GTG_COLOR_RED_POS) |
		(g << GTG_COLOR_GREEN_POS) |
		(b << GTG_COLOR_BLUE_POS));
}


/**
 * \ingroup GTGColor
 * \var GTG_BLACK
 * \brief Default black color. (R,G,B) = (0, 0, 0)
 */
extern gtg_color_t GTG_BLACK;

/**
 * \ingroup GTGColor
 * \var GTG_RED
 * \brief Default red color. (R,G,B) = (255, 0, 0)
 */
extern gtg_color_t GTG_RED;

/**
 * \ingroup GTGColor
 * \var GTG_GREEN
 * \brief Default green color. (R,G,B) = (0, 255, 0)
 */
extern gtg_color_t GTG_GREEN;

/**
 * \ingroup GTGColor
 * \var GTG_BLUE
 * \brief Default blue color. (R,G,B) = (0, 0, 255)
 */
extern gtg_color_t GTG_BLUE;

/**
 * \ingroup GTGColor
 * \var GTG_WHITE
 * \brief Default white color. (R,G,B) = (255, 255, 255)
 */
extern gtg_color_t GTG_WHITE;

/**
 * \ingroup GTGColor
 * \var GTG_TEAL
 * \brief Default teal color. (R,G,B) = (0, 255, 255)
 */
extern gtg_color_t GTG_TEAL;

/**
 * \ingroup GTGColor
 * \var GTG_DARKGREY
 * \brief Default dark grey color. (R,G,B) = (85, 85, 85)
 */
extern gtg_color_t GTG_DARKGREY;

/**
 * \ingroup GTGColor
 * \var GTG_YELLOW
 * \brief Default yellow color. (R,G,B) = (255, 255, 0)
 */
extern gtg_color_t GTG_YELLOW;

/**
 * \ingroup GTGColor
 * \var GTG_PURPLE
 * \brief Default purple color. (R,G,B) = (153, 25, 230)
 */
extern gtg_color_t GTG_PURPLE;

/**
 * \ingroup GTGColor
 * \var GTG_LIGHTBROWN
 * \brief Default light brown color. (R,G,B) = (170, 130, 130)
 */
extern gtg_color_t GTG_LIGHTBROWN;

/**
 * \ingroup GTGColor
 * \var GTG_LIGHTGREY
 * \brief Default light grey color. (R,G,B) = (200, 200, 200)
 */
extern gtg_color_t GTG_LIGHTGREY;

/**
 * \ingroup GTGColor
 * \var GTG_DARKBLUE
 * \brief Default dark blue color. (R,G,B) = (0, 0, 80)
 */
extern gtg_color_t GTG_DARKBLUE;

/**
 * \ingroup GTGColor
 * \var GTG_PINK
 * \brief Default pink color. (R,G,B) = (255, 0, 255)
 */
extern gtg_color_t GTG_PINK;

/**
 * \ingroup GTGColor
 * \var GTG_DARKPINK
 * \brief Default dark pink color. (R,G,B) = (180, 80, 180)
 */
extern gtg_color_t GTG_DARKPINK;

/**
 * \ingroup GTGColor
 * \var GTG_SEABLUE
 * \brief Default sea blue color. (R,G,B) = (25, 128, 200)
 */
extern gtg_color_t GTG_SEABLUE;

/**
 * \ingroup GTGColor
 * \var GTG_KAKI
 * \brief Default kaki color. (R,G,B) = (80, 100, 25)
 */
extern gtg_color_t GTG_KAKI;

/**
 * \ingroup GTGColor
 * \var GTG_REDBLOOD
 * \brief Default red blood color. (R,G,B) = (200, 25, 25)
 */
extern gtg_color_t GTG_REDBLOOD;

/**
 * \ingroup GTGColor
 * \var GTG_BROWN
 * \brief Default brown color. (R,G,B) = (100, 25, 25)
 */
extern gtg_color_t GTG_BROWN;

/**
 * \ingroup GTGColor
 * \var GTG_GRENAT
 * \brief Default grenat color. (R,G,B) = (100, 0, 80)
 */
extern gtg_color_t GTG_GRENAT;

/**
 * \ingroup GTGColor
 * \var GTG_ORANGE
 * \brief Default orange color. (R,G,B) = (255, 160, 0)
 */
extern gtg_color_t GTG_ORANGE;

/**
 * \ingroup GTGColor
 * \var GTG_MAUVE
 * \brief Default mauve color. (R,G,B) = (128, 0, 255)
 */
extern gtg_color_t GTG_MAUVE;

/**
 * \ingroup GTGColor
 * \var GTG_LIGHTPINK
 * \brief Default light pink color. (R,G,B) = (255, 128, 255)
 */
extern gtg_color_t GTG_LIGHTPINK;

void gtg_color_init();
void gtg_color_exit();

gtg_color_t gtg_color_create( const char *name, uint8_t r, uint8_t g, uint8_t b);
void        gtg_color_free(gtg_color_t color);

#endif /* _GTGCOLOR_H_ */
