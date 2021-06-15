#pragma once

namespace ee
{
	class GraphicsDevice : public ThreadSafeManager
	{
	public:
		GraphicsDevice();
		~GraphicsDevice();

	public:
		int NumberOfGraphicsThreads{ 1 };
	};
}
