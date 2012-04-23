/**
 *  \file GTGtypes.h
 *  \version 0.1
 *  \brief
 *
 *  types define some types that are used in the library.
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
#ifndef _GTG_TYPES_H_
#define _GTG_TYPES_H_
/**
 * \defgroup type Types used
 */

/**
 * \ingroup type Type used for time and value in C
 * \brief Use the double precision type for time and value.
 */
typedef double varPrec;

/**
 * \ingroup type Return values of functions
 * \enum trace_return_t
 * \brief Define various return values
 */
typedef enum trace_return_t{
    TRACE_SUCCESS  , /*!< Success of the call.           */
    TRACE_ERR_OPEN , /*!< Failed to open files to write. */
    TRACE_ERR_CLOSE, /*!< Failed to close file.          */
    TRACE_ERR_WRITE,  /*!< Failed to write trace.         */      
    TRACE_ERR_NOT_IMPL  /*!< Function not impleneted.         */      
} trace_return_t;

#endif /* _GTG_TYPES_H_ */
