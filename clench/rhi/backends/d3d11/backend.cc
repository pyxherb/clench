#include "backend.h"
#include "device.h"

using namespace clench;
using namespace clench::rhi;

CLCRHI_API D3D11RHIBackend::D3D11RHIBackend() : RHIBackend("d3d11") {
}

CLCRHI_API D3D11RHIBackend::~D3D11RHIBackend() {
}

CLCRHI_API RHIDevice *D3D11RHIBackend::createDevice() {
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

	ComPtr<ID3D11Device> d3dDevice1;
	ComPtr<ID3D11DeviceContext> d3dDeviceContext1;

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

	if (dxgiFactory2) {
		if (FAILED(result = d3dDevice.As(&d3dDevice1)))
			throw std::runtime_error("Error converting ID3D11Device to ID3D11Device1");

		if (FAILED(result = d3dDeviceContext.As(&d3dDeviceContext1)))
			throw std::runtime_error("Error converting ID3D11DeviceContext to ID3D11DeviceContext1");

		return new D3D11RHIDevice(
			this,
			d3dDevice1.Get(),
			d3dDeviceContext1.Get(),
			dxgiDevice.Get(),
			dxgiAdapter.Get(),
			dxgiFactory1.Get());
	} else {
		return new D3D11RHIDevice(
			this,
			d3dDevice.Get(),
			d3dDeviceContext.Get(),
			dxgiDevice.Get(),
			dxgiAdapter.Get(),
			dxgiFactory1.Get());
	}
}
