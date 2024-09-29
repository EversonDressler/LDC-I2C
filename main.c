
#include <stdio.h>
#include <string.h>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"



const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// Sinalizadores para o modo de entrada de exibição
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// Sinalizadores para exibição e controle do cursor
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// Bandeiras para exibição e mudança de cursor
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// Sinalizadores para o conjunto de funções
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// Bandeira para controle da luz de fundo
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// Por padrão, esses drivers de tela LCD estão no endereço de ônibus 0x27
static int addr = 0x27;

// Modos para lcd_send_byte
#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define MAX_LINES      2
#define MAX_CHARS      16

void read_uC_temperatura();


/* Função de auxiliar rápido para transferências de bytes únicas */
void i2c_write_byte(uint8_t val) {
#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
#endif
}

void lcd_toggle_enable(uint8_t val) {
    // Toggle enable PIN na tela LCD
    // Não podemos fazer isso muito rapidamente ou as coisas não funcionam
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

// A tela é enviada um byte, pois duas transferências de mordisinhas separadas
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

// vá para o local no LCD
void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

static void inline lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

int main() {

    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);                // PARA TESTE COM POTENCIOMETRO COLOCAR VALOR 0


    i2c_init(i2c_default, 100 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Disponibilize os pinos i2c para Picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_init();

    static char *message[] =
    {
        "Everson", "Dressler",
        "Temperatura", "do Nucleo",
        "Raspberry", "PI PICO"
    };

    for(int m = 0; m < sizeof(message) / sizeof(message[0]); m += MAX_LINES){
        for (int line = 0; line < MAX_LINES; line++) {
            lcd_set_cursor(line, (MAX_CHARS / 2) - strlen(message[m + line]) / 2);
            lcd_string(message[m + line]);
        }
        sleep_ms(2000);
        lcd_clear();
    }

    for(;;){
        
        read_uC_temperatura();
        sleep_ms(700);
        
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(300);

    }
}


void read_uC_temperatura(){
    
    char c[5], f[5];

    float adc = (float)adc_read() * 3.3f / (1 << 12);
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    printf("Temperatura do uC = %.02f °C\n", tempC);
    printf("Temperatura do uC = %.02f °F\n\n", (tempC * (9/5)) + 32);

    sprintf(c, "%.02f",  tempC);
    sprintf(f, "%.02f",  (tempC * (9/5)) + 32);

    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_string("  Temperaturas  ");

    lcd_set_cursor(1, 0);
    lcd_string(c);
    lcd_string("C");

    lcd_set_cursor(1, 9);
    lcd_string(f);
    lcd_string("F");

    gpio_put(PICO_DEFAULT_LED_PIN, 1);
}