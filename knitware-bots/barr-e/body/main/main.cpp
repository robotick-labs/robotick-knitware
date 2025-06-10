// Copyright Robotick Labs
// SPDX-License-Identifier: Apache-2.0

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

void populate_model_esp32(robotick::Model& model)
{
	auto steering_mixer = model.add("SteeringMixerWorkload", "steering_mixer");
	auto basex = model.add("BaseXWorkload", "basex");
	auto heartbeat = model.add("HeartbeatDisplayWorkload", "heartbeat");

	model.connect("steering_mixer.outputs.left_motor", "basex.inputs.motor3_speed");
	model.connect("steering_mixer.outputs.right_motor", "basex.inputs.motor4_speed");

	std::vector<robotick::WorkloadHandle> esp32_children = {steering_mixer, basex, heartbeat};
	auto esp32_root = model.add("SequencedGroupWorkload", "esp_control_sequence", esp32_children, 30.0);

	model.set_root(esp32_root);
}

void populate_model_host(robotick::Model& model, const robotick::Model& remote_esp32_model)
{
	// Register remote model for device
	model.add_remote_model(remote_esp32_model, "esp32", "uart:/dev/ttyUSB0"
		/* or: "ip:192.168.1.42"
		   or: "ip:esp32.local"
		   or: "ip:localhost"  // for simulation/testing
		   or: "local"         // force host-local execution */
	);

	// Host Workloads:
	auto remote_control = model.add("RemoteControlWorkload", "remote_control");
	auto console_telem = model.add("ConsoleTelemetryWorkload", "console", 5.0);

	// Data Connections:
	model.connect("remote_control.outputs.left_x", "|esp32|steering_mixer.inputs.turn_rate");
	model.connect("remote_control.outputs.left_y", "|esp32|steering_mixer.inputs.speed");

	model.connect("|esp32|basex.outputs.motor3_speed", "console.inputs.motor3_speed");
	model.connect("|esp32|basex.outputs.motor4_speed", "console.inputs.motor4_speed");

	// Group everything
	std::vector<robotick::WorkloadHandle> synced_group = {remote_control, console_telem};
	auto root = model.add("SyncedGroupWorkload", "main", synced_group, 30.0);

	model.set_root(root);
}

void populate_model_common(robotick::Model& model)
{
	robotick::Model esp32_model;
	populate_model_esp32(esp32_model);

	populate_model_host(model, esp32_model);
}

ROBOTICK_ENTRYPOINT
{
	robotick::setup_exit_handler(signal_handler);

	robotick::Model model;

#if defined(ROBOTICK_PLATFORM_ESP32)
	populate_model_esp32(model); // ^- temp measure - we would ideally have generic code on all hosts
#else
	populate_model_common(model);
#endif // #if defined(ROBOTICK_PLATFORM_ESP32)

	robotick::Engine engine;
	engine.load(model);
	engine.run(g_stop_flag);

#if !defined(ROBOTICK_PLATFORM_ESP32)
	return 0;
#endif
}