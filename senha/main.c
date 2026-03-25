#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"

const int BTN_PIN_G = 28;
const int BTN_PIN_B = 27;
const int BTN_PIN_Y = 21;
const int BTN_PIN_W = 17;

const int LED_PIN_P = 26;
const int LED_PIN_G = 9;
const int LED_PIN_R = 2;

volatile int flag_btn_g = 0;
volatile int flag_btn_b = 0;
volatile int flag_btn_y = 0;
volatile int flag_btn_w = 0;


void btn_callback(uint gpio, uint events){
    if (gpio == BTN_PIN_G){
        if (events == 0x4){ //fall
            flag_btn_g = 1;
        }
    }
    else if(gpio == BTN_PIN_B){
        if (events == 0x4){
            flag_btn_b = 1;
        }
    }
    else if(gpio == BTN_PIN_Y){
        if (events == 0x4){
            flag_btn_y = 1;
        }
    }
    else if(gpio == BTN_PIN_W){
        if (events == 0x4){
            flag_btn_w = 1;
        }
    }
}

int main() {
    stdio_init_all();
    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);  
    
    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);  

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);
    
    gpio_init(BTN_PIN_W);
    gpio_set_dir(BTN_PIN_W, GPIO_IN);
    gpio_pull_up(BTN_PIN_W);   

    gpio_init(LED_PIN_P);
    gpio_set_dir(LED_PIN_P, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    
    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_PIN_W, GPIO_IRQ_EDGE_FALL, true);

    int senha_informada = 0;
    int tentativa_feita = 0;
    int senha_correta[4];
    int senha_tentativa[4];
    int botoes_apertados = 0;
    int led_status_p = 0;
    int led_status_g = 0;
    int led_status_r = 0;

    int igual = 1; 
    while (1) {
        if (senha_informada == 0){
            if (flag_btn_g && botoes_apertados < 4){
                flag_btn_g = 0;
                senha_correta[botoes_apertados] = 28;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_b && botoes_apertados < 4){
                flag_btn_b = 0;
                senha_correta[botoes_apertados] = 27;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_y && botoes_apertados < 4){
                flag_btn_y = 0;
                senha_correta[botoes_apertados] = 21;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_w && botoes_apertados < 4){
                flag_btn_w = 0;
                senha_correta[botoes_apertados] = 17;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
        }
        if (botoes_apertados == 4 && senha_informada == 0){
            senha_informada = 1;
            botoes_apertados = 0;
        }
        if (tentativa_feita == 0 && senha_informada){
            if (flag_btn_g && botoes_apertados < 4){
                flag_btn_g = 0;
                senha_tentativa[botoes_apertados] = 28;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_b && botoes_apertados < 4){
                flag_btn_b = 0;
                senha_tentativa[botoes_apertados] = 27;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_y && botoes_apertados < 4){
                flag_btn_y = 0;
                senha_tentativa[botoes_apertados] = 21;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
            if (flag_btn_w && botoes_apertados < 4){
                flag_btn_w = 0;
                senha_tentativa[botoes_apertados] = 17;
                botoes_apertados ++;
                led_status_p = 1;
                gpio_put(LED_PIN_P,led_status_p);
                sleep_ms(300);
                led_status_p = 0;
                gpio_put(LED_PIN_P,led_status_p);
            }
        }
        if (botoes_apertados == 4 && tentativa_feita == 0){
            tentativa_feita = 1;
            botoes_apertados = 0;
        }
        if (tentativa_feita){
            for (int i = 0; i < 4; i++) {
                if (senha_correta[i] != senha_tentativa[i]) {
                    igual = 0;
                    break;
                }
            }
            if (igual){
                led_status_g = 1;
                gpio_put(LED_PIN_G,led_status_g);
                sleep_ms(300);
                led_status_g = 0;
                gpio_put(LED_PIN_G,led_status_g);
                senha_informada = 0;
                tentativa_feita = 0;
                botoes_apertados = 0;
                igual = 1;
            } else if (igual == 0){
                led_status_r = 1;
                gpio_put(LED_PIN_R,led_status_r);
                sleep_ms(300);
                led_status_r = 0;
                gpio_put(LED_PIN_R,led_status_r);
                tentativa_feita = 0;
                botoes_apertados = 0;
                igual = 1;
            }
        }
    }
}