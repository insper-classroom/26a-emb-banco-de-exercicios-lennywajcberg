#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

const int BTN_PIN_G = 28;
const int BTN_PIN_Y = 26;

const int LED_PIN_G = 5;
const int LED_PIN_Y = 9;

volatile int flag_btn_g = 0;
volatile int flag_btn_y = 0;
volatile int flag_timer_g = 0;
volatile int flag_timer_y = 0;
volatile int flag_alarm_g = 0;
volatile int flag_alarm_y = 0;




void gpio_callback(uint gpio, uint events){
    if (gpio == BTN_PIN_G){
        if (events == 0x4){
            flag_btn_g = 1;
        }
    }else if(gpio == BTN_PIN_Y){
        if (events == 0x4){
            flag_btn_y = 1;
        }
    }
}

bool timer_g_callback(repeating_timer_t *rt) {
    flag_timer_g = 1;
    return true;  // keep repeating
}

bool timer_y_callback(repeating_timer_t *rt) {
    flag_timer_y = 1;
    return true;  // keep repeating
}


int64_t alarm_g_callback(alarm_id_t id, void *user_data) {
    flag_alarm_g = 1;
    return 0;
}

int64_t alarm_y_callback(alarm_id_t id, void *user_data) {
    flag_alarm_y = 1;
    return 0;
}


int main(){
    stdio_init_all();
    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);  

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    
    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);   

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);

    repeating_timer_t timer_g;
    if (!add_repeating_timer_ms(100, timer_g_callback, NULL, &timer_g)) {
        printf("Failed to add timer\n");
    }

    repeating_timer_t timer_y;
    if (!add_repeating_timer_ms(250, timer_y_callback, NULL, &timer_y)) {
        printf("Failed to add timer\n");
    }
    
    
    alarm_id_t alarm_g = NULL;
    alarm_id_t alarm_y = NULL;

    int led_status_g = 0;
    int led_status_y = 0;

    
    int alarm_enable_g = 0;
    int alarm_enable_y = 0;
 

    while (1) {
        if (flag_btn_g){
            flag_btn_g = 0;
            alarm_g = add_alarm_in_ms(1000, alarm_g_callback, NULL, false);
            alarm_enable_g = 1;
        }
        if (flag_btn_y){
            flag_btn_y = 0;
            alarm_y = add_alarm_in_ms(2000, alarm_y_callback, NULL, false);
            alarm_enable_y = 1;
        }

        if (alarm_enable_g && flag_timer_g){
            led_status_g = !led_status_g;
            gpio_put(LED_PIN_G,led_status_g);
            flag_timer_g = 0;
        } else if (alarm_enable_g == 0){
            gpio_put(LED_PIN_G,0);
        }

        if (alarm_enable_y && flag_timer_y){
            led_status_y = !led_status_y;
            gpio_put(LED_PIN_Y,led_status_y);
            flag_timer_y = 0;
        }else if (alarm_enable_y == 0){
            gpio_put(LED_PIN_Y,0);
        }

        if (flag_alarm_g == 1 || flag_alarm_y == 1){
            flag_alarm_g = 0;
            alarm_enable_g = 0;

            flag_alarm_y = 0;
            alarm_enable_y = 0;
            cancel_alarm(alarm_g);
            cancel_alarm(alarm_y);
        }
        }
    }

