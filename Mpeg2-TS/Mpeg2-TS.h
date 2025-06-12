// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the MPEG2TS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// MPEG2TS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef MPEG2TS_H
#define MPEG2TS_H

//#ifdef MPEG2TS_EXPORTS
//#define MPEG2TS_API __declspec(dllexport)
//#else
//#define MPEG2TS_API __declspec(dllimport)
//#endif

// This class is exported from the Mpeg2-TS.dll
//class MPEG2TS_API CMpeg2TS {
//public:
//	CMpeg2TS(void);
//	// TODO: add your methods here.
//};
//
//extern MPEG2TS_API int nMpeg2TS;

//MPEG2TS_API int fnMpeg2TS(void);

typedef unsigned char BYTE;

#define false 0
#define true 1
// typedef int bool; // or #define bool int

int handle_psi(BYTE *packet, int is_partial, int is_dish);

int fake_pmt(BYTE *packet, int is_partial, int is_dish);

int build_metadata_pes(BYTE *packet_in, unsigned short size, BYTE *packet_out, BYTE pid, BYTE service_id, int *general_offset, BYTE *user_pts);

void set_metadatas_to_send(int stat, int dyn, int anch, int ent);

BYTE *get_last_pts();

void enable_descriptors();

#endif