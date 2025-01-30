#include "backend.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API GLBackend::GLBackend(peff::Alloc *selfAllocator) : Backend("opengl", selfAllocator) {
}

CLCACRI_API GLBackend::~GLBackend() {
}

CLCACRI_API base::ExceptionPtr GLBackend::createDevice(ghal::Device *ghalDevice, peff::Alloc *selfAllocator, peff::Alloc *resourceAllocator, Device *&deviceOut) {
	std::unique_ptr<GLDevice, peff::DeallocableDeleter<GLDevice>> ptr(GLDevice::alloc(ghalDevice, selfAllocator, resourceAllocator));
	if (!ptr)
		return base::OutOfMemoryException::alloc();

	{
		peff::RcObjectPtr<ghal::VertexShader> solidColorVertexShader;
		peff::RcObjectPtr<ghal::FragmentShader> solidColorFragmentShader;

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createVertexShader(
				g_triangle_solidcolor_vertex_330,
				g_triangle_solidcolor_vertex_330_length,
				nullptr,
				solidColorVertexShader.getRef()));

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createFragmentShader(
				g_triangle_solidcolor_fragment_330,
				g_triangle_solidcolor_fragment_330_length,
				nullptr,
				solidColorFragmentShader.getRef()));

		ghal::Shader *shaders[] = {
			solidColorVertexShader.get(),
			solidColorFragmentShader.get()
		};

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->linkShaderProgram(
				shaders, std::size(shaders),
				ptr->deviceResources.forTriangle.solidColorShaderProgram.getRef()));

		static clench::ghal::VertexLayoutElementDesc descs[] = {
			{ clench::ghal::InputVertexShaderSemanticType::Color,
				0,
				{ clench::ghal::VertexElementType::Float, 4 },
				0,
				0 },
			{ clench::ghal::InputVertexShaderSemanticType::Position,
				0,
				{ clench::ghal::VertexElementType::Float, 2 },
				sizeof(float) * 2,
				sizeof(float) * 4 }
		};

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createVertexLayout(
				descs,
				std::size(descs),
				solidColorVertexShader.get(),
				ptr->deviceResources.forTriangle.solidColorVertexLayout.getRef()));
	}

	deviceOut = ptr.release();
	return {};
}

CLCACRI_API void GLBackend::dealloc() {
	peff::destroyAndRelease<GLBackend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API GLBackend *GLBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
