#ifndef RHI_PIPELINR_PROGRAM_PARAMS
#define RHI_PIPELINE_PROGRAM_PARAMS
#include "RHIDefine.h"
#include "RHIProgramParam.h"

// note:Ŀǰֻ֧���������͵���ɫ��
struct RHIPipelineParamsInfo
{
	RHIParamInfo vertexParams;
	RHIParamInfo fragmentParams;
};


class RHIPipelineParams
{
public:
	RHIPipelineParams();
	~RHIPipelineParams();

private:

};

#endif