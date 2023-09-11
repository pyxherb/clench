#include "backend.h"
#include <clench/debug.h>

using namespace clench::graphics;

static std::unique_ptr<std::map<std::string, GraphicsBackend*>> _registeredBackends;

GraphicsBackend::GraphicsBackend(std::string name) {
	if (!_registeredBackends)
		_registeredBackends = std::make_unique<std::map<std::string, GraphicsBackend*>>();
	(*_registeredBackends)[name] = this;
	clench::debug::info("Registered graphics backend: " + name);
}

void clench::graphics::initBackends() {
	for (auto& i : *_registeredBackends)
		i.second->init();
}

void clench::graphics::deinitBackends() {
	for (auto& i : *_registeredBackends)
		i.second->deinit();
}

GraphicsContext* clench::graphics::createGraphicsContext(Window* window, std::deque<std::string> preferredBackends) {
	auto factories = *_registeredBackends;
	std::deque<GraphicsBackend*> backendQueue;

	for (auto& i : preferredBackends) {
		if (factories.count(i))
			backendQueue.push_back(factories.at(i));
		factories.erase(i);
	}

	for (auto& i : factories)
		backendQueue.push_back(i.second);

	for (auto i : backendQueue) {
		auto context = i->createGraphicsContext(window);
		if (context)
			return context;
	}

	throw std::runtime_error("No available graphics backend");
}
