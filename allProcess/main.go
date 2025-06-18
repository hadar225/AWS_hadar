package main

import (
	"fmt"
	"io"
	"net"
	"os"

	// "os/exec"
	"strconv"
	"time"

	//betterklv "github.com/hadar225/AWS_hadar/betterklv"
	mux "github.com/hadar225/AWS_hadar/mux"
)

func main() {
	//levelA - convert mp4 to mp2ts - completed
	// inputFile := "allProcess/input.mp4"
	// outputFile := "allProcess/output.ts"
	// cmd := exec.Command("ffmpeg", "-i", inputFile, "-c:v", "mpeg2video", "-c:a", "mp2", outputFile)
	// // Set command output to the terminal
	// cmd.Stdout = os.Stdout
	// cmd.Stderr = os.Stderr
	// fmt.Println("Converting MP4 to MPEG-TS...")
	// if err := cmd.Run(); err != nil {
	// 	fmt.Printf("Error during conversion: %v\n", err)
	// 	return
	// }
	// fmt.Printf("Conversion complete! Output file: %s\n", outputFile)

	//levelB - build KLV for muxer - completed
	//levelB.1 - receive KLV - completed
	

	//levelB.2 - builds a KLV-encoded byte buffer - completed
	

	// עכשיו אפשר לשמור את klvPacket לקובץ או לשלוח אותו
	//fmt.Printf("KLV Packet: %x\n", klvPacket)

	//levelB.2 - buildKLVDataBuffer - completed, res is the decoded result of the values
	// res, err := betterklv.HandleKLVBuffer(klvPacket)
	// if err != nil {
	// 	fmt.Println("Error parsing KLV:", err)
	// 	return
	// } else {
	// 	fmt.Println("Decoded KLV buffer:", res)
	// }
	// for k, v := range res {
	// 	if k == betterklv.VMTI_DATA_SET {
	// 		fmt.Println(k, ":")
	// 		for k2, v2 := range v.(map[string]interface{}) {
	// 			fmt.Println(" ", k2, ":", v2)
	// 		}
	// 	} else {
	// 		fmt.Println(k, ":", v)
	// 	}
	// }

	//levelC - broadcats the .ts video and muxedklv
	udpAddr := "235.235.235.235:1234"
	conn, err := net.Dial("udp", udpAddr)
	if err != nil {
		fmt.Println("Failed to open UDP connection:", err)
		return
	}
	defer conn.Close()

	tsFile, err := os.Open("allProcess/output.ts")
	if err != nil {
		fmt.Println("Failed to open TS file:", err)
		return
	}
	defer tsFile.Close()

	packetBuf := make([]byte, 188)

	mux.EnableDescriptors()
	mux.SetMetadatasToSend(0, 1, 0, 0)

	var tsPacketCount int = 0
	var pos int = 0
	muxedKlv := make([]byte, 188)
	for {
		// Read one TS packet from file
		n, err := io.ReadFull(tsFile, packetBuf)
		if err == io.EOF {
			fmt.Println("End of TS file reached, looping...")
			tsFile.Seek(0, 0) // rewind file to start again for continuous streaming
			continue
		} else if err != nil {
			fmt.Println("Error reading TS file:", err)
			break
		}
		if n != 188 {
			fmt.Println("Incomplete TS packet read")
			break
		}

		mux.HandlePSI(packetBuf, 0, 0)
		// Send TS packet over UDP
		_, err = conn.Write(packetBuf)
		if err != nil {
			fmt.Println("Failed to send TS packet:", err)
			break
		}

		tsPacketCount++
		if tsPacketCount%3 == 0 {
			klvPacket := BuildDynamicKLV() 
			err := SendKLV(conn, klvPacket, &pos, muxedKlv, 0xFD, 1)
			if err != nil {
				fmt.Println("Error sending KLV:", err)
				break
			}
		}

		// האטה של 4 מילישניות בין חבילות (כ-250 חבילות לשנייה)
		time.Sleep(4 * time.Millisecond)
	}
}

func BuildDynamicKLV() []byte {
	duration, _ := GetVideoDuration("allProcess/output.ts")
	
	startTime := uint64(time.Now().Unix())  // e.g. 2021-05-03 00:00:00 UTC
	endTime := startTime + uint64(duration) // endTime = startTime + duration in seconds
	serialNumber := "ABC123456"

	var localSetPayload []byte

	localSetPayload = append(localSetPayload, EncodeLocalSetField(72, EncodeUint64Value(startTime))...)
	localSetPayload = append(localSetPayload, EncodeLocalSetField(2, EncodeUint64Value(endTime))...)
	localSetPayload = append(localSetPayload, EncodeLocalSetField(12, []byte(strconv.FormatFloat(duration, 'f', 2, 64)))...)
	localSetPayload = append(localSetPayload, EncodeLocalSetField(10, []byte(serialNumber))...)

	// Build full KLV packet: UK + BER length + payload
	UK := UniversalKey0601
	lengthBytes := EncodeBERLength(len(localSetPayload))

	klvPacket := []byte{}
	klvPacket = append(klvPacket, UK...)
	klvPacket = append(klvPacket, lengthBytes...)
	klvPacket = append(klvPacket, localSetPayload...)

	return klvPacket
}


// SendKLV מנסה לשלוח KLV PES Packet אם יש עוד נתונים זמינים
func SendKLV(conn net.Conn, klvPacket []byte, pos *int, muxedKlv []byte, pid uint8, serviceID int) error {
	// אין יותר מה לשלוח
	if *pos >= len(klvPacket) {
		*pos = 0 // איפוס המיקום לאפס כדי להתחיל את השידור מחדש
		fmt.Println("KLV buffer exhausted, restarting KLV stream.")
	}

	// נסה לבנות PES Packet
	ok := mux.BuildMetadataPes(klvPacket[*pos:], len(klvPacket), muxedKlv, pid, serviceID, pos, nil)
	if ok == 1 {
		_, err := conn.Write(muxedKlv)
		if err != nil {
			return fmt.Errorf("failed to send KLV packet: %w", err)
		}
		fmt.Println("Sent KLV TS packet")
	}
	return nil
}
