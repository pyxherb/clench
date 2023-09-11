#include "window.h"

#include <stdexcept>

using namespace clench;
using namespace clench::graphics;

std::unique_ptr<std::unordered_map<std::string, WindowBackend*>> _windowBackends;

WindowBackend::WindowBackend(std::string name) : _name(name) {
	if (!_windowBackends)
		_windowBackends = std::make_unique<std::unordered_map<std::string, WindowBackend*>>();
	(*_windowBackends)[name] = this;
}

Window* graphics::createWindow(
	CreateWindowFlags flags,
	std::wstring title,
	std::deque<std::string> preferredBackends) {
	std::deque<WindowBackend*> _windowBackendConditionQueue;

	{
		auto factories = *_windowBackends;

		for (auto& i : preferredBackends) {
			if (factories.count(i))
				_windowBackendConditionQueue.push_back(factories.at(i));
			factories.erase(i);
		}

		for (auto& i : factories) {
			_windowBackendConditionQueue.push_back(i.second);
		}
	}

	for (auto i : _windowBackendConditionQueue) {
		auto window = i->createWindow(flags, title);
		if (window)
			return window;
	}

	throw std::runtime_error("No available window backend");
}
