/* sect_mpe.h
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

#ifndef __SECT_MPE_H__
#define __SECT_MPE_H__

#include <glib.h>

/*
 * datagramm_section as defined in 
 * ETSI EN 301 192 V1.2.1 (1999-06), table 3.
 */
#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_mpe3e {

	char	table_id;
    
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	section_length_hi        :4;
	char	reserved0                :2;
	char	private_indicator        :1;
	char	section_syntax_indicator :1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	section_syntax_indicator :1;
	char	private_indicator        :1;
	char	reserved0                :2;
	char	section_length_hi        :4;
#else
#error "Please specify the byte order!"
#endif

	char	section_length_lo;

 	unsigned char MAC_hi[2];	

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	current_next_indicator   	:1;
	char	LLC_SNAP_flag				:1;
	char	address_scrambling_control	:2;
	char	payload_scrambling_control	:2;
	char	reserved1                	:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved1                	:2;
	char	payload_scrambling_control	:2;
	char	address_scrambling_control	:2;
	char	LLC_SNAP_flag				:1;
	char	current_next_indicator   	:1;
#else
#error "Please specify the byte order!"
#endif
    
	char	section_number;
	char	last_section_number;

	unsigned char MAC_lo[4];

	// if (LLC_SNAP_flag == 1)
	//    LLC_SNAP()
	// else
	//    IP datagram byte[N1]

	// if (section_number == last_section_number)
	//    stuffing_byte[N2]

	// if (section_syntax_indicator == '0')
	//    checksum
	// else
	//    CRC_32

} dvb_sect_mpe3e_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif


/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 */
#define DVB_GET_SECTION_LENGTH(ps) \
  ((((ps)->section_length_hi) << 8) | (ps)->section_length_lo)

#endif   /* __SECT_MPE_H__ */