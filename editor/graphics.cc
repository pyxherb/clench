#include "graphics.h"

std::unique_ptr<clench::ghal::GHALDevice, peff::DeallocableDeleter> clench::engine::g_mainGhalDevice;
