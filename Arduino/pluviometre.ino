#include <ServoTimer2.h>
#include <Wire.h>
#include "RTClib.h"

// ----- Carte SD ----- //
// MOSI pin11, MISO pin12, SCK pin13, CS pin4, Alim 5V, Gnd

#include <SD.h>   // Contient déjà SPI.h
#include <VirtualWire.h> // On inclus la librairie VirtualWire
#include <VirtualWire_Config.h>

File fichierSD;    // Definition de fichierSD qui permet d'acceder
int interrupteur;
int pinCapot;
int pinRobinet; 
ServoTimer2 capot; 
ServoTimer2 robinet; 
int rainsensor;
int levelwater;
int result;
const int precisionLevel = 10; //on regarde les variations du capteur de niveau lorsque le niveau d'eau ne varie pas et c'est +ou- cette valeur
int levelmoyen = 0;
unsigned long debutDelai = 0;
int compteur = 0;
char Msg[30];// La chaine de caractère que nous allons envoyer au recepteur

RTC_PCF8523 rtc;


 #define SensorPin 2          // analog 2
unsigned long int avgValue;  //Stock la valeur moyenne de la réponse du capteur
float b;
int buf[10],temp;


void setup() {
  
  while (!Serial) {
    delay(1);  
  }
  
  Serial.begin(9600);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  pinCapot=8; // servo pin 8
  capot.attach(pinCapot); // on relie l'objet au pin de commande
  capot.write(544); // en microseconde. Le nombre de microsecondes correspond à la largeur de l'impulsion qui est envoyée au servomoteur
  pinRobinet=9; // robinet
  robinet.attach(pinRobinet);
  robinet.write(544);
  result=((levelwater*40)/700); // result est en mm
  SD.begin(4);          // Debut de la communication carte SD
  interrupteur = 2; // interrupteur pin 2
  pinMode(interrupteur, INPUT); //mode lecture pour la pin 2
  
  /* default min is 544, max is 2400
   *  c'est les valeurs en microsecondes pour les servos
   *  On peut modifier la librairie pour mettre tout en degres
   *  
   */
    // Initialise the IO and ISR
   vw_setup(2000); // Bits par sec
  vw_set_tx_pin(10);// mettre le pin Tx. Par défaut c'est la 12
 
} // FIN DU SETUP

void loop() {

rainsensor = analogRead(0);   //on choisit l'analog sur lequel on veut brancher 
 //Serial.println(rainsensor); //je lis la donnée brute direct dans le moniteur série  
levelwater = analogRead (1);// Lecture de la valeur du capteur
Serial.println (result);// Affichage de la hauteur d'eau en mm

if(rainsensor>700){
    capot.write(2200); // ouvir capot (millisconde)
 //delay(200); 

//--------------------- Ph metre--------------------------------//


for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    delay(10);
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 



 //--------------------- HORLOGE --------------------------------//

DateTime now = rtc.now();
    
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.println();
 
 //--------------------- Ecriture SD --------------------------------//   
    
    fichierSD = SD.open("pluvio_1_#.log", FILE_WRITE);   // nom du fichier. On peut mettre .txt si on veut un fichier texte
        fichierSD.println("DATE ="); // date
        fichierSD.print(now.day(), DEC);
        fichierSD.print('/');
        fichierSD.print(now.month(), DEC);
        fichierSD.print('/');
        fichierSD.print(now.year(), DEC);
        fichierSD.print(" ");
        fichierSD.print(now.hour(), DEC);
        fichierSD.print(':');
        fichierSD.println(now.minute(), DEC); 
        fichierSD.println("Hauteur d'eau =");         // hauteur d'eau 
        fichierSD.print(result);
        fichierSD.print("mm");
        fichierSD.println("Ph =");                    // Ph         
        fichierSD.print(phValue,2);
        fichierSD.println();   
        fichierSD.close();    // Fermeture du fichier necessaire a sa sauvegarde
        
  
  //---------------------Envoi des données par radio --------------------------------//


  sprintf(Msg, "%d,%d,%d", phValue, rtc, result);
 
 // Turn on a light to show transmitting
 // digitalWrite(ledPin, HIGH); 
 //LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);  
  delay(100); 
  vw_send((uint8_t *)Msg, strlen(Msg));
  vw_wait_tx(); // Wait until the whole message is gone
 // Turn off a light after transmission
 //  digitalWrite(ledPin, LOW); 
   delay(60000);
}

  //---------------------SINON --------------------------------//
/*
  levelmoyen change si levelwater est plus grand 
  que (levelmoyen + la précision) ou plus petite que (levelmoyen - la précision)
 */
else {
  //Si le niveau d'eau précédent est différent du nouveau niveau plus ou moins l'incertitude, alors modifier le niveau
  if( levelwater > levelmoyen+precisionLevel ) 
  { 
    levelmoyen = levelwater - precisionLevel;
    debutDelai = millis();
  } else
  if( levelwater < levelmoyen-precisionLevel ) 
  {
    levelmoyen = levelwater + precisionLevel;
    debutDelai = millis();
  }

// si le niveau ne varie pas au bout de 10 min on ferme le capot et on vidange
  if( millis()-debutDelai > 600000 ) 

  {
    capot.write(544);
    delay(3000);// on ferme le capot
    robinet.write(2200);
    delay(10000);
    robinet.write(544);
      
  }
    
}

//---------------------INTERRUPTEUR pour l'ouverture manuelle du capot --------------------------------//

boolean etatinterrup = digitalRead(interrupteur); //lecture de l'état du bouton
  
  if (etatinterrup==HIGH) //interrupteur fermé donc on ouvre le capot
  {
    capot.write(2200);
  }
  if (etatinterrup==LOW)//interrupteur ouvert donc on laisse le capot fermé
  {
    capot.write(544);
     
  }
  delay(100);

    
} // FIN DU LOOP



