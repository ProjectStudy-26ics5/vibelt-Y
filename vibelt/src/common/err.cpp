#include "../common.hpp"
#include "../peripherals.hpp"

namespace vb {

Result::Result(ResultKind kind__, const char *msg__): kind_(kind__) {
  if(msg__ == nullptr) {
    msg_ = "";
  }else {
    msg_ = msg__;
  }
}

Result Result::Ok(const char *msg__) {
  return Result(ResultKind::Ok, msg__);
}

Result Result::Err(const char *msg__) {
  return Result(ResultKind::Err, msg__);
}

bool Result::ok() const {
  return kind_ == ResultKind::Ok;
}

const char *Result::what() const {
  return msg_;
}



void panic(const char *msg) {
  Serial.printf("panic: %s\n", msg);
  
  for(;;) {
    neopixelWrite(LED_GPIO, 50, 0, 0);
    delay(500);
    neopixelWrite(LED_GPIO, 0, 0, 0);
    delay(500);
  }
}

void check(Result result) {
  if(!result.ok()) {
    panic(result.what());
  }
}

};
