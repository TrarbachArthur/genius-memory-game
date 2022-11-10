#include <Arduino.h>
#include <stdlib.h>
#include <time.h>

/*EzButton - Olhar biblioteca pra tentar refatorar essa obra de arte*/ 

#define led_1 10
#define led_2 11
#define led_3 12
#define led_4 13

#define dificuldade1_pin 3
#define dificuldade2_pin 2

#define bt_1_pin 7
#define bt_2_pin 6
#define bt_3_pin 5
#define bt_4_pin 4

int dificuldade = 0;
int leds_aleatorio[100];
int delay_dificuldade = 300;
int max_leds = 0;
int atual_leds = -1;

int delay_padrao = 500;

int botoes_apertados[100];
int botao_atual = 0;
// se apertar qualquer botao, inicia o jogo

bool inicia_jogo = false;
bool ta_jogando = false;
bool perdeu = false;
bool acertou = false;

void gera_aleatorio() {
	atual_leds++;
    randomSeed(analogRead(A0));
    leds_aleatorio[atual_leds] = 10 + (random(300000000) % 4);
}
/*
int mapeia_leds(int x) {
    if (x == 0) return led_1;
    if (x == 1) return led_2;
    if (x == 3) return led_3;
    if (x == 2) return led_4;

    return 0;
}
*/
void liga_led_aleatorio() {
    int led_ligar;

    for (int i = 0; i <= atual_leds; i++) {
        led_ligar = leds_aleatorio[i];

        digitalWrite(led_ligar, HIGH);
        delay(delay_dificuldade);
        digitalWrite(led_ligar, LOW);
        delay(delay_dificuldade);
    }
}

void pisca_led(int porta_led) {
	digitalWrite(porta_led, HIGH);
    delay(delay_padrao);
    digitalWrite(porta_led, LOW);
    delay(50);
}

void rodar() {
    int pisca = 100;
    digitalWrite(led_1, HIGH);
    delay(pisca);
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, HIGH);
    delay(pisca);
    digitalWrite(led_2, LOW);
    digitalWrite(led_4, HIGH);
    delay(pisca);
    digitalWrite(led_4, LOW);
    digitalWrite(led_3, HIGH);
    delay(pisca);
    digitalWrite(led_3, LOW);
}

void desligaLeds() {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
    digitalWrite(led_4, LOW);
}

void start() {
    if (!inicia_jogo) inicia_jogo = true;
}

void dificuldade1_bt() {
	if (!dificuldade) {
		Serial.println("Apertou botao da dificuldade 1!");
		dificuldade = 1;
		delay_dificuldade = 300;
		max_leds = 5;
	}
}

void dificuldade2_bt() {
	if (!dificuldade) {
		Serial.println("Apertou botao da dificuldade 2!");
		dificuldade = 2;
		delay_dificuldade = 120;
		max_leds = 8;
	}
}

bool conferir_botoes_atuais() {
    for (int i = 0; i <= atual_leds && i < botao_atual; i++) {
        if (leds_aleatorio[i] != botoes_apertados[i]) {
            perdeu = true;
            return false;
        }
    }
    return true;
}

void bt1() {
    Serial.println("Apertou botao 1");
	pisca_led(led_1);
    botoes_apertados[botao_atual] = led_1;
    botao_atual += 1;

    if (!conferir_botoes_atuais()) {
        perdeu = true;
    }
}

void bt2() {
    Serial.println("Apertou botao 2");
	pisca_led(led_2);
    botoes_apertados[botao_atual] = led_2;
    botao_atual += 1;

    if (!conferir_botoes_atuais()) {
        perdeu = true;
    }
}

void bt3() {
    Serial.println("Apertou botao 3");
	pisca_led(led_3);
    botoes_apertados[botao_atual] = led_3;
    botao_atual += 1;

    if (!conferir_botoes_atuais()) {
        perdeu = true;
    }
}

void bt4() {
    Serial.println("Apertou botao 4");
	pisca_led(led_4);
    botoes_apertados[botao_atual] = led_4;
    botao_atual += 1;

    if (!conferir_botoes_atuais()) {
        perdeu = true;
    }
}

void encerra_partida() {
    dificuldade = 0;
    delay_dificuldade = 300;
    max_leds = 0;
    atual_leds = -1;
    botao_atual = 0;
    // se apertar qualquer botao, inicia o jogo
    inicia_jogo = false;
    ta_jogando = false;
    perdeu = false;
    acertou = false;
}

void setup() {
    Serial.begin(9600);

	pinMode(dificuldade1_pin, INPUT_PULLUP);
    pinMode(dificuldade2_pin, INPUT_PULLUP);

    pinMode(bt_1_pin, INPUT_PULLUP);
    pinMode(bt_2_pin, INPUT_PULLUP);
    pinMode(bt_3_pin, INPUT_PULLUP);
    pinMode(bt_4_pin, INPUT_PULLUP);

    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
    pinMode(led_4, OUTPUT);

    desligaLeds();
    rodar();
    rodar();
    rodar();

    attachInterrupt(digitalPinToInterrupt(dificuldade1_pin), dificuldade1_bt,
                    FALLING);
    attachInterrupt(digitalPinToInterrupt(dificuldade2_pin), dificuldade2_bt,
                    FALLING);
}

void loop() {
	while(!dificuldade) {
		digitalWrite(led_1, HIGH);
		digitalWrite(led_2, HIGH);
		digitalWrite(led_3, HIGH);
		digitalWrite(led_4, HIGH);
		delay(delay_padrao);
		digitalWrite(led_1, LOW);
		digitalWrite(led_2, LOW);
		digitalWrite(led_3, LOW);
		digitalWrite(led_4, LOW);
		delay(delay_padrao);
	}

    botao_atual = 0;
    gera_aleatorio();
    delay(500);
    liga_led_aleatorio();
    ta_jogando = true;

    while (botao_atual <= atual_leds) {
        if (digitalRead(bt_1_pin) == LOW) {
            bt1();
        }

        if (digitalRead(bt_2_pin) == LOW) {
            bt2();
        }

        if (digitalRead(bt_3_pin) == LOW) {
            bt3();
        }

        if (digitalRead(bt_4_pin) == LOW) {
            bt4();
        }
        if (perdeu) break;
    }

    if (botao_atual == atual_leds+1) {
        if (conferir_botoes_atuais()) {
            acertou = true;
        }
        // ta_jogando = false quando perder ou ganhar
        // acertou = true
        // perdeu = true
    }

    if (acertou && (atual_leds == max_leds-1)) {
        for (int i = 0; i < 5; i++) {
            rodar();
        }
        encerra_partida();
    }

    if (acertou) {
        acertou = false;
        Serial.println("Acertou!");
        delay(1500);
    }

    if (perdeu) {
        perdeu = false;
        Serial.println("Perdeu!");
        for (int i = 0; i < 5; i++) {
            digitalWrite(led_1, HIGH);
            digitalWrite(led_2, HIGH);
            digitalWrite(led_3, HIGH);
            digitalWrite(led_4, HIGH);
            delay(500);
            digitalWrite(led_1, LOW);
            digitalWrite(led_2, LOW);
            digitalWrite(led_3, LOW);
            digitalWrite(led_4, LOW);
            delay(500);
        }
        encerra_partida();
        delay(1500);
    }
}