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
				b->color.r, b->color.g, b->color.b, b->color.a,
				params.vertices[1].x, params.vertices[1].y,
				b->color.r, b->color.g, b->color.b, b->color.a,
				params.vertices[2].x, params.vertices[2].y
			};

			std::lock_guard<std::mutex> triangleSolidColorMutex(localDeviceResources.forTriangle.solidColorLock);

			ghalDeviceContext->setShaderProgram(((GLDevice *)device)->deviceResources.solidColorShaderProgram.get());
			ghalDeviceContext->bindVertexBuffer(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), sizeof(vertices));
			ghalDeviceContext->bindVertexLayout(((GLDevice *)device)->deviceResources.solidColorVertexLayout.get());
			ghalDeviceContext->setData(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), vertices);

			ghalDeviceContext->drawTriangle(1);

			break;
		}
	}
}

CLCACRI_API void GLDeviceContext::drawPolygon(const PolygonParams &params, Brush *brush, float width) {
}

CLCACRI_API void GLDeviceContext::fillPolygon(const PolygonParams &params, Brush *brush) {
	bool isConcave = false;
	{
		math::Vec2f lastVec, curVec(params.vertices[1] - params.vertices[0]);

		bool sign = math::crossZ(lastVec, curVec) > 0;

		for (size_t i = 1; i < params.nVertices - 1; ++i) {
			lastVec = curVec;
			curVec = params.vertices[i + 1] - params.vertices[i];

			if (sign != (math::crossZ(lastVec, curVec) > 0)) {
				isConcave = true;
				goto concaveDetectionEnd;
			}
		}

		if (sign != (math::crossZ(curVec, params.vertices[0] - params.vertices[params.nVertices - 1]) > 0)) {
			isConcave = true;
		}
	}

concaveDetectionEnd:;

	switch (brush->brushType) {
		case BrushType::SolidColor: {
			SolidColorBrush *b = (SolidColorBrush *)brush;

			if(isConcave) {
				std::terminate();
			} else {
				for(size_t i = 2; i < params.nVertices; ++i) {
					TriangleParams triangleParams;

					triangleParams.vertices[0] = params.vertices[0];
					triangleParams.vertices[1] = params.vertices[i - 1];
					triangleParams.vertices[2] = params.vertices[i];

					fillTriangle(triangleParams, brush);
				}
			}

			break;
		}
	}
}

CLCACRI_API void GLDeviceContext::drawRect(const RectParams &params, Brush *brush, float width) {
}

CLCACRI_API void GLDeviceContext::fillRect(const RectParams &params, Brush *brush) {
	switch (brush->brushType) {
		case BrushType::SolidColor: {
			SolidColorBrush *b = (SolidColorBrush *)brush;
			float vertices[] = {
				b->color.r, b->color.g, b->color.b, b->color.a,
				params.left, params.bottom,

				b->color.r, b->color.g, b->color.b, b->color.a,
				params.left, params.top,

				b->color.r, b->color.g, b->color.b, b->color.a,
				params.right, params.bottom,

				b->color.r, b->color.g, b->color.b, b->color.a,
				params.left, params.top,

				b->color.r, b->color.g, b->color.b, b->color.a,
				params.right, params.bottom,

				b->color.r, b->color.g, b->color.b, b->color.a,
				params.right, params.top
			};

			std::lock_guard<std::mutex> rectSolidColorMutex(localDeviceResources.forRect.solidColorLock);

			ghalDeviceContext->setShaderProgram(((GLDevice *)device)->deviceResources.solidColorShaderProgram.get());
			ghalDeviceContext->bindVertexBuffer(localDeviceResources.forRect.solidColorVertexBuffer.get(), sizeof(float) * 18);
			ghalDeviceContext->bindVertexLayout(((GLDevice *)device)->deviceResources.solidColorVertexLayout.get());
			ghalDeviceContext->setData(localDeviceResources.forRect.solidColorVertexBuffer.get(), vertices);

			ghalDeviceContext->drawTriangle(2);
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
