#include "device.h"
#include <cmath>

#undef max
#undef min

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
		math::Vec2f lastVec(params.vertices[0] - params.vertices[params.nVertices - 1]), curVec(params.vertices[1] - params.vertices[0]);

		bool sign = math::crossZ(lastVec, curVec) >= 0;

		for (size_t i = 1; i < params.nVertices - 1; ++i) {
			lastVec = curVec;
			curVec = params.vertices[i + 1] - params.vertices[i];

			if (sign != (math::crossZ(lastVec, curVec) >= 0)) {
				isConcave = true;
				goto concaveDetectionEnd;
			}
		}

		if (sign != (math::crossZ(curVec, params.vertices[0] - params.vertices[params.nVertices - 1]) >= 0)) {
			isConcave = true;
		}
	}

concaveDetectionEnd:;

	switch (brush->brushType) {
		case BrushType::SolidColor: {
			SolidColorBrush *b = (SolidColorBrush *)brush;

			if (isConcave) {
				bool isAntiClockwise = false;
				{
					size_t idxVertexWithMaxY;
					float maxY = -INFINITY;

					for (size_t i = 0; i < params.nVertices; ++i) {
						if (maxY < params.vertices[i].y) {
							idxVertexWithMaxY = i;
							maxY = params.vertices[i].y;
						}
					}

					size_t idxFormer, idxLatter;
					if (!idxVertexWithMaxY) {
						idxFormer = params.nVertices - 1;
						idxLatter = idxVertexWithMaxY + 1;
					} else if (idxVertexWithMaxY + 1 == params.nVertices) {
						idxFormer = idxVertexWithMaxY - 1;
						idxLatter = 0;
					} else {
						idxFormer = idxVertexWithMaxY - 1;
						idxLatter = idxVertexWithMaxY + 1;
					}

					if (math::crossZ(params.vertices[idxVertexWithMaxY] - params.vertices[idxFormer], params.vertices[idxLatter] - params.vertices[idxFormer]) >= 0) {
						isAntiClockwise = true;
					}
				}

				peff::Set<size_t> earVertices, idxVertices;

				// Find out all convex vertices and create the ear vertex set and the total vertex set.
				{
					peff::Set<size_t> convexVertices;
					if (isAntiClockwise) {
						math::Vec2f a, b;

						for (size_t i = 0; i < params.nVertices - 3; ++i) {
							a = params.vertices[i + 1] - params.vertices[i];
							b = params.vertices[i + 2] - params.vertices[i];

							if (math::crossZ(a, b) < 0) {
							} else {
								convexVertices.insert(i + 1);
							}
							idxVertices.insert(i + 1);
						}

						a = params.vertices[params.nVertices - 3] - params.vertices[params.nVertices - 2];
						b = params.vertices[params.nVertices - 3] - params.vertices[params.nVertices - 1];

						if (math::crossZ(a, b) < 0) {
						} else {
							convexVertices.insert(params.nVertices - 3 + 1);
						}
						idxVertices.insert(params.nVertices - 3 + 1);

						a = params.vertices[params.nVertices - 2] - params.vertices[params.nVertices - 1];
						b = params.vertices[params.nVertices - 2] - params.vertices[0];

						if (math::crossZ(a, b) < 0) {
						} else {
							convexVertices.insert(params.nVertices - 2 + 1);
						}
						idxVertices.insert(params.nVertices - 2 + 1);

						a = params.vertices[params.nVertices - 1] - params.vertices[0];
						b = params.vertices[params.nVertices - 1] - params.vertices[1];

						if (math::crossZ(a, b) < 0) {
							;
						} else {
							convexVertices.insert(0);
						}
						idxVertices.insert(0);
					} else {
						math::Vec2f a, b;

						for (size_t i = params.nVertices - 1; i > 2; --i) {
							a = params.vertices[i - 1] - params.vertices[i];
							b = params.vertices[i - 2] - params.vertices[i];

							if (math::crossZ(a, b) < 0) {
							} else {
								convexVertices.insert(i - 1);
							}
							idxVertices.insert(i - 1);
						}

						a = params.vertices[1] - params.vertices[2];
						b = params.vertices[0] - params.vertices[2];

						if (math::crossZ(a, b) < 0) {
						} else {
							convexVertices.insert(1);
						}
						idxVertices.insert(1);

						a = params.vertices[0] - params.vertices[1];
						b = params.vertices[params.nVertices - 1] - params.vertices[1];

						if (math::crossZ(a, b) < 0) {
						} else {
							convexVertices.insert(0);
						}
						idxVertices.insert(0);

						a = params.vertices[params.nVertices - 1] - params.vertices[0];
						b = params.vertices[params.nVertices - 1 - 1] - params.vertices[0];

						if (math::crossZ(a, b) < 0) {
						} else {
							convexVertices.insert(params.nVertices - 1);
						}
						idxVertices.insert(params.nVertices - 1);
					}

					for (auto i : convexVertices) {
						size_t idxFormer, idxLatter;

						if (!i) {
							idxFormer = *idxVertices.beginReversed();
						} else {
							idxFormer = i - 1;
						}

						if (i == *idxVertices.beginReversed()) {
							idxLatter = 0;
						} else {
							idxLatter = i + 1;
						}

						for (auto j : idxVertices) {
							if ((j == idxFormer) || (j == idxLatter) || (j == i))
								continue;

							math::Vec2f
								pa = params.vertices[idxFormer] - params.vertices[j],
								pb = params.vertices[i] - params.vertices[j],
								pc = params.vertices[idxLatter] - params.vertices[j];
							{
								float prodA =
									math::crossZ(pa, pb);

								float prodB =
									math::crossZ(pb, pc);

								float prodC =
									math::crossZ(pc, pa);

								uint32_t signA = *((uint32_t *)&prodA) >> 31, signB = *((uint32_t *)&prodB) >> 31, signC = *((uint32_t *)&prodC) >> 31;

								if ((signA == signB) &&
									(signA == signC) &&
									(signB == signC)) {
									goto skipCurVertex;
								}
							}
						}

						earVertices.insert(+i);

					skipCurVertex:;
					}
				}

				peff::List<TriangleParams> triangles;

				// Try to apply ear-clipping algorithm for our polygon.
				while (idxVertices.size() > 3) {
					auto curEarIt = earVertices.begin();
					assert(curEarIt != earVertices.end());
					size_t idxCurEar = *curEarIt;

					size_t idxFormer, idxFormerFormer, idxLatter, idxLatterLatter;

					{
						auto latterIt = idxVertices.find(idxCurEar);
						++latterIt;
						if (latterIt == idxVertices.end()) {
							latterIt = idxVertices.begin();
						}

						auto latterLatterIt = latterIt;
						++latterLatterIt;
						if (latterLatterIt == idxVertices.end()) {
							latterLatterIt = idxVertices.begin();
						}

						idxLatter = *latterIt;
						idxLatterLatter = *latterLatterIt;
					}
					if (idxCurEar) {
						if (auto it = idxVertices.findMaxLteq(idxCurEar - 1); it != idxVertices.end()) {
							idxFormer = *it;
						} else {
							idxFormer = *idxVertices.beginReversed();
						}
					} else {
						idxFormer = *idxVertices.beginReversed();
					}
					if (idxFormer) {
						if (auto it = idxVertices.findMaxLteq(idxFormer - 1); it != idxVertices.end()) {
							idxFormerFormer = *it;
						} else {
							idxFormerFormer = *idxVertices.beginReversed();
						}
					} else {
						idxFormerFormer = *idxVertices.beginReversed();
					}

					bool isFormerVertexConvex = false, isLatterVertexConvex = false;
					math::Vec2f a, b;

					a = params.vertices[idxFormer] - params.vertices[idxFormerFormer];
					b = params.vertices[idxLatter] - params.vertices[idxFormerFormer];

					if (math::crossZ(a, b) >= 0) {
						isFormerVertexConvex = true;
					} else {
						isFormerVertexConvex = false;
					}

					a = params.vertices[idxLatter] - params.vertices[idxFormer];
					b = params.vertices[idxLatterLatter] - params.vertices[idxFormer];

					if (math::crossZ(a, b) >= 0) {
						isLatterVertexConvex = true;
					} else {
						isLatterVertexConvex = false;
					}

					idxVertices.remove(idxCurEar);
					earVertices.remove(idxCurEar);

					if (isFormerVertexConvex) {
						for (auto j : idxVertices) {
							if ((j == idxFormer) || (j == idxLatter) || (j == idxFormerFormer))
								continue;

							math::Vec2f
								pa = params.vertices[idxFormerFormer] - params.vertices[j],
								pb = params.vertices[idxFormer] - params.vertices[j],
								pc = params.vertices[idxLatter] - params.vertices[j];
							{
								float prodA =
									math::crossZ(pa, pb);

								float prodB =
									math::crossZ(pb, pc);

								float prodC =
									math::crossZ(pc, pa);

								uint32_t signA = *((uint32_t *)&prodA) >> 31, signB = *((uint32_t *)&prodB) >> 31, signC = *((uint32_t *)&prodC) >> 31;

								if ((signA == signB) &&
									(signA == signC) &&
									(signB == signC)) {
									goto skipCurFormerVertex;
								}
							}
						}

						earVertices.insert(+idxFormer);
					skipCurFormerVertex:;
					}

					if (isLatterVertexConvex) {
						for (auto j : idxVertices) {
							if ((j == idxFormer) || (j == idxLatter) || (j == idxLatterLatter))
								continue;

							math::Vec2f
								pa = params.vertices[idxFormer] - params.vertices[j],
								pb = params.vertices[idxLatter] - params.vertices[j],
								pc = params.vertices[idxLatterLatter] - params.vertices[j];
							{
								float prodA =
									math::crossZ(pa, pb);

								float prodB =
									math::crossZ(pb, pc);

								float prodC =
									math::crossZ(pc, pa);

								uint32_t signA = *((uint32_t *)&prodA) >> 31, signB = *((uint32_t *)&prodB) >> 31, signC = *((uint32_t *)&prodC) >> 31;

								if ((signA == signB) &&
									(signA == signC) &&
									(signB == signC)) {
									goto skipCurLatterVertex;
								}
							}
						}

						earVertices.insert(+idxLatter);

					skipCurLatterVertex:;
					}

					TriangleParams curParams;

					curParams.vertices[0] = params.vertices[idxFormer];
					curParams.vertices[1] = params.vertices[idxCurEar];
					curParams.vertices[2] = params.vertices[idxLatter];

					triangles.pushBack(std::move(curParams));
				}

				// Push the last triangle.
				TriangleParams curParams;

				curParams.vertices[0] = params.vertices[*idxVertices.begin()];
				curParams.vertices[1] = params.vertices[*++idxVertices.begin()];
				curParams.vertices[2] = params.vertices[*idxVertices.beginReversed()];

				triangles.pushBack(std::move(curParams));

				for (auto &i : triangles) {
					fillTriangle(i, brush);
				}
			} else {
				for (size_t i = 2; i < params.nVertices; ++i) {
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
	int x, y, width, height;
	float minDepth, maxDepth;
	ghalDeviceContext->getViewport(x, y, width, height, minDepth, maxDepth);

	//
	// n >= PI/(arccos(1 - (hmax/R)))
	//
	// n: Number of segments.
	// hmax: Maximum chord height error rate (we set to size of one pixel in window coordinates).
	// R: Radius of the circle.
	//
	// Deduction of the formula:
	//
	// hmax = R-R*cos(PI/n)
	// hmax = R*(1-cos(PI/n))
	// hmax/R = 1-cos(PI/n)
	// hmax/R-1 = -cos(PI/n)
	// 1-hmax/R = cos(PI/n)
	// PI/n = acos(1-hmax/R)
	// n/PI = 1/acos(1-hmax/R)
	// n = PI*1/acos(1-hmax/R)
	//
	int maxAxisLen = std::max(width, height);
	float minSizePerPixel = 1.0f / (float)maxAxisLen;

	float maxRadius = std::max(params.radiusX * width, params.radiusY * height);

	size_t nSegments = (M_PIf / acosf(1.0f - minSizePerPixel / maxRadius) + 0.5f);

	float angle;
	float deltaAngle = M_PIf * 2 / nSegments;
	for (size_t i = 0; i < nSegments; ++i) {
		TriangleParams triangleParams;

		angle = deltaAngle * i;

		triangleParams.vertices[0] = { params.origin.x + params.radiusX * cosf(angle), params.origin.y + params.radiusY * sinf(angle) };

		triangleParams.vertices[1] = { params.origin.x, params.origin.y };

		angle = deltaAngle * (i + 1);

		triangleParams.vertices[2] = { params.origin.x + params.radiusX * cosf(angle), params.origin.y + params.radiusY * sinf(angle) };

		fillTriangle(triangleParams, brush);
	}
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
