# ESP32 LED Sound Reactive System with OLED Display

## Description:
This project is a sound-reactive LED control system that uses a microphone to detect audio levels and adjusts the brightness of three LEDs accordingly. The system offers multiple LED modes, including basic, breathing, flashing, and chasing effects, all displayed on an OLED screen. Users can cycle through LED modes and timer settings via two push buttons, which are used for mode switching and resetting the system to its default state.

The project utilizes an **ESP32** microcontroller and an **SSD1306 OLED display** to provide a user-friendly interface for displaying the current mode and countdown timer (if active). The system also includes sound sensitivity, which uses a microphone sensor connected to GPIO 19 to trigger LED changes based on ambient sound.

## Key Features:

- **Four LED modes**: default, breathing, flashing, chasing, and sound-reactive mode.
- **OLED display**: showing the current LED mode and timer status.
- **Buttons**: Two push buttons for mode selection and reset functionality.
- **Timer**: Functionality that can be set for 1, 2, or 3 minutes.
- **Sound-responsive**: LED behavior based on microphone input, adjusting the brightness of all three LEDs.
- **PWM**: Uses PWM to control LED brightness for smooth transitions.

## How to Use the Project:

### 1. Clone or Download the Project:
   - Clone the repository using Git:
     ```bash
     git clone https://github.com/yourusername/LED-Sound-Reactive-System.git
     ```
   - Or, download the ZIP file from the GitHub page and extract it to your preferred location.

### 2. Install the Required Libraries:
   This project uses several libraries, which must be installed before compiling:
   
   - **Adafruit SSD1306** library for controlling the OLED display.
   - **Adafruit GFX** library for graphics rendering.
   - **Bounce2** library for debouncing the push buttons.
   
   To install the libraries:
   - Open Arduino IDE and navigate to **Sketch > Include Library > Manage Libraries**.
   - Search for the libraries below and install them:
     - **Adafruit SSD1306**
     - **Adafruit GFX**
     - **Bounce2**

### 3. Set Up the ESP32 Board:
   - Open the **Arduino IDE**.
   - Go to **File > Preferences**, and in the **Additional Boards Manager URLs** field, add the following URL:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to **Tools > Board** and select **ESP32 Dev Module** (or your specific ESP32 model).
   - Select the correct port under **Tools > Port**.

### 4. Upload the Code:
   - Open the project in Arduino IDE by selecting **File > Open** and navigate to the `.ino` file in the project folder.
   - Click the **Upload** button (right arrow icon) to compile and upload the code to your ESP32.

### 5. Wiring and Hardware Setup:
   - **LEDs**: Connect the three LEDs to **GPIO 4**, **GPIO 16**, and **GPIO 25**.
   - **Buttons**: Connect the mode selection button to **GPIO 26** and the reset button to **GPIO 27**.
   - **Microphone**: Connect the microphone sensor to **GPIO 19**.
   - **OLED Display**: Use the **I2C** interface (SDA and SCL) for the **SSD1306 OLED display**.

### 6. Operation:
   - After uploading the code to the ESP32, the OLED will display the current LED mode and timer status.
   - Press the **mode button** to cycle through different LED modes. 
   - If the **mode button** is held for more than 1 second, it will switch to the timer mode and allow you to select the timer duration (1, 2, or 3 minutes).
   - Press the **reset button** to return the system to the default mode.

### 7. Sound Reactive Mode:
   - When the system is in sound-reactive mode, the **microphone sensor** will detect ambient sound and adjust the brightness of all three LEDs accordingly.
   - The LED brightness will respond to sound levels by adjusting the PWM values, making the LEDs blink or change brightness based on the sound input.

## Troubleshooting:
- **Libraries not found**: Ensure you have installed the required libraries in Arduino IDE.
- **OLED display not showing up**: Verify the I2C connections for the OLED display and ensure the correct I2C address (`0x3C`) is used in the code.
- **Microphone not responding**: Check the connection to GPIO 19 and ensure the microphone sensor is functioning properly.
- **Incorrect LED behavior**: Make sure the LEDs are connected to the correct GPIO pins as defined in the code.

## License:
This project is open source and available under the MIT License.
