/* sect_pat.h
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
#ifndef __SECT_PAT_H__
#define __SECT_PAT_H__

//#include <glib.h>
//#include <WinSock2.h>
#include <arpa/inet.h>
#include "sect_gen.h"

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_sect_pat {

	dvb_sect_gen_t	gen_hdr;

	// dvb_sect_pat_program_t[N];

	// dvb_CRC32_t;

} dvb_sect_pat_t attr_pack;


typedef struct _dvb_sect_pat_program {

	unsigned short	program_number;

	union _u {
		struct _network_PID {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			char		network_PID_hi       :5;
			char		reserved0            :3;
#elif __BYTE_ORDER == __BIG_ENDIAN
			char		reserved0            :3;
			char		network_PID_hi       :5;
#else
#error "Please specify the byte order!"
#endif
			char		network_PID_lo;
		} network_PID;
		struct _program_map_PID {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			char		program_map_PID_hi   :5;
			char		reserved0            :3;
#elif __BYTE_ORDER == __BIG_ENDIAN
			char		reserved0            :3;
			char		program_map_PID_hi   :5;
#else
#error "Please specify the byte order!"
#endif
			char		program_map_PID_lo;
		} program_map_PID;
	} u;
} dvb_sect_pat_program_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p1)
#endif

/**
 * @param  p   pointer to dvb_sect_pat_t.
 *
 * @note   9 because section length is the length of the remainder
 *         of the section starting with the first byte after the 
 *         section length field, but including the CRC32.
 */
#define DVB_GET_PAT_PROG_COUNT(p) \
  ((((((p)->gen_hdr.section_length_hi) << 8) | (p)->gen_hdr.section_length_lo) - 9) >> 2)


/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 */
#define DVB_GET_PAT_PROGNR(pp) \
  (unsigned __int16)ntohs( (pp)->program_number )

/**
 * @param  ps   pointer to dvb_gen_secthdr_t.
 * @param  id   id to set in section header.
 */
#define DVB_SET_PAT_PROGNR(pp,pnr) \
  (pp)->program_number = htons( ((unsigned __int16)(pnr)) )


/**
 * @param  S     type of PAT Program Info (PMT or NIT)
 * @param  ps    pointer to dvb_sect_pat_program_t.
 */
#define DVB_GET_PAT_PROG_PID(S,ps) \
  ((((ps)->u.S.S ## _hi) << 8) | (ps)->u.S.S ## _lo)

/**
 * @param  S    type of PAT Program Info (PMT or NIT)
 * @param  ps   pointer to dvb_sect_pat_program_t.
 * @param  l    PID to set.
 */
#define DVB_SET_PAT_PROG_PID(S,ps,l) \
do { \
  (ps)->u.S.S ## _lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (ps)->u.S.S ## _hi = ((char)((((unsigned __int16)(l)) & 0x1F00) >> 8)); \
} while (0)


#endif  /* __SECT_PAT_H__ */
