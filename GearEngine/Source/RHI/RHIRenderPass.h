#ifndef RHI_RENDER_PASS_H
#define RHI_RENDER_PASS_H
#include "RHIDefine.h"
#include "Utility/Hash.h"
#include <unordered_map>
/**
  1.ʹ��hashΪ����ȥ����vkrenderpass����(�ѷ���)
  2.���Ⱪ¶�����ͽṹ������ʹ��vulkanԭ���ṹ(���ڿ���ʵ����ʱʹ��vulkanԭ���ṹ)
  3.�ݲ�֧�ֶ��subpass
*/

class RHIDevice;

struct RHIColorAttachmentInfo {
	VkFormat format = VK_FORMAT_UNDEFINED;
	uint32_t numSample = 1;
	VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
};

struct RHIDepthStencilAttachmentInfo {
	VkFormat format = VK_FORMAT_UNDEFINED;
	uint32_t numSample = 1;
	VkAttachmentLoadOp depthLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	VkAttachmentStoreOp depthStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
};

struct RHIRenderPassInfo {
	RHIColorAttachmentInfo color[8];
	RHIDepthStencilAttachmentInfo depthStencil;
	uint32_t numColorAttachments;
	bool hasDepth;
};

class RHIRenderPass
{
public:
	RHIRenderPass(RHIDevice* device, const RHIRenderPassInfo& info);
	virtual ~RHIRenderPass();
	uint32_t getID() { return mID; }
	VkRenderPass getHandle() { return mRenderPass; }
private:
	RHIDevice* mDevice;
	VkRenderPass mRenderPass;
	uint32_t mID;

	static uint32_t sNextValidID;
};
#endif
