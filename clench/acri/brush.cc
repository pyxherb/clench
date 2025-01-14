#include "brush.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Brush::Brush(ACRIContext *context, BrushType brushType) : ACRIResource(context), brushType(brushType) {
}

CLCACRI_API Brush::~Brush() {
}
