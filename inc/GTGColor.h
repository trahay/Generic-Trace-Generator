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
 * \ingroup color
 * \brief Black color
 */
#define GTG_BLACK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 0)
#define GTG_COLOR_BLACK(var) \
	gtg_color_t var = {.color_name = "BLACK", .rgb = GTG_BLACK_RGB}

/**
 * \ingroup color
 * \brief Red color
 */
#define GTG_RED_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 0, 0)
#define GTG_COLOR_RED(var) \
	gtg_color_t var = {.color_name = "RED", .rgb = GTG_RED_RGB}

/**
 * \ingroup color
 * \brief Green color
 */
#define GTG_GREEN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 255, 0)
#define GTG_COLOR_GREEN(var) \
	gtg_color_t var = {.color_name = "GREEN", .rgb = GTG_GREEN_RGB}

/**
 * \ingroup color
 * \brief Blue color
 */
#define GTG_BLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 255)
#define GTG_COLOR_BLUE(var) \
	gtg_color_t var = {.color_name = "BLUE", .rgb = GTG_BLUE_RGB}

/**
 * \ingroup color
 * \brief White color
 */
#define GTG_WHITE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 255, 255)
#define GTG_COLOR_WHITE(var) \
	gtg_color_t var = {.color_name = "WHITE", .rgb = GTG_WHITE_RGB}

/**
 * \ingroup color
 * \brief Teal color
 */
#define GTG_TEAL_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 255, 255)
#define GTG_COLOR_TEAL(var) \
	gtg_color_t var = {.color_name = "TEAL", .rgb = GTG_TEAL_RGB}

/**
 * \ingroup color
 * \brief Dark grey color
 */
#define GTG_DARKGREY_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(85, 85, 85)
#define GTG_COLOR_DARKGREY(var) \
	gtg_color_t var = {.color_name = "DARKGREY", .rgb = GTG_DARKGREY_RGB}

/**
 * \ingroup color
 * \brief Yellow color
 */
#define GTG_YELLOW_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 255, 0)
#define GTG_COLOR_YELLOW(var) \
	gtg_color_t var = {.color_name = "YELLOW", .rgb = GTG_YELLOW_RGB}

/**
 * \ingroup color
 * \brief Purple color
 */
#define GTG_PURPLE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(153, 25, 230)
#define GTG_COLOR_PURPLE(var) \
	gtg_color_t var = {.color_name = "PURPLE", .rgb = GTG_PURPLE_RGB}

/**
 * \ingroup color
 * \brief Light brown color
 */
#define GTG_LIGHTBROWN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(170, 130, 130)
#define GTG_COLOR_LIGHTBROWN(var) \
	gtg_color_t var = {.color_name = "LIGHTBROWN", .rgb = GTG_LIGHTBROWN_RGB}


/**
 * \ingroup color
 * \brief Light grey color
 */
#define GTG_LIGHTGREY_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(200, 200, 200)
#define GTG_COLOR_LIGHTGREY(var) \
	gtg_color_t var = {.color_name = "LIGHTGREY", .rgb = GTG_LIGHTGREY_RGB}


/**
 * \ingroup color
 * \brief Dark blue color
 */
#define GTG_DARKBLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 80)
#define GTG_COLOR_DARKBLUE(var) \
	gtg_color_t var = {.color_name = "DARKBLUE", .rgb = GTG_DARKBLUE_RGB}


/**
 * \ingroup color
 * \brief Pink color
 */
#define GTG_PINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 0, 255)
#define GTG_COLOR_PINK(var) \
	gtg_color_t var = {.color_name = "PINK", .rgb = GTG_PINK_RGB}

/**
 * \ingroup color
 * \brief Dark pink color
 */
#define GTG_DARKPINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(180, 80, 180)
#define GTG_COLOR_DARKPINK(var) \
	gtg_color_t var = {.color_name = "DARKPINK", .rgb = GTG_DARKPINK_RGB}

/**
 * \ingroup color
 * \brief Sea blue color
 */
#define GTG_SEABLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(25, 128, 200)
#define GTG_COLOR_SEABLUE(var) \
	gtg_color_t var = {.color_name = "SEABLUE", .rgb = GTG_SEABLUE_RGB}


/**
 * \ingroup color
 * \brief Kaki color
 */
#define GTG_KAKI_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(80, 100, 25)
#define GTG_COLOR_KAKI(var) \
	gtg_color_t var = {.color_name = "KAKI", .rgb = GTG_KAKI_RGB}


/**
 * \ingroup color
 * \brief Red blood color
 */
#define GTG_REDBLOOD_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(200, 25, 25)
#define GTG_COLOR_REDBLOOD(var) \
	gtg_color_t var = {.color_name = "REDBLOOD", .rgb = GTG_REDBLOOD_RGB}

/**
 * \ingroup color
 * \brief Brown color
 */
#define GTG_BROWN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(100, 25, 25)
#define GTG_COLOR_BROWN(var) \
	gtg_color_t var = {.color_name = "BROWN", .rgb = GTG_BROWN_RGB}


/**
 * \ingroup color
 * \brief Grenat color
 */
#define GTG_GRENAT_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(100, 0, 80)
#define GTG_COLOR_GRENAT(var) \
	gtg_color_t var = {.color_name = "GRENAT", .rgb = GTG_GRENAT_RGB}


/**
 * \ingroup color
 * \brief Orange color
 */
#define GTG_ORANGE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 160, 0)
#define GTG_COLOR_ORANGE(var) \
	gtg_color_t var = {.color_name = "ORANGE", .rgb = GTG_ORANGE_RGB}


/**
 * \ingroup color
 * \brief Mauve color
 */
#define GTG_MAUVE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(128, 0, 255)
#define GTG_COLOR_MAUVE(var) \
	gtg_color_t var = {.color_name = "MAUVE", .rgb = GTG_MAUVE_RGB}


/**
 * \ingroup color
 * \brief Light pink color
 */
#define GTG_LIGHTPINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 128, 255)
#define GTG_COLOR_LIGHTPINK(var) \
	gtg_color_t var = {.color_name = "LIGHTPINK", .rgb = GTG_LIGHTPINK_RGB}

/**
 * declare a default gtg_color_t variables (GTG_BLACK, GTG_RED, etc.)
 */
static GTG_COLOR_BLACK      (GTG_BLACK);
static GTG_COLOR_RED        (GTG_RED);
static GTG_COLOR_GREEN      (GTG_GREEN);
static GTG_COLOR_BLUE       (GTG_BLUE);
static GTG_COLOR_WHITE      (GTG_WHITE);
static GTG_COLOR_TEAL       (GTG_TEAL);
static GTG_COLOR_DARKGREY   (GTG_DARKGREY);
static GTG_COLOR_YELLOW     (GTG_YELLOW);
static GTG_COLOR_PURPLE     (GTG_PURPLE);
static GTG_COLOR_LIGHTBROWN (GTG_LIGHTBROWN);
static GTG_COLOR_LIGHTGREY  (GTG_LIGHTGREY);
static GTG_COLOR_DARKBLUE   (GTG_DARKBLUE);
static GTG_COLOR_PINK       (GTG_PINK);
static GTG_COLOR_DARKPINK   (GTG_DARKPINK);
static GTG_COLOR_SEABLUE    (GTG_SEABLUE);
static GTG_COLOR_KAKI       (GTG_KAKI);
static GTG_COLOR_REDBLOOD   (GTG_REDBLOOD);
static GTG_COLOR_BROWN      (GTG_BROWN);
static GTG_COLOR_GRENAT     (GTG_GRENAT);
static GTG_COLOR_ORANGE     (GTG_ORANGE);
static GTG_COLOR_MAUVE      (GTG_MAUVE);
static GTG_COLOR_LIGHTPINK  (GTG_LIGHTPINK);



#endif
