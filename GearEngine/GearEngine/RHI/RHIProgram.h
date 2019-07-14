#ifndef RHI_PROGRAM_H
#define RHI_PROGRAM_H
#include <string>

enum class RHIProgramType
{
	Vertex,
	Fragment
};

struct RHIProgramDesc
{
	std::string source;
	std::string entryPoint;
	RHIProgramType type = RHIProgramType::Vertex;
};

//����ҪProgram�����Լ������������벼��
#endif