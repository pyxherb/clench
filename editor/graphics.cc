#include "graphics.h"

using namespace clench;

std::unique_ptr<ghal::Device, peff::DeallocableDeleter<ghal::Device>> clench::editor::g_mainGhalDevice;
std::unique_ptr<acri::Device, peff::DeallocableDeleter<acri::Device>> clench::editor::g_mainAcriDevice;
