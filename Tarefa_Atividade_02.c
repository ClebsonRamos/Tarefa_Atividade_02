//-----BIBLIOTECAS NECESSÁRIAS-----
#include "pico/stdlib.h"
#include "pico/time.h"

//-----CONSTANTES PARA OS PINOS GPIO-----
#define PINO_LED_VERMELHO 13
#define PINO_LED_VERDE 11
#define PINO_LED_AZUL 12
#define PINO_BOTAO_A 5

//-----PROTÓTIPOS DAS FUNÇÕES-----
void ativacao_dos_leds(void);
int64_t funcao_de_callback(alarm_id_t id, void *user_data);
void inicializacao_dos_pinos(void);

//-----VARIÁVEIS GLOBAIS-----
absolute_time_t temporizador;
static volatile uint led_aceso = 0;
static volatile bool botao_pressionado = false;

//-----FUNÇÃO PRINCIPAL-----
int main(void){
    inicializacao_dos_pinos();

    while(true){
        if(gpio_get(PINO_BOTAO_A) == 0 && !botao_pressionado){
            sleep_ms(50);
            if(gpio_get(PINO_BOTAO_A) == 0){
                ativacao_dos_leds();
                add_alarm_in_ms(3000, funcao_de_callback, NULL, false);
                add_alarm_in_ms(6000, funcao_de_callback, NULL, false);
                add_alarm_in_ms(9000, funcao_de_callback, NULL, false);
            }
        }
    }

    return 0;
}

//-----FUNÇÕES COMPLEMENTARES-----
void ativacao_dos_leds(void){
    gpio_put(PINO_LED_AZUL, true);
    gpio_put(PINO_LED_VERDE, true);
    gpio_put(PINO_LED_VERMELHO, true);
}

int64_t funcao_de_callback(alarm_id_t id, void *user_data){
    switch(led_aceso){
        case 0:
            gpio_put(PINO_LED_AZUL, false);
            break;
        case 1:
            gpio_put(PINO_LED_VERDE, false);
            break;
        case 2:
            gpio_put(PINO_LED_VERMELHO, false);
            botao_pressionado = !botao_pressionado;
            break;
    }

    led_aceso--;

    return 0;
}

void inicializacao_dos_pinos(void){
    gpio_init(PINO_LED_AZUL);
    gpio_set_dir(PINO_LED_AZUL, GPIO_OUT);

    gpio_init(PINO_LED_VERDE);
    gpio_set_dir(PINO_LED_VERDE, GPIO_OUT);

    gpio_init(PINO_LED_VERMELHO);
    gpio_set_dir(PINO_LED_VERMELHO, GPIO_OUT);

    gpio_init(PINO_BOTAO_A);
    gpio_set_dir(PINO_BOTAO_A, GPIO_IN);
    gpio_pull_up(PINO_BOTAO_A);
}
