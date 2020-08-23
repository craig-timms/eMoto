
// MCU pins
#define DAC_THROTTLE_OUT    25
#define DAC_BRAKE_OUT       26
#define GPIO_MCU_POWER      23
#define GPIO_BRAKE_AN       1
#define GPIO_FOOT           3

// Signal pins
#define GPIO_LB         34
#define GPIO_HB         27
#define GPIO_HORN_OUT   19
#define GPIO_LED_TURN   0
#define GPIO_LED_BACK   15
#define GPIO_EXTRA_12V  18

#define NUM_LEDS_B      8
#define NUM_LEDS_TURN   8
#define NUM_LEDS_ANGEL  8

// Dash pins
#define GPIO_LED_DASH   14
// #define GPIO_POT_DASH   35

#define NUM_LEDS_DASH      7

// Monitor pins
#define GPIO_WAKEUP_OUT     5
#define GPIO_KEY_IN         12
#define ADC_THROTTLE_IN     33  
#define GPIO_HL_IN          32  //
#define GPIO_R_IN           4   //
#define GPIO_L_IN           2   //
#define GPIO_HORN_IN        13  //
#define ADC_MOTOR_TEMP_1    39
#define ADC_MOTOR_TEMP_2    36
#define ADC_REGEN           35

// Com pins
#define GPIO_SCL        22  // I2C
#define GPIO_SDA        21  // I2C
#define GPIO_CAN_TX     17  // UART
#define GPIO_CAN_RX     16  // UART

// CAN IDs
#define CANID_CHARGER_GET   0x18EB2440
#define CANID_CHARGER_SET   0x18E54024
#define CANID_MCU_1         0x0CF11E05
#define CANID_MCU_2         0x0CF11F05
#define CANID_CONTROLS      0x18EB243C
#define CANID_STATE         0x18EB244B
#define CANID_BATTERY       0x18EB2432


////////////////////////////////
//////////  App   //////////////
////////////////////////////////
// Motor
#define VP_RPM  0
#define VP_I    1
#define VP_V    2
#define VP_T    3
#define VP_LCD  4 // 16x2
#define VP_B1   5  // ignition
#define VP_B2   6
#define VP_SLD  7

// Charger
#define VP_start_charge 22
#define VP_charging     25
#define VP_sld_Vmax     20
#define VP_sld_Imax     21
#define VP_val_V        23
#define VP_val_I        24
#define VP_CERR_VAC     26
#define VP_CERR_HW      27
#define VP_CERR_TEMP    28
#define VP_CERR_COM     29

// Lights
#define VP_angelRGB     30
#define VP_angel        31
#define VP_headlight    32
#define VP_brake        33
#define VP_turn         34
