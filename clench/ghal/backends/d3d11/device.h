#ifndef _CLENCH_GHAL_BACKENDS_D3D11_DEVICE_H_
#define _CLENCH_GHAL_BACKENDS_D3D11_DEVICE_H_

#include "backend.h"
#include "views.h"
#include "buffer.h"
#include <clench/ghal/device.h>

namespace clench {
	namespace ghal {
		class D3D11Device : public Device {
		public:
			D3D11Backend *backend;

			ComPtr<ID3D11Device> d3dDevice;
			ComPtr<ID3D11DeviceContext> d3dImmediateDeviceContext;

			ComPtr<ID3D11Device> d3dDevice1;
			ComPtr<ID3D11DeviceContext1> d3dImmediateDeviceContext1;

			ComPtr<IDXGIDevice> dxgiDevice;
			ComPtr<IDXGIAdapter> dxgiAdapter;
			ComPtr<IDXGIFactory1> dxgiFactory1;
			ComPtr<IDXGIFactory2> dxgiFactory2;

			CLENCH_NO_COPY_MOVE_METHODS(D3D11Device);

			CLCGHAL_API D3D11Device(
				D3D11Backend *backend,
				ID3D11Device *d3dDevice,
				ID3D11DeviceContext *d3dImmediateDeviceContext,
				IDXGIDevice *dxgiDevice,
				IDXGIAdapter *dxgiAdapter,
				IDXGIFactory1 *dxgiFactory1);
			CLCGHAL_API D3D11Device(
				D3D11Backend *backend,
				ID3D11Device *d3dDevice,
				ID3D11Device1 *d3dDevice1,
				ID3D11DeviceContext *d3dImmediateDeviceContext,
				ID3D11DeviceContext1 *d3dImmediateDeviceContext1,
				IDXGIDevice *dxgiDevice,
				IDXGIAdapter *dxgiAdapter,
				IDXGIFactory1 *dxgiFactory1,
				IDXGIFactory2 *dxgiFactory2);
			CLCGHAL_API virtual ~D3D11Device();

			CLCGHAL_API virtual Backend *getBackend() override;

			CLCGHAL_API virtual DeviceContext *createDeviceContextForWindow(
				clench::wsal::NativeWindow *window) override;

			CLCGHAL_API virtual VertexLayout *createVertexLayout(
				VertexLayoutElementDesc *elementDescs,
				size_t nElementDescs,
				VertexShader *vertexShader) override;

			CLCGHAL_API virtual VertexShader *createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) override;
			CLCGHAL_API virtual FragmentShader *createFragmentShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;
			CLCGHAL_API virtual GeometryShader *createGeometryShader(const char *sources, size_t size, ShaderSourceInfo *sourceInfo) override;

			CLCGHAL_API virtual ShaderProgram *linkShaderProgram(Shader **shaders, size_t nShaders) override;

			CLCGHAL_API virtual Buffer *createBuffer(const BufferDesc &bufferDesc, const void *initialData) override;

			CLCGHAL_API virtual Texture1D *createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) override;
			CLCGHAL_API virtual Texture2D *createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) override;
			CLCGHAL_API virtual Texture3D *createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) override;

			CLCGHAL_API virtual RenderTargetView *createRenderTargetViewForTexture2D(Texture2D *texture) override;

			CLENCH_FORCEINLINE bool isD3D11_1Capable() {
				return d3dImmediateDeviceContext1;
			}
		};

		class D3D11DeviceContext : public DeviceContext {
		public:
			D3D11Device *device;

			ComPtr<IDXGISwapChain> dxgiSwapChain;
			ComPtr<IDXGISwapChain1> dxgiSwapChain1;

			ComPtr<ID3D11Texture2D> d3dBackBuffer;
			ComPtr<ID3D11RenderTargetView> d3dRenderTargetView;  // RTV for the back buffer.

			peff::RcObjectPtr<D3D11RenderTargetView> renderTargetView;

			ComPtr<ID3D11Texture2D> d3dDepthStencilBuffer;
			ComPtr<ID3D11DepthStencilView> d3dDepthStencilView; // DSV for the depth stencil buffer.

			ComPtr<ID3D11DeviceContext> d3dDeviceContext;
			ComPtr<ID3D11DeviceContext1> d3dDeviceContext1;

			int viewportX = 0, viewportY = 0,
				viewportWidth = 0, viewportHeight = 0,
				viewportMinDepth = 0.0f, viewportMaxDepth = 0.0f,
				windowWidth = 0, windowHeight = 0;

			CLENCH_NO_COPY_MOVE_METHODS(D3D11DeviceContext);

			CLCGHAL_API D3D11DeviceContext(
				D3D11Device *device,
				ID3D11DeviceContext *deviceContext,
				IDXGISwapChain *swapChain);
			CLCGHAL_API D3D11DeviceContext(
				D3D11Device *device,
				ID3D11DeviceContext *deviceContext,
				ID3D11DeviceContext1 *deviceContext1,
				IDXGISwapChain *swapChain,
				IDXGISwapChain1 *swapChain1);
			CLCGHAL_API virtual ~D3D11DeviceContext();

			CLCGHAL_API virtual RenderTargetView *getDefaultRenderTargetView() override;

			CLCGHAL_API virtual void onResize(int width, int height) override;
			CLCGHAL_API virtual void clearRenderTargetView(
				RenderTargetView *renderTargetView,
				float r,
				float g,
				float b,
				float a) override;
			CLCGHAL_API virtual void clearDepth(
				DepthStencilView *depthStencilView,
				float depth) override;
			CLCGHAL_API virtual void clearStencil(
				DepthStencilView *depthStencilView,
				uint8_t stencil) override;

			CLCGHAL_API virtual void bindVertexBuffer(Buffer *buffer, size_t stride) override;
			CLCGHAL_API virtual void bindIndexBuffer(Buffer *buffer) override;

			CLCGHAL_API virtual void bindVertexLayout(VertexLayout *vertexArray) override;

			CLCGHAL_API virtual void setData(Buffer *buffer, const void *data) override;

			CLCGHAL_API virtual void setShaderProgram(ShaderProgram *shaderProgram) override;

			CLCGHAL_API virtual void setRenderTarget(
				RenderTargetView *renderTargetView,
				DepthStencilView *depthStencilView) override;

			CLCGHAL_API virtual void setViewport(
				int x,
				int y,
				int width,
				int height,
				float minDepth,
				float maxDepth) override;
			CLCGHAL_API virtual void getViewport(
				int &xOut,
				int &yOut,
				int &widthOut,
				int &heightOut,
				float &minDepthOut,
				float &maxDepthOut) override;

			CLCGHAL_API virtual void drawTriangle(unsigned int nTriangles) override;
			CLCGHAL_API virtual void drawIndexed(unsigned int nIndices) override;

			CLCGHAL_API virtual void begin() override;
			CLCGHAL_API virtual void end() override;

			CLCGHAL_API virtual void present() override;
		};

		CLCGHAL_API DXGI_FORMAT textureFormatToDXGIFormat(TextureFormat textureFormat);
	}
}

#endif
