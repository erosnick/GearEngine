#include "RHICommandBuffer.h"
#include "RHIDevice.h"

RHICommandBuffer::RHICommandBuffer(RHIDevice* device)
	:mDevice(device)
{
}

RHICommandBuffer::~RHICommandBuffer()
{
	vkFreeCommandBuffers(mDevice->getDevice(), mCommandPool, 1, &mCommandBuffer);
}

void RHICommandBuffer::begin()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//�������ó�ÿ���ύ�󶼻��Զ���������
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin command buffer!");
	}
}

void RHICommandBuffer::end()
{
	//
	if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to end command buffer!");
	}
}
