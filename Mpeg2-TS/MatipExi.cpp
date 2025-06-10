#include "stdafx.h"

#include "Mpeg2-TS.h"

#include "grammarGenerator.h"
#include "EXIParser.h"
#include "stringManipulate.h"
#include "EXISerializer.h"

MPEG2TS_API int __stdcall matip_2_exi(BYTE* packet, BYTE* packet_exi, char* schemaBuffer, int schemaLen)
{
	ExipSchema schema;
	errorCode tmp_err_code = generateSchemaInformedGrammars(schemaBuffer, schemaLen, 
			schemaLen, NULL, SCHEMA_FORMAT_XSD_EXI, &schema);

	int t = 0;
	t++;
}