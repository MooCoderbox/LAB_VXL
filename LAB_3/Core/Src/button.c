#include "main.h"
#include "button.h"
#include "config.h"

/* The board wiring in this project uses active-low buttons with pull-ups. */
// typedef struct {
//    GPIO_TypeDef *port;
//    uint16_t pin;
//    uint8_t candidate_pressed;
//    uint8_t stable_pressed;
//    uint8_t debounce_count;
//    volatile uint8_t pressed_event;
// } ButtonState;

// static ButtonState buttons[BUTTON_COUNT];

// void Button_Init(void)
// {
//    buttons[BUTTON_MODE].port = GPIOB;
//    buttons[BUTTON_MODE].pin = But1_Pin;

//    buttons[BUTTON_INCREMENT].port = GPIOB;
//    buttons[BUTTON_INCREMENT].pin = But2_Pin;

//    buttons[BUTTON_CONFIRM].port = GPIOB;
//    buttons[BUTTON_CONFIRM].pin = But3_Pin;

//    for (uint32_t i = 0U; i < BUTTON_COUNT; i++) {
//        uint8_t raw_pressed =
//            (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == GPIO_PIN_RESET)
//                ? 1U : 0U;

//        buttons[i].candidate_pressed = raw_pressed;
//        buttons[i].stable_pressed = raw_pressed;
//        buttons[i].debounce_count = BUTTON_DEBOUNCE_SAMPLES;
//        buttons[i].pressed_event = 0U;
//    }
// }

// /* This routine is called at 10 ms intervals from the TIM2 interrupt. */
// void Button_ScanISR(void)
// {
//    for (uint32_t i = 0U; i < BUTTON_COUNT; i++) {
//        uint8_t raw_pressed =
//            (HAL_GPIO_ReadPin(buttons[i].port, buttons[i].pin) == GPIO_PIN_RESET)
//                ? 1U : 0U;

//        if (raw_pressed == buttons[i].candidate_pressed) {
//            if (buttons[i].debounce_count < BUTTON_DEBOUNCE_SAMPLES) {
//                buttons[i].debounce_count++;
//            }
//        } else {
//            buttons[i].candidate_pressed = raw_pressed;
//            buttons[i].debounce_count = 1U;
//        }

//        if ((buttons[i].debounce_count >= BUTTON_DEBOUNCE_SAMPLES) &&
//            (buttons[i].stable_pressed != raw_pressed)) {
//            buttons[i].stable_pressed = raw_pressed;

//            /* Generate an event only on the transition to PRESSED. */
//            if (buttons[i].stable_pressed != 0U) {
//                buttons[i].pressed_event = 1U;
//            }
//        }
//    }
// }

// uint8_t Button_GetPressedEvent(ButtonId id)
// {
//    uint8_t event;
//    uint32_t primask;

//    if ((id < 0) || (id >= BUTTON_COUNT)) {
//        return 0U;
//    }

//    /* Atomic read-and-clear relative to the TIM2 ISR. */
//    primask = __get_PRIMASK();
//    __disable_irq();
//    event = buttons[id].pressed_event;
//    buttons[id].pressed_event = 0U;
//    __set_PRIMASK(primask);

//    return event;
// }

#define NORMAL_STATE GPIO_PIN_SET
#define PRESSED_STATE GPIO_PIN_RESET
#define NUM_BUTTONS 3

static GPIO_TypeDef* const button_ports[NUM_BUTTONS] = {
    But1_GPIO_Port, But2_GPIO_Port, But3_GPIO_Port
};
static const uint16_t button_pins[NUM_BUTTONS] = {
    But1_Pin, But2_Pin, But3_Pin
};

int KeyReg0[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg1[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg2[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg3[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int button_flag[NUM_BUTTONS] = {0, 0, 0};

static int isButtonPressed(int index){
    if(button_flag[index] == 1){
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

int isButton1Pressed(){
    return isButtonPressed(0);
}

int isButton2Pressed(){
    return isButtonPressed(1);
}

int isButton3Pressed(){
    return isButtonPressed(2);
}

void getKeyInput(){
    for(int i = 0; i < NUM_BUTTONS; i++){
        KeyReg0[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg2[i];
        KeyReg2[i] = HAL_GPIO_ReadPin(button_ports[i], button_pins[i]);
        if((KeyReg0[i] == KeyReg1[i]) && (KeyReg1[i] == KeyReg2[i])){
            if(KeyReg3[i] != KeyReg2[i]){
                KeyReg3[i] = KeyReg2[i];
                if(KeyReg2[i] == PRESSED_STATE){
                    button_flag[i] = 1;
                }
            }
        }
    }
}


