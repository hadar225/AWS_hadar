/* sect_tdt.h
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
#ifndef __SECT_TDT_H__
#define __SECT_TDT_H__

#include <glib.h>

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_tdt {

	char		table_id;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		section_length_hi:			4;
	char		reserved:					2;
	char		reserved_future_use:		1;
	char		section_syntax_indicator:	1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		section_syntax_indicator:	1;
	char		reserved_future_use:		1;
	char		reserved:					2;
	char		section_length_hi:			4;
#else
#error "Please specify the byte order!"
#endif

	char		section_length_lo;

	char		UTC_time_0;
	char		UTC_time_1;
	char		UTC_time_2;
	char		UTC_time_3;
	char		UTC_time_4;

    // NOTE: The TDT section has no checksum and the
	// section_syntax_indicator must be set to '0'.

} dvb_sect_tdt_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define DVB_GET_TDT_MJD( t ) \
  (((t)->UTC_time_0 << 8) | (t)->UTC_time_1)

#define DVB_SET_TDT_MJD( t, s ) \
do { \
  (t)->UTC_time_0 = (((unsigned __int16)(s)) & 0xFF00) >> 8; \
  (t)->UTC_time_1 = (((unsigned __int16)(s)) & 0x00FF); \
} while (0)

#define DVB_GET_TDT_UTC( t ) \
  (((t)->UTC_time_2 << 16) | ((t)->UTC_time_3 << 8) | (t)->UTC_time_4)

#define DVB_SET_TDT_UTC( t, s ) \
do { \
  (t)->UTC_time_2 = (((unsigned int)(s)) & 0x00FF0000) >> 16; \
  (t)->UTC_time_3 = (((unsigned int)(s)) & 0x0000FF00) >> 8; \
  (t)->UTC_time_4 = (((unsigned int)(s)) & 0x000000FF); \
} while (0)


#endif  /* __SECT_TDT_H__ */