/* sect_gen.h
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

//#include <glib.h>

#ifndef __SECT_GEN_H__
#define __SECT_GEN_H__

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_sect_gen {

	char	table_id;
    
#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	section_length_hi        :4;
	char	reserved0                :2;
	char	private_indicator        :1;
	char	section_syntax_indicator :1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	section_syntax_indicator :1;
	char	private_indicator        :1;   // always 0
	char	reserved0                :2;
	char	section_length_hi        :4;
#else
#error "Please specify the byte order!"
#endif

	char	section_length_lo;

	char	id_hi;   // Meaning depends on specific table type
	char	id_lo;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	current_next_indicator   :1;
	char	version_number           :5;
	char	reserved1                :2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	reserved1                :2;
	char	version_number           :5;
	char	current_next_indicator   :1;
#else
#error "Please specify the byte order!"
#endif
    
	char	section_number;
	char	last_section_number;

} dvb_sect_gen_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif
typedef unsigned int	dvb_CRC32_t;


/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 */
#define DVB_GET_SECTION_LENGTH(ps) \
  ((((ps)->section_length_hi) << 8) | (ps)->section_length_lo)

/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 * @param  l    length to set in section header.
 */
#define DVB_SET_SECTION_LENGTH(ps,l) \
do { \
  (ps)->section_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (ps)->section_length_hi = ((char)((((unsigned __int16)(l)) & 0x0F00) >> 8)); \
} while (0)

/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 */
#define DVB_GET_SECTION_ID(ps) \
  (((ps)->id_hi << 8) | (ps)->id_lo)

/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 * @param  id   id to set in section header.
 */
#define DVB_SET_SECTION_ID(ps,id) \
do { \
  (ps)->id_lo = ((char)(((uint16)(id)) & 0x00FF)); \
  (ps)->id_hi = ((char)((((uint16)(id)) & 0xFF00) >> 8)); \
} while (0)


/**
 * Table ID's
 */
#define DVB_TABLE_ID_PAT		0x00
#define DVB_TABLE_ID_CAT		0x01
#define DVB_TABLE_ID_PMT		0x02
#define DVB_TABLE_ID_TSDT		0x03
/* 0x04 - 0x39  reserved */
#define DVB_TABLE_ID_DSMCC_MPE	0x3A
#define DVB_TABLE_ID_DSMCC_UN	0x3B
#define DVB_TABLE_ID_DSMCC_DDB	0x3C
#define DVB_TABLE_ID_DSMCC_SD	0x3D
#define DVB_TABLE_ID_DSMCC_PRIV	0x3E
/* 0x3F ISO/IEC 13818-6 reserved */
#define DVB_TABLE_ID_NIT_ACT	0x40
#define DVB_TABLE_ID_NIT_OTH	0x41
#define DVB_TABLE_ID_SDT_ACT	0x42
/* 0x43 - 0x45  reserved */
#define DVB_TABLE_ID_SDT_OTH	0x46
/* 0x43 - 0x49  reserved */
#define DVB_TABLE_ID_BAT		0x4A
/* 0x4B - 0x4D  reserved */
#define DVB_TABLE_ID_EIT_ACT_PF	0x4E
#define DVB_TABLE_ID_EIT_OTH_PF	0x4F
/* 0x50 - 0x5F  EIT_ACT_SCHEDULE */
/* 0x60 - 0x6F  EIT_OTH_SCHEDULE */
#define DVB_TABLE_ID_TDT		0x70
#define DVB_TABLE_ID_RST		0x71
#define DVB_TABLE_ID_ST			0x72
#define DVB_TABLE_ID_TOT		0x73
#define DVB_TABLE_ID_AIT		0x74
/* 0x75 - 0x7D  reserved */
#define DVB_TABLE_ID_DIT		0x7E
#define DVB_TABLE_ID_SIT		0x7F
/* User-Defined: SkyPlex Signalling Channel: */
#define DVB_TABLE_ID_SSC		0x80
/* User-Defined: EMBRACE Ethernet in Sections: */
#define DVB_TABLE_ID_ETH		0x88
/* 0x80 - 0xFE  user defined */
/* 0xFF         reserved */


/**
 * Table PIDs
 */
#define	DVB_TABLE_PID_PAT			0x0000
#define	DVB_TABLE_PID_NIT			0x0010
#define	DVB_TABLE_PID_SDT			0x0011
#define	DVB_TABLE_PID_TDT			0x0014
#define	DVB_TABLE_PID_EIT_ACT_PF	0x0012


/**
 * Recommended table refresh interval (in milliseconds)
 */
#define	DVB_TABLE_INTV_PAT			400
#define	DVB_TABLE_INTV_PMT			400
#define	DVB_TABLE_INTV_NIT			9900
#define	DVB_TABLE_INTV_SDT_ACT		1900
#define	DVB_TABLE_INTV_TDT			29900
#define	DVB_TABLE_INTV_EIT_ACT_PF	1900


#endif   /* __SECT_GEN_H__ */