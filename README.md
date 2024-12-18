# ESP32 Led Sound Reactive System with OLED Display

##Description:
This project is a sound-reactive LED control system that uses a microphone to detect audio levels and adjusts the brightness of three LEDs accordingly. The system offers multiple LED modes, including basic, breathing, flashing, and chasing effects, all displayed on an OLED screen. Users can cycle through LED modes and timer settings via two push buttons, which are used for mode switching and resetting the system to its default state.

The project utilizes an ESP32 microcontroller and an SSD1306 OLED display to provide a user-friendly interface for displaying the current mode and countdown timer (if active). The system also includes sound sensitivity, which uses a microphone sensor connected to GPIO 19 to trigger LED changes based on ambient sound.

##Key Features:

- **Four LED modes**: default, breathing, flashing, chasing, and sound-reactive mode.
- **OLED display**: showing the current LED mode and timer status.
- **BUttons**: Two push buttons for mode selection and reset functionality.
- **Timer**: functionality that can be set for 1, 2, or 3 minutes.
- **Sound-responsive**: LED behavior based on microphone input, adjusting the brightness of all three LEDs.
- **PWM**: Uses PWM to control LED brightness for smooth transitions.
