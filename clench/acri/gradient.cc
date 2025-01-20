#include "gradient.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API Gradient::Gradient(ACRIDevice *device, GradientType gradientType) : ACRIResource(device), gradientType(gradientType) {
}

CLCACRI_API Gradient::~Gradient() {
}

CLCACRI_API LinearGradient::LinearGradient(ACRIDevice *device) : Gradient(device, GradientType::Linear) {
}

CLCACRI_API LinearGradient::~LinearGradient() {
}
