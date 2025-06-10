
#ifndef __METADATA_DESC_H
#define __METADATA_DESC_H

#ifdef __GNUC__
#define attr_pack __attribute__((packed))
#elif defined(_WIN32)
#define attr_pack
#pragma pack(push,p,1)
#else
#error "Compiler not supported"
#endif



typedef struct _metadata_descriptor{			// KLV example
	char		descriptor_tag ;					//= 0x26 (38)
	char		descriptor_length ;					//= 0x09 (9)
	__int16		metadata_application_format;		//= 0x0100-0x0103 (see Table 4)
	char		metadata_format;					//= 0xFF
	//unsigned int		metadata_format_identifier;			//= 0x4B4C5641 = �KLVA�
	char mfi1;
	char mfi2;
	char mfi3;
	char mfi4;
	char		metadata_service_id;				//= 0x00
#if		__BYTE_ORDER == __LITTLE_ENDIAN
	char		reserved				: 4;		//= �1111�
	char		DSM_CC_flag				: 1;		//= �0�
	char		decoder_config_flags	: 3;		//= �000�
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		decoder_config_flags	: 3;		//= �000�
	char		DSM_CC_flag				: 1;		//= �0�
	char		reserved				: 4;		//= �1111�
#else
#error "Please specify the byte order!"
#endif
} metadata_descriptor_t attr_pack;

typedef struct _metadata_AU_cell{				// KLV example
	char		metadata_service_id;			//= 0x00
	char		sequance_number;				//= 0x00
#if		__BYTE_ORDER == __LITTLE_ENDIAN
	char		reserved					: 4;		//= �1111�
	char		random_access_indicator		: 1;		//= �0�
	char		decoder_config_flag			: 1;		//= �0�
	char		cell_fragment_indication	: 2;		//= �0�
#elif __BYTE_ORDER == __BIG_ENDIAN
	char		cell_fragment_indication	: 2;		//= �0�
	char		decoder_config_flag			: 1;		//= �0�
	char		random_access_indicator		: 1;		//= �0�
	char		reserved					: 4;		//= �1111�
#else
#error "Please specify the byte order!"
#endif
	
	// LEON old 05-01-16
	//unsigned __int16		AU_cell_data_length;	
	// new 
	char	au_cell_data_length_hi;
	char	au_cell_data_length_lo;

} metadata_AU_cell_t attr_pack;

// LEON added 05-01-16
#define DVB_GET_AU_CELL_DATA_LEN(p) \
  ((((p)->au_cell_data_length_hi) << 8) | ((p)->au_cell_data_length_lo))

// LEON added 05-01-16
#define DVB_SET_AU_CELL_DATA_LEN(p,l) \
do { \
  (p)->au_cell_data_length_lo = ((char)(((unsigned __int16)(l)) & 0x00FF)); \
  (p)->au_cell_data_length_hi = ((char)((((unsigned __int16)(l)) & 0xFF00) >> 8)); \
} while (0)

#ifdef _WIN32
#pragma pack(pop,p1)
#endif

#endif   /* __METADATA_DESC_H */