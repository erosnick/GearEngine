#ifndef RHI_PROGRAM_PARAM
#define RHI_PROGRAM_PARAM

#include "RHIDefine.h"
#include <vector>
#include <map>
#include <string>

enum class RHIParamDataType
{
	FLOAT1 = 1,
	FLOAT2 = 2,
	FLOAT3 = 3,
	FLOAT4 = 4,
	MATRIX_2X2 = 11,
	MATRIX_2X3 = 12,
	MATRIX_2X4 = 13,
	MATRIX_3X2 = 14,
	MATRIX_3X3 = 15,
	MATRIX_3X4 = 16,
	MATRIX_4X2 = 17,
	MATRIX_4X3 = 18,
	MATRIX_4X4 = 19,
	INT1 = 20,
	INT2 = 21,
	INT3 = 22,
	INT4 = 23,
	BOOL = 24,
	STRUCT = 25,
	COLOR = 26,
	COUNT = 27,
	UNKNOWN = 0xffff
};

// note:�����Ƶ���������
enum RHIParamObjectType
{
	SAMPLER1D = 1,
	SAMPLER2D = 2,
	SAMPLER3D = 3,
	SAMPLERCUBE = 4,
	TEXTURE1D = 11,
	TEXTURE2D = 12,
	TEXTURE3D = 13,
	TEXTURECUBE = 14,
	TEXTURE1DARRAY = 21,
	TEXTURE2DARRAY = 22,
	TEXTURE3DARRAY = 23,
	TEXTURECUBEARRAY = 24,
	UNKNOWN = 0xffff
};

// note:
// ��vulkan��shader�в���ֱ��ʹ�õ�һ����,ֻ��ʹ��uniform buffer(����ʹ��ParamBlock��ʾ)
struct RHIParamDataInfo
{
	std::string name;
	RHIParamDataType type;
	uint32_t elementSize;
	uint32_t arraySize;
	uint32_t arrayElementStride;
	uint32_t paramBlockSlot;
	uint32_t paramBlockSet;
	//shader��������ݻ��Զ�����(������ʾ���),�����ڴ��ƫ�Ʋ���ֱ�Ӱ�����������ȥ���м���
	uint32_t memOffset;
};

struct RHIParamObjectInfo
{
	std::string name;
	RHIParamObjectType type;
	uint32_t slot;
	uint32_t set;
};

struct RHIParamBlockInfo
{
	std::string name;
	uint32_t slot;
	uint32_t set;
	uint32_t blockSize;
	//�Ƿ����ò�ͬ���߽׶ι���
	bool isShareable = false;
};

// note:�Ժ���Ҫ��glslang�Խ�,���Խṹ���Ϳ����������
struct RHIParamInfo
{
	std::map<std::string, RHIParamBlockInfo> paramBlocks;
	std::map<std::string, RHIParamDataInfo> params;
	std::map<std::string, RHIParamObjectInfo> samplers;
};

// note:Ŀǰֻ֧���������͵���ɫ��
struct RHIProgramParamListInfo
{
	RHIParamInfo vertexParams;
	RHIParamInfo fragmentParams;
};

class RHIProgramParam
{
public:
	RHIProgramParam();
	~RHIProgramParam();

private:

};

class RHIProgramParamList
{
public:
	RHIProgramParamList();
	~RHIProgramParamList();
	
private:

};

#endif
