/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

const int BTN_PIN = 28;
const int LED_PIN = 5;

volatile int flag_btn = 0;
volatile int flag_timer = 0;
volatile int flag_pisca_timer = 0;



void btn_callback(uint gpio, uint events){
    if (events == 0x4){ //fall
        flag_btn = 1;
    }else if(events == 0x8){ //rise
        flag_btn = 0;
    }
}

bool timer_callback(repeating_timer_t *rt) {
    flag_timer = 1;
    return true;  // keep repeating
}

bool timer_pisca_callback(repeating_timer_t *rt) {
    flag_pisca_timer = 1;
    return true;  // keep repeating
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);  

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE , true, &btn_callback);

    uint64_t tempo_fall, tempo_rise;
    repeating_timer_t timer;
    repeating_timer_t timer_piscadas;

    int led_status = 0;
    int botao_apertado = 0;
    
    while (true) {
        if (flag_btn == 1 && botao_apertado == 0){ //pra pegar a primeira instancia do tempo
            tempo_fall = to_us_since_boot(get_absolute_time());
            gpio_put(LED_PIN,0);
            botao_apertado = 1;
        }
        if (flag_btn == 0 && botao_apertado == 1){
            botao_apertado = 0;
            tempo_rise = to_us_since_boot(get_absolute_time());
            int64_t duracao = absolute_time_diff_us(tempo_fall, tempo_rise);
            printf("fall: %llu", tempo_fall);
            printf("rise: %llu", tempo_rise);
            printf("duracao: %lld", duracao);
            add_repeating_timer_us(duracao, timer_callback, NULL, &timer);
            add_repeating_timer_us(250000, timer_pisca_callback, NULL, &timer_piscadas);
        }
        if (!flag_timer){
            if (flag_pisca_timer){
                led_status = !led_status;
                gpio_put(LED_PIN,led_status);
                flag_pisca_timer = 0;
            }
        }

        if (flag_timer){
            cancel_repeating_timer(&timer);
            cancel_repeating_timer(&timer_piscadas);
            led_status = 0;
            gpio_put(LED_PIN,0);
            botao_apertado = 0;
            flag_timer = 0;

        }
    }
}