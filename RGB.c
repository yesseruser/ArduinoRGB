#define ZELENA 11
#define MODRA 10
#define CERVENA 9
#define MAX_PWM 256
#define DELKA_RETEZCE 7
#define RYCHLOST_PRENOSU 9600
#define DELKA_SPRAVNYCH_ZNAKU 22
const char spravneZnaky[] = "0123456789abcdefABCDEF";
typedef struct {
  uint8_t cervena;
  uint8_t zelena;
  uint8_t modra;
}
BarvaLed;
void setup() {
  // put your setup code here, to run once:
  pinMode(ZELENA, OUTPUT);
  pinMode(MODRA, OUTPUT);
  pinMode(CERVENA, OUTPUT);
  Serial.begin(RYCHLOST_PRENOSU);
}
void loop() {
  // put your main code here, to run repeatedly:
  char nactenePole[DELKA_RETEZCE];
  if(prislaData()){
    nactiPole(&nactenePole[0]);
    vypisPole(&nactenePole[0]);
  }
  nastavBarvu(5, 153, 182);
  delay(20);
}
 
void nastavBarvu(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(CERVENA, red);
  analogWrite(ZELENA, green);
  analogWrite(MODRA, blue);
}

bool prislaData() {
  if (Serial.available() == DELKA_RETEZCE) {
    return true;
  }
  while (Serial.available() != 0) {
    Serial.read();
  }
  return false;
}

bool nactiPole(char * pole) {
  for (uint8_t i = 0; i < DELKA_RETEZCE; i++) {
    *pole = Serial.read();
    pole++;
  }
  pole -= DELKA_RETEZCE;
  if (pole[0] == '#'){
    for (uint8_t i = 1; i < DELKA_RETEZCE; i++){
      bool znakNalezen = false;
      for (uint8_t j = 0; j < DELKA_SPRAVNYCH_ZNAKU; j++){
        if(pole[i] == spravneZnaky[j]){
          znakNalezen = true;
          break;
        }
      }
      if(!znakNalezen){
        Serial.println("Nespravny vstup");
        return false;
      }
    }
  }else{
    Serial.println("Nespravny vstup");
    return false;
  }
  return true;
}

void vypisPole(char *pole) {
  for(uint8_t i = 0; i < DELKA_RETEZCE; i++){
    Serial.print(*pole);
    Serial.print(" = ");
    int x = pole;
    Serial.println(x, DEC);
    pole++;
  }
}
void dekodujZpravu(char * pole){
  
}
