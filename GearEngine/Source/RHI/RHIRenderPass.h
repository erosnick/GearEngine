#ifndef RHI_RENDER_PASS_H
#define RHI_RENDER_PASS_H
#include "RHIDefine.h"
#include "Utility/Hash.h"
#include <unordered_map>
/**
  1.ʹ��hashΪ����ȥ����vkrenderpass����
  2.���Ⱪ¶�����ͽṹ������ʹ��vulkanԭ���ṹ(���ڿ���ʵ����ʱʹ��vulkanԭ���ṹ)
  3.�ݲ�֧�ֶ��subpass
*/

class RHIDevice;

struct RHIRenderPassAttachmentInfo {
	VkFormat format = VK_FORMAT_UNDEFINED;
	uint32_t numSample = 1;
};

struct RHIRenderPassInfo {
	RHIRenderPassAttachmentInfo color[8];
	RHIRenderPassAttachmentInfo depthStencil;
	uint32_t numColorAttachments;
	bool hasDepth;
};

class RHIRenderPass
{
public:
	RHIRenderPass(RHIDevice* device, const RHIRenderPassInfo& desc);
	virtual ~RHIRenderPass();
	uint32_t getID() { return mID; }
	VkRenderPass getVkRenderPass(LoadMaskBits load, StoreMaskBits store, ClearMaskBits clear);
private:
	struct VariantKey
	{
		VariantKey(LoadMaskBits load, StoreMaskBits store, ClearMaskBits clear);

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
		StoreMaskBits storeMask;
		ClearMaskBits clearMask;
	};

	VkRenderPass createVariant(LoadMaskBits load, StoreMaskBits store, ClearMaskBits clear);
private:
	RHIDevice* mDevice;
	VkRenderPass mRenderPass;
	RHIRenderPassInfo mDesc;
	uint32_t mID;
	std::unordered_map<VariantKey, VkRenderPass, VariantKey::HashFunction, VariantKey::EqualFunction> mVariants;

	static uint32_t sNextValidID;
};
#endif
