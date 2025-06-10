/* pes_hdr.h
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
#ifndef __DVB_PES_HDR_H__
#define __DVB_PES_HDR_H__

//#include <glib.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#else
#error "Please specify the byte order!"
#endif

#define PACKET_START_CODE_PREFIX	0x000001
#ifdef __GNUC__
#define attr_pack __attribute__((packed)) 
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_pes_hdr {

	char	packet_start_code_prefix_hi;
	char	packet_start_code_prefix_mid;
	char	packet_start_code_prefix_lo;

	char	stream_id;
	char	pes_packet_length_hi;
	char	pes_packet_length_lo;

} dvb_pes_hdr_t attr_pack;

/**
 * Get the PES packet start code prefix
 *
 * @param   p  pointer to dvb_pes_hdr_t.
 */
#define DVB_GET_PACKET_START_CODE_PREFIX(p) \
((((p)->packet_start_code_prefix_hi) << 16) | (((p)->packet_start_code_prefix_mid) << 8) | (p)->packet_start_code_prefix_lo)

/**
 * Get the PES packet length value
 *
 * @param   p  pointer to dvb_pes_hdr_t.
 */
#define DVB_GET_PES_LEN(p) \
  ((((p)->pes_packet_length_hi) << 8) | ((p)->pes_packet_length_lo))

/**
 * Get the PES packet length value
 *
 * @param   p  pointer to dvb_pes_hdr_t.
 * @param   l  pes lendth
 */
#define DVB_SET_PES_LEN(p,l) \
do { \
  (p)->pes_packet_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->pes_packet_length_hi = ((char)((((unsigned __int16)(l)) & 0xFF00) >> 8)); \
} while (0)


typedef struct _pes_flags {
#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	original_or_copy			:1;
	char	copyright					:1;
	char	data_alignment_indicator	:1;
	char	pes_priority				:1;
	char	pes_scrambling_control		:2;
	char	pes_start_code_prefix		:2;		// must be 0x02 (10b)

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pes_start_code_prefix		:2;		// must be 0x02 (10b)
	char	pes_scrambling_control		:2;
	char	pes_priority				:1;
	char	data_alignment_indicator	:1;
	char	copyright					:1;
	char	original_or_copy			:1;

#else
#error "Please specify the byte order!"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	pes_extension_flag			:1;
	char	pes_crc_flag				:1;
	char	additional_copy_info_flag	:1;
	char	dsm_trick_mode_flag			:1;
	char	es_rate_flag				:1;
	char	escr_flag					:1;
	char	pts_dts_flag				:2;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pts_dts_flag				:2;
	char	escr_flag					:1;
	char	es_rate_flag				:1;
	char	dsm_trick_mode_flag			:1;
	char	additional_copy_info_flag	:1;
	char	pes_crc_flag				:1;
	char	pes_extension_flag			:1;

#else
#error "Please specify the byte order!"
#endif

	char	pes_header_data_length		:8;

} pes_flags_t attr_pack;

#define DVB_NO_PTS_NO_DTS	0x00
#define DVB_PTS_ONLY		0x02
#define DVB_PTS_AND_DTS		0x03

typedef struct _pts_dts_flag_2 {
#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	marker_bit1					:1;
	char	pts_32_30					:3;
	char	pts_start_code_prefix		:4;		// must be 0010b

	char	pts_29_15_hi				:8;
	
	char	marker_bit2					:1;
	char	pts_29_15_lo				:7;

	char	pts_14_0_hi					:8;
	
	char	marker_bit3					:1;
	char	pts_14_0_lo					:7;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pts_start_code_prefix		:4;		// must be 0010b
	char	pts_32_30					:3;
	char	marker_bit1					:1;

	char	pts_29_15_hi				:8;

	char	pts_29_15_lo				:7;
	char	marker_bit2					:1;

	char	pts_14_0_hi					:8;

	char	pts_14_0_lo					:7;
	char	marker_bit3					:1;

#else
#error "Please specify the byte order!"
#endif

} pts_dts_flag2_t attr_pack;

/**
 * Get the PTS value.
 *
 * @param	pt	pointer to pts_dts_flag2_t or pts_dts_flag3_t
 */
#define DVB_GET_PTS(pt) \
  ((unsigned long long)(((pt)->pts_32_30 << 30) | ((pt)->pts_29_15_hi << 22) | ((pt)->pts_29_15_lo << 15) | \
   ((pt)->pts_14_0_hi << 7) | (pt)->pts_14_0_lo))

/**
 * Get the PTS 29 15 timestamp.
 *
 * @param   pt  pointer to pts_dts_flag2_t.
 */
#define DVB_GET_PTS_2915(pt) \
((((pt)->pts_29_15_hi) << 7) | ((pt)->pts_29_15_lo))

/**
 * Get the PTS 14 0 timestamp.
 *
 * @param   pt  pointer to pts_dts_flag2_t.
 */
#define DVB_GET_PTS_140(pt) \
((((pt)->pts_14_0_hi) << 7) | ((pt)->pts_14_0_lo))


 /**
 * Set the PTS_2915 value in a pes flag field.
 *
 * @param    pt  pointer to _pts_dts_flag_2.
 * @param    v   value
 */
#define DVB_SET_PTS_2915(pt, v) \
do { \
  (pt)->pts_29_15_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (pt)->pts_29_15_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)


/**
 * Set the PTS_140 value in a pes flag field.
 *
 * @param    pt  pointer to _pts_dts_flag_2
 * @param    v   value
 */
#define DVB_SET_PTS_140(pt, v) \
do { \
  (pt)->pts_14_0_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (pt)->pts_14_0_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)


typedef struct _pts_dts_flag_3 {
#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	marker_bit1					:1;
	char	pts_32_30					:3;
	char	pts_start_code_prefix		:4;		// must be 0010b

	char	pts_29_15_hi				:8;

	char	marker_bit2					:1;
	char	pts_29_15_lo				:7;

	char	pts_14_0_hi					:8;

	char	marker_bit3					:1;
	char	pts_14_0_lo					:7;

	char	marker_bit4					:1;
	char	dts_32_30					:3;
	char	dts_start_code_prefix		:4;		// must be 0001b

	char	dts_29_15_hi				:8;

	char	marker_bit5					:1;
	char	dts_29_15_lo				:7;

	char	dts_14_0_hi					:8;

	char	marker_bit6					:1;
	char	dts_14_0_lo					:7;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pts_start_code_prefix		:4;		// must be 0011b
	char	pts_32_30					:3;
	char	marker_bit1					:1;

	char	pts_29_15_hi				:8;

	char	pts_29_15_lo				:7;
	char	marker_bit2					:1;

	char	pts_14_0_hi					:8;

	char	pts_14_0_lo					:7;
	char	marker_bit3					:1;

	char	dts_start_code_prefix		:4;		// must be 0001b
	char	dts_32_30					:3;
	char	marker_bit4					:1;

	char	dts_29_15_hi				:8;

	char	dts_29_15_lo				:7;
	char	marker_bit5					:1;

	char	dts_14_0_hi					:8;

	char	dts_14_0_lo					:7;
	char	marker_bit6					:1;

#else
#error "Please specify the byte order!"
#endif

} pts_dts_flag3_t attr_pack;


/**
 * Get the DTS value.
 *
 * @param	pt	pointer to pts_dts_flag3_t
 */
#define DVB_GET_DTS(pt) \
  ((unsigned long long)(((pt)->dts_32_30 << 30) | ((pt)->dts_29_15_hi << 22) | ((pt)->dts_29_15_lo << 15) | \
   ((pt)->dts_14_0_hi << 7) | (pt)->dts_14_0_lo))

/**
 * Get the PTS 29 15 timestamp.
 *
 * @param   pt  pointer to pts_dts_flag2_t.
 */
#define DVB_GET_DTS_2915(pt) \
((((pt)->dts_29_15_hi) << 7) | ((pt)->dts_29_15_lo))

/**
 * Get the PTS 14 0 timestamp.
 *
 * @param   pt  pointer to pts_dts_flag2_t.
 */
#define DVB_GET_DTS_140(pt) \
((((pt)->dts_14_0_hi) << 7) | ((pt)->dts_14_0_lo))


typedef struct _escr_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	marker_bit1					:1;
	char	escr_base_29_15_hi			:2;
	char	escr_base_32_30				:3;
	char	escr_reserved				:2;

	char	escr_base_29_15_mid			:8;

	char	escr_base_14_0_hi			:2;
	char	marker_bit2					:1;
	char	escr_base_29_15_lo			:5;

	char	escr_base_14_0_mid			:8;

	char	escr_extension_hi			:2;
	char	marker_bit3					:1;
	char	escr_base_14_0_lo			:5;

	char	marker_bit4					:1;
	char	escr_extension_lo			:7;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	escr_reserved				:2;
	char	escr_base_32_30				:3;
	char	marker_bit1					:1;
	char	escr_base_29_15_hi			:2;

	char	escr_base_29_15_mid			:8;

	char	escr_base_29_15_lo			:5;
	char	marker_bit2					:1;
	char	escr_base_14_0_hi			:2;

	char	escr_base_14_0_mid			:8;

	char	escr_base_14_0_lo			:5;
	char	marker_bit3					:1;
	char	escr_extension_hi			:2;

	char	escr_extension_lo			:7;
	char	marker_bit4					:1;

#else
#error "Please specify the byte order!"
#endif

} escr_flag_t attr_pack;

#define DVB_GET_ESCR_BASE(e) \
  ((unsigned long long)(((e)->escr_base_32_30 << 30) | ((e)->escr_base_29_15_hi << 28) | ((e)->escr_base_29_15_mid << 20) | ((e)->escr_base_29_15_lo << 15) | ((e)->escr_base_14_0_hi << 13) | ((e)->escr_base_14_0_mid << 5) | (e)->escr_base_14_0_lo))

#define DVB_GET_ESCR_EXT(e) \
  ((unsigned short)(((e)->escr_extension_hi << 7) | (e)->escr_extension_lo))

#define DVB_GET_ESCR(e) \
  ((unsigned long long)((DVB_GET_ESCR_BASE(e) * 300) + DVB_GET_ESCR_EXT(e)))


typedef struct _es_rate_flag {
	// the elementary stream rate is a 22 bit unsigned int in units of 50 bytes/sec
#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	es_rate_hi			:7;
	char	marker_bit1			:1;

	char	es_rate_mid			:8;

	char	marker_bit2			:1;
	char	es_rate_lo			:7;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	marker_bit1			:1;
	char	es_rate_hi			:7;
	
	char	es_rate_mid			:8;

	char	es_rate_lo			:7;
	char	marker_bit2			:1;

#else
#error "Please specify the byte order!"
#endif

} es_rate_flag_t attr_pack;

#define DVB_GET_ES_RATE(e) \
  ((unsigned long)(((e)->es_rate_hi << 15) | ((e)->es_rate_mid << 7) | (e)->es_rate_lo))

typedef struct _dsm_trick_mode_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	union _mode {
		struct _fast_forward {
			char	frequency_truncation	:2;
			char	intra_slice_refresh		:1;
			char	field_id				:2;
		} fast_forward;
		
		struct _slow_motion {
			char	rep_cntrl				:5;
		} slow_motion;
		
		struct _freeze_frame {
			char	reserved 				:3;
			char	field_id				:2;
		} freeze_frame;
		
		struct _fast_reverse {
			char	frequency_truncation	:2;
			char	intra_slice_refresh 	:1;
			char	field_id				:2;
		} fast_reverse;
		
		struct _slow_reverse {
			char	rep_cntrl				:5;
		} slow_reverse;
	} mode;
	char	trick_mode_control				:3;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	trick_mode_control				:3;
	union _mode {
		struct _fast_forward {
			char	field_id				:2;
			char	intra_slice_refresh		:1;
			char	frequency_truncation	:2;
		} fast_forward;
		
		struct _slow_motion {
			uint	rep_cntrl				:5;
		} slow_motion;
		
		struct _freeze_frame {
			char	field_id				:2;
			char	reserved 				:3;
		} freeze_frame;
		
		struct _fast_reverse {
			char	field_id				:2;
			char	intra_slice_refresh 	:1;
			char	frequency_truncation	:2;
		} fast_reverse;
		
		struct _slow_reverse {
			char	rep_cntrl				:5;
		} slow_reverse;
	} mode;

#else
#error "Please specify the byte order!"
#endif

} dsm_trick_mode_flag_t attr_pack;

#define TRICK_MODE_FFWD		0x0		// fast forward
#define TRICK_MODE_SLMOT	0x1		// slow motion
#define TRICK_MODE_FRZFR	0x2		// freeze frame
#define TRICK_MODE_FREV		0x3		// fast reverse
#define TRICK_MODE_SLREV	0x4		// slow reverse


typedef struct _additional_copy_info_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	additional_copy_info			: 7;
	char	marker_bit						: 1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	marker_bit						: 1;
	char	additional_copy_info			: 7;

#else
#error "Please specify the byte order!"
#endif

} additional_copy_info_flag_t attr_pack;


typedef struct _pes_crc_flag {

	char	previous_pes_packet_crc_hi		:8;
	char	previous_pes_packet_crc_lo		:8;

} pes_crc_flag_t attr_pack;

#define DVB_GET_PES_CRC(c) \
  (((c)->previous_pes_packet_crc_hi << 8) | (c)->previous_pes_packet_crc_lo)


typedef struct _pes_extension_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	pes_extension_flag2						:1;
	char	reserved0								:3;
	char	pstd_buffer_flag						:1;
	char	program_packet_sequence_counter_flag	:1;
	char	pack_header_field_flag					:1;
	char	pes_private_data_flag					:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pes_private_data_flag					:1;
	char	pack_header_field_flag					:1;
	char	program_packet_sequence_counter_flag	:1;
	char	pstd_buffer_flag						:1;
	char	reserved0								:3;
	char	pes_extension_flag2						:1;
	
#else
#error "Please specify the byte order!"
#endif

} pes_extension_flag_t attr_pack;


// if pes_private_data_flag == 1
//		char	pes_private_data[16]


// if pes_pack_header_field_flag == 1
//
typedef struct _pes_pack_hdr_field_flag {

	char	pack_field_length;
	// char pack_header[pack_field_length];

} pes_pack_hdr_field_flag_t attr_pack;


// if program_packet_sequence_counter_flag == 1
//
typedef struct _program_packet_sequence_counter_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN


	char	program_packet_squence_counter	:7;
	char	marker_bit1						:1;

	char	original_stuff_length			:6;
	char	mpeg1_mpeg2_identifier			:1;
	char	marker_bit2						:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	marker_bit1						:1;
	char	program_packet_squence_counter	:7;

	char	marker_bit2						:1;
	char	mpeg1_mpeg2_identifier			:1;
	char	original_stuff_length			:6;

#else
#error "Please specify the byte order!"
#endif

} program_packet_sequence_counter_flag_t attr_pack;


// if psdt_buffer_flag == 1
//
typedef struct _pstd_buffer_flag {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	pstd_buffer_size_hi		:5;
	char	pstd_buffer_scale		:1;
	char	reserved0				:2;		// must be 01b

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	reserved0				:2;		// must be 01b
	char	pstd_buffer_scale		:1;
	char	pstd_buffer_size_hi		:5;

#else
#error "Please specify the byte order!"
#endif

	char	pstd_buffer_size_lo		:8;

} pstd_buffer_flag_t attr_pack;

#define GET_PSTD_BUFFER_SIZE(p) (((p)->pstd_buffer_size_hi << 8) | (p)->pstd_buffer_size_lo)


// if pes_extension_flag2 == 1
//
typedef struct _pes_extension_flag2 {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	pes_extension_field_length	:7;
	char	marker_bit					:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	marker_bit					:1;
	char	pes_extension_field_length	:7;

#else
#error "Please specify the byte order!"
#endif

	// char pes_extension_field_data[pes_extension_field_length];

} pes_extension_flag2_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define STREAM_PROGRAM_END		0xB9	// PS END: 000001B9
#define STREAM_PACK				0xBA
#define STREAM_SYS_HDR			0xBB
#define STREAM_PROGRAM_MAP		0xBC
#define STREAM_PRIVATE_1		0xBD
#define STREAM_PADDING			0xBE
#define STREAM_PRIVATE_2		0xBF
#define STREAM_AUDIO_START		0xC0	// C0..DF: 110X XXXX
#define STREAM_AUDIO_END		0xDF
#define STREAM_VIDEO_START		0xE0	// E0..EF: 1110 XXXX
#define STREAM_VIDEO_END		0xEF
#define STREAM_ECM				0xF0
#define STREAM_EMM				0xF1
#define STREAM_DSMCC			0xF2
#define STREAM_ITU_A			0xF4	
#define STREAM_ITU_B			0xF5	
#define STREAM_ITU_C			0xF6	
#define STREAM_ITU_D			0xF7	
#define STREAM_ITU_E			0xF8	
#define STREAM_ANCILLARY		0xF9
#define STREAM_RESERVED_START	0xFA
#define STREAM_RESERVED_END		0xFE
#define STREAM_PROGRAM_DIR		0xFF

#define STREAM_AUDIO_MASK		0xE0
#define STREAM_AUDIO_NR			0xC0
#define STREAM_VIDEO_MASK		0xF0
#define STREAM_VIDEO_NR			0xE0


#endif   /* __DVB_PES_HDR_H__ */