#ifndef BUFFER_H
#define BUFFER_H
#include "RHIDefine.h"

//ֻʹ��vulkan�Ļ��Ͳ��õ����ٳ���һ��RHIBufferUsageBit
class RHIDevice;
class RHIBuffer
{
public:
	RHIBuffer(RHIDevice* device, VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
	~RHIBuffer();
	void readData(uint32_t offset, uint32_t size, void* dest);
	void writeData(uint32_t offset, uint32_t size, void* source);
	VkBuffer getBuffer() { return mBuffer; }
	VkDeviceMemory getMemory() { return mMemory; }
	VkDeviceSize getSize() { return mSize; }
private:
	RHIDevice* mDevice;
	VkBuffer mBuffer;
	VkDeviceMemory mMemory;
	VkDeviceSize mSize;
};
#endif