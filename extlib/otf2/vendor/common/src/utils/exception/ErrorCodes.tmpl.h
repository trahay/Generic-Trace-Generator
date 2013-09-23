/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2012,
 *    RWTH Aachen University, Germany
 *    Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *    Technische Universitaet Dresden, Germany
 *    University of Oregon, Eugene, USA
 *    Forschungszentrum Juelich GmbH, Germany
 *    German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *    Technische Universitaet Muenchen, Germany
 *
 * See the COPYING file in the package base directory for details.
 *
 */

#ifndef @PACKAGE_SYM_CAPS@_ERROR_CODES_H
#define @PACKAGE_SYM_CAPS@_ERROR_CODES_H


/**
 * @file            @PACKAGE_SYM_CAPS@_ErrorCodes.h
 * @maintainer      Daniel Lorenz <d.lorenz@fz-juelich.de>
 * @status          ALPHA
 * @ingroup         @PACKAGE_SYM_CAPS@_Exception_module
 *
 * @brief           Error codes and error handling.
 *
 * @author          Dominic Eschweiler <d.eschweiler@fz-juelich.de>
 */

#include <errno.h>
#include <stdint.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This is the list of error codes for @PACKAGE_SYM_CAPS@.
 */
typedef enum
{
    /** Special marker for error messages which indicates an deprecation. */
    @PACKAGE_SYM_CAPS@_DEPRECATED    = -3,

    /** Special marker when the application will be aborted. */
    @PACKAGE_SYM_CAPS@_ABORT         = -2,

    /** Special marker for error messages which are only warnings. */
    @PACKAGE_SYM_CAPS@_WARNING       = -1,

    /** Operation successful */
    @PACKAGE_SYM_CAPS@_SUCCESS       = 0,

    /** Invalid error code
     *
     * Should only be used internally and not as an actual error code.
     */
    @PACKAGE_SYM_CAPS@_ERROR_INVALID = 1,

    /* These are the internal implementation of POSIX error codes. */
    /** The list of arguments is to long */
    @PACKAGE_SYM_CAPS@_ERROR_E2BIG,
    /** Not enough rights */
    @PACKAGE_SYM_CAPS@_ERROR_EACCES,
    /** Address is not available */
    @PACKAGE_SYM_CAPS@_ERROR_EADDRNOTAVAIL,
    /** Address family is not supported */
    @PACKAGE_SYM_CAPS@_ERROR_EAFNOSUPPORT,
    /** Resource temporaly not available */
    @PACKAGE_SYM_CAPS@_ERROR_EAGAIN,
    /** Connection is already processed */
    @PACKAGE_SYM_CAPS@_ERROR_EALREADY,
    /** Invalid file pointer */
    @PACKAGE_SYM_CAPS@_ERROR_EBADF,
    /** Invalid message */
    @PACKAGE_SYM_CAPS@_ERROR_EBADMSG,
    /** Resource or device is busy */
    @PACKAGE_SYM_CAPS@_ERROR_EBUSY,
    /** Operation was aborted */
    @PACKAGE_SYM_CAPS@_ERROR_ECANCELED,
    /** No child process available */
    @PACKAGE_SYM_CAPS@_ERROR_ECHILD,
    /** Connection was refused */
    @PACKAGE_SYM_CAPS@_ERROR_ECONNREFUSED,
    /** Connection was reset */
    @PACKAGE_SYM_CAPS@_ERROR_ECONNRESET,
    /** Resolved deadlock */
    @PACKAGE_SYM_CAPS@_ERROR_EDEADLK,
    /** Destination address was expected */
    @PACKAGE_SYM_CAPS@_ERROR_EDESTADDRREQ,
    /** Domain error */
    @PACKAGE_SYM_CAPS@_ERROR_EDOM,
    /** Reserved */
    @PACKAGE_SYM_CAPS@_ERROR_EDQUOT,
    /** File does already exist */
    @PACKAGE_SYM_CAPS@_ERROR_EEXIST,
    /** Invalid Address */
    @PACKAGE_SYM_CAPS@_ERROR_EFAULT,
    /** File is to big */
    @PACKAGE_SYM_CAPS@_ERROR_EFBIG,
    /** Operation is work in progress */
    @PACKAGE_SYM_CAPS@_ERROR_EINPROGRESS,
    /** Interuption of an operating system call */
    @PACKAGE_SYM_CAPS@_ERROR_EINTR,
    /** Invalid argument */
    @PACKAGE_SYM_CAPS@_ERROR_EINVAL,
    /** Generic I/O error */
    @PACKAGE_SYM_CAPS@_ERROR_EIO,
    /** Socket is already connected */
    @PACKAGE_SYM_CAPS@_ERROR_EISCONN,
    /** Target is a directory */
    @PACKAGE_SYM_CAPS@_ERROR_EISDIR,
    /** To many layers of symbolic links */
    @PACKAGE_SYM_CAPS@_ERROR_ELOOP,
    /** To many opened files */
    @PACKAGE_SYM_CAPS@_ERROR_EMFILE,
    /** To many links */
    @PACKAGE_SYM_CAPS@_ERROR_EMLINK,
    /** Message buffer is to small */
    @PACKAGE_SYM_CAPS@_ERROR_EMSGSIZE,
    /** Reserved */
    @PACKAGE_SYM_CAPS@_ERROR_EMULTIHOP,
    /** Filename is to long */
    @PACKAGE_SYM_CAPS@_ERROR_ENAMETOOLONG,
    /** Network is down */
    @PACKAGE_SYM_CAPS@_ERROR_ENETDOWN,
    /** Connection was reset from the network */
    @PACKAGE_SYM_CAPS@_ERROR_ENETRESET,
    /** Network is not reachable */
    @PACKAGE_SYM_CAPS@_ERROR_ENETUNREACH,
    /** To much opened files */
    @PACKAGE_SYM_CAPS@_ERROR_ENFILE,
    /** No buffer space available */
    @PACKAGE_SYM_CAPS@_ERROR_ENOBUFS,
    /** No more data left in the queue */
    @PACKAGE_SYM_CAPS@_ERROR_ENODATA,
    /** This device does not support this function */
    @PACKAGE_SYM_CAPS@_ERROR_ENODEV,
    /** File or Directory does not exist */
    @PACKAGE_SYM_CAPS@_ERROR_ENOENT,
    /** Can not execute binary */
    @PACKAGE_SYM_CAPS@_ERROR_ENOEXEC,
    /** Locking failed */
    @PACKAGE_SYM_CAPS@_ERROR_ENOLCK,
    /** Reserved */
    @PACKAGE_SYM_CAPS@_ERROR_ENOLINK,
    /** Not enough main memory available */
    @PACKAGE_SYM_CAPS@_ERROR_ENOMEM,
    /** Message has not the expected type */
    @PACKAGE_SYM_CAPS@_ERROR_ENOMSG,
    /** This protocol is not available */
    @PACKAGE_SYM_CAPS@_ERROR_ENOPROTOOPT,
    /** No space left on device */
    @PACKAGE_SYM_CAPS@_ERROR_ENOSPC,
    /** No stream available */
    @PACKAGE_SYM_CAPS@_ERROR_ENOSR,
    /** This is not a stream */
    @PACKAGE_SYM_CAPS@_ERROR_ENOSTR,
    /** Requested function is not implemented */
    @PACKAGE_SYM_CAPS@_ERROR_ENOSYS,
    /** Socket is not connected */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTCONN,
    /** This is not an directory */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTDIR,
    /** This directory is not empty */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTEMPTY,
    /** No socket */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTSOCK,
    /** This operation is not supported */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTSUP,
    /** This IOCTL is not supported by the device */
    @PACKAGE_SYM_CAPS@_ERROR_ENOTTY,
    /** Device is not yet configured */
    @PACKAGE_SYM_CAPS@_ERROR_ENXIO,
    /** Operation is not supported by this socket */
    @PACKAGE_SYM_CAPS@_ERROR_EOPNOTSUPP,
    /** Value is to long for the datatype */
    @PACKAGE_SYM_CAPS@_ERROR_EOVERFLOW,
    /** Operation is not permitted */
    @PACKAGE_SYM_CAPS@_ERROR_EPERM,
    /** Broken pipe */
    @PACKAGE_SYM_CAPS@_ERROR_EPIPE,
    /** Protocoll error */
    @PACKAGE_SYM_CAPS@_ERROR_EPROTO,
    /** Protocoll is not supported */
    @PACKAGE_SYM_CAPS@_ERROR_EPROTONOSUPPORT,
    /** Wrong protocoll type for this socket */
    @PACKAGE_SYM_CAPS@_ERROR_EPROTOTYPE,
    /** Value is out of range */
    @PACKAGE_SYM_CAPS@_ERROR_ERANGE,
    /** Filesystem is read only */
    @PACKAGE_SYM_CAPS@_ERROR_EROFS,
    /** This seek is not allowed */
    @PACKAGE_SYM_CAPS@_ERROR_ESPIPE,
    /** No matching process found */
    @PACKAGE_SYM_CAPS@_ERROR_ESRCH,
    /** Reserved */
    @PACKAGE_SYM_CAPS@_ERROR_ESTALE,
    /** Timout in file stream or IOCTL */
    @PACKAGE_SYM_CAPS@_ERROR_ETIME,
    /** Connection timed out */
    @PACKAGE_SYM_CAPS@_ERROR_ETIMEDOUT,
    /** File couldn't be executed while it is opened */
    @PACKAGE_SYM_CAPS@_ERROR_ETXTBSY,
    /** Operation would be blocking */
    @PACKAGE_SYM_CAPS@_ERROR_EWOULDBLOCK,
    /** Invalid link between devices */
    @PACKAGE_SYM_CAPS@_ERROR_EXDEV,

    /* These are the error codes specific to the @PACKAGE_NAME@ package */
    @PACKAGE_SPECIFIC_ERROR_CODES@
} @PACKAGE_SYM_CAPS@_ErrorCode;


/**
 * Returns the name of an error code.
 *
 * @param errorCode : Error Code
 *
 * @returns Returns the name of a known error code, and "INVALID_ERROR" for
 *          invalid or unknown error IDs.
 * @ingroup @PACKAGE_SYM_CAPS@_Exception_module
 */
const char*
@PACKAGE_SYM_CAPS@_Error_GetName( @PACKAGE_SYM_CAPS@_ErrorCode errorCode );


/**
 * Returns the description of an error code.
 *
 * @param errorCode : Error Code
 *
 * @returns Returns the description of a known error code.
 * @ingroup @PACKAGE_SYM_CAPS@_Exception_module
 */
const char*
@PACKAGE_SYM_CAPS@_Error_GetDescription( @PACKAGE_SYM_CAPS@_ErrorCode errorCode );


/**
 * Signature of error handler callback functions. The error handler can be set
 * with @ref @PACKAGE_SYM_CAPS@_Error_RegisterCallback.
 *
 * @param userData        : Data passed to this function as given by the registry call.
 * @param file            : Name of the source-code file where the error appeared
 * @param line            : Line number in the source-code where the error appeared
 * @param function        : Name of the function where the error appeared
 * @param errorCode       : Error Code
 * @param msgFormatString : Format string like it is used at the printf family.
 * @param va              : Variable argument list
 *
 * @returns Should return the errorCode
 */
typedef @PACKAGE_SYM_CAPS@_ErrorCode
( *@PACKAGE_SYM_CAPS@_ErrorCallback )( void*          userData,
                         const char*    file,
                         uint64_t       line,
                         const char*    function,
                         @PACKAGE_SYM_CAPS@_ErrorCode errorCode,
                         const char*    msgFormatString,
                         va_list        va );


/**
 * Register a programmers callback function for error handling.
 *
 * @param errorCallbackIn : Fucntion will be called instead of printing a
 *                          default message to standard error.
 * @param userData :        Data pointer passed to the callback.
 *
 * @returns Function pointer to the former error handling function.
 * @ingroup @PACKAGE_SYM_CAPS@_Exception_module
 *
 */
@PACKAGE_SYM_CAPS@_ErrorCallback
@PACKAGE_SYM_CAPS@_Error_RegisterCallback( @PACKAGE_SYM_CAPS@_ErrorCallback errorCallbackIn,
                             void*              userData );


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* @PACKAGE_SYM_CAPS@_ERROR_CODES_H */
