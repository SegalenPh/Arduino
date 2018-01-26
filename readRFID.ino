#include <SPI.h>     //On inclue les librairies.
#include <RFID.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RCSwitch.h>

#define SS_PIN 10                   // On définit que le SDA est connecté au pin 10
#define RST_PIN 9                   // On définit que le RST est connécté au pin 9
#define Buzzer 3                    // On définit que le buzzer est connecté au pin 3
#define Alarm 6                     // On définit que l'alarme est connectée au pin 3
LiquidCrystal_I2C lcd(0x3f, 16, 2); // On définit l'écran lcd
RCSwitch mySwitch = RCSwitch();     // On définit le recepteur

RFID rfid(SS_PIN, RST_PIN);

int i = 0, j = 0, k = 0; // Variables de boucle de la sirène
int idcard;         // On crée une variable idcard
String str;         // On crée une variable string str
String read_string; // On crée une variable string read_string
String content = "";// On crée une variable string content avec un chaine de caractères vide
char character;     // On crée une variable char character
bool is_clear = 0;  // On crée une  variable booléenne is_clear à 0

//**************** FONCTION SETUP = Code d'initialisation *****
// La fonction setup() est exécutée en premier et 1 seule fois, au démarrage du programme
void setup() {
  pinMode(Alarm, OUTPUT);   // On met le pin de sortie
  pinMode(Buzzer, OUTPUT);  // On met le pin de sortie
  digitalWrite(Buzzer, LOW); // On met le buzzer en off
  mySwitch.enableReceive(0); // On active la réception du rc433
  Serial.begin(9600);    // On définit la vitesse de transmission
  SPI.begin();           //
  rfid.init();           // Initialise le RFid
  lcd.begin();           // Debut de l'affichage de l'écran lcd
  lcd.backlight();
  lcd.print("Scannez carte"); // Affichage à l'écran lcd
}

//*************** FONCTION LOOP = Boucle sans fin = coeur du programme *************
void loop() {

  buzzer(5,50);

  delay(1000);  // Delai 1 sec

}

void printlcd() { // Fonction printlcd
  if (!is_clear)   // Si is_clear différent de 1
  {
    lcd.clear();    // On efface sur l'écran LCD
    lcd.print("Scannez carte"); // Affiche "Scannez carte" sur l'écran LCD
    is_clear = 1;   // On passe is_clear à 1
  }

  while (Serial.available()) {  // Tant que le port serie disponible
    character = Serial.read();  // Lit les requestes envoyé par le raspberry

    content.concat(character); // Concatène character avec content

    delay(3);
  }

  if (content != "") {  // Si content différent de ""
    lcd.clear();        // Efface l'écran LCD
    lcd.print(content); // Affiche le content à l'écran
    if (content == "Access denied") {  // Si l'écran affiche "Acces denied", lancer buzzer et l'alarme
      delay(100);
      buzzer(40, 25);
      delay(5000);
      siren(3);
    }
    if (content == "Alarme porte") { // Si l'écran affiche "Alarme porte", lancer la sirène

      delay(10000);
      siren(3);


    }
    is_clear = 0;
    delay(1000);
    content = "";
  }
}

void read_card() {  // Fonction read_card
  if (rfid.isCard()) {  // Si il y a un carte
    if (rfid.readCardSerial()) {  // On lit la carte
      lcd.clear();      // On efface l'écran
      idcard = rfid.serNum[4];  // On attribut le 4eme nombre comme idcard
      str = "SCAN " + String(idcard);
      buzzer(2, 50);    // Active le buzzer pour confirmer la lecture de la carte
      Serial.println(str);  // On envoie au raspberry l'idcard
    }
    rfid.halt();
  }
}
void siren(int loop_s) {  // Fonction sirene


  for (int y = 0; y < loop_s; y++){ // Répéte la boucle jusqu'a i égale à la valeur de loop_s
    for (i = 500; i < 1000; i++) {
      tone(Alarm, i);
      delay(1);
    }




  for (i = 1000; i > 500; i--) {
    tone(Alarm, i);
    delay(1);
  }
}
noTone(Alarm);  // Fin de l'alarme
}

void RCswitch() {  //Fonction RCswitch
  if (mySwitch.available()) {  // Si le récepterur est activé rc433

    int value = mySwitch.getReceivedValue();  //  On définie value récupérant l'id du détecteur d'ouverture de porte
    if (value == 0) {
      Serial.print("Unknown encoding");
    } else {
      Serial.println( mySwitch.getReceivedValue() );  // Sinon envoyer au raspberry l'id du détecteur

    }

    mySwitch.resetAvailable();
  }
}


void buzzer(int loop_b, int delay_b) {  //Fonction buzzer, int loop_b le nombre de répétition, delay_b le delai du buzzer
  for (int i = 0; i < loop_b; i++)  // Répéte la boucle jusqu'a i égale à la valeur de loop_b
  {
    digitalWrite (Buzzer, HIGH) ;// Buzzer On
    delay (delay_b) ;
    digitalWrite (Buzzer, LOW) ;// Buzzer Off
    delay (delay_b) ;
  }

}

