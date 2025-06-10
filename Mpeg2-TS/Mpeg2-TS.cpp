// Mpeg2-TS.cpp : Defines the exported functions for the DLL application.
//

#include "Mpeg2-TS.h"

#include "ts_hdr.h"
#include "pes_hdr.h"
#include "sect_gen.h"
#include "sect_pat.h"
#include "sect_pmt.h"
#include "pes_hdr.h"
#include "dvb_crc32_table.h"
#include "metadata_descriptor.h"
#include <math.h>

#include <ext/hash_map>
#include <memory.h>

//using namespace std;
//using namespace __gnu_cxx;

typedef struct reas_info{
	/** PID packet identifier */ 
	unsigned short PID; 
	/** frame used to key to identify the datagram in the hash table */	
	unsigned int frame;
	/** defr_off offset into the defragmented packet */
	unsigned int	 defr_off;
	/** rest bytes left to reassemble */
	unsigned int	 rest;
	/** slen length of the complete datagram */
	unsigned short slen;
 } reas_info_t;


reas_info_t *reas, *recvr_inf =NULL, *actual_inf = NULL;

unsigned int pmt_pid = 0;


dvb_CRC32_t crc, crc_new = 0;

char				metadata_counter_matip ;// = 0;
char				metadata_counter_smatip ;// = 0;
char				metadata_counter_moran ;// = 0;
char				metadata_counter_entities ;// = 0;
pts_dts_flag2_t		last_pts = {0};
BYTE*				metadata_pmt_section = NULL;

// TODO: update metadata_service_id into 0x20, 0x55, 0x12, 0xeb

static const BYTE VIMS_METADATA_SERVICE_ID_STAT = 0x01;
static const BYTE VIMS_METADATA_SERVICE_ID_DYN = 0x02;
static const BYTE VIMS_METADATA_SERVICE_ID_ANCH = 0x03;
static const BYTE VIMS_METADATA_SERVICE_ID_ENTITIES = 0x04;

dvb_CRC32_t dvb_crc32_calc( const unsigned char *sectbuf, unsigned int size )
{
	dvb_CRC32_t crc32 = 0xffffffff;
	unsigned int i = 0;

	for (i = 0; i < size; i++)
		crc32 = (crc32 << 8) ^ dvb_crc_table[(((crc32 >> 24) ^ sectbuf[i]) & 0xff)];

	return crc32;
}

int build_metadata_pes(BYTE* packet_in, unsigned __int16 size, BYTE* packet_out, BYTE pid, BYTE service_id, int *general_offset, BYTE* user_pts)
{
	dvb_ts_adaptation_field_t*	adapt;
	dvb_ts_hdr_t*				header;
	dvb_pes_hdr_t*				pes_hdr;
	pes_flags_t*				pes_flags;
	pts_dts_flag2_t*			pts;
	int							res = 0;
	int							pos = *general_offset;
	BYTE*						stuffing;
	int							offset, i, j, len;
	unsigned __int8				sequance_number = 0;
	int						need_to_break = false;
	// LEON !!!
	int							 ts_payload_counter = 0;

	// LEON !!!
	// Sets the current pos
	//if(iterator != 0)
		//pos = iterator * 6 * (TS_LEN - TS_HDR_LEN);

	for (i = 0; i < 7; i++)
	{	
		header = (dvb_ts_hdr_t*)(packet_out + i*TS_LEN);		/* TS Header*/
		header->sync_byte = TS_SYNC_BYTE;
		DVB_SET_TS_PID(header, pid);
		header->transport_error_indicator = 0;
		header->transport_priority = 0;
		header->transport_scrambling_control = TS_SCV_NOT_SCRAMBLED;
		
		if(service_id == VIMS_METADATA_SERVICE_ID_DYN)
		{
			header->continuity_counter = metadata_counter_matip;
			metadata_counter_matip++;
			metadata_counter_matip%=16;
		}
		else if(service_id == VIMS_METADATA_SERVICE_ID_ANCH)
		{
			header->continuity_counter = metadata_counter_moran;
			metadata_counter_moran++;
			metadata_counter_moran%=16;
		}
		else if(service_id == VIMS_METADATA_SERVICE_ID_STAT)
		{
			header->continuity_counter = metadata_counter_smatip;
			metadata_counter_smatip++;
			metadata_counter_smatip%=16;
		}
		else if(service_id == VIMS_METADATA_SERVICE_ID_ENTITIES)
		{
			header->continuity_counter = metadata_counter_entities;
			metadata_counter_entities++;
			metadata_counter_entities%=16;
		}

		// Sets the output ts packet offset
		offset = TS_HDR_LEN;

		// PES
		if(*general_offset == 0 && i == 0)
		{
			header->payload_unit_start_indicator = true;
			// packet_in bigger or equal to the pes packet size(whats left affter the headers)
			if(size >= TS_LEN - TS_HDR_LEN - sizeof(dvb_pes_hdr_t) - 
				sizeof(pes_flags_t) - sizeof(pts_dts_flag2_t) - 
				sizeof(metadata_AU_cell_t)) //188 - 4 - 6 - 3 - 5 - 5 = 165
			{
				header->adaptation_field_control = TS_ADP_PAYLOAD_ONLY;
			}
			// packet in is smaller then pes
			else
			{
				header->adaptation_field_control = TS_ADP_BOTH;					
				adapt = (dvb_ts_adaptation_field_t*) (packet_out + i*TS_LEN + offset);
				adapt->adaptation_field_length = 165 - 1 - size;
			
				*((BYTE*) adapt + 1) = 0x00; // set adaptation field flags to false (=0)
			
				stuffing = (BYTE*) adapt + 2;
			
				// stuffing
				for (j=0; j<adapt->adaptation_field_length; j++)
					stuffing[j] = 0xFF;

				// Adding the 
				offset += (sizeof(dvb_ts_adaptation_field_t)-1) + adapt->adaptation_field_length;

				// LEON 
				// 17-12-2015 need to break because we dont need more peses then this pes
				need_to_break = true;
			}
			
			pes_hdr = (dvb_pes_hdr_t*)(packet_out + i*TS_LEN + offset);
			pes_hdr->stream_id = 0xfc;
			pes_hdr->packet_start_code_prefix_hi = 0;
			pes_hdr->packet_start_code_prefix_mid = 0;
			pes_hdr->packet_start_code_prefix_lo = 1;

			int size_with_au_header = size + sizeof(pes_flags_t) + sizeof(pts_dts_flag2_t) + sizeof(metadata_AU_cell_t) + 
				ceil(((double)size - 165) / 179) * sizeof(metadata_AU_cell_t);
			
			DVB_SET_PES_LEN(pes_hdr, size_with_au_header); // Size + metadata AU
			
			offset += sizeof(dvb_pes_hdr_t);

			pes_flags = (pes_flags_t*)(packet_out + i*TS_LEN + offset);
			pes_flags->pes_start_code_prefix = 0x02;		// static
			pes_flags->pes_scrambling_control = 0;			// no scramling
			pes_flags->pes_priority = 0;					// no priority
			pes_flags->data_alignment_indicator = 0;		// no aligntment
			pes_flags->copyright = 0;						
			pes_flags->original_or_copy = 0;				
			pes_flags->pts_dts_flag = DVB_PTS_ONLY;			// only PTS
			pes_flags->escr_flag = 0;						
			pes_flags->es_rate_flag = 0;					
			pes_flags->dsm_trick_mode_flag = 0;				
			pes_flags->additional_copy_info_flag = 0;		
			pes_flags->pes_crc_flag = 0;					// no CRC
			pes_flags->pes_extension_flag = 0;				// no extention
			pes_flags->pes_header_data_length = sizeof(pts_dts_flag2_t); //5

			offset += sizeof(pes_flags_t);

			pts = (pts_dts_flag2_t*)(packet_out + i*TS_LEN + offset);
			
			// memcpy(pts, &last_pts, sizeof(pts_dts_flag2_t)); //OLD
			// LEON NEW SYNC UPDATE
			if(user_pts == NULL)
				memcpy(pts, &last_pts, sizeof(pts_dts_flag2_t));
			else
				memcpy(pts, user_pts, sizeof(pts_dts_flag2_t));

			offset += sizeof(pts_dts_flag2_t);

			metadata_AU_cell_t* au_cell = (metadata_AU_cell_t*)(packet_out + i*TS_LEN + offset);

			au_cell->metadata_service_id = service_id;
			au_cell->sequance_number = sequance_number;
			sequance_number++;
			
			if(header->adaptation_field_control == -1)//TS_ADP_BOTH)
			{
				au_cell->cell_fragment_indication = -1;
				au_cell->random_access_indicator = 1;
			}
			else
			{
				au_cell->cell_fragment_indication = -2;
				au_cell->random_access_indicator = 0;
			}
			
			au_cell->reserved = 0xF;
			
			offset += sizeof(metadata_AU_cell_t);

			if(header->adaptation_field_control == TS_ADP_PAYLOAD_ONLY)
			{
				// LEON old 05-01-16
				//au_cell->AU_cell_data_length = (unsigned __int16)(TS_LEN - offset);// Bigger or equal to one pes
				DVB_SET_AU_CELL_DATA_LEN(au_cell,(unsigned __int16)(TS_LEN - offset));
			}
			else
			{
				// LEON old 05-01-16
				//au_cell->AU_cell_data_length = (unsigned __int16)size;// Smaller then one pes
				DVB_SET_AU_CELL_DATA_LEN(au_cell,(unsigned __int16)size);
			}

			memcpy(packet_out + i*TS_LEN + offset, packet_in + pos, TS_LEN - offset);		// copy the data to the PES

			pos += TS_LEN - offset;

			// LEON !!!
			ts_payload_counter++;
		}
		else
		{
			header->payload_unit_start_indicator = false;

			// Whats left in packet_in is bigger or equal then ts packet
			if(size - pos >= TS_LEN - TS_HDR_LEN - sizeof(metadata_AU_cell_t)) //188 - 4 - 5 = 179
			{
				header->adaptation_field_control = TS_ADP_PAYLOAD_ONLY;
			}
			// Whats left in packet_in is smaller then ts packet
			else if ((size - pos) < (TS_LEN - TS_HDR_LEN - sizeof(metadata_AU_cell_t)) && 
				(size - pos) > 0)
			{
				header->adaptation_field_control = TS_ADP_BOTH;
			}
			// All packet_in packetize into the transport stream and now 
			// we fill it with
			else
			{
				header->adaptation_field_control = TS_ADP_ADAPTFIELD_ONLY;
			}

			// Handle adptation field
			if(header->adaptation_field_control != TS_ADP_PAYLOAD_ONLY)
			{
				adapt = (dvb_ts_adaptation_field_t*) (packet_out + i*TS_LEN + offset);

				if(header->adaptation_field_control == -2)
					adapt->adaptation_field_length = 183;				// Maximum length (NULL TS packet)
				else
					adapt->adaptation_field_length =					// Set the length of the stuffed
					(TS_LEN - TS_HDR_LEN - sizeof(metadata_AU_cell_t)) - (size - pos) - 1;				// 178 - what's left in packet
			
				if(((unsigned char)adapt->adaptation_field_length) > 0)
				{
					*((BYTE*) adapt + 1) = 0x00;	// set adaptation field flags to false (=0)
			
					BYTE* stuffing = (BYTE*) adapt;
					stuffing += 2;

					unsigned char stuf_len = ((unsigned char)adapt->adaptation_field_length) - 1;
			
					// stuffing
					for (j=0; j<stuf_len; j++)
						stuffing[j] = 0xFF;

					// Adding the 
					offset += sizeof(dvb_ts_adaptation_field_t) + stuf_len;

					// LEON !!!
					// if we are here, its mean we are stuffing the packet it is the last packet in the chain
					// so we should break from the loop
					need_to_break = true;

				}
				else if(((unsigned char)adapt->adaptation_field_length) == 0)
					offset++;
			}

			// Handle the payload data
			if(header->adaptation_field_control != -2) //TS_ADP_ADAPTFIELD_ONLY
			{	
				// Calc the lendth of the payload
				if(header->adaptation_field_control == TS_ADP_PAYLOAD_ONLY)
					len = TS_LEN - TS_HDR_LEN - sizeof(metadata_AU_cell_t);
				else
					len = size - pos; //TS_LEN - TS_HDR_LEN + 1 - sizeof(dvb_ts_adaptation_field_t) - (unsigned char)adapt->adaptation_field_length;
				 
				metadata_AU_cell_t* au_cell = (metadata_AU_cell_t*)(packet_out + i*TS_LEN + offset);

				au_cell->metadata_service_id = service_id;
				au_cell->sequance_number = sequance_number;
				sequance_number++;
				
				if(size > pos + len)
					au_cell->cell_fragment_indication = 0;
				else
					au_cell->cell_fragment_indication = 1;

				au_cell->random_access_indicator = 0;
				au_cell->reserved = 0xF;

				// LEON old 05-01-16
				// au_cell->AU_cell_data_length = len;
				DVB_SET_AU_CELL_DATA_LEN(au_cell, len);
				
				
				offset += sizeof(metadata_AU_cell_t);				

				memcpy(packet_out + i*TS_LEN + offset, packet_in + pos, len); // Copy the data to payload

				pos += len; // Adds the lendgth

				// moved to outside of the if
				//ts_payload_counter++;
				// | 
				// |
				// V
			}

			ts_payload_counter++;
		}

		if(need_to_break)
			break;
	}

	// LEON !!!
	// Sets the return value (0 = finished, iterator + 1 = there is more to send, -1 = error)
	/*if(pos == size)
		res = 0;
	else if (pos < size)
		res = iterator + 1;
	else
		res = -1;
	*/

	if(pos > size)
		return -1;

	*general_offset = pos;

	return ts_payload_counter;
}

void update_pts(dvb_ts_hdr_t * header)
{
	int offset = TS_HDR_LEN;

	if(!header->payload_unit_start_indicator)
		return;

	if(header->adaptation_field_control & 0x02)
	{
		//old
		//BYTE* len = (BYTE*)(header + offset);
		//LEON
		BYTE* len = ((BYTE*)header) + 4;
		offset += *len;
		offset++;
	}

	dvb_pes_hdr_t* pes_hdr = (dvb_pes_hdr_t*)(((BYTE*)header) + offset);

	if(pes_hdr->stream_id == 0xBC ||
		pes_hdr->stream_id == 0xBE ||
		pes_hdr->stream_id == 0xBF ||
		pes_hdr->stream_id == 0xF0 ||
		pes_hdr->stream_id == 0xF1 ||
		pes_hdr->stream_id == 0xFF ||
		pes_hdr->stream_id == 0xF8 ||
		pes_hdr->stream_id == 0xF2)
		return;

	offset += sizeof(dvb_pes_hdr_t);

	pes_flags_t* pes_flags = (pes_flags_t*) (((BYTE*)header) + offset);

	if(pes_flags->pts_dts_flag < 0)
	{
		offset += sizeof(pes_flags_t);
		pts_dts_flag2_t* pts = (pts_dts_flag2_t*) (((BYTE*)header) + offset);
		memcpy(&last_pts, (void*)(pts), sizeof(pts_dts_flag2_t));
		pts->pts_start_code_prefix = 0x2;
	}
}

// LEON
unsigned __int16	s_last_pcr = 0;
unsigned __int16	s_last_desc_len = 0;
unsigned __int16	s_last_prog_info_len = 0;
int				s_update_crc = false;
int				s_stat = false;
int				s_dyn = false;
int				s_anch = false;
int				s_reserved = false;
int				s_enable_descriptors = false;

int					add_metadata_descriptor(BYTE* pmt_section, BYTE service_id, BYTE entry_id, int startPos);
int					add_dish_metadata_descriptors(BYTE* pmt_section, BYTE service_id, BYTE entry_id, int startPos);

// Read packect data
int handle_psi(BYTE* packet, int is_partial, int is_dish)
{
	dvb_ts_hdr_t * header = (dvb_ts_hdr_t*)packet;		/* TS Header*/
	dvb_ts_adaptation_field_t * adapt; 					/* TS adaption field header */
	dvb_sect_gen_t* secth;  							/* minimal Section Header including sectionlength*/
	dvb_pes_hdr_t* pesh;								/* PES Header to check section or PES */
	
	unsigned short current_pid = DVB_GET_TS_PID(header);

	if(current_pid != 0 &&
		current_pid != pmt_pid)
	{
		if(header->payload_unit_start_indicator)
			update_pts(header);
		
		return 0;
	}

	unsigned __int8 pf = 0;	
	int offset = TS_HDR_LEN;
	int res = 0;
	

	if(header->adaptation_field_control & 0x02)
	{
		adapt = (dvb_ts_adaptation_field_t*) (packet + offset);
		offset += 1 + (unsigned __int8)adapt->adaptation_field_length;
	}

	if(header->payload_unit_start_indicator)
	{
		pf = *(unsigned __int8*)(packet + offset);		
	}	

	// PAT
	if(current_pid == 0 && pmt_pid == 0)
	{
		offset += pf + 1; // Adds the pointer_field

		dvb_sect_pat_t* pat = (dvb_sect_pat_t*) (packet + offset);
		
		UINT y = (UINT) DVB_GET_PAT_PROG_COUNT(pat);
		
		offset += sizeof(dvb_sect_pat_t);

		for(int i=0; i<y; i++)
		{
			dvb_sect_pat_program_t * pat_prog = (dvb_sect_pat_program_t*) (packet + offset + i * sizeof(dvb_sect_pat_program_t));
			unsigned __int16 pn = (unsigned __int16) DVB_GET_PAT_PROGNR(pat_prog);
			pmt_pid = (unsigned __int16)DVB_GET_PAT_PROG_PID(program_map_PID, pat_prog);
		}
	}
	// PMT
	else if(current_pid == pmt_pid)
	{
		offset += pf + 1; // Adds the pointer_field

		if(header->payload_unit_start_indicator)
		{
			//offset += pf; // Adds the pointer_field

			dvb_sect_pmt_t* pmt = (dvb_sect_pmt_t*) (packet + offset);
			
			// LEON
			if(is_partial)
			{
				pmt->gen_hdr.current_next_indicator = 0x1;
				pmt->gen_hdr.version_number =  (pmt->gen_hdr.version_number + 1) % 32;
			}
			// LEON
			unsigned __int16 pcr = DVB_GET_PMT_PCRPID(pmt);
			if(pcr != s_last_pcr)
				s_update_crc = true;
			s_last_pcr = pcr;
			unsigned __int16 desc_len = DVB_GET_SECTION_LENGTH(&pmt->gen_hdr);
			if(desc_len != s_last_desc_len)
				s_update_crc = true;
			s_last_desc_len = desc_len;
			unsigned __int16 prog_info_len =  DVB_GET_PROGINFO_LENGTH(pmt);
			if(prog_info_len != s_last_prog_info_len)
				s_update_crc = true;
			s_last_prog_info_len = prog_info_len;

			//4 * 16; (5 + 11) * 4; Stream info + Metadata descriptor * 4 (static, dynamic, anchor, entities)

			int metadata_count = 0;
			if(s_stat)
				metadata_count++;
			
			if(s_dyn)
				metadata_count++;
			
			if(s_anch)
				metadata_count++;
			
			if(s_reserved)
				metadata_count++;
			
			// LEON OLD
			//int insert_bytes = 64;

			int insert_bytes;
			
			insert_bytes = metadata_count * 5; // entry size is 5 X number of entrys

			if(s_enable_descriptors)
				insert_bytes = metadata_count *  16; //4 * 16; (5 + 11) * 4; Stream info + Metadata descriptor * 4 (static, dynamic, anchor, entities)

			//insert_bytes = 160; OLD0
			if (is_dish)
				insert_bytes = metadata_count * 40; //4 * 40; (5 + 13 + 11 + 11) * 4; Stream info + Metadata pointer descriptor + Metadata descriptor + Metadata STD descriptor + * 4 (static, dynamic, anchor, entities)


			if(metadata_pmt_section == NULL)
			{
				metadata_pmt_section = new BYTE[insert_bytes];
				
				int pos = 0;
				
				if (is_dish)
				{
					if(s_stat)
						pos = add_dish_metadata_descriptors(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_STAT, 0xfc, pos);
					if(s_dyn)
						pos = add_dish_metadata_descriptors(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_DYN, 0xfd, pos);
					if(s_anch)
						pos = add_dish_metadata_descriptors(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_ANCH, 0xfe, pos);
					if(s_reserved)
						pos = add_dish_metadata_descriptors(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_ENTITIES, 0xfb, pos);
				}
				else
				{
					
					if(s_stat)
						pos = add_metadata_descriptor(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_STAT, 0xfc, pos);
					if(s_dyn)
						pos = add_metadata_descriptor(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_DYN, 0xfd, pos);
					if(s_anch)
						pos = add_metadata_descriptor(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_ANCH, 0xfe, pos);
					if(s_reserved)
						pos = add_metadata_descriptor(metadata_pmt_section, VIMS_METADATA_SERVICE_ID_ENTITIES, 0xfb, pos);
				}
				
			}

			if(header->adaptation_field_control == TS_ADP_PAYLOAD_ONLY)
			{
				unsigned __int16 pil = DVB_GET_PROGINFO_LENGTH(pmt);
				unsigned __int16 scl = DVB_GET_SECTION_LENGTH(&pmt->gen_hdr);

				unsigned __int16 end_of_sec = scl + 3 + offset - 4; // pmt ands without crc
				offset += sizeof(dvb_sect_pmt_t);
				offset += pil;
				dvb_sect_pmt_entry_t* entry;
				unsigned __int16 espid;
				do
				{
					entry = (dvb_sect_pmt_entry_t*) (packet + offset);
					espid = DVB_GET_SECT_PMT_EPID(entry);
					offset += sizeof(dvb_sect_pmt_entry_t) + DVB_GET_SECT_PMT_ESINFO_LEN(entry);
				}
				while(offset < end_of_sec);				

				//int insert_bytes = 14;//5 + 9; // Stream info + Metadata descriptor
				desc_len += insert_bytes;
				DVB_SET_SECTION_LENGTH(&pmt->gen_hdr, desc_len);

				memcpy((void*)(BYTE*)(packet + offset),
					(void*)metadata_pmt_section, 
					insert_bytes);

				if(!crc_new || s_update_crc)
				{
					crc_new = dvb_crc32_calc((const unsigned char*)pmt, 3 + desc_len - 4);
					crc_new = ntohl(crc_new);

					// LEON
					s_update_crc = false;
				}
				
				*(unsigned int*)(packet + offset + insert_bytes) = crc_new;

				res = (unsigned int)header->continuity_counter;

				// LEON
				if (is_partial)
				{
					header->continuity_counter++;
					header->continuity_counter %= 16;
				}
				res = 1;
			}
			else
			{
				
				//crc =  dvb_crc32_calc((BYTE*)(packet + offset) , (UINT) desc_len + 3 - 4);
				DVB_SET_SECTION_LENGTH(&pmt->gen_hdr, desc_len + insert_bytes);
				//BYTE* arr_temp = new BYTE[TS_LEN - 4 - offset + pf + 1];
				
				// Relocate the memory
				int rel_off = offset - pf - 1;
				
				memcpy((void*)(BYTE*)(packet + rel_off - insert_bytes), 
					(void*)(BYTE*)(packet + rel_off), 
					sizeof(dvb_sect_pmt_t) + prog_info_len + 1);

				memcpy((void*)(BYTE*)(packet + offset + sizeof(dvb_sect_pmt_t) + prog_info_len - insert_bytes),
					(void*)metadata_pmt_section, 
					insert_bytes);

				if(header->adaptation_field_control & 0x02)
					adapt->adaptation_field_length -= insert_bytes;
				
				if(!crc_new || s_update_crc)
				{
					crc_new = dvb_crc32_calc((const unsigned char*)(packet + offset - pf - insert_bytes) , 
						TS_LEN - 4 - (offset - pf - insert_bytes));
					crc_new = ntohl(crc_new);

					// LEON
					s_update_crc = false;
				}
				
				*(unsigned int*)(packet + 184) = crc_new;

				res = (unsigned int)header->continuity_counter;
				
				// LEON
				if(is_partial)
				{
					header->continuity_counter++;
					header->continuity_counter %= 16;
				}
				res = 1;
			}
		}
	}

	return res;
}

int fake_pmt(BYTE* packet, int is_partial, int is_dish)
{
	dvb_ts_hdr_t * header = (dvb_ts_hdr_t*)packet;		/* TS Header*/
	dvb_ts_adaptation_field_t * adapt; 					/* TS adaption field header */
	dvb_sect_gen_t* secth;  							/* minimal Section Header including sectionlength*/
	dvb_pes_hdr_t* pesh;								/* PES Header to check section or PES */
	
	unsigned short current_pid = DVB_GET_TS_PID(header);

	if(current_pid != 0 &&
		current_pid != pmt_pid)
	{
		if(header->payload_unit_start_indicator)
			update_pts(header);
		
		return 0;
	}

	unsigned __int8 pf = 0;	
	int offset = TS_HDR_LEN;
	int res = 0;
	

	if(header->adaptation_field_control & 0x02)
	{
		adapt = (dvb_ts_adaptation_field_t*) (packet + offset);
		offset += 1 + (unsigned __int8)adapt->adaptation_field_length;
	}

	if(header->payload_unit_start_indicator)
	{
		pf = *(unsigned __int8*)(packet + offset);		
	}

	// PMT
	if(current_pid == pmt_pid)
	{
		offset += pf + 1; // Adds the pointer_field

		if(header->payload_unit_start_indicator)
		{
			//offset += pf; // Adds the pointer_field

			dvb_sect_pmt_t* pmt = (dvb_sect_pmt_t*) (packet + offset);
			
			// LEON
			if(is_partial)
			{
				pmt->gen_hdr.current_next_indicator = 0x0;
				pmt->gen_hdr.version_number = (pmt->gen_hdr.version_number+1) % 32;
				/*else if(pmt->gen_hdr.version_number < 0 && pmt_ver > 16 - pmt->gen_hdr.version_number - 1)
					pmt_ver = 16 - pmt->gen_hdr.version_number - 2;*/
				//s_pmt_ver = (s_pmt_ver + 1) % 32;
			}
			// LEON
			unsigned __int16 pcr = DVB_GET_PMT_PCRPID(pmt);
			if(pcr != s_last_pcr)
				s_update_crc = true;
			s_last_pcr = pcr;
			unsigned __int16 desc_len = DVB_GET_SECTION_LENGTH(&pmt->gen_hdr);
			if(desc_len != s_last_desc_len)
				s_update_crc = true;
			s_last_desc_len = desc_len;
			unsigned __int16 prog_info_len =  DVB_GET_PROGINFO_LENGTH(pmt);
			if(prog_info_len != s_last_prog_info_len)
				s_update_crc = true;
			s_last_prog_info_len = prog_info_len;

			if(header->adaptation_field_control == TS_ADP_PAYLOAD_ONLY)
			{
				unsigned __int16 pil = DVB_GET_PROGINFO_LENGTH(pmt);
				unsigned __int16 scl = DVB_GET_SECTION_LENGTH(&pmt->gen_hdr);

				unsigned __int16 end_of_sec = scl + 3 + offset - 4; // pmt ands without crc
				offset += sizeof(dvb_sect_pmt_t);
				offset += pil;
				dvb_sect_pmt_entry_t* entry;
				unsigned __int16 espid;
				do
				{
					entry = (dvb_sect_pmt_entry_t*) (packet + offset);
					espid = DVB_GET_SECT_PMT_EPID(entry);
					offset += sizeof(dvb_sect_pmt_entry_t) + DVB_GET_SECT_PMT_ESINFO_LEN(entry);
				}
				while(offset < end_of_sec);				

				//int insert_bytes = 14;//5 + 9; // Stream info + Metadata descriptor

				DVB_SET_SECTION_LENGTH(&pmt->gen_hdr, desc_len);

				/*memcpy((void*)(BYTE*)(packet + offset),
					(void*)metadata_pmt_section, 
					insert_bytes);*/

				if(!crc_new || s_update_crc)
				{
					crc_new = dvb_crc32_calc((const unsigned char*)pmt, 3 + desc_len - 4);
					crc_new = ntohl(crc_new);

					// LEON
					s_update_crc = false;
				}
				
				//*(unsigned int*)(packet + offset + insert_bytes) = crc_new;

				res = (unsigned int)header->continuity_counter;

				// LEON
				if (is_partial)
				{
					//header->continuity_counter++;
					//header->continuity_counter %= 16;
				}
				res = 1;
			}
			else
			{
				
				//crc =  dvb_crc32_calc((BYTE*)(packet + offset) , (UINT) desc_len + 3 - 4);

				//BYTE* arr_temp = new BYTE[TS_LEN - 4 - offset + pf + 1];
				
				// Relocate the memory
				int rel_off = offset - pf - 1;
				
				memcpy((void*)(BYTE*)(packet + rel_off), 
					(void*)(BYTE*)(packet + rel_off), 
					sizeof(dvb_sect_pmt_t) + prog_info_len + 1);
				
				if(!crc_new || s_update_crc)
				{
					crc_new = dvb_crc32_calc((const unsigned char*)(packet + offset - pf) , 
						TS_LEN - 4 - (offset - pf));
					crc_new = ntohl(crc_new);

					// LEON
					s_update_crc = false;
				}
				
				*(unsigned int*)(packet + 184) = crc_new;

				res = (unsigned int)header->continuity_counter;
				
				// LEON
				if(is_partial)
				{
					//header->continuity_counter++;
					//header->continuity_counter %= 16;
				}
				res = 1;
			}
		}		
	}

	return res;
}

void enable_descriptors()
{
	s_enable_descriptors = true;
}

void set_metadatas_to_send(int stat, int dyn, int anch, int reserved)
{
	s_stat = stat;
	s_dyn = dyn;
	s_anch = anch;
	s_reserved = reserved;
}

BYTE* get_last_pts()
{
	BYTE* pts = (BYTE*)malloc(sizeof(pts_dts_flag2_t));
	memcpy(pts,&last_pts,sizeof(pts_dts_flag2_t));
	return pts;
}

int add_metadata_descriptor(BYTE* pmt_section, BYTE service_id, BYTE entry_id, int startPos)
{
	int offset = 0;

	dvb_sect_pmt_entry_t* entry_new = (dvb_sect_pmt_entry_t*)(metadata_pmt_section + startPos);
	entry_new->stream_type = 0x15;
	DVB_SET_SECT_PMT_EPID(entry_new, entry_id);
	DVB_SET_SECT_PMT_ESINFO_LEN(entry_new, 0); // 0 = no descriptors

	offset += 5; // enrty size = 5

	if(s_enable_descriptors)
	{
		DVB_SET_SECT_PMT_ESINFO_LEN(entry_new, 11); // 11 = size of metadata desciptor

		// Metadata Stream descriptor
		metadata_descriptor_t* metadata_desc = (metadata_descriptor_t*)(metadata_pmt_section + 5 + startPos);
		metadata_desc->descriptor_tag = 0x26;
		metadata_desc->descriptor_length = 0x09;
		metadata_desc->metadata_application_format = 0x0001; // user defined				
		metadata_desc->metadata_format = 0xFF;
		//metadata_desc->metadata_format_identifier = 0x534D4956; // = VIMS //0x4E4D4150; //(int)"PAMN"; //0x41564C4B; // KLVA //(int)"PAMN";
		metadata_desc->mfi1 = 0x56;
		metadata_desc->mfi2 = 0x49;
		metadata_desc->mfi3 = 0x4D;
		metadata_desc->mfi4 = 0x53;

	
		metadata_desc->metadata_service_id = service_id;

		metadata_desc->reserved = 0XF;
		metadata_desc->decoder_config_flags = 0;
		metadata_desc->DSM_CC_flag = 0;

		offset += 11; // descriptor size = 11
	}
	return startPos + offset;
}

int add_dish_metadata_descriptors(BYTE* pmt_section, BYTE service_id, BYTE entry_id, int startPos)
{
	dvb_sect_pmt_entry_t* entry_new = (dvb_sect_pmt_entry_t*) (pmt_section + startPos); //27
	entry_new->stream_type = 0x15;			
					
	DVB_SET_SECT_PMT_ESINFO_LEN (entry_new, 35); // 11 = size of metadata desciptor
	// Metadata Stream descriptor
	metadata_descriptor_t* metadata_desc_pointer = (metadata_descriptor_t*)(pmt_section + 5 + startPos);

	metadata_desc_pointer->descriptor_tag = 0x25;
	metadata_desc_pointer->descriptor_length = 0x0b;
	metadata_desc_pointer->metadata_application_format = 0x0001; // user defined				
	metadata_desc_pointer->metadata_format = 0xFF;
	//metadata_desc_pointer->metadata_format_identifier = 0x534D4956; // = VIMS
	DVB_SET_SECT_PMT_EPID (entry_new, entry_id); /******************** need to generate available pid */
	metadata_desc_pointer->metadata_service_id = service_id;

	pmt_section[16 + startPos] = (int)0x00;
	pmt_section[17 + startPos] = (int)0x01;
				
	metadata_desc_pointer->reserved = 0X0;
	metadata_desc_pointer->decoder_config_flags = 0x00;
	metadata_desc_pointer->DSM_CC_flag = 0x0;

	int pos = 18;

	// Metadata Stream descriptor
	metadata_descriptor_t* metadata_desc = (metadata_descriptor_t*)(pmt_section + pos + startPos); // +16

	metadata_desc->descriptor_tag = 0x26;
	metadata_desc->descriptor_length = 0x09;
	metadata_desc->metadata_application_format = 0x0001; // user defined				
	metadata_desc->metadata_format = 0xFF;
	//metadata_desc->metadata_format_identifier = 0x534D4956; // = VIMS

	metadata_desc->metadata_service_id = service_id;

	metadata_desc->reserved = 0x00;//0XF;
	metadata_desc->decoder_config_flags = 0;
	metadata_desc->DSM_CC_flag = 0;

	
	metadata_descriptor_t* metadata_std_desc = (metadata_descriptor_t*)(pmt_section + 11 + pos + startPos); //+32

	metadata_std_desc->descriptor_tag = 0x27;
	metadata_std_desc->descriptor_length = 0x09;
	metadata_std_desc->metadata_application_format = 0x02c0; // user defined				
	metadata_std_desc->metadata_format = 0x71;
	//metadata_std_desc->metadata_format_identifier = 0xc00200c0; // DISH PLAYER CONSTRAINT

	metadata_std_desc->metadata_service_id = service_id;

	metadata_std_desc->reserved = 0X0;
	metadata_std_desc->decoder_config_flags = 0;
	metadata_std_desc->DSM_CC_flag = 0;
	

	return pos + 22 + startPos;
}

// This is the constructor of a class that has been exported.
// see Mpeg2-TS.h for the class definition
/*CMpeg2TS::CMpeg2TS()
{
	return;
}*/

int main()
{
	printf("test\n");
	return 0;
}
