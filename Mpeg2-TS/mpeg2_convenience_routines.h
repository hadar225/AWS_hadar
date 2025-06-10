/* mpeg2_convenience_routines.h
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
#ifndef __MPEG2_CONVENIENCE_ROUTINES_H__
#define __MPEG2_CONVENIENCE_ROUTINES_H__

#define BCD2INT(b) ((((b) & 0xF0)>>4) * 10 + ((b) & 0x0F))
#define GET_ULONG(b) ((gulong)((b)[0] << 24 | (b)[1] << 16 | (b)[2] << 8 | (b)[3]))
#define GET_USHORT(b) ((gushort)((b)[0] << 8 | (b)[1]))
unsigned int bcd2int( unsigned int bcd );
unsigned int int2bcd( unsigned int integer );
/**
 * Returns a string containing the name of a table_id
 *
 * @param  tid inter with the table_id
 */
extern
char *dvb_table_id_str(char tid);

/**
 * Returns a string specifying the program element carried with a specific PID
*
* @param stype integer holding the stream_type
*/
extern
char *dvb_stream_type_str( char stype );

/**
 * Returns a string containing the name of the descriptor
 *
 * @param dtag integer holding the descriptor tag
 */
extern
char *dvb_descr_tag_str( char dtag );

/**
 * Returns a string specifying the service 
 * 
 * @param stype integer holding the service type
 */

extern
char *dvb_service_type_str( char stype );

/**
 * Returns a string containig the name of a CA system id
 *
 * @param casid integer holding the CA system id
 */
extern
char *dvb_CA_system_id_str( unsigned __int16 casid );

/**
 * Returns a string containing the name of linkage type
 * 
 * @param linkage_type integer holding the linkage type
 */
extern
char *dvb_linkage_type_str( char linkage_type );

/**
 * Returns a string specifying the content
 *
 * @param nibble integer
 */
extern 
char *dvb_content_str(int nibble);

/**
 * Returns a string containing the name of the stream content and component type
 *
 * @param cont_comp integer holding the stream content and the component type
 */
extern
char *dvb_cont_comp_str(int cont_comp);

/**
 * Returns a string containing the name of the teletext type
 *
 * @param type integer holding the teletext type
 */
extern
char *dvb_descr_txt_type(int type);

/**
 * Returns a string containing the name of the VBI type 
 * 
 * @param type integer holding the VBI type
 */
extern
char *dvb_descr_vbi_str(int type);

/**
 * Returns a string containing the name of the audio type 
 *
 * @param type integer holding the audio type
 */
extern
char *dvb_descr_audio_str(int type);

/**
 * Returns a string containing the name of the private data specifiaction
 *
 * @param type integer holding the private data specification type
 */ 
extern
char *dvb_descr_pds_str(int type);


extern
char *char2alphabet( unsigned char fb);

/**
 * Convenience function for the better displayability for long strings in Ethereal
 *
 * @param tree protocol tree displayed in the middle pane
 * @param tvb  testy virtulaizable buffer
 * @param textstr rather long string
 * @param len length of the string
 */
//extern
//void proto_tree_add_longtext(proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset, unsigned char *textstr, char len);

/**
 * Returns a string containing the name of a data broadcast id 
 *
 * @param dbid integer holding the data broadcast id
 */
extern
char *dvb_data_broadcast_id_str( unsigned __int16 dbid );

/**
 * Returns a string containing the name of application type
 * 
 * @param app_type integer holding the application type
 */
extern
char *dvb_app_type_str( unsigned __int16 app_type );

#endif