#include <iostream>
#include "Application/Application.h"
#include "Application/Window.h"
#include "RHI/RHI.h"
#include "RHI/RHIPipelineState.h"
#include "Utility/FileSystem.h"

class MyApplication : public Application
{
public:
	MyApplication()
		:Application()
	{
	}
	~MyApplication()
	{
	}
	virtual void prepare()
	{
		RHIDevice* device = RHI::instance().getDevice();
		// ����cmd buffer
		mTestCmdBuffer = device->allocCommandBuffer(CommandBufferType::GRAPHICS, true);

		// ����pipeline state
		RHIProgramInfo programInfo;
		programInfo.type = RHIProgramType::Vertex;
		programInfo.entryPoint = "main";
		readFile("D:/GearEngine/GearEngine/Resource/Shaders/default.vert", programInfo.source);
		mTestVertexProgram = device->createProgram(programInfo);
		mTestVertexProgram->compile();

		programInfo.type = RHIProgramType::Fragment;
		programInfo.entryPoint = "main";
		readFile("D:/GearEngine/GearEngine/Resource/Shaders/default.frag", programInfo.source);
		mTestFragmentProgram = device->createProgram(programInfo);
		mTestFragmentProgram->compile();

		RHIPipelineStateInfo pipelineInfo;
		pipelineInfo.vertexProgram = mTestVertexProgram;
		pipelineInfo.fragmentProgram = mTestFragmentProgram;
		// pso����Ӧ����device����,Ϊ�˿�����ֱ֤�Ӵ���pso����
		mTestPipeline = new RHIGraphicsPipelineState(device, pipelineInfo);

		// ����render target
		RHIRenderPassAttachmentDesc color;
		color.format = VK_FORMAT_B8G8R8A8_UNORM;
		color.numSample = 1;
		RHIRenderPassDesc passInfo;
		passInfo.color[0] = color;
		passInfo.hasDepth = false;
		passInfo.numColorAttachments = 1;

		mTestRenderpass = new RHIRenderPass(device, passInfo);
	}
	virtual void runMainLoop()
	{
		while (!glfwWindowShouldClose(mWindow->getWindowPtr()))
		{
			mWindow->beginFrame();
			// ����ִ�д���
			
			mWindow->endFrame();
			Input::instance().update();
			glfwPollEvents();
		}
	}
	virtual void finish()
	{
		delete mTestCmdBuffer;
		delete mTestVertexProgram;
		delete mTestFragmentProgram;
		delete mTestPipeline;
		delete mTestRenderpass;
		//delete mTextFramebuffer;
	}

private:
	RHICommandBuffer* mTestCmdBuffer;
	RHIProgram* mTestVertexProgram;
	RHIProgram* mTestFragmentProgram;
	RHIGraphicsPipelineState* mTestPipeline;
	RHIRenderPass* mTestRenderpass;
	//RHIFramebuffer* mTextFramebuffer;
};

int main()
{
	MyApplication app;
	app.prepare();
	app.runMainLoop();
	app.finish();
	return 0;
}