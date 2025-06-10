 /* Ethereal - Network traffic analyzer
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


#ifndef __DVB_AVPES_HDR_H__
#define __DVB_AVPES_HDR_H__

#if __BYTE_ORDER == __LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#else
#error "Please specify the byte order!"
#endif

#define	SyncWord	0x4156

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_avpes_hdr {
	
	char	SyncWord_hi;
	char	SyncWord_lo;

	char	StreamID;	
	char	Counter; 	
	char	Reserved;	

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	PreBytes		:2;
	char	PostBytes		:2;
	char	PTS_Flag		:1;
	char	ReservedFlags	:3;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	ReservedFlags	:3;		
	char	PTS_Flag		:1;
	char	PostBytes		:2;
	char	PreBytes		:2;

#else
#error "Please specify the byte order!"
#endif

	char 	Length_hi;
	char	Length_lo;		

} dvb_avpes_hdr_t attr_pack;


/**
 * Get the SyncWord value from the AVPES header.
 *
 * @param   p  pointer to dvb_avpes_hdr_t.
 */
#define AVPES_GET_SYNCWORD(p) \
  ((((p)->SyncWord_hi) << 8) | ((p)->SyncWord_lo))


/**
 * Set the SyncWord value in a AVPES header.
 *
 * @param    p   pointer to dvb_ts_hdr_t.
 * @param    v   SyncWord
 */
#define AVPES_SET_SYNCWORD(p,v) \
do { \
  (p)->SyncWord_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (p)->SyncWord_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)



/**
 * StreamIDs
 */
#define AVPES_VIDEO_STREAM_ID		0x01
#define AVPES_MAIN_AUDIO_STREAM_ID	0x02



/**
 * Get the length of the AVPES payload.
 *
 * @param   p  pointer to dvb_avpes_hdr_t.
 */
#define AVPES_GET_LENGTH(p) \
  ((((p)->Length_hi) << 8) | ((p)->Length_lo))


/**
 * Set the length of the payload in a AVPES header.
 *
 * @param    p   pointer to dvb_ts_hdr_t.
 * @param    v   length
 */
#define AVPES_SET_LENGTH(p,v) \
do { \
  (p)->Length_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (p)->Length_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)


#define MAX_VID_PAYLOADLENGTH	0x17f8	
#define MAX_AUD_PAYLOADLENGTH	0x07f8

typedef struct _dvb_avpes_PTS {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	/*char	pts_31_30					:2;
	
	  char	pts_29_15_lo					:7;
	  char	pts_29_15_hi					:8;
	
	  char	pts_14_0_lo					:7;
	
	  char	pts_14_0_hi					:8;
	*/

	char	pts_29_15_hi_6			:6;
	char	pts_31_30				:2;

	char	pts_29_15_mid_6			:6;
	char	pts_29_15_mid_2			:2;

	char	pts_14_0_hi_7			:7;
	char	pts_29_15_lo_1			:1;

	char	pts_14_0_lo_7			:7;
	char   pts_14_0_mid_1			:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pts_31_30				:2;
	char	pts_29_15_hi_6			:6;

	char	pts_29_15_mid_2			:2;
	char	pts_29_15_mid_6			:6;

	char	pts_29_15_lo_1			:1;
	char	pts_14_0_hi_7			:7;

	char   pts_14_0_mid_1			:1;
	char	pts_14_0_lo_7			:7;

#else
#error "Please specify the byte order!"
#endif

} dvb_avpes_PTS_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif



/**
 * Get the PTS 29 15 timestamp.
 *
 * @param   pt  pointer to _dvb_avpes_PTS
 */
#define AVPES_GET_PTS_2915(pt) \
  ((((pt)->pts_29_15_hi_6) << 9) | (((pt)->pts_29_15_mid_2) << 7) |  (((pt)->pts_29_15_mid_6) << 1) | ((pt)->pts_29_15_lo_1))

/**
 * Get the PTS 14 0 timestamp.
 *
 * @param   pt  pointer to _dvb_avpes_PTS
 */
#define AVPES_GET_PTS_140(pt) \
  ((((pt)->pts_14_0_hi_7) << 8) | (((pt)->pts_14_0_mid_1) << 7) | ((pt)->pts_14_0_lo_7))




/**
 * Set the PTS_2915 timestamp.
 *
 * @param    pt  pointer to _dvb_avpes_PTS.
 * @param    v   value
 */
#define AVPES_SET_PTS_2915(pt, v) \
do { \
  (pt)->pts_29_15_lo_1  = ((char)(((unsigned __int16)(v)) & 0x0001)); \
  (pt)->pts_29_15_mid_6 = ((char)((((unsigned __int16)(v)) & 0x007E) >> 1)); \
  (pt)->pts_29_15_mid_2 = ((char)((((unsigned __int16)(v)) & 0x0180) >> 7)); \
  (pt)->pts_29_15_hi_6  = ((char)((((unsigned __int16)(v)) & 0x7E00) >> 9)); \
} while (0)


/**
 * Set the PTS_140 timestamp.
 *
 * @param    pt  pointer to _dvb_avpes_PTS
 * @param    v   value
 */
#define AVPES_SET_PTS_140(pt, v) \
do { \
  (pt)->pts_14_0_lo_7  = ((char)(((unsigned __int16)(v)) & 0x007F)); \
  (pt)->pts_14_0_mid_1 = ((char)((((unsigned __int16)(v)) & 0x0080) >> 7)); \
  (pt)->pts_14_0_hi_7  = ((char)((((unsigned __int16)(v)) & 0x7F00) >> 8)); \
} while (0)



#endif   /* __DVB_AVPES_HDR_H__ */ /* Ethereal - Network traffic analyzer
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


#ifndef __DVB_AVPES_HDR_H__
#define __DVB_AVPES_HDR_H__

#include <glib.h>

#if __BYTE_ORDER == __LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#else
#error "Please specify the byte order!"
#endif

#define	SyncWord	0x4156

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif
typedef struct _dvb_avpes_hdr {

	char	SyncWord_hi;
	char	SyncWord_lo;

	char	StreamID;	
	char	Counter; 	
	char	Reserved;	

#if __BYTE_ORDER == __LITTLE_ENDIAN

	char	PreBytes		:2;
	char	PostBytes		:2;
	char	PTS_Flag		:1;
	char	ReservedFlags	:3;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	ReservedFlags	:3;		
	char	PTS_Flag		:1;
	char	PostBytes		:2;
	char	PreBytes		:2;

#else
#error "Please specify the byte order!"
#endif

	char 	Length_hi;
	char	Length_lo;		

} dvb_avpes_hdr_t attr_pack;


/**
 * Get the SyncWord value from the AVPES header.
 *
 * @param   p  pointer to dvb_avpes_hdr_t.
 */
#define AVPES_GET_SYNCWORD(p) \
  ((((p)->SyncWord_hi) << 8) | ((p)->SyncWord_lo))


/**
 * Set the SyncWord value in a AVPES header.
 *
 * @param    p   pointer to dvb_ts_hdr_t.
 * @param    v   SyncWord
 */
#define AVPES_SET_SYNCWORD(p,v) \
do { \
  (p)->SyncWord_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (p)->SyncWord_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)



/**
 * StreamIDs
 */
#define AVPES_VIDEO_STREAM_ID		0x01
#define AVPES_MAIN_AUDIO_STREAM_ID	0x02



/**
 * Get the length of the AVPES payload.
 *
 * @param   p  pointer to dvb_avpes_hdr_t.
 */
#define AVPES_GET_LENGTH(p) \
  ((((p)->Length_hi) << 8) | ((p)->Length_lo))


/**
 * Set the length of the payload in a AVPES header.
 *
 * @param    p   pointer to dvb_ts_hdr_t.
 * @param    v   length
 */
#define AVPES_SET_LENGTH(p,v) \
do { \
  (p)->Length_lo = ((char)(((unsigned __int16)(v)) & 0x00FF)); \
  (p)->Length_hi = ((char)((((unsigned __int16)(v)) & 0xFF00) >> 8)); \
} while (0)


#define MAX_VID_PAYLOADLENGTH	0x17f8	
#define MAX_AUD_PAYLOADLENGTH	0x07f8

typedef struct _dvb_avpes_PTS {

#if __BYTE_ORDER == __LITTLE_ENDIAN

	/*char	pts_31_30					:2;
	
	  char	pts_29_15_lo					:7;
	  char	pts_29_15_hi					:8;
	
	  char	pts_14_0_lo					:7;
	
	  char	pts_14_0_hi					:8;
	*/

	char	pts_29_15_hi_6			:6;
	char	pts_31_30				:2;

	char	pts_29_15_mid_6			:6;
	char	pts_29_15_mid_2			:2;

	char	pts_14_0_hi_7			:7;
	char	pts_29_15_lo_1			:1;

	char	pts_14_0_lo_7			:7;
	char   pts_14_0_mid_1			:1;

#elif __BYTE_ORDER == __BIG_ENDIAN

	char	pts_31_30				:2;
	char	pts_29_15_hi_6			:6;

	char	pts_29_15_mid_2			:2;
	char	pts_29_15_mid_6			:6;

	char	pts_29_15_lo_1			:1;
	char	pts_14_0_hi_7			:7;

	char   pts_14_0_mid_1			:1;
	char	pts_14_0_lo_7			:7;

#else
#error "Please specify the byte order!"
#endif

} dvb_avpes_PTS_t attr_pack;
#ifdef _WIN32
#pragma pack(pop,p,1)
#endif



/**
 * Get the PTS 29 15 timestamp.
 *
 * @param   pt  pointer to _dvb_avpes_PTS
 */
#define AVPES_GET_PTS_2915(pt) \
  ((((pt)->pts_29_15_hi_6) << 9) | (((pt)->pts_29_15_mid_2) << 7) |  (((pt)->pts_29_15_mid_6) << 1) | ((pt)->pts_29_15_lo_1))

/**
 * Get the PTS 14 0 timestamp.
 *
 * @param   pt  pointer to _dvb_avpes_PTS
 */
#define AVPES_GET_PTS_140(pt) \
  ((((pt)->pts_14_0_hi_7) << 8) | (((pt)->pts_14_0_mid_1) << 7) | ((pt)->pts_14_0_lo_7))




/**
 * Set the PTS_2915 timestamp.
 *
 * @param    pt  pointer to _dvb_avpes_PTS.
 * @param    v   value
 */
#define AVPES_SET_PTS_2915(pt, v) \
do { \
  (pt)->pts_29_15_lo_1  = ((char)(((unsigned __int16)(v)) & 0x0001)); \
  (pt)->pts_29_15_mid_6 = ((char)((((unsigned __int16)(v)) & 0x007E) >> 1)); \
  (pt)->pts_29_15_mid_2 = ((char)((((unsigned __int16)(v)) & 0x0180) >> 7)); \
  (pt)->pts_29_15_hi_6  = ((char)((((unsigned __int16)(v)) & 0x7E00) >> 9)); \
} while (0)


/**
 * Set the PTS_140 timestamp.
 *
 * @param    pt  pointer to _dvb_avpes_PTS
 * @param    v   value
 */
#define AVPES_SET_PTS_140(pt, v) \
do { \
  (pt)->pts_14_0_lo_7  = ((char)(((unsigned __int16)(v)) & 0x007F)); \
  (pt)->pts_14_0_mid_1 = ((char)((((unsigned __int16)(v)) & 0x0080) >> 7)); \
  (pt)->pts_14_0_hi_7  = ((char)((((unsigned __int16)(v)) & 0x7F00) >> 8)); \
} while (0)



#endif   /* __DVB_AVPES_HDR_H__ */