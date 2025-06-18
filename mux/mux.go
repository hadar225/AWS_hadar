package mux

/*
#cgo CFLAGS: -I../Mpeg2-TS        
#cgo LDFLAGS: -L. -lmux -lm
#include "Mpeg2-TS.h"
*/
import "C"
import (
	"unsafe"
)

// int handle_psi(BYTE* packet, int is_partial, int is_dish);

// int fake_pmt(BYTE* packet, int is_partial, int is_dish);

// int build_metadata_pes(BYTE* packet_in, unsigned short size, BYTE* packet_out, BYTE pid, BYTE service_id, int* general_offset, BYTE* user_pts);

// void set_metadatas_to_send(int stat, int dyn, int anch, int ent);

// BYTE* get_last_pts();

// void enable_descriptors();

// HandlePSI func
func HandlePSI(packet []byte, is_partial int, is_dish2 int) int {
	return (int)(C.handle_psi((*C.uchar)(unsafe.Pointer(&packet[0])), (C.int)(is_partial), (C.int)(is_dish2)))
}

// EnableDescriptors func
func EnableDescriptors() {
	C.enable_descriptors()
}

// SetMetadatasToSend func
func SetMetadatasToSend(stat int, dyn int, anch int, ent int) {
	C.set_metadatas_to_send((C.int)(stat), (C.int)(dyn), (C.int)(anch), (C.int)(ent))
}

func BuildMetadataPes(packet_in []byte, size int, packet_out []byte, pid uint8, service_id int, general_offset *int, user_pts []byte) int {
	if user_pts != nil {
		return int(C.build_metadata_pes((*C.uchar)(unsafe.Pointer(&packet_in[0])), (C.ushort)(uint16(size)), (*C.uchar)(unsafe.Pointer(&packet_out[0])), (C.uchar)(pid), (C.uchar)(service_id), (*C.int)(unsafe.Pointer(general_offset)), (*C.uchar)(unsafe.Pointer(&user_pts[0]))))
	}
	return int(C.build_metadata_pes((*C.uchar)(unsafe.Pointer(&packet_in[0])), (C.ushort)(uint16(size)), (*C.uchar)(unsafe.Pointer(&packet_out[0])), (C.uchar)(pid), (C.uchar)(service_id), (*C.int)(unsafe.Pointer(general_offset)), (*C.uchar)(unsafe.Pointer(uintptr(0)))))
}

// HandleTS func
// func HandleTS(packet []byte, buffer []byte, pid *int32, left *int32, size *int32, au_left *int32, metadata_formats []int32, last_pcr *int32, pmt_version_number *int32, pmt_ver_skip_count *int32, stream_types []int32) int32 {
// 	//handle_ts(unsigned char* packet, unsigned char* buffer, unsigned int* pid, int* metadata_tlen, int* metadata_tsize, int* au_left, unsigned int* metadata_formats,int* s_last_pcr,int* pmt_version_number,int* pmt_ver_skip_count,unsigned int* stream_types)
// 	//handle_ts(ts, metadata, &pid, &metadata_size_left, &metadata_size, &au_left, metadata_formats, &s_last_pcr, &pmt_version_number, &pmt_ver_skip_count, stream_types);

// 	return (int32)(C.handle_ts((*C.uchar)(unsafe.Pointer(&packet[0])),
// 		(*C.uchar)(unsafe.Pointer(&buffer[0])),
// 		(*C.uint)(unsafe.Pointer(pid)),
// 		(*C.int)(unsafe.Pointer(left)),
// 		(*C.int)(unsafe.Pointer(size)),
// 		(*C.int)(unsafe.Pointer(au_left)),
// 		(*C.uint)(unsafe.Pointer(&metadata_formats[0])),
// 		(*C.int)(unsafe.Pointer(last_pcr)),
// 		(*C.int)(unsafe.Pointer(pmt_version_number)),
// 		(*C.int)(unsafe.Pointer(pmt_ver_skip_count)),
// 		(*C.uint)(unsafe.Pointer(&stream_types[0]))))
// 	//C.test(666)
// 	/*
// 		cs := C.CString("Hello from stdio\n")
// 		C.myprint(cs)
// 		C.free(unsafe.Pointer(cs))
// 	*/
// }
