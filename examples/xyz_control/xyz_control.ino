#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "M5Stack.h"
#include "MODULE_DIRECT_STEPMOTOR.h"

static DIRECT_STEPMOTOR driver;

void stepUpdateTask(void *arg) {
    for (;;) {
        digitalWrite(16,1);
        digitalWrite(12,1);
        digitalWrite(15,1);
        vTaskDelay(1);
        digitalWrite(16,0);
        digitalWrite(12,0);
        digitalWrite(15,0);
        vTaskDelay(1);
    }
}

void setup() {
    M5.begin(true, false, true, false);
    M5.Lcd.clear(TFT_BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString("DIRECT_STEPMOTOR", 160, 120, 4);

    Wire.begin(21, 22, 400000);
    driver.init(Wire);
    driver.enableMotor(1);
    driver.setMicrostepResolution(DIRECT_STEPMOTOR::kMicrosteps16);

    Serial2.begin(115200, SERIAL_8N1, 34, 26);
    Serial2.setTimeout(100);

    pinMode(16,OUTPUT);
    pinMode(12,OUTPUT);
    pinMode(15,OUTPUT);

    pinMode(17,OUTPUT);
    pinMode(13,OUTPUT);
    pinMode(0,OUTPUT);

    digitalWrite(17, 1);
    digitalWrite(13, 1);
    digitalWrite(0, 1);
    xTaskCreatePinnedToCore(stepUpdateTask, "stepUpdateTask", 4 * 1024, NULL, 1, NULL, 0);
}

void loop() {
    static uint8_t step_dir = 1;
    static DIRECT_STEPMOTOR::MicrostepResolution_t micro_res = DIRECT_STEPMOTOR::kMicrosteps16;
    M5.update();
    if (M5.BtnA.wasPressed()) {
        step_dir = 1 - step_dir;
        digitalWrite(17, step_dir);
        digitalWrite(13, step_dir);
        digitalWrite(0, step_dir);
    }

    if (M5.BtnC.wasPressed()) {
        if (micro_res == DIRECT_STEPMOTOR::kMicrosteps16) {
            micro_res = DIRECT_STEPMOTOR::kMicrosteps8;
        } else {
            micro_res = DIRECT_STEPMOTOR::kMicrosteps16;
        }
        driver.setMicrostepResolution(micro_res);
    }

    driver.getExtIOStatus();
    if (driver.ext_io_status[3]) {
        M5.Lcd.fillRect(70, 10, 20, 20, TFT_RED);
    } else {
        M5.Lcd.fillRect(70, 10, 20, 20, TFT_GREEN);
    }

    if (driver.ext_io_status[2]) {
        M5.Lcd.fillRect(120, 10, 20, 20, TFT_RED);
    } else {
        M5.Lcd.fillRect(120, 10, 20, 20, TFT_GREEN);
    }

    if (driver.ext_io_status[1]) {
        M5.Lcd.fillRect(170, 10, 20, 20, TFT_RED);
    } else {
        M5.Lcd.fillRect(170, 10, 20, 20, TFT_GREEN);
    }

    if (driver.ext_io_status[0]) {
        M5.Lcd.fillRect(220, 10, 20, 20, TFT_RED);
    } else {
        M5.Lcd.fillRect(220, 10, 20, 20, TFT_GREEN);
    }
}

