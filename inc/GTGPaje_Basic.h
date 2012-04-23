/**
 *  \file GTGPaje_Basic.h
 *  \version 0.1
 *  \brief
 *  paje_GTGBasic1 is the Paje implementation of the basic interface to generate traces (GTGBasic1).
 *
 *  \authors
 *    Developers are : \n
 *        Francois Rue      - francois.rue@labri.fr \n
 *        Francois Trahay   - francois.trahay@labri.fr \n
 *        Johnny   Jazeix   - jazeix@enseirb-matmeca.fr \n
 *        Kevin    Coulomb  - kevin.coulomb@gmail.com \n
 *        Mathieu  Faverge  - faverge@labri.fr \n
 *        Olivier  Lagrasse - lagrasse@enseirb-matmeca.fr \n
 *
 */
#ifndef _GTG_PAJE_BASIC_H_
#define _GTG_PAJE_BASIC_H_

#include "GTGBasic.h"

/**
 * \brief Constant to create a paje trace
 */
#define FMT_PAJE 0
/**
 * \brief Constant to create a vite trace
 */
#define FMT_VITE 1

/**
 * \defgroup cpaje Paje interface in C of the GTGBasic1 API
 */

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeInitTrace   (const char* filename, int rank, gtg_flag_t flags, int fmt)
 * \brief Initialize a VITE trace ( *.ept)
 * \param filename Root name of the file to create
 * \param rank Rank of the processor
 * \param flags One of GTG_FLAG_NONE, GTG_FLAG_USE_MPI, GTG_FLAG_NOTBUF.
 * \param fmt Format, paje or vite
 * \return 0 if success
 *         An error code otherwise
 */
trace_return_t pajeInitTrace   (const char* filename, int rank, gtg_flag_t flags, int fmt);

/**
 * \ingroup cpaje
 * \fn char* pajeGetName (int rk)
 * \param rk Rank of the proc you want the filename containing it
 * \brief Function to get the name of the file containing all the data for the proc of rank rk
 * \return Name of the file. 
 */
char* pajeGetName (int rk);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeSetCompress(int val)
 * \brief Enable trace compression.
 * \param val 0 means no compression, otherwize the output files will be compressed.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSetCompress(int val);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeAddContType   (const char* alias,
 *                             const char* contType, 
 *                             const char* name)
 * \brief Add a Container Type.
 * \param alias Alias on the container
 * \param contType Type of container
 * \param name Name of the container type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeAddContType   (const char* alias, const char* contType, 
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
 * \fn trace_return_t pajeSeqAddContainer (varPrec  time,
 *                                         const char  * alias,
 *                                         const char  * type, 
 *                                         const char  * container,
 *                                         const char  * name)
 * \brief Add a Container (PAJE format).
 * \param time Time at which the container is added
 * \param alias Alias on the new container
 * \param type Type of the container
 * \param container Container parent
 * \param name Name of the variable type
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeSeqAddContainer   (varPrec time, const char* alias    ,
                                      const char*  type, const char* container,
                                      const char*  name);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeDestroyContainer (varPrec  time,
 *                                          const char  * name,
 *                                          const char  * type)
 * \brief Destroy a Container.
 * \param time Time at which the container is destroyed
 * \param name Name on the container to destroy
 * \param type Type of the container
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeDestroyContainer     (varPrec time, const char*  name,
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
 * \fn trace_return_t pajeStartLink (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * src,
 *                     const char  * val,
 *                     const char  * key)
 * \brief Start a link
 * \param time Time at which the link starts
 * \param type Type of the link
 * \param cont Container parent of the source and destination containers containing the link
 * \param src  Source container
 * \param val  Value of the link
 * \param key  Key used to match start link with end link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeStartLink   (varPrec time, const char* type,
                                const char*   cont, const char* src,
                                const char*   val , const char* key);

/**
 * \ingroup cpaje
 * \fn trace_return_t pajeEndLink (varPrec  time,
 *                     const char  * type,
 *                     const char  * cont,
 *                     const char  * dest,
 *                     const char  * val,
 *                     const char  * key)
 * \brief Start a link
 * \param time Time at which the link starts
 * \param type Type of the link
 * \param cont Container parent of the source and destination containers containing the link
 * \param dest Source container
 * \param val  Value of the link
 * \param key  Key used to match start link with end link
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeEndLink   (varPrec time, const char* type,
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
 * \fn trace_return_t pajeAddComment   (const char*  comment)
 *
 * \brief Add some Comment in Trace file.
 * \param comment Comment to be added
 * \return TRACE_SUCCESS on success \n
 *         An error code otherwise
 */
trace_return_t pajeAddComment   (const char*  comment);

/**
 * \ingroup cpaje
 * \fn pajeEndTrace ()
 * \brief Finalize a PAJE trace.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t pajeEndTrace ();

/**
 * \ingroup cpaje
 * \fn viteEndTrace ()
 * \brief Finalize a VITE trace.
 * \return 0 if success \n
 *         An error code otherwise
 */
trace_return_t viteEndTrace ();

/*
 * Functions to handle extra-parameters in the Paje Definitions
 * This will need to be cleaned to be closer to OTF interface
 */
enum gtg_paje_evtdef_e {
  GTG_PAJE_EVTDEF_DefineContainerType,
  GTG_PAJE_EVTDEF_DefineStateType,
  GTG_PAJE_EVTDEF_DefineEventType,
  GTG_PAJE_EVTDEF_DefineEntityValue,
  GTG_PAJE_EVTDEF_CreateContainer,
  GTG_PAJE_EVTDEF_DestroyContainer,
  GTG_PAJE_EVTDEF_SetState,
  GTG_PAJE_EVTDEF_PushState,
  GTG_PAJE_EVTDEF_PopState,
  GTG_PAJE_EVTDEF_NewEvent,
  GTG_PAJE_EVTDEF_DefineLinkType,
  GTG_PAJE_EVTDEF_StartLink,
  GTG_PAJE_EVTDEF_EndLink,
  GTG_PAJE_EVTDEF_DefineVariableType,
  GTG_PAJE_EVTDEF_SetVariable,
  GTG_PAJE_EVTDEF_AddVariable,
  GTG_PAJE_EVTDEF_SubVariable,
  GTG_PAJE_EVTDEF_NBR
};

enum gtg_paje_fieldtype_e { 
  GTG_PAJE_FIELDTYPE_Int,
  GTG_PAJE_FIELDTYPE_Hex,
  GTG_PAJE_FIELDTYPE_Date,
  GTG_PAJE_FIELDTYPE_Double,
  GTG_PAJE_FIELDTYPE_String,
  GTG_PAJE_FIELDTYPE_Color,
  GTG_PAJE_FIELDTYPE_NBR
};

void pajeEventDefAddParam( enum gtg_paje_evtdef_e event, const char *name, 
                           enum gtg_paje_fieldtype_e type );

#endif /* _GTG_PAJE_BASIC_H_ */



