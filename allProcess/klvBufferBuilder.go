package main

import (
	"bytes"
	"encoding/binary"
	"encoding/json"
	"math"
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

// Create BER length field
func EncodeBERLength(length int) []byte {
	if length < 0x80 {
		return []byte{byte(length)}
	}
	// Long-form BER (e.g. 0x81 xx)
	result := []byte{}
	tmp := length
	for tmp > 0 {
		result = append([]byte{byte(tmp & 0xFF)}, result...)
		tmp >>= 8
	}
	result = append([]byte{0x80 | byte(len(result))}, result...)
	return result
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
