/**
 *  \file GTGBasic1.h
 *  \version 0.1
 *  \brief
 *
 *  GTGBasic is a basic interface to generate trace in various formats. \n
 *  It has been initiated in 2010 by *eztrace* and *ViTE* projects that both needs a good library to generate traces.
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
#ifndef _TRACEGENERATORBASIC_
#define _TRACEGENERATORBASIC_
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "GTGColor.h"

/**
 * \defgroup traceType Trace type handler
 */

/**
 * \ingroup traceType
 * \enum traceType
 * \brief The type of trace generated
 *
 */
typedef enum traceType{
    PAJE, /*!< Trace format of Paje. */
    VITE, /*!< Trace format of ViTE. */
    OTF,  /*!< Trace format of OTF.  */
    TAU   /*!< Trace format of TAU.  */
}traceType_t;

/**
 * \ingroup traceType
 * \fn void setTraceType (traceType_t type)
 * \brief C function to set the type of the trace to be generated
 * \param type Type of trace to generate
 */
void setTraceType (traceType_t type);

/**
 * \ingroup traceType
 * \fn traceType_t getTraceType ()
 * \brief C function to get the type of the trace to be generated
 * \return The type of the trace
 */
traceType_t getTraceType ();

/** 
 * \defgroup init To init the generated trace file(s)
 */
/**
 * \ingroup init
 * \fn int initTrace   (const char* filename)
 * \brief C function to init a trace
 * \param filename Root name of the file to create
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t initTrace   (const char* filename, int rank);
/**
 * \ingroup init
 * \fn trace_return_t endTrace   ()
 * \brief C function to end writting the trace
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t endTrace   ();
/**
 * \ingroup init
 * \fn trace_return_t setCompress (int val)
 * \brief Usefull for OTF traces only.
 * \param val 0 means no compression, otherwize the output files will be compressed
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t setCompress (int val);

/**
 * \defgroup procf Functions linked to the containers
 */
/**
 * \ingroup procf
 * \ fn trace_return_t addProcType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief C function to add a container type
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addProcType   (const char* alias, const char* contType, 
                   const char* name);

/**
 * \ingroup procf
 * \fn trace_return_t addProcTypeNB   (const char* alias,
 *                          const char* contType, 
 *                          const char* name)
 * \brief C function to add a container type in a non bufferized mode
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addProcTypeNB (const char* alias, const char* contType, 
                   const char* name);
/**
 * \defgroup statef Functions linked to the states
 */
/**
 * \ingroup statef
 * \fn trace_return_t addStateType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief C function to add a state type
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addStateType   (const char* alias, const char* contType, 
                    const char* name);

/**
 * \ingroup statef
 * \fn trace_return_t addStateTypeNB   (const char* alias,
 *                           const char* contType, 
 *                           const char* name)
 * \brief C function to add a state type in a non bufferized mode
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addStateTypeNB (const char* alias, const char* contType, 
                    const char* name);

/**
 * \defgroup eventf Functions linked to the events
 */
/**
 * \ingroup eventf
 * \fn trace_return_t addEventType   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief C function to add an event type
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEventType   (const char* alias, const char* contType, 
                    const char* name);
/**
 * \ingroup eventf
 * \fn trace_return_t addEventTypeNB   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief C function to add an event type in a non bufferized mode
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEventTypeNB (const char* alias, const char* contType, 
                    const char* name);

/**
 * \defgroup linkf Functions linked to links
 */
/**
 * \ingroup linkf
 * \fn trace_return_t addLinkType   (const char* alias,
 *                        const char* name,
 *                        const char* contType, 
 *                        const char* srcContType,
 *                        const char* destContType);
 * \brief C function to add a link type
 * \param alias Alias on the link type
 * \param name Name of the link type
 * \param contType Type of container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addLinkType   (const char* alias   , const char* name,
                   const char* contType, const char* srcContType,
                   const char* destContType);
/**
 * \ingroup linkf
 * \fn trace_return_t addLinkTypeNB   (const char* alias,
 *                          const char* name,
 *                          const char* contType, 
 *                          const char* srcContType,
 *                          const char* destContType);
 * \brief C function to add a link type in a non bufferized mode
 * \param alias Alias on the link type
 * \param name Name of the link type
 * \param contType Type of container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addLinkTypeNB (const char* alias   , const char* name,
                   const char* contType, const char* srcContType,
                   const char* destContType);

/**
 * \defgroup varf Functions linked to variables
 */
/**
 * \ingroup varf
 * \fn trace_return_t addVarType   (const char* alias,
 *                       const char* name,
 *                       const char* contType)
 * \brief C function to add a variable type
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addVarType   (const char* alias   , const char* name,
                  const char* contType); 
/**
 * \ingroup varf
 * \fn trace_return_t addVarTypeNB   (const char* alias,
 *                         const char* contType, 
 *                         const char* name)
 * \brief C function to add a variable type in a non bufferized mode
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addVarTypeNB (const char* alias   , const char* name,
                  const char* contType);

/**
 * \ingroup init
 * \fn trace_return_t addEntityValue   (const char* alias,
 *                           const char* entType, 
 *                           const char* name,
 *                           gtg_rgb_color_t p_color)
 * \brief C function to add an entity value
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param p_color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEntityValue   (const char* alias, const char* entType, 
                      const char* name , gtg_color_t p_color);
/**
 * \ingroup init
 * \fn trace_return_t addEntityValueNB (const char* alias,
 *                           const char* entType, 
 *                           const char* name,
 *                           gtg_rgb_color_t p_color)
 * \brief C function to add an entity value in a non bufferized mode
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param p_color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEntityValueNB (const char* alias, const char* entType, 
                      const char* name , gtg_color_t p_color);

/**
 * \ingroup procf
 * \fn trace_return_t addContainer (varPrec  time,
 *                       const char  * alias,
 *                       const char  * type, 
 *                       const char  * container,
 *                       const char  * name,
 *                       const char  * file)
 * \brief C function to add a container
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addContainer   (varPrec time, const char* alias    ,
                    const char*  type, const char* container,
                    const char*  name, const char* file);
/**
 * \ingroup procf
 * \fn trace_return_t addContainerNB (varPrec  time,
 *                         const char  * alias,
 *                         const char  * type, 
 *                         const char  * container,
 *                         const char  * name,
 *                         const char  * file)
 * \brief C function to add a container in a non bufferized mode
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addContainerNB (varPrec time, const char* alias    ,
                    const char*  type, const char* container,
                    const char*  name, const char* file);

/**
 * \ingroup procf
 * \fn trace_return_t destroyContainer (varPrec  time,
 *                           const char  * name,
 *                           const char  * type) 
 * \brief C function to destroy a container
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t destroyContainer     (varPrec time, const char*  name,
                          const char*  type);
/**
 * \ingroup procf
 * \fn trace_return_t destroyContainerNB (varPrec  time,
 *                             const char  * name,
 *                             const char  * type) 
 * \brief C function to destroy a container in a non bufferized mode
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t destroyContainerNB   (varPrec time, const char*  name,
                          const char*  type);

/**
 * \ingroup statef
 * \fn trace_return_t setState (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   const char  * val)
 * \brief C function to set a proc in a state
 * \param time Time the state is set
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t setState   (varPrec time, const char* type,
                const char*  cont, const char* val);
/**
 * \ingroup statef
 * \fn trace_return_t setStateNB (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * val)
 * \brief C function to set a proc in a state in a non bufferized mode
 * \param time Time the state is set
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t setStateNB (varPrec time, const char* type,
                const char*  cont, const char* val);

/**
 * \ingroup statef
 * \fn trace_return_t pushState (varPrec  time,
 *                    const char  * type,
 *                    const char  * cont,
 *                    const char  * val)
 * \brief C function to push a state in a proc
 * \param time Time the state is pushed
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pushState   (varPrec time, const char* type,
                 const char*  cont, const char* val);

/**
 * \ingroup statef
 * \fn trace_return_t pushStateNB (varPrec  time,
 *                      const char  * type,
 *                      const char  * cont,
 *                      const char  * val)
 * \brief C function to push a state in a proc in a non bufferized mode
 * \param time Time the state is pushed
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pushStateNB (varPrec time, const char* type,
                 const char*  cont, const char* val);

/**
 * \ingroup statef
 * \fn trace_return_t popState (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont)
 * \brief C function to pop a state in a proc
 * \param time Time the state is poped
 * \param type Type of the state
 * \param cont Container in this state
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t popState   (varPrec time, const char* type,
                const char*  cont);

/**
 * \ingroup statef
 * \fn trace_return_t popStateNB (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont)
 * \brief C function to pop a state in a proc in a non bufferized mode
 * \param time Time the state is poped
 * \param type Type of the state
 * \param cont Container in this state
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t popStateNB (varPrec time, const char* type,
                const char*  cont);

/**
 * \ingroup eventf
 * \fn trace_return_t addEvent (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   const char  * val)
 * \brief C function to add an event
 * \param time Time the event happens
 * \param type Type of the event
 * \param cont Container in this event
 * \param val Entity value of the event of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEvent   (varPrec time, const char* type,
                const char*  cont, const char* val);

/**
 * \ingroup eventf
 * \fn trace_return_t addEventNB (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * val)
 * \brief C function to add an event in a non bufferized mode
 * \param time Time the event happens
 * \param type Type of the event
 * \param cont Container in this event
 * \param val Entity value of the event of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addEventNB (varPrec time, const char* type,
                const char*  cont, const char* val);

/**
 * \ingroup linkf
 * \fn trace_return_t startLink (varPrec  time,
 *                    const char  * type,
 *                    const char  * cont,
 *                    const char  * src,
 *                    const char  * dest,
 *                    const char  * val,
 *                    const char  * key)
 * \brief C function to start a link. If creating a paje trace, dest param is useless.
 * \param time Time the comm starts
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t startLink   (varPrec time, const char* type,
                 const char*  cont, const char* src,
                 const char*  dest, const char* val,
                 const char* key);

/**
 * \ingroup linkf
 * \fn trace_return_t startLinkNB (varPrec  time,
 *                      const char  * type,
 *                      const char  * cont,
 *                      const char  * src,
 *                      const char  * dest,
 *                      const char  * val,
 *                      const char  * key)
 * \brief C function to start a link in a non bufferized mode. If creating a paje trace, dest param is useless.
 * \param time Time the comm starts
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t startLinkNB (varPrec time, const char* type,
                 const char*  cont, const char* src,
                 const char*  dest, const char* val,
                 const char* key);

/**
 * \ingroup linkf
 * \fn trace_return_t endLink (varPrec  time,
 *                  const char  * type,
 *                  const char  * cont,
 *                  const char  * src,
 *                  const char  * dest,
 *                  const char  * val,
 *                  const char  * key)
 * \brief C function to end a link. If creating a paje trace, src param is useless.
 * \param time Time the comm ends
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t endLink   (varPrec time, const char* type,
               const char*  cont, const char* src,
               const char*  dest, const char* val,
               const char* key);
/**
 * \ingroup linkf
 * \fn trace_return_t endLinkNB (varPrec  time,
 *                    const char  * type,
 *                    const char  * cont,
 *                    const char  * src,
 *                    const char  * dest,
 *                    const char  * val,
 *                    const char  * key)
 * \brief C function to end a link in a non bufferized mode. If creating a paje trace, src param is useless.
 * \param time Time the comm ends
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t endLinkNB (varPrec time, const char* type,
               const char*  cont, const char* src,
               const char*  dest, const char* val,
               const char* key);

/**
 * \ingroup varf
 * \fn trace_return_t setVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief C function to set a variable value
 * \param time Time the variable is set
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t setVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
/**
 * \ingroup varf
 * \fn trace_return_t setVarNB (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   varPrec  val)
 * \brief C function to set a variable value in a non bufferized mode
 * \param time Time the variable is set
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t setVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val);

/**
 * \ingroup varf
 * \fn trace_return_t addVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief C function to add a value to a variable
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
/**
 * \ingroup varf
 * \fn trace_return_t addVarNB (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   varPrec  val)
 * \brief C function to add a value to a variable in a non bufferized mode
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t addVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val);

/**
 * \ingroup varf
 * \fn trace_return_t subVar (varPrec  time,
 *                 const char  * type,
 *                 const char  * cont,
 *                 varPrec  val)
 * \brief C function to substract a value to a variable
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */

trace_return_t subVar   (varPrec time, const char*  type,
              const char*  cont, varPrec val);
/**
 * \ingroup varf
 * \fn trace_return_t subVarNB (varPrec  time,
 *                   const char  * type,
 *                   const char  * cont,
 *                   varPrec  val)
 * \brief C function to substract a value to a variable in a non bufferized mode
 * \param time Time the variable is decremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t subVarNB (varPrec time, const char*  type,
              const char*  cont, varPrec val);



#endif



