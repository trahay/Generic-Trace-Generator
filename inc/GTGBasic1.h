/**
 *  \file GTGBasic1.h
 *  \version 0.1
 *  \brief
 *
 *  GTGBasic is a basic interface to generate trace in various formats.
 *
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
 *  It has been initiated in 2010 by *eztrace* and *ViTE* projects that both needs a good library to generate traces.
 *
 */
#ifndef _TRACEGENERATORBASIC_
#define _TRACEGENERATORBASIC_
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "GTGColor.h"

/**
 * \brief For drawing the traces in a non bufferized mode
 */
#define NOTBUF 1
/**
 * \brief For drawing the traces in a bufferized mode
 */
#define BUFF   0


/**
 * \defgroup traceType Trace type handler
 */

/**
 * \ingroup traceType
 * \enum traceType
 * \brief The type of the output trace.
 *
 */
typedef enum traceType{
    PAJE, /*!< Paje trace format. */
    VITE, /*!< ViTE-specific trace format. */
    OTF,  /*!< OTF trace format.  */
    TAU   /*!< TAU Trace format.  */
}traceType_t;

/**
 * \ingroup traceType
 * \fn void setTraceType (traceType_t type, int mode)
 * \brief Set the type of output trace and the mode to write it (bufferized or not).
 * \param type Type of trace to generate
 * \param mode Bufferized or non bufferized mode to write the trace
 */
void setTraceType (traceType_t type, int mode);

/**
 * \ingroup traceType
 * \fn traceType_t getTraceType ()
 * \brief Get the type of the output trace.
 * \return The type of the trace
 */
traceType_t getTraceType ();

/**
 * \ingroup traceType
 * \fn int getMode ()
 * \brief To get the mode used to write the trace (bufferized or not)
 * \return The mode used
 */
int getMode ();


/** 
 * \defgroup init To init the generated trace file(s)
 */
/**
 * \ingroup init
 * \fn int initTrace   (const char* filename, int rank)
 * \brief Initialize a trace.
 * \param filename Root name of the file to create
 * \param rank Process number of the file to create
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t initTrace   (const char* filename, int rank);
/**
 * \ingroup init
 * \fn trace_return_t endTrace   ()
 * \brief Finalize a trace.
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t endTrace   ();
/**
 * \ingroup init
 * \fn trace_return_t setCompress (int val)
 * \brief Enable trace compression (only available for OTF traces).
 * \param val 0 means no compression, otherwize the output files will be compressed
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t setCompress (int val);

/**
 * \defgroup procf Functions related to the containers
 */
/**
 * \ingroup procf
 * \fn trace_return_t addContType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief Add a Container Type.
 * \param alias Alias on the container added
 * \param contType Type of the parent container
 * \param name Alternative name of the new container type
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addContType   (const char* alias, const char* contType, 
                   const char* name);

///**
// * \ingroup procf
// * \fn trace_return_t addProcTypeNB   (const char* alias,
// *                          const char* contType, 
// *                          const char* name)
// * \brief Add a Container Type in a non-bufferized mode.
// * \param alias Alias on the container
// * \param contType Type of container
// * \param name Name of the container type
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addProcTypeNB (const char* alias, const char* contType, 
//                   const char* name);

/**
 * \defgroup statef Functions related to the states
 */
/**
 * \ingroup statef
 * \fn trace_return_t addStateType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief Add a State Type.
 * \param alias Alias on the state type added
 * \param contType Type of container of these states
 * \param name Alternative name of the state type
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addStateType   (const char* alias, const char* contType, 
                    const char* name);

///**
// * \ingroup statef
// * \fn trace_return_t addStateTypeNB   (const char* alias,
// *                           const char* contType, 
// *                           const char* name)
// * \brief Add a State Type in a non-bufferized mode.
// * \param alias Alias on the state type
// * \param contType Type of container
// * \param name Name of the state type
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addStateTypeNB (const char* alias, const char* contType, 
//                    const char* name);

/**
 * \defgroup eventf Functions related to the events
 */
/**
 * \ingroup eventf
 * \fn trace_return_t addEventType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief Add an Event Type.
 * \param alias Alias on the event type
 * \param contType Type of container of these events
 * \param name Alternative name of the event type
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addEventType   (const char* alias, const char* contType, 
                    const char* name);
///**
// * \ingroup eventf
// * \fn trace_return_t addEventTypeNB   (const char* alias,
// *                         const char* contType, 
// *                         const char* name)
// * \brief Add an Event Type in a non-bufferized mode.
// * \param alias Alias on the event type
// * \param contType Type of container
// * \param name Name of the event type
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addEventTypeNB (const char* alias, const char* contType, 
//                    const char* name);

/**
 * \defgroup linkf Functions related to links
 */
/**
 * \ingroup linkf
 * \fn trace_return_t addLinkType   (const char* alias,
 *                        const char* name,
 *                        const char* contType, 
 *                        const char* srcContType,
 *                        const char* destContType);
 * \brief Add a Link Type.
 * \param alias Alias on the link type
 * \param name Alternative name of the link type
 * \param contType Type of common ancestral container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addLinkType   (const char* alias   , const char* name,
                   const char* contType, const char* srcContType,
                   const char* destContType);
///**
// * \ingroup linkf
// * \fn trace_return_t addLinkTypeNB   (const char* alias,
// *                          const char* name,
// *                          const char* contType, 
// *                          const char* srcContType,
// *                          const char* destContType);
// * \brief Add a Link Type in a non-bufferized mode.
// * \param alias Alias on the link type
// * \param name Name of the link type
// * \param contType Type of container
// * \param srcContType Type of the source container
// * \param destContType Type of the destination container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addLinkTypeNB (const char* alias   , const char* name,
//                   const char* contType, const char* srcContType,
//                   const char* destContType);

/**
 * \defgroup varf Functions related to variables
 */
/**
 * \ingroup varf
 * \fn trace_return_t addVarType   (const char* alias,
 *                       const char* name,
 *                       const char* contType)
 * \brief Add a Variable Type.
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Alternative name of the variable type
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addVarType   (const char* alias   , const char* name,
                  const char* contType); 
///**
// * \ingroup varf
// * \fn trace_return_t addVarTypeNB   (const char* alias,
// *                         const char* contType, 
// *                         const char* name)
// * \brief Add a Variable Type in a non-bufferized mode.
// * \param alias Alias on the variable type
// * \param contType Type of container
// * \param name Name of the variable type
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addVarTypeNB (const char* alias   , const char* name,
//                  const char* contType);

/**
 * \ingroup init
 * \fn trace_return_t addEntityValue   (const char* alias,
 *                           const char* entType, 
 *                           const char* name,
 *                           gtg_color_t p_color)
 * \brief Add an Entity Value.
 * \param alias Alias on the entity value
 * \param entType Type of the entity that can have the value
 * \param name Alternative name of the variable type
 * \param p_color Color of the entity
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addEntityValue   (const char* alias, const char* entType, 
                      const char* name , gtg_color_t p_color);
///**
// * \ingroup init
// * \fn trace_return_t addEntityValueNB (const char* alias,
// *                           const char* entType, 
// *                           const char* name,
// *                           gtg_color_t p_color)
// * \brief Add an Entity Value in a non-bufferized mode.
// * \param alias Alias on the entity value
// * \param entType Type of the entity
// * \param name Name of the variable type
// * \param p_color Color of the entity
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addEntityValueNB (const char* alias, const char* entType, 
//                      const char* name , gtg_color_t p_color);

/**
 * \ingroup procf
 * \fn trace_return_t addContainer (varPrec  time,
 *                       const char  * alias,
 *                       const char  * type, 
 *                       const char  * container,
 *                       const char  * name,
 *                       const char  * file)
 * \brief Add a Container.
 * \param time Time at which the container is added
 * \param alias Alias of the new container
 * \param type Type of the new container
 * \param container Container parent
 * \param name Alternative name of the variable type
 * \param file File containing the container for vite format. Use "0" or "" chains for other formats.
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addContainer   (varPrec time, const char* alias    ,
                    const char*  type, const char* container,
                    const char*  name, const char* file);
///**
// * \ingroup procf
// * \fn trace_return_t addContainerNB (varPrec  time,
// *                         const char  * alias,
// *                         const char  * type, 
// *                         const char  * container,
// *                         const char  * name,
// *                         const char  * file)
// * \brief Add a Container in a non-bufferized mode.
// * \param time Time at which the container is added
// * \param alias Alias of the new container
// * \param type Type of the container
// * \param container Container parent
// * \param name Name of the variable type
// * \param file File containing the container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addContainerNB (varPrec time, const char* alias    ,
//                    const char*  type, const char* container,
//                    const char*  name, const char* file);

/**
 * \ingroup procf
 * \fn trace_return_t destroyContainer (varPrec  time,
 *                           const char  * name,
 *                           const char  * type) 
 * \brief Destroy a Container.
 * \param time Time at which the container is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t destroyContainer     (varPrec time, const char*  name,
                          const char*  type);
///**
// * \ingroup procf
// * \fn trace_return_t destroyContainerNB (varPrec  time,
// *                             const char  * name,
// *                             const char  * type) 
// * \brief Destroy a Container in a non-bufferized mode.
// * \param time Time at which the container is destroyed
// * \param name Name of the container
// * \param type Type of the container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t destroyContainerNB   (varPrec time, const char*  name,
//                          const char*  type);

/**
 * \ingroup statef
 * \fn trace_return_t setState (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   const char  * val)
 * \brief Set the State of a Container.
 * \param time Time the state changes
 * \param type Type of the state
 * \param cont Container whose state changes
 * \param val Value of new state of container
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t setState   (varPrec time, const char* type,
                const char*  cont, const char* val);
///**
// * \ingroup statef
// * \fn trace_return_t setStateNB (varPrec  time,
// *                     const char  * type,
// *                     const char  * cont,
// *                     const char  * val)
// * \brief Set the State of a Container in a state in a non-bufferized mode.
// * \param time Time at which the state is set
// * \param type Type of the state
// * \param cont Container in this state
// * \param val Entity value of the state of the container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t setStateNB (varPrec time, const char* type,
//                const char*  cont, const char* val);

/**
 * \ingroup statef
 * \fn trace_return_t pushState (varPrec  time,
 *                    const char  * type,
 *                    const char  * cont,
 *                    const char  * val)
 * \brief Save the current State on a stack and change the State of a Container.
 * \param time Time the state changes
 * \param type Type of the state
 * \param cont Container whose state changes
 * \param val Value of state of container
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t pushState   (varPrec time, const char* type,
                 const char*  cont, const char* val);

///**
// * \ingroup statef
// * \fn trace_return_t pushStateNB (varPrec  time,
// *                      const char  * type,
// *                      const char  * cont,
// *                      const char  * val)
// * \brief Save the current State on a stack and change the State of a Container in a non-bufferized mode.
// * \param time Time at which the state is pushed
// * \param type Type of the state
// * \param cont Container in this state
// * \param val Entity value of the state of the container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t pushStateNB (varPrec time, const char* type,
//                 const char*  cont, const char* val);

/**
 * \ingroup statef
 * \fn trace_return_t popState (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont)
 * \brief Revert the State of a Container to its previous value.
 * \param time Time the state changes
 * \param type Type of the state
 * \param cont Container whose state changes
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t popState   (varPrec time, const char* type,
                const char*  cont);

///**
// * \ingroup statef
// * \fn trace_return_t popStateNB (varPrec  time,
// *                     const char  * type,
// *                     const char  * cont)
// * \brief Revert the State of a Container to its previous value in a non-bufferized mode.
// * \param time Time at which the state is poped
// * \param type Type of the state
// * \param cont Container in this state
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t popStateNB (varPrec time, const char* type,
//                const char*  cont);

/**
 * \ingroup eventf
 * \fn trace_return_t addEvent (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   const char  * val)
 * \brief Add an Event.
 * \param time Time the event happens
 * \param type Type of the event
 * \param cont Container that produced the event
 * \param val Value of the new event
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addEvent   (varPrec time, const char* type,
                const char*  cont, const char* val);

///**
// * \ingroup eventf
// * \fn trace_return_t addEventNB (varPrec  time,
// *                     const char  * type,
// *                     const char  * cont,
// *                     const char  * val)
// * \brief Add an Event in a non-bufferized mode.
// * \param time Time at which the event happens
// * \param type Type of the event
// * \param cont Container in this event
// * \param val Entity value of the event of the container
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addEventNB (varPrec time, const char* type,
//                const char*  cont, const char* val);

/**
 * \ingroup linkf
 * \fn trace_return_t startLink (varPrec  time,
 *                    const char  * type,
 *                    const char  * cont,
 *                    const char  * src,
 *                    const char  * dest,
 *                    const char  * val,
 *                    const char  * key)
 * \brief Start a Link.
 * \param time Time the link starts
 * \param type Type of the link
 * \param cont Container containing the link (an ancestor of source and destination container)
 * \param src  Source container
 * \param dest Destination container
 * \param val  Value of the link
 * \param key  Key to match the end link
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t startLink   (varPrec time, const char* type,
                 const char*  cont, const char* src,
                 const char*  dest, const char* val,
                 const char* key);

///**
// * \ingroup linkf
// * \fn trace_return_t startLinkNB (varPrec  time,
// *                      const char  * type,
// *                      const char  * cont,
// *                      const char  * src,
// *                      const char  * dest,
// *                      const char  * val,
// *                      const char  * key)
// * \brief Start a Link in a non-bufferized mode.
// * \param time Time at which the link starts
// * \param type Type of the link
// * \param cont Container containing the link
// * \param src  Source container
// * \param dest Destination container
// * \param val  Entity value of the link
// * \param key  Key to identify the link
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t startLinkNB (varPrec time, const char* type,
//                 const char*  cont, const char* src,
//                 const char*  dest, const char* val,
//                 const char* key);

/**
 * \ingroup linkf
 * \fn trace_return_t endLink (varPrec  time,
 *                  const char  * type,
 *                  const char  * cont,
 *                  const char  * src,
 *                  const char  * dest,
 *                  const char  * val,
 *                  const char  * key)
 * \brief End a Link.
 * \param time Time the link ends
 * \param type Type of the link
 * \param cont Container containing the link (an ancestor of source and destination container)
 * \param src  Source container
 * \param dest Destination container
 * \param val  Value of the link
 * \param key  Key to match the start link
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t endLink   (varPrec time, const char* type,
               const char*  cont, const char* src,
               const char*  dest, const char* val,
               const char* key);
///**
// * \ingroup linkf
// * \fn trace_return_t endLinkNB (varPrec  time,
// *                    const char  * type,
// *                    const char  * cont,
// *                    const char  * src,
// *                    const char  * dest,
// *                    const char  * val,
// *                    const char  * key)
// * \brief End a Link in a non-bufferized mode.
// * \param time Time at which the link ends
// * \param type Type of the link
// * \param cont Container containing the link
// * \param src  Source container
// * \param dest Destination container
// * \param val  Entity value of the link
// * \param key  Key to identify the link
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t endLinkNB (varPrec time, const char* type,
//               const char*  cont, const char* src,
//               const char*  dest, const char* val,
//               const char* key);

/**
 * \ingroup varf
 * \fn trace_return_t setVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief Set a Variable value.
 * \param time Time the variable changes
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  New value of the variable
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t setVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
///**
// * \ingroup varf
// * \fn trace_return_t setVarNB (varPrec  time,
// *                   const char  * type,
// *                   const char  * cont,
// *                   varPrec  val)
// * \brief Set a Variable value in a non-bufferized mode.
// * \param time Time at which the variable is set
// * \param type Type of the variable
// * \param cont Container containing the variable
// * \param val  Value of the variable
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t setVarNB (varPrec time, const char*  type,
//              const char*  cont, varPrec val);

/**
 * \ingroup varf
 * \fn trace_return_t addVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief Add a value to a Variable.
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value added
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t addVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
///**
// * \ingroup varf
// * \fn trace_return_t addVarNB (varPrec  time,
// *                   const char  * type,
// *                   const char  * cont,
// *                   varPrec  val)
// * \brief Add a value to a Variable in a non-bufferized mode.
// * \param time Time at which the variable is incremented
// * \param type Type of the variable
// * \param cont Container containing the variable
// * \param val  Value added
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t addVarNB (varPrec time, const char*  type,
//              const char*  cont, varPrec val);

/**
 * \ingroup varf
 * \fn trace_return_t subVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief Substract a value from a Variable.
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value substracted
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */

trace_return_t subVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
///**
// * \ingroup varf
// * \fn trace_return_t subVarNB (varPrec  time,
// *                   const char  * type,
// *                   const char  * cont,
// *                   varPrec  val)
// * \brief Substract a value from a Variable in a non-bufferized mode.
// * \param time Time at which the variable is decremented
// * \param type Type of the variable
// * \param cont Container containing the variable
// * \param val  Value substracted
// * \return TRACE_SUCCESS on success \n
// *         An error code otherwise
// */
//trace_return_t subVarNB (varPrec time, const char*  type,
//              const char*  cont, varPrec val);



#endif



