# Smart Zap Watch

A DIY wearable that uses gentle TENS-style nudges to help with waking up, focus, and alertness.



# Overview

Smart Zap Watch is a DIY wearable I will be building using the Seeed XIAO ESP32-S3.
The goal is to create a compact wrist device that generates safe micro-stimulation pulses, displays basic info, and reacts to motion — all while being small enough to wear comfortably.

This project combines embedded systems, IoT, custom electronics, and experimental bio-interaction technology.


# Features (Planned)
	•	⚡ Gentle stimulation pulses using a custom flyback HV circuit
	•	⏰ Wake-up nudge system
	•	🖥️ 0.96” OLED display for time + modes
	•	🎛️ IMU/Gyroscope for motion tracking
	•	📳 Vibration motor for soft alerts
	•	🔋 Runs on a compact Li-ion battery
	•	🌐 ESP32-S3 allows WiFi/BLE expansion


# Hardware Components
	•	Seeed Studio XIAO ESP32-S3
	•	0.96” OLED Display (I2C)
	•	IMU / Gyroscope sensor
	•	Vibration motor
	•	Rechargeable Li-ion battery
	•	Custom flyback high-voltage generator
	•	Coupled inductor
	•	Dual MOSFET switching
	•	Basic wiring + planned 3D-printed enclosure


# How It Will Work
	1.	The ESP32-S3 controls a dual-MOSFET flyback converter, boosting 3.7V to small HV pulses.
	2.	The device outputs low-intensity, safe pulses similar to TENS for mild stimulation.
	3.	The OLED displays time, movement data, and modes.
	4.	The IMU detects wrist movement and can trigger smart alerts.
	5.	The vibration motor works as a secondary alert option.

The idea is to create a wearable nudge assistant for alertness and daily productivity.


# Why I’m Building This
	•	To learn more about embedded systems + wearables
	•	To understand bio-stimulation and nerve response
	•	To design miniaturized electronics
	•	To practice IoT and low-power design
	•	Because building weird, useful hardware is fun


# Learning Goals

  I expect to learn:
	•	Safe flyback HV design
	•	Power management in compact devices
	•	ESP32-S3 deep sleep + IO control
	•	Display + IMU integration
	•	Enclosure design and assembly
	•	Better hardware debugging



# Future Improvements
	•	Smartphone app control (BLE)
	•	Advanced nudge patterns
	•	Sleep tracking
	•	Safety-enhanced HV driver
	•	Refined 3D-printed enclosure
	•	Modular electrode system
