#include "device.h"
#include "vertex_array.h"
#include "shader.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API D3D11RHIDevice::D3D11RHIDevice(
	D3D11RHIBackend *backend,
	ID3D11Device *d3dDevice,
	ID3D11DeviceContext *d3dImmediateDeviceContext,
	IDXGIDevice *dxgiDevice,
	IDXGIAdapter *dxgiAdapter,
	IDXGIFactory1 *dxgiFactory1)
	: RHIDevice(),
	  backend(backend),
	  d3dDevice(d3dDevice),
	  d3dImmediateDeviceContext(d3dImmediateDeviceContext),
	  dxgiDevice(dxgiDevice),
	  dxgiAdapter(dxgiAdapter),
	  dxgiFactory1(dxgiFactory1) {
}

CLCRHI_API D3D11RHIDevice::D3D11RHIDevice(
	D3D11RHIBackend *backend,
	ID3D11Device1 *d3dDevice,
	ID3D11DeviceContext1 *d3dImmediateDeviceContext,
	IDXGIDevice *dxgiDevice,
	IDXGIAdapter *dxgiAdapter,
	IDXGIFactory1 *dxgiFactory1,
	IDXGIFactory2 *dxgiFactory2)
	: RHIDevice(),
	  backend(backend),
	  d3dDevice1(d3dDevice),
	  d3dImmediateDeviceContext1(d3dImmediateDeviceContext),
	  dxgiDevice(dxgiDevice),
	  dxgiAdapter(dxgiAdapter),
	  dxgiFactory1(dxgiFactory1),
	  dxgiFactory2(dxgiFactory2) {
}

CLCRHI_API D3D11RHIDevice::~D3D11RHIDevice() {
}

CLCRHI_API RHIBackend *D3D11RHIDevice::getBackend() {
	return backend;
}

CLCRHI_API RHIDeviceContext *D3D11RHIDevice::createDeviceContextForWindow(clench::wsi::Window *window) {
	HRESULT result;
	D3D11RHIDeviceContext *rhiDeviceContext;
	int width, height;
	window->getSize(width, height);

	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<IDXGISwapChain1> swapChain1;
	if (isD3D11_1Capable()) {
		DXGI_SWAP_CHAIN_DESC1 sd;
		memset(&sd, 0, sizeof(sd));

		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferCount = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
		fd.RefreshRate.Numerator = 60;
		fd.RefreshRate.Denominator = 1;
		fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fd.Windowed = TRUE;

		if (FAILED(result = dxgiFactory2->CreateSwapChainForHwnd(
					   d3dDevice.Get(),
					   window->nativeHandle,
					   &sd, &fd,
					   nullptr,
					   &swapChain1))) {
			throw std::runtime_error("Error creating swap chain");
		}

		if (FAILED(swapChain1.As(&swapChain)))
			throw std::runtime_error("Error converting IDXGISwapChain1 to IDXGISwapChain");

		rhiDeviceContext = new D3D11RHIDeviceContext(
			this,
			swapChain1.Get());
	} else {
		DXGI_SWAP_CHAIN_DESC sd;
		memset(&sd, 0, sizeof(sd));

		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = window->nativeHandle;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		if (FAILED(result = dxgiFactory1->CreateSwapChain(
					   d3dDevice.Get(),
					   &sd,
					   &swapChain)))
			throw std::runtime_error("Error creating swap chain");

		rhiDeviceContext = new D3D11RHIDeviceContext(
			this,
			swapChain.Get());
	}

	dxgiFactory1->MakeWindowAssociation(window->nativeHandle, DXGI_MWA_NO_ALT_ENTER);

	rhiDeviceContext->onResize(width, height);

	return rhiDeviceContext;
}

CLCRHI_API VertexArray *D3D11RHIDevice::createVertexArray(
	VertexArrayElementDesc *elementDescs,
	size_t nElementDescs,
	VertexShader *vertexShader) {
	ComPtr<ID3D11InputLayout> inputLayout;

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> inputElementDescs(new D3D11_INPUT_ELEMENT_DESC[nElementDescs]);

	memset(inputElementDescs.get(), 0, sizeof(D3D11_INPUT_ELEMENT_DESC) * nElementDescs);

	for (size_t i = 0; i < nElementDescs; ++i) {
		D3D11_INPUT_ELEMENT_DESC &curDesc = (inputElementDescs.get())[i];
		VertexArrayElementDesc &curInputDesc = elementDescs[i];

		switch (curInputDesc.semanticType) {
			case InputVertexShaderSemanticType::None:
				curDesc.SemanticName = nullptr;
				break;
			case InputVertexShaderSemanticType::Binormal:
				curDesc.SemanticName = "BINORMAL";
				break;
			case InputVertexShaderSemanticType::BlendIndices:
				curDesc.SemanticName = "BLENDINDICES";
				break;
			case InputVertexShaderSemanticType::BlendWeight:
				curDesc.SemanticName = "BLENDWEIGHT";
				break;
			case InputVertexShaderSemanticType::Color:
				curDesc.SemanticName = "COLOR";
				break;
			case InputVertexShaderSemanticType::Normal:
				curDesc.SemanticName = "NORMAL";
				break;
			case InputVertexShaderSemanticType::Position:
				curDesc.SemanticName = "POSITION";
				break;
			case InputVertexShaderSemanticType::TransformPosition:
				curDesc.SemanticName = "POSITIONT";
				break;
			case InputVertexShaderSemanticType::PointSize:
				curDesc.SemanticName = "PSIZE";
				break;
			case InputVertexShaderSemanticType::Tangent:
				curDesc.SemanticName = "TANGENT";
				break;
		}
		curDesc.SemanticIndex = curInputDesc.semanticIndex;

		DXGI_FORMAT format;
		switch (curInputDesc.dataType) {
			case VertexDataType::Int:
				switch (curInputDesc.nElements) {
					case 1:
						format = DXGI_FORMAT_R32_SINT;
						break;
					case 2:
						format = DXGI_FORMAT_R32G32_SINT;
						break;
					case 3:
						format = DXGI_FORMAT_R32G32B32_SINT;
						break;
					case 4:
						format = DXGI_FORMAT_R32G32B32A32_SINT;
						break;
					default:
						throw std::runtime_error("Unsupported element number");
				}
				break;
			case VertexDataType::UInt:
				switch (curInputDesc.nElements) {
					case 1:
						format = DXGI_FORMAT_R32_UINT;
						break;
					case 2:
						format = DXGI_FORMAT_R32G32_UINT;
						break;
					case 3:
						format = DXGI_FORMAT_R32G32B32_UINT;
						break;
					case 4:
						format = DXGI_FORMAT_R32G32B32A32_UINT;
						break;
					default:
						throw std::runtime_error("Unsupported element number");
				}
				break;
			case VertexDataType::Float:
				switch (curInputDesc.nElements) {
					case 1:
						format = DXGI_FORMAT_R32_FLOAT;
						break;
					case 2:
						format = DXGI_FORMAT_R32G32_FLOAT;
						break;
					case 3:
						format = DXGI_FORMAT_R32G32B32_FLOAT;
						break;
					case 4:
						format = DXGI_FORMAT_R32G32B32A32_FLOAT;
						break;
					default:
						throw std::runtime_error("Unsupported element number");
				}
				break;
			default:
				throw std::runtime_error("Unsupported element type");
				break;
		}
		curDesc.Format = format;

		curDesc.AlignedByteOffset = curInputDesc.off;
		curDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	}

	if (FAILED(d3dDevice->CreateInputLayout(inputElementDescs.get(), nElementDescs, nullptr, 0, &inputLayout)))
		throw std::runtime_error("Error creating new vertex array");

	return new D3D11VertexArray(this, inputLayout.Get());
}

VertexShader *D3D11RHIDevice::createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo){
	ComPtr<ID3D11VertexShader> shader;

	if (FAILED(d3dDevice->CreateVertexShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11VertexShader(this, shader.Get());
}

FragmentShader *D3D11RHIDevice::createFragmentShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	ComPtr<ID3D11PixelShader> shader;

	if (FAILED(d3dDevice->CreatePixelShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11FragmentShader(this, shader.Get());
}

GeometryShader *D3D11RHIDevice::createGeometryShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	ComPtr<ID3D11GeometryShader> shader;

	if (FAILED(d3dDevice->CreateGeometryShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11GeometryShader(this, shader.Get());
}

CLCRHI_API void D3D11RHIDeviceContext::_resizeBuffersAndViewport(int width, int height) {
	ComPtr<ID3D11Texture2D> backBuffer;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer)))
		throw std::runtime_error("Error getting back buffer");

	if (FAILED(device->d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView)))
		throw std::runtime_error("Error creating render target view");

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (FAILED(device->d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer)))
		throw std::runtime_error("Error creating depth stencil buffer");

	if (FAILED(device->d3dDevice->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &depthStencilView)))
		throw std::runtime_error("Error creating depth stencil view");

	device->d3dImmediateDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	device->d3dImmediateDeviceContext->RSSetViewports(1, &viewport);
}

CLCRHI_API D3D11RHIDeviceContext::D3D11RHIDeviceContext(
	D3D11RHIDevice *device,
	IDXGISwapChain *swapChain)
	: RHIDeviceContext(),
	  device(device),
	  swapChain(swapChain) {
}

CLCRHI_API D3D11RHIDeviceContext::D3D11RHIDeviceContext(
	D3D11RHIDevice *device,
	IDXGISwapChain1 *swapChain1)
	: RHIDeviceContext(),
	  device(device),
	  swapChain1(swapChain1) {
}

CLCRHI_API D3D11RHIDeviceContext::~D3D11RHIDeviceContext() {
}

CLCRHI_API void D3D11RHIDeviceContext::onResize(int width, int height) {
	renderTargetView.Reset();
	depthStencilView.Reset();
	depthStencilBuffer.Reset();
	backBuffer.Reset();

	if (FAILED(swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		throw std::runtime_error("Error resizing back buffer");

	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer)))
		throw std::runtime_error("Error getting back buffer");

	if (FAILED(device->d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView)))
		throw std::runtime_error("Error creating render target view");

	_resizeBuffersAndViewport(width, height);
}

CLCRHI_API void D3D11RHIDeviceContext::clear(float r, float g, float b, float a) {
	float color[] = { r, g, b, a };

	device->d3dImmediateDeviceContext->ClearRenderTargetView(renderTargetView.Get(), color);
}

CLCRHI_API void D3D11RHIDeviceContext::clearDepthBuffer(float depth) {
	device->d3dImmediateDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH, depth, 0);
}

CLCRHI_API void D3D11RHIDeviceContext::clearStencilBuffer(uint8_t stencil) {
	device->d3dImmediateDeviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_STENCIL, 0.0f, stencil);
}

CLCRHI_API void D3D11RHIDeviceContext::begin() {
}

CLCRHI_API void D3D11RHIDeviceContext::end() {
}

CLCRHI_API void D3D11RHIDeviceContext::present() {
	swapChain->Present(0, 0);
}
