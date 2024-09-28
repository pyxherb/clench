#include "device.h"
#include "vertex_array.h"
#include "shader.h"
#include "buffer.h"
#include "views.h"
#include "texture.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11GHALDevice::D3D11GHALDevice(
	D3D11GHALBackend *backend,
	ID3D11Device *d3dDevice,
	ID3D11DeviceContext *d3dImmediateDeviceContext,
	IDXGIDevice *dxgiDevice,
	IDXGIAdapter *dxgiAdapter,
	IDXGIFactory1 *dxgiFactory1)
	: GHALDevice(),
	  backend(backend),
	  d3dDevice(d3dDevice),
	  d3dImmediateDeviceContext(d3dImmediateDeviceContext),
	  dxgiDevice(dxgiDevice),
	  dxgiAdapter(dxgiAdapter),
	  dxgiFactory1(dxgiFactory1) {
}

CLCGHAL_API D3D11GHALDevice::D3D11GHALDevice(
	D3D11GHALBackend *backend,
	ID3D11Device *d3dDevice,
	ID3D11Device1 *d3dDevice1,
	ID3D11DeviceContext *d3dImmediateDeviceContext,
	ID3D11DeviceContext1 *d3dImmediateDeviceContext1,
	IDXGIDevice *dxgiDevice,
	IDXGIAdapter *dxgiAdapter,
	IDXGIFactory1 *dxgiFactory1,
	IDXGIFactory2 *dxgiFactory2)
	: GHALDevice(),
	  backend(backend),
	  d3dDevice(d3dDevice),
	  d3dDevice1(d3dDevice1),
	  d3dImmediateDeviceContext(d3dImmediateDeviceContext),
	  d3dImmediateDeviceContext1(d3dImmediateDeviceContext1),
	  dxgiDevice(dxgiDevice),
	  dxgiAdapter(dxgiAdapter),
	  dxgiFactory1(dxgiFactory1),
	  dxgiFactory2(dxgiFactory2) {
}

CLCGHAL_API D3D11GHALDevice::~D3D11GHALDevice() {
}

CLCGHAL_API GHALBackend *D3D11GHALDevice::getBackend() {
	return backend;
}

CLCGHAL_API GHALDeviceContext *D3D11GHALDevice::createDeviceContextForWindow(clench::wsal::NativeWindow *window) {
	HRESULT result;
	D3D11GHALDeviceContext *ghalDeviceContext;
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

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
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
					   d3dDevice1.Get(),
					   window->nativeHandle,
					   &sd, &fd,
					   nullptr,
					   &swapChain1))) {
			throw std::runtime_error("Error creating swap chain");
		}

		if (FAILED(swapChain1.As(&swapChain)))
			throw std::runtime_error("Error converting IDXGISwapChain1 to IDXGISwapChain");

		ghalDeviceContext = new D3D11GHALDeviceContext(
			this,
			d3dImmediateDeviceContext.Get(),
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

		ghalDeviceContext = new D3D11GHALDeviceContext(
			this,
			d3dImmediateDeviceContext.Get(),
			d3dImmediateDeviceContext1.Get(),
			swapChain.Get(),
			swapChain1.Get());
	}

	dxgiFactory1->MakeWindowAssociation(window->nativeHandle, DXGI_MWA_NO_ALT_ENTER);

	ghalDeviceContext->onResize(width, height);

	return ghalDeviceContext;
}

CLCGHAL_API VertexArray *D3D11GHALDevice::createVertexArray(
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

	if (FAILED(d3dDevice->CreateInputLayout(
			inputElementDescs.get(),
			nElementDescs,
			((D3D11VertexShader *)vertexShader)->source.get(),
			((D3D11VertexShader *)vertexShader)->szSource,
			&inputLayout)))
		throw std::runtime_error("Error creating new vertex array");

	return new D3D11VertexArray(this, inputLayout.Get());
}

CLCGHAL_API VertexShader *D3D11GHALDevice::createVertexShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	ComPtr<ID3D11VertexShader> shader;

	if (FAILED(d3dDevice->CreateVertexShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11VertexShader(this, shader.Get(), source, size);
}

CLCGHAL_API FragmentShader *D3D11GHALDevice::createFragmentShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	ComPtr<ID3D11PixelShader> shader;

	if (FAILED(d3dDevice->CreatePixelShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11FragmentShader(this, shader.Get());
}

CLCGHAL_API GeometryShader *D3D11GHALDevice::createGeometryShader(const char *source, size_t size, ShaderSourceInfo *sourceInfo) {
	ComPtr<ID3D11GeometryShader> shader;

	if (FAILED(d3dDevice->CreateGeometryShader(source, size, nullptr, &shader)))
		throw std::runtime_error("Error creating vertex shader");

	return new D3D11GeometryShader(this, shader.Get());
}

CLCGHAL_API Buffer *D3D11GHALDevice::createBuffer(const BufferDesc &bufferDesc, const void *initialData) {
	D3D11_BUFFER_DESC desc;
	ComPtr<ID3D11Buffer> buffer;

	memset(&desc, 0, sizeof(desc));

	desc.ByteWidth = bufferDesc.size;
	switch (bufferDesc.proposedTarget) {
		case BufferTarget::Vertex:
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			break;
		case BufferTarget::Index:
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			break;
		default:
			throw std::logic_error("Invalid buffer target");
	}
	switch (bufferDesc.usage) {
		case BufferUsage::Default:
			desc.Usage = D3D11_USAGE_DEFAULT;
			break;
		case BufferUsage::Dynamic:
			desc.Usage = D3D11_USAGE_DYNAMIC;
			break;
		case BufferUsage::Static:
			if (!initialData)
				throw std::logic_error("Static buffer must have initial data");
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			break;
		case BufferUsage::Staging:
			desc.Usage = D3D11_USAGE_STAGING;
			break;
	}
	if (bufferDesc.usage != BufferUsage::Static) {
		if (bufferDesc.cpuReadable)
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
		if (bufferDesc.cpuWritable)
			desc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}

	if (initialData) {
		D3D11_SUBRESOURCE_DATA subresourceData;
		memset(&subresourceData, 0, sizeof(subresourceData));

		subresourceData.pSysMem = initialData;

		if (FAILED(d3dDevice->CreateBuffer(&desc, &subresourceData, &buffer)))
			throw std::runtime_error("Error creating GHAL buffer");
	} else {
		if (FAILED(d3dDevice->CreateBuffer(&desc, nullptr, &buffer)))
			throw std::runtime_error("Error creating GHAL buffer");
	}

	return new D3D11Buffer(this, bufferDesc, buffer.Get());
}

CLCGHAL_API Texture1D *D3D11GHALDevice::createTexture1D(const char *data, size_t size, const Texture1DDesc &desc) {
	D3D11_TEXTURE1D_DESC d3dTextureDesc;
	ComPtr<ID3D11Texture1D> texture1d;

	d3dTextureDesc.Width = desc.width;
	d3dTextureDesc.MipLevels = desc.mipmapLevel;
	d3dTextureDesc.ArraySize = 1;

	if ((d3dTextureDesc.Format = textureFormatToDXGIFormat(desc.format)) == DXGI_FORMAT_UNKNOWN)
		throw std::runtime_error("Unsupported texture format");

	if (data) {
		D3D11_SUBRESOURCE_DATA d3dSubresourceData;
		memset(&d3dSubresourceData, 0, sizeof(d3dSubresourceData));
		d3dSubresourceData.pSysMem = data;

		if (FAILED(d3dDevice->CreateTexture1D(&d3dTextureDesc, &d3dSubresourceData, &texture1d)))
			throw std::runtime_error("Error creating texture");
	} else {
		if (FAILED(d3dDevice->CreateTexture1D(&d3dTextureDesc, nullptr, &texture1d)))
			throw std::runtime_error("Error creating texture");
	}

	return new D3D11Texture1D(
		this,
		desc,
		texture1d.Get());
}

CLCGHAL_API Texture2D *D3D11GHALDevice::createTexture2D(const char *data, size_t size, const Texture2DDesc &desc) {
	D3D11_TEXTURE2D_DESC d3dTextureDesc;
	ComPtr<ID3D11Texture2D> texture2d;

	memset(&d3dTextureDesc, 0, sizeof(d3dTextureDesc));

	d3dTextureDesc.Width = desc.width;
	d3dTextureDesc.Height = desc.height;
	d3dTextureDesc.MipLevels = desc.mipmapLevel;
	d3dTextureDesc.ArraySize = 1;

	if ((d3dTextureDesc.Format = textureFormatToDXGIFormat(desc.format)) == DXGI_FORMAT_UNKNOWN)
		throw std::runtime_error("Unsupported texture format");

	d3dTextureDesc.SampleDesc.Count = 1;
	d3dTextureDesc.SampleDesc.Quality = 0;
	d3dTextureDesc.Usage = D3D11_USAGE_DEFAULT;

	if (desc.bindingFlags & TEXTURE_BINDING_RENDER_TARGET)
		d3dTextureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	if (desc.bindingFlags & TEXTURE_BINDING_SHADER_RESOURCE)
		d3dTextureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

	if (data) {
		D3D11_SUBRESOURCE_DATA d3dSubresourceData;

		memset(&d3dSubresourceData, 0, sizeof(d3dSubresourceData));
		d3dSubresourceData.pSysMem = data;
		d3dSubresourceData.SysMemPitch = getPixelSizeForTextureFormat(desc.format) * desc.width;

		if (FAILED(d3dDevice->CreateTexture2D(&d3dTextureDesc, &d3dSubresourceData, &texture2d)))
			throw std::runtime_error("Error creating texture");
	} else {
		if (FAILED(d3dDevice->CreateTexture2D(&d3dTextureDesc, nullptr, &texture2d)))
			throw std::runtime_error("Error creating texture");
	}

	return new D3D11Texture2D(
		this,
		desc,
		texture2d.Get());
}

CLCGHAL_API Texture3D *D3D11GHALDevice::createTexture3D(const char *data, size_t size, const Texture3DDesc &desc) {
	D3D11_TEXTURE3D_DESC d3dTextureDesc;
	ComPtr<ID3D11Texture3D> texture3d;

	d3dTextureDesc.Width = desc.width;
	d3dTextureDesc.Height = desc.height;
	d3dTextureDesc.Depth = desc.depth;
	d3dTextureDesc.MipLevels = desc.mipmapLevel;

	if ((d3dTextureDesc.Format = textureFormatToDXGIFormat(desc.format)) == DXGI_FORMAT_UNKNOWN)
		throw std::runtime_error("Unsupported texture format");

	if (data) {
		D3D11_SUBRESOURCE_DATA d3dSubresourceData;

		memset(&d3dSubresourceData, 0, sizeof(d3dSubresourceData));
		d3dSubresourceData.pSysMem = data;
		d3dSubresourceData.SysMemPitch = getPixelSizeForTextureFormat(desc.format) * desc.width;
		d3dSubresourceData.SysMemSlicePitch = getPixelSizeForTextureFormat(desc.format) * desc.width * desc.height;

		if (FAILED(d3dDevice->CreateTexture3D(&d3dTextureDesc, &d3dSubresourceData, &texture3d)))
			throw std::runtime_error("Error creating texture");
	} else {
		if (FAILED(d3dDevice->CreateTexture3D(&d3dTextureDesc, nullptr, &texture3d)))
			throw std::runtime_error("Error creating texture");
	}

	return new D3D11Texture3D(
		this,
		desc,
		texture3d.Get());
}

CLCGHAL_API RenderTargetView *D3D11GHALDevice::createRenderTargetViewForTexture2D(Texture2D *texture) {
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	D3D11_RENDER_TARGET_VIEW_DESC desc;
	desc.Format = textureFormatToDXGIFormat(texture->textureDesc.format);
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = 0;

	if (FAILED(d3dDevice->CreateRenderTargetView(
			((D3D11Texture2D *)texture)->texture2D.Get(),
			&desc,
			&renderTargetView)))
		throw std::runtime_error("Error creating render target view");

	return new D3D11RenderTargetView(this, RenderTargetViewType::Texture2D, renderTargetView.Get());
}

CLCGHAL_API D3D11GHALDeviceContext::D3D11GHALDeviceContext(
	D3D11GHALDevice *device,
	ID3D11DeviceContext *deviceContext,
	IDXGISwapChain *swapChain)
	: GHALDeviceContext(),
	  device(device),
	  d3dDeviceContext(deviceContext),
	  dxgiSwapChain(swapChain) {
}

CLCGHAL_API D3D11GHALDeviceContext::D3D11GHALDeviceContext(
	D3D11GHALDevice *device,
	ID3D11DeviceContext *deviceContext,
	ID3D11DeviceContext1 *deviceContext1,
	IDXGISwapChain *swapChain,
	IDXGISwapChain1 *swapChain1)
	: GHALDeviceContext(),
	  device(device),
	  d3dDeviceContext(deviceContext),
	  d3dDeviceContext1(deviceContext1),
	  dxgiSwapChain(swapChain),
	  dxgiSwapChain1(swapChain1) {
}

CLCGHAL_API D3D11GHALDeviceContext::~D3D11GHALDeviceContext() {
}

CLCGHAL_API RenderTargetView *D3D11GHALDeviceContext::getDefaultRenderTargetView() {
	return renderTargetView.get();
}

CLCGHAL_API void D3D11GHALDeviceContext::onResize(int width, int height) {
	renderTargetView.reset();

	d3dRenderTargetView.Reset();
	d3dDepthStencilView.Reset();
	d3dDepthStencilBuffer.Reset();
	d3dBackBuffer.Reset();

	width = width ? width : 1;
	height = height ? height : 1;

	if (FAILED(dxgiSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		throw std::runtime_error("Error resizing back buffer");

	if (FAILED(dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&d3dBackBuffer)))
		throw std::runtime_error("Error getting back buffer");

	if (FAILED(device->d3dDevice->CreateRenderTargetView(d3dBackBuffer.Get(), nullptr, &d3dRenderTargetView)))
		throw std::runtime_error("Error creating render target view");

	ComPtr<ID3D11Texture2D> backBuffer;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	if (FAILED(dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer)))
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

	if (FAILED(device->d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &d3dDepthStencilBuffer)))
		throw std::runtime_error("Error creating depth stencil buffer");

	if (FAILED(device->d3dDevice->CreateDepthStencilView(d3dDepthStencilBuffer.Get(), nullptr, &d3dDepthStencilView)))
		throw std::runtime_error("Error creating depth stencil view");

	d3dDeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), d3dDepthStencilView.Get());

	this->renderTargetView = new D3D11RenderTargetView(
		device,
		RenderTargetViewType::Buffer,
		d3dRenderTargetView.Get());

	windowWidth = width, windowHeight = height;

	setViewport(0, 0, width, height, 0.0f, 1.0f);
}

CLCGHAL_API void D3D11GHALDeviceContext::clearRenderTargetView(
	RenderTargetView *renderTargetView,
	float r,
	float g,
	float b,
	float a) {
	float color[] = { r, g, b, a };

	float left = -1.0f,
		  right = 1.0f,
		  top = 1.0f,
		  bottom = -1.0f;

	float data[] = {
		left, top, 0.0f,
		r, g, b, a,

		right, top, 0.0f,
		r, g, b, a,

		left, bottom, 0.0f,
		r, g, b, a,

		right, bottom, 0.0f,
		r, g, b, a
	};

	clearDepth(nullptr, 1.0f);
	clearStencil(nullptr, 0);

	d3dDeviceContext->IASetInputLayout(device->clearInputLayout.Get());

	setData(device->clearVertexBuffer.get(), data);
	bindVertexBuffer(device->clearVertexBuffer.get(), sizeof(float) * 7);
	bindIndexBuffer(device->clearIndexBuffer.get());

	d3dDeviceContext->VSSetShader(device->clearVertexShader.Get(), nullptr, 0);
	d3dDeviceContext->PSSetShader(device->clearFragmentShader.Get(), nullptr, 0);

	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->DrawIndexed(6, 0, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::clearDepth(
	DepthStencilView *depthStencilView,
	float depth) {
	d3dDeviceContext->ClearDepthStencilView(
		depthStencilView
			? ((D3D11DepthStencilView *)depthStencilView)->depthStencilView.Get()
			: this->d3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH, depth, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::clearStencil(
	DepthStencilView *depthStencilView,
	uint8_t stencil) {
	d3dDeviceContext->ClearDepthStencilView(
		depthStencilView
			? ((D3D11DepthStencilView *)depthStencilView)->depthStencilView.Get()
			: this->d3dDepthStencilView.Get(),
		D3D11_CLEAR_STENCIL, 0.0f, stencil);
}

CLCGHAL_API void D3D11GHALDeviceContext::bindVertexBuffer(Buffer *buffer, size_t stride) {
	UINT strideParam = stride;
	UINT offsetParam = 0;
	d3dDeviceContext->IASetVertexBuffers(
		0,
		1,
		((D3D11Buffer *)buffer)->buffer.GetAddressOf(),
		&strideParam,
		&offsetParam);
}

CLCGHAL_API void D3D11GHALDeviceContext::bindIndexBuffer(Buffer *buffer) {
	d3dDeviceContext->IASetIndexBuffer(
		((D3D11Buffer *)buffer)->buffer.Get(),
		DXGI_FORMAT_R32_UINT,
		0);
}

CLCGHAL_API void D3D11GHALDeviceContext::bindVertexArray(VertexArray *vertexArray) {
	d3dDeviceContext->IASetInputLayout(((D3D11VertexArray *)vertexArray)->inputLayout.Get());
}

CLCGHAL_API void D3D11GHALDeviceContext::setData(Buffer *buffer, const void *data) {
	ID3D11Buffer *d3dBuffer = ((D3D11Buffer *)buffer)->buffer.Get();
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	if (FAILED(d3dDeviceContext->Map(
			d3dBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&mappedSubresource)))
		throw std::runtime_error("Error mapping GHAL buffer");

	memcpy(mappedSubresource.pData, data, buffer->bufferDesc.size);

	d3dDeviceContext->Unmap(d3dBuffer, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::setVertexShader(VertexShader *vertexShader) {
	d3dDeviceContext->VSSetShader(((D3D11VertexShader *)vertexShader)->shader.Get(), nullptr, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::setFragmentShader(FragmentShader *vertexShader) {
	d3dDeviceContext->PSSetShader(((D3D11FragmentShader *)vertexShader)->shader.Get(), nullptr, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::setRenderTarget(
	RenderTargetView *renderTargetView,
	DepthStencilView *depthStencilView) {
	d3dDeviceContext->OMSetRenderTargets(
		1,
		((D3D11RenderTargetView *)renderTargetView)->renderTargetView.GetAddressOf(),
		((D3D11DepthStencilView *)depthStencilView)->depthStencilView.Get());
}

CLCGHAL_API void D3D11GHALDeviceContext::setViewport(
	int x,
	int y,
	int width,
	int height,
	float minDepth,
	float maxDepth) {
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;

	viewportX = x;
	viewportY = windowHeight - y - height;
	viewportWidth = width;
	viewportHeight = windowHeight - y;
	viewportMinDepth = minDepth;
	viewportMaxDepth = maxDepth;

	d3dDeviceContext->RSSetViewports(1, &viewport);
}

CLCGHAL_API void D3D11GHALDeviceContext::getViewport(
	int &xOut,
	int &yOut,
	int &widthOut,
	int &heightOut,
	float &minDepthOut,
	float &maxDepthOut) {
	xOut = viewportX;
	yOut = viewportY;
	widthOut = viewportWidth;
	heightOut = viewportHeight;
	minDepthOut = minDepthOut;
	maxDepthOut = maxDepthOut;
}

CLCGHAL_API void D3D11GHALDeviceContext::drawIndexed(unsigned int nIndices) {
	d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dDeviceContext->DrawIndexed(nIndices, 0, 0);
}

CLCGHAL_API void D3D11GHALDeviceContext::begin() {
}

CLCGHAL_API void D3D11GHALDeviceContext::end() {
}

CLCGHAL_API void D3D11GHALDeviceContext::present() {
	dxgiSwapChain->Present(0, 0);
}

CLCGHAL_API DXGI_FORMAT clench::ghal::textureFormatToDXGIFormat(TextureFormat textureFormat) {
	switch (textureFormat) {
		case TextureFormat::RGBA8:
			return DXGI_FORMAT_R8G8B8A8_UINT;
		case TextureFormat::RGB32F:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default:
			return DXGI_FORMAT_UNKNOWN;
	}
}
