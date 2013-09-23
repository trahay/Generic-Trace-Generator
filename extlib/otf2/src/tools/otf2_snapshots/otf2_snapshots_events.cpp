/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2013,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 */

#include <config.h>

#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include <otf2/otf2.h>

#include "otf2_snapshots_events.hpp"


/*___con- and destructor for otf2_snapshots_event class._____________________ */

otf2_snapshots_event::otf2_snapshots_event( otf2_snapshots_event_type type,
                                            uint64_t                  time,
                                            OTF2_AttributeList*       attributes )
{
    this->type       = type;
    this->time       = time;
    this->attributes = NULL;
    this->attributes = OTF2_AttributeList_New();

    OTF2_Type           type_id;
    uint32_t            attribute_id;
    OTF2_AttributeValue attribute_value;

    for ( uint32_t i = 0;
          i < OTF2_AttributeList_GetNumberOfElements( attributes );
          ++i )
    {
        OTF2_AttributeList_GetAttributeByIndex( attributes,
                                                i,
                                                &attribute_id,
                                                &type_id,
                                                &attribute_value );
        OTF2_AttributeList_AddAttribute( this->attributes,
                                         attribute_id,
                                         type_id,
                                         attribute_value );
    }
}

otf2_snapshots_event::~otf2_snapshots_event()
{
    OTF2_AttributeList_Delete( attributes );
}

#include "otf2_snapshots_events_inc.cpp"
