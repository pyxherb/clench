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
				g_triangle_solidcolor_fill_vertex_330,
				g_triangle_solidcolor_fill_vertex_330_length,
				nullptr,
				solidColorVertexShader.getRef()));

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createFragmentShader(
				g_triangle_solidcolor_fill_fragment_330,
				g_triangle_solidcolor_fill_fragment_330_length,
				nullptr,
				solidColorFragmentShader.getRef()));

		ghal::Shader *shaders[] = {
			solidColorVertexShader.get(),
			solidColorFragmentShader.get()
		};

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->linkShaderProgram(
				shaders, std::size(shaders),
				ptr->deviceResources.solidColorFillShaderProgram.getRef()));

		{
			static clench::ghal::VertexLayoutElementDesc descs[] = {
				{ clench::ghal::InputVertexShaderSemanticType::Position,
					0,
					{ clench::ghal::ShaderElementType::Float, 2, 1, 1 },
					sizeof(float) * 2,
					0 }
			};

			CLENCH_RETURN_IF_EXCEPT(
				ghalDevice->createVertexLayout(
					descs,
					std::size(descs),
					solidColorVertexShader.get(),
					ptr->deviceResources.solidColorFillVertexLayout.getRef()));
		}
	}

	{
		peff::RcObjectPtr<ghal::VertexShader> solidColorVertexShader;
		peff::RcObjectPtr<ghal::FragmentShader> solidColorFragmentShader;

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createVertexShader(
				g_ellipse_solidcolor_fill_vertex_330,
				g_ellipse_solidcolor_fill_vertex_330_length,
				nullptr,
				solidColorVertexShader.getRef()));

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createFragmentShader(
				g_ellipse_solidcolor_fill_fragment_330,
				g_ellipse_solidcolor_fill_fragment_330_length,
				nullptr,
				solidColorFragmentShader.getRef()));

		ghal::Shader *shaders[] = {
			solidColorVertexShader.get(),
			solidColorFragmentShader.get()
		};

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->linkShaderProgram(
				shaders, std::size(shaders),
				ptr->deviceResources.solidColorEllipseFillShaderProgram.getRef()));
	}

	{
		peff::RcObjectPtr<ghal::VertexShader> solidColorVertexShader;
		peff::RcObjectPtr<ghal::FragmentShader> solidColorFragmentShader;

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createVertexShader(
				g_ellipse_solidcolor_draw_vertex_330,
				g_ellipse_solidcolor_draw_vertex_330_length,
				nullptr,
				solidColorVertexShader.getRef()));

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->createFragmentShader(
				g_ellipse_solidcolor_draw_fragment_330,
				g_ellipse_solidcolor_draw_fragment_330_length,
				nullptr,
				solidColorFragmentShader.getRef()));

		ghal::Shader *shaders[] = {
			solidColorVertexShader.get(),
			solidColorFragmentShader.get()
		};

		CLENCH_RETURN_IF_EXCEPT(
			ghalDevice->linkShaderProgram(
				shaders, std::size(shaders),
				ptr->deviceResources.solidColorEllipseDrawShaderProgram.getRef()));

		{
			static clench::ghal::VertexLayoutElementDesc descs[] = {
				{ clench::ghal::InputVertexShaderSemanticType::Position,
					0,
					{ clench::ghal::ShaderElementType::Float, 2, 1, 1 },
					sizeof(float) * 2,
					0 }
			};

			CLENCH_RETURN_IF_EXCEPT(
				ghalDevice->createVertexLayout(
					descs,
					std::size(descs),
					solidColorVertexShader.get(),
					ptr->deviceResources.solidColorDrawVertexLayout.getRef()));
		}
	}

	deviceOut = ptr.release();
	return {};
}

base::ExceptionPtr GLBackend::createDeviceContext(
	ghal::DeviceContext *ghalDeviceContext,
	Device *acriDevice,
	DeviceContext *&deviceContextOut) {
	std::unique_ptr<GLDeviceContext, peff::RcObjectUniquePtrDeleter> ptr(
		GLDeviceContext::alloc(acriDevice, ghalDeviceContext));
	if (!ptr)
		return base::OutOfMemoryException::alloc();

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(float) * 2 * 3;
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::Vertex;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forTriangle.solidColorFillVertexBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(TriangleSolidColorFillRenderInfo);
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::UniformBuffer;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forTriangle.solidColorFillUniformBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(float) * 2 * 3 * 2;
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::Vertex;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forRect.solidColorFillVertexBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(TriangleSolidColorFillRenderInfo);
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::UniformBuffer;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forRect.solidColorFillUniformBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(float) * 2 * 3 * 2;
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::Vertex;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forEllipse.solidColorFillVertexBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(EllipseSolidColorFillRenderInfo);
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::UniformBuffer;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forEllipse.solidColorFillUniformBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(float) * 2 * 3 * 2;
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::Vertex;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forEllipse.solidColorDrawVertexBuffer.getRef()));
	}

	{
		ghal::BufferDesc bufDesc;

		bufDesc.size = sizeof(EllipseSolidColorDrawRenderInfo);
		bufDesc.usage = ghal::BufferUsage::Dynamic;
		bufDesc.proposedTarget = ghal::BufferTarget::UniformBuffer;
		bufDesc.cpuWritable = true;
		bufDesc.cpuReadable = false;

		CLENCH_RETURN_IF_EXCEPT(
			acriDevice->associatedDevice->createBuffer(
				bufDesc,
				nullptr,
				ptr->localDeviceResources.forEllipse.solidColorDrawUniformBuffer.getRef()));
	}

	ptr->incRef();
	deviceContextOut = ptr.release();
	return {};
}

CLCACRI_API void GLBackend::dealloc() {
	peff::destroyAndRelease<GLBackend>(selfAllocator.get(), this, sizeof(std::max_align_t));
}

CLCACRI_API GLBackend *GLBackend::alloc(peff::Alloc *selfAllocator) {
	return peff::allocAndConstruct<GLBackend>(selfAllocator, sizeof(std::max_align_t), selfAllocator);
}
