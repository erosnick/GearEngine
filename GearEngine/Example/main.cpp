#include <iostream>
#include "Application/Application.h"
#include "Application/Window.h"
#include "RHI/RHI.h"
#include "RHI/RHIPipelineState.h"
#include "RHI/RHITexture.h"
#include "RHI/RHITextureView.h"
#include "RHI/RHIFramebuffer.h"
#include "Utility/FileSystem.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

float vertices[] = {
	0.0f,  0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0,
	-0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0,
	 0.5f, -0.5f, 0.0f, 0.0, 0.0, 0.0, 0.0, 0.0
};

unsigned int indices[] = {
	2, 1, 0
};

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
		// ������ֱ֤�Ӵ���pso����
		mTestPipeline = new RHIGraphicsPipelineState(device, pipelineInfo);

		mTestVertexbuffer = device->createVertexBuffer(sizeof(Vertex), 3);
		mTestVertexbuffer->writeData(0,sizeof(vertices), vertices);
		
		mTestIndexbuffer = device->createIndexBuffer(sizeof(unsigned int),3);
		mTestIndexbuffer->writeData(0,sizeof(indices), indices);
	}
	virtual void runMainLoop()
	{
		while (!glfwWindowShouldClose(mWindow->getWindowPtr()))
		{
			mWindow->beginFrame();
			// ����ִ�д���
			mTestCmdBuffer->setRenderTarget(mWindow->getFramebuffer());
			mTestCmdBuffer->setViewport(glm::vec4(0, 0, 800, 600));
			mTestCmdBuffer->setScissor(glm::vec4(0, 0, 800, 600));
			mTestCmdBuffer->bindGraphicsPipelineState(mTestPipeline);
			mTestCmdBuffer->bindIndexBuffer(mTestIndexbuffer);
			mTestCmdBuffer->bindVertexBuffer(mTestVertexbuffer);
			mTestCmdBuffer->begin();
			mTestCmdBuffer->beginRenderPass(glm::vec4(0,0,800,600));
			mTestCmdBuffer->drawIndexed(3,1,0,0,0);
			mTestCmdBuffer->endRenderPass();
			mTestCmdBuffer->end();
			
			mWindow->endFrame(mTestCmdBuffer);
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
		delete mTestVertexbuffer;
		delete mTestIndexbuffer;
	}

private:
	RHICommandBuffer* mTestCmdBuffer;
	RHIProgram* mTestVertexProgram;
	RHIProgram* mTestFragmentProgram;
	RHIGraphicsPipelineState* mTestPipeline;
	RHIVertexBuffer* mTestVertexbuffer;
	RHIIndexBuffer* mTestIndexbuffer;
};

int main()
{
	MyApplication app;
	app.prepare();
	app.runMainLoop();
	app.finish();
	return 0;
}