// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin



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
#define MEDIA_MAX 8
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
