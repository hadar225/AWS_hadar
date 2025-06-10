/* sect_sdt.h
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
#ifndef __SECT_SDT_H__
#define __SECT_SDT_H__

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
typedef struct _dvb_sect_sdt {

	dvb_sect_gen_t	gen_hdr;

	char		original_network_id_hi;
	char		original_network_id_lo;
	
	char		reserved_future_use;

	// dvb_sect_sdt_entry_t[N];

	// dvb_CRC32_t;

} dvb_sect_sdt_t attr_pack;

#define DVB_GET_SDT_ONET_ID(t) \
  (((t)->original_network_id_hi << 8) | (t)->original_network_id_lo)

#define DVB_SET_SDT_ONET_ID(t,i) \
do { \
  (t)->original_network_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->original_network_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


typedef struct _dvb_sect_sdt_entry {
	
	char		service_id_hi;
	char		service_id_lo;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		EIT_present_following_flag	:1;
	char		EIT_schedule_flag			:1;
	char		reserved_future_use			:6;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		reserved_future_use			:6;
	char		EIT_schedule_flag			:1;
	char		EIT_present_following_flag	:1;
#else
#error "Please specify the byte order!"
#endif

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

} dvb_sect_sdt_entry_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define DVB_GET_SDT_SERVICE_ID(t) \
  (((t)->service_id_hi << 8) | (t)->service_id_lo)

#define DVB_SET_SDT_SERVICE_ID(t,i) \
do { \
  (t)->service_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->service_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#define DVB_GET_SDT_DL_LEN(t) \
  (((t)->descriptor_loop_length_hi << 8) | (t)->descriptor_loop_length_lo)

#define DVB_SET_SDT_DL_LEN(t,i) \
do { \
  (t)->descriptor_loop_length_hi = (((unsigned __int16)(i)) & 0x0F00) >> 8; \
  (t)->descriptor_loop_length_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#endif  /* __SECT_SDT_H__ */