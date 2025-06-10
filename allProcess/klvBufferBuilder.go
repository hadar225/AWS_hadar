package main

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"os/exec"
	"strconv"
)

type FFProbeFormat struct {
	Duration string `json:"duration"`
}

type FFProbeOutput struct {
	Format FFProbeFormat `json:"format"`
}

func GetVideoDuration(path string) (float64, error) {
	cmd := exec.Command("ffprobe",
		"-v", "quiet",
		"-print_format", "json",
		"-show_format",
		path,
	)

	var out bytes.Buffer
	cmd.Stdout = &out

	if err := cmd.Run(); err != nil {
		return 0, err
	}

	var ffprobeData FFProbeOutput
	if err := json.Unmarshal(out.Bytes(), &ffprobeData); err != nil {
		return 0, err
	}

	// Duration is a string, convert to float64 seconds
	duration, err := strconv.ParseFloat(ffprobeData.Format.Duration, 64)
	if err != nil {
		return 0, err
	}

	return duration, nil
}

type KLVData struct {
	StartTime    uint64
	EndTime      uint64
	Duration     float64
	SerialNumber string
}

// Helper to encode a length in BER format (simplified).
func encodeBERLength(length int) []byte {
	if length < 128 {
		return []byte{byte(length)}
	}
	// Multi-byte length encoding
	buf := []byte{}
	for length > 0 {
		buf = append([]byte{byte(length & 0xFF)}, buf...)
		length >>= 8
	}
	return append([]byte{byte(len(buf) | 0x80)}, buf...)
}

func BuildKLVDataBuffer(data KLVData) []byte {
	var buffer bytes.Buffer

	// Example keys
	const (
		KEY_START_TIME    = 0x01
		KEY_END_TIME      = 0x02
		KEY_LENGTH        = 0x03
		KEY_SERIAL_NUMBER = 0x04
	)

	// Start Time
	buffer.WriteByte(KEY_START_TIME) // Key
	startTimeBytes := make([]byte, 8)
	binary.BigEndian.PutUint64(startTimeBytes, data.StartTime) // Value
	buffer.Write(encodeBERLength(len(startTimeBytes)))         // Length
	buffer.Write(startTimeBytes)                               // Value

	// End Time
	buffer.WriteByte(KEY_END_TIME) // Key
	endTimeBytes := make([]byte, 8)
	binary.BigEndian.PutUint64(endTimeBytes, data.EndTime) // Value
	buffer.Write(encodeBERLength(len(endTimeBytes)))       // Length
	buffer.Write(endTimeBytes)                             // Value

	// Length
	buffer.WriteByte(KEY_LENGTH) // Key
	lengthBytes := make([]byte, 4)
	binary.BigEndian.PutUint32(lengthBytes, uint32(data.Duration)) // Value
	buffer.Write(encodeBERLength(len(lengthBytes)))      // Length
	buffer.Write(lengthBytes)                            // Value

	// Serial Number
	buffer.WriteByte(KEY_SERIAL_NUMBER)             // Key
	serialBytes := []byte(data.SerialNumber)        // Value
	buffer.Write(encodeBERLength(len(serialBytes))) // Length
	buffer.Write(serialBytes)                       // Value

	return buffer.Bytes()
}
