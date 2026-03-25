#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int BTN_PIN_Y = 28;
const int BTN_PIN_B = 22;
const int BTN_PIN_G = 18;
const int LED_PIN_Y = 6;
const int LED_PIN_B = 10;
const int LED_PIN_G = 14;

volatile int flag_btn_y  = 0;
volatile int flag_btn_g  = 0;
volatile int flag_btn_b  = 1;


void gpio_callback(uint gpio, uint32_t events){
  if (gpio == BTN_PIN_Y){
    if (events == 0x4){
      flag_btn_y = 1;
    }
  }
  else if(gpio == BTN_PIN_B){
    if (events == 0x8){
      flag_btn_b = 0;
    }
  }
  else if(gpio == BTN_PIN_G){
    if(events == 0x4){
      flag_btn_g = 1;
    }
    else if (events == 0x8){
      flag_btn_g = 0;
    }
  }
}

int main() {
  stdio_init_all();
  gpio_init(BTN_PIN_Y);
  gpio_set_dir(BTN_PIN_Y, GPIO_IN);
  gpio_pull_up(BTN_PIN_Y);  

  gpio_init(LED_PIN_Y);
  gpio_set_dir(LED_PIN_Y, GPIO_OUT);
  
  gpio_init(BTN_PIN_B);
  gpio_set_dir(BTN_PIN_B, GPIO_IN);
  gpio_pull_up(BTN_PIN_B);   

  gpio_init(LED_PIN_B);
  gpio_set_dir(LED_PIN_B, GPIO_OUT);

  gpio_init(BTN_PIN_G);
  gpio_set_dir(BTN_PIN_G, GPIO_IN);
  gpio_pull_up(BTN_PIN_G);   

  gpio_init(LED_PIN_G);
  gpio_set_dir(LED_PIN_G, GPIO_OUT);

  gpio_set_irq_enabled_with_callback(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
  gpio_set_irq_enabled(BTN_PIN_B,GPIO_IRQ_EDGE_RISE, true);
  gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE , true);

  int amarelo = 0;
  int azul = 1;

  int led_status_y = 0;
  int led_status_b = 0;
  int led_status_g = 0;


  while (true) {
    if(flag_btn_y && amarelo == 0){
      amarelo = 1;
      flag_btn_y = 0;
    }

    if(amarelo == 1){
      led_status_y = !led_status_y;
      gpio_put(LED_PIN_Y,led_status_y);
      sleep_ms(200);
    }
    if(flag_btn_y && amarelo == 1){
      amarelo = 0;
      flag_btn_y = 0;
      led_status_y = 0;
      gpio_put(LED_PIN_Y,led_status_y);
    }

    if(flag_btn_b == 0 && azul == 1){
      azul = 0;
      flag_btn_b = 1;
    }
     if(azul == 0){
      led_status_b = !led_status_b;
      gpio_put(LED_PIN_B,led_status_b);
      sleep_ms(200);
    }
    if(flag_btn_b == 0 && azul == 0){
      azul = 1;
      flag_btn_b= 1;
      led_status_b = 0;
      gpio_put(LED_PIN_B,led_status_b);
    }
    if (flag_btn_g == 1){
      led_status_g = !led_status_g;
      gpio_put(LED_PIN_G,led_status_g);
      sleep_ms(200);
    }
    if(flag_btn_g == 0){
      led_status_g = 0;
      gpio_put(LED_PIN_G,led_status_g);
    }
  }
}