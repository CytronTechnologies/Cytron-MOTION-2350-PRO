/*
DESCRIPTION:
This example code will play the tones using buzzer, 
press the buttons on MOTION 2350 Pro to play the tones.

AUTHOR  : Cytron Technologies Sdn Bhd
WEBSITE  : www.cytron.io
EMAIL    : support@cytron.io
*/

const int buzzerPin = 22;
const int btn1 = 20;
const int btn2 = 21;

int melody_note[10] = {659, 659, 0, 659, 0, 523, 659, 0, 784, 0};
int melody_duration[10] = {150, 150, 150, 150, 150, 150, 150, 150, 200, 150};

void setup(){
  // Initialize buzzer pin as output
  pinMode(buzzerPin, OUTPUT);

  // Initialize buttons
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  // Play melody during start up
  play_melody(buzzerPin);
  
}

void loop(){
  // Check button 1 (GP20) is pressed
  if (!digitalRead(btn1)) {
    // Play tones
    tone(buzzerPin,262,100);
    delay(100);
    tone(buzzerPin,659,100);
    delay(100);
    tone(buzzerPin,784,100);
    delay(100);
    noTone(buzzerPin);
  }

  // Check button 1 (GP21) is pressed
  if (!digitalRead(btn2)) {
    // Play tones
    tone(buzzerPin,784,100);
    delay(100);
    tone(buzzerPin,659,100);
    delay(100);
    tone(buzzerPin,262,100);
    delay(100);
    noTone(buzzerPin);
  }

}

void play_melody(int pin){
  for(int i=0; i<10; i++){
    if(melody_note[i] == 0){
      noTone(pin);
    } 
    else{
     tone(pin, melody_note[i], 100);
    }
    delay(int(melody_duration[i]));
  }
}

