#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 41 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led azul

const int buttonPin = 18;  // o pin que vou utilizar para o botão
int botaoESTADO = 0;  // variavel para ler o estado do botão


//configurando o ldr

int pinLDR = 38; // pino do ldr
int pwm = 0;
int leituraLDR = 0; // leitura do ldr
int limiteluz = 1000; // limite de luz maximo para mudar o estado para modo normal




// aqui é o setup, aonde inicializamos as configurações inicias
// para o programa funcionar, como, por exempo, os modos de cada pino
void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_verde,OUTPUT);
  pinMode(led_vermelho,OUTPUT);
  pinMode(led_amarelo, OUTPUT);


  

  // Inicialização das entradas
  pinMode(buttonPin, INPUT); // atribuindo o  modo de input para o botão, ou seja, ele envia dados, não recebe

//indicando a configuração inicial de cada led
 
  digitalWrite(led_verde, LOW); // aqui diz a mesma coisa, só que para o led verde, ou seja, ele também incia desligado, no caso 'low'
  digitalWrite(led_vermelho, LOW); // aqui diz a mesma coisa, só que para o led vermelho, ou seja, ele também incia desligado, no caso 'low'

// aqui estou iniciando o monitor serial, com uma taxa de 9600

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

//aqui estamos iniciando a configuração do wifi
  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

// aqui está criando uma estrutura condicional que diz
// se o status do wifi for falha, ele fica imprimindo '.', indicando que a conexão não está conseguindo ser feita
  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100);
    Serial.print(".");
  }

  // aqui, caso saia da estrutura while, ou seja, o if de vefificação de falha no wifi não satisfeito. Nisso o wifi foi conectado
  //ai ele imprime que o wifi foi conectado
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  

  
 
  if(WiFi.status() == WL_CONNECTED){ // Se o ESP32 estiver conectado à Internet
    HTTPClient http;

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str());

    int httpResponseCode = http.GET(); // Código do Resultado da Requisição HTTP

    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      }
      http.end();
    }

  else {
    Serial.println("WiFi Desconectado");
  }
}

// função loop - fica executando a todo momento
void loop() {


  // Verifica estado do botão -- a variavel estado do botao passa a ter o valor real do estado atual do botão
  botaoESTADO = digitalRead(buttonPin);
 
 leituraLDR = digitalRead(pinLDR);
  char msg[30];
  sprintf(msg, "Leitura LDR: %d", leituraLDR);
  Serial.println(msg);
  //intervalo de leitura do sensor 
  //significativa
  pwm = (pwm = map(leituraLDR, 2045, 4095, 0, 255)) < 0? 0: pwm;
  delay(50); 


// fazendo a leitura da luminosidade
  if (leituraLDR < limiteluz){
    Serial.print("A luminosidade está baixa, ativando o modo noturno");

    //ligando o led amarelo

    digitalWrite(led_amarelo, HIGH);

    //esperando 1 seg
    delay(1000);

    //desligando ele
    digitalWrite(led_amarelo, LOW);
  }


// se a leitura do ldr for maior que o limite de luz, ele ativa o modo normal
  else if (leituraLDR > limiteluz){
    Serial.print(" A luminosidade está alta, ativando modo normal");


//liga o led verde
    digitalWrite(led_verde, HIGH);

    //espera 3 seg
    delay(3000);
    //desliga o led verde
    digitalWrite(led_verde, LOW);
    //liga o amarelo
    digitalWrite(led_amarelo, HIGH);
    //espera 2 seg
    delay(2000);

    //desliga o amarelo
    digitalWrite(led_amarelo, LOW);

    //liga o vermelho
    digitalWrite(led_vermelho, HIGH);


    //condicional para, se o botão for apertado durante a espera do led vermelho, ele espera só 1 segundo antes de desligar, se não for apertado, espera 5

      if(botaoESTADO == HIGH){
        delay(1000);
        digitalWrite(led_vermelho, LOW);
      }
      else{
          delay(5000);
          digitalWrite(led_vermelho, LOW);
      }

  }


   if (botaoESTADO == HIGH) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão não pressionado!");
  }

}