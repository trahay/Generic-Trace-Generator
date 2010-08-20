/**
 *  \file paje_GTGBasic1.h
 *  \version 0.1
 *  \brief
 *  paje_GTGBasic1 is the Paje implementation of the basic interface to generate traces (GTGBasic1). \n
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
#ifndef _PAJETRACEGENERATORBASIC_
#define _PAJETRACEGENERATORBASIC_
#include "types.h"
/**
 * \defgroup cpaje Paje interface in C of the GTGBasic1 API
 */

/**
 * \ingroup cpaje
 * \enum pajeReturn
 * \brief Return values of the paje functions
 */
enum pajeReturn{
    PAJE_SUCCESS,   /*!< Success of the paje call. */
    PAJE_ERR_OPEN,  /*!< Call to fopen failed. */
    PAJE_ERR_CLOSE, /*!< Call to fclose failed. */
    PAJE_ERR_WRITE  /*!< Tried to write in a nil stream. (e.g. if call a non bufferized function after a bufferized init)*/
}pajeReturn;

/**
 * \ingroup cpaje
 * \fn int pajeInitTrace   (const char* filename)
 * \brief To init a trace in a VITE format ( *.ept)
 * \param filename Root name of the file to create
 * \return 0 if sucess
 *         An error code otherwise
 */
int pajeInitTrace   (const char* filename);

/**
 * \ingroup cpaje
 * \fn int pajeSeqInitTrace   (const char* filename)
 * \brief To init a trace in a PAJE format (*.trace)
 * \param filename Root name of the file to create
 * \return 0 if sucess
 *         An error code otherwise
 */
int pajeSeqInitTrace   (const char* filename);
/**
 * \ingroup cpaje
 * \ fn int pajeAddProcType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a container type
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddProcType   (const char* alias, const char* contType, 
                       const char* name);

/**
 * \ingroup cpaje
 * \fn int pajeAddProcTypeNB   (const char* alias,
 *                              const char* contType, 
 *                              const char* name)
 * \brief To add a container type in a non bufferized mode
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddProcTypeNB (const char* alias, const char* contType, 
                       const char* name);
/**
 * \ingroup cpaje
 * \fn int pajeAddStateType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a state type
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddStateType   (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cpaje
 * \fn int pajeAddStateTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief To add a state type in a non bufferized mode
 * \param alias Alias on the state type
 * \param contType Type of container
 * \param name Name of the state type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddStateTypeNB (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cpaje
 * \fn int pajeAddEventType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add an event type
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddEventType   (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cpaje
 * \fn int pajeAddEventTypeNB   (const char* alias,
 *                               const char* contType, 
 *                               const char* name)
 * \brief To add an event type in a bufferized mode
 * \param alias Alias on the event type
 * \param contType Type of container
 * \param name Name of the event type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddEventTypeNB (const char* alias, const char* contType, 
                        const char* name);
/**
 * \ingroup cpaje
 * \fn int pajeAddLinkType   (const char* alias,
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
int pajeAddLinkType   (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);
/**
 * \ingroup cpaje
 * \fn int pajeAddLinkTypeNB   (const char* alias,
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
int pajeAddLinkTypeNB (const char* alias   , const char* name,
                       const char* contType, const char* srcContType,
                       const char* destContType);

/**
 * \ingroup cpaje
 * \fn int pajeAddVarType   (const char* alias,
 *                           const char* contType, 
 *                           const char* name)
 * \brief To add a variable type
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddVarType   (const char* alias   , const char* name,
                      const char* contType); 
/**
 * \ingroup cpaje
 * \fn int pajeAddVarTypeNB   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief To add a variable type in a non bufferized mode
 * \param alias Alias on the variable type
 * \param contType Type of container
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddVarTypeNB (const char* alias   , const char* name,
                      const char* contType);
                  

/**
 * \ingroup cpaje
 * \fn int pajeAddEntityValue   (const char* alias,
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
int pajeAddEntityValue   (const char* alias, const char* entType, 
                          const char* name , const char* color);
/**
 * \ingroup cpaje
 * \fn int pajeAddEntityValueNB (const char* alias,
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
int pajeAddEntityValueNB (const char* alias, const char* entType, 
                          const char* name , const char* color);

/**
 * \ingroup cpaje
 * \fn int pajeAddContainer (varPrec  time,
 *                           const char  * alias,
 *                           const char  * type, 
 *                           const char  * container,
 *                           const char  * name,
 *                           const char  * file)
 * \brief To add a container in ViTE format
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container trace
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddContainer (varPrec time, const char* alias    ,
                      const char*  type, const char* container,
                      const char*  name, const char* file);


/**
 * \ingroup cpaje
 * \fn int pajeAddContainerNB (varPrec  time,
 *                             const char  * alias,
 *                             const char  * type, 
 *                             const char  * container,
 *                             const char  * name,
 *                             const char  * file)
 * \brief To add a container in a non bufferized mode in ViTE format
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \param file File containing the container
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeAddContainerNB (varPrec time, const char* alias    ,
                        const char*  type, const char* container,
                        const char*  name, const char* file);
/**
 * \ingroup cpaje
 * \fn int pajeSeqAddContainer (varPrec  time,
 *                              const char  * alias,
 *                              const char  * type, 
 *                              const char  * container,
 *                              const char  * name)
 * \brief To add a container in paje format
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeSeqAddContainer (varPrec time, const char* alias    ,
                         const char*  type, const char* container,
                         const char*  name);

/**
 * \ingroup cpaje
 * \fn int pajeSeqAddContainerNB (varPrec  time,
 *                                const char  * alias,
 *                                const char  * type, 
 *                                const char  * container,
 *                                const char  * name)
 * \brief To add a container in a non bufferized mode in paje format
 * \param time Time the proc is added
 * \param alias Alias on the proc added
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeSeqAddContainerNB (varPrec time, const char* alias    ,
                           const char*  type, const char* container,
                           const char*  name);
/**
 * \ingroup cpaje
 * \fn int pajeDestroyContainer (varPrec  time,
 *                               const char  * name,
 *                               const char  * type) 
 * \brief To destroy a container
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeDestroyContainer     (varPrec time, const char*  name,
                              const char*  type);
/**
 * \ingroup cpaje
 * \fn int pajeDestroyContainerNB (varPrec  time,
 *                                 const char  * name,
 *                                 const char  * type) 
 * \brief To destroy a container in a bufferized mode
 * \param time Time the proc is destroyed
 * \param name Name of the container
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeDestroyContainerNB   (varPrec time, const char*  name,
                              const char*  type);

/**
 * \ingroup cpaje
 * \fn int pajeSetState (varPrec  time,
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
int pajeSetState   (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn int pajeSetStateNB (varPrec  time,
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
int pajeSetStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val);

/**
 * \ingroup cpaje
 * \fn int pajePushState (varPrec  time,
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
int pajePushState   (varPrec time, const char* type,
                     const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn int pajePushStateNB (varPrec  time,
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
int pajePushStateNB (varPrec time, const char* type,
                     const char*  cont, const char* val);

/**
 * \ingroup cpaje
 * \fn int pajePopState (varPrec  time,
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
int pajePopState   (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn int pajePopStateNB (varPrec  time,
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
int pajePopStateNB (varPrec time, const char* type,
                    const char*  cont, const char* val);


/**
 * \ingroup cpaje
 * \fn int pajeAddEvent (varPrec  time,
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
int pajeAddEvent   (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn int pajeAddEventNB (varPrec  time,
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
int pajeAddEventNB (varPrec time, const char* type,
                    const char*  cont, const char* val);
/**
 * \ingroup cpaje
 * \fn int pajeStartLink (varPrec  time,
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
int pajeStartLink   (varPrec time, const char* type,
                     const char*   cont, const char* src,
                     const char*   val , const char* key);
/**
 * \ingroup cpaje
 * \fn int pajeStartLinkNB (varPrec  time,
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
int pajeStartLinkNB (varPrec time, const char* type,
                     const char*  cont , const char* src,
                     const char*  val  , const char* key);
/**
 * \ingroup cpaje
 * \fn int pajeEndLink (varPrec  time,
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
int pajeEndLink   (varPrec time, const char* type,
                   const char*  cont , const char* dest,
                   const char*  val  , const char* key);
/**
 * \ingroup cpaje
 * \fn int pajeEndLinkNB (varPrec  time,
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
int pajeEndLinkNB (varPrec time, const char* type,
                   const char*   cont, const char* dest,
                   const char*   val , const char* key);


/**
 * \ingroup cpaje
 * \fn int pajeSetVar (varPrec  time,
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
int pajeSetVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);
/**
 * \ingroup cpaje
 * \fn int pajeSetVarNB (varPrec  time,
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
int pajeSetVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn int pajeAddVar (varPrec  time,
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
int pajeAddVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);
/**
 * \ingroup cpaje
 * \fn int pajeAddVarNB (varPrec  time,
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
int pajeAddVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn int pajeSubVar (varPrec  time,
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

int pajeSubVar   (varPrec time, const char*  type,
                  const char*  cont, varPrec val);
/**
 * \ingroup cpaje
 * \fn int pajeSubVarNB (varPrec  time,
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
int pajeSubVarNB (varPrec time, const char*  type,
                  const char*  cont, varPrec val);

/**
 * \ingroup cpaje
 * \fn pajeEndTrace ()
 * \brief To end writting the trace and clean data
 * \return 0 if success \n
 *         An error code otherwise
 */
int pajeEndTrace ();

#endif



