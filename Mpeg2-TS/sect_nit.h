/* sect_nit.h
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
#ifndef __SECT_NIT_H__
#define __SECT_NIT_H__

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
typedef struct _dvb_sect_nit {

	dvb_sect_gen_t	gen_hdr attr_pack;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		network_descriptors_length_hi	:4;
	char		reserved_future_use		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		reserved_future_use		:4;
	char		network_descriptors_length_hi	:4;
#else
#error "Please specify the byte order!"
#endif
    
	char		network_descriptors_length_lo;

	// netowrk descriptor[N]

	// reserved
	//               >  dvb_sect_nit_ext_t  (see below)
	// loop_length

	// TS descriptors[N1]

	// dvb_CRC32_t;

} dvb_sect_nit_t attr_pack;


#define DVB_GET_NIT_NETDESC_LEN(p) \
  (((p)->network_descriptors_length_hi << 8) | (p)->network_descriptors_length_lo)

#define DVB_SET_NIT_NETDESC_LEN(p,l) \
do { \
  (p)->network_descriptors_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->network_descriptors_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)


typedef struct _dvb_sect_nit_ext {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		transport_stream_loop_length_hi	:4;
	char		reserved_future_use		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		reserved_future_use		:4;
	char		transport_stream_loop_length_hi	:4;
#else
#error "Please specify the byte order!"
#endif

	char		transport_stream_loop_length_lo;

} dvb_sect_nit_ext_t attr_pack;


#define DVB_GET_NIT_TSL_LEN(p) \
  (((p)->transport_stream_loop_length_hi << 8) | (p)->transport_stream_loop_length_lo)

#define DVB_SET_NIT_TSL_LEN(p,l) \
do { \
  (p)->transport_stream_loop_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->transport_stream_loop_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)



typedef struct _dvb_sect_nit_entry {

	char	transport_stream_id_hi;
	char	transport_stream_id_lo;

	char	original_network_id_hi;
	char	original_network_id_lo;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	transport_descriptors_length_hi	:4;
	char	reserved_future_use		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved_future_use		:4;
	char	transport_descriptors_length_hi	:4;
#else
#error "Please specify the byte order!"
#endif
	char	transport_descriptors_length_lo;

} dvb_sect_nit_entry_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define DVB_GET_NIT_TS_ID(p) \
  ((((p)->transport_stream_id_hi) << 8) | (p)->transport_stream_id_lo)

#define DVB_SET_NIT_TS_ID(p,i) \
do { \
  (p)->transport_stream_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (p)->transport_stream_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#define DVB_GET_NIT_ONET_ID(p) \
  ((((p)->original_network_id_hi) << 8) | (p)->original_network_id_lo)

#define DVB_SET_NIT_ONET_ID(p,i) \
do { \
  (p)->original_network_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (p)->original_network_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


#define DVB_GET_NIT_TSDESC_LEN(p) \
  (((p)->transport_descriptors_length_hi << 8) | (p)->transport_descriptors_length_lo)

#define DVB_SET_NIT_TSDESC_LEN(p,l) \
do { \
  (p)->transport_descriptors_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->transport_descriptors_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)



#endif  /* __SECT_PMT_H__ */