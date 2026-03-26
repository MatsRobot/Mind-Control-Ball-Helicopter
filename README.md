# 🧠 Mindwave-Helicopter
## 🤖 Neuro-Robotics: The Brain-Computer Interface (BCI)

<p align="left">
  <a href="#-uart-universal-asynchronous">
    <img src="https://img.shields.io/badge/UART-Bluetooth_Serial-0366d6?style=for-the-badge&logo=bluetooth&logoColor=white" />
  </a>
  <a href="#-i2c-inter-integrated-circuit">
    <img src="https://img.shields.io/badge/I2C-LCD_Display-24292e?style=for-the-badge&logo=target&logoColor=white" />
  </a>
  <a href="#-pwm-pulse-width-modulation">
    <img src="https://img.shields.io/badge/PWM-Servo_Control-0366d6?style=for-the-badge&logo=arduino&logoColor=white" />
  </a>
</p>

**Mindwave-Helicopter** documents the integration of EEG biosensors with embedded hardware to achieve thought-based flight control. From **NeuroSky** attention processing to **Atmega32U4** servo actuation, this repo tracks how neural focus is converted into mechanical lift.


---

<table width="100%">
  <tr>
    <td width="50%" align="left" valign="middle">
      <h2>🔌 The Mission: Biological Command</h2>
    </td>
    <td width="50%" align="center" valign="middle">
      <img src="https://github.com/user-attachments/assets/21e9879f-6367-41f5-9e01-42553881edd1" alt="Brain and Ball Helicopter Interface Visualization" width="350" style="border-radius: 8px;" />
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <p>
        In this system, human focus serves as the primary input. The <b>Mindwave Headset</b> performs the heavy lifting of onboard EEG filtration, while the <b>Arduino Micro</b> manages real-time flight stability and cockpit telemetry.
      </p>
      <p>
        Synchronizing biology and hardware requires high-speed protocols: <b>UART</b> captures the 115200 baud neuro-stream, <b>I2C</b> drives the pilot's heads-up display, and <b>PWM</b> maps mental intensity to the helicopter's throttle and dial servos.
      </p>
    </td>
  </tr>
</table>

---

## 🏗️ Protocol Architecture

The project utilizes three primary communication layers:

### 1. 📟 [UART (Universal Asynchronous)](#)
- **Primary Use:** The data bridge between the **Mindwave Headset** and the **Micro 32U4 (Central Nervous System)**.
- **Application:** Streaming 8-bit brainwave packets including Attention, Meditation, and EEG power bands.
- **Hardware:** Utilizes hardware Serial1 at 115200 baud for wireless Bluetooth reception.

### 2. 🖇️ [I2C (Inter-Integrated Circuit)](#)
- **Primary Use:** Visual telemetry for the pilot's "Ground Control" display.
- **Application:** Connecting the **LiquidCrystal_I2C** display to show real-time Attention and Meditation scores.
- **Advantage:** Allows the system to update flight status without interrupting the high-speed UART neuro-stream.

### 3. ⚡ [PWM (Pulse Width Modulation)](#)
- **Primary Use:** Translating mental "Intensity" into mechanical "Force".
- **Application:** Driving the **DialServo** and **ThrottleServo** to control helicopter lift based on focus thresholds.
- **Hardware:** Maps 0–100 neuro-values to specific servo positions (55–145 for dial, 5–150 for throttle) for precise actuation.

---

## 🛠️ The Hardware Stack

| Category      | Component           | Communication Role                 |
| :------------ | :------------------ | :--------------------------------- |
| **Brain** | NeuroSky Mindwave   | Master EEG Data Provider |
| **Controller**| Arduino Micro 32U4  | Main Packet Parser & Servo Driver |
| **Actuators** | Servos (Dial/Throttle)| PWM Position Control |
| **Display** | I2C 20x4 LCD        | Real-time Telemetry Monitor |
| **Wireless** | Bluetooth Module    | UART Gateway to NeuroSky |

---

## 📐 Implementation Pipeline

1. **Packet Decoding:** Implementing a checksum-validated parser to extract neuro-values from the 0xAA sync stream.
2. **Frequency Analysis:** Isolating Delta, Theta, Alpha, Beta, and Gamma waves for multi-dimensional flight logic.
3. **Signal Smoothing:** Using `constrain()` and `map()` functions to prevent servo jitter and ensure valid data ranges.
4. **Visual Feedback:** Mapping the attention threshold (>40) to onboard status LEDs for the pilot.
5. **Integration:** Synchronizing the I2C display refresh with the incoming payload data from the headset.

---

## 📺 Featured Hardware: Ball Helicopter Rig

**Key Highlights:**
- **Attention Mapping:** Mental focus is directly proportional to throttle lift (Higher Attention = More Power).
- **Meditation Tracking:** Secondary data stream used for analog visual monitoring on pins 5 and 6.
- **Failsafe System:** Hardware LEDs indicate signal quality to ensure control is only active during valid EEG reception.


---

<small>© 2026 MatsRobot | Licensed under the [MIT License](https://github.com/MatsRobot/matsrobot.github.io/blob/main/LICENSE)</small>
