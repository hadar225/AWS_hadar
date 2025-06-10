/* ts_hdr.h
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
#ifndef __DVB_TS_HDR_H__
#define __DVB_TS_HDR_H__

//#include <glib.h>

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Complier not supported"
#endif
typedef struct _dvb_ts_hdr {
    
	char	sync_byte;	// 0x47

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	PID_hi                       :5;
	char	transport_priority           :1;
	char	payload_unit_start_indicator :1;
	char	transport_error_indicator    :1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	transport_error_indicator    :1;
	char	payload_unit_start_indicator :1;
	char	transport_priority           :1;
	char	PID_hi                       :5;
#else
#error "Please specify the byte order!"
#endif

	char	PID_lo;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	continuity_counter           :4;
	char	adaptation_field_control     :2;
	char	transport_scrambling_control :2;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	transport_scrambling_control :2;
	char	adaptation_field_control     :2;
	char	continuity_counter           :4;
#else
#error "Please specify the byte order!"
#endif

} dvb_ts_hdr_t attr_pack;

#define TS_LEN		188
#define TS_HDR_LEN	sizeof(dvb_ts_hdr_t)	// 4 bytes
#define TS_DATA_LEN 188 - TS_HDR_LEN		// 184 bytes

#define TS_SYNC_BYTE	0x47


/**
 * Maximum PID number.
 */
#define DVB_MAX_PID	0x1FFF


/**
 * Get the PID value from the TS header.
 *
 * @param   ts  pointer to dvb_ts_hdr_t.
 */
#define DVB_GET_TS_PID(t) \
((((t)->PID_hi) << 8) | ((t)->PID_lo))

/**
 * Set the PID value in a TS header.
 *
 * @param    ts  pointer to dvb_ts_hdr_t.
 * @param    p   PID
 */
#define DVB_SET_TS_PID(ts,p) \
do { \
  (ts)->PID_lo = ((char)(((unsigned __int16)(p)) & 0x00FF)); \
  (ts)->PID_hi = ((char)((((unsigned __int16)(p)) & 0x1F00) >> 8)); \
} while (0)


/**
 * Scrambling Control values.
 */
#define TS_SCV_NOT_SCRAMBLED	0x00
#define TS_SCV_USER_DEFINED_01	0x01
#define TS_SCV_USER_DEFINED_10	0x02
#define TS_SCV_USER_DEFINED_11	0x03

/**
 * Adaptation Field Control values.
 */
#define TS_ADP_RESERVED		0x00
#define TS_ADP_PAYLOAD_ONLY	0x01
#define TS_ADP_ADAPTFIELD_ONLY	0x02
#define TS_ADP_BOTH		0x03


/**
 * Fixed-size part of Adaptation Field.
 */
typedef struct _dvb_ts_adaptation_field {

	char	adaptation_field_length;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	char	adaptation_field_extension_flag       :1;
	char	transport_private_data_flag           :1;
	char	splicing_point_flag                   :1;
	char	OPCR_flag                             :1;
	char	PCR_flag                              :1;
	char	elementary_stream_priority_indicator  :1;
	char	random_access_indicator               :1;
	char	discontinuity_indicator               :1;
#elif __BYTE_ORDER == __BIG_ENDIAN
	char	discontinuity_indicator               :1;
	char	random_access_indicator               :1;
	char	elementary_stream_priority_indicator  :1;
	char	PCR_flag                              :1;
	char	OPCR_flag                             :1;
	char	splicing_point_flag                   :1;
	char	transport_private_data_flag           :1;
	char	adaptation_field_extension_flag       :1;
#else
#error "Please specify the byte order!"
#endif

} dvb_ts_adaptation_field_t attr_pack;

#define TS_AF_HDR_LEN	sizeof(dvb_ts_adaptation_field_t)


typedef struct _dvb_ts_pcr {

	char	program_clock_reference_base_1;
	char	program_clock_reference_base_2;
	char	program_clock_reference_base_3;
	char	program_clock_reference_base_4;

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	program_clock_reference_extension_1	:1;
	char	reserved0							:6;
	char	program_clock_reference_base_5		:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	program_clock_reference_base_5		:1;
	char	reserved0							:6;
	char	program_clock_reference_extension_1	:1;

#else
#error "Please specify the byte order!"
#endif

	char	program_clock_reference_extension_2;

} dvb_ts_pcr_t attr_pack;

#define TS_PCR_LEN	sizeof(dvb_ts_pcr_t)

/**
 * Get the PCR base value as number from a PCR or OPCR struct.
 *
 * @param	ph		pointer to dvb_ts_pcr_t or dvb_ts_opcr_t
 * @return	the pcr base, an unsigned long long
 */
#define TS_GET_PCR_BASE(ph) \
  (((ph)->program_clock_reference_base_1 << 25) | ((ph)->program_clock_reference_base_2 << 17) | \
   ((ph)->program_clock_reference_base_3 << 9) | ((ph)->program_clock_reference_base_4 << 1) | \
   (ph)->program_clock_reference_base_5)

/**
 * Get the PCR extension value as number from a PCR or OPCR struct.
 *
 * @param	ph		pointer to dvb_ts_pcr_t or dvb_ts_opcr_t
 * @return	the pcr extension, an unsigned short
 */
#define TS_GET_PCR_EXT(ph) \
  ((ph->program_clock_reference_extension_1 << 8) | ph->program_clock_reference_extension_2 )

/**
 * Get the complete PCR value as number from a PCR or OPCR struct.
 *
 * @param	ph		pointer to dvb_ts_pcr_t or dvb_ts_opcr_t
 * @return	the pcr, an unsigned long long, in units of 27 MHz
 */
#define TS_GET_PCR(ph)	\
  ((TS_GET_PCR_BASE(ph) * 300) + TS_GET_PCR_EXT(ph))

static /*inline*/ unsigned long long ts_pcr2uint64( dvb_ts_pcr_t *ph )
{
	unsigned long long pcr = 0LL;
	unsigned short pcr_ext = 0;

	pcr = ph->program_clock_reference_base_1;
	pcr = pcr << 8 |  ph->program_clock_reference_base_2;
	pcr = pcr << 8 |  ph->program_clock_reference_base_3;
	pcr = pcr << 8 |  ph->program_clock_reference_base_4;
	pcr = pcr << 1 |  ph->program_clock_reference_base_5;
	pcr *= 300;

	pcr_ext = ph->program_clock_reference_extension_1;
	pcr_ext = pcr_ext << 8 | ph->program_clock_reference_extension_2;

	pcr += pcr_ext;

	return pcr;
}


/**
 * Convert pcr value to microseconds (µs).
 *
 * @param	p	pcr value (unsigned long long)
 * @return	µs value corresponding to the given pcr.
 */
#define TS_PCR_2_US(p)	((p) / 27)


typedef struct _dvb_ts_pcr dvb_ts_opcr_t;	/* same structure as pcr */


/**
 * Complete TS cell: 188 bytes long.
 */
struct _dvb_ts_cell {
	dvb_ts_hdr_t	tsc_header;
	union {
		unsigned char tsc_bytes[184];
		struct _with_af {
			dvb_ts_adaptation_field_t tsc_af;
			unsigned char tsc_af_bytes[184-sizeof(dvb_ts_adaptation_field_t)];
		} with_af;
	} tsc_payload;
} attr_pack;

typedef struct _dvb_ts_cell dvb_ts_cell_t;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif
#endif   /* __DVB_TS_HDR_H__ */