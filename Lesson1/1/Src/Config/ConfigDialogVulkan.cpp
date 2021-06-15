#include "ExceptEnginePCH.h"

using namespace ee;

typedef struct {
	VkLayerProperties properties;
	std::vector<VkExtensionProperties> instance_extensions;
	std::vector<VkExtensionProperties> device_extensions;
} layer_properties;

typedef struct _swap_chain_buffers {
	VkImage image;
	VkImageView view;
} swap_chain_buffer;

struct texture_object {
	VkSampler sampler;

	VkImage image;
	VkImageLayout imageLayout;

	VkDeviceMemory mem;
	VkImageView view;
	int32_t tex_width, tex_height;
};

struct sample_info {
#define APP_NAME_STR_LEN 80
	HINSTANCE connection;        // hInstance - Windows Instance
	char name[APP_NAME_STR_LEN]; // Name to put on the window/icon
	HWND window;                 // hWnd - window handle
	VkSurfaceKHR surface;
	bool prepared;
	bool use_staging_buffer;
	bool save_images;

	std::vector<const char *> instance_layer_names;
	std::vector<const char *> instance_extension_names;
	std::vector<layer_properties> instance_layer_properties;
	std::vector<VkExtensionProperties> instance_extension_properties;
	VkInstance inst;

	std::vector<const char *> device_extension_names;
	std::vector<VkExtensionProperties> device_extension_properties;
	std::vector<VkPhysicalDevice> gpus;
	VkDevice device;
	VkQueue graphics_queue;
	VkQueue present_queue;
	uint32_t graphics_queue_family_index;
	uint32_t present_queue_family_index;
	VkPhysicalDeviceProperties gpu_props;
	std::vector<VkQueueFamilyProperties> queue_props;
	VkPhysicalDeviceMemoryProperties memory_properties;

	VkFramebuffer *framebuffers;
	int width, height;
	VkFormat format;

	uint32_t swapchainImageCount;
	VkSwapchainKHR swap_chain;
	std::vector<swap_chain_buffer> buffers;
	VkSemaphore imageAcquiredSemaphore;

	VkCommandPool cmd_pool;

	struct {
		VkFormat format;

		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	} depth;

	std::vector<texture_object> textures;

	struct {
		VkBuffer buf;
		VkDeviceMemory mem;
		VkDescriptorBufferInfo buffer_info;
	} uniform_data;

	struct {
		VkDescriptorImageInfo image_info;
	} texture_data;
	VkDeviceMemory stagingMemory;
	VkImage stagingImage;

	struct {
		VkBuffer buf;
		VkDeviceMemory mem;
		VkDescriptorBufferInfo buffer_info;
	} vertex_buffer;
	VkVertexInputBindingDescription vi_binding;
	VkVertexInputAttributeDescription vi_attribs[2];

	VkCommandBuffer cmd; // Buffer for initialization commands
	VkPipelineLayout pipeline_layout;
	std::vector<VkDescriptorSetLayout> desc_layout;
	VkPipelineCache pipelineCache;
	VkRenderPass render_pass;
	VkPipeline pipeline;

	VkPipelineShaderStageCreateInfo shaderStages[2];

	VkDescriptorPool desc_pool;
	std::vector<VkDescriptorSet> desc_set;

	PFN_vkCreateDebugReportCallbackEXT dbgCreateDebugReportCallback;
	PFN_vkDestroyDebugReportCallbackEXT dbgDestroyDebugReportCallback;
	PFN_vkDebugReportMessageEXT dbgBreakCallback;
	std::vector<VkDebugReportCallbackEXT> debug_report_callbacks;

	uint32_t current_buffer;
	uint32_t queue_family_count;

	VkViewport viewport;
	VkRect2D scissor;
};

VkResult init_global_extension_properties(layer_properties &layer_props) {
	VkExtensionProperties *instance_extensions;
	uint32_t instance_extension_count;
	VkResult res;
	char *layer_name = NULL;

	layer_name = layer_props.properties.layerName;

	do {
		res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, NULL);
		if (res) return res;

		if (instance_extension_count == 0) {
			return VK_SUCCESS;
		}

		layer_props.instance_extensions.resize(instance_extension_count);
		instance_extensions = layer_props.instance_extensions.data();
		res = vkEnumerateInstanceExtensionProperties(layer_name, &instance_extension_count, instance_extensions);
	} while (res == VK_INCOMPLETE);

	return res;
}

VkResult init_global_layer_properties(struct sample_info &info) {
	uint32_t instance_layer_count;
	VkLayerProperties *vk_props = NULL;
	VkResult res;

	/*
	* It's possible, though very rare, that the number of
	* instance layers could change. For example, installing something
	* could include new layers that the loader would pick up
	* between the initial query for the count and the
	* request for VkLayerProperties. The loader indicates that
	* by returning a VK_INCOMPLETE status and will update the
	* the count parameter.
	* The count parameter will be updated with the number of
	* entries loaded into the data pointer - in case the number
	* of layers went down or is smaller than the size given.
	*/
	do {
		res = vkEnumerateInstanceLayerProperties(&instance_layer_count, NULL);
		if (res) return res;

		if (instance_layer_count == 0) {
			return VK_SUCCESS;
		}

		vk_props = (VkLayerProperties *)realloc(vk_props, instance_layer_count * sizeof(VkLayerProperties));

		res = vkEnumerateInstanceLayerProperties(&instance_layer_count, vk_props);
	} while (res == VK_INCOMPLETE);

	/*
	* Now gather the extension list for each instance layer.
	*/
	for (uint32_t i = 0; i < instance_layer_count; i++) {
		layer_properties layer_props;
		layer_props.properties = vk_props[i];
		res = init_global_extension_properties(layer_props);
		if (res) return res;
		info.instance_layer_properties.push_back(layer_props);
	}
	free(vk_props);

	return res;
}

VkResult init_instance(struct sample_info &info, char const *const app_short_name) {
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = app_short_name;
	app_info.applicationVersion = 1;
	app_info.pEngineName = app_short_name;
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledLayerCount = (uint32_t)info.instance_layer_names.size();
	inst_info.ppEnabledLayerNames = info.instance_layer_names.size() ? info.instance_layer_names.data() : NULL;
	inst_info.enabledExtensionCount = (uint32_t)info.instance_extension_names.size();
	inst_info.ppEnabledExtensionNames = info.instance_extension_names.data();

	VkResult res = vkCreateInstance(&inst_info, NULL, &info.inst);
	assert(res == VK_SUCCESS);

	return res;
}

void VulkanTest::Test1()
{
	uint32_t display_count;
	uint32_t mode_count;
	uint32_t plane_count;
	VkDisplayPropertiesKHR display_props;
	VkDisplayKHR display;
	VkDisplayModePropertiesKHR mode_props;
	VkDisplayPlanePropertiesKHR *plane_props;
	VkBool32 found_plane = VK_FALSE;
	uint32_t plane_index;
	VkResult err;

	struct sample_info info = {};
	init_global_layer_properties(info);
	init_instance(info, "vulkansamples_enumerate");

	uint32_t gpu_count = 0;
	VkResult res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, NULL);
	assert(!res && gpu_count > 0);

	VkPhysicalDevice *gpu = new VkPhysicalDevice[gpu_count];
	res = vkEnumeratePhysicalDevices(info.inst, &gpu_count, gpu);
	assert(res == VK_SUCCESS);

	for (uint32_t i = 0; i < gpu_count; ++i) {
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(gpu[i], &properties);

		std::stringstream ret;
		ret << "apiVersion: ";
		ret << ((properties.apiVersion >> 22) & 0xfff) << '.';  // Major.
		ret << ((properties.apiVersion >> 12) & 0x3ff) << '.';  // Minor.
		ret << (properties.apiVersion & 0xfff);                 // Patch.
		ret << '\n';

		ret << "driverVersion: " << properties.driverVersion << '\n';

		ret << std::showbase << std::internal << std::setfill('0') << std::hex;
		ret << "vendorId: " << std::setw(6) << properties.vendorID << '\n';
		ret << "deviceId: " << std::setw(6) << properties.deviceID << '\n';
		ret << std::noshowbase << std::right << std::setfill(' ') << std::dec;

		ret << "deviceType: ";
		switch (properties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			ret << "VK_PHYSICAL_DEVICE_TYPE_OTHER";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			ret << "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			ret << "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			ret << "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			ret << "VK_PHYSICAL_DEVICE_TYPE_CPU";
			break;
		default:
			break;
		}
		ret << '\n';

		ret << "deviceName: " << properties.deviceName << '\n';

		ret << "pipelineCacheUUID: ";
		ret << std::setfill('0') << std::hex;

		for (int j = 0; j < VK_UUID_SIZE; ++j) {
			ret << std::setw(2) << (uint32_t)properties.pipelineCacheUUID[j];
			if (j == 3 || j == 5 || j == 7 || j == 9) {
				ret << '-';
			}
		}

		ret << std::setfill(' ') << std::dec;
		ret << '\n';
		ret << '\n';

		LOG.Write(L"%s", String::MultibyteToUnicode(ret.str()).c_str());

		err = vkGetPhysicalDeviceDisplayPropertiesKHR(gpu[i], &display_count, NULL);
		assert(!err);

		if (display_count == 0) {
			LOG.Write(L"Cannot find any display!\n");
			break;
		}

		display_count = 1;
		err = vkGetPhysicalDeviceDisplayPropertiesKHR(gpu[i], &display_count, &display_props);
		assert(!err || (err == VK_INCOMPLETE));

		LOG.Write(L"Found %d displays!\n", display_count);

		display = display_props.display;

		err = vkGetDisplayModePropertiesKHR(gpu[i], display, &mode_count, NULL);
		assert(!err);

		if (mode_count == 0) {
			LOG.Write(L"Cannot find any mode for the display!\n");
			break;
		}

		mode_count = 1;
		err = vkGetDisplayModePropertiesKHR(gpu[i], display, &mode_count, &mode_props);
		assert(!err || (err == VK_INCOMPLETE));

		LOG.Write(L"Found %d modes!\n", mode_count);

		err = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(gpu[i], &plane_count, NULL);
		assert(!err);

		if (plane_count == 0) {
			LOG.Write(L"Cannot find any plane!\n");
			break;
		}

		LOG.Write(L"Found %d planes!\n", plane_count);

		plane_props = (VkDisplayPlanePropertiesKHR*)malloc(sizeof(VkDisplayPlanePropertiesKHR) * plane_count);
		assert(plane_props);

		err = vkGetPhysicalDeviceDisplayPlanePropertiesKHR(gpu[i], &plane_count, plane_props);
		assert(!err);

		for (plane_index = 0; plane_index < plane_count; plane_index++) {
			uint32_t supported_count;
			VkDisplayKHR *supported_displays;

			// Disqualify planes that are bound to a different display
			if ((plane_props[plane_index].currentDisplay != VK_NULL_HANDLE) && (plane_props[plane_index].currentDisplay != display)) {
				continue;
			}

			err = vkGetDisplayPlaneSupportedDisplaysKHR(gpu[i], plane_index, &supported_count, NULL);
			assert(!err);

			if (supported_count == 0) {
				continue;
			}

			supported_displays = (VkDisplayKHR*)malloc(sizeof(VkDisplayKHR) * supported_count);
			assert(supported_displays);

			err = vkGetDisplayPlaneSupportedDisplaysKHR(gpu[i], plane_index, &supported_count, supported_displays);
			assert(!err);

			for (uint32_t i = 0; i < supported_count; i++) {
				if (supported_displays[i] == display) {
					found_plane = VK_TRUE;
					break;
				}
			}

			free(supported_displays);

			if (found_plane) {
				break;
			}
		}

		if (!found_plane) {
			LOG.Write(L"Cannot find a plane compatible with the display!\n");
			free(plane_props);
			break;
		}

		free(plane_props);

	}

	delete[] gpu;

	/* VULKAN_KEY_END */

	vkDestroyInstance(info.inst, NULL);

}

