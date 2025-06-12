// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

#include "../shared/model.h"

#include "robotick/framework/Engine.h"
#include "robotick/framework/Model.h"
#include "robotick/platform/EntryPoint.h"
#include "robotick/platform/NetworkManager.h"
#include "robotick/platform/Signals.h"
#include "robotick/platform/Threading.h"

robotick::AtomicFlag g_stop_flag;

void signal_handler()
{
	g_stop_flag.set();
}

ROBOTICK_ENTRYPOINT
{
	robotick::NetworkHotspotConfig hotspot_config;
	if(!robotick::NetworkHotspot::start(hotspot_config)); // start a local wifi-hotspot (default creds for now) - needs security pass later
	{
		ROBOTICK_FATAL_EXIT("BARR.e Brain - Failed to start wifi-hotspot!");
	}

	robotick::setup_exit_handler(signal_handler);

	robotick::Model model;
	barr_e::populate_model_common(model);

	robotick::Engine engine;
	engine.load(model);
	engine.run(g_stop_flag);

	robotick::NetworkHotspot::stop(); // stop our local wifi-hotspot

	return 0;
}