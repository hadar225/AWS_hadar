/* sect_cat.h
 * Routines supporting MPEG2 Transport Stream ansalyzation
 * Copyright 2004, Dipl.-Ing. Wolfram STERING <wolfi@xxxxxxxxxxxxxx>
 *
 * $Id: $
 *
 * Ethereal - Network traffic analyzer
 * By Gerald Combs <gerald@xxxxxxxxxxxx>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __SECT_CAT_H__
#define __SECT_CAT_H__

#include <glib.h>
#include "sect_gen.h"

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_cat {

	dvb_sect_gen_t	gen_hdr;

	// descriptor[N];

	// dvb_CRC32_t;

} dvb_sect_cat_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#endif  /* __SECT_CAT_H__ */
iff -ruN ethereal-0.10.7/dvb_incs/sect_eit.h ethereal-0.10.7_mpeg2_recent/dvb_incs/sect_eit.h
-- ethereal-0.10.7/dvb_incs/sect_eit.h	1970-01-01 01:00:00.000000000 +0100
++ ethereal-0.10.7_mpeg2_recent/dvb_incs/sect_eit.h	2004-12-02 16:14:18.549853161 +0100
@ -0,0 +1,149 @@
/* sect_eit.h
 * Routines supporting MPEG2 Transport Stream ansalyzation
 * Copyright 2004, Dipl.-Ing. Wolfram STERING <wolfi@xxxxxxxxxxxxxx>
 *
 * $Id: $
 *
 * Ethereal - Network traffic analyzer
 * By Gerald Combs <gerald@xxxxxxxxxxxx>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef __SECT_EIT_H__
#define __SECT_EIT_H__

#include <glib.h>
#include "sect_gen.h"

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_sect_eit {

	dvb_sect_gen_t	gen_hdr;

	char		transport_stream_id_hi;
	char		transport_stream_id_lo;
	char		original_network_id_hi;
	char		original_network_id_lo;
	
	char		segment_last_section_number;
	char		last_table_id;

	// dvb_sect_eit_entry_t[N];

	// dvb_CRC32_t;

} dvb_sect_eit_t attr_pack;


#define DVB_GET_EIT_TS_ID(t) \
  (((t)->transport_stream_id_hi << 8) | (t)->transport_stream_id_lo)

#define DVB_SET_EIT_TS_ID(t,i) \
do { \
  (t)->transport_stream_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->transport_stream_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)

#define DVB_GET_EIT_ONET_ID(t) \
  (((t)->original_network_id_hi << 8) | (t)->original_network_id_lo)

#define DVB_SET_EIT_ONET_ID(t,i) \
do { \
  (t)->original_network_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->original_network_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


typedef struct _dvb_sect_eit_entry {
	
	char		event_id_hi;
	char		event_id_lo;

	char		start_time_0;
	char		start_time_1;
	char		start_time_2;
	char		start_time_3;
	char		start_time_4;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		descriptor_loop_length_hi	:4;
	char		free_CA_mode				:1;
	char		running_status				:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		running_status				:3;
	char		free_CA_mode				:1;
	char		descriptor_loop_length_hi	:4;
#else
#error "Please specify the byte order!"
#endif

	char		descriptor_loop_length_lo;
	
	// descriptor[N]

} dvb_sect_eit_entry_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define DVB_GET_EIT_EVENT_ID(t) \
  (((t)->event_id_hi << 8) | (t)->event_id_lo)

#define DVB_SET_EIT_EVENT_ID(t,i) \
do { \
  (t)->event_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->event_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#define DVB_GET_EIT_START_MJD( t ) \
  (((t)->start_time_0 << 8) | (t)->start_time_1)

#define DVB_SET_EIT_START_MJD( t, s ) \
do { \
  (t)->start_time_0 = (((unsigned __int16)(s)) & 0xFF00) >> 8; \
  (t)->start_time_1 = (((unsigned __int16)(s)) & 0x00FF); \
} while (0)

#define DVB_GET_EIT_START_UTC( t ) \
  (((t)->start_time_2 << 16) | ((t)->start_time_3 << 8) || (t)->start_time_4)

#define DVB_SET_EIT_START_UTC( t, s ) \
do { \
  (t)->start_time_2 = (((unsigned int)(s)) & 0x00FF0000) >> 16; \
  (t)->start_time_3 = (((unsigned int)(s)) & 0x0000FF00) >> 8; \
  (t)->start_time_4 = (((unsigned int)(s)) & 0x000000FF); \
} while (0)


#define DVB_GET_EIT_DL_LEN(t) \
  (((t)->descriptor_loop_length_hi << 8) | (t)->descriptor_loop_length_lo)

#define DVB_SET_EIT_DL_LEN(t,i) \
do { \
  (t)->descriptor_loop_length_hi = (((unsigned __int16)(i)) & 0x0F00) >> 8; \
  (t)->descriptor_loop_length_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#endif  /* __SECT_EIT_H__ */
