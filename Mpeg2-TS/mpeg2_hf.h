/* mpeg2_hf.h
 * Routines supporting MPEG2 Transport Stream ansalyzation
 * Copyright 2004, Lutz Findeisen lfindeis@xxxxxxxxxxxxxx
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
#ifndef __MPEG2_HF_H__
#define __MPEG2_HF_H__
/* Initialize the protocol and registered fields */
/* general field */
 int hf_reserved1 = -1;
 int hf_reserved = -1;
 int hf_Descriptors = -1;
 int hf_info_msg = -1;


 int proto_table_print = -1;

/*PAT print fields */
 int hf_service_nr = -1;
 int hf_program_number = -1;
 int hf_network_PID = -1;
 int hf_program_map_PID = -1;

/*NIT print fields */
 int hf_reserved_future_use = -1;
 int hf_network_descriptors_length = -1;
 int hf_transport_stream_loop_length = -1;
 int hf_tsid = -1;
 int hf_onid = -1;
 int hf_TS_descriptor_length = -1;
 int hf_Transport_Stream_Descriptions = -1;
 int hf_Network_Descriptors = -1;

/*mpe3e print fields */
 int hf_payload_scrambling_control = -1;
 int hf_LLC_SNAP_flag = -1;
 int hf_address_scrambling_control  =  -1;
 int hf_MAC_address = -1;
 int hf_LLC_SNAP = -1;

/*pmt print fields*/
 int hf_PCR_PID = -1;
 int hf_program_info_length = -1;
 int hf_program_info_descriptors = -1;
 int hf_stream_type = -1;
 int hf_EPID = -1;
 int hf_ES_info_length = -1;
 int hf_Elementary_stream_descriptions = -1;
 
 /*sdt print fields */
 int hf_original_network_PID = -1;
 int hf_SDT_Entry_loop = -1;
 int hf_reserved_future_use0 = -1;
 int hf_service_id = -1; /*also used for: service list descr*/
 int hf_running_status = -1;
 int hf_EIT_schedule_flag = -1;
 int hf_free_CA_mode = -1;
 int hf_EIT_present_following_flag = -1;
 int hf_descriptor_loop_length = -1;
 int hf_reserved_future_use1 = -1;
 
 /* dsmcc ait fields */
 
 /*dsmcc3b/3c print */
 int hf_protocol_discriminator = -1;
 int hf_dsmcc_type = -1;
 int hf_message_id = -1;
 int hf_transaction_id = -1;
 int hf_reserved_dsmcc = -1;
 int hf_adaptation_length = -1;
 int hf_message_length =-1;
 int hf_dsmcc_adapt_field = -1;
 int hf_message = -1;
 int hf_download_data_block = -1;
 int hf_block_number = -1;
 
 /*ait print*/
 int hf_common_descriptor_length = -1;
 int hf_common_descriptors = -1;
 int hf_Application_loop_length = -1;
 int hf_organization_id = -1;
 int hf_application_id = -1;
 int hf_application_control_code = -1;
 int hf_application_descriptor_len = -1; 
 int hf_Application_descriptor = -1; 

/*time print*/
 int hf_DAY = -1;
 int hf_UNIX_time = -1;

/*eit print*/
 int hf_transport_stream_id  = -1;
 int hf_original_network_id = -1;
 int hf_segment_last_section_number = -1;
 int hf_last_label_id = -1;
 int hf_event_loop = -1;
 
/*eit event print*/
 int hf_event_id = -1;
 int hf_start_time = -1;
 int hf_duration = -1;
 
/*Descriptor printer fields */
 int hf_descriptor_tag = -1;

/* conditional access print */
 int hf_CA_PID = -1;
 
/*video stream print */
 int hf_multiple_frame_rate_flag = -1;
 int hf_constraint_parameter_flag = -1;
 int hf_MPEG_1_only_flag = -1;
 int hf_still_picture_flag = -1;
 int hf_frame_rate_code = -1;
 int hf_sprofile_and_level_indication = -1;
 int hf_frame_rate_extension_flag = -1;
 int hf_chroma_format = -1;
 int hf_constrained_parameter_flag = -1;
 
 /*audio stream print */
 int hf_free_format_flag = -1;
 int hf_id = -1;
 int hf_layer = -1;
 int hf_variable_rate_audio_indicator =-1;
 
 /*network name print */
 int hf_network_name = -1;
 
 /* service list print */
 int hf_service_type = -1;
 
 /* satellite delivery system */
 /* cable delivery system */
 int hf_frequency_terr = -1;
 int hf_frequency = -1;
 int hf_modulation = -1;
 int hf_orbital_position = -1;
 int hf_symbol_rate = -1;
 int hf_west_east_flag = -1;
 int hf_FEC_inner = -1;
 int hf_polarization = -1;
 int hf_FEC_outer = -1;
 
 /*terrestrial delivery system print */
 int hf_bandwitdth = -1;
 int hf_constellation = -1;
 int hf_hirachy = -1;
 int hf_code_rate_HP = -1;
 int hf_code_rate_LP = -1;
 int hf_quard_internal = -1;
 int hf_transmission_mode = -1;
 int hf_other_freq = -1;
 
 /* service print */
 int hf_service_provider_name  = -1;
 int hf_service_name = -1;
 
 /* data broadcast print */
 int hf_data_broadcast_id = -1;
 int hf_component_tag = -1;
 int hf_selector_length = -1;
 int hf_selector = -1;
 int hf_ISO_639_language_code = -1;
 int hf_text_length = -1;
 int hf_text = -1;
 
 /*mpe info print*/
 int hf_MAC_address_range = -1;
 int hf_MAC_IP_mapping_flag = -1;
 int hf_alignment_indicator = -1;
 int hf_section_per_datagram = -1;
 
 /*dbc dc info print*/
 int hf_carousel_type_id = -1;
 int hf_timeout_DSI = -1;
 int hf_timeout_DII = -1;
 int hf_leak_rate = -1;
 
/* dbc oc info print*/
 int hf_language_code = -1;
 int hf_object_name = -1;

/*ca id print*/
 int hf_CA_system_id = -1;
 
/*linkage print*/
 int hf_linkage_type = -1;
 int hf_private_data_byte = -1; 
 

/*corousel id print */
 int hf_carousel_id = -1;
 int hf_format_id = -1;
 int hf_private_data = -1;

/*stream id print */
 int hf_component_id = -1;
 
/* appsig print */
int hf_application_type = -1;
int hf_ait_version = -1;

/*assoc tag print*/
int hf_association_tag = -1; 
int hf_timeout = -1;
int hf_use = -1;

/*short event print */
int hf_event_text = -1;
int hf_event_name = -1;

/*extendet event print */
int hf_descriptor_number = -1;
int hf_last_descr_number = -1;
int hf_length_of_items = -1;
int hf_item_descr_len = -1;
int hf_textstr = -1;

/*tshift event print*/
int hf_reference_service_id = -1;
int hf_reference_event_id = -1;

/*ac3 print */
int hf_Flags = -1;
int hf_comp_type = -1;
int hf_bsid = -1;
int hf_mainid = -1;
int hf_asvc = -1;
int hf_component_type = -1;
int hf_bs_id = -1;
int hf_main_id = -1;
int hf_asvc_value = -1;
int hf_additional_info = -1;


/*Content print */
int hf_content_nibble_level1 = -1;
int hf_content_nibble_level2 = -1;
int hf_user_nibble = -1;

int hf_country_code = -1;
int hf_rating = -1;

/*telephone print*/
int hf_foreign_availability = -1;
int hf_connection_type = -1;
int hf_country_prefix_length = -1;
int hf_international_area_code_length = -1;
int hf_operator_code_length = -1;
int hf_national_area_code_length = -1;
int hf_core_number_length = -1;

/*component print */
int hf_stream_content = -1;

/*PDC print*/
int hf_programme_identification_label = -1;

/*Teletext print*/
int hf_teletext_type = -1;
int hf_teletext_magazine_number = -1;
int hf_teletext_page_number = -1;

/*VBI data print*/
int hf_data_service_descriptor_length = -1;
int hf_data_service_id = -1;
int hf_field_parity = -1;
int hf_line_offset = -1;

/*iso 639 print*/
int hf_audo_type = -1;

/*sdt print*/
int hf_leak_valid_flag = -1;

/*registration print */
int hf_format_identifier = -1;
int hf_additional_identification_info = -1;

/*country avail. print*/
int hf_country_availability_flag = -1;

/*priv data spec print */
int hf_priv_data_spec = -1;

/*frequency list print*/
int hf_coding_type = -1;
int hf_centre_frequency = -1;

/*local time offset print*/
int hf_country_region_id = -1;
int hf_local_time_offset_polarity = -1;
int hf_time_of_change = -1;
int hf_local_time_offset = -1;
int hf_next_time_offset = -1;

/*mosaic print*/
int hf_mosaic_entry_point = -1;
int hf_nohec = -1;
int hf_novec = -1;
int hf_log_cell_id = -1;
int hf_log_cell_pres_nfo = -1;
int hf_el_cell_fl = -1;
int hf_el_cell_id = -1;
int hf_cell_link_nfo = -1;
int hf_bouquet_id = -1;

/*multiling. bouqet name print*/
int hf_bouquet_name_length = -1;

/*multiling. network name print*/
int hf_network_name_length = -1;

/*multiling. service name print*/
int hf_service_provider_name_length = -1;
int hf_service_name_length = -1;




/**************************************
 *									  *
 *          MHP descriptors           *
 *									  *
 **************************************/


/*mhp app print*/
int hf_application_profiles = -1;
int hf_application_profile = -1;
int hf_service_bound_flag = -1;
int hf_visibility = -1;
int hf_application_priority = -1;
int hf_transport_protocol_lables = -1;
int hf_application_name = -1;

/*mhp trprot print*/
int hf_protocol_id = -1;
int hf_protocol_label = -1;

/*mhp dvbjapp print*/
int hf_len = -1;

/*mhp japp loc print */
int hf_base_directory = -1;
int hf_classpath_extension = -1;
int hf_initial_class = -1;

/*mhp label print*/
int hf_label = -1;

/*mhp prio print */
int hf_priority_value = -1;
int hf_transparency_value = -1;

/*dsmcc dii print*/
int hf_download_id = -1;
int hf_block_sz = -1;
int hf_window_sz = -1;
int hf_ack_period = -1;
int hf_tc_dl_window = -1;
int hf_tc_dl_scenario = -1;
int hf_compability_descr_length  =-1;
int hf_compability_descr = -1;
int hf_number_of_modules = -1;
int hf_module = -1;
int hf_module_id = -1;
int hf_module_sz = -1;
int hf_module_version = -1;
int hf_module_info_length = -1;
int hf_module_info = -1;
int hf_module_timeout = -1;
int hf_block_timeout = -1;
int hf_min_block_time = -1;
int hf_taps_count = -1;
int hf_usr_info_len = -1;
int hf_private_data_length = -1;

/*dsmcc dsi print*/
int hf_server_id = -1;

/* dsmcc taps print */
int hf_tap = -1;
int hf_selector_type = -1;
int hf_tap_id = -1;
int hf_tap_use = -1;

/* dvb dsmcc srginfo print*/
int hf_service_gateway_info = -1;
int hf_download_taps_count = -1;
int hf_service_context_list_count = -1;

/* dvb dsmcc IOP IOR Print */
int hf_IOP_IOR = -1;
int hf_type_id_len = -1;
int hf_alignment_gap = -1;
int hf_tagged_profiles_count = -1;
int hf_IOP_IOR_id = -1;

/* dvb dsmcc tagged profile print*/
int hf_profile_id_tag = -1;
int hf_profile_data_len = -1;
int hf_byte_order = -1;
int hf_lite_components_count = -1;

/* dvb dsmcc lite component print */
int hf_component_data_len = -1;
int hf_version = -1;
int hf_object_key_len = -1;

/* dvb dsmcc sc print*/
int hf_service_context_id = -1;
int hf_data_len = -1;

/*dvb dsmcc biop print*/
int hf_BIOP_message = -1;
int hf_magic = -1;
int hf_BIOP_version = -1;
int hf_message_type = -1;
int hf_message_size = -1;
int hf_object_key = -1;
int hf_object_kind_len = -1;
int hf_object_kind = -1;
int hf_object_info_len = -1;
int hf_content_size  = -1;
int hf_message_body_len = -1;
int hf_content_length = -1;
int hf_bindings_count = -1;


/*dvb dsmcc BIOP bindings print */
int hf_binding = -1;
int hf_binding_type = -1;
int hf_end_binding = -1;

/*dvb dsmcc BIOP name print */
int hf_name_components_count = -1;
int hf_kind_len = -1;
int hf_BIOP_id  = -1;

#endif