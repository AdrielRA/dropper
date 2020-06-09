#include <Wire.h>              // Contém funções necessárias para usar comunicação I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para lidar com display usando módulo I2C

#define LDR       A0   // Pino conectado ao LDR
#define DISPLAY_  0x27 // Endereço I2C do Display LCD
#define COLUMNS   16   // Número de colunas do display
#define LINES     2    // Número de linhas do display
#define SENSE     5   // Sensibilidade do LDR para + ou para -
#define DRIPPED   950  // Valor indicativo que uma gota pingou
#define INTERVAL  1000 // Duração intervalo de contagem

unsigned long  time_now, last_time = 0; // Usadas para contar o tempo
int val_LDR = 0, dropper = 0;           // Valor lido pelo LDR e contador 

LiquidCrystal_I2C lcd(DISPLAY_, COLUMNS, LINES); // Configuração display usando I2C

void setup(){
  pinMode(LDR, INPUT); // Definição peno de entrada LDR
  lcd.init();          // Inicião comunicação com display
  lcd.backlight();     // Liga luz de fundo do display
  lcd.clear();         // Limpa tela do display
}

void loop(){
  time_now = millis();                               // Pega o tempo atual
  val_LDR = analogRead(LDR);                         // Lê o valor do LDR
  if(drop(val_LDR)){ dropper++; }                    // Contabiliza se identificar uma gota
  
  if(elapsed_time(time_now))                         // Condição caso um intervalo de tempo tenha se passado
  {
    lcd.setCursor(0,0);                              // Posiciona cursor no display
    lcd.print("Gotas: " + String(dropper) + "/s");   // Escreve última contagem
    dropper = 0;                                     // Zera o contador de gotas
  }  
  delay(120);                                        // Espera um intervalo
}

bool elapsed_time(unsigned long time){               // Função calcula e verifica tempo decorrido
  bool time_passed = (time - last_time) >= INTERVAL; // Verifica se o tempo desde última verificação é maior que o intervalo desejado
  if(time_passed) last_time = time;                  // Se sim, atualiza o valor do último tempo contabilizado
  return time_passed;                                // Retorna verdadeiro toda vez que um intervalo é passado
}

bool drop(int val){                    // Função que verifica se uma gota pingou
  return val >= (DRIPPED - SENSE) &&   // Verifica se valor do LDR é maior ou menor que o valor indicativo de que uma gota pingou
         val <= (DRIPPED + SENSE);     // Levando em conta a sensibilidade como margem de erro para + ou para -
}
