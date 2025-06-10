/* $Id: dvb_types.h,v 1.2 2000/02/11 10:33:53 pmm Exp $ */

#ifndef __DVB_TYPES_H__
#define __DVB_TYPES_H__

typedef unsigned int 	uint8 	__attribute__(( __mode__( __QI__ ) ));
typedef unsigned int 	uint16	__attribute__(( __mode__( __HI__ ) ));
typedef unsigned int 	uint32	__attribute__(( __mode__( __SI__ ) ));

typedef uint16	dvb_pid_t;
typedef	uint16	dvb_sid_t;
typedef uint8	dvb_table_t;

#endif  /* __DVB_TYPES_H__ */