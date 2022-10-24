# sAPI HID Keyboard

## Ejemplos > C > sAPI > USB > Device > HID Keyboard > Source 

### App HID Keyboard
Se invoca a las funciones:
- usbDeviceConfig, pasando como parámetro la macro USB_HID_KEYBOARD (simplemente indica qué clase de HID es)
- usbDeviceKeyboardCheckKeysCallbackSet, pasando como parámetro la función que a su vez invoca usbDeviceKeyboardPress(), pasando la macro de una tecla definida.
- usbDeviceKeyboardTasks(), en el super-loop

Nos interesa estudiar la primera, para ver cómo se realiza la configuración de un tipo de HID particular.

### USBD Keyboard HID Desc
Se define Keyboard_ReportDescSize y Keyboard_ReportDescriptor[], siendo el primero un sizeof del segundo.


## Librerias > sAPI > sAPI v0.6.2 > SoC > Periféricos > USB > Device > Source

### sAPI USB Device
Acá está implementada la función usbDeviceInit (alias usbDeviceConfig), solamente esta es pública. Para el caso Keyboard, se llama:
- usbDeviceLpcInit, solamente se le indica que, en el caso Keyboard, se usan 2 endpoints.
- usbDeviceKeyboardInit
- usbDeviceLpcInterruptInit(), esto lo realiza una vez ya inicializado (status OK).

### USBD Keyboard
Acá están implementadas varias funciones públicas, destacando usbDeviceKeyboardInit y usbDeviceKeyboardTasks(). La primera crea una variable hid_param del tipo USB_HID_INIT_PARAM_T. Le asigna las siguientes funciones a los campos correspondientes de hid_param. Estas funciones están definidas en este mismo archivo, nomás son privadas.

- Keyboard_UpdateReport
- Keyboard_GetReport
- Keyboard_SetReport
- KeyboardEpIN_Hdlr

Este archivo pide que, externamente, se defina Keyboard_ReportDescSize y Keyboard_ReportDescriptor[], cosa que se hace en el ejemplo.
