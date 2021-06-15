#pragma once

namespace ee
{
	class GraphicsDeviceVulkan : public GraphicsDevice
	{
	public:
		GraphicsDeviceVulkan(bool debuglayer = false);
		~GraphicsDeviceVulkan();

	};

}

