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

[cite_start]**Mindwave-Helicopter** documents the integration of EEG biosensors with embedded hardware to achieve thought-based flight control[cite: 1]. [cite_start]From **NeuroSky** attention processing to **Atmega32U4** servo actuation, this repo tracks how neural focus is converted into mechanical lift[cite: 5, 20, 31].

---

<table width="100%">
  <tr>
    <td width="50%" align="left" valign="middle">
      <h2>🔌 The Mission: Biological Command</h2>
    </td>
    <td width="50%" align="center" valign="middle">
      <img src="https://github.com/user-attachments/assets/16fdaa31-6648-426e-9237-6d00fc5ef95e" alt="Circuitry Visualization" width="350" style="border-radius: 8px;" />
    </td>
  </tr>
  <tr>
    <td colspan="2">
      <p>
        In this system, human focus serves as the primary input. The <b>Mindwave Headset</b> performs the heavy lifting of onboard EEG filtration, while the <b>Arduino Micro</b> manages real-time flight stability and cockpit telemetry[cite: 5, 31, 34].
      </p>
      <p>
        Synchronizing biology and hardware requires high-speed protocols: <b>UART</b> captures the 115200 baud neuro-stream [cite: 5, 18], <b>I2C</b> drives the pilot's heads-up display [cite: 2, 21], and <b>PWM</b> maps mental intensity to the helicopter's throttle and dial servos[cite: 48, 49].
      </p>
    </td>
  </tr>
</table>

---

## 🏗️ Protocol Architecture

The project utilizes three primary communication layers:

### 1. 📟 [UART (Universal Asynchronous)](#)
- [cite_start]**Primary Use:** The data bridge between the **Mindwave Headset** and the **Micro 32U4 (Central Nervous System)**[cite: 1, 5].
- [cite_start]**Application:** Streaming 8-bit brainwave packets including Attention, Meditation, and EEG power bands[cite: 6, 31, 32, 34].
- [cite_start]**Hardware:** Utilizes hardware Serial1 at 115200 baud for wireless Bluetooth reception[cite: 5, 18].

### 2. 🖇️ [I2C (Inter-Integrated Circuit)](#)
- [cite_start]**Primary Use:** Visual telemetry for the pilot's "Ground Control" display[cite: 21, 23].
- [cite_start]**Application:** Connecting the **LiquidCrystal_I2C** display to show real-time Attention and Meditation scores[cite: 2, 47].
- [cite_start]**Advantage:** Allows the system to update flight status without interrupting the high-speed UART neuro-stream[cite: 10, 21].

### 3. ⚡ [PWM (Pulse Width Modulation)](#)
- [cite_start]**Primary Use:** Translating mental "Intensity" into mechanical "Force"[cite: 48, 49].
- [cite_start]**Application:** Driving the **DialServo** and **ThrottleServo** to control helicopter lift based on focus thresholds[cite: 19, 50].
- [cite_start]**Hardware:** Maps 0–100 neuro-values to specific servo positions (55–145 for dial, 5–150 for throttle) for precise actuation[cite: 24, 25, 26, 48, 49].

---

## 🛠️ The Hardware Stack

| Category      | Component           | Communication Role                 |
| :------------ | :------------------ | :--------------------------------- |
| **Brain** | NeuroSky Mindwave   | [cite_start]Master EEG Data Provider [cite: 1] |
| **Controller**| Arduino Micro 32U4  | [cite_start]Main Packet Parser & Servo Driver [cite: 5, 20] |
| **Actuators** | Servos (Dial/Throttle)| [cite_start]PWM Position Control [cite: 4, 19, 50] |
| **Display** | I2C 20x4 LCD        | [cite_start]Real-time Telemetry Monitor [cite: 2, 21] |
| **Wireless** | Bluetooth Module    | [cite_start]UART Gateway to NeuroSky [cite: 5, 18] |

---

## 📐 Implementation Pipeline

1. [cite_start]**Packet Decoding:** Implementing a checksum-validated parser to extract neuro-values from the 0xAA sync stream[cite: 28, 29].
2. [cite_start]**Frequency Analysis:** Isolating Delta, Theta, Alpha, Beta, and Gamma waves for multi-dimensional flight logic[cite: 13, 14, 15, 16].
3. [cite_start]**Signal Smoothing:** Using `constrain()` and `map()` functions to prevent servo jitter and ensure valid data ranges[cite: 44, 48, 49].
4. [cite_start]**Visual Feedback:** Mapping the attention threshold (>40) to onboard status LEDs for the pilot[cite: 42, 43].
5. [cite_start]**Integration:** Synchronizing the I2C display refresh with the incoming payload data from the headset[cite: 46, 47].

---

## 📺 Featured Hardware: Ball Helicopter Rig

**Key Highlights:**
- [cite_start]**Attention Mapping:** Mental focus is directly proportional to throttle lift (Higher Attention = More Power)[cite: 48, 49].
- [cite_start]**Meditation Tracking:** Secondary data stream used for analog visual monitoring on pins 5 and 6[cite: 32, 51].
- [cite_start]**Failsafe System:** Hardware LEDs indicate signal quality to ensure control is only active during valid EEG reception[cite: 18, 42].

---

<small>© 2026 MatsRobot | Project based on Arduino Bluetooth Interface with Mindwave</small>
