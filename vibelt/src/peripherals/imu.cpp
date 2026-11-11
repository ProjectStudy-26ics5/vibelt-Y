#include "../peripherals.hpp"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <math.h>

namespace vb {

static Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
static sensors_event_t event;

Result imu_setup() {
  Wire.begin(IMU_I2C_SDA, IMU_I2C_SCL);

  if(!bno.begin()) {
    return Result::Err("bno initialization failed.");
  }
  bno.setExtCrystalUse(true);

  return Result::Ok();
}

Result imu_runtime() {
  bno.getEvent(&event);

  return Result::Ok();
}

float imu_compass() {
  // センサの取り付け向きによって多少変更する可能性があります
  // 0以上360未満が期待されます
  return event.orientation.x;
}

};
