// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

#include "../../shared/model.h"

#include "robotick/framework/Engine.h"
#include "robotick/framework/Model.h"
#include "robotick/framework/registry/WorkloadRegistry.h"
#include "robotick/platform/Threading.h"

#include "esp_log.h"
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
	void ensure_workloads()
	{
		ROBOTICK_KEEP_WORKLOAD(SteeringMixerWorkload)
		ROBOTICK_KEEP_WORKLOAD(BaseXWorkload)
		ROBOTICK_KEEP_WORKLOAD(HeartbeatDisplayWorkload)
		ROBOTICK_KEEP_WORKLOAD(SequencedGroupWorkload)
	}

} // namespace robotick

void run_engine_on_core1(void* param)
{
	ESP_LOGI(ENGINE_TASK_NAME, "Running on CPU%d", xPortGetCoreID());

	auto* engine = static_cast<robotick::Engine*>(param);

	robotick::Model model;
	barr_e::populate_model_spine(model);

	ESP_LOGI("Robotick", "Loading Robotick model...");
	engine->load(model); // Ensures memory locality on Core 1

	ESP_LOGI("Robotick", "Starting tick loop...");
	robotick::AtomicFlag dummy_flag{false};
	engine->run(dummy_flag);

	vTaskDelete(nullptr);
}

extern "C" void app_main(void)
{
	ESP_LOGI("main_task", "Started on CPU%d", xPortGetCoreID());
    
	// Kill Watchdog for this task (your main thread)
    esp_task_wdt_delete(nullptr);

    // Disable the watchdog for the idle task on Core 1
    TaskHandle_t idle1 = xTaskGetIdleTaskHandleForCore(1);
    esp_task_wdt_delete(idle1);

	robotick::ensure_workloads();

	static robotick::Engine engine;

	ESP_LOGI("Robotick", "Launching Robotick engine task on core 1...");

	xTaskCreatePinnedToCore(run_engine_on_core1, ENGINE_TASK_NAME, ENGINE_STACK_SIZE, &engine, ENGINE_TASK_PRIORITY, nullptr, ENGINE_CORE_ID);
}
