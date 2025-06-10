package betterklv

import (
	"encoding/binary"
	"errors"
	"fmt"
	"math"
	"reflect"
	"strconv"
)

func DecodeBERTag(bytes []byte, index uint64) (uint64, int, error) {
	//this decodes the value of a BER tag. Check Metadata protocol for details
	if index > uint64(len(bytes)) {
		return 0, 0, fmt.Errorf("DecodeBERTag: index %v is out of range %v", index, len(bytes)-1)
	}

	var value int = 0
	for {
		value = value << 7
		value += int(bytes[index] & 0x7F)
		index++
		if index >= uint64(len(bytes)) || (bytes[index-1]&0x80) == 0 {
			break
		}
	}
	return index, value, nil
}

func DecodeBERLength(bytes []byte, index uint64) (uint64, uint64, error) {
	//this decodes the length of a BER packet. Check Metadata protocol for details
	if index > uint64(len(bytes)) {
		return 0, 0, fmt.Errorf("DecodeBERLength: index %v is out of range %v", index, len(bytes)-1)
	}

	var payloadSize uint64
	if (bytes[index] & 0x80) == 0 { // Short length
		payloadSize = uint64(bytes[index])
		index += 1
	} else {
		lengthSize := uint64(bytes[index] & 0x7f)

		if index+lengthSize > uint64(len(bytes))-1 {
			return 0, 0, fmt.Errorf("DecodeBERLength: 'length' extends beyond byte slice range (%v + %v > %v)", index, lengthSize, len(bytes)-1)
		}

		// unit64 is 8 bytes so we can't have a bigger length size
		// if we would need to support bigger size we would need to make a dynamic sized number based on a slice of bytes and to also rework 'index' which also uses uint64
		if lengthSize > 8 {
			return 0, 0, fmt.Errorf("DecodeBERLength: 'length' size is bigger than maximum of 8")
		}

		index += 1
		for i := uint64(0); i < lengthSize; i++ {
			payloadSize = (payloadSize << 8) + uint64(bytes[index]) // check if its ok the connect two bytes as two integers
			index = index + 1
		}
	}

	if payloadSize == 0 {
		return 0, 0, fmt.Errorf("DecodeBERLength: decoded payload size is 0")
	}

	if index+payloadSize > uint64(len(bytes)) {
		return index, payloadSize, fmt.Errorf("DecodeBERLength: decoded payload size %v is bigger than the remaining space (%v - %v = %v)", payloadSize, uint64(len(bytes)), index, uint64(len(bytes))-index)
	}

	return index, payloadSize, nil
}

// checksum will be calculated for the entire klv buffer: 16 byte header, total length, all the tags, checksum tag, checksum length
func CalculateChecksum(bytes []byte) uint16 {
	chksm := uint16(0)
	for i, b := range bytes {
		chksm += uint16(b) << (8 * (i + 1) % 2)
	}
	return chksm
}

func EncodeTagKey(tagValue int) ([]byte, error) {
	prepend := func(bs []byte, b byte) []byte {
		return append([]byte{b}, bs...)
	}

	if tagValue > MAX_TAG_VALUE || tagValue <= 0 {
		return nil, errors.New("tag value invalid. valid range is 1..(2^14)-1")
	}

	result := make([]byte, binary.MaxVarintLen64)

	uTagValue := uint64(tagValue)
	numOfBytes := 0
	for uTagValue > 0 {
		numOfBytes++
		result = prepend(result, byte(uTagValue<<1)/2) // prepend 7 bits at  a time
		if numOfBytes > 1 {
			result[0] |= 0x80 // mark inserted byte for extention
		}
		uTagValue >>= 7
	}

	return result[0:numOfBytes], nil
}

func EncodeSetLength(length int) ([]byte, error) {
	if length > math.MaxUint16 || length <= 0 {
		return nil, errors.New("set value invalid. valid range is 1..(2^14)-1")
	}

	if length <= LONG_LENGTH_THRESHOLD {
		return []byte{byte(length)}, nil //return as is
	}

	//long length
	bytesRequired := 1 + uint64(length/256)
	result := make([]byte, 1)
	result[0] = byte(bytesRequired) | 0x80 //setting msb to 1
	if bytesRequired == 2 {
		result = binary.BigEndian.AppendUint16(result, uint16(length))
	} else if bytesRequired == 1 {
		result = append(result, byte(length))
	} else {
		return nil, errors.New("set value invalid. valid range is 1..(2^14)-1")
	}
	return result, nil
}

func BuildKLVTag(key int, value []byte) ([]byte, error) {
	if value == nil {
		return nil, errors.New("nil tag value received")
	}
	length := len(value)
	if length == 0 {
		return nil, errors.New("empty tag value received for tag " + strconv.Itoa(key))
	}
	//encode length tag
	lengthArray, err := EncodeSetLength(length)
	if err != nil {
		return nil, errors.New("failed to encode tag length")
	}
	//encode key tag
	keyArray, err := EncodeTagKey(key)
	if err != nil {
		return nil, errors.New("failed to encode tag key")
	}
	//build and return
	result := append(keyArray, lengthArray...)
	result = append(result, value...)
	return result, nil
}

func DecodeNumber(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	var value uint64
	var result float64
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeNumber: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	for i := uint64(0); i < size; i++ {
		value = (value << 8) + uint64(bytes[startIndex+i])
	}
	result = float64(value)
	signedFlag := 0
	if props.Signed {
		signedFlag = 1
		if (1<<(size*8-1))&value != 0 {
			// 2s complement
			mask := uint64(0xFFFFFFFFFFFFFFFF << (size * 8))
			value = (^value & ^mask) + 1
			result = -1.0 * float64(value)
		}
	}
	// normalize value to 1
	if props.Scale != 0 {
		result /= math.Pow(2, float64(size*8-uint64(signedFlag))) - 1
		result *= props.Scale
	}
	result += props.Offset
	return result, nil
}

func DecodeString(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeString: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	return string(bytes[startIndex : startIndex+size]), nil
}

func DecodeBytes(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeBytes: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	return bytes[startIndex : startIndex+size], nil
}

func DecodeKLVSet(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeKLVSet: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	result := make(map[string]interface{})
	i := startIndex
	for i < startIndex+size && i < uint64(len(bytes)) {
		var key int
		var tagLength uint64
		var err error
		i, key, err = DecodeBERTag(bytes, i)
		if err != nil {
			return result, fmt.Errorf("DecodeKLVSet: error while decoding BER tag %w", err)
		}

		i, tagLength, err = DecodeBERLength(bytes, i)
		if err != nil {
			return result, fmt.Errorf("DecodeKLVSet: error while decoding BER tag %w", err)
		}

		tag, ok := props.KLVSet[key]
		if ok && ((tag.Properties.Size > 0 && tagLength == tag.Properties.Size) || tag.Properties.Size == 0) {
			value, err := tag.Decode(bytes, i, tagLength, tag.Properties)
			if err != nil {
				return result, fmt.Errorf("DecoderKLVSet: error while decoding tak with key %v at index %v with length %v", key, i, tagLength)
			}
			result[tag.Name] = value
		}
		i += tagLength
	}
	return result, nil
}

func DecodeFlagSet(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeFlagSet: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	result := make(map[string]bool)

	var value uint64
	for i := uint64(0); i < size; i++ {
		value = (value << 8) + uint64(bytes[startIndex+i])
	}

	sizeInBits := size * 8
	for i := uint64(0); i < sizeInBits && i < uint64(len(props.FlagSet)); i++ {
		result[props.FlagSet[i]] = (value >> (sizeInBits - i - 1) & 1) == 0
	}

	return result, nil
}

func DecodeVtargets(bytes []byte, startIndex uint64, size uint64, props DecodeProperties) (interface{}, error) {
	if startIndex+size > uint64(len(bytes)) {
		return nil, fmt.Errorf("DecodeVtargets: out of range, startIndex %v + size %v > len(bytes) %v", startIndex, size, len(bytes))
	}

	res := make(map[string]interface{})
	i := startIndex
	for i < startIndex+size && i < uint64(len(bytes)) {
		//decode id as tag
		var err error
		var targetLength uint64
		i, targetLength, err = DecodeBERLength(bytes, i)
		if err != nil {
			return nil, fmt.Errorf("DecodeVtargets: error while decoding length: %w", err)
		}

		newi, id, err := DecodeBERTag(bytes, i)
		if err != nil {
			return nil, fmt.Errorf("DecodeVtargets: error while decoding tag: %w", err)
		}

		idLen := newi - i
		i = newi
		res[strconv.Itoa(id)], err = DecodeKLVSet(bytes, i, targetLength-idLen, DecodeProperties{KLVSet: VTARGETSet})
		if err != nil {
			return nil, fmt.Errorf("DecodeVtargets: error while decoding klv set: %w", err)
		}

		i += targetLength
	}
	return res, nil
}

// defined in "Remote Video Terminal Metadata Set"
func HandleUserDefinedSet(bytes []byte, startIndex uint64, size uint64, systemName string) (interface{}, error) {
	i := startIndex
	res := make(map[string]interface{})
	for i < size && i < uint64(len(bytes)) {
		var err error
		var numericIdForDataTypeTag int
		i, numericIdForDataTypeTag, err = DecodeBERTag(bytes, i)
		if err != nil {
			return res, fmt.Errorf("HandleUserDefinedSet: error while decoding 'numericIdForDataTypeTag' tag")
		}
		if numericIdForDataTypeTag != 1 {
			return res, fmt.Errorf("HandleUserDefinedSet: 'numericIdForDataTypeTag' tag must have value of 1")
		}

		var numericIdForDataTypeLength uint64
		i, numericIdForDataTypeLength, err = DecodeBERLength(bytes, i)
		if err != nil {
			return res, fmt.Errorf("HandleUserDefinedSet: error while decoding length of 'numericIdForDataType'")
		}
		if numericIdForDataTypeLength != 1 {
			return res, fmt.Errorf("HandleUserDefinedSet: 'numericIdForDataType' length must be 1")
		}

		numericIdForDataTypeInterface, err := DecodeNumber(bytes, i, numericIdForDataTypeLength, DecodeProperties{Size: 1, Signed: false})
		if err != nil {
			return res, fmt.Errorf("HandleUserDefinedSet: error while decoding key")
		}
		numericIdForDataType := numericIdForDataTypeInterface.(float64)

		i += numericIdForDataTypeLength

		var userDataTag int
		i, userDataTag, err = DecodeBERTag(bytes, i)
		if err != nil {
			return res, fmt.Errorf("HandleUserDefinedSet: error while decoding 'userDataTag' tag")
		}
		if userDataTag != 2 {
			return res, fmt.Errorf("HandleUserDefinedSet: 'userDataTag' length must be 2")
		}

		var userDataLength uint64
		i, userDataLength, err = DecodeBERLength(bytes, i)
		if err != nil {
			return res, fmt.Errorf("HandleUserDefinedSet: error while decoding 'userDataLength' tag")
		}
		if userDataLength != 4 {
			return res, fmt.Errorf("HandleUserDefinedSet: 'userData' length must be 4")
		}

		tag, ok := UserDefinedSets[systemName][int(numericIdForDataType)]
		if ok {
			value, err := tag.Decode(bytes, i, userDataLength, tag.Properties)
			if err != nil {
				return res, fmt.Errorf("HandleUserDefinedSet: error while decoding tag: %w", err)
			}
			res[tag.Name] = value
		}
		i += userDataLength
	}
	return res, nil
}

func HandleKLVBuffer(bytes []byte) (map[string]interface{}, error) {
	if !reflect.DeepEqual(bytes[:16], HEADER) {
		return nil, fmt.Errorf("HandleKLVBuffer: header %v is not a valid header", bytes[:16])
	}

	i, length, err := DecodeBERLength(bytes, 16)
	if err != nil {
		return nil, fmt.Errorf("HandleKLVBuffer: error when decoding length: %w", err)
	}

	res, err := DecodeKLVSet(bytes, i, length, DecodeProperties{KLVSet: MISB0601Set})
	if err != nil {
		return nil, fmt.Errorf("HandleKLVBuffer: error when decoding klv set: %w", err)
	}

	VMTIDataSet, VMTIPresent := res.(map[string]interface{})[VMTI_DATA_SET]
	RVTDataSet, RVTPresent := res.(map[string]interface{})[RVT_LOCAL_SET]
	if VMTIPresent && RVTPresent {
		SystemName, SystemNamePresent := VMTIDataSet.(map[string]interface{})[VMTI_SYSTEM_NAME]
		UserDefinedData, UserDefinedPresent := RVTDataSet.(map[string]interface{})[USER_DEFINED_LS]
		if SystemNamePresent && UserDefinedPresent {
			// handle user defined
			_, SystemNameUserDefinedPresent := UserDefinedSets[SystemName.(string)]
			if SystemNameUserDefinedPresent {
				res.(map[string]interface{})[USER_DEFINED_LS], err = HandleUserDefinedSet(UserDefinedData.([]byte), 0, uint64(len(UserDefinedData.([]byte))), SystemName.(string))
				if err != nil {
					return nil, fmt.Errorf("HandleKLVBuffer: error when handling user defined set: %w", err)
				}
				delete(res.(map[string]interface{})[RVT_LOCAL_SET].(map[string]interface{}), USER_DEFINED_LS)
			}
		}
	}
	return res.(map[string]interface{}), nil
}

// func main() {
// 	test := []byte{
// 		6, 14, 43, 52, 2, 11, 1, 1, 14, 1, 3, 1, 1, 0, 0, 0, 129, 225, 2, 8, 0, 6, 17, 252, 199, 234, 0, 113, 4, 5, 49, 55, 52, 53, 52, 14, 4, 24, 123, 183, 68, 13, 4, 44, 201, 206, 182, 15, 2, 29, 128, 3, 2, 49, 53, 23, 4, 44, 205, 127, 157, 24, 4, 24, 123, 75, 34, 25, 2, 12, 43, 82, 4, 44, 205, 171, 219, 83, 4, 24, 123, 48, 142, 84, 4, 44, 205, 184, 151, 85, 4, 24, 123, 91, 53, 86, 4, 44, 205, 86, 222, 87, 4, 24, 123, 101, 142, 88, 4, 44, 205, 75, 105, 89, 4, 24, 123, 59, 137, 18, 4, 4, 212, 92, 229, 19, 4, 0, 0, 0, 0, 20, 4, 0, 0, 0, 0, 16, 2, 0, 0, 17, 2, 0, 0, 10, 12, 84, 104, 117, 110, 100, 101, 114, 83, 116, 111, 114, 109, 11, 5, 72, 121, 100, 114, 97, 74, 58, 7, 3, 0, 150, 27, 8, 3, 0, 1, 224, 9, 3, 0, 1, 224, 101, 27, 26, 129, 235, 62, 19, 4, 0, 0, 0, 149, 20, 4, 0, 0, 1, 82, 102, 9, 2, 7, 85, 78, 75, 78, 79, 87, 78, 3, 12, 84, 104, 117, 110, 100, 101, 114, 83, 116, 111, 114, 109, 73, 11, 11, 9, 1, 1, 65, 2, 4, 0, 0, 0, 1, 1, 2, 223, 125}
// 	res,_ := HandleKLVBuffer(test)
// 	for k, v := range res {
// 		if k == VMTI_DATA_SET {
// 			fmt.Println(k, ":")
// 			for k2, v2 := range v.(map[string]interface{}) {
// 				fmt.Println(" ", k2, ":", v2)
// 			}
// 		} else {
// 			fmt.Println(k, ":", v)
// 		}
// 	}
// }
