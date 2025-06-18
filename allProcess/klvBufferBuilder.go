package main

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"fmt"
	"math"
	"os/exec"
	"strconv"
)

func EncodePTS(pts uint64) []byte {
	if pts > (1 << 33) {
		pts %= (1 << 33) // Wrap around at 33 bits
	}

	b := make([]byte, 5)

	// Set the '0010' prefix for PTS (PTS_DTS_flags = '10')
	b[0] = byte(0x20 | (((pts >> 30) & 0x07) << 1) | 0x01)       
	b[1] = byte((pts >> 22) & 0xFF)
	b[2] = byte((((pts >> 15) & 0x7F) << 1) | 0x01)              
	b[3] = byte((pts >> 7) & 0xFF)
	b[4] = byte(((pts & 0x7F) << 1) | 0x01)                      

	return b
}

type FFProbeFormat struct {
	Duration string `json:"duration"`
}

type FFProbeOutput struct {
	Format FFProbeFormat `json:"format"`
}

func GetVideoDuration(path string) (float64, error) {
	fmt.Println("Running ffprobe on file:", path)
	cmd := exec.Command("ffprobe",
		"-v", "quiet",
		"-print_format", "json",
		"-show_format",
		path,
	)

	var out bytes.Buffer
	var stderr bytes.Buffer
	cmd.Stdout = &out
	cmd.Stderr = &stderr

	err := cmd.Run()
    if err != nil {
        return 0, fmt.Errorf("ffprobe failed: %v\nstderr: %s", err, stderr.String())
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

// ---------- Encoder Helpers ----------
func EncodeUint64(tag byte, value uint64) []byte {
	buf := make([]byte, 10)
	buf[0] = tag
	buf[1] = 8
	binary.BigEndian.PutUint64(buf[2:], value)
	return buf
}

func EncodeFloat64(tag byte, value float64) []byte {
	buf := make([]byte, 10)
	buf[0] = tag
	buf[1] = 8
	binary.BigEndian.PutUint64(buf[2:], math.Float64bits(value))
	return buf
}

func EncodeString(tag byte, value string) []byte {
	data := []byte(value)
	buf := make([]byte, 2+len(data))
	buf[0] = tag
	buf[1] = byte(len(data))
	copy(buf[2:], data)
	return buf
}

// Wrap in Local Set (i.e., 0x06 for VMTI)
func WrapInLocalSet(setTag byte, inner []byte) []byte {
	buf := make([]byte, 2+len(inner))
	buf[0] = setTag
	buf[1] = byte(len(inner))
	copy(buf[2:], inner)
	return buf
}

// Wrap in full KLV (Universal Key + BER Length + Local Set)
func WrapInKLV(klvData []byte) []byte {
	universalKey := []byte{
		0x06, 0x0E, 0x2B, 0x34,
		0x02, 0x0B, 0x01, 0x01,
		0x0E, 0x01, 0x03, 0x01,
		0x01, 0x00, 0x00, 0x00, // MISB 0601 UID
	}
	berLength := EncodeBERLength(len(klvData))
	return append(append(universalKey, berLength...), klvData...)
}

func EncodeBERLength(length int) []byte {
	if length < 0x80 {
		return []byte{byte(length)}
	}
	// תומך ב-length קטן, אפשר להרחיב לגדולים יותר אם צריך
	return []byte{0x81, byte(length)}
}


func EncodeLocalSetField(tag byte, value []byte) []byte {
	length := len(value)
	buf := []byte{tag}
	buf = append(buf, EncodeBERLength(length)...)
	buf = append(buf, value...)
	return buf
}

func EncodeUint64Value(val uint64) []byte {
	b := make([]byte, 8)
	for i := uint(0); i < 8; i++ {
		b[7-i] = byte(val >> (i * 8))
	}
	return b
}

var UniversalKey0601 = []byte{
	0x06, 0x0E, 0x2B, 0x34,
	0x02, 0x0B, 0x01, 0x01,
	0x0E, 0x01, 0x03, 0x01,
	0x01, 0x00, 0x00, 0x00,
}

