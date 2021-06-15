#pragma once

namespace ee
{
	class GraphicsDeviceD3D11 : public GraphicsDevice
	{
	public:
		GraphicsDeviceD3D11(bool debuglayer = false);
		~GraphicsDeviceD3D11();

	private:
		std::vector<ID3D11DeviceContext*>	deviceContexts;
		std::vector<ID3D11CommandList*>		commandLists;
		std::vector<UINT>					stencilRef;
		std::vector<DirectX::XMFLOAT4>		blendFactor;

	};
}

