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

#ifndef OTF2_SNAPSHOTS_CALLBACKS_H
#define OTF2_SNAPSHOTS_CALLBACKS_H

#include <otf2/otf2.h>


OTF2_GlobalDefReaderCallbacks*
otf2_snapshots_get_global_def_callbacks( void );


OTF2_GlobalEvtReaderCallbacks*
otf2_snapshots_get_event_callbacks( void );


#endif /* OTF2_SNAPSHOTS_CALLBACKS_H */
