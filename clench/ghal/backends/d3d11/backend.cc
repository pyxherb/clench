#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::ghal;

CLCGHAL_API D3D11GHALBackend::D3D11GHALBackend() : GHALBackend("d3d11") {
}

CLCGHAL_API D3D11GHALBackend::~D3D11GHALBackend() {
}

CLCGHAL_API GHALDevice *D3D11GHALBackend::createDevice() {
	HRESULT result;

	UINT createDeviceFlags = 0;

#ifndef NDEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static D3D_DRIVER_TYPE driverTypeList[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	static D3D_FEATURE_LEVEL featureLevelList[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_DRIVER_TYPE d3dDriverType;
	D3D_FEATURE_LEVEL d3dFeatureLevel;

	ComPtr<ID3D11Device> d3dDevice;
	ComPtr<ID3D11DeviceContext> d3dDeviceContext;

	ComPtr<ID3D11Device1> d3dDevice1;
	ComPtr<ID3D11DeviceContext1> d3dDeviceContext1;

	for (size_t i = 0; i < ARRAYSIZE(driverTypeList); ++i) {
		d3dDriverType = driverTypeList[i];

		for (size_t j = 0; j < ARRAYSIZE(featureLevelList); ++j) {
			if (SUCCEEDED(result = D3D11CreateDevice(
							  nullptr,
							  d3dDriverType,
							  NULL,
							  createDeviceFlags,
							  featureLevelList, ARRAYSIZE(featureLevelList),
							  D3D11_SDK_VERSION,
							  &d3dDevice,
							  &d3dFeatureLevel,
							  &d3dDeviceContext)))
				break;
		}

		if (SUCCEEDED(result))
			break;
	}

	if (FAILED(result))
		throw std::runtime_error("Error creating D3D11 device");

	switch (d3dFeatureLevel) {
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
			break;
		default:
			throw std::runtime_error("Unsupported D3D version");
	}

	ComPtr<IDXGIDevice> dxgiDevice;
	ComPtr<IDXGIAdapter> dxgiAdapter;
	ComPtr<IDXGIFactory1> dxgiFactory1;
	ComPtr<IDXGIFactory2> dxgiFactory2;

	if (FAILED(result = d3dDevice.As(&dxgiDevice)))
		throw std::runtime_error("Error querying IDXGIDevice interface");

	if (FAILED(result = dxgiDevice->GetAdapter(&dxgiAdapter)))
		throw std::runtime_error("Error getting DXGI adapter");

	if (FAILED(result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), (void **)&dxgiFactory1)))
		throw std::runtime_error("Error querying IDXGIFactory1 interface");

	result = dxgiFactory1.As(&dxgiFactory2);

	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3D11VertexShader> clearVertexShader;
	ComPtr<ID3D11PixelShader> clearPixelShader;

	if (FAILED(d3dDevice->CreateVertexShader(
			g_clearRtvVertShaderByteCode,
			g_clearRtvVertShaderByteCode_length,
			nullptr,
			&clearVertexShader)))
		throw std::runtime_error("Error creating initial shaders");

	if (FAILED(d3dDevice->CreatePixelShader(
			g_clearRtvFragShaderByteCode,
			g_clearRtvFragShaderByteCode_length,
			nullptr,
			&clearPixelShader)))
		throw std::runtime_error("Error creating initial shaders");

	ComPtr<ID3D11InputLayout> clearInputLayout;

	D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(d3dDevice->CreateInputLayout(
			inputElementDescs,
			ARRAYSIZE(inputElementDescs),
			g_clearRtvVertShaderByteCode,
			g_clearRtvVertShaderByteCode_length,
			&clearInputLayout)))
		throw std::runtime_error("Error creating initial input layouts");

	D3D11GHALDevice *ghalDevice;

	if (dxgiFactory2) {
		if (FAILED(result = d3dDevice.As(&d3dDevice1)))
			throw std::runtime_error("Error converting ID3D11Device to ID3D11Device1");

		if (FAILED(result = d3dDeviceContext.As(&d3dDeviceContext1)))
			throw std::runtime_error("Error converting ID3D11DeviceContext to ID3D11DeviceContext1");

		ghalDevice = new D3D11GHALDevice(
			this,
			d3dDevice.Get(),
			d3dDevice1.Get(),
			d3dDeviceContext.Get(),
			d3dDeviceContext1.Get(),
			dxgiDevice.Get(),
			dxgiAdapter.Get(),
			dxgiFactory1.Get(),
			dxgiFactory2.Get());
	} else {
		ghalDevice = new D3D11GHALDevice(
			this,
			d3dDevice.Get(),
			d3dDeviceContext.Get(),
			dxgiDevice.Get(),
			dxgiAdapter.Get(),
			dxgiFactory1.Get());
	}

	ghalDevice->clearInputLayout = clearInputLayout;
	ghalDevice->clearVertexShader = clearVertexShader;
	ghalDevice->clearFragmentShader = clearPixelShader;

	{
		clench::ghal::BufferDesc vertexBufferDesc, indexBufferDesc;

		static float data[] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f,

			1.0f, -1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f,

			-1.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 1.0f,

			1.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		vertexBufferDesc.size = sizeof(data);
		vertexBufferDesc.usage = clench::ghal::BufferUsage::Dynamic;
		vertexBufferDesc.proposedTarget = clench::ghal::BufferTarget::Vertex;
		vertexBufferDesc.cpuReadable = false;
		vertexBufferDesc.cpuWritable = true;

		ghalDevice->clearVertexBuffer = (D3D11Buffer*)ghalDevice->createBuffer(vertexBufferDesc, data);

		static int indices[] = {
			0, 1, 2,
			3, 2, 1
		};

		indexBufferDesc.size = sizeof(indices);
		indexBufferDesc.usage = clench::ghal::BufferUsage::Static;
		indexBufferDesc.proposedTarget = clench::ghal::BufferTarget::Index;
		indexBufferDesc.cpuReadable = false;
		indexBufferDesc.cpuWritable = false;
		ghalDevice->clearIndexBuffer = (D3D11Buffer*)ghalDevice->createBuffer(indexBufferDesc, indices);
	}

	return ghalDevice;
}
