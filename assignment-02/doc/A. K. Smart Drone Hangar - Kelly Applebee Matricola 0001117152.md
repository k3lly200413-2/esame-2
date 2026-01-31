
# 1. Introduction
* ## 1.1 Objective 
	This project simulates an automated **Drone Hangar** system using distributed physical components. The system manages the drone's lifecycle (Storage, Takeoff, Flight, Landing) while actively monitoring environmental conditions to ensure safety.

* ## 1.2 FSM Approach
	An **FSM (Finite State Machine)** approach was chosen to ensure scalability, ease of debugging, and code readability. This architecture guarantees non-blocking operations and provides deterministic output control based on sensor inputs, which is critical for real-time embedded systems.

* ## 1.3 System Architecture
	**Components Used:**
		1x Arduino UNO
		1x SG90 9G micro servo motor
		1x Pir Motion Sensor
		1x HC - SR04 Ultrasonic Sensor
		1x DHT11 Temperature and humidity sensor
		1x Lcd Display 16x2
		1x I2C converter 
		2x Green LED
		1x Red LED
		1x Push Button
		3x 220 Ω Resistor
		1x 1k Ω Resistor
		
	![[esame_3_bb.png]]
 
* ## 1.4 FMS Design

	* #### 1.4.1 State Definition
		Idle
		Drone Inside
		Take Off
		Flying
		Landing
		Pre Alarm
		Full Alarm

	* #### 1.4.2 I/O Definition
		**Inputs:**
		* Pir Motion Sensor
		* HC - SR04 Ultrasonic Sensor
		* DHT11 Temperature and humidity sensor
		* Push Button

		**Outputs:**
		* LEDs
		* Lcd Display 16x2
		* SC90 9G micro servo motor
	
	![[Untitled Diagram.drawio 1.png]]
* ## 1.5 Implementation Details

	The system is implemented using the **C++ State Design Pattern**. Unlike a traditional monolithic `switch-case` structure, this approach encapsulates each state (e.g., `TakeOff`, `Flying`) into its own class inheriting from a common `GenericState`.

**Key Architectural Features:**

- **The State Manager (Context):** Holds a pointer to the current state object and handles transition logic.
    
- **Concrete States:** Separate `.cpp` files define the specific behavior (Mealy/Moore outputs) for each phase.
    
- **Context-Aware Safety:** The system includes a global safety guard implemented in the parent class. Transitions to `ALARM` states are triggered by high temperatures, but are conditionally gated by the `canEmergencyStop` flag. This allows specific critical states (like unstable flight) to override thermal cutoffs to prevent crashes.
    
- **History Mechanism:** Upon recovering from a thermal alarm, the system does not reset to `IDLE`; it resumes the exact state the drone was in prior to the fault.