#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API GLDevice::GLDevice(ghal::Device *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) : Device(associatedDevice, selfAllocator, resourceAllocator) {}
CLCACRI_API GLDevice::~GLDevice() {}

CLCACRI_API void GLDevice::dealloc() {
	peff::destroyAndRelease<GLDevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API GLDevice *GLDevice::alloc(ghal::Device *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) {
	std::unique_ptr<GLDevice, peff::DeallocableDeleter<GLDevice>> ptr(
		peff::allocAndConstruct<GLDevice>(selfAllocator, sizeof(std::max_align_t), associatedDevice, selfAllocator, resourceAllocator));
	if (!ptr) {
		return nullptr;
	}

	return ptr.release();
}

CLCACRI_API GLDeviceContext::GLDeviceContext(Device *device, ghal::DeviceContext *deviceContext) : DeviceContext(device, deviceContext) {}
CLCACRI_API GLDeviceContext::~GLDeviceContext() {}

CLCACRI_API void GLDeviceContext::dealloc() {
	peff::destroyAndRelease<GLDeviceContext>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API void GLDeviceContext::drawTriangle(const TriangleParams &params, Brush *brush, float width) {
}

CLCACRI_API void GLDeviceContext::fillTriangle(const TriangleParams &params, Brush *brush) {
	switch (brush->brushType) {
		case BrushType::SolidColor: {
			SolidColorBrush *b = (SolidColorBrush *)brush;
			float vertices[] = {
				b->color.r, b->color.g, b->color.b, b->color.a,
				params.vertices[0].x, params.vertices[0].y,
				params.vertices[1].x, params.vertices[1].y,
				params.vertices[2].x, params.vertices[2].y
			};

			std::lock_guard<std::mutex> triangleSolidColorMutex(localDeviceResources.forTriangle.solidColorLock);

			ghalDeviceContext->bindVertexLayout(((GLDevice *)device)->deviceResources.forTriangle.solidColorVertexLayout.get());
			ghalDeviceContext->bindVertexBuffer(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), sizeof(vertices));
			ghalDeviceContext->setData(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), vertices);
			ghalDeviceContext->setShaderProgram(((GLDevice *)device)->deviceResources.forTriangle.solidColorShaderProgram.get());

			break;
		}
	}
}

CLCACRI_API void GLDeviceContext::drawEllipse(const EllipseParams &params, Brush *brush, float width) {
}

CLCACRI_API void GLDeviceContext::fillEllipse(const EllipseParams &params, Brush *brush) {
}

CLCACRI_API void GLDeviceContext::drawPath(const PathParams &params, Brush *brush, float width) {
}

CLCACRI_API void GLDeviceContext::fillPath(const PathParams &params, Brush *brush) {
}

CLCACRI_API GLDeviceContext *GLDeviceContext::alloc(Device *device, ghal::DeviceContext *deviceContext) {
	std::unique_ptr<GLDeviceContext, peff::RcObjectUniquePtrDeleter> ptr(
		peff::allocAndConstruct<GLDeviceContext>(device->resourceAllocator.get(), sizeof(std::max_align_t), device, deviceContext));
	if (!ptr) {
		return nullptr;
	}

	return ptr.release();
}
