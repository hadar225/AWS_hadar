package main

import (
	"fmt"
	"io"
	"net"
	"os"

	//"os/exec"
	"strconv"
	"time"

	betterklv "github.com/hadar225/AWS_hadar/betterklv"
	mux "github.com/hadar225/AWS_hadar/mux"
)

func main() {
	//levelA - convert mp4 to mp2ts - completed
	// start := time.Now()
	// inputFile := "sender/c.mp4"
	// outputFile := "sender/output.ts"
	// cmd := exec.Command("ffmpeg", "-i", inputFile, "-c:v", "mpeg2video", "-c:a", "mp2", outputFile)
	// // Set command output to the terminal
	// cmd.Stdout = os.Stdout
	// cmd.Stderr = os.Stderr
	// fmt.Println("Converting MP4 to MPEG-TS...")
	// if err := cmd.Run(); err != nil {
	// 	fmt.Printf("Error during conversion: %v\n", err)
	// 	return
	// }
	// elapsed := time.Since(start)
	// fmt.Printf("time take is: %s\n", elapsed)
	// fmt.Printf("Conversion complete! Output file: %s\n", outputFile)

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

	tsFile, err := os.Open("sender/output.ts")
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
	time.Sleep(20 * time.Second)
	klvPacket := BuildDynamicKLV()
	fmt.Println("klvPacket:", klvPacket)
	res, err := betterklv.HandleKLVBuffer(klvPacket)
	if err != nil {
		fmt.Println("Error parsing KLV:", err)
		return
	} else {
		fmt.Println("Decoded KLV buffer:", res)
	}
	fmt.Printf("KLV packet length: %d bytes\n", len(klvPacket))
	for {
		// Read one TS packet from file
		n, err := io.ReadFull(tsFile, packetBuf)
		if err == io.EOF {
			fmt.Println("End of TS file reached. Stopping.")
			break
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
		if tsPacketCount%3 == 0 && pos < len(klvPacket) {
			fmt.Println("🔁 Current KLV pos:", pos)
			err := SendKLV(conn, klvPacket, &pos, muxedKlv, 0xFD, 1)
			if err != nil {
				fmt.Println("Error sending KLV:", err)
				break
			}
		}

		time.Sleep(5 * time.Second)
	}
}

// func BuildDynamicKLV() []byte {
// 	duration, _ := GetVideoDuration("sender/output.ts")

// 	startTime := uint64(time.Now().Unix())  // e.g. 2021-05-03 00:00:00 UTC
// 	endTime := startTime + uint64(duration) // endTime = startTime + duration in seconds
// 	serialNumber := "ABC123456"

// 	var localSetPayload []byte

// 	localSetPayload = append(localSetPayload, EncodeLocalSetField(72, EncodeUint64Value(startTime))...)
// 	localSetPayload = append(localSetPayload, EncodeLocalSetField(2, EncodeUint64Value(endTime))...)
// 	localSetPayload = append(localSetPayload, EncodeLocalSetField(12, []byte(strconv.FormatFloat(duration, 'f', 2, 64)))...)
// 	localSetPayload = append(localSetPayload, EncodeLocalSetField(10, []byte(serialNumber))...)

// 	// Build full KLV packet: UK + BER length + payload
// 	UK := UniversalKey0601
// 	lengthBytes := EncodeBERLength(len(localSetPayload))

// 	klvPacket := []byte{}
// 	klvPacket = append(klvPacket, UK...)
// 	klvPacket = append(klvPacket, lengthBytes...)
// 	klvPacket = append(klvPacket, localSetPayload...)

// 	return klvPacket
// }

func BuildDynamicKLV() []byte {
	// שלב 1: Universal Key תקני
	universalKey := []byte{
		0x06, 0x0E, 0x2B, 0x34,
		0x02, 0x0B, 0x01, 0x01,
		0x0E, 0x01, 0x03, 0x01,
		0x01, 0x00, 0x00, 0x00,
	}

	// שלב 2: ערכים דינמיים
	startTime := uint64(time.Now().Unix())
	endTime := startTime + 60 // לדוגמה: וידאו של דקה
	duration := float64(60.0)
	serialNumber := "ABC123456"

	// שלב 3: בניית ה־Local Set (Tag + Length + Value)
	var localSet []byte
	localSet = append(localSet, EncodeLocalSetField(0x48, EncodeUint64Value(startTime))...) // Tag 72: Start Time
	localSet = append(localSet, EncodeLocalSetField(0x02, EncodeUint64Value(endTime))...)   // Tag 2: End Time
	localSet = append(localSet, EncodeLocalSetField(0x0C, []byte(strconv.FormatFloat(duration, 'f', 2, 64)))...) // Tag 12: Duration as ASCII
	localSet = append(localSet, EncodeLocalSetField(0x0A, []byte(serialNumber))...)         // Tag 10: Serial Number

	// שלב 4: Encode BER Length
	lengthBytes := EncodeBERLength(len(localSet))

	// שלב 5: חיבור מלא - UK + BER length + Local Set
	klvPacket := []byte{}
	klvPacket = append(klvPacket, universalKey...)
	klvPacket = append(klvPacket, lengthBytes...)
	klvPacket = append(klvPacket, localSet...)

	return klvPacket
}


func SendKLV(conn net.Conn, klvPacket []byte, pos *int, muxedKlv []byte, pid uint8, serviceID int) error {
	for *pos < len(klvPacket) {
		ok := mux.BuildMetadataPes(klvPacket[*pos:], len(klvPacket), muxedKlv, pid, serviceID, pos, nil)
		fmt.Printf("📦 muxedKlv first 4 bytes: % X\n", muxedKlv[:4])
		pid := int(muxedKlv[1]&0x1F)<<8 | int(muxedKlv[2])
		fmt.Printf("🎯 muxedKlv PID: 0x%X (%d)\n", pid, pid)

		if ok == 1 {
			_, err := conn.Write(muxedKlv)
			if err != nil {
				return fmt.Errorf("failed to send KLV packet: %w", err)
			}
			fmt.Println("📤 Sent KLV TS packet at pos", *pos)
		} else {
			// Not enough data to produce a full TS packet yet
			break
		}
	}

	if *pos >= len(klvPacket) {
		fmt.Println("✅ Finished sending full KLV payload.")
	}

	return nil
}
