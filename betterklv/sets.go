package betterklv

var HEADER = []byte{6, 14, 43, 52, 2, 11, 1, 1, 14, 1, 3, 1, 1, 0, 0, 0}

const MAX_TAG_VALUE = 1000
const LONG_LENGTH_THRESHOLD = 127
const QUARTER_CIRCLE = 90
const HALF_CIRCLE = 180
const FULL_CIRCLE = 360
const FRAME_CENTER_MAX_LONGITUDE = 180
const FRAME_CENTER_MAX_LATITUDE = 90
const MAX_HEIGHT = 19900
const ALTITUDE_DEC_VALUE = 900
const PLATFORM_PITCH_MAX_ANGLE = 20
const PLATFORM_ROLL_MAX_ANGLE = 50
const COORDINATE_OFFSET_MAX_VALUE = 19.2
const MAX_SLANT_RANGE = 5000000
const MAX_TARGET_WIDTH = 10000
const MAX_CORNER_OFFSET_RANGE = 0.075
const MAX_WIND_SPEED = 100
const MAX_PRESSURE = 5000
const MAX_NORTH_VELOCITY = 327
const THUNDERSTORM_USER_DEFINED_SET = "ThunderStorm"
const ANANAS_INVESTIGATOR_USER_DEFINED_SET = "Investigator"

type Tag struct {
	Name       string
	Decode     func([]byte, uint64, uint64, DecodeProperties) (interface{}, error)
	Properties DecodeProperties
}

type DecodeProperties struct {
	Size    uint64
	Signed  bool
	Scale   float64
	Offset  float64
	KLVSet  map[int]Tag
	FlagSet []string
}

var MISB0601Set map[int]Tag = map[int]Tag{
	1:  {Name: CHECKSUM, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	2:  {Name: UNIX_TIMESTAMP, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false, Scale: 0, Offset: 0}},
	3:  {Name: MISSION_ID, Decode: DecodeString, Properties: DecodeProperties{}},
	4:  {Name: PLATFORM_TAIL_NUMBER, Decode: DecodeString, Properties: DecodeProperties{}},
	5:  {Name: PLATFORM_HEADING_ANGLE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: FULL_CIRCLE, Offset: 0}},
	6:  {Name: PLATFORM_PITCH_ANGLE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: PLATFORM_PITCH_MAX_ANGLE, Offset: 0}},
	7:  {Name: PLATFORM_ROLL_ANGLE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: PLATFORM_ROLL_MAX_ANGLE, Offset: 0}},
	8:  {Name: PLATFORM_TRUE_AIRSPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	9:  {Name: PLATFORM_INDICATED_AIRSPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	10: {Name: PLATFORM_DESIGNATION, Decode: DecodeString, Properties: DecodeProperties{}},
	11: {Name: IMAGE_SOURCE_SENSOR, Decode: DecodeString, Properties: DecodeProperties{}},
	12: {Name: IMAGE_COORDINATE_SYSTEM, Decode: DecodeString, Properties: DecodeProperties{}},
	13: {Name: SENSOR_LATITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE, Offset: 0}},
	14: {Name: SENSOR_LONGITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE, Offset: 0}},
	15: {Name: SENSOR_TRUE_ALTITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	16: {Name: SENSOR_HORIZONTAL_FOV, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: HALF_CIRCLE, Offset: 0}},
	17: {Name: SENSOR_VERTICAL_FOV, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: HALF_CIRCLE, Offset: 0}},
	18: {Name: SENSOR_RELATIVE_AZIMUTH, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: FULL_CIRCLE, Offset: 0}},
	19: {Name: SENSOR_RELATIVE_ELEVATION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: HALF_CIRCLE, Offset: 0}},
	20: {Name: SENSOR_RELATIVE_ROLL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: FULL_CIRCLE, Offset: 0}},
	21: {Name: SLANT_RANGE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: MAX_SLANT_RANGE, Offset: 0}},
	22: {Name: TARGET_WIDTH, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_TARGET_WIDTH, Offset: 0}},
	23: {Name: FRAME_CENTER_LATITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE, Offset: 0}},
	24: {Name: FRAME_CENTER_LONGITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE, Offset: 0}},
	25: {Name: FRAME_CENTER_ELEVATION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	26: {Name: OFFSET_CORNER_LATITUDE_1, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	27: {Name: OFFSET_CORNER_LONGITUDE_1, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	28: {Name: OFFSET_CORNER_LATITUDE_2, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	29: {Name: OFFSET_CORNER_LONGITUDE_2, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	30: {Name: OFFSET_CORNER_LATITUDE_3, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	31: {Name: OFFSET_CORNER_LONGITUDE_3, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	32: {Name: OFFSET_CORNER_LATITUDE_4, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	33: {Name: OFFSET_CORNER_LONGITUDE_4, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_CORNER_OFFSET_RANGE, Offset: 0}},
	34: {Name: ICING_DETECTED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	35: {Name: WIND_DIRECTION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: FULL_CIRCLE}},
	36: {Name: WIND_SPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: MAX_WIND_SPEED}},
	37: {Name: STATIC_PRESSURE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_PRESSURE}},
	38: {Name: DENSITY_ALTITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	39: {Name: OUTSIDE_AIR_TEMPERATURE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true}},
	40: {Name: TARGET_LOCATION_LATITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	41: {Name: TARGET_LOCATION_LONGITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	42: {Name: TARGET_LOCATION_ELEVATION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	43: {Name: TARGET_TRACK_GATE_WIDTH, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	44: {Name: TARGET_TRACK_GATE_HEIGHT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	45: {Name: TARGET_ERROR_ESTIMATE_CE90, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false}},
	46: {Name: TARGET_ERROR_ESTIMATE_LE90, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false}},
	47: {Name: GENERIC_FLAG_DATA, Decode: DecodeFlagSet, Properties: DecodeProperties{Size: 1, FlagSet: GenericFlagSet}},
	//48: security metadata set - not implemented for now
	49: {Name: DIFFERENTIAL_PRESSURE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_PRESSURE}},
	50: {Name: PLATFORM_ANGLE_OF_ATTACK, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: PLATFORM_PITCH_MAX_ANGLE}},
	51: {Name: PLATFORM_VERTICAL_SPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: HALF_CIRCLE}},
	52: {Name: PLATFORM_SIDESLIP_ANGLE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: PLATFORM_PITCH_MAX_ANGLE}},
	53: {Name: AIRFIELD_BAROMETRIC_PRESSURE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_PRESSURE}},
	54: {Name: AIRFIELD_ELEVATION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	55: {Name: RELATIVE_HUMIDITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: MAX_WIND_SPEED}},
	56: {Name: PLATFORM_GROUND_SPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	57: {Name: GROUD_RANGE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: MAX_SLANT_RANGE}},
	58: {Name: PLATFORM_FUEL_REMAINING, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_TARGET_WIDTH}},
	59: {Name: PLATFORM_CALL_SIGN, Decode: DecodeString, Properties: DecodeProperties{}},
	60: {Name: WEAPON_LOAD, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false}},
	61: {Name: WEAPON_FIRED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	62: {Name: LASER_PRF_CODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false}},
	63: {Name: SENSOR_FOV_NAME, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	64: {Name: PLATFORM_MAGNETIC_HEADING, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: FULL_CIRCLE}},
	65: {Name: UAS_DATALINK_LS_VERSION_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	//66: TARGET_LOCATION_COVARIANCE_MATRIX, details TBD, not implementing for now
	67: {Name: ALTERNATE_PLATFORM_LATITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	68: {Name: ALTERNATE_PLATFORM_LONGITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	69: {Name: ALTERNATE_PLATFORM_ALTITUDE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	70: {Name: ALTERNATE_PLATFORM_NAME, Decode: DecodeString, Properties: DecodeProperties{}},
	71: {Name: ALTERNATE_PLATFORM_HEADING, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: FULL_CIRCLE}},
	72: {Name: EVENT_START_TIME_UTC, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false}},
	73: {Name: RVT_LOCAL_SET, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: MISB0806Set}},
	74: {Name: VMTI_DATA_SET, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: MISB0903Set}},
	75: {Name: SENSOR_ELLIPSOID_HEIGHT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	76: {Name: ALTERNATE_PLATFORM_ELLIPSOID_HEIGHT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	77: {Name: OPERATIONAL_MODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false}},
	78: {Name: FRAME_CENTER_HEIGHT_ABOVE_ELLIPSOID, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	79: {Name: SENSOR_NORTH_VELOCITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_NORTH_VELOCITY}},
	80: {Name: SENSOR_EAST_VELOCITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: true, Scale: MAX_NORTH_VELOCITY}},
	//81: IMAGE_HORIZON_PIXEL_PACK - not implementing for now
	82: {Name: CORNER_LATITUDE_POINT_1_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	83: {Name: CORNER_LONGITUDE_POINT_1_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	84: {Name: CORNER_LATITUDE_POINT_2_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	85: {Name: CORNER_LONGITUDE_POINT_2_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	86: {Name: CORNER_LATITUDE_POINT_3_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	87: {Name: CORNER_LONGITUDE_POINT_3_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	88: {Name: CORNER_LATITUDE_POINT_4_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LATITUDE}},
	89: {Name: CORNER_LONGITUDE_POINT_4_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FRAME_CENTER_MAX_LONGITUDE}},
	90: {Name: PLATFORM_PITCH_ANGLE_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: QUARTER_CIRCLE}},
	91: {Name: PLATFORM_ROLL_ANGLE_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: QUARTER_CIRCLE}},
	92: {Name: PLATFORM_ANGLE_OF_ATTACK_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: QUARTER_CIRCLE}},
	93: {Name: PLATFORM_SIDESLIP_ANGLE_FULL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: QUARTER_CIRCLE}},
	//94: MIIS_CORE_IDENTIFIER is a struct, not gonna implement
	//95: SAR_MOTION_IMAGERY_METADATA is a struct, not gonna implement
}

var UserDefinedSets map[string]map[int]Tag = map[string]map[int]Tag{
	THUNDERSTORM_USER_DEFINED_SET: {
		65: {Name: WINDOW_ID, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		66: {Name: SUB_FRAME_ID, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		67: {Name: ZOOM_OUT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		68: {Name: PLAY_SPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		69: {Name: BRIGHTNESS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		70: {Name: CONTRAST, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		71: {Name: SHARPNESS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		72: {Name: DENOISE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		73: {Name: POLARITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		74: {Name: ANCHORING_INFRASTRUCTURE_VERSION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		75: {Name: DTM_VERSION, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		76: {Name: SOLUTION_TYPE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		77: {Name: GRADE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		78: {Name: WINDOW_MODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		79: {Name: WINDOW_TYPE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		80: {Name: TRACK_VMD_STATUS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		81: {Name: WIN_HISTORY_MODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		82: {Name: SENSOR_TYPE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		83: {Name: WINDOW_CORNER1_FF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		84: {Name: WINDOW_CORNER1_FF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		85: {Name: WINDOW_CORNER2_FF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		86: {Name: WINDOW_CORNER2_FF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		87: {Name: WINDOW_CORNER3_FF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		88: {Name: WINDOW_CORNER3_FF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		89: {Name: WINDOW_CORNER4_FF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		90: {Name: WINDOW_CORNER4_FF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		91: {Name: WINDOW_CENTER_FF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		92: {Name: WINDOW_CENTER_FF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		93: {Name: WINDOW_CENTER_SF_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
		94: {Name: WINDOW_CENTER_SF_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
	},

	ANANAS_INVESTIGATOR_USER_DEFINED_SET: {
		128: {Name: INVESTIGATOR_USER_DEFINED, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: AnanasInvestigator}},
	},
}

var MISB0806Set map[int]Tag = map[int]Tag{
	1:  {Name: CRC_32, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: 0, Offset: 0}},
	3:  {Name: PLATFORM_TRUE_AIRSPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	4:  {Name: PLATFORM_INDICATED_AIRSPEED, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	6:  {Name: FRAG_CIRCLE_RADIUS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	7:  {Name: FRAME_CODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: 0, Offset: 0}},
	8:  {Name: UAS_LS_VERSION_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	9:  {Name: VIDEO_DATA_RATE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: 0, Offset: 0}},
	10: {Name: DIGITAL_VIDEO_FILE_FORMAT, Decode: DecodeString, Properties: DecodeProperties{}},
	11: {Name: USER_DEFINED_LS, Decode: DecodeBytes, Properties: DecodeProperties{}},
	12: {Name: POINT_OF_INTEREST_LS, Decode: DecodeBytes, Properties: DecodeProperties{}},
	13: {Name: AREA_OF_INTEREST_LS, Decode: DecodeBytes, Properties: DecodeProperties{}},
	14: {Name: MGRS_ZONE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	15: {Name: MGRS_LATITUDE_BAND_AND_GRID_SQUARE, Decode: DecodeString, Properties: DecodeProperties{}},
	16: {Name: MGRS_EASTING, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	17: {Name: MGRS_NORTHING, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	18: {Name: MGRS_ZONE_SECOND_VALUE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	19: {Name: MGRS_LATITUDE_BAND_AND_GRID_SQUARE_SECOND_VALUE, Decode: DecodeString, Properties: DecodeProperties{}},
	20: {Name: MGRS_EASTING_SECOND_VALUE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	21: {Name: MGRS_NORTHING_SECOND_VALUE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
}

var VTrackerSet map[int]Tag = map[int]Tag{
	1: {Name: TARGET_ID, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false, Scale: 0, Offset: 0}},
	2: {Name: DETECTION_STATUS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	3: {Name: START_TIMESTAMP, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false, Scale: 0, Offset: 0}},
	4: {Name: END_TIMESTAMP, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false, Scale: 0, Offset: 0}},
	6: {Name: ALGORITHM, Decode: DecodeString, Properties: DecodeProperties{}},
	7: {Name: CONFIDENCE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	8: {Name: NUMBER_OF_POINTS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
}

var VFeatureSet map[int]Tag = map[int]Tag{
	1: {Name: SCHEMA, Decode: DecodeString, Properties: DecodeProperties{}},
	2: {Name: FEATURE, Decode: DecodeString, Properties: DecodeProperties{}},
}

var VChipSet map[int]Tag = map[int]Tag{
	1: {Name: IMAGE_TYPE, Decode: DecodeString, Properties: DecodeProperties{}},
	2: {Name: IMAGE_URI, Decode: DecodeString, Properties: DecodeProperties{}},
	3: {Name: EMBEDDED_IMAGE, Decode: DecodeString, Properties: DecodeProperties{}},
}

var VObjectSet map[int]Tag = map[int]Tag{
	1: {Name: ONTOLOGY, Decode: DecodeString, Properties: DecodeProperties{}},
	2: {Name: CLASS, Decode: DecodeString, Properties: DecodeProperties{}},
}

var VTARGETSet map[int]Tag = map[int]Tag{
	1:  {Name: TARGET_CENTROID_PIXEL_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 6, Signed: false, Scale: 0, Offset: 0}},
	2:  {Name: BOUNDING_BOX_TOP_LEFT_PIXEL_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 6, Signed: false, Scale: 0, Offset: 0}},
	3:  {Name: BOUNDING_BOX_BOTTOM_RIGHT_PIXEL_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 6, Signed: false, Scale: 0, Offset: 0}},
	4:  {Name: TARGET_PRIORITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	5:  {Name: TARGET_CONFIDENCE_LEVEL, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	6:  {Name: TARGET_HISTORY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	7:  {Name: PERCENTAGE_OF_TARGET_PIXELS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: false, Scale: 0, Offset: 0}},
	8:  {Name: TARGET_COLOR, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	9:  {Name: TARGET_INTENSITY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	10: {Name: TARGET_LOCATION_LATITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	11: {Name: TARGET_LOCATION_LONGITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	12: {Name: TARGET_HEIGHT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: MAX_HEIGHT, Offset: -1 * ALTITUDE_DEC_VALUE}},
	13: {Name: BOUNDING_BOX_TOP_LEFT_LATITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	14: {Name: BOUNDING_BOX_TOP_LEFT_LONGITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	15: {Name: BOUNDING_BOX_BOTTOM_RIGHT_LATITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	16: {Name: BOUNDING_BOX_BOTTOM_RIGHT_LONGITUDE_OFFSET, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: true, Scale: COORDINATE_OFFSET_MAX_VALUE, Offset: 0}},
	19: {Name: TARGET_CENTROID_PIXEL_ROW, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: 0, Offset: 0}},
	20: {Name: TARGET_CENTROID_PIXEL_COLUMN, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: false, Scale: 0, Offset: 0}},
	// 17, 18, 21, 101 are structs, not gonna implement for now
	102: {Name: VOBJECT, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: VObjectSet}},
	103: {Name: VFEATURE_LDS, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: VFeatureSet}},
	104: {Name: VTRACKER_LDS, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: VTrackerSet}},
	105: {Name: VCHIP_LDS, Decode: DecodeKLVSet, Properties: DecodeProperties{KLVSet: VChipSet}},
}

var MISB0903Set map[int]Tag = map[int]Tag{
	1:   {Name: CHECKSUM, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	2:   {Name: UNIX_TIMESTAMP, Decode: DecodeNumber, Properties: DecodeProperties{Size: 8, Signed: false, Scale: 0, Offset: 0}},
	3:   {Name: VMTI_SYSTEM_NAME, Decode: DecodeString, Properties: DecodeProperties{}},
	4:   {Name: VMTI_LDS_VERSION_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	5:   {Name: TOTAL_NUMBER_OF_TARGETS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	6:   {Name: NUMBER_OF_REPORTED_TARGETS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	7:   {Name: VIDEO_FRAME_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 3, Signed: false, Scale: 0, Offset: 0}},
	8:   {Name: FRAME_WIDTH, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	9:   {Name: FRAME_HEIGHT, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: 0, Offset: 0}},
	10:  {Name: VMTI_SOURCE_SENSOR, Decode: DecodeString, Properties: DecodeProperties{}},
	11:  {Name: VMTI_SENSOR_HORIZONTAL_FOV, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: HALF_CIRCLE, Offset: 0}},
	12:  {Name: VMTI_SENSOR_VERTICAL_FOV, Decode: DecodeNumber, Properties: DecodeProperties{Size: 2, Signed: false, Scale: HALF_CIRCLE, Offset: 0}},
	101: {Name: VTARGET_SERIES, Decode: DecodeVtargets, Properties: DecodeProperties{}},
}

var AnanasInvestigator map[int]Tag = map[int]Tag{
	6:   {Name: IMAGE_IMPROVED_MODE, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	7:   {Name: DIGITAL_ZOOM, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	8:   {Name: DIVIDE_OR_MULTIPLY, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	17:  {Name: RANGE_REPORTER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	20:  {Name: INVESTIGATOR_PLATFORMS_LOS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: FULL_CIRCLE, Offset: 0}},
	37:  {Name: TRACKER_CROSS_POSITION_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
	38:  {Name: TRACKER_CROSS_POSITION_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
	40:  {Name: TRACKING_STATUS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	41:  {Name: UAS_LDS_VERSION_NUMBER, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	100: {Name: TRACKER_BOUNDING_BOX_SIZE_X, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
	101: {Name: TRACKER_BOUNDING_BOX_SIZE_Y, Decode: DecodeNumber, Properties: DecodeProperties{Size: 4, Signed: true, Scale: 0, Offset: 0}},
	102: {Name: ICD_VERSION, Decode: DecodeString, Properties: DecodeProperties{}},
	103: {Name: BRIGHTNESS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	104: {Name: CONTRAST, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
	105: {Name: SHARPNESS, Decode: DecodeNumber, Properties: DecodeProperties{Size: 1, Signed: true, Scale: 0, Offset: 0}},
}

var GenericFlagSet []string = []string{"laserRange", "autoTrack", "irPolarity", "icingStatus", "slantRange", "imageInvalid"}
