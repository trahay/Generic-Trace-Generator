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

#ifndef OTF2_SNAPSHOTS_EVENTS_H
#define OTF2_SNAPSHOTS_EVENTS_H


enum otf2_snapshots_event_type
{
    @otf2 for event in events | snapshot_events:
    OTF2_SNAPSHOTS_EVENT_@@event.upper@@ = @@event.id@@,
    @otf2 endfor
};

class otf2_snapshots_event
{
public:

    otf2_snapshots_event_type type;
    uint64_t                  time;
    OTF2_AttributeList*       attributes;

    otf2_snapshots_event( otf2_snapshots_event_type type,
                          uint64_t                  time,
                          OTF2_AttributeList*       attributes );
    virtual ~otf2_snapshots_event();
};

/*___derived event classes._____________________________________________ */

@otf2 for event in events | snapshot_events:

class otf2_snapshots_event_@@event.lower@@: public otf2_snapshots_event
{
public:

    @otf2 for attr in event.attributes:
    @@attr.type@@ @@attr.lower@@;
    @otf2  for array_attr in attr.array_attributes:
    @@array_attr.type@@* @@array_attr.lower@@;
    @otf2  endfor
    @otf2 endfor

    otf2_snapshots_event_@@event.lower@@( uint64_t            time,
                                          OTF2_AttributeList* attributes @@event.funcargs()@@ );

    @otf2 if event.all_array_attributes:
    ~otf2_snapshots_event_@@event.lower@@();
    @otf2 endif
};

@otf2 endfor

#endif /* OTF2_SNAPSHOTS_EVENTS_H */
