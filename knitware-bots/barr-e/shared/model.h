#pragma once

#include "robotick/framework/Model.h"
#include "robotick/platform/NetworkManager.h"

namespace barr_e
{
	static inline void get_network_hotspot_config(robotick::NetworkHotspotConfig& hotspot_config)
	{
		hotspot_config.ssid = "BARR.e";
		hotspot_config.password = "tortoise123";
		hotspot_config.iface = "wlp88s0f0";
	}

	static inline void populate_model_spine(robotick::Model& model)
	{
		auto steering_mixer = model.add("SteeringMixerWorkload", "steering_mixer");
		auto basex = model.add("BaseXWorkload", "basex");
		auto face = model.add("FaceDisplayWorkload", "face");

		model.connect("steering_mixer.outputs.left_motor", "basex.inputs.motor3_speed");
		model.connect("steering_mixer.outputs.right_motor", "basex.inputs.motor4_speed");

		std::vector<robotick::WorkloadHandle> esp32_children = {steering_mixer, basex, face};
		auto esp32_root = model.add("SequencedGroupWorkload", "esp_control_sequence", esp32_children, 30.0);

		model.set_root(esp32_root);
	}

	static inline void populate_model_brain(robotick::Model& model, const robotick::Model& remote_spine_model)
	{
		// Register remote model for device
		model.add_remote_model(remote_spine_model, "spine", "ip:10.42.0.60");
		// or: "uart:/dev/ttyACM1"
		// or: "ip:esp32.local"
		// or: "ip:localhost"  // for simulation/testing
		// or: "local"         // force host-local execution */

		// Host Workloads:
		auto remote_control = model.add("RemoteControlWorkload", "remote_control");
		auto console_telem = model.add("ConsoleTelemetryWorkload", "console", 5.0);

		// Data Connections:
		model.connect("remote_control.outputs.left_x", "|spine|steering_mixer.inputs.turn_rate");
		model.connect("remote_control.outputs.left_y", "|spine|steering_mixer.inputs.speed");

		// Group everything
		std::vector<robotick::WorkloadHandle> synced_group = {remote_control, console_telem};
		auto root = model.add("SyncedGroupWorkload", "main", synced_group, 30.0);

		model.set_root(root);
	}

	static inline void populate_model_common(robotick::Model& model)
	{
		robotick::Model remote_spine_model;
		populate_model_spine(remote_spine_model);

		populate_model_brain(model, remote_spine_model);
	}

} // namespace barr_e