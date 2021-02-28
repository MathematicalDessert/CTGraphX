#include <memory>
#include <spdlog/spdlog.h>
#include <Eigen/Dense>

#include "application.h"

int main() {
	auto app = std::make_unique<Application>();

	if (!app->initialize()) {
		spdlog::error("Failed to start application!");
		return 1;
	}

	if (!app->run()) {
		spdlog::error("Failed to successfully run application!");
		return 2;
	}

	return !app->deinitialize();
}