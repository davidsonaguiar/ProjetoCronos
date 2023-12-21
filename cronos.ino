// Importando a biblioteca para utilizaição do LCD
#include <LiquidCrystal_I2C.h>

// Struct para especificar os dados necessários para 
// o botão do aluno
// Serve como modelo
struct Button {
  char name;
  int input;
  int output;
};

// Lista com os dados dos botões
// Nome - Entrada - Saída
Button Buttons[5] = {
  {'A', 3, 9},
  {'B', 4, 10},
  {'C', 5, 11},
  {'D', 6, 12},  
  {'E', 7, 13}
};

// Lista para salvar a ordem de quem apertou o botão primeiro
char position[5] = {' ', ' ', ' ', ' ', ' '};

// Estado para alterar as fases do jogo
bool state = true;

// status para fazer o led piscar
unsigned long previousMillisLed = 0; 

// Estado do led
bool ledState = false;

unsigned long previousMillisButton = 0; 

// Config do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);


// SETUP and LOOP

void setup() {
  
  // Botao do professor
  pinMode(8, INPUT_PULLUP);
  
  // Botoes dos alunos
  for (int i = 0; i < 5; i++) {
  	io(Buttons[i].input, Buttons[i].output);
  };
  
  // Config Led para mensagem inicial
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Cronos 4.0");
  lcd.setCursor(1, 1);
  lcd.print("Aperte o Botao");
  
  Serial.begin(9600);
}

void loop() {
  
  // Se estado for verdadeiro o led liga
  if(state) {
  	onLed();  
  }
  
  // Evento para verificar click no botao do aluno
  if(state) {
    for(int i = 0; i < 5; i++) {
      if(digitalRead(Buttons[i].input) == HIGH) {
        order(Buttons[i].name);
        printOrder();
      }
    }
  }
  
  // Condicao para verificar se houve clique no botao do professor
  if(digitalRead(8) == HIGH) {
    // Chama a funcao responsavel por alterar o estado
  	toggleState();
  }

  // Se o estado for falso faz com que o led pisque de for alternada
  if(!state) {
    blinkLed();
  } else {
  	offAllLeds();
  }
}


// FUNCTIONS

// funcao para definir as entradas e saidas de cada controle
void io(int input, int output) {
  pinMode(input, INPUT);  
  pinMode(output, OUTPUT);
}

// Funcao que pega a ordem do aperto no botao dos alunos.
void order(char name) {
  for (int i = 0; i < 5; i++) {
    if (position[i] == name) {
      break;
    } else {
      if (position[i] == ' ') {
        position[i] = name;
        break;
      }
    }
  }
  
  for (int i = 0; i < 5; i++) {
    Serial.println(position[i]);
  }
}	

// Funcao para ligar os leds
void onLed() {
  for(int i = 0; i < 5; i++) {
    if(position[0] == Buttons[i].name) {
      digitalWrite(Buttons[i].output, HIGH);
    }
  }
}

// Funcao responsavel por fazer o led piscar
void blinkLed() {
  const unsigned long interval = 250;  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisLed >= interval) {
    previousMillisLed = currentMillis;  

    ledState = !ledState;

    for (int i = 0; i < 5; i++) {
      digitalWrite(Buttons[i].output, ledState);
    }
  }
}

// Desliga os leds
void offAllLeds() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(Buttons[i].output, LOW);
  }
}

// Mostra no display dos alunos que apertaram o botao
void printOrder() {
  if(state) {
  	lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ordem:          ");
    lcd.setCursor(0, 1);
    for(int i = 0; i < 5; i++) {
      lcd.print(position[i]);
      if(i < 3) {
      	lcd.print(" - ");
      }
    }
  }
}

// Funcao para alterar o estado
void toggleState() {
  const unsigned long interval = 300;  
  unsigned long currentMillis = millis();
    
  if(currentMillis - previousMillisButton >= interval) {
  	previousMillisButton = currentMillis;
    
    state = !state;
  }

  if(!state) {
    lcd.setCursor(0, 0);
    lcd.print("Fase da         ");
    lcd.setCursor(0, 1);
    lcd.print("Pergunta!       ");

    for(int i = 0; i < 5; i++) {
      position[i] = ' ';
      digitalWrite(Buttons[i].output, LOW);
    }

  } else {
    lcd.setCursor(0, 0);
    lcd.print("Fase da         ");
    lcd.setCursor(0, 1);
    lcd.print("resposta!       ");
  }
}
