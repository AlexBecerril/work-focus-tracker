const int leds[4] = {11, 10, 9, 8};
const int buttons[4] = {5, 4, 3, 2};
const int events[4] = {1, 2, 3, 4};
const int buzzer = 7;
const int p_led = 12;
int previousState[4];

unsigned long event3Time = 0;
unsigned long lastToggleBuzzer = 0;

bool timerActive = false;
bool buzzerActive = false;
bool buzzerState = false;

const unsigned long ALERT_TIME = 600000;      // 10 minutes
const unsigned long BUZZER_INTERVAL = 500;     // 0.5 seconds

int lastEvent = -1;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize buzzer
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  // Initialize buttons and leds
  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
    previousState[i] = HIGH;
  }
  pinMode(p_led, OUTPUT);
  digitalWrite(p_led, LOW);
}

void loop() {
  digitalWrite(p_led, HIGH); 

  if (timerActive && !buzzerActive) {
    if (millis() - event3Time >= ALERT_TIME) {
      buzzerActive = true;
      lastToggleBuzzer = millis();
    }
  }

  if (buzzerActive) {
    if (millis() - lastToggleBuzzer >= BUZZER_INTERVAL) {
      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState ? HIGH : LOW);
      lastToggleBuzzer = millis();
    }
  }
  
  for (int i = 0; i < 4; i++) {
    int estadoActual = digitalRead(buttons[i]);

    // Detects pressed button (HIGH -> LOW)
    if (previousState[i] == HIGH && estadoActual == LOW) {

      // Event log
      if(events[i] != lastEvent){
        Serial.println(events[i]);
        lastEvent = events[i];
      }

      //Event 1
      if(i==0){
        digitalWrite(leds[0], HIGH);
      }
      //Event 2
      if(i==1){
        digitalWrite(leds[1], HIGH);
        digitalWrite(leds[2], LOW);
        //------------------------
        timerActive = false;
        buzzerActive = false;
        buzzerState = false;
        digitalWrite(buzzer, LOW);
        //------------------------
      }
      //Event 3
      if(i==2){
        digitalWrite(leds[2], HIGH);
        digitalWrite(leds[1], LOW);
        //------------------------
        event3Time = millis();
        timerActive = true;
        buzzerActive = false;
        buzzerState = false;
        digitalWrite(buzzer, LOW);
        //------------------------
      }
      //Event 4
      if(i==3){
        digitalWrite(leds[3], HIGH);
        digitalWrite(leds[0], LOW);
        digitalWrite(leds[1], LOW);
        digitalWrite(leds[2], LOW);
        //------------------------
        timerActive = false;
        buzzerActive = false;
        buzzerState = false;
        digitalWrite(buzzer, LOW);
        //------------------------
        delay(2000);
        digitalWrite(leds[3], LOW);
      }
    }
    previousState[i] = estadoActual;
  }
}
