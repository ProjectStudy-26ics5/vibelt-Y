#include "../peripherals.hpp"
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <math.h>

namespace vb {

static HardwareSerial uart(1);
static TinyGPSPlus gps;
double gps_lon = NAN;
double gps_lat = NAN;
  
Result gps_setup() {
  uart.begin(9600, SERIAL_8N1, GPS_UART_RX, GPS_UART_TX);

  return Result::Ok();
}

Result gps_runtime() {
  while(uart.available()) {
    gps.encode(uart.read());
  }

  if(gps.location.isUpdated()) {
    gps_lon = gps.location.lng();
    gps_lat = gps.location.lat();
  }

  // Serial.printf("satellites = %d\n", gps.satellites.value());

  return Result::Ok();
}

};
