// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

#include "../shared/model.h"

#include "robotick/framework/Engine.h"
#include "robotick/framework/model/Model.h"
#include "robotick/platform/EntryPoint.h"
#include "robotick/platform/NetworkManager.h"
#include "robotick/platform/Signals.h"
#include "robotick/platform/Threading.h"

robotick::AtomicFlag g_stop_flag;

void signal_handler()
{
	g_stop_flag.set();
}

void start_wifi_hotspot()
{
	robotick::NetworkHotspotConfig hotspot_config;
	barr_e::get_network_hotspot_config(hotspot_config);

	ROBOTICK_INFO("==============================================================\n");
	ROBOTICK_INFO("BARR.e Brain - setting up wifi hotspot...");
	const bool hotspot_success = robotick::NetworkHotspot::start(hotspot_config);
	if (!hotspot_success)
	{
		ROBOTICK_FATAL_EXIT("BARR.e Brain - Failed to start wifi-hotspot!");
	}
	ROBOTICK_INFO("\n");
	ROBOTICK_INFO("==============================================================\n");
}

ROBOTICK_ENTRYPOINT
{
	// start a local wifi-hotspot (hard-coded creds for now) - needs security pass later:
	const bool should_start_wifi_hotspot = false;
	if (should_start_wifi_hotspot)
	{
		start_wifi_hotspot();
	}

	robotick::setup_exit_handler(signal_handler);

	robotick::Model model;
	barr_e::populate_model_brain(model);

	robotick::Engine engine;
	engine.load(model);
	engine.run(g_stop_flag);

	return 0;
}