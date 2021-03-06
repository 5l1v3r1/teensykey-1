/* TeensyKey

   TeensyKey receives an int from a dedicated program
   running on a Linux or Mac machine and returns a
   password belonging to that int.

   You must select Raw HID from the "Tools > USB Type" menu
*/

// Teensy 3.0 has the LED on pin 13
const int ledPin = 13;

void setup() {
  Serial.begin(9600);
  Serial.println(F("RawHID Example"));

  pinMode(ledPin, OUTPUT);

  blink(3);
}

// RawHID packets are always 64 bytes
byte buffer[64];
unsigned int waitTimeout = 10000;
unsigned int packetCount = 0;

const char passwords[][64] = {
"Password1",
"1234",
"TOP SECRET",
"Samsung1"
};

void loop() {
  int n, password_index;
  int password_char_index = 0;

  Serial.println(F("Waiting for password index ..."));
  blink(1);

  n = RawHID.recv(buffer, waitTimeout);
  if (n > 0) {

    // the computer sent a message.
    Serial.print(F("Received packet, first byte: "));
    Serial.println((int)buffer[0]);

    password_index = (int)buffer[0];

    while(passwords[password_index][password_char_index] != 0){
      buffer[password_char_index + 1] = passwords[password_index][password_char_index];
      Serial.print("Password char is: ");
      Serial.println((char) buffer[password_char_index + 1]);
      password_char_index += 1;
    }

    buffer[password_char_index + 1] = 0;

    // put the length of the password, including \0 in the first byte
    buffer[0] = password_char_index;

    // fill the rest with zeros
    for (int i = password_char_index + 1; i < 64 - password_char_index; i++) {
      buffer[i] = 0;
    }

    // actually send the packet
    n = RawHID.send(buffer, 200);
    if (n > 0) {
      Serial.println(F("Transmit password"));
    } else {
      Serial.println(F("Unable to transmit packet"));
    }

    blink(2);
  }
}

void blink(int times){
  int i;
  for(i = 0; i < times; i++){
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
