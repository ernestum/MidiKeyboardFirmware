#include <Bounce.h>

int midi_channel = 1;
int midi_velocity = 95; // Controls how strong a note is played. This can also be changed via midi control change messages on controller 0.
int midi_random_velocity_variation = 5; // Controls how much the velocity is varied randomly with each new keystroke. This can also be changed via midi control change messages on controller 1.
int key_sensivity = 5; // Smaller values make the keys react more quickly
int deepest_note = 60; // 60 is a C. Here is a list of numbers for all notes http://www.tonalsoft.com/pub/news/pitch-bend.aspx This can also be changed via midi control change messages on controller 2.
// You can send all those midi control change messages (CC) using pizmidi: http://thepiz.org/plugins/?p=pizmidi I think it is a bitwig plugin.
class Key : public Bounce {
  public:
    Key(int pin) : Bounce(pin, key_sensivity) {}
};

Key keys [] = {0, 4, 3, 8, 2, 10, 5, 9, 6, 12, 1, 11, 19, 17, 18, 14, 16, 15, 22, 13, 20, 7, 21, 23};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 24; ++i)
    pinMode(i, INPUT_PULLUP);
}

void loop() {
  for (int i = 0; i < 24; ++i) {
    keys[i].update();
    if (keys[i].fallingEdge()) {
      int velocity = random(midi_velocity - midi_random_velocity_variation, midi_velocity + midi_random_velocity_variation);
      int note = deepest_note + i;
      usbMIDI.sendNoteOff(note, velocity, midi_channel);
      Serial.print(i); Serial.println(" off");
    }
    if (keys[i].risingEdge()) {
      int velocity = random(midi_velocity - midi_random_velocity_variation, midi_velocity + midi_random_velocity_variation);
      int note = deepest_note + i;
      usbMIDI.sendNoteOn(note, velocity, midi_channel);
      Serial.print(i); Serial.println(" on");
    }
  }
  while (usbMIDI.read()) {} // Reads incomming midi commands
}

void OnControlChange(byte channel, byte control, byte value) {
  if (channel == midi_channel) {
    if (control == 0)
      midi_velocity = value;
    if (control == 1)
      midi_random_velocity_variation = value;
    if (control == 2)
      deepest_note = value;
  }
}

