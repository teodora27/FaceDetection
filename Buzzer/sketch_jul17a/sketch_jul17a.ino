const int buzzerPin = 14; // GPIO pin connected to the buzzer I/O pin
bool ok;

void setup() {
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as an output
  ok=true;
}

void loop() {
  // Test sound for correct and incorrect actions
  
  //aici trebuie sa primim date de la camera

  if(ok)
  {
    playCorrectSound();
    delay(2000);
  } // 2-second pause between sounds for testing
}

void playCorrectSound() {
  tone(buzzerPin, 1000, 30); // First tone, 1000 Hz for 30 ms
  noTone(buzzerPin);
}
