#include "Source/initVulkan.h"
#include "Control.h"

void updateMVP()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
	glm::mat4 model1 = glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(470.0f)), glm::radians(1.0f) * static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.0f));
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::mat4 view1 = glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(Wwidth) / Wheight, 0.1f, 500.0f);
	projection[1][1] *= -1;

	uniformBufferObject0 ubo0{
	.model = model,
	.view = view,
	.proj = projection,
	.model1 = model1,
	.view1 = view1,
	.lightSpace = lightSpaceMatrix,
	.lightPos = glm::rotate(glm::mat4(1.0f), glm::radians(40.0f) * static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(90.0f, -20.0f, 90.0f, 0.0f) } ;
	memcpy(uniformBufferAllocInfo0.pMappedData, &ubo0, sizeof(ubo0));
}

void recordDrawCmdBuffer(uint32_t imageIndex)
{
	const VkCommandBufferBeginInfo beginInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
	vkBeginCommandBuffer(cmdBuffer, &beginInfo);
	{
		colorValue = { 0.0f, 0.0094f, 0.0656f, 1.0f };
		std::vector<VkClearValue> clearValues = { colorValue, depthValue };
		const VkRenderPassBeginInfo renPassBI{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = renPass,
		.framebuffer = frameBuffers[imageIndex],
		.renderArea{.offset = {0, 0}, .extent = {Wwidth, Wheight} },
		.clearValueCount = static_cast<uint32_t>(clearValues.size()),
		.pClearValues = clearValues.data() };

		VkRenderPassBeginInfo shadowRenpassBI{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = shadowMapRenPass,
		.framebuffer = shadowMapFramebuffer,
		.renderArea = { {0, 0}, {2048, 2048} },
		.clearValueCount = 1,
		.pClearValues = &depthValue };

		vkCmdBeginRenderPass(cmdBuffer, &renPassBI, VK_SUBPASS_CONTENTS_INLINE);
		{
			vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLayout, 0, 1, &dstSet, 0, nullptr);
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, offsets);
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphPipeline0);
			vkCmdDrawIndexed(cmdBuffer, misato.getIndicesCount(), 1, 0, 0, 0);
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer1, 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindVertexBuffers(cmdBuffer, 1, 1, &vertexBuffer1, offsets);
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphPipeline1);
			vkCmdDrawIndexed(cmdBuffer, kokuraCastle.getIndicesCount(), 1, 0, 0, 0);
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer2, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindVertexBuffers(cmdBuffer, 2, 1, &vertexBuffer2, offsets);
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphPipeline2);
			vkCmdDrawIndexed(cmdBuffer, sphere.getIndicesCount(), 1, 0, 0, 0);
			vkCmdBindIndexBuffer(cmdBuffer, indexBuffer3, 0, VK_INDEX_TYPE_UINT16);
			vkCmdBindVertexBuffers(cmdBuffer, 3, 1, &vertexBuffer3, offsets);
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphPipeline3);
			vkCmdDrawIndexed(cmdBuffer, cubeMap.getIndicesCount(), 1, 0, 0, 0);
		}
		vkCmdEndRenderPass(cmdBuffer);
	}
	vkEndCommandBuffer(cmdBuffer);
}
void presentFrame()
{
	vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &fence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, waitSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(cmdBuffer, 0);
	recordDrawCmdBuffer(imageIndex);

	VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	const VkSubmitInfo submitInfo{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = &waitSemaphore,
	.pWaitDstStageMask = &dstStageMask,
	.commandBufferCount = 1,
	.pCommandBuffers = &cmdBuffer,
	.signalSemaphoreCount = 1,
	.pSignalSemaphores = &signalSemaphore,
	};
	vkQueueSubmit(queue, 1, &submitInfo, fence);

	const VkPresentInfoKHR presentInfo{ .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
	.waitSemaphoreCount = 1,
	.pWaitSemaphores = &signalSemaphore,
	.swapchainCount = 1,
	.pSwapchains = &swapchain,
	.pImageIndices = &imageIndex };
	vkQueuePresentKHR(queue, &presentInfo);
}

void loop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		control();
		updateMVP();
		presentFrame();
	}
}