#pragma once

#define VK_USE_PLATFORM_WIN_32
#include <vulkan/vulkan.h>
#include "source/initWindow.h"
#include "Shaders/shaderReader.h"
#include "Models/vertexInfo.h"

#define VMA_IMPLEMENTATION
#include <VMA/vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

VkInstance instance;
VkPhysicalDevice physDevice;
VkDevice device;
VkQueue queue;
VkSurfaceKHR surface;
VkSwapchainKHR swapchain;
std::vector<VkImage> swapImages;
std::vector<VkImageView> swapImageViews;
VkRenderPass renPass;
VkDescriptorSetLayout descriptSetLayout;
VkPipelineLayout pipeLayout;
VkPipeline graphPipeline0;
VkPipeline graphPipeline1;
VkPipeline graphPipeline2;
VkPipeline graphPipeline3;
VkDescriptorPool descriptPool;
VkDescriptorSet dstSet;
std::vector<VkFramebuffer> frameBuffers;
VkCommandPool cmdPool;
VkCommandBuffer cmdBuffer;

VkSemaphore waitSemaphore;
VkSemaphore signalSemaphore;
VkFence fence;
VkSemaphore shadowSemaphore;

VmaAllocator allocator;

const VkDeviceSize offsets[] = { 0 };
VkBuffer vertexBuffer;
VmaAllocation vertexBufferAlloc;
VkBuffer indexBuffer;
VmaAllocation indexBufferAlloc;
VkBuffer vertexBuffer1;
VmaAllocation vertexBufferAlloc1;
VkBuffer indexBuffer1;
VmaAllocation indexBufferAlloc1;
VkBuffer vertexBuffer2;
VmaAllocation vertexBufferAlloc2;
VkBuffer indexBuffer2;
VmaAllocation indexBufferAlloc2;
VkBuffer vertexBuffer3;
VmaAllocation vertexBufferAlloc3;
VkBuffer indexBuffer3;
VmaAllocation indexBufferAlloc3;

uint32_t mipLevels;
VkImage textureImage;
VmaAllocation textureImageAlloc;
VkImageView textureImageView;
VkSampler textureSampler;
VkImage textureImage1;
VmaAllocation textureImageAlloc1;
VkImageView textureImageView1;
VkSampler textureSampler1;
VkImage textureImage3;
VmaAllocation textureImageAlloc3;
VkImageView textureImageView3;
VkSampler textureSampler3;
VkImage shadowDepthImage;
VmaAllocation shadowDepthImageAlloc;
VkImageView shadowDepthImageView;
VkSampler shadowDepthSampler;

VkBuffer uniformBuffer0;
VmaAllocation uniformBufferAlloc0;
VmaAllocationInfo uniformBufferAllocInfo0 = {};

VkSampleCountFlagBits samplesCount = VK_SAMPLE_COUNT_4_BIT;
VkImage msaaImage;
VmaAllocation msaaImageAlloc;
VkImageView msaaImageView;

VkImage depthTestImage;
VmaAllocation depthImageAlloc;
VkImageView depthImageView;
VkClearValue depthValue = { 1.0f, 0.0f };

VkFormat depthFormat = VK_FORMAT_D32_SFLOAT;
VkRenderPass shadowMapRenPass;
VkFramebuffer shadowMapFramebuffer;
VkPipelineLayout shadowDepthPipelineLayout;
VkPipeline shadowDepthPipeline;

#ifdef NDEBUG
std::vector<const char*> validLayer{ };
#else
std::vector<const char*> validLayer{ "VK_LAYER_KHRONOS_validation" };
#endif

VkFormat rendering_format;

std::vector<const char*> ExtsIns{ "VK_KHR_surface", "VK_KHR_win32_surface" };
std::vector<const char*> ExtDev{ "VK_KHR_swapchain" };

VkShaderModule vsModule;
VkShaderModule fsModule;
VkShaderModule polyVSmodule;
VkShaderModule polyFSmodule;
VkShaderModule sphereVSmodule;
VkShaderModule sphereFSmodule;
VkShaderModule cubeMapVSmodule;
VkShaderModule cubeMapFSmodule;
VkShaderModule shadowMapModule;
std::vector<char> vsCode = shaderReader("Shaders/vert.spv");
std::vector<char> fsCode = shaderReader("Shaders/frag.spv");
std::vector<char> polyVSCode = shaderReader("Shaders/polyVS.spv");
std::vector<char> polyFSCode = shaderReader("Shaders/polyFS.spv");
std::vector<char> sphereVSCode = shaderReader("Shaders/sphereVS.spv");
std::vector<char> sphereFSCode = shaderReader("Shaders/sphereFS.spv");
std::vector<char> cubeMapVSCode = shaderReader("Shaders/cubeMapVS.spv");
std::vector<char> cubeMapFSCode = shaderReader("Shaders/cubeMapFS.spv");
std::vector<char> shadowMapVs = shaderReader("Shaders/shadowMap.spv");

struct uniformBufferObject0 {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 model1;
	glm::mat4 view1;
	glm::mat4 lightSpace;
	glm::vec3 lightPos;
}ubo0;

glm::mat4 model = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(-2.0f, -5.0, -6.0f));
glm::mat4 modification =
	glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
			  0.0f,-1.0f, 0.0f, 0.0f,
			  0.0f, 0.0f, 0.5f, 0.0f,
			  0.0f, 0.0f, 0.5f, 1.0f );
glm::mat4 lightProjection = modification * glm::perspective(glm::radians(45.0f), static_cast<float>(2048) / 2048, 1.0f, 1000.0f);
glm::mat4 lightView = glm::lookAt(glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f)) * glm::vec4(20.0, 45.0f, 35.0f, 1.0f)), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
glm::mat4 lightSpaceMatrix = lightProjection * lightView;

//glm::mat4 lightView = glm::lookAt(glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f)) * glm::vec4(20.0, 45.0f, 30.0f, 1.0f)), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

struct Push_constant{
	glm::mat4 model;
	glm::mat4 lightSpace;
}pc;

void createShaderModeles(const std::vector<char> &code, VkShaderModule* sModule)
{
	const VkShaderModuleCreateInfo smci{ .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	.codeSize = code.size(),
	.pCode = reinterpret_cast<const uint32_t*>(code.data()) };

	vkCreateShaderModule(device, &smci, nullptr, sModule);
}

void createAllocator()
{
	const VmaAllocatorCreateInfo allocatorCI{
	.physicalDevice = physDevice,
	.device = device,
	.instance = instance };

	vmaCreateAllocator(&allocatorCI, &allocator);
}

VkCommandBuffer startTimeCommands()
{
	const VkCommandBufferAllocateInfo cmdBufferAI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	.commandPool = cmdPool,
	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	.commandBufferCount = 1 };
	vkAllocateCommandBuffers(device, &cmdBufferAI, &cmdBuffer);

	const VkCommandBufferBeginInfo cmdBI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT };
	vkBeginCommandBuffer(cmdBuffer, &cmdBI);

	return cmdBuffer;
}

void endTimeCommands()
{
	vkEndCommandBuffer(cmdBuffer);

	const VkSubmitInfo SI = { .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.commandBufferCount = 1,
	.pCommandBuffers = &cmdBuffer };

	vkQueueSubmit(queue, 1, &SI, VK_NULL_HANDLE);
	vkQueueWaitIdle(queue);
}

void createBuffer(VkBuffer& buffer, VkDeviceSize bufferSize, void* data, VkBufferUsageFlags flag, VmaAllocation& bufferAlloc)
{
	VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = bufferSize,
	.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	VmaAllocationCreateInfo bAllocCI{
	.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };

	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingBufferAllocInfo{};
	vmaCreateBuffer(allocator, &bufferCI, &bAllocCI, &stagingBuffer, &stagingAllocation, &stagingBufferAllocInfo);

	memcpy(stagingBufferAllocInfo.pMappedData, data, bufferCI.size);

	bufferCI.flags = 0;
	bufferCI.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | flag;

	vmaCreateBuffer(allocator, &bufferCI, &bAllocCI, &buffer, &bufferAlloc, nullptr);

	VkCommandBuffer cmdBuffer = 
	startTimeCommands();
	{
		VkBufferCopy region{ .size = bufferCI.size };
		vkCmdCopyBuffer(cmdBuffer, stagingBuffer, buffer, 1, &region);
	}
	endTimeCommands();

	vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
	stagingBuffer = VK_NULL_HANDLE;
	stagingAllocation = VK_NULL_HANDLE;
}

void createUnibuffer(VkBuffer& uniformBuffer, VkDeviceSize bufferSize, VmaAllocation& uniformBufferAlloc, VmaAllocationInfo& allocInfo)
{
	const VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = bufferSize,
	.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	const VmaAllocationCreateInfo allocCI{
	.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };
	vmaCreateBuffer(allocator, &bufferCI, &allocCI, &uniformBuffer, &uniformBufferAlloc, &allocInfo);
}

void uploadTexture(const char* filename, VkImage& textureImage, VmaAllocation& imageAlloc)
{
	textureRead.load(filename);
	uint32_t textureWidth = textureRead.getTextureWidth();
	uint32_t textureHeight = textureRead.getTextureHeight();
	void* getPicture = textureRead.getPicture();
	uint32_t textureSize = textureRead.getTextureSize();
	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(textureWidth, textureHeight)))) + 1;

	const VkBufferCreateInfo bufferCI{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	.size = textureSize,
	.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE };

	VmaAllocationCreateInfo allocCI{
	.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT | VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT,
	.usage = VMA_MEMORY_USAGE_AUTO };

	VkBuffer stagingBuffer = VK_NULL_HANDLE;
	VmaAllocation stagingAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingAllocInfo = {};
	vmaCreateBuffer(allocator, &bufferCI, &allocCI, &stagingBuffer, &stagingAllocation, &stagingAllocInfo);
	memcpy(stagingAllocInfo.pMappedData, getPicture, bufferCI.size);

	VkImageCreateInfo imageCI = { .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	.imageType = VK_IMAGE_TYPE_2D,
	.format = VK_FORMAT_R8G8B8A8_SRGB,
	.extent = { textureWidth, textureHeight, 1 },
	.mipLevels = mipLevels,
	.arrayLayers = 1,
	.samples = VK_SAMPLE_COUNT_1_BIT,
	.tiling = VK_IMAGE_TILING_OPTIMAL,
	.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED };

	allocCI = { .flags = 0, .usage = VMA_MEMORY_USAGE_UNKNOWN };

	vmaCreateImage(allocator, &imageCI, &allocCI, &textureImage, &imageAlloc, nullptr);

	VkCommandBuffer cmdBuffer =
	startTimeCommands();
	{
		const VkImageMemoryBarrier ImageBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.image = textureImage,
		.subresourceRange{ .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						   .baseMipLevel = 0,
						   .levelCount = mipLevels,
						   .layerCount = 1 } };
		vkCmdPipelineBarrier(cmdBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr,
			0, nullptr, 1,
			&ImageBarrier);

		const VkBufferImageCopy region{
		.imageSubresource{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .mipLevel = 0, .layerCount = 1 },
		.imageOffset = { 0, 0, 0 },
		.imageExtent = { textureWidth, textureHeight, 1 } };
		vkCmdCopyBufferToImage(cmdBuffer, stagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		VkImageMemoryBarrier barrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.image = textureImage,
		.subresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
						 .levelCount = 1,
						 .baseArrayLayer = 0,
						 .layerCount = 1 } };

		for (uint32_t i = 1; i < mipLevels; i++) {
			barrier.subresourceRange.baseMipLevel = i - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			const VkImageBlit imageBlit{
			.srcSubresource{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
							 .mipLevel = i - 1,
							 .baseArrayLayer = 0,
							 .layerCount = 1 },
			.srcOffsets{ { 0, 0, 0 }, { static_cast<int32_t>(textureWidth), static_cast<int32_t>(textureHeight), 1 } },
			.dstSubresource{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
							 .mipLevel = i,
							 .baseArrayLayer = 0,
							 .layerCount = 1 },
			.dstOffsets{ { 0, 0, 0 }, { static_cast<int32_t>(textureWidth) > 1 ? static_cast<int32_t>(textureWidth) / 2 : 1, static_cast<int32_t>(textureHeight) > 1 ? static_cast<int32_t>(textureHeight) / 2 : 1, 1 } } };

			vkCmdBlitImage(cmdBuffer,
				textureImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &imageBlit,
				VK_FILTER_LINEAR);

			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if (static_cast<int32_t>(textureWidth) > 1) textureWidth /= 2;
			if (static_cast<int32_t>(textureHeight) > 1) textureHeight /= 2;
		}

		barrier.subresourceRange.baseMipLevel = mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(cmdBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);
	}
	endTimeCommands();

	textureRead.destroy();
	vmaDestroyBuffer(allocator, stagingBuffer, stagingAllocation);
	stagingBuffer = VK_NULL_HANDLE;
	stagingAllocation = VK_NULL_HANDLE;
}

VkImageView createImageViews(VkImage& image, VkFormat format, VkImageAspectFlags imageAspectFlag, uint32_t mipLevels)
{
	VkImageView imageView;
	const VkImageViewCreateInfo imageViewCI = { .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	.image = image,
	.viewType = VK_IMAGE_VIEW_TYPE_2D,
	.format = format,
	.subresourceRange{.aspectMask = imageAspectFlag,
					  .baseMipLevel = 0,
					  .levelCount = mipLevels,
					  .baseArrayLayer = 0,
					  .layerCount = 1 } };
	vkCreateImageView(device, &imageViewCI, nullptr, &imageView);

	return imageView;
}

void createDepthTestImage()
{
	const VkImageCreateInfo depthImageCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	.imageType = VK_IMAGE_TYPE_2D,
	.format = VK_FORMAT_D24_UNORM_S8_UINT,
	.extent = {Wwidth, Wheight, 1},
	.mipLevels = 1,
	.arrayLayers = 1,
	.samples = samplesCount,
	.tiling = VK_IMAGE_TILING_OPTIMAL,
	.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED };

	const VmaAllocationCreateInfo depthAllocCI{ .usage = VMA_MEMORY_USAGE_AUTO };

	vmaCreateImage(allocator, &depthImageCI, &depthAllocCI, &depthTestImage, &depthImageAlloc, nullptr);
}

void createDepthTestImageView()
{
	depthImageView = createImageViews(depthTestImage, VK_FORMAT_D24_UNORM_S8_UINT, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 1);
}

void createTextureImageView(VkImageView& textureImageView, VkImage& textureImage)
{
	textureImageView = createImageViews(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels);
}

void createSampler(VkSampler& textureSampler)
{
	const VkSamplerCreateInfo samplerCI{ .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
	.magFilter = VK_FILTER_LINEAR,
	.minFilter = VK_FILTER_LINEAR,
	.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
	.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
	.anisotropyEnable = VK_TRUE,
	.maxAnisotropy = 3.0f,
	.compareOp = VK_COMPARE_OP_ALWAYS,
	.minLod = 0.0f,
	.maxLod = static_cast<float>(mipLevels) };

	vkCreateSampler(device, &samplerCI, nullptr, &textureSampler);
}

void createDepthTestImageLayout()
{
	VkCommandBuffer cmdBuffer =
	startTimeCommands();
	{
		const VkImageMemoryBarrier depthImageMemBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		.image = depthTestImage,
		.subresourceRange{ .aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
						   .levelCount = 1,
						   .layerCount = 1 } };
		vkCmdPipelineBarrier(cmdBuffer,
			VK_PIPELINE_STAGE_HOST_BIT | VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			0, 0, nullptr, 
			0, nullptr, 1, 
			&depthImageMemBarrier);
	}
	endTimeCommands();
}

void graphPipeline_create(VkShaderModule vsModule, VkShaderModule fsModule, VkGraphicsPipelineCreateInfo& pipeInfo, VkPipeline& graphPipeline, VkCullModeFlags cull_mode)
{
	const VkPipelineShaderStageCreateInfo pipeVShaderStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.stage = VK_SHADER_STAGE_VERTEX_BIT,
	.module = vsModule,
	.pName = "main" }, pipeFShaderStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
	.module = fsModule,
	.pName = "main" };
	std::vector<VkPipelineShaderStageCreateInfo> pipeShaderStCIs = { pipeVShaderStCI, pipeFShaderStCI };

	const VkPipelineInputAssemblyStateCreateInfo pipeInputAssemblyStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };

	const VkViewport viewport{
	.x = 0.0f,
	.y = 0.0f,
	.width = static_cast<float>(Wwidth),
	.height = static_cast<float>(Wheight),
	.minDepth = 0.0f,
	.maxDepth = 1.0f };

	const VkRect2D scissor{
	.offset = {0, 0},
	.extent = { Wwidth, Wheight } };

	const VkPipelineViewportStateCreateInfo pipeViewportStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	.viewportCount = 1,
	.pViewports = &viewport,
	.scissorCount = 1,
	.pScissors = &scissor };

	const VkPipelineRasterizationStateCreateInfo pipeRasterStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	.polygonMode = VK_POLYGON_MODE_FILL,
	.cullMode = cull_mode,
	.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
	.lineWidth = 1.0f };

	const VkPipelineMultisampleStateCreateInfo pipeMultisamplingStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	.rasterizationSamples = samplesCount };

	const VkPipelineDepthStencilStateCreateInfo pipeDepthStencilStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	.depthTestEnable = VK_TRUE,
	.depthWriteEnable = VK_TRUE,
	.depthCompareOp = VK_COMPARE_OP_LESS };

	const VkPipelineColorBlendAttachmentState pipeColorBlendAttachmentSt{
	.blendEnable = VK_FALSE,
	.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

	const VkPipelineColorBlendStateCreateInfo pipeColorBlendStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	.attachmentCount = 1,
	.pAttachments = &pipeColorBlendAttachmentSt };

	const VkGraphicsPipelineCreateInfo pipelineCI{ .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	.stageCount = static_cast<uint32_t>(pipeShaderStCIs.size()),
	.pStages = pipeShaderStCIs.data(),
	.pVertexInputState = pipeInfo.pVertexInputState,
	.pInputAssemblyState = &pipeInputAssemblyStCI,
	.pViewportState = &pipeViewportStCI,
	.pRasterizationState = &pipeRasterStCI,
	.pMultisampleState = &pipeMultisamplingStCI,
	.pDepthStencilState = &pipeDepthStencilStCI,
	.pColorBlendState = &pipeColorBlendStCI,
	.layout = pipeLayout,
	.renderPass = renPass };

	vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCI, nullptr, &graphPipeline);

	vkDestroyShaderModule(device, vsModule, nullptr);
	vkDestroyShaderModule(device, fsModule, nullptr);
}

void shadowDepthImage_create()
{
	const VkImageCreateInfo shadowDepthImageCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	.imageType = VK_IMAGE_TYPE_2D,
	.format = depthFormat,
	.extent = {2048, 2048, 1},
	.mipLevels = 1,
	.arrayLayers = 1,
	.samples = VK_SAMPLE_COUNT_1_BIT,
	.tiling = VK_IMAGE_TILING_OPTIMAL,
	.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
	.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
	.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED };

	const VmaAllocationCreateInfo allocCI{ .usage = VMA_MEMORY_USAGE_AUTO };

	vmaCreateImage(allocator, &shadowDepthImageCI, &allocCI, &shadowDepthImage, &shadowDepthImageAlloc, nullptr);
}

void shadowDepthImageView_create()
{
	shadowDepthImageView = createImageViews(shadowDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
}

void shadowMapRenPass_craete()
{
	const VkAttachmentDescription attachDsp{
	.format = depthFormat,
	.samples = VK_SAMPLE_COUNT_1_BIT,
	.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
	.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
	.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
	.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

	const VkAttachmentReference attachRef{
	.attachment = 0,
	.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	const VkSubpassDescription subpass{
	.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
	.pDepthStencilAttachment = &attachRef };

	const VkSubpassDependency subDependency{
	.srcSubpass = VK_SUBPASS_EXTERNAL,
	.dstSubpass = 0,
	.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
	.srcAccessMask = VK_ACCESS_SHADER_READ_BIT,
	.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
	.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT }, subDependency1{
	.srcSubpass = 0,
	.dstSubpass = VK_SUBPASS_EXTERNAL,
	.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
	.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
	.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
	.dstAccessMask = VK_ACCESS_SHADER_READ_BIT,
	.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT };
	std::vector<VkSubpassDependency> subDependeices{ subDependency, subDependency1 };

	const VkRenderPassCreateInfo renpassCI{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	.attachmentCount = 1,
	.pAttachments = &attachDsp,
	.subpassCount = 1,
	.pSubpasses = &subpass,
	.dependencyCount = 2,
	.pDependencies = subDependeices.data() };
	vkCreateRenderPass(device, &renpassCI, nullptr, &shadowMapRenPass);
}

void shadowMapFramebuffer_create()
{
	const VkFramebufferCreateInfo framebufferCI{ .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	.renderPass = shadowMapRenPass,
	.attachmentCount = 1,
	.pAttachments = &shadowDepthImageView,
	.width = 2048,
	.height = 2048,
	.layers = 1 };
	vkCreateFramebuffer(device, &framebufferCI, nullptr, &shadowMapFramebuffer);
}

void shadowMapPipelineLayout_create()
{
	VkPushConstantRange pushConstant{ .stageFlags = VK_SHADER_STAGE_VERTEX_BIT, .offset = 0, .size = sizeof(pc)};

	VkPipelineLayoutCreateInfo pipelayoutCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	.pushConstantRangeCount = 1,
	.pPushConstantRanges = &pushConstant };
	vkCreatePipelineLayout(device, &pipelayoutCI, nullptr, &shadowDepthPipelineLayout);
}

void shadowDepthGraphPipeline_create()
{
	createShaderModeles(shadowMapVs, &shadowMapModule);

	VkPipelineShaderStageCreateInfo pipeShaderStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	.stage = VK_SHADER_STAGE_VERTEX_BIT,
	.module = shadowMapModule,
	.pName = "main" };

	const VkVertexInputBindingDescription vertexBindingDsn{
	.binding = 0,
	.stride = sizeof(float) * 3,
	.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

	const VkVertexInputAttributeDescription positionAttributeDsn{
	.location = 0,
	.binding = 0,
	.format = VK_FORMAT_R32G32B32_SFLOAT,
	.offset = 0 };

	const VkPipelineVertexInputStateCreateInfo pipeVertexInputStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	.vertexBindingDescriptionCount = 1,
	.pVertexBindingDescriptions = &vertexBindingDsn,
	.vertexAttributeDescriptionCount = 1,
	.pVertexAttributeDescriptions = &positionAttributeDsn };

	const VkPipelineInputAssemblyStateCreateInfo pipeInputAssemblyStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST };

	const VkViewport viewport{
	.x = 0.0f,
	.y = 0.0f,
	.width = 2048,
	.height = 2048,
	.minDepth = 0.0f,
	.maxDepth = 1.0f };

	const VkRect2D scissor{
	.offset = { 0, 0 },
	.extent = { 2048, 2048 } };

	const VkPipelineViewportStateCreateInfo pipeViewportStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	.viewportCount = 1,
	.pViewports = &viewport,
	.scissorCount = 1,
	.pScissors = &scissor };

	const VkPipelineRasterizationStateCreateInfo pipeRasterStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	.polygonMode = VK_POLYGON_MODE_FILL,
	.cullMode = 0,
	.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
	.lineWidth = 1.0f };

	const VkPipelineMultisampleStateCreateInfo pipeMSAAStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
	.minSampleShading = 1.0f };

	const VkPipelineDepthStencilStateCreateInfo pipeDepthStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	.depthTestEnable = VK_TRUE,
	.depthWriteEnable = VK_TRUE,
	.depthCompareOp = VK_COMPARE_OP_LESS,
	.depthBoundsTestEnable = VK_FALSE,
	.stencilTestEnable = VK_FALSE,
	.minDepthBounds = 0.0f,
	.maxDepthBounds = 1.0f };

	const VkPipelineColorBlendAttachmentState pipeColorBlendAttachSt{
	.blendEnable = VK_FALSE,
	.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT };

	const VkPipelineColorBlendStateCreateInfo pipeColorBlendStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	.attachmentCount = 1,
	.pAttachments = &pipeColorBlendAttachSt };

	const VkGraphicsPipelineCreateInfo pipeCI{ .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	.stageCount = 1,
	.pStages = &pipeShaderStCI,
	.pVertexInputState = &pipeVertexInputStCI,
	.pInputAssemblyState = &pipeInputAssemblyStCI,
	.pViewportState = &pipeViewportStCI,
	.pRasterizationState = &pipeRasterStCI,
	.pMultisampleState = &pipeMSAAStCI,
	.pDepthStencilState = &pipeDepthStCI,
	.pColorBlendState = &pipeColorBlendStCI,
	.layout = shadowDepthPipelineLayout,
	.renderPass = shadowMapRenPass,
	.subpass = 0 };

	vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeCI, nullptr, &shadowDepthPipeline);

	vkDestroyShaderModule(device, shadowMapModule, nullptr);
}

void shadowMapSampler_create()
{
	const VkSamplerCreateInfo samplerCI{ .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
	.magFilter = VK_FILTER_LINEAR,
	.minFilter = VK_FILTER_LINEAR,
	.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
	.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
	.mipLodBias = 0.0f,
	.anisotropyEnable = VK_FALSE,
	.maxAnisotropy = 1.0f,
	.compareEnable = VK_FALSE,
	.compareOp = VK_COMPARE_OP_ALWAYS,
	.minLod = 0.0f,
	.maxLod = 100.0f,
	.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
	.unnormalizedCoordinates = VK_FALSE };
	vkCreateSampler(device, &samplerCI, nullptr, &shadowDepthSampler);
}

void shadowMapRenPass_draw()
{
	Push_constant pc{ .model = model, .lightSpace = lightSpaceMatrix };

	const VkCommandBufferAllocateInfo cmdBufferAI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	.commandPool = cmdPool,
	.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
	.commandBufferCount = 1 };
	vkAllocateCommandBuffers(device, &cmdBufferAI, &cmdBuffer);

	const VkCommandBufferBeginInfo cmdBI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	vkBeginCommandBuffer(cmdBuffer, &cmdBI);
	{
		const VkRenderPassBeginInfo shadowRenpassBI{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = shadowMapRenPass,
		.framebuffer = shadowMapFramebuffer,
		.renderArea = { {0, 0}, {2048, 2048} },
		.clearValueCount = 1,
		.pClearValues = &depthValue };

		vkCmdBeginRenderPass(cmdBuffer, &shadowRenpassBI, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdPushConstants(cmdBuffer, shadowDepthPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pc), &pc);
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer1, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer1, offsets);
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, shadowDepthPipeline);
			vkCmdDrawIndexed(cmdBuffer, kokuraCastle.getIndicesCount(), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(cmdBuffer);
	}
	vkEndCommandBuffer(cmdBuffer);

	const VkSemaphoreCreateInfo semaphoreCI{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
	
	vkCreateSemaphore(device, &semaphoreCI, nullptr, &shadowSemaphore);

	const VkSubmitInfo submitI{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.pWaitDstStageMask = 0,
	.commandBufferCount = 1,
	.pCommandBuffers = &cmdBuffer,
	.signalSemaphoreCount = 1,
	.pSignalSemaphores = &shadowSemaphore };
	vkQueueSubmit(queue, 1, &submitI, nullptr);
}