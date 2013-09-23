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

/*___derived stack event class con- and destructors._____________________________________________ */

@otf2 for event in events | snapshot_events:

otf2_snapshots_event_@@event.lower@@::
otf2_snapshots_event_@@event.lower@@( uint64_t time,
                                      OTF2_AttributeList* attributes @@event.funcargs()@@ )
    : otf2_snapshots_event( OTF2_SNAPSHOTS_EVENT_@@event.upper@@, time, attributes )
{
    /* initialize event specific variables */

    @otf2 for attr in event.attributes:
    this->@@attr.lower@@ = @@attr.name@@;
    @otf2  for array_attr in attr.array_attributes:
    this->@@array_attr.lower@@ = new @@array_attr.type@@[ @@attr.lower@@ ];
    memcpy( this->@@array_attr.lower@@,
            @@array_attr.lower@@,
            @@attr.lower@@ * sizeof( @@array_attr.type@@ ) );
    @otf2  endfor
    @otf2 endfor
};

@otf2 if event.all_array_attributes:

otf2_snapshots_event_@@event.lower@@::
~otf2_snapshots_event_@@event.lower@@()
{
    @otf2 for array_attr in event.all_array_attributes:
    delete[] this->@@array_attr.lower@@;
    @otf2  endfor
}

@otf2 endif
@otf2 endfor
