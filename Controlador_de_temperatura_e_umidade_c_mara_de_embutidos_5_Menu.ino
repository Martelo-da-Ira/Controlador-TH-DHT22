//Programa: Controlador de temperatura e umidade

// ========================================================================================================
// --- Bibliotecas Auxiliares ---

#include <Wire.h>

#include <DHT.h>                                                   //Inclui a bilbioteca DHT

#include <LiquidCrystal_I2C.h>                                     //Inclui a biblioteca LiquidCrystal



// ========================================================================================================
// --- Mapeamento de Hardware ---

//Sensor DHT22
#define DHTPIN 12                                                  //Define o pino do DHT no Arduino
#define DHTTYPE DHT22                                              //Define modelo do DHT

//Definicoes do sensor : pino, tipo
DHT dht(DHTPIN, DHTTYPE);

//botoes
int BotaoAumentar = 3;
int BotaoDiminuir = 4;  
int BotaoMenu = 5;  

//Relés
int ReleTemp = 6;  
int ReleUmid = 7;


// ========================================================================================================
// --- Declaração de Objetos ---

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);      //Define os pinos do LCD (RS, E, D4, D5, D6, D7)


// ========================================================================================================
// --- Variavies Auxiliares ---

//Variável de temperatura
float temperatura = 13.0;

float TempHisterese = 1.0;


//Variável de umidade
float umidade = 75.0;

float UmidHisterese = 2.0;


//tela atual
int tela = 0;
unsigned long idleTime = 0;
unsigned long idleTimeTela0 = 0;
float t;
float h;

// ========================================================================================================
// --- Configurações Iniciais ---
void setup() 
{//Start Setup

 //Define os pinos como entrada ou saída
 pinMode(BotaoAumentar, INPUT_PULLUP);
 pinMode(BotaoDiminuir, INPUT_PULLUP);
 pinMode(BotaoMenu, INPUT_PULLUP);
 pinMode(ReleTemp, OUTPUT);
 pinMode(ReleUmid, OUTPUT);


 //Define estado inicial das portas
 digitalWrite(ReleTemp, HIGH);
 digitalWrite(ReleUmid, HIGH); 


 //Configurações do Display
 //Inicializa o display LCD 16x2
 lcd.begin (16, 2);
 lcd.print("Aguardando dados...");
 idleTime = millis();  

 //Iniclaiza o sensor DHT  
 dht.begin();

   
}//end setup

// ========================================================================================================
// --- Loop Infinito ---

void loop() 
{//Start loop

  t = dht.readTemperature();      //Leitura do valor de temperatura
  h = dht.readHumidity();         //Leitura do valor de umidade


 //reseta tela pra inicial
   if(digitalRead(BotaoMenu)==1 && digitalRead(BotaoAumentar)==1 && digitalRead(BotaoDiminuir)==1 && tela!=1 && tela!=2 && tela!=3 && tela!=4)
    {
     tela0();  
    }
    
  //Verifica se os botoes de telas e ajustes foram pressioandos
  if(digitalRead(BotaoMenu)==0 && digitalRead(BotaoAumentar)==1 && digitalRead(BotaoDiminuir)==1)
    {
       mudaTela();
       delay(500); 
     //Serial.println("Mudou tela");
     }
  
  if(digitalRead(BotaoMenu)==1 && digitalRead(BotaoAumentar)==0 && digitalRead(BotaoDiminuir)==1)
    {
       aumentaValorAtual();
       delay(300); 
     //Serial.println("Aumentou valor");
    }
  
  if(digitalRead(BotaoMenu)==1 && digitalRead(BotaoAumentar)==1 && digitalRead(BotaoDiminuir)==0)
    {
       diminuiValorAtual();
       delay(300); 
     //Serial.println("Diminuiu valor");
    }

   //reseta tela pra inicial
   if(millis()-idleTime>=10000 && idleTime!=0 && tela!=0)
    {
       tela0();
    }//END verifica se os botoes de telas e ajustes foram pressionados



// Analisa se a temperatura está igual a que foi setada e aciono o rele caso não seja
  if (t >= temperatura + TempHisterese) 
   {
    // Liga a geladeira
    digitalWrite(ReleTemp, LOW);
    delay(500);
   }
   
  if (t <= temperatura - TempHisterese)
   {
   // Desliga a geladeira
    digitalWrite(ReleTemp, HIGH);
    delay(500);
   }
     
     
 // Analisa se a umidade está igual a que foi setada e aciono o rele caso não seja
  if (h <= umidade - UmidHisterese) 
   {
    // Liga o umidificador
    digitalWrite(ReleUmid, LOW);
    delay(500);
   }
   
  if (h >= umidade + UmidHisterese) 
   {
   // Desliga o umidificador
    digitalWrite(ReleUmid, HIGH);
    delay(500);
   }


  /*if(isnan(t)) 
  {
   
    software_Reset();
    
  }*/


}//End loop



// ========================================================================================================
// --- Loop Reset ---


void software_Reset() 
  {
    asm volatile (" jmp 0");
  }




// ========================================================================================================
// --- Loop acréscimo das variáveis ---

//Aumentando o valor baseado na tela atual
void aumentaValorAtual ()
 {
  switch(tela)
   {
    case 1:
      temperatura+=0.1;   
      if(temperatura>100) temperatura=100;
      tela1();
      break;

    case 2:
      TempHisterese+=0.1;   
      if(TempHisterese>5) TempHisterese=5;
      tela2();
      break;
      
    case 3:
      umidade+=1;   
      if(umidade>100) umidade=100;
      tela3();
      break;
      
    case 4:
      UmidHisterese+=1;   
      if(UmidHisterese>20) UmidHisterese=20;
      tela4();
      break;
    
  }  
  idleTime = millis();
  
  if(tela!=0);
  
    
  }//END aumentando o valor baseado na tela atual


// ========================================================================================================
// --- Loop decréscimo das variáveis ---
//diminuindo o valor baseado na tela atual 
void diminuiValorAtual ()
 {
  switch(tela)
   {
    case 1:
      temperatura-=0.1;
      if(temperatura<-5) temperatura=-5;
      tela1();
      break;

    case 2:
      TempHisterese-=0.1;
      if(TempHisterese<0) TempHisterese=0;
      tela2();
      break;
      
    case 3:
      umidade-=1;
      if(umidade<0) umidade=0;
      tela3();
      break;
      
    case 4:
      UmidHisterese-=1;
      if(UmidHisterese<0) UmidHisterese=0;
      tela4();
      break;
            
   }
   idleTime = millis();
   
   if(tela!=0);
   
  } //END dominuindo o valor baseado na tela atual


// ========================================================================================================
// --- Loop mudança de tela ---

//mudando a tela
void mudaTela()
 {
  tela++;
  
  if(tela>4) tela=0;
     switch(tela){
           
           case 1:
             tela1();
             break;
           
           case 2:
             tela2();
             break;
    
           case 3:
             tela3();
             break;

           case 4:
             tela4();
             break;
    
  }
  
  idleTime = millis();
  
}//END mudando a tela



// ========================================================================================================
// --- Loop Telas Menu ---


void tela1()
  {
  tela=1;
  lcd.clear();
  lcd.setCursor(0,0); // Column, line
  lcd.print("Temperatura:");
  lcd.setCursor(0,1);
  lcd.print(temperatura);
  lcd.setCursor(6,1);                  //Posiciona o símbolo de grau formado pelo array
  lcd.write(223);                   //Mostra o símbolo de grau 
  lcd.print("C");                       //Mostra o símbolo da unidade Celsius
  }


void tela2()
  {
  tela=2;
  lcd.clear();
  lcd.setCursor(0,0); // Column, line
  lcd.print("Temp Histerese:");
  lcd.setCursor(0,1);
  lcd.print(TempHisterese);
  lcd.setCursor(6,1);                  
  lcd.write(223);                       //Mostra o símbolo de grau 
  lcd.print("C");                       //Mostra o símbolo da unidade Celsius
  }
  
  
void tela3()
  {
  tela=3;
  lcd.clear();
  lcd.setCursor(0,0); // Column, line
  lcd.print("Umidade:");
  lcd.setCursor(0,1);
  lcd.print(umidade);
  lcd.setCursor(6,1);                   //Posiciona o símbolo de grau formado pelo array
  lcd.print("%");                       //Mostra o símbolo da unidade Celsius
  }


void tela4()
  {
  tela=4;
  lcd.clear();
  lcd.setCursor(0,0); // Column, line
  lcd.print("Umid Histerese:");
  lcd.setCursor(0,1);
  lcd.print(UmidHisterese);
  lcd.setCursor(6,1);                   //Posiciona o símbolo de grau formado pelo array
  lcd.print("%");                       //Mostra o símbolo da unidade Celsius
  }

  
void tela0(){
    if(tela!=0 || millis()-idleTimeTela0>=2000){
    tela=0;
     lcd.clear();
     lcd.setCursor(0,0);                   //Posiciona o cursor no canto superior esquerdo do display
     lcd.print("Temp: ");                  //
     lcd.print(t);                         //Inclui a informação de temperatura aferida pelo sensor
     lcd.write(223);                       //Mostra o símbolo de grau 
     lcd.print("C");                       //Mostra o símbolo da unidade Celsius
     
     lcd.setCursor(0,1);                   //Posiciona o cursor no canto inferior esquerdo do display
     lcd.print("Umid: ");                  //
     lcd.print(h);  
     lcd.setCursor(11,1);                  //Inclui a informação de umidade aferida pelo sensor
     lcd.print("%");  
     idleTimeTela0 = millis();            //Mostra o símbolo de porcentagem de umidade
    }
  }
