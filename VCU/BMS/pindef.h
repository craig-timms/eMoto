
// Isolation
#define GPIO_ISOP   25
#define ADC_ISON    26
#define GPIO_ISOPR  23
#define ADC_ISONR   1

// HV Control
#define GPIO_CONTACTOR_1    25
#define GPIO_CONTACTOR_2    26   
#define GPIO_PRE            1
#define GPIO_BLEED          5
#define GPIO_BLEED_T        3   // top of battery
#define GPIO_BLEED_B        23  // bottom of battery
#define GPIO_PRE_2          18
#define GPIO_BLEED_2        19

// Display
#define GPIO_LED_DISPLAY    15

#define NUM_LEDS_DISPLAY    7

// LV Monitor
#define GPIO_12V_READ   32
#define ADC_12V_READ    36      
#define ADC_12VI_C      34
#define ADC_12VI_O      39        
#define ADC_5VI         35

// Com pins
#define GPIO_SCL        22  // I2C
#define GPIO_SDA        21  // I2C
#define GPIO_CAN_TX     17  // UART
#define GPIO_CAN_RX     16  // UART

// Misc
#define GPIO_CHARGE     0
#define GPIO_WAKEUP     4
#define GPIO_IDC        13

// CAN IDs
#define CANID_CHARGER_GET   0x18EB2440
#define CANID_CHARGER_SET   0x18E54024
#define CANID_MCU_1         0x0CF11E05
#define CANID_MCU_2         0x0CF11F05
#define CANID_CONTROLS      0x18EB243C
#define CANID_STATE         0x18EB244B
#define CANID_BATTERY       0x18EB2432

// App
#define VP_PRE 0
#define VP_BLEED 1
#define VP_VDIGITAL 2
#define VP_VLEVEL 3
