/* descriptors.h
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

#ifndef __DESCRIPTORS_H__
#define __DESCRIPTORS_H__

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * DVB Stream Content values:
 */
#define DVB_STREAM_CONTENT_VIDEO	0x01
#define DVB_STREAM_CONTENT_AUDIO	0x02
#define DVB_STREAM_CONTENT_TELETEXT	0x03

/**
 * DVB Component Type values:
 */
#define DVB_COMP_TYPE_V_4_3		0x01	// video 4:3 aspect ratio
#define DVB_COMP_TYPE_V_16_9_PV		0x02	// video 16:9 aspect ratio with pan vectors
#define DVB_COMP_TYPE_v_19_9		0x03	// video 16:9 aspect ratio without pan vectors
// 0x04 - 0xFF: Reserved for future use

#define DVB_COMP_TYPE_A_MONO1		0x01	// audio: single mono channel
#define DVB_COMP_TYPE_A_MONO2		0x02	// audio: dual mono channel
#define DVB_COMP_TYPE_A_STEREO		0x03	// audio: stereo
#define DVB_COMP_TYPE_A_MULTI		0x04	// audio: multilungual multichannel
#define DVB_COMP_TYPE_A_SURROUND	0x05	// audio: surround
// 0x06 - 0x3F: Reserved
#define DVB_COMP_TYPE_A_VIMPAIRED	0x40	// audio: for visually impaired
#define DVB_COMP_TYPE_A_AIMAPIRED	0x41	// audio: for the hard of hearing
// 0x42 - 0xAF: Reserved for future use
// 0xB0 - 0xFE: User defined
// 0xFF: Reserved for future use

#define DVB_COMP_TYPE_T_EBU_TTST	0x01	// EBU Teletext subtitles
#define DVB_COMP_TYPE_T_AEBU_TT		0x02	// Associated EBU Teletext
// 0x03 - 0x0F: Reserved for future use
#define DVB_COMP_TYPE_T_DVB_ST		0x10	// DVB Subtitles (normal) with no monitor aspect ratio criticality
#define DVB_COMP_TYPE_T_DVB_ST_4_3	0x11	// DVB Subtitles (normal) for display on 4:3 aspect ratio monitor
#define DVB_COMP_TYPE_T_DVB_ST_16_9 	0x12	// DVB Subtitles (normal) for display on 16:9 aspect ratio monitor
#define DVB_COMP_TYPE_T_DVB_ST_221_1 	0x13	// DVB Subtitles (normal) for display on 2.21:1 aspect ration monitor
// 0x14 - 0x1F: Reserved for future use
#define DVB_COMP_TYPE_T_DVB_HH_ST	0x10	// DVB Subtitles (hard of hearing) with no monitor aspect ratio criticality
#define DVB_COMP_TYPE_T_DVB_HH_ST_4_3	0x11	// DVB Subtitles (hard of hearing) for display on 4:3 aspect ratio monitor
#define DVB_COMP_TYPE_T_DVB_HH_ST_16_9	0x12 // DVB Subtitles (hard of hearing) for display on 16:9 aspect ratio monitor
#define DVB_COMP_TYPE_T_DVB_HH_ST_221_1 0x13 // DVB Subtitles (hard of hearing) for display on 2.21:1 aspect ration monitor


/**
 * Generic DVB descriptor type.
 * All DVB descriptors share this generic information
 * in the first two bytes.
 */
#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_descr_gen {

	char	descriptor_tag;
	char	descriptor_length;

	// descriptor data

} dvb_descr_gen_t attr_pack;


/**
 * Values for descriptor_tag:
 */
#define DVB_DESCR_TAG_VIDEO_STREAM		0x02	// PMT
#define DVB_DESCR_TAG_AUDIO_STREAM		0x03	// PMT
#define DVB_DESCR_TAG_HIERARCHY			0x04	// PMT
#define DVB_DESCR_TAG_REGISTRATION		0x05	// PMT
#define DVB_DESCR_TAG_DATA_STREAM_ALIGN	0x06	// PMT
#define DVB_DESCR_TAG_TARGET_BG_GRID	0x07	// PMT
#define DVB_DESCR_TAG_VIDEO_WINDOW		0x08	// PMT
#define DVB_DESCR_TAG_CA				0x09	// PMT
#define DVB_DESCR_TAG_ISO_639_LANG		0x0A	// PMT
#define DVB_DESCR_TAG_SYSTEM_CLOCK		0x0B	// PMT
#define DVB_DESCR_TAG_MPX_BUF_UTIL		0x0C	// PMT
#define DVB_DESCR_TAG_COPYRIGHT			0x0D	// PMT
#define DVB_DESCR_TAG_MAX_BITRATE		0x0E	// PMT
#define DVB_DESCR_TAG_PRIVATE_DATA		0x0F	// PMT
#define DVB_DESCR_TAG_SMOOTHING_BUF		0x10	// PMT
#define DVB_DESCR_TAG_SDT				0x11	// PMT
#define DVB_DESCR_TAG_IBP				0x12	// PMT

#define DVB_DESCR_TAG_CAROUSEL_ID		0x13	// PMT, DSM-CC
#define DVB_DESCR_TAG_ASSOC_TAG			0x14
#define DVB_DESCR_TAG_DEFERRED_ASSOC	0x15

#define DVB_DESCR_TAG_NETWORK_NAME		0x40	// NIT
#define DVB_DESCR_TAG_SERVICE_LIST		0x41	// NIT
#define DVB_DESCR_TAG_STUFFING			0x42	// NIT, SDT
#define DVB_DESCR_TAG_SAT_DELIV_SYS		0x43	// NIT
#define DVB_DESCR_TAG_CABLE_DELIV_SYS	0x44	// NIT
#define DVB_DESCR_TAG_VBI_DATA			0x45	// PMT
#define DVB_DESCR_TAG_VBI_TELETEXT		0x46	// PMT
#define DVB_DESCR_TAG_BOUQUET_NAME		0x47	// SDT
#define DVB_DESCR_TAG_SERVICE			0x48	// SDT
#define DVB_DESCR_TAG_COUNTRY_AVAIL		0x49	// SDT
#define DVB_DESCR_TAG_LINKAGE			0x4A	// NIT, SDT
#define DVB_DESCR_TAG_NVOD_REFERENCE	0x4B	// SDT
#define DVB_DESCR_TAG_TIME_SHIFTED		0x4C	// SDT
#define DVB_DESCR_TAG_SHORT_EVENT		0x4D	// EIT, SIT
#define DVB_DESCR_TAG_EXTENDED_EVENT	0x4E	// EIT, SIT
#define DVB_DESCR_TAG_TSHIFT_EVENT		0x4F	// EIT, SIT
#define DVB_DESCR_TAG_COMPONENT			0x50	// EIT, SIT
#define DVB_DESCR_TAG_MOSAIC			0x51	// PMT, SDT
#define DVB_DESCR_TAG_STREAM_ID			0x52	// PMT
#define DVB_DESCR_TAG_CA_ID				0x53	// SDT
#define DVB_DESCR_TAG_CONTENT			0x54	// EIT, SIT
#define DVB_DESCR_TAG_PARENTAL_RATING	0X55	// EIT, SIT
#define DVB_DESCR_TAG_TELETEXT			0x56	// PMT
#define DVB_DESCR_TAG_TELEPHONE			0x57	// SDT
#define DVB_DESCR_TAG_LOC_TIME_OFF		0x58	// TOT
#define DVB_DESCR_TAG_SUBTITLING		0x59	// PMT
#define DVB_DESCR_TAG_TERR_DELIV_SYS	0x5A	// NIT
#define DVB_DESCR_TAG_MULING_NET_NAME	0x5B	// NIT
#define DVB_DESCR_TAG_MULING_BOU_NAME	0x5C	// BAT
#define DVB_DESCR_TAG_MULING_SERV_NAME	0x5D	// SDT
#define DVB_DESCR_TAG_MULING_COMP		0x5E	// EIT, SIT
#define DVB_DESCR_TAG_PRIV_DATA_SPEC	0x5F	// PMT, NIT, SDT
#define DVB_DESCR_TAG_SERVICE_MOVE		0x60	// PMT
#define DVB_DESCR_TAG_FREQ_LIST			0x62	// NIT
#define DVB_DESCR_TAG_DATA_BROADCAST	0x64	// SDT
#define DVB_DESCR_TAG_CA_SYSTEM			0x65	// PMT
#define DVB_DESCR_TAG_DATA_BROADCAST_ID	0x66	// PMT
#define DVB_DESCR_TAG_PDC				0x69	// EIT
#define DVB_DESCR_TAG_AC3				0x6A	// PMT
#define DVB_DESCR_TAG_APP_SIG			0x6F	// PMT


/**
 * Values for descriptor_tag for MHP descriptors.
 * They are most likely encounteren in AIT's and the descriptor loops of DII messages.
 */
#define DVB_DESCR_MHP_APP				0x00
#define DVB_DESCR_MHP_APPNAME			0x01
#define DVB_DESCR_MHP_TRPROT			0x02
#define DVB_DESCR_MHP_DVBJAPP			0x03
#define DVB_DESCR_MHP_DVBJAPPLOC		0x04
#define DVB_DESCR_MHP_APPICON			0x0B
#define DVB_DESCR_MHP_LABEL				0x70
#define DVB_DESCR_MHP_CACHEPRI			0x71
#define DVB_DESCR_DSMCC_COMPRESSED		0x09


/**
 * Video Stream Descriptor
 */
typedef struct _dvb_descr_video_stream {

	dvb_descr_gen_t	gen_descr;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		still_picture_flag			:1;
	char		constrained_parameter_flag	:1;
	char		MPEG_1_only_flag			:1;
	char		frame_rate_code				:4;	// defined in H.262
	char		multiple_frame_rate_flag	:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		multiple_frame_rate_flag	:1;
	char		frame_rate_code				:4;	// defined in H.262
	char		MPEG_1_only_flag			:1;
	char		constrained_parameter_flag	:1;
	char		still_picture_flag			:1;
#else
#error "Please specify the byte order!"
#endif
	// if MPEG_1_only_flag == 1
	//   dvb_descr_vs_mpeg1
} dvb_descr_video_stream_t attr_pack;

typedef struct _dvb_descr_vs_mpeg1 {
	char		profile_and_level_indication;
    
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		reserved					:5;
	char		frame_rate_extension_flag	:1;
	char		chroma_format				:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		chroma_format				:2;
	char		frame_rate_extension_flag	:1;
	char		reserved					:5;
#else
#error "Please specify the byte order!"
#endif
    
} dvb_descr_vs_mpeg1_t attr_pack;


/**
 * Audio Stream Descriptor.
 */
typedef struct _dvb_descr_audio_stream {
    
	dvb_descr_gen_t	gen_descr;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		reserved						:3;
	char		variable_rate_audio_indicator	:1;
	char		layer							:2;
	char		ID								:1;
	char		free_format_flag				:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		free_format_flag				:1;
	char		ID								:1;
	char		layer							:2;
	char		variable_rate_audio_indicator	:1;
	char		reserved						:3;
#else
#error "Please specify the byte order!"
#endif
    
} dvb_descr_audio_stream_t attr_pack;


/**
 * The DVB Mosaic Descriptor.
 */
typedef struct _dvb_descr_mosaic {
    
	dvb_descr_gen_t	gen_descr;
    
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		number_of_vertical_elementary_cells		:3;
	char		reserved_future_use						:1;
	char		number_of_horizontal_elementary_cells 	:3;
	char		mosaic_entry_point						:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		mosaic_entry_point						:1;
	char		number_of_horizontal_elementary_cells 	:3;
	char		reserved_future_use						:1;
	char		number_of_vertical_elementary_cells		:3;
#else
#error "Please specify the byte order!"
#endif

	// Cell description...

} dvb_descr_mosaic_t attr_pack;


/**
 * The DVB Stream ID Descriptor.
 */
typedef struct _dvb_descr_stream_id {
    
	dvb_descr_gen_t	gen_descr;

	char		component_tag;

} dvb_descr_stream_id_t attr_pack;

/**
 * The DVB PDC Descriptor
 * This one contains the program deliverey control
 */

typedef struct _pil {
	dvb_descr_gen_t gen_descr;
#if __BYTE_ORDER == __LITTLE_ENDIAN	
	unsigned int day_hi 			:4;
	unsigned int reserved			:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int reserved			:4;
	unsigned int day_hi			:4;
#else
#error "Please specify the byte order!"
#endif
	
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int hour_hi			:3;
	unsigned int month				:4;
	unsigned int day_lo			:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int day_lo			:1;
	unsigned int month				:4;
	unsigned int hour_hi			:3;
#else
#error "Please specify the byte order!"
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int minute			:6;
	unsigned int hour_lo			:2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	unsigned int hour_lo			:2;
	unsigned int minute			:6;
#else
#error "Please specify the byte order!"
#endif
} pil_t attr_pack;

/**
 * The DVB Teletext Descriptor.
 * This one contains an array of teletext_entry blocks.
 */
typedef struct _dvb_descr_teletext_entry {

	char	ISO_639_language_code_hi;
	char	ISO_639_language_code_med;
	char	ISO_639_language_code_lo;
    
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	teletext_magazine_number	:3;
	char	teletext_type				:5;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	teletext_type				:5;
	char	teletext_magazine_number	:3;
#else
#error "Please specify the byte order!"
#endif
	char	teletext_page_number;

} dvb_descr_teletext_entry_t attr_pack;

/**
 * Get DVB Teletext Descriptor's langauge code (24 bits) number.
 * @param   t   pointer to dvb_descr_teletext_entry_t.
 */
#define DVB_GET_TT_LANG_CODE(t) \
  (((t)->ISO_639_language_code_hi << 16) | ((t)->ISO_639_language_code_med << 8) | ((t)->ISO_639_language_code_lo))

/**
 * Set DVB Teletext Descriptor's languate code (24 bits) number.
 * @param   t   pointer to dvb_descr_teletext_entry_t.
 * @param   lc  language code (int, 24 bits signigicant).
 */
#define DVB_SET_TT_LANG_CODE(t,lc) \
do { \
  (t)->ISO_639_language_code_hi  = (((unsigned int)(lc)) & 0x00FF0000) >> 16; \
  (t)->ISO_639_language_code_med = (((unsigned int)(lc)) & 0x0000FF00) >> 8; \
  (t)->ISO_639_language_code_lo  = (((unsigned int)(lc)) & 0x000000FF); \
} while (0)


typedef struct _dvb_descr_teletext {
    
	dvb_descr_gen_t	gen_descr;

	// dvb_descr_teletext_entry_t[N]

} dvb_descr_teletext_t attr_pack;

/**
 * Values for dvb_descr_teletext_entry_t's teletext_type field:
 */
#define DVB_TELETEXT_TYPE_INITIAL	0x01
#define DVB_TELETEXT_TYPE_SUBTITLE	0x02
#define DVB_TELETEXT_TYPE_INFO		0x03
#define DVB_TELETEXT_TYPE_SCHEDULE	0x04
#define DVB_TELETEXT_TYPE_IMPAIRED	0x05


/**
 * The DVB Subtitling Descriptor.
 * Contains an array of dvb_descr_subtitling_entry's.
 */
typedef struct _dvb_descr_subtitling_entry {

	char	ISO_639_language_code_hi;
	char	ISO_639_language_code_med;
	char	ISO_639_language_code_lo;
    
	char	subtitling_type;
	char	composition_page_id_hi;
	char	composition_page_id_lo;
	char	ancillary_page_id_hi;
	char	ancillary_page_id_lo;

} dvb_descr_subtitling_entry_t attr_pack;

/**
 * Get DVB Teletext Descriptor's langauge code (24 bits) number.
 * @param   t   pointer to dvb_descr_teletext_entry_t.
 */
#define DVB_GET_ST_LANG_CODE(t) \
  (((t)->ISO_639_language_code_hi << 16) | ((t)->ISO_639_language_code_med << 8) | ((t)->ISO_639_language_code_lo))

/**
 * Set DVB Teletext Descriptor's languate code (24 bits) number.
 * @param   t   pointer to dvb_descr_teletext_entry_t.
 * @param   lc  language code (int, 24 bits signigicant).
 */
#define DVB_SET_ST_LANG_CODE(t,lc) \
do { \
  (t)->ISO_639_language_code_hi  = (((unsigned int)(lc)) & 0x00FF0000) >> 16; \
  (t)->ISO_639_language_code_med = (((unsigned int)(lc)) & 0x0000FF00) >> 8; \
  (t)->ISO_639_language_code_lo  = (((unsigned int)(lc)) & 0x000000FF); \
} while (0)

#define DVB_GET_ST_COMPOS_PAGE(s) \
  (((s)->composition_page_id_hi << 8) | (s)->composition_page_id_lo)

#define DVB_SET_ST_COMPOS_PAGE(s,cp) \
do { \
  (s)->composition_page_id_hi = (((unsigned __int16)(cp)) & 0xFF00) >> 8; \
  (s)->composition_page_id_lo = (((unsigned __int16)(cp)) & 0x00FF); \
} while (0)

#define DVB_GET_ST_ANCILL_PAGE(s) \
  (((s)->ancillary_page_id_hi << 8) | (s)->ancillary_page_id_lo)

#define DVB_SET_ST_ANCILL_PAGE(s,cp) \
do { \
  (s)->ancillary_page_id_hi = (((unsigned __int16)(cp)) & 0xFF00) >> 8; \
  (s)->ancillary_page_id_lo = (((unsigned __int16)(cp)) & 0x00FF); \
} while (0)

typedef struct _dvb_descr_subtitling {
    
	dvb_descr_gen_t	gen_descr;

	// dvb_descr_subtitling_entry_t[N]

} dvb_descr_subtitling_t attr_pack;


/**
 * The DVB Private Data Descriptor.
 */
typedef struct _dvb_descr_private_data {
    
	dvb_descr_gen_t	gen_descr;

	unsigned int		private_data_specifier;

} dvb_descr_private_data_t attr_pack;

#define DVB_GET_DESCR_PRIV_DATA(t) \
  g_ntohl( (t)->private_data_specifier )

#define DVB_SET_DESCR_PRIV_DATA(t,d) \
  (t)->private_data_specifier = htonl( (unsigned int)(d) )


/**
 * The DVB Service Move Descriptor.
 */
typedef struct _dvb_descr_service_move {
    
	dvb_descr_gen_t	gen_descr;

	unsigned __int16		new_original_network_id;
	unsigned __int16		new_transport_stream_id;
	unsigned __int16		new_service_id;

} dvb_descr_service_move_t attr_pack;

#define DVB_GET_DESCR_SERVICE_MOVE_NID(t) \
  g_ntohs( (t)->new_original_network_id )

#define DVB_SET_DESCR_SERVICE_MODE_NID(t,d) \
  (t)->new_original_network_id = htons( (unsigned __int16)(d) )

#define DVB_GET_DESCR_SERVICE_MOVE_TSID(t) \
  g_ntohs( (t)->new_transport_stream_id )

#define DVB_SET_DESCR_SERVICE_MODE_TSID(t,d) \
  (t)->new_transport_stream_id = htons( (unsigned __int16)(d) )

#define DVB_GET_DESCR_SERVICE_MOVE_SID(t) \
  g_ntohs( (t)->new_service_id )

#define DVB_SET_DESCR_SERVICE_MODE_SID(t,d) \
  (t)->new_service_id = htons( (unsigned __int16)(d) )


/**
 * The DVB CA System Descriptor.
 */
typedef struct _dvb_descr_ca_system {
    
	dvb_descr_gen_t	gen_descr;

	// CA_system_id[N]       described in ETR162

} dvb_descr_ca_system_t attr_pack;

/**
 * The CA Identifier Descriptor.
 */
typedef struct _dvb_descr_ca_id {

	dvb_descr_gen_t	gen_descr;

	// unsigned __int16 CA_id[N]       described in ETR162

} dvb_descr_ca_id_t attr_pack;

#define DVB_GET_DESCR_CA_ID(t) \
 ((t)[0] << 8 | (t)[1])

#define DVB_SET_DESCR_CA_ID(t,i) \
do { \
  (t)[0] = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)[1] = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)


/**
 * The DVB Data Broadcast ID Descriptor.
 */
typedef struct _dvb_descr_data_broadcast_id {
    
	dvb_descr_gen_t	gen_descr;

	unsigned __int16		data_broadcast_id;	// defined in ETR162

	// selector_byte[N]

} dvb_descr_data_broadcast_id_t attr_pack;


/**
 * Data Broadcast Descriptor.
 */ 
typedef struct _dvb_descr_data_broadcast {
	
	dvb_descr_gen_t gen_descr;

	char			data_broadcast_id_hi;
	char			data_broadcast_id_lo;

	char			component_tag;
	char			selector_length;

	// selector_byte[N]

	// dvb_descr_data_broadcast_ext
	
	// text_char[N]

} dvb_descr_data_broadcast_t attr_pack;

#define DVB_GET_DESCR_DBC_ID(t) \
 (((t)->data_broadcast_id_hi << 8) | (t)->data_broadcast_id_lo)

#define DVB_SET_DESCR_DBC_ID(t,i) \
do { \
  (t)->data_broadcast_id_hi = (((unsigned __int16)(i)) & 0xFF00) >> 8; \
  (t)->data_broadcast_id_lo = (((unsigned __int16)(i)) & 0x00FF); \
} while (0)

#define DVB_DBC_ID_DATA_PIPE			0x0001
#define DVB_DBC_ID_ASYNC_DATA_STREAM	0x0002
#define DVB_DBC_ID_SYNC_DATA_STREAM		0x0003
#define DVB_DBC_ID_SYNCED_DATA_STREAM	0x0004
#define DVB_DBC_ID_MPE					0x0005
#define DVB_DBC_ID_DATA_CAROUSEL		0x0006
#define DVB_DBC_ID_OBJECT_CAROUSEL		0x0007
#define DVB_DBC_ID_ATM_STREAMS			0x0008
#define DVB_DBC_ID_HIGHER_PROTO			0x0009

#define DVB_DBC_ID_MHP_CAROUSEL			0x00F0
#define DVB_DBC_ID_MHP_MPE				0x00F1


typedef struct _dvb_descr_data_broadcast_ext {

	char			ISO_639_language_code_0;
	char			ISO_639_language_code_1;
	char			ISO_639_language_code_2;

	char			text_length;
	
} dvb_descr_data_broadcast_ext_t attr_pack;


// Selector of data_broadcast descriptor for data_broadcast_id == 0x0005
typedef struct _dvb_mpe_info {

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		reserved			:3;
	char		alignment_indicator :1;
	char		mac_ip_map_flag		:1;
	char		mac_addr_range		:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		mac_addr_range		:3;
	char		mac_ip_map_flag		:1;
	char		alignment_indicator :1;
	char		reserved			:3;
#else
#error "Please specify the byte order!"
#endif

	char	max_sec_per_dg;

} dvb_mpe_info_t attr_pack;


#define DVB_GET_DESCR_DBC_ISO_639(t,s) \
do { \
  (s)[0] = (t)->ISO_639_language_code_0; \
  (s)[1] = (t)->ISO_639_language_code_1; \
  (s)[2] = (t)->ISO_639_language_code_2; \
} while (0)

#define DVB_SET_DESCR_DBC_ISO_639(t,s) \
do { \
  (t)->ISO_639_language_code_0 = (s)[0]; \
  (t)->ISO_639_language_code_1 = (s)[1]; \
  (t)->ISO_639_language_code_2 = (s)[2]; \
} while (0)


/**
 * Network Name Descriptor
 */
typedef struct _dvb_descr_network_name {

	dvb_descr_gen_t	gen_descr;

	// char[N]

} dvb_descr_network_name_t attr_pack;


/**
 * Service List descriptor entry
 */
typedef struct _dvb_service_list_entry {

	char		service_id_hi;
	char		service_id_lo;
	char		service_type;

} dvb_service_list_entry_t attr_pack;


#define DVB_GET_SVC_LIST_ENTRY_ID(t) \
  (((t)->service_id_hi << 8) | ((t)->service_id_lo))

#define DVB_SET_SVC_LIST_ENTRY_ID(t,d) \
do { \
  (t)->service_id_hi = (((unsigned __int16)(d)) & 0xFF00) >> 8; \
  (t)->service_id_lo = ((unsigned __int16)(d)) & 0x00FF; \
} while (0)


/**
 * Service List Descriptor
 */
typedef struct _dvb_descr_service_list {

	dvb_descr_gen_t	gen_descr;

	// dvb_service_list_entry_t[N]

} dvb_descr_service_list_t attr_pack;


/**
 * Values for dvb_service_list_entry_t's service_type.
 */
#define DVB_SVC_TYP_DIGI_TV			0x01
#define DVB_SVC_TYP_DIGI_RD_SOUND	0x02
#define DVB_SVC_TYP_TELETEXT		0x03
#define DVB_SVC_TYP_NVOD_REF		0x04
#define DVB_SVC_TYP_NVOD_TSHF		0x05
#define DVB_SVC_TYP_MOSAIC			0x06
#define DVB_SVC_TYP_PAL_CODED		0x07
#define DVB_SVC_TYP_SECAM_CODED		0x08
#define DVB_SVC_TYP_D_D2_MAC		0x09
#define DVB_SVC_TYP_FM_RADIO		0x0A
#define DVB_SVC_TYP_NTSC_CODED		0x0B
#define DVB_SVC_TYP_DATA_BROADCAST	0x0C
/* 0x0D - 0x7F  reserved for future use */
/* 0x80 - 0xFE  user defined */
/* 0xFF  reserved for future use */


/**
 * Service Descriptor.
 */
typedef struct _dvb_descr_service {
	
	dvb_descr_gen_t gen_descr;

	char			service_type;
	char			service_provider_name_length;
	// char service_provider_name[N]
	// char service_name_length
	// char service_name[N1]

} dvb_descr_service_t attr_pack;


/**
 * Stuffing Descriptor
 */
typedef struct _dvb_descr_stuffing {

	dvb_descr_gen_t	gen_descr;

	//	stuffing_byte[N]

} dvb_descr_stuffing_t attr_pack;


/**
 * Values for Satellite Delivery System Descriptor.
 */
#define	DVB_OUTER_FEC_NONE			0x1
#define	DVB_OUTER_FEC_RS			0x2

#define	DVB_INNER_FEC_1_2			0x1
#define	DVB_INNER_FEC_2_3			0x2
#define	DVB_INNER_FEC_3_4			0x3
#define	DVB_INNER_FEC_5_6			0x4
#define	DVB_INNER_FEC_7_8			0x5
#define	DVB_INNER_FEC_NONE			0xF

#define	DVB_CABLE_MOD_16_QAM		0x01
#define	DVB_CABLE_MOD_32_QAM		0x02
#define	DVB_CABLE_MOD_64_QAM		0x03
#define	DVB_CABLE_MOD_128_QAM		0x04
#define	DVB_CABLE_MOD_256_QAM		0x05

#define	DVB_SATELLITE_MOD_QPSK		0x01

#define	DVB_POLARIZATION_LIN_HORZ	0
#define	DVB_POLARIZATION_LIN_VERT	1
#define	DVB_POLARIZATION_CIRC_LT	2
#define	DVB_POLARIZATION_CIRC_RT	3

#define	DVB_WEST_EAST_WEST			0
#define	DVB_WEST_EAST_EAST			1


/**
 * Satellite Delivery System Descriptor
 */
typedef struct _dvb_descr_satellite_del_sys {

	dvb_descr_gen_t	gen_descr;

	char		frequency_0;
	char		frequency_1;
	char		frequency_2;
	char		frequency_3;

	char		orbital_position_hi;
	char		orbital_position_lo;
	
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		modulation			:5;
	char		polarization		:2;
	char		west_east_flag		:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		west_east_flag		:1;
	char		polarization		:2;
	char		modulation			:5;
#else
#error "Please specify the byte order!"
#endif
	char		symbol_rate_0;
	char		symbol_rate_1;
	char		symbol_rate_2;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		FEC_inner			:4;
	char		symbol_rate_3		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		symbol_rate_3		:4;
	char		FEC_inner			:4;
#else
#error "Please specify the byte order!"
#endif
} dvb_descr_satellite_del_sys_t attr_pack;


#define DVB_GET_SAT_DEL_SYS_FREQ(t) \
  (((t)->frequency_0 << 24) | ((t)->frequency_1 << 16) | ((t)->frequency_2 << 8) | (t)->frequency_3)

#define DVB_SET_SAT_DEL_SYS_FREQ(t,d) \
do { \
  (t)->frequency_0 = (((unsigned int)(d)) & 0xFF000000) >> 24; \
  (t)->frequency_1 = (((unsigned int)(d)) & 0x00FF0000) >> 16; \
  (t)->frequency_2 = (((unsigned int)(d)) & 0x0000FF00) >> 8; \
  (t)->frequency_3 = (((unsigned int)(d)) & 0x000000FF); \
} while (0)


#define DVB_GET_SAT_DEL_SYS_ORB(t) \
  (((t)->orbital_position_hi << 8) | (t)->orbital_position_lo)

#define DVB_SET_SAT_DEL_SYS_ORB(t,d) \
do { \
  (t)->orbital_position_hi = (((unsigned __int16)(d)) & 0xFF00) >> 8; \
  (t)->orbital_position_lo = (((unsigned __int16)(d)) & 0x00FF); \
} while (0)


#define DVB_GET_SAT_DEL_SYS_SYMBOL(t) \
  (((t)->symbol_rate_0 << 20) | ((t)->symbol_rate_1 << 12) | ((t)->symbol_rate_2 << 4) | ((t)->symbol_rate_3))

#define DVB_SET_SAT_DEL_SYS_SYMBOL(t,s) \
do { \
  (t)->symbol_rate_0 = (((unsigned int)(s)) & 0x0FF00000) >> 20; \
  (t)->symbol_rate_1 = (((unsigned int)(s)) & 0x000FF000) >> 12; \
  (t)->symbol_rate_2 = (((unsigned int)(s)) & 0x00000FF0) >> 4; \
  (t)->symbol_rate_3 = (((unsigned int)(s)) & 0x0000000F); \
} while (0)


/**
 * Cable Delivery System Descriptor
 */
typedef struct _dvb_descr_cable_del_sys {

	dvb_descr_gen_t	gen_descr;

	unsigned int		frequency;
	char		reserved_0;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		FEC_outer			:4;
	char		reserved_1			:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		reserved_1			:4;
	char		FEC_outer			:4;
#else
#error "Please specify the byte order!"
#endif
	char		modulation;
	char		symbol_rate_0;
	char		symbol_rate_1;
	char		symbol_rate_2;
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		FEC_inner			:4;
	char		symbol_rate_3		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		symbol_rate_3		:4;
	char		FEC_inner			:4;
#else
#error "Please specify the byte order!"
#endif
} dvb_descr_cable_del_sys_t attr_pack;


#define DVB_GET_CABLE_DEL_SYS_FREQ(t) \
  g_ntohl( (t)->frequency )

#define DVB_SET_CABLE_DEL_SYS_FREQ(t,d) \
  (t)->frequency = htonl( (unsigned int)(d) )

#define DVB_GET_CABLE_DEL_SYS_RESERVED(t) \
  (((t)->reserved_0 << 4) | ((t)->reserved_1))

#define DVB_SET_CABLE_DEL_SYS_RESERVED(t,s) \
do { \
  (t)->reserved_0 = (((unsigned __int16)(s)) & 0x0FF0) >> 4; \
  (t)->reserved_1 = (((unsigned __int16)(s)) & 0x000F); \
} while (0)

#define DVB_GET_CABLE_DEL_SYS_SYMBOL(t) \
  (((t)->symbol_rate_0 << 20) | ((t)->symbol_rate_1 << 12) | ((t)->symbol_rate_2 << 4) | ((t)->symbol_rate_3))

#define DVB_SET_CABLE_DEL_SYS_SYMBOL(t,s) \
do { \
  (t)->symbol_rate_0 = (((unsigned int)(s)) & 0x0FF00000) >> 20; \
  (t)->symbol_rate_1 = (((unsigned int)(s)) & 0x000FF000) >> 12; \
  (t)->symbol_rate_2 = (((unsigned int)(s)) & 0x00000FF0) >> 4; \
  (t)->symbol_rate_3 = (((unsigned int)(s)) & 0x0000000F); \
} while (0)


#define	DVB_LINKAGE_TYPE_RESSERVED			0x00
#define	DVB_LINKAGE_TYPE_INFO				0x01
#define	DVB_LINKAGE_TYPE_EPG				0x02
#define	DVB_LINKAGE_TYPE_CA_REPLACE			0x03
#define	DVB_LINKAGE_TYPE_BOUQUET_SI			0x04
#define	DVB_LINKAGE_TYPE_SVC_REPLACE		0x05
#define	DVB_LINKAGE_TYPE_DATA_BROADCAST		0x06


/**
 * Linkage Descriptor
 */
typedef struct _dvb_descr_linkage {

	dvb_descr_gen_t	gen_descr;

	unsigned __int16		transport_stream_id __attribute((packed));
	unsigned __int16		original_network_id __attribute((packed));
	unsigned __int16		service_id __attribute((packed));
	char		linkage_type __attribute((packed));

	//	private_data_byte[N]

} dvb_descr_linkage_t attr_pack;


#define DVB_GET_LINKAGE_TS_ID(t) \
  g_ntohs( (t)->transport_stream_id )

#define DVB_SET_LINKAGE_TS_ID(t,d) \
  (t)->transport_stream_id = htons( (unsigned __int16)(d) )

#define DVB_GET_LINKAGE_NETW_ID(t) \
  g_ntohs( (t)->original_network_id )

#define DVB_SET_LINKAGE_NETW_ID(t,d) \
  (t)->original_network_id = htons( (unsigned __int16)(d) )

#define DVB_GET_LINKAGE_SVC_ID(t) \
  g_ntohs( (t)->service_id )

#define DVB_SET_LINKAGE_SVC_ID(t,d) \
  (t)->service_id = htons( (unsigned __int16)(d) )


typedef struct _dvb_descr_carousel_id {

	dvb_descr_gen_t gen_descr;

	unsigned int	carousel_id	attr_pack;
	char	format_id	attr_pack;

	// if (format_id == 1)
	//   format_specifier()

	// private_data_byte[N]

} dvb_descr_carousel_id_t attr_pack;


typedef struct _dvb_descr_app_sig {

	dvb_descr_gen_t gen_descr;

	// dvb_appsig_entry_t[]

} dvb_descr_app_sig_t attr_pack;

typedef struct _dvb_appsig_entry {
	unsigned __int16	app_type attr_pack;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	ait_ver  :5 attr_pack;
	char	reserved :3 attr_pack;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved :3 attr_pack;
	char	ait_ver  :5 attr_pack;
#else
#error "Please specify the byte order!"
#endif

} dvb_appsig_entry_t attr_pack;


typedef struct _dvb_descr_assoctag
{
	dvb_descr_gen_t gen_descr attr_pack;

	unsigned __int16	assoc_tag attr_pack;
	unsigned __int16	use attr_pack;
	char	selector_len attr_pack;

	// char selector_bytes[selector_len];
	// char private_data_bytes[]
} dvb_descr_assoctag_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#ifdef __cplusplus
}	// extern "C"
#endif

#endif   /* __DESCRIPTORS_H__ */