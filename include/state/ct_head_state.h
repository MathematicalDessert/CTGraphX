#pragma once

#include <memory>

#include "ct/ct_head.h"

namespace state {
	struct CTHeadState {
		std::unique_ptr<ct::CTHead> source;
	};
}