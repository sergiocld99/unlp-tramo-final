#ifndef _GAMEPAD_H_
   #define _GAMEPAD_H_

   /// HID Gamepad Protocol Report.
   typedef struct TU_ATTR_PACKED
   {
     int8_t  x;         ///< Delta x  movement of left analog-stick
     int8_t  y;         ///< Delta y  movement of left analog-stick
     int8_t  z;         ///< Delta z  movement of right analog-joystick
     int8_t  rz;        ///< Delta Rz movement of right analog-joystick
     int8_t  rx;        ///< Delta Rx movement of analog left trigger
     int8_t  ry;        ///< Delta Ry movement of analog right trigger
     uint8_t hat;       ///< Buttons mask for currently pressed buttons in the DPad/hat
     uint32_t buttons;  ///< Buttons mask for currently pressed buttons
   } hid_gamepad_report_t;

   /*==================[inclusiones]============================================*/
   

/*==================[end of file]============================================*/
#endif /* _APP_H_ */


