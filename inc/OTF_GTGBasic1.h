/**
 *  \file OTF_GTGBasic1.h
 *  \version 0.1
 *  \brief
 *  OTF_GTGBasic1 is the OTF implementation of the basic interface to generate traces (GTGBasic). \n
 *
 *  \authors
 *    Developpers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - coulomb@enseirb-matmeca.fr \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 *
 */
#ifndef _OTFTRACEGENERATORBASIC_
#define _OTFTRACEGENERATORBASIC_
#include "types.h"
/**
 * \defgroup cotf OTF interface in C of the traceGeneratorBasic API
 */

/**
 * \ingroup cotf
 * \fn trace_return_t OTFInitTrace   (const char* filename)
 * \brief To init a trace in an OTF format
 * \param filename Root name of the file to create
 * \return 0 if sucess
 *         An error code otherwise
 */
trace_return_t OTFInitTrace   (const char* filename);
/**
 * \ingroup cotf
 * \ fn trace_return_t OTFAddProcType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a container type
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddProcType   (const char* alias, const char* contType, 
                       const char* name);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddProcTypeNB   (const char* alias,
 *                              const char* contType, 
 *                              const char* name)
 * \brief To add a container type in a non bufferized mode
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddProcTypeNB (const char* alias, const char* contType, 
                       const char* name);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddStateType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a state type
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
 * \fn trace_return_t OTFAddStateTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief To add a state type in a non bufferized mode
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddStateTypeNB (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEventType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add an event type
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
 * \fn trace_return_t OTFAddEventTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief To add an event type in a bufferized mode
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEventTypeNB (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddLinkType   (const char* alias,
 *                            const char* name,
 *                            const char* contType, 
 *                            const char* srcContType,
 *                            const char* destContType);
 * \brief To add a link type
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
 * \fn trace_return_t OTFAddLinkTypeNB   (const char* alias,
 *                              const char* name,
 *                              const char* contType, 
 *                              const char* srcContType,
 *                              const char* destContType);
 * \brief To add a link type in a bufferized mode
 * \param alias Alias on the link type in a non bufferized mode
 * \param name Name of the link type
 * \param contType Type of container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddLinkTypeNB (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddVarType   (const char* alias,
 *                           const char* contType, 
 *                           const char* name)
 * \brief To add a variable type
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
 * \fn trace_return_t OTFAddVarTypeNB   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a variable type in a non bufferized mode
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddVarTypeNB (const char* alias   , const char* name,
                      const char* contType);
                  

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEntityValue   (const char* alias,
 *                               const char* entType, 
 *                               const char* name,
 *                               const char* color)
 * \brief To add an entity value
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEntityValueNB (const char* alias,
 *                               const char* entType, 
 *                               const char* name,
 *                               const char* color)
 * \brief To add an entity value
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEntityValueNB (const char* alias, const char* entType, 
                          const char* name , const char* color);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddContainer (varPrec  time,
 *                           const char  * alias,
 *                           const char  * type, 
 *                           const char  * container,
 *                           const char  * name,
 *                           const char  * file)
 * \brief To add a container
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container trace
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddContainer (varPrec time, const char* alias    ,
                      const char*  type, const char* container,
                      const char*  name, const char* file);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddContainerNB (varPrec  time,
 *                             const char  * alias,
 *                             const char  * type, 
 *                             const char  * container,
 *                             const char  * name,
 *                             const char  * file)
 * \brief To add a container in a non bufferized mode
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddContainerNB (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFDestroyContainer (varPrec  time,
 *                               const char  * name,
 *                               const char  * type) 
 * \brief To destroy a container
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFDestroyContainer     (varPrec time, const char*  name,
                              const char*  type);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFDestroyContainerNB (varPrec  time,
 *                                 const char  * name,
 *                                 const char  * type) 
 * \brief To destroy a container in a bufferized mode
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFDestroyContainerNB   (varPrec time, const char*  name,
                              const char*  type);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSetState (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       const char  * val)
 * \brief To set a proc in a state
 * \param time Time the state is set
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
 * \fn trace_return_t OTFSetStateNB (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * val)
 * \brief To set a proc in a state in a bufferized mode
 * \param time Time the state is set
 * \param type Type of the state
 * \param cont Container in this state                                       
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSetStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFPushState (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * val)
 * \brief To push a state in a proc
 * \param time Time the state is pushed
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
 * \fn trace_return_t OTFPushStateNB (varPrec  time,
 *                          const char  * type,
 *                          const char  * cont,
 *                          const char  * val)
 * \brief To push a state in a proc in a bufferized mode
 * \param time Time the state is pushed
 * \param type Type of the state
 * \param cont Container in this state                                       
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFPushStateNB (varPrec time, const char* type,
                     const char*  cont, const char* val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFPopState (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       const char  * val)
 * \brief To pop a state in a proc
 * \param time Time the state is poped
 * \param type Type of the state
 * \param cont Container in this state                                       
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFPopState   (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFPopStateNB (varPrec  time,
 *                         const char  * type,
 *                         const char  * cont,
 *                         const char  * val)
 * \brief To pop a state in a proc in a bufferized mode
 * \param time Time the state is poped
 * \param type Type of the state
 * \param cont Container in this state                                       
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFPopStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val);


/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddEvent (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       const char  * val)
 * \brief To add an event
 * \param time Time the event happens
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
 * \fn trace_return_t OTFAddEventNB (varPrec  time,
 *                         const char  * type,
 *                         const char  * cont,
 *                         const char  * val)
 * \brief To add an event in a bufferized mode
 * \param time Time the event happens
 * \param type Type of the event
 * \param cont Container in this event
 * \param val Entity value of the event of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddEventNB (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFStartLink (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * src,
 *                        const char  * val,
 *                        const char  * key)
 * \brief To start a link
 * \param time Time the comm starts
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFStartLink   (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFStartLinkNB (varPrec  time,
 *                          const char  * type,
 *                          const char  * cont,
 *                          const char  * src,
 *                          const char  * val,
 *                          const char  * key)
 * \brief To start a link in a non bufferized mode
 * \param time Time the comm starts
 * \param type Type of the link
 * \param cont Container containning the link
 * \param src  Container source
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFStartLinkNB (varPrec time, const char* type,
                     const char*  cont , const char* src,
                     const char*  val  , const char* key);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFEndLink (varPrec  time,
 *                      const char  * type,
 *                      const char  * cont,
 *                      const char  * dest,
 *                      const char  * val,
 *                      const char  * key)
 * \brief To end a link
 * \param time Time the comm ends
 * \param type Type of the link
 * \param cont Container containning the link
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFEndLink   (varPrec time, const char* type,
                   const char*  cont , const char* dest,
                   const char*  val  , const char* key);
/**
 * \ingroup cotf
 * \fn trace_return_t OTFEndLinkNB (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * dest,
 *                        const char  * val,
 *                        const char  * key)
 * \brief To end a link in a bufferized mode
 * \param time Time the comm ends
 * \param type Type of the link
 * \param cont Container containning the link
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFEndLinkNB (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key);


/**
 * \ingroup cotf
 * \fn trace_return_t OTFSetVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief To set a variable value
 * \param time Time the variable is set
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
 * \fn trace_return_t OTFSetVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief To set a variable value in  a bufferized mode
 * \param time Time the variable is set
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSetVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFAddVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief To add a value to a variable
 * \param time Time the variable is incremented
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
 * \fn trace_return_t OTFAddVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief To add a value to a variable in a non bufferized mode
 * \param time Time the variable is incremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFAddVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cotf
 * \fn trace_return_t OTFSubVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief To substract a value to a variable
 * \param time Time the variable is incremented
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
 * \fn trace_return_t OTFSubVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief To substract a value to a variable in a non bufferized mode
 * \param time Time the variable is decremented
 * \param type Type of the variable
 * \param cont Container containning the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t OTFSubVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

#endif



