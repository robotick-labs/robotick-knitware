# Power Architecture Selection – Magg.e v1

## 1. Overview
This document captures the rationale behind the selection and assignment of power sources for the Magg.e v1 prototype. The robot comprises two onboard compute subsystems (Pi2 + BrickPi3; Pi5 + stereo camera), along with a servo gimbal for head articulation. Reliable, safe, and efficient power delivery was a design priority.

---

## 2. Subsystem Power Requirements

| Item                           | Est. Typical Power | Est. Peak Power | Voltage | Notes |
|--------------------------------|--------------------|-----------------|---------|-------|
| Raspberry Pi 5 (incl. fan)     | 4.5 W              | 6.5 W           | 5V      | Idle vs. light compute |
| ROS2 + SLAM workload           | 8.0 W              | 12.0 W          | 5V      | Active SLAM processing |
| Intel RealSense D435           | 2.0 W              | 3.5 W           | 5V      | Depth streaming |
| OAK-D Lite (alt camera)        | 1.5 W              | 3.0 W           | 5V      | Alternative sensor |
| WiFi (on Pi5)                  | 1.0 W              | 1.5 W           | 5V      | Bursty usage |
| **Pi5 + Camera - Subtotal**    | **17.5 W**         | **23.5 W**      | 5V      | Typical = 4.5+8+2+1  |
| **2× Gimbal Servos (SG90-class)**  | **1.5 W**              | **5.0 W**           | 5V      | With cap buffering |
||||||
| Raspberry Pi 2                 | 2.5 W              | 4.0 W           | 5V      | Idle vs. peak load |
| BrickPi3 logic board           | 1.0 W              | 1.5 W           | 12V     | Board overhead |
| EV3 Motors ×4 (typical use)    | 2.0 W              | 2.5 W           | 12V     | Active movement |
| EV3 Motors ×4 (all stalled)    | 0.0 W              | 10.0 W          | 12V     | Stall worst-case |
| **Pi2 + BrickPi - Subtotal**   | **5.5 W**          | **18.0 W**      | 12V     | Typical = 2.5+1+2 |
||||||
| **Grand Total**                | **24.5 W**         | **46.5 W**      | Mixed   | Combined all subsystems |

---

## 3. Power Source Options Considered

| Power Source Model                                       | Weight | Capabilities                                         | Notes |
|----------------------------------------------------------|--------|------------------------------------------------------|-------|
| [Talentcell NB7102 17500mAh / 64.75Wh](https://www.amazon.co.uk/dp/B09GFR9K8Q) | ~440g  | USB-C PD (5V/3A), USB-A (5V/2.4A), DC1 (24V/3A), DC2 (19.5V/3A) | ✔️ Powers Pi5 + camera + gimbal; dedicated DC for BrickPi not used. |
| [YABO YB120300 / Talentcell 3000mAh (12V/3A)](https://www.amazon.com/dp/B01M7Z9Z1N) | ~200g  | DC barrel (12V/3A)                                    | ✔️ Powers BrickPi + Pi2; light and dedicated. |
| Compact PD-only bank (~10,000mAh class)                  | ~250g  | USB-C PD (5V/3A, 18W), USB-A shared                  | ❌ Underpowered for combined loads. |
| USB-A only bank                                          | ~200g  | 5V/2.4A (12W)                                        | ❌ Not suitable for above subsystems. |

---

## 4. Chosen Configuration & Power Delivery Mapping

| Power Bank       | Output Port      | Output Voltage / Max | Avg Output Power | Peak Output Power | Target Device              | Input Voltage / Req. | Avg Input Power | Peak Input Power | Headroom (Avg) | Headroom (Peak) |
|------------------|------------------|-----------------------|------------------|-------------------|----------------------------|-----------------------|------------------|------------------|----------------|-----------------|
| NB7102 (~440g)   | USB-C PD         | 5V @ 3A (15W max)     | 17.5 W           | 23.5 W            | Pi5 + Camera + WiFi        | 5V                    | 17.5 W           | 23.5 W           | -2.5 W (-17%)  | -8.5 W (-57%)   |
| NB7102 (~440g)   | USB-A            | 5V @ 2.4A (12W max)   | 1.5 W            | 5.0 W             | 2× Servo Gimbal            | 5V                    | 1.5 W            | 5.0 W            | 10.5 W (700%)  | 7.0 W (140%)    |
| YB120300 (~200g) | DC barrel        | 12V @ 3A (36W max)    | 5.5 W            | 18.0 W            | Pi2 + BrickPi              | 12V                   | 5.5 W            | 18.0 W           | 30.5 W (554%)  | 18.0 W (100%)   |

*Headroom = Output capacity minus Input requirement*

### Servo Control Channel Plan

- Servo PWM control will be driven from the **Pi5’s GPIO**, using either:
  - A direct GPIO PWM signal
  - Or via a driver like the **PCA9685** over I²C
- Power and logic **grounds will be tied** between Pi5 and servo board to ensure reference stability
- Servo power will be kept electrically **isolated from Pi5’s power rail** to avoid noise or load interference

---

## 5. Risks and Mitigations

| Risk                              | Mitigation |
|-----------------------------------|------------|
| USB-C PD under-delivery for Pi5   | Consider lowering SLAM workload or using external 9V PD trigger + buck |
| Servo stalls causing USB-A dropouts | Limit gimbal speed, use capacitor buffering |
| BrickPi peak draw on YB120300     | Ensure motor commands avoid simultaneous stall, monitor current |
| Shared ground instability         | All GND rails tied; test for potential loops |
| Weight impacting balance          | Batteries mounted low for center of mass management |

---

## 6. Future Considerations
- Investigate external PD trigger module for 9V/12V to Pi5 to add headroom
- Log power usage over time for data-driven pack selection
- Evaluate unified power solution with dual isolated DC-DC converters in a custom pack
