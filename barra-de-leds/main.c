#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int SW_PIN = 28;
const int BTN = 22;

volatile int flag_sw = 0;
volatile int flag_btn = 0;

const int LED_1 = 2;
const int LED_2 = 3;
const int LED_3 = 4;
const int LED_4 = 5;
const int LED_5 = 6;

void bar_init(){
    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);
    gpio_init(LED_3);
    gpio_set_dir(LED_3, GPIO_OUT);
    gpio_init(LED_4);
    gpio_set_dir(LED_4, GPIO_OUT);
    gpio_init(LED_5);
    gpio_set_dir(LED_5, GPIO_OUT);
}

void gpio_callback(uint gpio, uint32_t events){
    if (gpio == BTN){
        if (events == 0x4){ //fall
            flag_btn = 1;
        }
    }
    else if(gpio == SW_PIN){
        if (events == 0x4){
            flag_sw = 0;
        }
        else if(events == 0x8){
            flag_sw = 1;
        }

    }
}

void bar_display(int val){
    gpio_put(LED_1, val >= 1);
    gpio_put(LED_2, val >= 2);
    gpio_put(LED_3, val >= 3);
    gpio_put(LED_4, val >= 4);
    gpio_put(LED_5, val >= 5);
}

int main() {
    stdio_init_all();
    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);   

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);   
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN, GPIO_IRQ_EDGE_FALL, true);
    

    bar_init();
    int val = 0;
    while (true) {
        if (flag_btn == 1){
            if (flag_sw == 0){
                val ++;
                if (val < 0){
                    val = 0;
                }
                if (val > 5){
                    val = 5;
                }
                bar_display(val);
            }
            if (flag_sw == 1){
                val --;
                if (val < 0){
                    val = 0;
                }
                if (val > 5){
                    val = 5;
                }
                bar_display(val);
            }
            flag_btn = 0;
        }
    }
}
