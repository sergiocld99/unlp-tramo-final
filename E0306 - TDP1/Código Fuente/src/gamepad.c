#include "gamepad.h"
#include "sapi.h"

// REPORTES

void sendReport(uint8_t valorEjeX, uint8_t valorEjeY){
   hid_gamepad_report_t report =
      {
        .x   = 0, .y = 0, .z = 0, .rz = 0, .rx = 0, .ry = 0,
        .hat = 0, .buttons = 0
      };
}