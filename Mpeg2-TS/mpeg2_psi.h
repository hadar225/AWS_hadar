/* mpeg2_psi.h
 * Routines supporting MPEG2 Transport Stream ansalyzation
 *
 * Copyright 2004, Dipl.-Ing. Wolfram STERING wolfi@xxxxxxxxxxxxxx
 * 							  Lutz FINDEISEN lfindeis@xxxxxxxxxxxxxx
 *
 * Project partially sponsored by ESA projectnr.: JR/UOS/17471/03/NL/ND
 * "Standardisation Support of enhanced IEFTF IP Encapsulation Tecniques"
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
/**
 * @file
 */
#ifndef __MPEG2_PSI_H__
#define __MPEG2_PSI_H__
#include "dvb_incs/sect_pat.h"
typedef void (*dvb_descr_print_t)( char *descbuf, proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );
void dvb_pat_prog_print( dvb_sect_pat_program_t *prog, proto_tree *tree, tvbuff_t *tvb, int nr);

/* Descriptor Printers. */
void dvb_descr_print_tbl( dvb_descr_print_t *dt, char *descrbuf, int maxlen,  proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset);
#define dvb_descr_print(f,b,l,i,h)	dvb_descr_print_tbl( descriptor_printer, (f), (b), (l), (i), (h))
#define dvb_descr_mhp_print(f,b,l,i,h)	dvb_descr_print_tbl( mhp_descr_printer, (f), (b), (l), (i), (h))

/**
 * Program association table print for human readable output in the middle pane of Ethereal
 * 
 * @param patbuf pointer to the raw data
 * @param tree protocol tree to construct the tree in the middle pane
 * @param tvb testy virtualizable buffer
 */
extern 
void dvb_pat_print( char *patbuf, proto_tree *tree, tvbuff_t *tvb);

/**
 * Program map table print for human readable output in the middle pane of Ethereal
 * 
 * @param patbuf pointer to the raw data
 * @param tree protocol tree to construct the tree in the middle pane
 * @param tvb testy virtualizable buffer
 */
extern
void dvb_pmt_print( char *pmtbuf, proto_tree *tree, tvbuff_t *tvb );

/**
 * Network information table print for human readable output in the middle pane of Ethereal
 * 
 * @param patbuf pointer to the raw data
 * @param tree protocol tree to construct the tree in the middle pane
 * @param tvb testy virtualizable buffer
 */
extern
void dvb_nit_print( char *nitbuf, proto_tree *tree, tvbuff_t *tvb );

/**
 * Service description table print for human readable output in the middle pane of Ethereal
 * 
 * @param patbuf pointer to the raw data
 * @param tree protocol tree to construct the tree in the middle pane
 * @param tvb testy virtualizable buffer
 */
extern
void dvb_sdt_print( char *sdtbuf, proto_tree *tree, tvbuff_t *tvb );

/**
 * Multioprotocol encapsulation table print for human readable output in the middle pane of Ethereal
 * 
 * @param patbuf pointer to the raw data
 * @param tree protocol tree to construct the tree in the middle pane
 * @param tvb testy virtualizable buffer
 */
extern
void dvb_mpe3e_print( char *mpe3ebuf,proto_tree *tree, tvbuff_t *tvb );
#endif
