package main

import (
	"fmt"
	"strconv"

	//"os"
	//"os/exec"
	betterklv "github.com/hadar225/AWS_hadar/betterklv"
)

func main() {
	//levelA - convert mp4 to mp2ts - completed
	// inputFile := "./input.mp4"
	outputFile := "./output.ts"
	// cmd := exec.Command("ffmpeg", "-i", inputFile, "-c:v", "mpeg2video", "-c:a", "mp2", outputFile)
	// // Set command output to the terminal
	// cmd.Stdout = os.Stdout
	// cmd.Stderr = os.Stderr
	// fmt.Println("Converting MP4 to MPEG-TS...")
	// if err := cmd.Run(); err != nil {
	// 	fmt.Printf("Error during conversion: %v\n", err)
	// 	return}
	// fmt.Printf("Conversion complete! Output file: %s\n", outputFile)

	//levelB - build KLV for muxer
	//levelB.1 - receive KLV - completed
	duration, err := GetVideoDuration(outputFile)
	if err != nil {
		fmt.Println("Error getting duration:", err)
		return
	}
	startTime := uint64(1620000000)         // e.g. 2021-05-03 00:00:00 UTC
	endTime := startTime + uint64(duration) // endTime = startTime + duration in seconds
	serialNumber := "ABC123456"

	//levelB.2 - buildKLVDataBuffer
	// Field Encoding
	klvPayload := []byte{}
	klvPayload = append(klvPayload, EncodeUint64(72, startTime)...)
	klvPayload = append(klvPayload, EncodeUint64(2, endTime)...)
	klvPayload = append(klvPayload, EncodeString(12, strconv.FormatFloat(duration, 'f', 2, 64))...)
	klvPayload = append(klvPayload, EncodeString(10, serialNumber)...)

	// Wrap as full KLV
	klvBuffer := WrapInKLV(klvPayload)

	fmt.Println("Encoded KLV buffer:", klvBuffer)

	// Parse
	res, err := betterklv.HandleKLVBuffer(klvBuffer)
	if err != nil {
		fmt.Println("Error parsing KLV:", err)
		return
	}

	fmt.Println("result:", res)

	// Print result
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
}
