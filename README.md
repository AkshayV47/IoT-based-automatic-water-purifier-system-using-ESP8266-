# IoT-based-automatic-water-purifier-system-using-ESP8266-

💧 Automatic Water Purifier Recirculation System using ESP8266
📌 Project Overview

The Automatic Water Purifier Recirculation System is an IoT-enabled solution designed to improve water purification efficiency. The system continuously monitors water quality using a turbidity sensor and automatically recirculates water back to the purifier if it is not sufficiently purified.

This system ensures that only clean and safe water is delivered, while allowing real-time monitoring through the Blynk IoT platform. It can be integrated inside existing water purifiers without major modification.

🎯 Key Features

💧 Real-time water quality monitoring using Turbidity Sensor

🔁 Automatic water recirculation until purification level is achieved

🚰 Smart control of Water Pump

🌐 WiFi-enabled system using ESP8266

📱 Live monitoring on Blynk IoT Platform

⚡ Low-power and compact design

🔧 Can be installed inside any water purifier

🛠️ Components Used

ESP8266 (NodeMCU)

Turbidity Sensor

Water Pump

Relay Module

Power Supply

Connecting Wires & Tubing

Blynk IoT Platform

⚙️ Working Principle

Water passes through the purifier and reaches the turbidity sensor.

The sensor measures the clarity of water in real time.

The ESP8266 compares the sensor value with a predefined purity threshold.

If water is not purified:

The pump is activated

Water is sent back to the purifier for reprocessing

This loop continues again and again until the water reaches acceptable purity.

Once purified, the pump stops and clean water is released.

All sensor data and system status are displayed on the Blynk dashboard.

🔁 System Flow
Water Output → Turbidity Sensor → ESP8266 → Quality Check
                                      ↓
                              Water Not Pure?
                                  ↓ Yes
                              Pump ON → Re-purification
                                  ↓ No
                               Clean Water Output

🌍 Applications

Domestic water purifiers

RO & UV purification systems

Smart homes

Industrial water filtration

IoT-based water quality monitoring

🚀 Advantages

Ensures consistent water purity

Reduces manual checking

Improves purifier efficiency

Real-time remote monitoring

Easy integration with existing systems

📌 Future Enhancements

Mobile alerts for poor water quality

Automatic valve control

Cloud data logging & analytics

Multi-parameter sensing (pH, TDS)

AI-based purity optimization

📂 Repository Contents

ESP8266 source code

Circuit diagram

Blynk dashboard setup

System flow diagram

Project documentation
