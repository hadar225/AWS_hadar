package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"net"
	"os"
	"strconv"
	"time"

	//"os"
	//"os/exec"
	// betterklv "github.com/hadar225/AWS_hadar/betterklv"
	mux "github.com/hadar225/AWS_hadar/mux"
)

func main() {
	//levelA - convert mp4 to mp2ts - completed
	// inputFile := "./input.mp4"
	//outputFile := "./output.ts"
	// cmd := exec.Command("ffmpeg", "-i", inputFile, "-c:v", "mpeg2video", "-c:a", "mp2", outputFile)
	// // Set command output to the terminal
	// cmd.Stdout = os.Stdout
	// cmd.Stderr = os.Stderr
	// fmt.Println("Converting MP4 to MPEG-TS...")
	// if err := cmd.Run(); err != nil {
	// 	fmt.Printf("Error during conversion: %v\n", err)
	// 	return}
	// fmt.Printf("Conversion complete! Output file: %s\n", outputFile)

	//levelB - build KLV for muxer - completed
	//levelB.1 - receive KLV - completed
	duration, err := GetVideoDuration("allProcess/output.ts")
	if err != nil {
		fmt.Println("Error getting duration:", err)
		return
	}
	startTime := uint64(1620000000)         // e.g. 2021-05-03 00:00:00 UTC
	endTime := startTime + uint64(duration) // endTime = startTime + duration in seconds
	serialNumber := "ABC123456"

	//levelB.2 - builds a KLV-encoded byte buffer - completed
	klvPayload := []byte{}
	klvPayload = append(klvPayload, EncodeUint64(72, startTime)...)
	klvPayload = append(klvPayload, EncodeUint64(2, endTime)...)
	klvPayload = append(klvPayload, EncodeString(12, strconv.FormatFloat(duration, 'f', 2, 64))...)
	klvPayload = append(klvPayload, EncodeString(10, serialNumber)...)
	klvBuffer := WrapInKLV(klvPayload)
	fmt.Println("Encoded KLV buffer:", klvBuffer)

	//levelB.2 - buildKLVDataBuffer - completed, res is the decoded result of the values
	// res, err := betterklv.HandleKLVBuffer(klvBuffer)
	// if err != nil {
	// 	fmt.Println("Error parsing KLV:", err)
	// 	return
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

	//levelC - make klv ready for broadcasting
	//levelC.1 - Prepare buffers and inputs + allocate an output buffer for the TS packets
	klvBytes := klvBuffer // From your KLV payload
	pos := 0              // Start position in buffer
	//size := 0                  // Start position in output buffer
	//LastPTSHeader := uint64(0) // מערכת הזמן ב־MPEG-TS היא ב־90KHz (כל שנייה = 90,000 יחידות זמן), נמצא ב־PES header ומציין מתי המידע אמור להיות מוצג (במונחים של זמן)
	//userPtsBytes := EncodePTS(LastPTSHeader)
	muxedKlv := make([]byte, 188) // Or larger depending on metadata size
	//levelC.2 - wrap that into a MPEG-TS packetized PES stream
	//mux.BuildMetadataPes(klvBytes[pos:], len(klvBytes), muxedKlv[size:], uint8(0xFD), 2, &pos, userPtsBytes)
	fmt.Printf("muxedKlv HEX:\n%s\n", hex.Dump(muxedKlv))

	//levelD - broadcats the .ts video and muxedklv
	//levelD.1 - create udp conn
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

	const packetSize = 188
	packetBuf := make([]byte, packetSize)

	mux.EnableDescriptors()
	mux.SetMetadatasToSend(0, 1, 0, 0)

	// Streaming loop
	var staticCounter int = 0
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
		if n != packetSize {
			fmt.Println("Incomplete TS packet read")
			break
		}

		mux.HandlePSI(packetBuf, 0, 0)
		// Send TS packet over UDP
		_, err = conn.Write(packetBuf)
		if err != nil {
			fmt.Println("Failed to send TS packet:", err)
			break
		} else {
			fmt.Println("Send video packet as 188-byte TS packets")
		}
		staticCounter++
		if staticCounter%50 == 0 {
			pos = 0
			//for pos < len(klvBytes) {
			mux.BuildMetadataPes(klvBytes[pos:], len(klvBytes), muxedKlv, uint8(0xFD), 2, &pos, nil)
			klvReader := bytes.NewReader(muxedKlv)
			for {
				n, err := io.ReadFull(klvReader, packetBuf)
				if err == io.EOF {
					break
				} else if err == io.ErrUnexpectedEOF {
					// Pad the incomplete final packet
					padded := make([]byte, packetSize)
					copy(padded, packetBuf[:n])
					_, err = conn.Write(padded)
					if err != nil {
						fmt.Println("Failed to send final padded KLV packet:", err)
					}
					break
				} else if err != nil {
					fmt.Println("Failed to read KLV packet:", err)
					break
				}

				_, err = conn.Write(packetBuf)
				if err != nil {
					fmt.Println("Failed to send KLV packet:", err)
					break
				}
			}
			//}
		}
		time.Sleep(10 * time.Millisecond)
	}
}
