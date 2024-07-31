#include "graphics.h"

std::unique_ptr<clench::rhi::RHIDevice> clench::engine::g_mainRhiDevice;
std::unique_ptr<clench::rhi::RHIDeviceContext> clench::engine::g_mainRhiDeviceContext;
