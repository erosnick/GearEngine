#include "RHICommandBuffer.h"
#include "RHIDevice.h"
#include "RHIQueue.h"
RHICommandBuffer::RHICommandBuffer(RHIDevice* device, RHIQueue* queue)
	:mDevice(device), mQueue(queue)
{
	//����fence
	//todo:����������fence����ת����device��
	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	
	if (vkCreateFence(mDevice->getDevice(), &fenceInfo, nullptr, &mFence) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create fence!");
	}
}

RHICommandBuffer::~RHICommandBuffer()
{
	vkDestroyFence(mDevice->getDevice(), mFence, nullptr);
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
