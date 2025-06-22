# 🎓 Graduation Project – Power Source Management System

## 📘 Project Overview

This Arduino-based project is a smart power source management system designed for critical applications where continuous power is essential.  
It automatically selects the most reliable power source among a main supply, two backup batteries, and an emergency fallback, based on real-time voltage monitoring.  
The system ensures seamless power switching using relays, preventing downtime or manual intervention.

...
🔌 Hardware Components
1: Arduino Uno
The main microcontroller responsible for reading sensor data and controlling relays.

2: Relays (5V)
Used to switch between different power sources based on voltage conditions.

3: Solar Panel (12V or higher)
Acts as a renewable energy power input, connected as one of the monitored sources.

4: Power Supply (220V AC to 12V DC Adapter)
The main source of power, converted from household electricity.

5: Rechargeable Batteries (12V)
Two batteries serve as backup sources in case the main power is unavailable.

6: Current Sensor (ACS712 or similar)
Measures the current flowing through the active power source to monitor load behavior.

7: Voltage Sensors
Voltage dividers or dedicated sensor modules used to measure the voltages of:

8: Battery (2)

9: Solar Charge Controller
(Optional but recommended) Regulates charging of batteries from the solar panel safely.

10: Wires and Connectors
For connecting all components securely on a breadboard or PCB.

11:Breadboard or Custom PCB
Used for prototyping the circuit before final implementation.

