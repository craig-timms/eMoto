
// MCU pins
#define DAC_THROTTLE_OUT    25
#define DAC_BRAKE_OUT       26
#define GPIO_MCU_POWER      23
#define GPIO_BRAKE_AN       18
#define GPIO_FOOT           3

// Signal pins
#define GPIO_LB         34
#define GPIO_HB         27
#define GPIO_HORN_OUT   19
// #define GPIO_EXTRA_12V  18
const uint8_t GPIO_LED_TURN = 0;
const uint8_t GPIO_LED_BACK = 15;

// const uint16_t  = 24+6+28;
const uint16_t NUM_LEDS_TURN = 24;
const uint16_t NUM_LEDS_DASH = 6;
const uint16_t NUM_LEDS_BACK = 28;
const uint16_t NUM_LEDS = NUM_LEDS_TURN + NUM_LEDS_DASH + NUM_LEDS_BACK;
const uint16_t NUM_LEDS_BT = 8;

const uint16_t DASH_LED_START = NUM_LEDS_TURN;
const uint16_t DASH_LED_END = NUM_LEDS_TURN+NUM_LEDS_DASH;
const uint16_t DASH_LED_1 = DASH_LED_START;
const uint16_t DASH_LED_2 = DASH_LED_START + 1;
const uint16_t DASH_LED_3 = DASH_LED_START + 2;
const uint16_t DASH_LED_4 = DASH_LED_START + 3;
const uint16_t DASH_LED_5 = DASH_LED_START + 4;
const uint16_t DASH_LED_6 = DASH_LED_START + 5;

// Dash pins
// const uint8_t GPIO_LED_DASH = 14;

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
#define CANID_CONTROLS      0x100
#define CANID_STATE         0x200
#define CANID_BATTERY       0x300


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
#define VP_MCU_POWER    41
#define VP_MCU_REVERSE  40
#define VP_MCU_BRAKEAN  42
#define VP_MCU_BRAKETH  43
#define VP_MCU_ONLINE   44

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
