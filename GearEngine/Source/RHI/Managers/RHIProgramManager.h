#ifndef RHI_PROGRAM_MANAGER_H
#define RHI_PROGRAM_MANAGER_H
#include "RHI/RHIProgram.h"
#include "Utility/Module.h"
/**
   ����program��ʵʱ����
*/
class RHIProgramManager : public Module<RHIProgramManager>
{
public:
	RHIProgramManager();
	~RHIProgramManager();
	void compile(RHIProgram* program);
private:
};

#endif