#pragma once

#include "robotick/api.h"
#include "robotick/platform/NetworkManager.h"

namespace barr_e
{
	static inline void get_network_hotspot_config(robotick::NetworkHotspotConfig& hotspot_config)
	{
		hotspot_config.ssid = "BARR.e";
		hotspot_config.password = "tortoise123";
		hotspot_config.iface = "wlan0";

		// or run manually: sudo nmcli dev wifi hotspot ifname wlan0 ssid BARR.e password tortoise123
	}

	static inline void populate_model_spine(robotick::Model& model)
	{
		static const float control_tick_rate_hz = 30.0f;
		static const float ui_tick_rate_hz = 15.0f;
		static const float root_tick_rate_hz = control_tick_rate_hz;

		// control workloads:
		static const robotick::WorkloadSeed steering_mixer{
			robotick::TypeId("SteeringMixerWorkload"), robotick::StringView("steering_mixer"), control_tick_rate_hz};

		static const robotick::WorkloadSeed basex{robotick::TypeId("BaseXWorkload"), robotick::StringView("basex"), control_tick_rate_hz};

		static const robotick::WorkloadSeed* control_children[] = {&steering_mixer, &basex};

		static const robotick::WorkloadSeed control_group{
			robotick::TypeId("SequencedGroupWorkload"), robotick::StringView("control_sequence"), control_tick_rate_hz, control_children};

		// UI workload(s)
		static const robotick::WorkloadSeed heart_ui{robotick::TypeId("HeartbeatDisplayWorkload"), robotick::StringView("heart_ui"), ui_tick_rate_hz};

		// root synced-group:
		static const robotick::WorkloadSeed* synced_children[] = {&control_group, &heart_ui};

		static const robotick::WorkloadSeed esp32_root{
			robotick::TypeId("SyncedGroupWorkload"), robotick::StringView("esp_synced_group"), root_tick_rate_hz, synced_children};

		static const robotick::WorkloadSeed* all_workloads[] = {&steering_mixer, &basex, &heart_ui, &esp32_root, &control_group};

		// --- Local Data Connections ---
		static const robotick::DataConnectionSeed conn_left_motor("steering_mixer.outputs.left_motor", "basex.inputs.motor1_speed");
		static const robotick::DataConnectionSeed conn_right_motor("steering_mixer.outputs.right_motor", "basex.inputs.motor2_speed");

		static const robotick::DataConnectionSeed* all_connections[] = {&conn_left_motor, &conn_right_motor};

		// --- Finalize model ---
		model.use_workload_seeds(all_workloads);
		model.use_data_connection_seeds(all_connections);
		model.set_root_workload(esp32_root);
	}

#ifdef ROBOTICK_ENABLE_MODEL_HEAP

	static inline void populate_model_brain(robotick::Model& model)
	{
		static const float tick_rate_hz_main = 30.0f;
		static const float tick_rate_hz_console = 5.0f; //  no real benefit of debug-telemetry being faster than this

		// Register remote model for device
		model.add_remote_model("spine", "ip:10.42.0.60")
			.connect("remote_control.outputs.left.x", "steering_mixer.inputs.turn_rate")
			.connect("remote_control.outputs.left.y", "steering_mixer.inputs.speed");

		// or: "uart:/dev/ttyACM1"
		// or: "ip:esp32.local"
		// or: "ip:localhost"  // for simulation/testing
		// or: "local"         // force host-local execution */

		const robotick::WorkloadSeed& remote_control = model.add("RemoteControlWorkload", "remote_control").set_tick_rate_hz(tick_rate_hz_main);

		const robotick::WorkloadSeed& face = model.add("FaceDisplayWorkload", "face").set_tick_rate_hz(tick_rate_hz_main);

		const robotick::WorkloadSeed& camera = model.add("CameraWorkload", "camera").set_tick_rate_hz(tick_rate_hz_main);

		const robotick::WorkloadSeed& console_telem = model.add("ConsoleTelemetryWorkload", "console").set_tick_rate_hz(tick_rate_hz_console);

		const robotick::WorkloadSeed& root = model.add("SyncedGroupWorkload", "root_group")
												 .set_children({&console_telem, &remote_control, &face, &camera})
												 .set_tick_rate_hz(tick_rate_hz_main);

		model.connect("camera.outputs.jpeg_data", "remote_control.inputs.jpeg_data");

		model.set_root_workload(root);
	}

#endif // #ifdef ROBOTICK_ENABLE_MODEL_HEAP

} // namespace barr_e