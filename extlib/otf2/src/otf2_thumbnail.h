/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
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

#ifndef OTF2_INTERNAL_THUMBNAIL_H
#define OTF2_INTERNAL_THUMBNAIL_H

/** @brief Wrapper for the OTF2_ThumbRecord_enum
 */
typedef uint8_t OTF2_ThumbRecord;


/** @brief Defines the size of a thumb chunk. */
#define OTF2_CHUNK_SIZE_THUMB ( uint64_t )( 1024 * 1024 )


/** @brief An enum which holds all values that are possible global definition types.
 */
enum OTF2_ThumbRecord_enum
{
    /* First five entries are reserved for OTF2 internals */

    /* OTF2 Records */
    OTF2_THUMB_HEADER = 10,  /**< */
    OTF2_THUMB_SAMPLE = 11,  /**< */

    OTF2_THUMB_RECORD_MAX_ID
};

/** @brief Keeps all necessary information about the thumb writer. */
struct OTF2_ThumbWriter_struct
{
    /** Archive handle */
    OTF2_Archive*     archive;
    /** Pointer to the memory buffer handle. */
    OTF2_Buffer*      buffer;
    /** Thumbnail ID. */
    uint32_t          thumb_id;
    /** Chain to next thumb writer, used in OTF2_Archive */
    OTF2_ThumbWriter* next;
    /** some header data used to check number of written samples */
    uint32_t          number_of_samples;
    uint32_t          number_of_metrics;
    /** Sample counter */
    uint64_t          sample_counter;
};

/** @brief Keeps all necessary information about the thumb reader. */
struct OTF2_ThumbReader_struct
{
    /** Archive handle */
    OTF2_Archive*     archive;
    /** Pointer to the memory buffer handle. */
    OTF2_Buffer*      buffer;
    /** Thumbnail ID. */
    uint32_t          thumb_id;
    /** Chain to next thumb writer, used in OTF2_Archive */
    OTF2_ThumbReader* next;

    /** header buffers */
    char*              name;
    char*              description;
    OTF2_ThumbnailType type;
    uint32_t           number_of_samples;
    uint32_t           number_of_metrics;
    uint64_t*          refs_to_defs;
};

OTF2_ThumbWriter*
otf2_thumb_writer_new( OTF2_Archive* archive,
                       uint32_t      thumbID );

OTF2_ErrorCode
otf2_thumb_writer_write_header( OTF2_ThumbWriter*  writerHandle,
                                const char*        name,
                                const char*        description,
                                OTF2_ThumbnailType type,
                                uint32_t           numberOfSamples,
                                uint32_t           numberOfMetrics,
                                const uint64_t*    refsToDefs );

OTF2_ErrorCode
otf2_thumb_writer_delete( OTF2_ThumbWriter* writer );


OTF2_ThumbReader*
otf2_thumb_reader_new( OTF2_Archive* archive,
                       uint8_t       thumbID );

OTF2_ErrorCode
otf2_thumb_reader_delete( OTF2_ThumbReader* writer );

OTF2_ErrorCode
otf2_thumb_reader_read_header( OTF2_ThumbReader* reader );

#endif /* OTF2_INTERNAL_THUMBNAIL_H */
