#include "graphics.h"

using namespace clench;

std::unique_ptr<ghal::GHALDevice, peff::DeallocableDeleter<ghal::GHALDevice>> clench::editor::g_mainGhalDevice;
