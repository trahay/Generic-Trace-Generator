/**
 *  \file GTGOTF_Basic.h
 *  \version 0.1
 *  \brief
 *  OTF_GTGBasic1 is the OTF implementation of the basic interface to generate traces (GTGBasic1).
 *
 *  \authors
 *    Developers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - kevin.coulomb@gmail.com \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 */
#ifndef _GTG_OTF_BASIC_H_
#define _GTG_OTF_BASIC_H_

#include "GTGTypes.h"
#include "GTGBasic.h"
#include "GTGOTF_Structs.h"

/**
 * \defgroup cotf OTF interface in C of the traceGeneratorBasic API
 */

/**
 * \ingroup cotf
 * \fn const char* OTF_get_color(gtg_color_t color)
 * \brief Converts a GTG color into a OTF color.
 * \param color GTG color to convert
 * \return The OTF color
 */
const otf_color_t OTF_get_color(gtg_color_t color);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFInitTrace   (const char* filename, gtg_flag_t flags)
 * \brief Initialize an OTF trace.
 * \param filename Root name of the file to create
 * \param flags One of GTG_FLAG_NONE, GTG_FLAG_USE_MPI, GTG_FLAG_NOTBUF.
 * \return 0 if success
 *         An error code otherwise
 */
trace_return_t OTFInitTrace   (const char* filename, gtg_flag_t flags);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSetCompress(int val)
 * \brief Enable trace compression.
 * \param val 0 means no compression, otherwize the output files will be compressed.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSetCompress(int val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddContType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a Container Type.
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddContType   (const char* alias, const char* contType, 
                       const char* name);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddStateType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a State Type.
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddStateType   (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEventType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add an Event Type.
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEventType   (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddLinkType   (const char* alias,
 *                            const char* name,
 *                            const char* contType, 
 *                            const char* srcContType,
 *                            const char* destContType);
 * \brief Add a Link Type.
 * \param alias Alias on the link type
 * \param name Name of the link type
 * \param contType Type of container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddVarType   (const char* alias,
 *                           const char* contType, 
 *                           const char* name)
 * \brief Add a Variable Type.
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddVarType   (const char* alias   , const char* name,
                      const char* contType); 

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEntityValue   (const char* alias,
 *                               const char* entType, 
 *                               const char* name,
 *                               const otf_color_t color)
 * \brief Add an Entity Value.
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEntityValue   (const char* alias, const char* entType, 
				    const char* name , const otf_color_t color);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFDefineContainer (varPrec  time,
 *                           const char  * alias,
 *                           const char  * type, 
 *                           const char  * container,
 *                           const char  * name,
 *                           const char  * file)
 * \brief Define a Container.
 * \param alias Alias of the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container trace
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFDefineContainer (const char* alias,
				   const char*  type, const char* container,
				   const char*  name, const char* file);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFStartContainer (varPrec  time,
 *                           const char  * alias,
 *                           const char  * type,
 *                           const char  * container,
 *                           const char  * name,
 *                           const char  * file)
 * \brief Start a Container.
 * \param time Time at which the container is added
 * \param alias Alias of the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container trace
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFStartContainer (varPrec time, const char* alias    ,
				  const char*  type, const char* container,
				  const char*  name, const char* file);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFDestroyContainer (varPrec  time,
 *                               const char  * name,
 *                               const char  * type) 
 * \brief Destroy a Container.
 * \param time Time at which the container is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFDestroyContainer     (varPrec time, const char*  name,
                              const char*  type);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSetState (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       const char  * val)
 * \brief Set the State of a Container.
 * \param time Time at which the state is set
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSetState   (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFPushState (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * val)
 * \brief Save the current State on a stack and change the State of a Container.
 * \param time Time at which the state is pushed
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFPushState   (varPrec time, const char* type,
                     const char*  cont, const char* val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFPopState (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont)
 * \brief Revert the State of a Container to its previous value.
 * \param time Time at which the state is poped
 * \param type Type of the state
 * \param cont Container in this state
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFPopState   (varPrec time, const char* type,
                    const char*  cont);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEvent (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       const char  * val)
 * \brief Add an Event.
 * \param time Time at which the event happens
 * \param type Type of the event
 * \param cont Container in this event
 * \param val Entity value of the event of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEvent   (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFStartLink (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * src,
 *                        const char  * val,
 *                        const char  * key)
 * \brief Start a Link.
 * \param time Time at which the link starts
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFStartLink   (varPrec time, const char* type,
                     const char*   src, const char* dest,
                     const char*   val , const char* key);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFEndLink (varPrec  time,
 *                      const char  * type,
 *                      const char  * cont,
 *                      const char  * dest,
 *                      const char  * val,
 *                      const char  * key)
 * \brief End a Link.
 * \param time Time at which the link ends
 * \param type Type of the link
 * \param cont Container containning the link
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFEndLink   (varPrec time, const char* type,
                   const char*  src, const char* dest,
                   const char*  val, const char* key);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSetVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Set a Variable value.
 * \param time Time at which the variable is set
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSetVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Add a value to a Variable.
 * \param time Time at which the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSubVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Substract a value from a Variable.
 * \param time Time at which the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSubVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddComment   (const char*  comment)
 *
 * \brief Add some Comment in Trace file.
 * \param comment Comment to be added
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t OTFAddComment   (const char*  comment);

/**
 * \ingroup cotf
 * \fn OTFEndTrace ()
 * \brief Finalize an OTF trace.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFEndTrace ();

#endif /* _GTG_OTF_BASIC_H_ */



