
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int BTN_PIN = 28;
const int LED_PIN_Y = 5;
const int LED_PIN_B = 9;

volatile int flag_btn = 0;
volatile int flag_y_timer = 0;
volatile int flag_b_timer = 0;
volatile int flag_alarm = 0;


void gpio_callback(uint gpio, uint32_t events){
  if (gpio == BTN_PIN){
    if (events == 0x4){ 
      flag_btn = 1;
    }
  }
}

bool timer_y_callback(repeating_timer_t *rt) {
    flag_y_timer = 1;
    return true;  // keep repeating
}

bool timer_b_callback(repeating_timer_t *rt) {
    flag_b_timer = 1;
    return true;  // keep repeating
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    flag_alarm = 1;
    return 0;
}

int main() {
    stdio_init_all();
    gpio_init(BTN_PIN);
    gpio_set_dir(BTN_PIN, GPIO_IN);
    gpio_pull_up(BTN_PIN);  

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);
    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    int led_status_y = 0;
    int led_status_b = 0;
    int botao_pressionado = 0;

    repeating_timer_t timer_amarelo;
    repeating_timer_t timer_azul;
    alarm_id_t alarm = 0;

    while (true) {
        if (flag_btn && botao_pressionado == 0){
            flag_btn = 0;
            alarm = add_alarm_in_ms(5000, alarm_callback, NULL, false);
            add_repeating_timer_ms(500, timer_y_callback, NULL, &timer_amarelo); // período é o ciclo de desligar e ligar, então se o período é 500ms, valor no timer é 250ms, pq a cada 250ms ele liga, e depois de 250ms desliga = 1 período.
            add_repeating_timer_ms(150, timer_b_callback, NULL, &timer_azul);
        }
        if (!flag_alarm && flag_y_timer){
            flag_y_timer = 0;
            led_status_y = !led_status_y;
            gpio_put(LED_PIN_Y,led_status_y);
        }
        if (!flag_alarm && flag_b_timer){
            flag_b_timer = 0;
            led_status_b = !led_status_b;
            gpio_put(LED_PIN_B,led_status_b);
        }
        if (flag_alarm){
            flag_alarm = 0;
            flag_y_timer = 0;
            flag_b_timer= 0;
            led_status_y = 0;
            led_status_b = 0;
            cancel_alarm(alarm);
            cancel_repeating_timer(&timer_amarelo);
            cancel_repeating_timer(&timer_azul);
            gpio_put(LED_PIN_Y,led_status_y);
            gpio_put(LED_PIN_B,led_status_b);
        }
    }
}
