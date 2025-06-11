// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

#include "../shared/model.h"

#include "robotick/framework/Engine.h"
#include "robotick/framework/Model.h"
#include "robotick/platform/EntryPoint.h"
#include "robotick/platform/Signals.h"
#include "robotick/platform/Threading.h"

robotick::AtomicFlag g_stop_flag;

void signal_handler()
{
	g_stop_flag.set();
}

ROBOTICK_ENTRYPOINT
{
	robotick::setup_exit_handler(signal_handler);

	robotick::Model model;
	barr_e::populate_model_common(model);

	robotick::Engine engine;
	engine.load(model);
	engine.run(g_stop_flag);

#if !defined(ROBOTICK_PLATFORM_ESP32)
	return 0;
#endif
}