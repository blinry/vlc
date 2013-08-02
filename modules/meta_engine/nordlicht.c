/*****************************************************************************
 * nordlicht.c
 *****************************************************************************
 * Copyright (C) 2013 VLC authors and VideoLAN
 * $Id$
 *
 * Authors: Sebastian Morr <sebastian@morr.cc>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/stat.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_art_finder.h>
#include <vlc_fs.h>
#include <vlc_url.h>
#include <vlc_input_item.h>

#include <nordlicht.h>

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int FindMeta( vlc_object_t * );
static void *Run            (void *);

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

vlc_module_begin ()
    set_shortname( N_( "nordlicht" ) )
    set_description( N_("Creates video barcodes for navigation") )
    set_capability( "art finder", 100 )
    set_callbacks( FindMeta, NULL )
vlc_module_end ()

/*****************************************************************************
 *****************************************************************************/
static int FindMeta( vlc_object_t *p_this )
{
    art_finder_t *p_finder = (art_finder_t *)p_this;
    input_item_t *p_item = p_finder->p_item;
    input_item_SetArtURL( p_item, vlc_path2uri("/tmp/nordlicht.png", "file"));

    vlc_thread_t *thread = malloc(sizeof(vlc_thread_t));
    int ret = vlc_clone(thread, Run, p_item, VLC_THREAD_PRIORITY_LOW);

    return VLC_SUCCESS;
}

static void *Run(void *data)
{
    input_item_t *p_item = (input_item_t *)data;

    if( !p_item )
        return NULL;

    char *psz_dir = input_item_GetURI( p_item );
    if( !psz_dir )
        return NULL;

    char *psz_path = make_path( psz_dir );
    free( psz_dir );
    if( psz_path == NULL )
        return NULL;

    nordlicht *code = nordlicht_create(1200, 100);

    nordlicht_input(code, psz_path);
    nordlicht_output(code, "/tmp/nordlicht.png");

    while (!nordlicht_done(code)) {
        nordlicht_step(code);
        input_item_SetArtURL(p_item, vlc_path2uri("/tmp/nordlicht.png", "file"));
        sleep(1);
    }

    return NULL;
}
