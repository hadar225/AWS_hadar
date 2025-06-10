/* sect_ssc.h
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
#ifndef __SECT_SSC_H__
#define __SECT_SSC_H__

#include <glib.h>


#define	DVB_SECT_SSC_VERSION	0x11

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_ssc {

	char	table_id;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	section_length_hi			:4;
	char	reserved0					:2;
	char	authority_indicator			:1;
	char	section_syntax_indicator	:1;		// always 0
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	section_syntax_indicator	:1;		// always 0
	char	authority_indicator			:1;
	char	reserved0					:2;
	char	section_length_hi			:4;
#else
#error "Please specify the byte order!"
#endif

	char	section_length_lo;

	char	station_id_32_0;
	char	station_id_32_1;
	char	station_id_32_2;
	char	station_id_32_3;

	char	option_field_length;

	//	for (i=0;i<option_field_length;i++) {
	//		char	option_field_data
	//	}
	//	scmp_packet_data()
	//	dvb_CRC32_t

} dvb_sect_ssc_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

/**
 * @param  ps   pointer to dvb_sect_ssc_t
 */
#define DVB_GET_SSC_SECTION_LENGTH(ps) \
  ((((ps)->section_length_hi) << 8) | (ps)->section_length_lo)

/**
 * @param  ps   pointer to dvb_sect_ssc_t.
 * @param  l    length to set in section header.
 */
#define DVB_SET_SSC_SECTION_LENGTH(ps,l) \
do { \
  (ps)->section_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (ps)->section_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)


/**
 * @param  ps   pointer to dvb_sect_ssc_t
 */
#define DVB_GET_SSC_STATION_ID_32(ps) \
  ((((ps)->station_id_32_0) << 24) | (((ps)->station_id_32_1) << 16) | \
	(((ps)->station_id_32_2) << 8) | (ps)->station_id_32_3)


/**
 * @param  ps   pointer to dvb_sect_ssc_t.
 * @param  l    station id.
 */
#define DVB_SET_SSC_STATION_ID_32(ps,l) \
do { \
  (ps)->station_id_32_3 = ((char)(((unsigned int)(l)) & 0x000000FF)); \
  (ps)->station_id_32_2 = ((char)((((unsigned int)(l)) & 0x0000FF00) >> 8)); \
  (ps)->station_id_32_1 = ((char)((((unsigned int)(l)) & 0x00FF0000) >> 16)); \
  (ps)->station_id_32_0 = ((char)((((unsigned int)(l)) & 0xFF000000) >> 24)); \
} while (0)


#endif  /* __SECT_SSC_H__ */