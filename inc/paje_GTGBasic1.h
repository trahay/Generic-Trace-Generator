/**
 *  \file paje_GTGBasic1.h
 *  \version 0.1
 *  \brief
 *  paje_GTGBasic1 is the Paje implementation of the basic interface to generate traces (GTGBasic1).
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
#ifndef _PAJETRACEGENERATORBASIC_
#define _PAJETRACEGENERATORBASIC_
#include "types.h"
/**
 * \defgroup cpaje Paje interface in C of the GTGBasic1 API
 */

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeInitTrace   (const char* filename, int rank)
 * \brief Initialize a VITE trace ( *.ept)
 * \param filename Root name of the file to create
 * \param rank Rank of the processor
 * \return 0 if sucess
 *         An error code otherwise
 */
trace_return_t pajeInitTrace   (const char* filename, int rank);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSeqInitTrace   (const char* filename)
 * \brief Initialize a PAJE trace (*.trace)
 * \param filename Root name of the file to create
 * \return 0 if sucess
 *         An error code otherwise
 */
trace_return_t pajeSeqInitTrace   (const char* filename);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddProcType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a Container Type.
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddProcType   (const char* alias, const char* contType, 
                       const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddProcTypeNB   (const char* alias,
 *                              const char* contType, 
 *                              const char* name)
 * \brief Add a Container Type in a non-bufferized mode.
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddProcTypeNB (const char* alias, const char* contType, 
                       const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddStateType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a State Type.
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddStateType   (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddStateTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief Add a State Type in a non-bufferized mode.
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddStateTypeNB (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEventType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add an Event Type.
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddEventType   (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEventTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief Add an Event Type in a non-bufferized mode.
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddEventTypeNB (const char* alias, const char* contType, 
                        const char* name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddLinkType   (const char* alias,
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
trace_return_t pajeAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddLinkTypeNB   (const char* alias,
 *                              const char* name,
 *                              const char* contType, 
 *                              const char* srcContType,
 *                              const char* destContType);
 * \brief Add a Link Type in a non-bufferized mode.
 * \param alias Alias on the link type
 * \param name Name of the link type
 * \param contType Type of container
 * \param srcContType Type of the source container
 * \param destContType Type of the destination container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddLinkTypeNB (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddVarType   (const char* alias,
 *                           const char* contType, 
 *                           const char* name)
 * \brief Add a Variable Type.
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddVarType   (const char* alias   , const char* name,
                      const char* contType); 

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddVarTypeNB   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a Variable Type in a non-bufferized mode.
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddVarTypeNB (const char* alias   , const char* name,
                      const char* contType);


/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEntityValue   (const char* alias,
 *                               const char* entType, 
 *                               const char* name,
 *                               const char* color)
 * \brief Add an Entity Value.
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEntityValueNB (const char* alias,
 *                               const char* entType, 
 *                               const char* name,
 *                               const char* color)
 * \brief Add an Entity Value in a non-buffered mode.
 * \param alias Alias on the entity value
 * \param entType Type of the entity
 * \param name Name of the variable type
 * \param color Color of the entity
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddEntityValueNB (const char* alias, const char* entType, 
                          const char* name , const char* color);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddContainer (varPrec  time,
 *                           const char  * alias,
 *                           const char  * type, 
 *                           const char  * container,
 *                           const char  * name,
 *                           const char  * file)
 * \brief Add a Container (VITE format).
 * \param time Time at which the container is added
 * \param alias Alias on the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container trace
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddContainer (varPrec time, const char* alias    ,
                      const char*  type, const char* container,
                      const char*  name, const char* file);


/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddContainerNB (varPrec  time,
 *                             const char  * alias,
 *                             const char  * type, 
 *                             const char  * container,
 *                             const char  * name,
 *                             const char  * file)
 * \brief Add a Container in a non-bufferized mode (VITE format).
 * \param time Time at which the container is added
 * \param alias Alias on the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddContainerNB (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSeqAddContainer (varPrec  time,
 *                              const char  * alias,
 *                              const char  * type, 
 *                              const char  * container,
 *                              const char  * name)
 * \brief Add a Container (PAJE format).
 * \param time Time at which the container is added
 * \param alias Alias on the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSeqAddContainer (varPrec time, const char* alias    ,
                         const char*  type, const char* container,
                         const char*  name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSeqAddContainerNB (varPrec  time,
 *                                const char  * alias,
 *                                const char  * type, 
 *                                const char  * container,
 *                                const char  * name)
 * \brief Add a Container in a non-bufferized mode (PAJE format).
 * \param time Time at which the container is added
 * \param alias Alias on the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSeqAddContainerNB (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeDestroyContainer (varPrec  time,
 *                               const char  * name,
 *                               const char  * type) 
 * \brief Destroy a Container.
 * \param time Time at which the container is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
                              const char*  type);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeDestroyContainerNB (varPrec  time,
 *                                 const char  * name,
 *                                 const char  * type) 
 * \brief Destroy a Container in a non-bufferized mode.
 * \param time Time at which the container is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeDestroyContainerNB   (varPrec time, const char*  name,
                              const char*  type);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSetState (varPrec  time,
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
trace_return_t pajeSetState   (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSetStateNB (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * val)
 * \brief Set the State of a Container in a state in a non-bufferized mode.
 * \param time Time at which the state is set
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSetStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajePushState (varPrec  time,
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
trace_return_t pajePushState   (varPrec time, const char* type,
                     const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajePushStateNB (varPrec  time,
 *                          const char  * type,
 *                          const char  * cont,
 *                          const char  * val)
 * \brief Save the current State on a stack and change the State of a Container in a non-bufferized mode.
 * \param time Time at which the state is pushed
 * \param type Type of the state
 * \param cont Container in this state
 * \param val Entity value of the state of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajePushStateNB (varPrec time, const char* type,
                     const char*  cont, const char* val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajePopState (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont)
 * \brief Revert the State of a Container to its previous value.
 * \param time Time at which the state is poped
 * \param type Type of the state
 * \param cont Container in this state
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajePopState   (varPrec time, const char* type,
                    const char*  cont);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajePopStateNB (varPrec  time,
 *                         const char  * type,
 *                         const char  * cont)
 * \brief Revert the State of a Container to its previous value in a non-bufferized mode.
 * \param time Time at which the state is poped
 * \param type Type of the state
 * \param cont Container in this state
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajePopStateNB (varPrec time, const char* type,
                    const char*  cont);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEvent (varPrec  time,
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
trace_return_t pajeAddEvent   (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddEventNB (varPrec  time,
 *                         const char  * type,
 *                         const char  * cont,
 *                         const char  * val)
 * \brief Add an Event in a non-bufferized mode.
 * \param time Time at which the event happens
 * \param type Type of the event
 * \param cont Container in this event
 * \param val Entity value of the event of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddEventNB (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeStartLink (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * src,
 *                        const char  * val,
 *                        const char  * key)
 * \brief Start a Link.
 * \param time Time at which the link starts
 * \param type Type of the link
 * \param cont Container containing the link
 * \param src  Container source
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeStartLink   (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeStartLinkNB (varPrec  time,
 *                          const char  * type,
 *                          const char  * cont,
 *                          const char  * src,
 *                          const char  * val,
 *                          const char  * key)
 * \brief Start a Link in a non-bufferized mode.
 * \param time Time at which the link starts
 * \param type Type of the link
 * \param cont Container containing the link
 * \param src  Container source
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeStartLinkNB (varPrec time, const char* type,
                     const char*  cont , const char* src,
                     const char*  val  , const char* key);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeEndLink (varPrec  time,
 *                      const char  * type,
 *                      const char  * cont,
 *                      const char  * dest,
 *                      const char  * val,
 *                      const char  * key)
 * \brief End a link.
 * \param time Time at which the link ends
 * \param type Type of the link
 * \param cont Container containing the link
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeEndLink   (varPrec time, const char* type,
                   const char*  cont , const char* dest,
                   const char*  val  , const char* key);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeEndLinkNB (varPrec  time,
 *                        const char  * type,
 *                        const char  * cont,
 *                        const char  * dest,
 *                        const char  * val,
 *                        const char  * key)
 * \brief End a Link in a non-bufferized mode.
 * \param time Time at which the link ends
 * \param type Type of the link
 * \param cont Container containing the link
 * \param dest Container destination
 * \param val  Entity value of the link
 * \param key  Key to identify the link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeEndLinkNB (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key);


/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSetVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Set a Variable value
 * \param time Time at which the variable is set
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSetVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSetVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief Set a Variable value in a non-bufferized mode
 * \param time Time at which the variable is set
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value of the variable
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSetVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Add a value to a Variable.
 * \param time Time at which the variable is incremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief Add a value to a Variable in a non-bufferized mode.
 * \param time Time at which the variable is incremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value added
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSubVar (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     varPrec  val)
 * \brief Substract a value from a Variable.
 * \param time Time at which the variable is incremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSubVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);
/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSubVarNB (varPrec  time,
 *                       const char  * type,
 *                       const char  * cont,
 *                       varPrec  val)
 * \brief Aubstract a value from a variable in a non-bufferized mode * \param time Time at which the variable is decremented
 * \param type Type of the variable
 * \param cont Container containing the variable
 * \param val  Value substracted
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSubVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn pajeEndTrace ()
 * \brief Finalize a PAJE or VITE trace.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeEndTrace ();

#endif



