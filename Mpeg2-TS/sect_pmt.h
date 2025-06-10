/* sect_pmt.h
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
#ifndef __SECT_PMT_H__
#define __SECT_PMT_H__

//#include <glib.h>
#include "sect_gen.h"

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_pmt {

	dvb_sect_gen_t	gen_hdr attr_pack;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char		PCR_PID_hi             :5;
	char		reserved0              :3;
	char		PCR_PID_lo;
	char		program_info_length_hi :4;
	char		reserved1              :4;
	char		program_info_length_lo;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		reserved0              :3;
	char		PCR_PID_hi             :5;
	char		PCR_PID_lo;
	char		reserved1              :4;
	char		program_info_length_hi :4;
	char		program_info_length_lo;
#else
#error "Please specify the byte order!"
#endif

	// descriptor[N]
	// PMT entries[N1] + ES_descr[N]

    // dvb_CRC32_t;

} dvb_sect_pmt_t attr_pack;


#define DVB_GET_PMT_PCRPID(p) \
(((p)->PCR_PID_hi << 8) | (p)->PCR_PID_lo)

#define DVB_SET_PMT_PCRPID(p,pid) \
do { \
  (p)->PCR_PID_lo = ((char)(((unsigned __int16)(pid)) & 0x00FF)); \
  (p)->PCR_PID_hi = ((char)((((unsigned __int16)(pid)) & 0x1F00) >> 8)); \
} while (0)


#define DVB_GET_PROGINFO_LENGTH(p) \
  ((((p)->program_info_length_hi) << 8) | (p)->program_info_length_lo)

#define DVB_SET_PROGINFO_LENGTH(p,l) \
do { \
  (p)->program_info_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->program_info_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)


typedef struct _dvb_sect_pmt_entry {

	char	stream_type;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	elementary_PID_hi	:5;
	char	reserved0		:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved0		:3;
	char	elementary_PID_hi	:5;
#else
#error "Please specify the byte order!"
#endif

	char	elementary_PID_lo;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	ES_info_length_hi	:4;
	char	reserved1		:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved1		:4;
	char	ES_info_length_hi	:4;
#else
#error "Please specify the byte order!"
#endif

	char	ES_info_length_lo;

	// ES Descriptors[N2]

} dvb_sect_pmt_entry_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif

#define DVB_GET_SECT_PMT_EPID(p) \
  ((((p)->elementary_PID_hi) << 8) | (p)->elementary_PID_lo)

#define DVB_SET_SECT_PMT_EPID(p,ep) \
do { \
  (p)->elementary_PID_hi = (((unsigned __int16)(ep)) & 0x1F00) >> 8; \
  (p)->elementary_PID_lo = (((unsigned __int16)(ep)) & 0x00FF); \
} while (0)

#define DVB_GET_SECT_PMT_ESINFO_LEN(p) \
 ((((p)->ES_info_length_hi) << 8) | (p)->ES_info_length_lo)

#define DVB_SET_SECT_PMT_ESINFO_LEN(p,l) \
do { \
  (p)->ES_info_length_hi = (((unsigned __int16)(l)) & 0x0F00) >> 8; \
  (p)->ES_info_length_lo = (((unsigned __int16)(l)) & 0x00FF); \
} while (0)


/**
 * Values for dvb_sect_pmt_entry_t's stream_type:
 */
#define DVB_PMT_STREAM_IEC_11172_VIDEO		0x01
#define DVB_PMT_STREAM_IEC_13818_2_VIDEO	0x02
#define DVB_PMT_STREAM_IEC_11172_AUDIO		0x03
#define DVB_PMT_STREAM_IEC_13818_3_AUDIO	0x04
#define DVB_PMT_STREAM_IEC_13818_1_PRIVATE	0x05
#define DVB_PMT_STREAM_IEC_13818_1_PES		0x06
#define DVB_PMT_STREAM_IEC_13522_MHEG		0x07
#define DVB_PMT_STREAM_IEC_13818_1_DSMCC	0x08
#define DVB_PMT_STREAM_ITUT_H222_1		0x09
#define DVB_PMT_STREAM_IEC_13818_6_A		0x0A
#define DVB_PMT_STREAM_IEC_13818_6_B		0x0B
#define DVB_PMT_STREAM_IEC_13818_6_C		0x0C
#define DVB_PMT_STREAM_IEC_13818_6_D		0x0D
#define DVB_PMT_STREAM_IEC_13818_1_AUX		0x0E
// 0x0F - 0x7F: Reserved
// 0x80 - 0xFF: User defined



#endif  /* __SECT_PMT_H__ */