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

	fileInfo, err := tsFile.Stat()
	if err != nil {
		fmt.Println("Failed to get file info:", err)
		return
	}
	totalSize := fileInfo.Size()
	var currentOffset int64

	packetBuf := make([]byte, 188)

	mux.EnableDescriptors()
	mux.SetMetadatasToSend(0, 1, 0, 0)

	var tsPacketCount int
	var pos int

	const MaxMuxOutPackets = 10
	const tsSize = 188
	muxedKlv := make([]byte, tsSize*MaxMuxOutPackets)

	time.Sleep(20 * time.Second)

	// klvPacket := []byte{
	// 	6, 14, 43, 52, 2, 11, 1, 1, 14, 1, 3, 1, 1, 0, 0, 0, 129, 225, 2, 8, 0, 6, 17, 252, 199, 234, 0, 113, 4, 5, 49, 55, 52, 53, 52, 14, 4, 24, 123, 183, 68, 13, 4, 44, 201, 206, 182, 15, 2, 29, 128, 3, 2, 49, 53, 23, 4, 44, 205, 127, 157, 24, 4, 24, 123, 75, 34, 25, 2, 12, 43, 82, 4, 44, 205, 171, 219, 83, 4, 24, 123, 48, 142, 84, 4, 44, 205, 184, 151, 85, 4, 24, 123, 91, 53, 86, 4, 44, 205, 86, 222, 87, 4, 24, 123, 101, 142, 88, 4, 44, 205, 75, 105, 89, 4, 24, 123, 59, 137, 18, 4, 4, 212, 92, 229, 19, 4, 0, 0, 0, 0, 20, 4, 0, 0, 0, 0, 16, 2, 0, 0, 17, 2, 0, 0, 10, 12, 84, 104, 117, 110, 100, 101, 114, 83, 116, 111, 114, 109, 11, 5, 72, 121, 100, 114, 97, 74, 58, 7, 3, 0, 150, 27, 8, 3, 0, 1, 224, 9, 3, 0, 1, 224, 101, 27, 26, 129, 235, 62, 19, 4, 0, 0, 0, 149, 20, 4, 0, 0, 1, 82, 102, 9, 2, 7, 85, 78, 75, 78, 79, 87, 78, 3, 12, 84, 104, 117, 110, 100, 101, 114, 83, 116, 111, 114, 109, 73, 11, 11, 9, 1, 1, 65, 2, 4, 0, 0, 0, 1, 1, 2, 223, 125,
	// }

	klvPacket := BuildDynamicKLV()

	fmt.Println("klvPacket:", klvPacket)
	res, err := betterklv.HandleKLVBuffer(klvPacket)
	if err != nil {
		fmt.Println("Error parsing KLV:", err)
		return
	}
	fmt.Println("Decoded KLV buffer:", res)
	fmt.Printf("KLV packet length: %d bytes\n", len(klvPacket))

	for {
		n, err := io.ReadFull(tsFile, packetBuf)
		if err == io.EOF || err == io.ErrUnexpectedEOF {
			fmt.Println("📁 End of TS file reached. Stopping.")
			break
		} else if err != nil {
			fmt.Println("❌ Error reading TS file:", err)
			break
		}

		mux.HandlePSI(packetBuf, 0, 0)
		_, err = conn.Write(packetBuf)
		if err != nil {
			fmt.Println("Failed to send TS packet:", err)
			break
		}

		tsPacketCount++
		currentOffset += int64(n)

		fmt.Printf("🎥 Sent TS video packet #%d (%d/%d bytes)\n",
			tsPacketCount, currentOffset, totalSize)

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

func SendKLV(conn net.Conn, klvPacket []byte, pos *int, muxedKlv []byte, pid uint8, serviceID int) error {
	const tsSize = 188

	ok := mux.BuildMetadataPes(klvPacket, len(klvPacket), muxedKlv, pid, serviceID, pos, nil)
	if ok <= 0 {
		fmt.Printf("❌ BuildMetadataPes failed at once, pos: %d, size: %d\n", *pos, len(klvPacket))
		return fmt.Errorf("BuildMetadataPes failed")
	}

	// כעת שולחים את כל ה-TS packets שהפונקציה יצרה במקביל, לפי מספר חבילות:
	packetsCount := (*pos + tsSize - 1) / tsSize // כמות חבילות ts שהתקבלו

	for i := 0; i < packetsCount; i++ {
		start := i * tsSize
		end := start + tsSize
		if end > len(muxedKlv) {
			end = len(muxedKlv)
		}
		_, err := conn.Write(muxedKlv[start:end])
		if err != nil {
			return fmt.Errorf("failed to send TS packet: %w", err)
		}
		fmt.Printf("📤 Sent KLV TS packet # %d, current pos is %d/%d\n", i, *pos, len(klvPacket))
		time.Sleep(5 * time.Millisecond)
	}

	fmt.Println("✅ Finished sending full KLV payload at once.")
	return nil
}

func BuildDynamicKLV() []byte {
	// Universal Key תקני - STANAG 4609 MISB 0601
	universalKey := []byte{
		0x06, 0x0E, 0x2B, 0x34,
		0x02, 0x0B, 0x01, 0x01,
		0x0E, 0x01, 0x03, 0x01,
		0x01, 0x00, 0x00, 0x00,
	}

	startTime := uint64(time.Now().Unix())
	endTime := startTime + 60 // דקה
	duration := float64(60.0)
	serialNumber := "ABC12345678901234567890" // ארוך יותר

	// נבנה local set מורחב עם שדות נוספים
	var localSet []byte

	// פונקציה עזר: Tag + Length + Value
	encodeField := func(tag byte, value []byte) []byte {
		encoded := []byte{tag}
		// length קצר (בהנחה שעד 127, אחרת צריך BER מורכב)
		encoded = append(encoded, byte(len(value)))
		encoded = append(encoded, value...)
		return encoded
	}

	// הוספת שדות
	localSet = append(localSet, encodeField(0x48, EncodeUint64Value(startTime))...)                      // Start Time
	localSet = append(localSet, encodeField(0x02, EncodeUint64Value(endTime))...)                        // End Time
	localSet = append(localSet, encodeField(0x0C, []byte(strconv.FormatFloat(duration, 'f', 2, 64)))...) // Duration ASCII
	localSet = append(localSet, encodeField(0x0A, []byte(serialNumber))...)                              // Serial Number

	// Encode BER Length של כל ה־localSet
	lengthBytes := EncodeBERLength(len(localSet))

	// Build final packet
	klvPacket := []byte{}
	klvPacket = append(klvPacket, universalKey...)
	klvPacket = append(klvPacket, lengthBytes...)
	klvPacket = append(klvPacket, localSet...)

	return klvPacket
}
