// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

#include "../../shared/model.h"

#include "robotick/api.h"
#include "robotick/platform/EntryPoint.h"
#include "robotick/platform/NetworkManager.h"
#include "robotick/platform/Threading.h"

#include "esp_task_wdt.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Constants for engine task configuration
static constexpr const char* ENGINE_TASK_NAME = "robotick_main";
static constexpr uint32_t ENGINE_STACK_SIZE = 8192; // in bytes
static constexpr UBaseType_t ENGINE_TASK_PRIORITY = 5;
static constexpr BaseType_t ENGINE_CORE_ID = 1;

namespace robotick
{
	extern "C" void robotick_force_register_primitives();
	extern "C" void robotick_force_register_fixed_vector_types();

	void ensure_workloads()
	{
		ROBOTICK_KEEP_WORKLOAD(CameraWorkload)
		ROBOTICK_KEEP_WORKLOAD(SteeringMixerWorkload)
		ROBOTICK_KEEP_WORKLOAD(BaseXWorkload)
		ROBOTICK_KEEP_WORKLOAD(FaceDisplayWorkload)
		ROBOTICK_KEEP_WORKLOAD(SequencedGroupWorkload)

		robotick_force_register_primitives();
		robotick_force_register_fixed_vector_types();
	}

} // namespace robotick

void run_engine_on_core1(void* param)
{
	ROBOTICK_INFO("BARR.e Spine - Running on CPU%d", xPortGetCoreID());

	robotick::Model model;
	barr_e::populate_model_spine(model);

	ROBOTICK_INFO("BARR.e Spine - Loading Robotick model...");
	robotick::Engine engine;
	engine.load(model); // Ensures memory locality on Core 1

	ROBOTICK_INFO("BARR.e Spine - Starting tick loop...");
	robotick::AtomicFlag dummy_flag{false};
	engine.run(dummy_flag);
}

ROBOTICK_ENTRYPOINT
{
	ROBOTICK_INFO("BARR.e Spine - Started on CPU%d", xPortGetCoreID());

	robotick::ensure_workloads();

	// connect to our local wifi-hotspot (hard-coded creds for now) - needs security pass later:
	robotick::NetworkHotspotConfig hotspot_config;
	barr_e::get_network_hotspot_config(hotspot_config);

	robotick::NetworkClientConfig client_config;
	client_config.type = hotspot_config.type;
	client_config.iface = "wlan0"; // TBC - may not even be needed on ESP?
	client_config.ssid = hotspot_config.ssid;
	client_config.password = hotspot_config.password;

	ROBOTICK_INFO("==============================================================\n");
	ROBOTICK_INFO("BARR.e Brain - connecting to wifi hotspot...");
	const bool hotspot_success = robotick::NetworkClient::connect(client_config);
	if (!hotspot_success)
	{
		ROBOTICK_FATAL_EXIT("BARR.e Brain - Failed to connect to wifi-hotspot!");
	}
	ROBOTICK_INFO("\n");
	ROBOTICK_INFO("==============================================================\n");

	ROBOTICK_INFO("BARR.e Spine - Launching Robotick engine task on core 1...");

	xTaskCreatePinnedToCore(run_engine_on_core1, ENGINE_TASK_NAME, ENGINE_STACK_SIZE, nullptr, ENGINE_TASK_PRIORITY, nullptr, ENGINE_CORE_ID);
}
