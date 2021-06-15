#pragma once

namespace ee
{
	class GraphicsDeviceD3D12 : public GraphicsDevice
	{
	public:
		GraphicsDeviceD3D12(bool debuglayer = false);
		~GraphicsDeviceD3D12();

	};
}

