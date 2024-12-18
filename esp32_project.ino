#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin Definitions
#define LED1_PIN 4
#define LED2_PIN 16
#define LED3_PIN 25
#define BUTTON_MODE_PIN 26
#define BUTTON_RESET_PIN 27
#define MIC_PIN 19      // Microphone sensor connected to GPIO 19

// Variables for LED Effects
int ledMode = 0; // Current LED mode
int timerMode = 0; // Timer mode (0 = off, 1 = 1 min, 2 = 2 min, 3 = 3 min)
unsigned long timerDuration[] = {0, 60000, 120000, 180000};
unsigned long timerStartTime = 0;

// Button Debounce
Bounce buttonMode;
Bounce buttonReset;

// Semaphore for synchronizing tasks
SemaphoreHandle_t xSemaphore;

// Task Handles
TaskHandle_t taskLedHandle;
TaskHandle_t taskOledHandle;

// LED PWM Variables
int pwmValue1 = 0, pwmValue2 = 0, pwmValue3 = 0;
int pwmIncrement1 = 5, pwmIncrement2 = 3, pwmIncrement3 = 7;

// Function to reset to default mode
void resetToDefault() {
  ledMode = 0;
  timerMode = 0;
  timerStartTime = 0;
}

// LED Effect Tasks
void taskLed(void *parameter) {
  while (true) {
    switch (ledMode) {
      case 0: // Basic mode (default)
        digitalWrite(LED1_PIN, HIGH);
        digitalWrite(LED2_PIN, LOW);
        digitalWrite(LED3_PIN, LOW);
        break;
      case 1: // Breathing effect
        pwmValue1 += pwmIncrement1;
        if (pwmValue1 <= 0 || pwmValue1 >= 255) pwmIncrement1 = -pwmIncrement1;
        analogWrite(LED1_PIN, pwmValue1);
        break;
      case 2: // Flashing effect
        pwmValue2 = !pwmValue2 ? 255 : 0;
        analogWrite(LED2_PIN, pwmValue2);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        break;
      case 3: // Chasing effect
        pwmValue3 += pwmIncrement3;
        if (pwmValue3 <= 0 || pwmValue3 >= 255) pwmIncrement3 = -pwmIncrement3;
        analogWrite(LED3_PIN, pwmValue3);
        break;
      case 4: // Microphone-triggered flashing effect (using the 3 existing LEDs)
        int micValue = analogRead(MIC_PIN); // Read microphone input
        int pwmMicValue = map(micValue, 0, 1023, 0, 255); // Map the mic value to PWM range

        // Distribute PWM effect across all 3 LEDs based on mic input
        analogWrite(LED1_PIN, pwmMicValue);  // LED1 reacts to microphone input
        analogWrite(LED2_PIN, pwmMicValue);  // LED2 reacts to microphone input
        analogWrite(LED3_PIN, pwmMicValue);  // LED3 reacts to microphone input

        vTaskDelay(10 / portTICK_PERIOD_MS); // Small delay to stabilize
        break;
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// OLED Display Task
void taskOled(void *parameter) {
  while (true) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY)) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      // Hiển thị chế độ LED
      display.println("LED Mode:");
      display.println(ledMode);

      // Hiển thị chế độ hẹn giờ
      display.println("Timer Mode:");
      if (timerMode == 0) {
        display.println("OFF");
      } else {
        // Tính toán thời gian đếm ngược còn lại
        unsigned long elapsedTime = millis() - timerStartTime;
        unsigned long remainingTime = (timerDuration[timerMode] > elapsedTime) 
                                      ? (timerDuration[timerMode] - elapsedTime) 
                                      : 0;

        // Hiển thị thời gian còn lại
        unsigned long secondsRemaining = remainingTime / 1000; // Đổi ra giây
        display.print("Remaining: ");
        display.print(secondsRemaining);
        display.println(" sec");

        // Nếu hết thời gian, thoát chế độ hẹn giờ
        if (remainingTime == 0) {
          resetToDefault();
        }
      }

      // Cập nhật thông tin trên OLED
      display.display();
      xSemaphoreGive(xSemaphore);

      // Tần số làm mới hiển thị
      vTaskDelay(500 / portTICK_PERIOD_MS);
    }
  }
}

// Button Handling
void handleButtons() {
  buttonMode.update();
  buttonReset.update();

  // Reset button: nhấn nút Reset để về chế độ mặc định
  if (buttonReset.fell()) {
    resetToDefault();
  }

  // Mode button
  if (buttonMode.fell()) {
    timerStartTime = millis(); // Ghi nhận thời gian bắt đầu nhấn
  } 

  if (buttonMode.rose()) { // Khi thả nút ra
    unsigned long pressDuration = millis() - timerStartTime;

    if (pressDuration < 1000) { 
      // Nhấn ngắn (< 1 giây): Đổi chế độ LED
      ledMode = (ledMode + 1) % 5; // Chuyển qua các chế độ LED (0, 1, 2, 3, 4)
    } else { 
      // Nhấn giữ (> 1 giây): Đổi chế độ hẹn giờ
      timerMode = (timerMode + 1) % 4; // Chuyển qua các chế độ hẹn giờ (0, 1, 2, 3)
      if (timerMode > 0) {
        timerStartTime = millis(); // Cập nhật thời gian bắt đầu hẹn giờ
      }
    }
  }
}

// Timer Handling
void handleTimer() {
  if (timerMode > 0 && millis() - timerStartTime > timerDuration[timerMode]) {
    resetToDefault();
  }
}

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Initialize OLED
 // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  
  display.clearDisplay();
  display.display();

  // Initialize pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUTTON_MODE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_RESET_PIN, INPUT_PULLUP);
  pinMode(MIC_PIN, INPUT);

  // Attach buttons
  buttonMode.attach(BUTTON_MODE_PIN);
  buttonMode.interval(25);
  buttonReset.attach(BUTTON_RESET_PIN);
  buttonReset.interval(25);

  // Create semaphore
  xSemaphore = xSemaphoreCreateMutex();

  // Create tasks
  xTaskCreate(taskLed, "LED Task", 1000, NULL, 1, &taskLedHandle);
  xTaskCreate(taskOled, "OLED Task", 2000, NULL, 1, &taskOledHandle);

  // Reset to default state
  resetToDefault();
}

void loop() {
  handleButtons();
  handleTimer();
  delay(10); // Allow FreeRTOS to run other tasks
}
