#include "device.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API void GLACRIDevice::dealloc() {
	peff::destroyAndRelease<GLACRIDevice>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API GLACRIDevice *GLACRIDevice::alloc(ghal::GHALDevice *associatedDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator) {
	std::unique_ptr<GLACRIDevice, peff::RcObjectUniquePtrDeleter> ptr(
		peff::allocAndConstruct<GLACRIDevice>(selfAllocator, sizeof(std::max_align_t), associatedDevice, selfAllocator, resourceAllocator));
	if (!ptr) {
		return nullptr;
	}

	{
		peff::RcObjectPtr<ghal::VertexShader> solidColorVertexShader;
		peff::RcObjectPtr<ghal::FragmentShader> solidColorFragmentShader;

		if (!(solidColorVertexShader = associatedDevice->createVertexShader(
				  g_triangle_solidcolor_vertex_330,
				  g_triangle_solidcolor_vertex_330_len,
				  nullptr))) {
			return nullptr;
		}

		if (!(solidColorFragmentShader = associatedDevice->createFragmentShader(
				  g_triangle_solidcolor_fragment_330,
				  g_triangle_solidcolor_fragment_330_len,
				  nullptr))) {
			return nullptr;
		}

		ghal::Shader *shaders[] = {
			solidColorVertexShader.get(),
			solidColorFragmentShader.get()
		};

		if (!(ptr->deviceResources.forTriangle.solidColorShaderProgram =
					associatedDevice->linkShaderProgram(shaders, std::size(shaders)))) {
			return nullptr;
		}
	}

	return ptr.release();
}

CLCACRI_API void GLACRIDeviceContext::dealloc() {
	peff::destroyAndRelease<GLACRIDeviceContext>(device->resourceAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API void GLACRIDeviceContext::drawTriangle(TriangleGeometry *geometry, Brush *brush, float width) {
	switch (brush->brushType) {
		case BrushType::SolidColor: {
			SolidColorBrush *b = (SolidColorBrush *)brush;
			float vertices[] = {
				b->color.r, b->color.g, b->color.b, b->color.a,
				geometry->vertices[0].x, geometry->vertices[0].y, geometry->vertices[0].z,
				geometry->vertices[1].x, geometry->vertices[1].y, geometry->vertices[1].z,
				geometry->vertices[2].x, geometry->vertices[2].y, geometry->vertices[2].z
			};

			std::lock_guard<std::mutex> triangleSolidColorMutex(localDeviceResources.forTriangle.solidColorLock);

			ghalDeviceContext->bindVertexLayout(((GLACRIDevice *)device)->deviceResources.forTriangle.solidColorVertexLayout.get());
			ghalDeviceContext->bindVertexBuffer(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), sizeof(vertices));
			ghalDeviceContext->setData(localDeviceResources.forTriangle.solidColorVertexBuffer.get(), vertices);
			ghalDeviceContext->setShaderProgram(((GLACRIDevice *)device)->deviceResources.forTriangle.solidColorShaderProgram.get());

			break;
		}
	}
}

CLCACRI_API void GLACRIDeviceContext::fillTriangle(TriangleGeometry *geometry, Brush *brush) {
}

CLCACRI_API void GLACRIDeviceContext::drawEllipse(EllipseGeometry *geometry, Brush *brush, float width) {
}

CLCACRI_API void GLACRIDeviceContext::fillEllipse(EllipseGeometry *geometry, Brush *brush) {
}

CLCACRI_API void GLACRIDeviceContext::drawPath(PathGeometry *geometry, Brush *brush, float width) {
}

CLCACRI_API void GLACRIDeviceContext::fillPath(PathGeometry *geometry, Brush *brush) {
}
