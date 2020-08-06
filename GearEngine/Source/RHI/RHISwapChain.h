#ifndef RHI_SWAP_CHAIN_H
#define RHI_SWAP_CHAIN_H
#include "RHIDefine.h"
#include <vector>
/**
 ���������ṩ�ع�����,�����ڳߴ緢���ı�ʱӦ���´���һ��������
 ��������֡�����ֲ�֧����Ȼ���
*/
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class RHIDevice;
class RHIRenderPass;
class RHIFramebuffer;
class RHITextureView;

struct RHISwapChainInfo {
    void* windowHandle;
    uint32_t width;
    uint32_t height;
};

class RHISwapChain
{
public:
	RHISwapChain(RHIDevice* device, VkSurfaceKHR surface, uint32_t width, uint32_t height);
	~RHISwapChain();
	VkSwapchainKHR getHandle() { return mSwapChain; }
	RHIFramebuffer* getFramebuffer(uint32_t index);
private:
	SwapChainSupportDetails querySwapChainSupport();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
private:
	friend class Window;
	RHIDevice* mDevice;
	RHIRenderPass* mRenderPass;
	std::vector<RHIFramebuffer*> mFramebuffers;
	std::vector<RHITextureView*> mTextureViews;
	VkSurfaceKHR mSurface;
	VkSwapchainKHR mSwapChain;
	std::vector<VkImage> mSwapChainImages;
	VkFormat mSwapChainImageFormat;
	uint32_t mWidth;
	uint32_t mHeight;
};

#endif
