#ifndef RHI_PIPELINE_STATE_H
#define RHI_PIPELINE_STATE_H
#include "RHIDefine.h"
#include "RHIProgram.h"
#include "RHIProgramParam.h"
#include "RHIRenderPass.h"
#include "Utility/Hash.h"

#include <unordered_map>

// note:�ݲ�����������
// note:ʹ������ָ�������λ��һ��?

struct RHIPipelineStateInfo
{
	//programs
	RHIProgram* vertexProgram = nullptr;
	RHIProgram* fragmentProgram = nullptr;

	//todo:render states
};

class RHIDevice;

class RHIGraphicsPipelineState
{
public:
	RHIGraphicsPipelineState(RHIDevice* device, const RHIPipelineStateInfo& info);
	~RHIGraphicsPipelineState();
	VkPipeline getPipeline(RHIRenderPass* renderPass);
	VkPipelineLayout getLayout() { return mPipelineLayout; }
	std::vector<VkDescriptorSet>& getDescSets() { return mDescriptorSets2; }
private:
	struct VariantKey
	{
		// note:�����׶ν�ʹ��render pass����hash key,�������л��Ʒ�ʽ�Լ��������벼��
		VariantKey(uint32_t inRenderpassID);

		class HashFunction
		{
		public:
			std::size_t operator()(const VariantKey& key) const;
		};

		class EqualFunction
		{
		public:
			bool operator()(const VariantKey& lhs, const VariantKey& rhs) const;
		};

		uint32_t renderpassID;
	};

	VkPipeline createVariant(RHIRenderPass* renderPass);

private:
	RHIDevice* mDevice;
	RHIProgram* mVertexProgram = nullptr;
	RHIProgram* mFragmentProgram = nullptr;
	RHIProgramParamList* mParamList = nullptr;
	std::unordered_map<VariantKey, VkPipeline, VariantKey::HashFunction, VariantKey::EqualFunction> mPipelines;
	// ����һ���޴��DescriptorPool�ô����ڷ���,��������������ڴ��˷�
	// ���ڷ���ΪΪÿ��RHIPipeline��ӵ��һ��DescriptorPool
	VkDescriptorPool mDescriptorPool;
	std::map<uint32_t, VkDescriptorSetLayout> mDescriptorSetLayouts;
	std::map<uint32_t, VkDescriptorSet> mDescriptorSets;
	// ��ʱ����
	std::vector<VkDescriptorSet> mDescriptorSets2;
	VkPipelineLayout mPipelineLayout;
	std::vector<uint32_t> mSets;
};

#endif