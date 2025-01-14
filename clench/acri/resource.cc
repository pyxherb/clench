#include "resource.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API ACRIResource::ACRIResource(ACRIContext *context) : context(context) {
}

CLCACRI_API ACRIResource::~ACRIResource() {
}

CLCACRI_API void ACRIResource::onRefZero() {
	dealloc();
}
