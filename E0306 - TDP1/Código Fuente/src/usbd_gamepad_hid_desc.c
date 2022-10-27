/**
   
   Código fuente basado en usbd_keyboard_hid_desc del ejemplo sAPI
   Adaptación realizada mediante la especificación de TinyUSB para Gamepad
   
   Autor: Calderón Sergio
   Fecha: 24 de octubre de 2022

**/

// Includes
#include "lpc_app_usbd_cfg.h"
#include "usbd_keyboard_endpoints.h"

// Macros no definidas en sAPI (importado de TinyUSB)
#define HID_PHYSICAL_MAX_N(x, n)                         HID_REPORT_ITEM(x, 4, 1, n)
#define HID_REPORT_ITEM(data, tag, type, size)           (((tag) << 4) | ((type) << 2) | (size)) HID_REPORT_DATA_##size(data)
#define HID_REPORT_DATA_2(data)                          , U16_TO_U8S_LE(data)
#define U16_TO_U8S_LE(_u16)                              TU_U16_LOW(_u16), TU_U16_HIGH(_u16)
#define TU_U16_HIGH(_u16)                                ((uint8_t) (((_u16) >> 8) & 0x00ff))
#define TU_U16_LOW(_u16)                                 ((uint8_t) ((_u16) & 0x00ff))

// Especificación del descriptor de reportes (arreglo)
const uint8_t Gamepad_ReportDescriptor[] = {
   HID_UsagePage(HID_USAGE_PAGE_GENERIC),
   HID_Usage(HID_USAGE_GENERIC_GAMEPAD),
   HID_Collection(HID_Application),
   HID_UsagePage( HID_USAGE_PAGE_GENERIC ),
   HID_Usage( HID_USAGE_GENERIC_X ),
   HID_Usage( HID_USAGE_GENERIC_Y ) ,
   HID_Usage( HID_USAGE_GENERIC_Z ) ,
   HID_Usage( HID_USAGE_GENERIC_RZ ) ,
   HID_Usage( HID_USAGE_GENERIC_RX ) ,
   HID_Usage( HID_USAGE_GENERIC_RY ) ,
   HID_LogicalMin( 0x81 ),
   HID_LogicalMax( 0x7f ),
   HID_ReportCount(6),
   HID_ReportSize(8),
   HID_Input(HID_Data | HID_Variable | HID_Absolute),
   // Mapeo de 8-bit DPad / Hat
   HID_UsagePage( HID_USAGE_PAGE_GENERIC ),
   HID_Usage( HID_USAGE_GENERIC_HATSWITCH ),
   HID_LogicalMin( 1 ),
   HID_LogicalMax( 8 ),
   HID_PhysicalMin(0),
   HID_PHYSICAL_MAX_N(315, 2),
   HID_ReportCount(1),
   HID_ReportSize(8),
   HID_Input(HID_Data | HID_Variable | HID_Absolute),
   // Mapeo de 32-bit button
   HID_UsagePage( HID_USAGE_PAGE_BUTTON ),
   HID_UsageMin(1),
   HID_UsageMax(32),
   HID_LogicalMin(0),
   HID_LogicalMax(1),
   HID_ReportCount(32),
   HID_ReportSize(1),
   HID_Input(HID_Data | HID_Variable | HID_Absolute),
   HID_EndCollection,
};

// Tamaño del descriptor de reportes
const uint16_t Gamepad_ReportDescSize = sizeof(Gamepad_ReportDescriptor);

// Conversiones auxiliares
/**
   HID_PHYSICAL_MAX_N ( 315, 2 )
      = HID_REPORT_ITEM(315, RI_GLOBAL_PHYSICAL_MAX, RI_TYPE_GLOBAL, 2)
      = ( (RI_GLOBAL_PHYSICAL_MAX << 4) | (RI_TYPE_GLOBAL << 2) | 2 ) HID_REPORT_DATA_2(315)
      = ( 4 << 4 | 1 << 2 | 2 ), U16_TO_U8S_LE(315)
      = ( 16 + 4 + 2), TU_U16_LOW(315), TU_U16_HIGH(315)
      = ( 22 ), ((uint8_t) (315 & 0x00ff)), ((uint8_t) ((315 >> 8) & 0x00ff))
      = ( 22 ), (0011 1011), (0000 0001)
      = 22, 59, 1 

315 = 256 + 32 + 16 + 8 + 2 + 1 = 0000 0001  0011 1011 = 

315 - 256 = 59 - 32 = 27 - 16 = 11 - 8 = 3 - 2 = 1
0x00FF = 255 (8 bits menos significativos en alto)

#define HID_REPORT_ITEM(data, tag, type, size)
      = (((tag) << 4) | ((type) << 2) | (size)) HID_REPORT_DATA_##size(data)
**/

// -------------------------- ESPECIFICACIONES PARA LPC OPEN ---------------------------

/**
 * USB HSConfiguration Descriptor
 * All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
 */
ALIGNED(4) uint8_t USB_HsConfigDescriptor[] = {
   /* Configuration 1 */
   USB_CONFIGURATION_DESC_SIZE,                /* bLength */
   USB_CONFIGURATION_DESCRIPTOR_TYPE,          /* bDescriptorType */
   WBVAL(                                      /* wTotalLength */
      USB_CONFIGURATION_DESC_SIZE +
      USB_INTERFACE_DESC_SIZE     +
      HID_DESC_SIZE               +
      USB_ENDPOINT_DESC_SIZE
      ),
   0x01,                                       /* bNumInterfaces */
   0x01,                                       /* bConfigurationValue */
   0x00,                                       /* iConfiguration */
   USB_CONFIG_SELF_POWERED,                    /* bmAttributes */
   USB_CONFIG_POWER_MA(2),                     /* bMaxPower */

   /* Interface 0, Alternate Setting 0, HID Class */
   USB_INTERFACE_DESC_SIZE,                    /* bLength */
   USB_INTERFACE_DESCRIPTOR_TYPE,              /* bDescriptorType */
   0x00,                                       /* bInterfaceNumber */
   0x00,                                       /* bAlternateSetting */
   0x01,                                       /* bNumEndpoints */
   USB_DEVICE_CLASS_HUMAN_INTERFACE,           /* bInterfaceClass */
   HID_SUBCLASS_BOOT,                          /* bInterfaceSubClass */
   HID_PROTOCOL_NONE,                      /* bInterfaceProtocol */
   0x04,                                       /* iInterface */
   /* HID Class Descriptor */
   /* HID_DESC_OFFSET = 0x0012 */
   HID_DESC_SIZE,                              /* bLength */
   HID_HID_DESCRIPTOR_TYPE,                    /* bDescriptorType */
   WBVAL(0x0111),                              /* bcdHID : 1.11*/
   0x00,                                       /* bCountryCode */
   0x01,                                       /* bNumDescriptors */
   HID_REPORT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
   WBVAL(sizeof(Gamepad_ReportDescriptor)),   /* wDescriptorLength */
   /* Endpoint, HID Interrupt In */
   USB_ENDPOINT_DESC_SIZE,                     /* bLength */
   USB_ENDPOINT_DESCRIPTOR_TYPE,               /* bDescriptorType */
   HID_EP_IN,                                  /* bEndpointAddress */
   USB_ENDPOINT_TYPE_INTERRUPT,                /* bmAttributes */
   WBVAL(0x0008),                              /* wMaxPacketSize */
   0x08,                                       /* bInterval */
   /* Terminator */
   0                                           /* bLength */
};

/**
 * USB FSConfiguration Descriptor
 * All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
 */

ALIGNED(4) uint8_t USB_FsConfigDescriptor[] = {
   /* Configuration 1 */
   USB_CONFIGURATION_DESC_SIZE,                /* bLength */
   USB_CONFIGURATION_DESCRIPTOR_TYPE,          /* bDescriptorType */
   WBVAL(                                      /* wTotalLength */
      USB_CONFIGURATION_DESC_SIZE   +
      USB_INTERFACE_DESC_SIZE     +
      HID_DESC_SIZE               +
      USB_ENDPOINT_DESC_SIZE
      ),
   0x01,                                       /* bNumInterfaces */
   0x01,                                       /* bConfigurationValue */
   0x00,                                       /* iConfiguration */
   USB_CONFIG_SELF_POWERED,                    /* bmAttributes */
   USB_CONFIG_POWER_MA(2),                     /* bMaxPower */

   /* Interface 0, Alternate Setting 0, HID Class */
   USB_INTERFACE_DESC_SIZE,                    /* bLength */
   USB_INTERFACE_DESCRIPTOR_TYPE,              /* bDescriptorType */
   0x00,                                       /* bInterfaceNumber */
   0x00,                                       /* bAlternateSetting */
   0x01,                                       /* bNumEndpoints */
   USB_DEVICE_CLASS_HUMAN_INTERFACE,           /* bInterfaceClass */
   HID_SUBCLASS_BOOT,                          /* bInterfaceSubClass */
   HID_PROTOCOL_NONE,                      /* bInterfaceProtocol */
   0x04,                                       /* iInterface */
   /* HID Class Descriptor */
   /* HID_DESC_OFFSET = 0x0012 */
   HID_DESC_SIZE,                              /* bLength */
   HID_HID_DESCRIPTOR_TYPE,                    /* bDescriptorType */
   WBVAL(0x0111),                              /* bcdHID : 1.11*/
   0x00,                                       /* bCountryCode */
   0x01,                                       /* bNumDescriptors */
   HID_REPORT_DESCRIPTOR_TYPE,                 /* bDescriptorType */
   WBVAL(sizeof(Gamepad_ReportDescriptor)),   /* wDescriptorLength */
   /* Endpoint, HID Interrupt In */
   USB_ENDPOINT_DESC_SIZE,                     /* bLength */
   USB_ENDPOINT_DESCRIPTOR_TYPE,               /* bDescriptorType */
   HID_EP_IN,                                  /* bEndpointAddress */
   USB_ENDPOINT_TYPE_INTERRUPT,                /* bmAttributes */
   WBVAL(0x0008),                              /* wMaxPacketSize */
   0x0A,                                       /* bInterval */
   /* Terminator */
   0                                           /* bLength */
};

/**
 * USB Standard Device Descriptor
 */
ALIGNED(4) const uint8_t USB_DeviceDescriptor[] = {
   USB_DEVICE_DESC_SIZE,         /* bLength */
   USB_DEVICE_DESCRIPTOR_TYPE,   /* bDescriptorType */
   WBVAL(0x0200),                /* bcdUSB: 2.00 */
   0x00,                         /* bDeviceClass */
   0x00,                         /* bDeviceSubClass */
   0x00,                         /* bDeviceProtocol */
   USB_MAX_PACKET0,              /* bMaxPacketSize0 */
   WBVAL(0x1FC9),                /* idVendor */
   WBVAL(0x0086),                /* idProduct */
   WBVAL(0x0100),                /* bcdDevice */
   0x01,                         /* iManufacturer */
   0x02,                         /* iProduct */
   0x03,                         /* iSerialNumber */
   0x01                          /* bNumConfigurations */
};

/**
 * USB Device Qualifier
 */
ALIGNED(4) const uint8_t USB_DeviceQualifier[] = {
   USB_DEVICE_QUALI_SIZE,                  /* bLength */
   USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,   /* bDescriptorType */
   WBVAL(0x0200),                          /* bcdUSB: 2.00 */
   0x00,                                   /* bDeviceClass */
   0x00,                                   /* bDeviceSubClass */
   0x00,                                   /* bDeviceProtocol */
   USB_MAX_PACKET0,                        /* bMaxPacketSize0 */
   0x01,                                   /* bNumOtherSpeedConfigurations */
   0x00                                    /* bReserved */
};


/**
 * USB String Descriptor (optional)
 */
const uint8_t USB_StringDescriptor[] = {
   /* Index 0x00: LANGID Codes */
   0x04,							      /* bLength */
   USB_STRING_DESCRIPTOR_TYPE,   /* bDescriptorType */
   WBVAL(0x0409),					   /* wLANGID  0x0409 = US English*/
   /* Index 0x01: Manufacturer */
   (18 * 2 + 2),					   /* bLength (18 Char + Type + length) */
   USB_STRING_DESCRIPTOR_TYPE,   /* bDescriptorType */
   'N', 0,
   'X', 0,
   'P', 0,
   ' ', 0,
   'S', 0,
   'e', 0,
   'm', 0,
   'i', 0,
   'c', 0,
   'o', 0,
   'n', 0,
   'd', 0,
   'u', 0,
   'c', 0,
   't', 0,
   'o', 0,
   'r', 0,
   's', 0,
   /* Index 0x02: Product */
   (16 * 2 + 2),					   /* bLength (16 Char + Type + length) */
   USB_STRING_DESCRIPTOR_TYPE,   /* bDescriptorType */
   'L', 0,
   'P', 0,
   'C', 0,
   '1', 0,
   '8', 0,
   'x', 0,
   'x', 0,
   ' ', 0,
   'G', 0,
   'A', 0,
   'M', 0,
   'E', 0,
   'P', 0,
   'A', 0,
   'D', 0,
   ' ', 0,
   /* Index 0x03: Serial Number */
   (13 * 2 + 2),					   /* bLength (13 Char + Type + length) */
   USB_STRING_DESCRIPTOR_TYPE,   /* bDescriptorType */
   'A', 0,
   'B', 0,
   'C', 0,
   'D', 0,
   '1', 0,
   '2', 0,
   '3', 0,
   '4', 0,
   '5', 0,
   '6', 0,
   '7', 0,
   '8', 0,
   '9', 0,
   /* Index 0x04: Interface 0, Alternate Setting 0 */
   (12 * 2 + 2),					   /* bLength (12 Char + Type + length) */
   USB_STRING_DESCRIPTOR_TYPE,   /* bDescriptorType */
   'H', 0,
   'I', 0,
   'D', 0,
   ' ', 0,
   'G', 0,
   'A', 0,
   'M', 0,
   'E', 0,
   'P', 0,
   'A', 0,
   'D', 0,
   ' ', 0,
};
