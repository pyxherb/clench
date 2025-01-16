#include "gradient.h"

using namespace clench;
using namespace clench::acri;

CLCACRI_API LinearGradient::LinearGradient(ACRIContext *context) : Gradient(context, GradientType::Linear) {
}

CLCACRI_API LinearGradient::~LinearGradient() {
}
