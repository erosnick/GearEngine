#ifndef RHI_RENDER_PASS_H
#define RHI_RENDER_PASS_H
#include "RHIDefine.h"
#include "Utility/Hash.h"
#include <unordered_map>
/**
  1.ʹ��hashΪ����ȥ����vkrenderpass����
  2.���Ⱪ¶�����ͽṹ������ʹ��vulkanԭ���ṹ(���ڿ���ʵ����ʱʹ��vulkanԭ���ṹ)
*/

class RHIDevice;

struct RHIColorAttachment {
	VkFormat format = VK_FORMAT_UNDEFINED;
	VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	uint32_t numSample = 1;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
};

struct RHIDepthStencilAttachment {
	VkFormat format = VK_FORMAT_UNDEFINED;
	VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	uint32_t numSample = 1;
	VkImageLayout initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	VkImageLayout finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
};

class RHIRenderTargetLayout
{
public:
	RHIRenderTargetLayout(RHIDevice* device);
	virtual ~RHIRenderTargetLayout();
private:
	RHIDevice* mDevice;
};

class RHIRenderPass
{
public:
	RHIRenderPass(RHIDevice* device, const RHIRenderTargetLayout& layout);
	virtual ~RHIRenderPass();
private:
	struct VariantKey
	{
		VariantKey(LoadMaskBits loadMask, ReadMaskBits readMask, ClearMaskBits clearMask);

		class HashFunction
		{
		public:
			size_t operator()(const VariantKey& key) const;
		};

		class EqualFunction
		{
		public:
			bool operator()(const VariantKey& lhs, const VariantKey& rhs) const;
		};

		LoadMaskBits loadMask;
		ReadMaskBits readMask;
		ClearMaskBits clearMask;
	};
private:
	VkRenderPass mRenderPass;
	std::unordered_map<VariantKey, VkRenderPass, VariantKey::HashFunction, VariantKey::EqualFunction> mVariants;
};
#endif
