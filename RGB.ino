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
  nastavBarvu();
  delay(20);
}
 
void nastavBarvu() {
  analogWrite(CERVENA, barva.cervena);
  analogWrite(ZELENA, barva.zelena);
  analogWrite(MODRA, barva.modra);
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
  barva.cervena = prevedZnakNaCislo(pole[1]) * 16;
  barva.cervena += prevedZnakNaCislo(pole[2]);
  
  barva.zelena = prevedZnakNaCislo(pole[3]) * 16;
  barva.zelena += prevedZnakNaCislo(pole[4]);
  
  barva.modra = prevedZnakNaCislo(pole[5]) * 16;
  barva.modra += prevedZnakNaCislo(pole[6]);
}

int prevedZnakNaCislo(char znak){
  int prevedeneCislo = 0;
  if (znak >= '0' && znak <= '9'){
    prevedeneCislo - znak - '0';
  }else if (znak >= 'a' && znak <= 'f'){
    prevedeneCislo = 10 + znak - 'a';
  }else{
    prevedeneCislo = 10 + znak - 'A';
  }
}
