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

- Plantilla oficial de descriptor (TinyUSB): ![link](https://github.com/hathach/tinyusb/blob/fd5bb6e5db8e8e997d66775e689cc73f149e7fc1/src/class/hid/hid_device.h). Notar que las macros NO están escritas exactamente igual.

## Librerias > LPC Open > LPC Chip 43xx > USBD ROM > USBD HID
Acá están varias macros usadas para los descriptores. Resultan de interés:
- HID_USAGE_GENERIC_GAMEPAD = 0x05, pasar como segundo valor con HID Usage.
- HID_USAGE_GENERIC_X = 0x30
- HID_USAGE_GENERIC_Y = 0x31
- HID_USAGE_GENERIC_Z = 0x32 ...


## Librerias > sAPI > sAPI v0.6.2 > SoC > Periféricos > USB > Device

### Includes > USBD Keyboard
Acá están todas las macros de las teclas (mapeadas en hexa), se define tamaño máximo de reporte igual a 8 y estas funciones macro:
- HID_KEYBOARD_CLEAR_REPORT(x), que reemplaza: memset(x, 0, 8) = copiar el valor 0 a las primeras 8 posiciones del vector x
- HID_KEYBOARD_REPORT_SET_KEY_PRESS(x, val), que reemplaza: x[2] = (uint8_t) val

### Source > sAPI USB Device
Acá está implementada la función usbDeviceInit (alias usbDeviceConfig), solamente esta es pública. Para el caso Keyboard, se llama:
- usbDeviceLpcInit, solamente se le indica que, en el caso Keyboard, se usan 2 endpoints.
- usbDeviceKeyboardInit
- usbDeviceLpcInterruptInit(), esto lo realiza una vez ya inicializado (status OK).

### Source > USBD Keyboard
Acá están implementadas varias funciones públicas, destacando usbDeviceKeyboardInit y usbDeviceKeyboardTasks(). La primera crea una variable hid_param del tipo USB_HID_INIT_PARAM_T. Le asigna las siguientes funciones a los campos correspondientes de hid_param. Estas funciones están definidas en este mismo archivo, nomás son privadas.

- Keyboard_UpdateReport: llama a una función macro HID_KEYBOARD_CLEAR_REPORT e invoca la función de callback (la del ejemplo).
- Keyboard_GetReport: llama a la anterior para pasar el reporte por parámetro. Son los valores que se transmitirán a la PC.
- Keyboard_SetReport: si el reporte recibido tiene longitud > 0, realiza acciones de salida (prende LED según NUM_LOCK de PC).
- KeyboardEpIN_Hdlr: manejador de interrupción de entrada (desde PC), marca TX busy como 0 en la variable g_keyBoard

Este archivo pide que, externamente, se defina Keyboard_ReportDescSize y Keyboard_ReportDescriptor[], cosa que se hace en el ejemplo. También se posee una variable privada (global al archivo) llamada "g_keyBoard", que en realidad es un struct que guarda la información cargada en la inicialización, un reporte y un flag de TX ocupado. La referencia a la función de callback de teclas presionadas también se almacena en este archivo como keyboardCheckKeysFunction.

Justamente, la función usbDeviceKeyboardPress(tecla) está definida acá también como un llamado a HID_KEYBOARD_REPORT_SET_KEY_PRESS, pasando el campo reporte de g_keyBoard y la tecla propiamente dicha.
