#include "graphics.h"

using namespace clench;
using namespace clench::ghal;

std::unique_ptr<clench::ghal::GHALDevice, peff::DeallocableDeleter<GHALDevice>> clench::editor::g_mainGhalDevice;
