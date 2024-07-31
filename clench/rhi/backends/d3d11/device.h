#ifndef _CLENCH_RHI_BACKENDS_D3D11_DEVICE_H_
#define _CLENCH_RHI_BACKENDS_D3D11_DEVICE_H_

#include "backend.h"
#include <clench/rhi/device.h>

namespace clench {
	namespace rhi {
		class D3D11RHIDevice : public RHIDevice {
		public:
			D3D11RHIBackend *backend;

			ComPtr<ID3D11Device> d3dDevice;
			ComPtr<ID3D11DeviceContext> d3dImmediateDeviceContext;

			ComPtr<ID3D11Device> d3dDevice1;
			ComPtr<ID3D11DeviceContext1> d3dImmediateDeviceContext1;

			ComPtr<IDXGIDevice> dxgiDevice;
			ComPtr<IDXGIAdapter> dxgiAdapter;
			ComPtr<IDXGIFactory1> dxgiFactory1;
			ComPtr<IDXGIFactory2> dxgiFactory2;

			NO_COPY_MOVE_METHODS(D3D11RHIDevice);

			CLCRHI_API D3D11RHIDevice(
				D3D11RHIBackend *backend,
				ID3D11Device* d3dDevice,
				ID3D11DeviceContext *d3dImmediateDeviceContext,
				IDXGIDevice *dxgiDevice,
				IDXGIAdapter *dxgiAdapter,
				IDXGIFactory1 *dxgiFactory1
			);
			CLCRHI_API D3D11RHIDevice(
				D3D11RHIBackend *backend,
				ID3D11Device1* d3dDevice,
				ID3D11DeviceContext1 *d3dImmediateDeviceContext,
				IDXGIDevice* dxgiDevice,
				IDXGIAdapter* dxgiAdapter,
				IDXGIFactory1* dxgiFactory1,
				IDXGIFactory2* dxgiFactory2
			);
			CLCRHI_API virtual ~D3D11RHIDevice();

			CLCRHI_API virtual RHIBackend* getBackend() override;

			CLCRHI_API virtual RHIDeviceContext* createDeviceContextForWindow(
				clench::wsi::Window* window) override;

			CLCRHI_API virtual VertexArray* createVertexArray(
				VertexArrayElementDesc* elementDescs,
				size_t nElementDescs,
				VertexShader* vertexShader
			) override;

			virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) override;
			virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;
			virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;

			FORCEINLINE bool isD3D11_1Capable() {
				return d3dImmediateDeviceContext1;
			}
		};

		class D3D11RHIDeviceContext : public RHIDeviceContext {
		private:
			CLCRHI_API void _resizeBuffersAndViewport(int width, int height);

		public:
			D3D11RHIDevice *device;

			ComPtr<IDXGISwapChain> swapChain;
			ComPtr<IDXGISwapChain1> swapChain1;

			ComPtr<ID3D11Texture2D> backBuffer;
			ComPtr<ID3D11RenderTargetView> renderTargetView;
			     
			ComPtr<ID3D11Texture2D> depthStencilBuffer;
			ComPtr<ID3D11DepthStencilView> depthStencilView;

			NO_COPY_MOVE_METHODS(D3D11RHIDeviceContext);

			CLCRHI_API D3D11RHIDeviceContext(
				D3D11RHIDevice *device,
				IDXGISwapChain *swapChain);
			CLCRHI_API D3D11RHIDeviceContext(
				D3D11RHIDevice *device,
				IDXGISwapChain1 *swapChain1);
			CLCRHI_API virtual ~D3D11RHIDeviceContext();

			CLCRHI_API virtual void onResize(int width, int height) override;
			CLCRHI_API virtual void clear(float r, float g, float b, float a) override;
			CLCRHI_API virtual void clearDepthBuffer(float depth) override;
			CLCRHI_API virtual void clearStencilBuffer(uint8_t stencil) override;

			CLCRHI_API virtual void begin() override;
			CLCRHI_API virtual void end() override;

			CLCRHI_API virtual void present() override;
		};
	}
}

#endif
