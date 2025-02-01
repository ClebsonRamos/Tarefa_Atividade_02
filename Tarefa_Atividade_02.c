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
int64_t funcao_de_callback_01(alarm_id_t id, void *user_data);
int64_t funcao_de_callback_02(alarm_id_t id, void *user_data);
int64_t funcao_de_callback_03(alarm_id_t id, void *user_data);
void inicializacao_dos_pinos(void);

//-----VARIÁVEIS GLOBAIS-----
static volatile bool botao_pressionado = false; // Variável para registro do pressionamento do botão.
static volatile bool alarme_ativado = false; // Variável para registro da ativação dos alarmes.

//-----FUNÇÃO PRINCIPAL-----
int main(void){
    inicializacao_dos_pinos(); // Inicialização dos pinos dos LEDs e do botão utilizados.

    while(true){
        // Desvio condicional para testar se o botão foi pressionado.
        // A variável botao_pressionado registra se o botão foi efetivamente pressionado, após o tratamento do debounce.
        if(gpio_get(PINO_BOTAO_A) == 0 && !botao_pressionado){
            sleep_ms(50); // Atraso de 50 milissegundos para o debounce.
            if(gpio_get(PINO_BOTAO_A) == 0){ // Testa novamente de o botão foi pressionado.
                // Registra o pressionamento do botão, para evitar pressionamentos consecutivos durante a execução da função de callback.
                botao_pressionado = !botao_pressionado;
                // Ativa todos os LEDs.
                ativacao_dos_leds();
                if(!alarme_ativado){
                    // Ativação dos alarmes temporizados.
                    add_alarm_in_ms(3000, funcao_de_callback_01, NULL, false);
                    add_alarm_in_ms(6000, funcao_de_callback_02, NULL, false);
                    add_alarm_in_ms(9000, funcao_de_callback_03, NULL, false);
                    // Registro da ativação dos alarmes.
                    alarme_ativado = !alarme_ativado;
                }
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

int64_t funcao_de_callback_01(alarm_id_t id, void *user_data){
    gpio_put(PINO_LED_AZUL, false);
    return 0;
}

int64_t funcao_de_callback_02(alarm_id_t id, void *user_data){
    gpio_put(PINO_LED_VERDE, false);
    return 0;
}

int64_t funcao_de_callback_03(alarm_id_t id, void *user_data){
    gpio_put(PINO_LED_VERMELHO, false);
    botao_pressionado = !botao_pressionado;
    alarme_ativado = !alarme_ativado;
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
