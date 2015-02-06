

// Variables will change:
bool ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
bool lastButtonState = LOW;   // the previous reading from the input pi

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin

int estado = 48;
int estadoAnterior = 48;



// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


// HSV fade/bounce for Arduino - scruss.com - 2010/09/12
// Note that there's some legacy code left in here which seems to do nothing
// but should do no harm ...

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int sensorMax = 0;
int sensorMin = 1023;
int threshold;
// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int electret = 0;  // the amplifier output is connected to analog pin 0

// don't futz with these, illicit sums later
#define RED       9 // pin for red LED
#define GREEN    10 // pin for green - never explicitly referenced
#define BLUE     11 // pin for blue - never explicitly referenced
#define SIZE    255
#define DELAY    5
#define HUE_MAX  6.0
#define HUE_DELTA 0.01
#define MEDIA_MAX 20
long deltas[3] = {
  5, 6, 7
};
long rgb[3];
long rgbval;
// for reasons unknown, if value !=0, the LED doesn't light. Hmm ...
// and saturation seems to be inverted
float hue = 0.0, saturation = 1.0, value = 0.5;

/*
chosen LED SparkFun sku: COM-09264
 has Max Luminosity (RGB): (2800, 6500, 1200)mcd
 so we normalize them all to 1200 mcd -
 R  1200/2800  =  0.428571428571429   =   109/256
 G  1200/6500  =  0.184615384615385   =    47/256
 B  1200/1200  =  1.0                 =   256/256
 */
long bright[3] = {
  109, 100, 256
};

bool Sensor = true;
bool input =  true;
int entrada = 0;
long k, temp_value;
int media = 0;



void setup () {
  randomSeed(analogRead(4));
  for (k = 0; k < 3; k++) {
    pinMode(RED + k, OUTPUT);
    rgb[k] = 0;
    analogWrite(RED + k, rgb[k] * bright[k] / 256);
    if (random(100) > 50) {
      deltas[k] = -1 * deltas[k]; // randomize direction
    }
  }

  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
  pinMode(13, OUTPUT);
  callib();
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println(Sensor ? "Modo Trans" : "Modo Sensivel");
  Serial.println(input ? "Sensor Mic" : "Entrada audio");
  Serial.print("Audio MAX: ");
  Serial.println(sensorMax);

}

void loop() {

  readBut();
  if (estado != estadoAnterior) atualizaEstado();

  if (Serial.available() > 0) {
    // get incoming byte:

  }
  if (Sensor) {
    sensorReading = analogRead(entrada);
    media = (sensorReading - threshold + media) / 2;
    if ((sensorReading >= threshold)) {
      Serial.println(sensorReading - threshold); //Will send only positive and absolute values of waveform
      hue = 5 * (sensorReading - threshold) * HUE_DELTA + hue + HUE_DELTA;
      value = 1.0; saturation =  1.0;

    }
  } else  {
    digitalWrite(13, LOW);
    value = 0.2; saturation = 1.0;
  } hue += HUE_DELTA;

  if (hue > HUE_MAX) {
    hue = 0.0;
  }
  if (media > 2 * MEDIA_MAX) {
    callib();

  }
  value -= 0.005;
  if (value > 0.2) {
    value = 0.2;
  }
  atualizaLEDS(hue, saturation, value);

  delay(DELAY);
}

void atualizaLEDS(float h, float s, float v) {

  rgbval = HSV_to_RGB(hue, saturation, value);
  rgb[0] = (rgbval & 0x00FF0000) >> 16; // there must be better ways
  rgb[1] = (rgbval & 0x0000FF00) >> 8;
  rgb[2] = rgbval & 0x000000FF;
  for (k = 0; k < 3; k++) { // for all three colours
    analogWrite(RED + k, rgb[k] * bright[k] / 256);
  }
}

long HSV_to_RGB( float h, float s, float v ) {
  /* modified from Alvy Ray Smith's site: http://www.alvyray.com/Papers/hsv2rgb.htm */
  // H is given on [0, 6]. S and V are given on [0, 1].
  // RGB is returned as a 24-bit long #rrggbb
  int i;
  float m, n, f;

  // not very elegant way of dealing with out of range: return black
  if ((s < 0.0) || (s > 1.0) || (v < 0.0) || (v > 1.0)) {
    return 0L;
  }

  if ((h < 0.0) || (h > 6.0)) {
    return long( v * 255 ) + long( v * 255 ) * 256 + long( v * 255 ) * 65536;
  }
  i = floor(h);
  f = h - i;
  if ( !(i & 1) ) {
    f = 1 - f; // if i is even
  }
  m = v * (1 - s);
  n = v * (1 - s * f);
  switch (i) {
    case 6:
    case 0:
      return long(v * 255 ) * 65536 + long( n * 255 ) * 256 + long( m * 255);
    case 1:
      return long(n * 255 ) * 65536 + long( v * 255 ) * 256 + long( m * 255);
    case 2:
      return long(m * 255 ) * 65536 + long( v * 255 ) * 256 + long( n * 255);
    case 3:
      return long(m * 255 ) * 65536 + long( n * 255 ) * 256 + long( v * 255);
    case 4:
      return long(n * 255 ) * 65536 + long( m * 255 ) * 256 + long( v * 255);
    case 5:
      return long(v * 255 ) * 65536 + long( m * 255 ) * 256 + long( n * 255);
  }
}


void callib() {
  digitalWrite(13, HIGH);
  while (millis() < 3000) {
    threshold = analogRead(entrada);

    // record the maximum sensor value
    if (threshold > sensorMax) {
      sensorMax = threshold;
    }

  }

  // signal the end of the calibration period
  digitalWrite(13, LOW);
  threshold = sensorMax - MEDIA_MAX;
  Serial.println("     ________________________________");
  Serial.print("         Audio MAX: ");
  Serial.println(threshold);

}

void readBut() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        estado++;
        ledState = !ledState;
        atualizaEstado();

      }
//      if ((millis() - lastDebounceTime) > 100 * debounceDelay) {
//        estado = 52;
//        atualizaEstado();
     // }


    }

    digitalWrite(ledPin, ledState);
    lastButtonState = reading;

  }
}





void serialEvent() {
  while (Serial.available()) {
    // get the new byte:

    char inByte = (char)Serial.read();
    estado = (int) inByte;
    atualizaEstado();
  }
}



void atualizaEstado() {
  estadoAnterior = estado;
  switch (estado) {


    case 48:
      Sensor = false;
      Serial.println(Sensor ? "Modo Sensivel" : "Modo Rotacao");
      break;

    case 49:
      input = true;
      entrada = input ? 0 : 1;
      Serial.println(input ? "Sensor Mic" : "Entrada audio");
      Sensor = true;
      Serial.println(Sensor ? "Modo Sensivel" : "Modo Rotacao");
      break;
      
    case 50:
      input = false;
      entrada = input ? 0 : 1;
      Serial.println(input ? "Sensor Mic" : "Entrada audio");
      Sensor = true;
      Serial.println(Sensor ? "Modo Sensivel" : "Modo Rotacao");
      break;
    case 51:
      estado = 48;
      break;

    case 52:
      callib();
      estado = 48;
      Sensor = true;
      Serial.println(Sensor ? "Modo Sensivel" : "Modo Rotacao");
      break;

    default:
      estado = 48;
      estadoAnterior = 47;
      break;
  }
  delay(1000);
}

void pisca(int d) {
  atualizaLEDS(3.0, 1.0, 1.0);
  delay(d);
  atualizaLEDS(1.0, 1.0, 0.0);
  delay(d);
}

void teste()

