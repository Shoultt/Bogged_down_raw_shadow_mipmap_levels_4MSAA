#pragma once
#include "Source/VulkanMonotony.h"

class initVK
{
public:
	void InitVulkan()
	{
		instance_create();
		physDevice_enum();
		device_create();
		surface_create();
		swapchain_create();
		renderPass_create();
		DescriptorSetUboLayout_create();
		graphPipelineLayout_create();
		graphPipilines_create();
		cmdPool_create();
		load_and_create_resources();
		msaa_create();
		depthTest_create();
		dstPool_create();
		dstSets_allocate();
		cmdBuffers_allocate();
		FrameBuffers_create();
		synchrone_create();
	}
	void load_and_create_resources()
	{
		createAllocator();
		indexBuffers_create();
		vertexBuffers_create();
		uniformBuffers_create();
		createTexture();
		shadowImage_create();
	}
private:
	void instance_create()
	{
		const VkApplicationInfo appInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Platformer",
		.applicationVersion = VK_MAKE_VERSION(0, 0, 1),
		.pEngineName = "Elf",
		.engineVersion = VK_MAKE_VERSION(0, 0, 1),
		.apiVersion = VK_API_VERSION_1_1 };

		const VkInstanceCreateInfo instanceCI{ .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = static_cast<uint32_t>(validLayer.size()),
		.ppEnabledLayerNames = validLayer.data(),
		.enabledExtensionCount = static_cast<uint32_t>(ExtsIns.size()),
		.ppEnabledExtensionNames = ExtsIns.data() };

		vkCreateInstance(&instanceCI, nullptr, &instance);
	}

	void physDevice_enum()
	{
		uint32_t physDeviceCount = 1;
		std::vector<VkPhysicalDevice> physDevices(physDeviceCount);
		vkEnumeratePhysicalDevices(instance, &physDeviceCount, physDevices.data());
		physDevice = physDevices[0];
	}

	void device_create()
	{
		float prior[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		VkDeviceQueueCreateInfo deviceQueueCI{ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueFamilyIndex = 0,
		.queueCount = 1,
		.pQueuePriorities = prior };

		VkPhysicalDeviceFeatures usedFeatures{ .samplerAnisotropy = VK_TRUE };

		VkDeviceCreateInfo DeviceCI{ .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &deviceQueueCI,
		.enabledExtensionCount = static_cast<uint32_t>(ExtDev.size()),
		.ppEnabledExtensionNames = ExtDev.data(),
		.pEnabledFeatures = &usedFeatures };
		vkCreateDevice(physDevice, &DeviceCI, nullptr, &device);

		vkGetDeviceQueue(device, 0, 0, &queue);
	}

	void surface_create()
	{
		glfwCreateWindowSurface(instance, window, nullptr, &surface);
		uint32_t count = 1;
		VkSurfaceFormatKHR surfaceFormat;
		vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &count, &surfaceFormat);
		rendering_format = surfaceFormat.format;
	}

	void swapchain_create()
	{
		const VkSwapchainCreateInfoKHR swapCI{ .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = 2,
		.imageFormat = rendering_format,
		.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		.imageExtent = {Wwidth, Wheight},
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR };

		vkCreateSwapchainKHR(device, &swapCI, nullptr, &swapchain);

		uint32_t swapchainImageCount;
		vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
		swapImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapImages.data());

		swapImageViews.resize(swapchainImageCount);

		for (uint32_t i = 0; i < swapchainImageCount; i++)
		{
			swapImageViews[i] = createImageViews(swapImages[i], rendering_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void renderPass_create()
	{
		const VkAttachmentDescription colorAttachDsp{
		.format = VK_FORMAT_B8G8R8A8_UNORM,
		.samples = samplesCount,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL },
		depthAttachDsp{
		.format = VK_FORMAT_D24_UNORM_S8_UINT,
		.samples = samplesCount,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL }, msaaAttechmentResolve{
		.format = rendering_format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR };
		const std::vector<VkAttachmentDescription> AttachDsps = { colorAttachDsp, depthAttachDsp, msaaAttechmentResolve };

		const VkAttachmentReference colorAttachReference{ .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }, 
		depthAttachReference{ .attachment = 1, .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL },
		msaaAttachResolveReference{ .attachment = 2, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		const VkSubpassDependency subDependency{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		.dependencyFlags = 0 };

		const VkSubpassDescription subpass{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachReference,
		.pResolveAttachments = &msaaAttachResolveReference,
		.pDepthStencilAttachment = &depthAttachReference };

		const VkRenderPassCreateInfo renPassCI{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = static_cast<uint32_t>(AttachDsps.size()),
		.pAttachments = AttachDsps.data(),
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 1,
		.pDependencies = &subDependency };

		vkCreateRenderPass(device, &renPassCI, nullptr, &renPass);
	}

	void DescriptorSetUboLayout_create()
	{
		const VkDescriptorSetLayoutBinding dstSetLayoutBindingUniform0{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT }, dstSetLayoutBindingSampler{
		.binding = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT }, dstSetLayoutBindingSampler1{
		.binding = 2,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT }, dstSetLayoutBindingSampler2{
		.binding = 3,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT }, dstSetLayoutBindingSampler3{
		.binding = 4,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT };
		const std::vector<VkDescriptorSetLayoutBinding> dstSetLayoutBindings = { dstSetLayoutBindingUniform0, dstSetLayoutBindingSampler, dstSetLayoutBindingSampler1, dstSetLayoutBindingSampler2, dstSetLayoutBindingSampler3 };

		const VkDescriptorSetLayoutCreateInfo descriptSetLayoutCI{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = static_cast<uint32_t>(dstSetLayoutBindings.size()),
		.pBindings = dstSetLayoutBindings.data()};
		vkCreateDescriptorSetLayout(device, &descriptSetLayoutCI, nullptr, &descriptSetLayout);
	}

	void graphPipelineLayout_create()
	{
		VkPipelineLayoutCreateInfo pipeLayoutCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptSetLayout };
		vkCreatePipelineLayout(device, &pipeLayoutCI, nullptr, &pipeLayout);
	}

	void graphPipilines_create()
	{
		createShaderModeles(vsCode, &vsModule);
		createShaderModeles(fsCode, &fsModule);

		const VkVertexInputBindingDescription vertexBindingDsn{
		.binding = 0,
		.stride = sizeof(float) * 8,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		const VkVertexInputAttributeDescription positionAttributeDsn{
		.location = 0,
		.binding = 0,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0 }, normalAttributeDsn{
		.location = 1,
		.binding = 0,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = sizeof(float) * 3}, texCoordAttributeDsn{
		.location = 2,
		.binding = 0,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = sizeof(float) * 6 };
		const std::vector<VkVertexInputAttributeDescription> vertexAttributeDsns = { positionAttributeDsn, normalAttributeDsn, texCoordAttributeDsn };

		const VkPipelineVertexInputStateCreateInfo pipeVertexInputStCI{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDsn,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDsns.size()),
		.pVertexAttributeDescriptions = vertexAttributeDsns.data() };

		VkGraphicsPipelineCreateInfo pipeCI{
		.pVertexInputState = &pipeVertexInputStCI };
		graphPipeline_create(vsModule, fsModule, pipeCI, graphPipeline0, VK_CULL_MODE_BACK_BIT);

		createShaderModeles(polyVSCode, &polyVSmodule);
		createShaderModeles(polyFSCode, &polyFSmodule);

		const VkVertexInputBindingDescription vertexBindingDsn1{
		.binding = 1,
		.stride = sizeof(float) * 8,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		const VkVertexInputAttributeDescription vertexAttributeDsn1{
		.location = 0,
		.binding = 1,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0 }, normalAttributeDsn1{
		.location = 1,
		.binding = 1,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = sizeof(float) * 3 }, texCoordAttributeDsn1{
		.location = 2,
		.binding = 1,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = sizeof(float) * 6 };
		const std::vector<VkVertexInputAttributeDescription> vertexAttributeDsns1 = { vertexAttributeDsn1, normalAttributeDsn1, texCoordAttributeDsn1 };

		const VkPipelineVertexInputStateCreateInfo pipeVertexInputStCI1{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDsn1,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDsns1.size()),
		.pVertexAttributeDescriptions = vertexAttributeDsns1.data() };

		VkGraphicsPipelineCreateInfo pipeCI1{
		.pVertexInputState = &pipeVertexInputStCI1 };
		graphPipeline_create(polyVSmodule, polyFSmodule, pipeCI1, graphPipeline1, 0);

		createShaderModeles(sphereVSCode, &sphereVSmodule);
		createShaderModeles(sphereFSCode, &sphereFSmodule);

		const VkVertexInputBindingDescription vertexBindingDsn2{
		.binding = 2,
		.stride = sizeof(float) * 3,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		const VkVertexInputAttributeDescription vertexAttributeDsn2{
		.location = 0,
		.binding = 2,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0 };
		const std::vector<VkVertexInputAttributeDescription> vertexAttributeDsns2 = { vertexAttributeDsn2 };

		const VkPipelineVertexInputStateCreateInfo pipeVertexInputStCI2{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDsn2,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDsns2.size()),
		.pVertexAttributeDescriptions = vertexAttributeDsns2.data() };

		VkGraphicsPipelineCreateInfo pipeCI2{
		.pVertexInputState = &pipeVertexInputStCI2 };
		graphPipeline_create(sphereVSmodule, sphereFSmodule, pipeCI2, graphPipeline2, VK_CULL_MODE_BACK_BIT);

		createShaderModeles(cubeMapVSCode, &cubeMapVSmodule);
		createShaderModeles(cubeMapFSCode, &cubeMapFSmodule);

		const VkVertexInputBindingDescription vertexBindingDsn3{
		.binding = 3,
		.stride = sizeof(float) * 5,
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX };

		const VkVertexInputAttributeDescription vertexAttributeDsn3{
		.location = 0,
		.binding = 3,
		.format = VK_FORMAT_R32G32B32_SFLOAT,
		.offset = 0 }, texCoordAttributeDsn3{
		.location = 1,
		.binding = 3,
		.format = VK_FORMAT_R32G32_SFLOAT,
		.offset = sizeof(float) * 3 };;
		const std::vector<VkVertexInputAttributeDescription> vertexAttributeDsns3 = { vertexAttributeDsn3, texCoordAttributeDsn3 };

		const VkPipelineVertexInputStateCreateInfo pipeVertexInputStCI3{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDsn3,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDsns3.size()),
		.pVertexAttributeDescriptions = vertexAttributeDsns3.data()};

		VkGraphicsPipelineCreateInfo pipeCI3{
		.pVertexInputState = &pipeVertexInputStCI3 };
		graphPipeline_create(cubeMapVSmodule, cubeMapFSmodule, pipeCI3, graphPipeline3, VK_CULL_MODE_FRONT_BIT);
	}
	
	void cmdPool_create()
	{
		const VkCommandPoolCreateInfo cmdPoolCI{ .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = 0 };

		vkCreateCommandPool(device, &cmdPoolCI, nullptr, &cmdPool);
	}

	void depthTest_create()
	{
		createDepthTestImage();
		createDepthTestImageView();
		createDepthTestImageLayout();
	}

	void msaa_create()
	{
		const VmaAllocationCreateInfo msaaAllocCI{ .usage = VMA_MEMORY_USAGE_AUTO };

		const VkImageCreateInfo msaaImageCI{ .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = rendering_format,
		.extent = {Wwidth, Wheight, 1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.samples = samplesCount,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.usage = VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED };

		vmaCreateImage(allocator, &msaaImageCI, &msaaAllocCI, &msaaImage, &msaaImageAlloc, nullptr);

		msaaImageView = createImageViews(msaaImage, rendering_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void indexBuffers_create()
	{
		misato.load("Models/ray2.glb", true, true, 8);
		kokuraCastle.load("Models/jap.glb", true, true, 8);
		sphere.load("Models/sphere.glb", false, false, 3);
		cubeMap.load("Models/cubeMap.glb", false, true, 5);
		createBuffer(indexBuffer, misato.getIndexSize(), misato.getIndexData(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBufferAlloc);
		createBuffer(indexBuffer1, kokuraCastle.getIndexSize(), kokuraCastle.getIndexData(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBufferAlloc1);
		createBuffer(indexBuffer2, sphere.getIndexSize(), sphere.getIndexData(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBufferAlloc2);
		createBuffer(indexBuffer3, cubeMap.getIndexSize(), cubeMap.getIndexData(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, indexBufferAlloc3);
	}

	void vertexBuffers_create()
	{
		createBuffer(vertexBuffer, misato.getVertexSize(), misato.getVertexData(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferAlloc);
		createBuffer(vertexBuffer1, kokuraCastle.getVertexSize(), kokuraCastle.getVertexData(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferAlloc1);
		createBuffer(vertexBuffer2, sphere.getVertexSize(), sphere.getVertexData(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferAlloc2);
		createBuffer(vertexBuffer3, cubeMap.getVertexSize(), cubeMap.getVertexData(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, vertexBufferAlloc3);
		misato.destroyModel();
		kokuraCastle.destroyModel();
		sphere.destroyModel();
		cubeMap.destroyModel();
	}

	void uniformBuffers_create()
	{
		VkDeviceSize uniformBufferSize0 = sizeof(ubo0);
		createUnibuffer(uniformBuffer0, uniformBufferSize0, uniformBufferAlloc0, uniformBufferAllocInfo0);
	}

	void createTexture()
	{
		uploadTexture("Textures/tex.jpg", textureImage, textureImageAlloc);
		createTextureImageView(textureImageView, textureImage);
		createSampler(textureSampler);
		uploadTexture("Textures/jap.jpg", textureImage1, textureImageAlloc1);
		createTextureImageView(textureImageView1, textureImage1);
		createSampler(textureSampler1);
		uploadTexture("Textures/ty.jpg", textureImage3, textureImageAlloc3);
		createTextureImageView(textureImageView3, textureImage3);
		createSampler(textureSampler3);
	}

	void shadowImage_create()
	{
		shadowDepthImage_create();
		shadowDepthImageView_create();
		shadowMapSampler_create();
		shadowMapRenPass_craete();
		shadowMapPipelineLayout_create();
		shadowDepthGraphPipeline_create();
		shadowMapFramebuffer_create();
		shadowMapRenPass_draw();
	}

	void dstPool_create()
	{
		const VkDescriptorPoolSize dstPoolSize1{
		.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1 }, dstPoolSize2{
		.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1};
		const VkDescriptorPoolSize dstPoolSizes[] = { dstPoolSize1, dstPoolSize2};

		const VkDescriptorPoolCreateInfo descriptPoolCI{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.maxSets = 1,
		.poolSizeCount = 2,
		.pPoolSizes = dstPoolSizes };

		vkCreateDescriptorPool(device, &descriptPoolCI, nullptr, &descriptPool);
	}

	void dstSets_allocate()
	{
		const VkDescriptorSetAllocateInfo dstSetAI{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = descriptPool,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptSetLayout };
		vkAllocateDescriptorSets(device, &dstSetAI, &dstSet);

		const VkDescriptorBufferInfo dstBIuniform0{
		.buffer = uniformBuffer0,
		.offset = 0,
		.range = sizeof(ubo0) };

		const VkDescriptorImageInfo dstIIsampler{
		.sampler = textureSampler,
		.imageView = textureImageView,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, dstIIsampler1{
		.sampler = textureSampler1,
		.imageView = textureImageView1,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, dstIIsampler2{
		.sampler = shadowDepthSampler,
		.imageView = shadowDepthImageView,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL }, dstIIsampler3{
		.sampler = textureSampler3,
		.imageView = textureImageView3,
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

		const std::vector<VkDescriptorImageInfo> dstIIsamplers = { dstIIsampler, dstIIsampler1, dstIIsampler2, dstIIsampler3 };
		const VkWriteDescriptorSet dstSetWriteUniformBuffer0{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = dstSet,
		.dstBinding = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.pBufferInfo = &dstBIuniform0 }, dstSetWriteSampler{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = dstSet,
		.dstBinding = 1,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &dstIIsampler }, dstSetWriteSampler1{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = dstSet,
		.dstBinding = 2,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &dstIIsampler1 }, dstSetWriteSampler2{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = dstSet,
		.dstBinding = 3,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &dstIIsampler2 }, dstSetWriteSampler3{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = dstSet,
		.dstBinding = 4,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImageInfo = &dstIIsampler3 };

		const std::vector<VkWriteDescriptorSet> dstSetWrites = { dstSetWriteUniformBuffer0, dstSetWriteSampler, dstSetWriteSampler1, dstSetWriteSampler2, dstSetWriteSampler3 };

		vkUpdateDescriptorSets(device, static_cast<uint32_t>(dstSetWrites.size()), dstSetWrites.data(), 0, nullptr);
	}

	void cmdBuffers_allocate()
	{
		const VkCommandBufferAllocateInfo cmdBufferAI{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = cmdPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1 };

		vkAllocateCommandBuffers(device, &cmdBufferAI, &cmdBuffer);
	}

	void FrameBuffers_create()
	{
		frameBuffers.resize(swapImageViews.size());
		for (int i = 0; i < swapImageViews.size(); i++)
		{
			std::vector<VkImageView> attachmentImages;
			attachmentImages.push_back(msaaImageView);
			attachmentImages.push_back(depthImageView);
			attachmentImages.push_back(swapImageViews[i]);

			const VkFramebufferCreateInfo fbci{ .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = renPass,
			.attachmentCount = static_cast<uint32_t>(attachmentImages.size()),
			.pAttachments = attachmentImages.data(),
			.width = Wwidth,
			.height = Wheight,
			.layers = 1 };

			vkCreateFramebuffer(device, &fbci, nullptr, &frameBuffers[i]);
		}
	}

	void synchrone_create()
	{
		const VkSemaphoreCreateInfo sci{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		vkCreateSemaphore(device, &sci, nullptr, &waitSemaphore);
		vkCreateSemaphore(device, &sci, nullptr, &signalSemaphore);

		const VkFenceCreateInfo fci{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT };
		vkCreateFence(device, &fci, nullptr, &fence);
	}
};