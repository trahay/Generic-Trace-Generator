/**
 *  \file GTGColor.c
 *  \version 0.1
 *  \brief
 *
 *  This file defines some basic colors to use in entity values for GTG.
 *
 */
#include <string.h>
#include <stdlib.h>
#include "GTGColor.h"

#define NB_DEFAULT_COLOR 22
struct gtg_color __gtg_colors[NB_DEFAULT_COLOR];

gtg_color_t GTG_BLACK;
gtg_color_t GTG_RED;
gtg_color_t GTG_GREEN;
gtg_color_t GTG_BLUE;
gtg_color_t GTG_WHITE;
gtg_color_t GTG_TEAL;
gtg_color_t GTG_DARKGREY;
gtg_color_t GTG_YELLOW;
gtg_color_t GTG_PURPLE;
gtg_color_t GTG_LIGHTBROWN;
gtg_color_t GTG_LIGHTGREY;
gtg_color_t GTG_DARKBLUE;
gtg_color_t GTG_PINK;
gtg_color_t GTG_DARKPINK;
gtg_color_t GTG_SEABLUE;
gtg_color_t GTG_KAKI;
gtg_color_t GTG_REDBLOOD;
gtg_color_t GTG_BROWN;
gtg_color_t GTG_GRENAT;
gtg_color_t GTG_ORANGE;
gtg_color_t GTG_MAUVE;
gtg_color_t GTG_LIGHTPINK;


/**
 * \ingroup color
 * \brief Black color
 */
#define GTG_BLACK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 0)

/**
 * \ingroup color
 * \brief Red color
 */
#define GTG_RED_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 0, 0)

/**
 * \ingroup color
 * \brief Green color
 */
#define GTG_GREEN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 255, 0)

/**
 * \ingroup color
 * \brief Blue color
 */
#define GTG_BLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 255)

/**
 * \ingroup color
 * \brief White color
 */
#define GTG_WHITE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 255, 255)

/**
 * \ingroup color
 * \brief Teal color
 */
#define GTG_TEAL_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 255, 255)

/**
 * \ingroup color
 * \brief Dark grey color
 */
#define GTG_DARKGREY_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(85, 85, 85)

/**
 * \ingroup color
 * \brief Yellow color
 */
#define GTG_YELLOW_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 255, 0)

/**
 * \ingroup color
 * \brief Purple color
 */
#define GTG_PURPLE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(153, 25, 230)

/**
 * \ingroup color
 * \brief Light brown color
 */
#define GTG_LIGHTBROWN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(170, 130, 130)

/**
 * \ingroup color
 * \brief Light grey color
 */
#define GTG_LIGHTGREY_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(200, 200, 200)

/**
 * \ingroup color
 * \brief Dark blue color
 */
#define GTG_DARKBLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(0, 0, 80)

/**
 * \ingroup color
 * \brief Pink color
 */
#define GTG_PINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 0, 255)

/**
 * \ingroup color
 * \brief Dark pink color
 */
#define GTG_DARKPINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(180, 80, 180)

/**
 * \ingroup color
 * \brief Sea blue color
 */
#define GTG_SEABLUE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(25, 128, 200)

/**
 * \ingroup color
 * \brief Kaki color
 */
#define GTG_KAKI_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(80, 100, 25)

/**
 * \ingroup color
 * \brief Red blood color
 */
#define GTG_REDBLOOD_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(200, 25, 25)

/**
 * \ingroup color
 * \brief Brown color
 */
#define GTG_BROWN_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(100, 25, 25)

/**
 * \ingroup color
 * \brief Grenat color
 */
#define GTG_GRENAT_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(100, 0, 80)

/**
 * \ingroup color
 * \brief Orange color
 */
#define GTG_ORANGE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 160, 0)


/**
 * \ingroup color
 * \brief Mauve color
 */
#define GTG_MAUVE_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(128, 0, 255)


/**
 * \ingroup color
 * \brief Light pink color
 */
#define GTG_LIGHTPINK_RGB      (gtg_rgb_color_t) GTG_COLOR_SET_COLOR(255, 128, 255)


void __init_color(gtg_color_t p_color, const char* name, gtg_rgb_color_t rgb)
{
	p_color->color_name = (char*)malloc((strlen(name)+1)*sizeof(char));
	strncpy(p_color->color_name, name, strlen(name)+1);
	p_color->rgb = rgb;
}

static int __gtg_color_initialized = 0;
void gtg_color_init()
{
	/* make sure that this function is only run once */
	if(__gtg_color_initialized)
		return;
	__gtg_color_initialized = 1;

	/* fill the default color structures */
	GTG_BLACK      = &__gtg_colors[0];
	GTG_RED        = &__gtg_colors[1];
	GTG_GREEN      = &__gtg_colors[2];
	GTG_BLUE       = &__gtg_colors[3];
	GTG_WHITE      = &__gtg_colors[4];
	GTG_TEAL       = &__gtg_colors[5];
	GTG_DARKGREY   = &__gtg_colors[6];
	GTG_YELLOW     = &__gtg_colors[7];
	GTG_PURPLE     = &__gtg_colors[8];
	GTG_LIGHTBROWN = &__gtg_colors[9];
	GTG_LIGHTGREY  = &__gtg_colors[10];
	GTG_DARKBLUE   = &__gtg_colors[11];
	GTG_PINK       = &__gtg_colors[12];
	GTG_DARKPINK   = &__gtg_colors[13];
	GTG_SEABLUE    = &__gtg_colors[14];
	GTG_KAKI       = &__gtg_colors[15];
	GTG_REDBLOOD   = &__gtg_colors[16];
	GTG_BROWN      = &__gtg_colors[17];
	GTG_GRENAT     = &__gtg_colors[18];
	GTG_ORANGE     = &__gtg_colors[19];
	GTG_MAUVE      = &__gtg_colors[20];
	GTG_LIGHTPINK  = &__gtg_colors[21];

	__init_color(GTG_BLACK     , "BLACK"     , GTG_BLACK_RGB);
	__init_color(GTG_RED       , "RED"       , GTG_RED_RGB);
	__init_color(GTG_GREEN     , "GREEN"     , GTG_GREEN_RGB);
	__init_color(GTG_BLUE      , "BLUE"      , GTG_BLUE_RGB);
	__init_color(GTG_WHITE     , "WHITE"     , GTG_WHITE_RGB);
	__init_color(GTG_TEAL      , "TEAL"      , GTG_TEAL_RGB);
	__init_color(GTG_DARKGREY  , "DARKGREY"  , GTG_DARKGREY_RGB);
	__init_color(GTG_YELLOW    , "YELLOW"    , GTG_YELLOW_RGB);
	__init_color(GTG_PURPLE    , "PURPLE"    , GTG_PURPLE_RGB);
	__init_color(GTG_LIGHTBROWN, "LIGHTBROWN", GTG_LIGHTBROWN_RGB);
	__init_color(GTG_LIGHTGREY , "LIGHTGREY" , GTG_LIGHTGREY_RGB);
	__init_color(GTG_DARKBLUE  , "DARKBLUE"  , GTG_DARKBLUE_RGB);
	__init_color(GTG_PINK      , "PINK"      , GTG_PINK_RGB);
	__init_color(GTG_DARKPINK  , "DARKPINK"  , GTG_DARKPINK_RGB);
	__init_color(GTG_SEABLUE   , "SEABLUE"   , GTG_SEABLUE_RGB);
	__init_color(GTG_KAKI      , "KAKI"      , GTG_KAKI_RGB);
	__init_color(GTG_REDBLOOD  , "REDBLOOD"  , GTG_REDBLOOD_RGB);
	__init_color(GTG_BROWN     , "BROWN"     , GTG_BROWN_RGB);
	__init_color(GTG_GRENAT    , "GRENAT"    , GTG_GRENAT_RGB);
	__init_color(GTG_ORANGE    , "ORANGE"    , GTG_ORANGE_RGB);
	__init_color(GTG_MAUVE     , "MAUVE"     , GTG_MAUVE_RGB);
	__init_color(GTG_LIGHTPINK , "LIGHTPINK" , GTG_LIGHTPINK_RGB);

}

void gtg_color_exit()
{
	int i;
	for (i=0; i<NB_DEFAULT_COLOR; i++)
		free(__gtg_colors[i].color_name);
}

gtg_color_t gtg_color_create( const char *name, uint8_t r, uint8_t g, uint8_t b)
{
    gtg_color_t color = (gtg_color_t)malloc(sizeof(struct gtg_color));
    __init_color( color, name, GTG_COLOR_SET_COLOR(r, g, b) );
    return color;
}

void gtg_color_free( gtg_color_t color ) {
    if ( color->color_name != NULL )
        free(color->color_name);
    free(color);
}
