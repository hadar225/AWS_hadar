/* mpeg2_descriptors.h
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
#ifndef __MPEG2_DESCRIPTORS_H__
#define __MPEG2_DESCRIPTORS_H__
/**
 * Video stream descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern 
void dvb_descr_video_stream_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset);

/**
 * Audio stream descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_audio_stream_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset);

/**
 * Networl name  descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_network_name_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset);

/**
 * Service list descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_service_list_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Satellite delivery system print descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_satellite_delivery_system_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );

/**
 * Cable delivery system descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_cable_delivery_system_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );

/**
 * Terrestrial delivery system descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_terr_delivery_system_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Service descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_service_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Data broadcast descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_data_broadcast_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );

/**
 * CA identifier descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_ca_id_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Linkage descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_linkage_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );

/**
 * Data broadcast identifier descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_data_broadcast_id_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb, unsigned __int16 offset );

/**
 * Carousel identifier descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_carousel_id_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Stream identifier descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_stream_id_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Application signaling descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_appsig_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Association tag descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_assoc_tag_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Short event descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_short_event_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Extended event descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_extended_event_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Time shifted event descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_tshift_event_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * AC3 descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_ac3_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * CA Identifier descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_CA_id_print( char *descrbuf,   proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Content descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_content_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Parental rating descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern 
void dvb_descr_parental_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Telephone descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_tefephone_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * Teletext  descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_teletext_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * Component descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_component_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * PDC descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_pdc_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * VBI descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_vbi_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * ISO639 language descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_iso639_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16	 offset);

/**
 * Service descriptor table descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_sdt_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Registration descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_reg_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Country availability descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_country_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Private data specification descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_priv_data_spec_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Frequency list descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_frequency_list_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Local time offset descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_loc_time_offset_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Mosaic descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_mosaic_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Multilingual bouquet name descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_multiling_bouq_name_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Multilingual component descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_multiling_component_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Multilingual network name descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_multiling_netname_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Mutlilingual service name descriptor printer
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_multiling_serv_name_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);

/**
 * Conditional Access descriptor
 * Descriptor printer display the information in human readable format in the middle pane of Ethereal
 * @param descrbuf pointer to a buffer containing the raw data
 * @param tree protocol tree for constructing the tree in the middle pane
 * @param offset integer holding the offset for the HEX pane in Ethereal
 */
extern
void dvb_descr_CA_print(char *descrbuf,  proto_tree *tree, tvbuff_t *tvb , unsigned __int16 offset);
#endif