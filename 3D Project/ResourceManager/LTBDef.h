#pragma once
#ifndef _LTB_DEFINE__
#define _LTB_DEFINE__
typedef unsigned int        uint; // This is at least 16 bits

typedef char			int8;
typedef short int		int16;
typedef int				int32;
typedef __int64         int64;

typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned int		uint32;

typedef unsigned __int64    uint64;

// File types...
enum E_LTB_FILE_TYPES {
	LTB_D3D_MODEL_FILE = 1,
	LTB_PS2_MODEL_FILE = 2,
	LTB_XBOX_MODEL_FILE = 3,
	LTB_ABC_MODEL_FILE = 4,
	LTB_D3D_RENDERSTYLE_FILE = 5,
	LTB_PS2_RENDERSTYLE_FILE = 6,
	LTB_D3D_RENDEROBJECT_FILE = 7
};

// LTB header 
struct LTB_Header {
	LTB_Header() { m_iFileType = 0; m_iVersion = 0; }
	unsigned char  m_iFileType;						// Should be one of E_LTB_FILE_TYPES
	unsigned short m_iVersion;						// Version number...
	unsigned char  m_iReserved1;					// Reserved for stuff we thing of later...
	unsigned int m_iReserved2;
	unsigned int m_iReserved3;
	unsigned int m_iReserved4;
};



enum    D3DPIECE_TYPE { eRigidMesh = 4, eSkelMesh = 5, eVAMesh = 6, eNullMesh = 7 };
struct Vector
{
	float x, y, z;
};
// The verts we'll use (for all the possible vertex structures)...
struct  VSTREAM_XYZ_B0 { float x; float y; float z; };
struct  VSTREAM_XYZ_B1 { float x; float y; float z; float blend1; };
struct  VSTREAM_XYZ_B2 { float x; float y; float z; float blend1; float blend2; };
struct  VSTREAM_XYZ_B3 { float x; float y; float z; float blend1; float blend2; float blend3; };
struct  VSTREAM_XYZ_B1_INDEX { float x; float y; float z; float blend1; uint8 Index[4]; };
struct  VSTREAM_XYZ_B2_INDEX { float x; float y; float z; float blend1; float blend2; uint8 Index[4]; };
struct  VSTREAM_XYZ_B3_INDEX { float x; float y; float z; float blend1; float blend2; float blend3; uint8 Index[4]; };
struct  VSTREAM_THE_REST { float nx; float ny; float nz; float u1; float v1; float u2, v2; float u3, v3; float u4, v4; uint32 ColorRGBA; Vector S, T, SxT; };


// VERTEX DATA TYPE FLAGS (Note: Should match those in the d3d structs header)...
#define	VERTDATATYPE_POSITION					0x0001
#define	VERTDATATYPE_NORMAL						0x0002
#define	VERTDATATYPE_COLOR						0x0004
#define	VERTDATATYPE_UVSETS_1					0x0010
#define	VERTDATATYPE_UVSETS_2					0x0020
#define	VERTDATATYPE_UVSETS_3					0x0040
#define	VERTDATATYPE_UVSETS_4					0x0080
#define	VERTDATATYPE_BASISVECTORS				0x0100


enum ANIMCOMPRESSIONTYPE { 	NONE = 0, RELEVANT, RELEVANT_16, RELEVANT_ROT16_ONLY};
#endif