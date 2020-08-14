#include "RHIDescriptorSet.h"
#include "RHIDevice.h"
#include "RHIBuffer.h"
#include "RHITexture.h"
#include <vector>

RHIDescriptorSet::RHIDescriptorSet(RHIDevice* device, RHIDescriptorSetInfo info)
	:mDevice(device), mInfo(info)
{
    std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
    for (int i = 0; i < info.bindingCount; ++i)
    {
        VkDescriptorSetLayoutBinding setLayoutBinding = {};
        setLayoutBinding.pImmutableSamplers = nullptr;
        setLayoutBinding.binding = info.bindings[i].binding;
        setLayoutBinding.descriptorCount = info.bindings[i].descriptorCount;

        if(info.bindings[i].type & DESCRIPTOR_TYPE_UNIFORM_BUFFER)
        {
            setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        }
        else if(info.bindings[i].type & DESCRIPTOR_TYPE_RW_BUFFER)
        {
            setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        }
        else if(info.bindings[i].type & DESCRIPTOR_TYPE_TEXTURE)
        {
            setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        }
        else if(info.bindings[i].type & DESCRIPTOR_TYPE_RW_TEXTURE)
        {
            setLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        }

        if(info.bindings[i].stage == PROGRAM_VERTEX)
        {
            setLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        }
        else if(info.bindings[i].stage == PROGRAM_FRAGMENT)
        {
            setLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        }
        else if(info.bindings[i].stage == PROGRAM_COMPUTE)
        {
            setLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
        }
        setLayoutBindings.push_back(setLayoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutCreateInfo = {};
    layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutCreateInfo.bindingCount = setLayoutBindings.size();
    layoutCreateInfo.pBindings = setLayoutBindings.data();

    CHECK_VKRESULT(vkCreateDescriptorSetLayout(mDevice->getDevice(), &layoutCreateInfo, nullptr, &mLayout));

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = mDevice->getDescriptorPool();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &mLayout;

    CHECK_VKRESULT(vkAllocateDescriptorSets(mDevice->getDevice(), &allocInfo, &mSet));
}

RHIDescriptorSet::~RHIDescriptorSet()
{
    vkDestroyDescriptorSetLayout(mDevice->getDevice(), mLayout, nullptr);
    vkFreeDescriptorSets(mDevice->getDevice(), mDevice->getDescriptorPool(), 1, &mSet);
}

void RHIDescriptorSet::updateBuffer(uint32_t binding, RHIBuffer *buffer, uint32_t size, uint32_t offset)
{
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = buffer->getHandle();
    bufferInfo.offset = offset;
    bufferInfo.range = size;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = mSet;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;

    if(buffer->getDescriptorType() & DESCRIPTOR_TYPE_UNIFORM_BUFFER)
    {
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }
    else if(buffer->getDescriptorType() & VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
    {
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }

    vkUpdateDescriptorSets(mDevice->getDevice(), 1, &descriptorWrite, 0, nullptr);
}

void RHIDescriptorSet::updateTexture(uint32_t binding, RHITexture* texture, RHISampler* sampler)
{
    // todo image layout
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    imageInfo.imageView = texture->getView();
    if(sampler)
        imageInfo.sampler = sampler->getHandle();
    else
        imageInfo.sampler = VK_NULL_HANDLE;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = mSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.pImageInfo = &imageInfo;
    descriptorWrite.descriptorCount = 1;

    if(texture->getDescriptorType() & DESCRIPTOR_TYPE_TEXTURE)
    {
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
    }
    else if(texture->getDescriptorType() & DESCRIPTOR_TYPE_RW_TEXTURE)
    {
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    }

    vkUpdateDescriptorSets(mDevice->getDevice(), 1, &descriptorWrite, 0, nullptr);
}