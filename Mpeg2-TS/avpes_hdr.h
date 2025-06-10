/* avpes_hdr.h
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

#ifndef __DVB_PS_HDR_H__
#define __DVB_PS_HDR_H__

#include "dvb_types.h"

/*
 * MPEG2 program stream, as defined by ISO-13818-1 (H.222.0), table 2-31:
 *
 * MPEG2_program_stream() {
 * 		do {
 *			pack()
 *		} while (nextbits() == pack_start_code)
 *		MPEG_program_end_code
 *	}
 *
 *
 * Program stream pack (table 2-32):
 *
 * pack() {
 *		pack_header()
 *		while (nextbits() == packet_start_code_prefix) {
 *			PES_packet()
 *		}
 *	}
 */

#define MPEG_PROGRAM_END_CODE	0x000001B9

/*
 * Generic program stream (PES, PACK) header:
 * Use stream_id to find out what you've actually got.
 */
typedef struct _dvb_ps_hdr {
	unsigned char	start_code_1;
	unsigned char	start_code_2;
	unsigned char	start_code_3;
	unsigned char	stream_id;
} dvb_ps_hdr_t;

#define GET_PS_START_CODE(p) \
  (((p)->start_code_1 << 16) | ((p)->start_code_2 << 8) | (p)->start_code_3)

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_pack_hdr {

	unsigned char	pack_start_code_1;
	unsigned char	pack_start_code_2;
	unsigned char	pack_start_code_3;
	unsigned char	pack_start_code_4;

#if __BYTE_ORDER == __LITTLE_ENDIAN

	unsigned char	system_clock_reference_base_2915_1	:2;
	unsigned char	marker_bit0							:1;		// 1b
	unsigned char	system_clock_reference_base_3230	:3;
	unsigned char	reserved0							:2;		// 01b

	unsigned char	system_clock_reference_base_2915_2;

	unsigned char	system_clock_reference_base_140_1	:2;
	unsigned char	marker_bit1							:1;		// 1b
	unsigned char	system_clock_reference_base_2915_3	:5;

	unsigned char	system_clock_reference_base_140_2;

	unsigned char	system_clock_reference_extension_1	:2;
	unsigned char	marker_bit2							:1;		// 1b
	unsigned char	system_clock_reference_base_140_3	:5;

	unsigned char	marker_bit3							:1;		// 1b
	unsigned char	system_clock_reference_extension_2	:7;

	unsigned char	program_mux_rate_1;							// measured in 50 B/s
	unsigned char	program_mux_rate_2;

	unsigned char	marker_bit5							:1;		// 1b
	unsigned char	marker_bit4							:1;		// 1b
	unsigned char	program_mux_rate_3					:6;

	unsigned char	pack_stuffing_length				:3;
	unsigned char	reserved1							:5;

#elif __BYTE_ORDER == __BIG_ENDIAN

	unsigned char	reserved0							:2;		// 01b
	unsigned char	system_clock_reference_base_3230	:3;
	unsigned char	marker_bit0							:1;		// 1b
	unsigned char	system_clock_reference_base_2915_1	:2;

	unsigned char	system_clock_reference_base_2915_2;

	unsigned char	system_clock_reference_base_2915_3	:5;
	unsigned char	marker_bit1							:1;		// 1b
	unsigned char	system_clock_reference_base_140_1	:2;

	unsigned char	system_clock_reference_base_140_2;

	unsigned char	system_clock_reference_base_140_3	:5;
	unsigned char	marker_bit2							:1;		// 1b
	unsigned char	system_clock_reference_extension_1	:2;

	unsigned char	system_clock_reference_extension_2	:7;
	unsigned char	marker_bit3							:1;		// 1b

	unsigned char	program_mux_rate_1;							// measured in 50 B/s
	unsigned char	program_mux_rate_2;

	unsigned char	program_mux_rate_3					:6;
	unsigned char	marker_bit4							:1;		// 1b
	unsigned char	marker_bit5							:1;		// 1b

	unsigned char	reserved1							:5;
	unsigned char	pack_stuffing_length				:3;

#else
#error "Please specify the byte order!"
#endif

	// unsigned char pack_stuffing_byte[pack_stuffing_length];	// all bytes 0xFF

} dvb_pack_hdr_t attr_pack;

#define PACK_START_CODE		0x000001BA

#define GET_PACK_START_CODE(p) \
  (((p)->pack_start_code_1 << 24) | ((p)->pack_start_code_2 << 16) | ((p)->pack_start_code_3 << 8) | (p)->pack_start_code_4)

#define GET_PACK_SCR_BASE(p) \
  ((unsigned long long)(((p)->system_clock_reference_base_3230 << 30) | ((p)->system_clock_reference_base_2915_1 << 28) | \
   ((p)->system_clock_reference_base_2915_2 << 20) | ((p)->system_clock_reference_base_2915_3 << 15) | \
   ((p)->system_clock_reference_base_140_1 << 13) | ((p)->system_clock_reference_base_140_2 << 5) | \
    (p)->system_clock_reference_base_140_3))

#define GET_PACK_SCR_EXT(p) \
  ((unsigned short)(((p)->system_clock_reference_extension_1 << 7) | (p)->system_clock_reference_extension_2))

#define GET_PACK_SCR(p) \
  ((unsigned long long)((GET_PACK_SCR_BASE(p) * 300) + GET_PACK_SCR_EXT(p)))

#define GET_PACK_PROGRAM_MUX_RATE(p) \
  ((unsigned long)(((p)->program_mux_rate_1 << 14) | ((p)->program_mux_rate_2 << 6) | (p)->program_mux_rate_3))


typedef struct _dvb_ps_system_hdr {

	unsigned char	system_header_start_code_1;
	unsigned char	system_header_start_code_2;
	unsigned char	system_header_start_code_3;
	unsigned char	system_header_start_code_4;

	unsigned char	system_header_length_1;
	unsigned char	system_header_length_2;

#if __BYTE_ORDER == __LITTLE_ENDIAN

	unsigned char	rate_bound_1						:7;
	unsigned char	marker_bit1							:1;		// 1b

	unsigned char	rate_bound_2;

	unsigned char	marker_bit2							:1;		// 1b
	unsigned char	rate_bound_3						:7;

	unsigned char	CSPS_flag							:1;
	unsigned char	fixed_flag							:1;
	unsigned char	audio_bound							:6;

	unsigned char	video_bound							:5;
	unsigned char	marker_bit3							:1;		// 1b
	unsigned char	system_video_lock_flag				:1;
	unsigned char	system_audio_lock_flag				:1;

	unsigned char	reserved_bits						:7;		// 111 1111b
	unsigned char	packet_rate_restriction_flag		:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	unsigned char	marker_bit1							:1;		// 1b
	unsigned char	rate_bound_1						:7;

	unsigned char	rate_bound_2;

	unsigned char	rate_bound_3						:7;
	unsigned char	marker_bit2							:1;		// 1b

	unsigned char	audio_bound							:6;
	unsigned char	fixed_flag							:1;
	unsigned char	CSPS_flag							:1;

	unsigned char	system_audio_lock_flag				:1;
	unsigned char	system_video_lock_flag				:1;
	unsigned char	marker_bit3							:1;		// 1b
	unsigned char	video_bound							:5;

	unsigned char	packet_rate_restriction_flag		:1;
	unsigned char	reserved_bits						:7;		// 111 1111b

#else
#error "Please specify the byte order!"
#endif

	/*
	 * 	while (nextbits() == '1') {
	 *		streadm_id	:8
	 *		'11'
	 *		P-STD_buffer_bound_scale	:1
	 *		P-STD_buffer_size_bound		:13
	 *	}
	 */

} dvb_ps_system_hdr_t attr_pack;

#define SYSTEM_HEADER_STREAM_ID		0xBB
#define SYSTEM_HEADER_START_CODE	0x000001BB

#define GET_SYS_HDR_START_CODE(p) \
  (((p)->system_header_start_code_1 << 24) | ((p)->system_header_start_code_2 << 16) | \
   ((p)->system_header_start_code_3 << 8) | (p)->system_header_start_code_4)

#define GET_SYS_HDR_LEN(p) \
  (((p)->system_header_length_1 << 8) | (p)->system_header_length_2)

#define GET_SYS_HDR_RATE_BOUND(p) \
  ((unsigned long)(((p)->rate_bound_1 << 15) | ((p)->rate_bound_2 << 7) | (p)->rate_bound_3))


typedef struct _dvb_system_hdr_2 {

	unsigned char	stream_id;

#if __BYTE_ORDER == __LITTLE_ENDIAN

	unsigned char	PSTD_buffer_size_bound_1		:5;
	unsigned char	PSTD_buffer_bound_scale			:1;
	unsigned char	reserved1						:2;		// 11b

#elif __BYTE_ORDER == __BIG_ENDIAN

	unsigned char	reserved1						:2;		// 11b
	unsigned char	PSTD_buffer_bound_scale			:1;
	unsigned char	PSTD_buffer_size_bound_1		:5;

#else
#error "Please specify the byte order!"
#endif

	unsigned char	PSTD_buffer_size_bound_2;

} dvb_system_hdr_2_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define GET_SYS_HDR_PSTD_BUF_SIZE_BOUND(p) \
  (((p)->PSTD_buffer_size_bound_1 << 8) | (p)->PSTD_buffer_size_bound_2)

#endif	/* __DVB_PS_HDR_H__ */