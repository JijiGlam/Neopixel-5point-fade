//this code uses the OCTOWS2811 library SPECIFICALLY for Teensy
//receive signal from arduino via RX
#include <OctoWS2811.h>
#define Uno Serial1

const int ledsPerStrip = 110;  //if you change the num of leds, change the numbers below

DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

class LEDSTRIP
{
  public:

    uint32_t ColorA1, ColorA2, ColorA3, ColorA4, ColorA5,
             ColorB1, ColorB2, ColorB3, ColorB4, ColorB5,
             firstColor, secondColor, thirdColor, fourthColor, fifthColor,
             Color1, Color2;
    int firstLED;
    uint8_t red, grn, blu;
    int16_t x, y;
    uint16_t ledNum = ledsPerStrip; //total number of LEDs in the strip
    uint16_t Index1 = 0; //location between color change
    byte a ; //counter for debugging
    int updateInterval;
    unsigned long lastUpdate;
    uint16_t TotalSteps = 255;

    LEDSTRIP(int firstLed)  //the index of the first pixel
    {
      firstLED = firstLed;
    }
    void Update()
    {
      FadeUpdate();
      leds.show();
    }

    void Fade(
      uint32_t colorA1, uint32_t colorA2, uint32_t colorA3, uint32_t colorA4, uint32_t colorA5,
      uint32_t colorB1, uint32_t colorB2, uint32_t colorB3, uint32_t colorB4, uint32_t colorB5,
      long interval )
    {
      ColorA1 = colorA1;
      ColorA2 = colorA2;
      ColorA3 = colorA3;
      ColorA4 = colorA4;
      ColorA5 = colorA5;
      ColorB1 = colorB1;
      ColorB2 = colorB2;
      ColorB3 = colorB3;
      ColorB4 = colorB4;
      ColorB5 = colorB5;
      updateInterval = interval;
    }

    void FadeUpdate()
    {
      if (Index1 <= TotalSteps) {
        ColorChange(ColorA1, ColorB1, ColorA2, ColorB2, ColorA3, ColorB3, ColorA4, ColorB4, ColorA5, ColorB5, Index1);
      }
      for (int x = 0; x <= ledNum / 2 ; x++) {
        ColorChangeSet1(firstColor, secondColor, thirdColor, x);
      }
      for (int x = ledNum / 2; x < ledNum; x++) {
        ColorChangeSet2(thirdColor, fourthColor, fifthColor, x);
      }

    }

    void ColorChange (uint32_t color_1, uint32_t color_2, uint32_t color_3, uint32_t color_4, uint32_t color_5, uint32_t color_6,
                      uint32_t color_7, uint32_t color_8, uint32_t color_9, uint32_t color_10, uint16_t IndexNum)
    //colorNum(number of colors in the fade constructor) is for increment function to update index1 at different fade mode.
    //only changes 5 pixels on the strip
    {
      uint8_t redX1 = ((Red(color_1) * (TotalSteps - IndexNum)) + (Red(color_2) * IndexNum)) / TotalSteps;
      uint8_t greenX1 = ((Green(color_1) * (TotalSteps - IndexNum)) + (Green(color_2) * IndexNum)) / TotalSteps;
      uint8_t blueX1 = ((Blue(color_1) * (TotalSteps - IndexNum)) + (Blue(color_2) * IndexNum)) / TotalSteps;

      uint8_t redX2 = ((Red(color_3) * (TotalSteps - IndexNum)) + (Red(color_4) * IndexNum)) / TotalSteps;
      uint8_t greenX2 = ((Green(color_3) * (TotalSteps - IndexNum)) + (Green(color_4) * IndexNum)) / TotalSteps;
      uint8_t blueX2 = ((Blue(color_3) * (TotalSteps - IndexNum)) + (Blue(color_4) * IndexNum)) / TotalSteps;

      uint8_t redX3 = ((Red(color_5) * (TotalSteps - IndexNum)) + (Red(color_6) * IndexNum)) / TotalSteps;
      uint8_t greenX3 = ((Green(color_5) * (TotalSteps - IndexNum)) + (Green(color_6) * IndexNum)) / TotalSteps;
      uint8_t blueX3 = ((Blue(color_5) * (TotalSteps - IndexNum)) + (Blue(color_6) * IndexNum)) / TotalSteps;

      uint8_t redX4 = ((Red(color_7) * (TotalSteps - IndexNum)) + (Red(color_8) * IndexNum)) / TotalSteps;
      uint8_t greenX4 = ((Green(color_7) * (TotalSteps - IndexNum)) + (Green(color_8) * IndexNum)) / TotalSteps;
      uint8_t blueX4 = ((Blue(color_7) * (TotalSteps - IndexNum)) + (Blue(color_8) * IndexNum)) / TotalSteps;

      uint8_t redX5 = ((Red(color_9) * (TotalSteps - IndexNum)) + (Red(color_10) * IndexNum)) / TotalSteps;
      uint8_t greenX5 = ((Green(color_9) * (TotalSteps - IndexNum)) + (Green(color_10) * IndexNum)) / TotalSteps;
      uint8_t blueX5 = ((Blue(color_9) * (TotalSteps - IndexNum)) + (Blue(color_10) * IndexNum)) / TotalSteps;

      firstColor = Color(redX1, greenX1, blueX1);
      secondColor = Color(redX2, greenX2, blueX2);
      thirdColor = Color(redX3, greenX3, blueX3);
      fourthColor = Color(redX4, greenX4, blueX4);
      fifthColor = Color(redX5, greenX5, blueX5);

      Increment();
    }

    void ColorChangeSet1 (uint32_t color_1, uint32_t color_2,  uint32_t color_3, uint16_t IndexNum)
    //automatically update all the other pixels
    {
      if (IndexNum <= (ledNum / 4 - 1)) {  //index <= 7
        x = IndexNum;
        y = ledNum / 4  ;
        Color1 = color_1;
        Color2 = color_2;
      }
      else if ( (IndexNum >= (ledNum / 4)) && (IndexNum < (ledNum / 2)) ) {  //index>=8
        x = IndexNum  - ledNum / 4 ;
        y = ledNum / 4 ;
        Color1 = color_2;
        Color2 = color_3;
        //    if (IndexNum == (ledNum / 2)) {  //a function that shows the middle color
        //      Serial.print("LEDNum   ");
        //      Serial.println (ledNum / 2);
        //      Serial.print ("   red   ");
        //      Serial.print (red);
        //      Serial.print ("   green   ");
        //      Serial.print (grn);
        //      Serial.print ("   blue   ");
        //      Serial.println (blu);
        //    }

      }
      red = ((Red(Color1) * (y - x)) + (Red(Color2) * x)) / y;
      grn = ((Green(Color1) * (y - x)) + (Green(Color2) * x)) / y;
      blu = ((Blue(Color1) * (y - x)) + (Blue(Color2) * x)) / y;
      leds.setPixel((firstLED + IndexNum), red, grn, blu);
    }

    void ColorChangeSet2 (uint32_t color_1, uint32_t color_2,  uint32_t color_3, uint16_t IndexNum)
    //automatically update all the other pixels
    {
      if ( (IndexNum < (ledNum * 3 / 4 - 1)) && (IndexNum >= (ledNum / 2)) ) { //index <= 7
        x = IndexNum - ledNum / 2;
        y = ledNum / 4  ;
        Color1 = color_1;
        Color2 = color_2;
      }
      else if (IndexNum >= (ledNum * 3 / 4)) { //index>=8
        x = IndexNum - ledNum * 3 / 4 ;
        y = ledNum / 4  ;
        Color1 = color_2;
        Color2 = color_3;
        //    if (IndexNum == (ledNum / 2)) {  //a function that shows the middle color
        //      Serial.print("LEDNum   ");
        //      Serial.println (ledNum / 2);
        //      Serial.print ("   red   ");
        //      Serial.print (red);
        //      Serial.print ("   green   ");
        //      Serial.print (grn);
        //      Serial.print ("   blue   ");
        //      Serial.println (blu);
        //    }

      }
      red = ((Red(Color1) * (y - x)) + (Red(Color2) * x)) / y;
      grn = ((Green(Color1) * (y - x)) + (Green(Color2) * x)) / y;
      blu = ((Blue(Color1) * (y - x)) + (Blue(Color2) * x)) / y;
      leds.setPixel((firstLED + IndexNum), red, grn, blu);
    }

    void Increment()
    {
      if ((millis() - lastUpdate) > updateInterval)
      {
        lastUpdate = millis();
        Index1++;
        if (Index1 > TotalSteps)
        {
          Index1 = 0;
          a++;
          Serial.println(a);
        }
      }
    }

    void ShutLED() {
      for (int i = 0; i < ledNum * 3; i++) {
        leds.setPixel(i, 0, 0, 0);
      }

      Index1 = 0;
      a = 0;
      leds.show();
    }

    void Zero() {
      Index1 = 0;
      a = 0;
    }


    // Returns the Red component of a 32 - bit color
    uint8_t Red(uint32_t color)
    {
      return (color >> 16) & 0xFF; // & symbol represent bitmath
    }

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color)
    {
      return (color >> 8) & 0xFF;
    }

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color)
    {
      return color & 0xFF;
    }

    uint32_t Color (uint8_t r, uint8_t g, uint8_t b) {
      return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
    }
};

const uint32_t Scheme1 [1000] = {
  0xFF6900, 0xFF6900, 0xFF690F, 0x873C0F, 0x964B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x69A50F, 0x4B4B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x69A50F, 0x4B4B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x693C0F, 0xFF0F3C,
  0x96FF3C, 0x96FF3C, 0x5AF03C, 0x693C0F, 0xFF0F3C,  //5
  0x96FF3C, 0x96FF3C, 0x5AF03C, 0xFF0F3C, 0xFF0F3C,
  0x96FF3C, 0x96FF3C, 0xFF0F3C, 0xFF0F3C, 0xFF0F3C,
  0x96FF3C, 0xFF0F3C, 0xFF0F3C, 0xFF1EFF, 0xFF3CD2,
  0xFF0F3C, 0xFF0F3C, 0xFF0FF0, 0xFF1EFF, 0xFF3CD2,
  0xFF0F3C, 0xFF0FF0, 0xFF1EF0, 0xFF1EFF, 0xFF3CD2,  //10
  0xFF1EF0, 0xFF1EF0, 0xFF1EF0, 0xFF1EFF, 0xFF3CD2,
  0xFF1EF0, 0xFF1EF0, 0xFF1EF0, 0xFFA5FF, 0x003CFF,
  0xFF1EF0, 0xFF1EF0, 0xFF1EF0, 0xFFA5FF, 0x003CFF,
  0xFF1EF0, 0xFF2D4B, 0xFF6900, 0xFFA5FF, 0x003CFF,
  0xFF1EF0, 0xFF2D0F, 0xFF6900, 0xFFA569, 0x003CFF   //15
};


const uint32_t Scheme2 [1000] = {
  0xF004FF, 0x3C005A, 0x00004B, 0x00005A, 0x0F003C,
  0xF004FF, 0x3C005A, 0x00004B, 0x00005A, 0x0F003C,
  0x4B04FF, 0x1E005A, 0x00004B, 0xFF0F4B, 0x0F3C96,
  0x4B00FF, 0x3C005A, 0x2D004B, 0x2D0F4B, 0xFF0F4B,
  0x4B00FF, 0x3C005A, 0x2D004B, 0x2D0F4B, 0xFF0F4B, //5
  0x4B00FF, 0x3C005A, 0x2D004B, 0xFF0F3C, 0xFF0F4B,
  0x4B00FF, 0x3C005A, 0xFF0F3C, 0xFF0F3C, 0xFF0F4B,
  0x4B00FF, 0x3C005A, 0xFF0F3C, 0xFF0F3C, 0xFF3CD2,
  0xFF0F3C, 0xA4005A, 0xFF0FC3, 0xFF3CD2, 0xFF3CD2,
  0xFF0F3C, 0xA40096, 0xFF2DC3, 0xFF3CD2, 0xFF3CD2, //10
  0xFF2DC3, 0xFF2DC3, 0xFF2DC3, 0xFF3CD2, 0xFF3CD2,
  0xFF2DC3, 0xFF2DC3, 0xFF2DC3, 0xFFA5D2, 0x003CFF,
  0xFF2DC3, 0xFF2D4B, 0xFF6900, 0xFFA5D2, 0x003CFF,
  0xFF2DC3, 0xFF2D4B, 0xFF6900, 0xFFA5D2, 0x003CFF,
  0xFF2DC3, 0xFF3D0F, 0xFF6900, 0xFFA569, 0x003CFF   //15
};


const uint32_t Scheme3 [1000] = {
  0xFF6900, 0xFF6900, 0xFF690F, 0x873C0F, 0x964B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x69A50F, 0x4B4B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x69A50F, 0x4B4B1E,
  0x0FFF00, 0x87FF1E, 0x5AFF0F, 0x693C0F, 0xFF0F3C,
  0x96FF3C, 0x96FF3C, 0x5AF03C, 0x693C0F, 0xFF0F3C,  //5
  0x96FF3C, 0x96FF3C, 0x5AF03C, 0xFF0F3C, 0xFF0F3C,
  0x96FF3C, 0x96FF3C, 0xFF0F3C, 0xFF0F3C, 0xFF0F3C,
  0x96FF3C, 0x96FF3C, 0xFF0F3C, 0xFF0F3C, 0xFF0F3C,
  0x96FF3C, 0xFF1E3C, 0xFF0F3C, 0xFF0F96, 0xFF1ED2,
  0xFF0F3C, 0xFF2D4B, 0xFF0FC3, 0xFF1EA5, 0xFF3CD2,  //10
  0xFF2DC3, 0xFF2DC3, 0xFF2DC3, 0xFF2DA5, 0xFF3CD2,
  0xFF2DC3, 0xFF2DC3, 0xFF2DC3, 0xFFA5D2, 0x003CFF,
  0xFF2DC3, 0xFF2DC3, 0xFF2DC3, 0xFFA5D2, 0x003CFF,
  0xFF2DC3, 0xFF2D4B, 0xFF6900, 0xFFA5D2, 0x003CFF,
  0xFF1EF0, 0xFF2D0F, 0xFF6900, 0xFFA569, 0x003CFF
};

const uint32_t SchemeA [1000] = {
  0x69000F, 0x000000, 0x000000, 0x000000, 0x000000,
  0x96002D, 0x3C000C, 0x000000, 0x000000, 0x000000,
  0xC3004B, 0x5A0017, 0x3C0009, 0x000000, 0x000000,
  0xFF005A, 0xB40035, 0x78001D, 0x87001E, 0x000000,
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A,  //5
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A, 0x9600FF,
  0xFF005A, 0xFF005A, 0xFF005A, 0x3C0FC3, 0x004BFF,
  0xFF005A, 0x5A00A5, 0x000FB4, 0x004BFF, 0x004BFF,
  0x4B00B4, 0x2D00E1, 0x000FB4, 0x00B4FF, 0x00FFFF,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x00B4FF, 0x00FFFF,  //10
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0xD2A512, 0x007896, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0xD2A512, 0x96784B, 0x00A5B4, 0x96D2FF, 0xB4A59F   //15
};


const uint32_t SchemeB [1000] = {
  0x780012, 0x000000, 0x000000, 0x000000, 0x000000,
  0xB4003F, 0x3C0016, 0x000000, 0x000000, 0x000000,
  0xB4003F, 0x87001D, 0x140008, 0x000000, 0x000000,
  0xFF005A, 0xD2003B, 0x6E0022, 0x000000, 0x000000,
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A,  //5
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF00FF, 0x004BFF,
  0xFF005A, 0xFF005A, 0xFF5A5A, 0x002DFF, 0x004BFF,
  0xFF005A, 0x5A00FF, 0x0F3C5A, 0x004BFF, 0x004BFF,
  0x3C00FF, 0x1E00FF, 0x0F2D5A, 0x00B4FF, 0x00FFFF,
  0x0000FF, 0x005AD2, 0x00785A, 0x00B4FF, 0x00FFFF,  //10
  0x0000FF, 0x005AD2, 0x00785A, 0x96D2FF, 0xB4A59F,
  0x0000FF, 0x005AD2, 0x00C300, 0x96D2FF, 0xB4A59F,
  0x004BFF, 0xD27800, 0xF02D00, 0x96D2FF, 0xB4A59F,
  0x4B782D, 0xFF5A00, 0xF00000, 0x96D2FF, 0xB4A59F,
  0x4B7800, 0xFF961E, 0xFF1E00, 0xFF0000, 0x3CFFFF
};


const uint32_t SchemeC [1000] = {
  0x69000F, 0x000000, 0x000000, 0x000000, 0x000000,
  0x96002D, 0x3C000C, 0x000000, 0x000000, 0x000000,
  0xC3004B, 0x5A0017, 0x3C0009, 0x000000, 0x000000,
  0xFF005A, 0xB40035, 0x78001D, 0x87001E, 0x000000,
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A,  //5
  0xFF005A, 0xFF005A, 0xFF005A, 0xFF005A, 0x9600FF,
  0xFF005A, 0xFF005A, 0xFF005A, 0x3C0FC3, 0x004BFF,
  0xFF005A, 0x5A00A5, 0x000FB4, 0x004BFF, 0x004BFF,
  0x4B00B4, 0x2D00E1, 0x000FB4, 0x00B4FF, 0x00FFFF,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x00B4FF, 0x00FFFF,  //10
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0x0000B4, 0x0F4BE1, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0xD2A512, 0x007896, 0x00A5B4, 0x96D2FF, 0xB4A59F,
  0xD2A512, 0x96784B, 0x00A5B4, 0x96D2FF, 0xB4A59F
};


uint32_t s1_color1_1, s1_color1_2, s1_color1_3, s1_color1_4, s1_color1_5,
         s2_color1_1, s2_color1_2, s2_color1_3, s2_color1_4, s2_color1_5,
         s3_color1_1, s3_color1_2, s3_color1_3, s3_color1_4, s3_color1_5,
         s4_color1_1, s4_color1_2, s4_color1_3, s4_color1_4, s4_color1_5,
         s1_color2_1, s1_color2_2, s1_color2_3, s1_color2_4, s1_color2_5,
         s2_color2_1, s2_color2_2, s2_color2_3, s2_color2_4, s2_color2_5,
         s3_color2_1, s3_color2_2, s3_color2_3, s3_color2_4, s3_color2_5,
         s4_color2_1, s4_color2_2, s4_color2_3, s4_color2_4, s4_color2_5;
byte A, B, C, D;
boolean flag1, flag2, flag3;
uint8_t intervalNum;
byte colorIndex1 = 11; //number of color changes
byte colorIndex2 = 15;
byte a1;
byte incomingByte;
byte oldByte;
LEDSTRIP strip1 (0);  //defines the first pixel where the strip starts
LEDSTRIP strip2 (ledsPerStrip);
LEDSTRIP strip3 (ledsPerStrip * 2);


void setup() {
  Serial.begin(9600);
  Uno.begin(9600);
  leds.begin();
  leds.show();

  strip1.Fade ( 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 5);
  strip2.Fade ( 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 5);
  strip3.Fade ( 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 5);
}

void loop() {
  checkRF();
  if (incomingByte == 4) {
    strip1.ShutLED();
    strip2.ShutLED();
    strip3.ShutLED();
  }

  else {

    

    if (incomingByte == 1) {  //A pressed
      flag1 = 1;
      flag2 = 0;   //c button
    }
    if (incomingByte == 2) {  //B pressed
      flag1 = 0;
      flag2 = 0;    //c button
    }
    if (incomingByte == 3 ) {
      flag2 = 1;
    }

    if (flag1 == 1) {
      if (strip1.a == colorIndex1) {
        strip1.a = 0;
        strip2.a = 0;
        strip3.a = 0;
      }
      checkRF();
      a1 = strip1.a;
      s1_color1_1 = Scheme1 [a1 * 5];  //TOP COLOR OF THE FIRST SCHEME
      s1_color1_2 = Scheme1 [a1 * 5 + 1]; //MID COLOR OF THE FIRST SCHEME
      s1_color1_3 = Scheme1 [a1 * 5 + 2]; //BOTTOM COLOR OF THE FIRST SCHEME
      s1_color1_4 = Scheme1 [a1 * 5 + 3]; //MID COLOR OF THE FIRST SCHEME
      s1_color1_5 = Scheme1 [a1 * 5 + 4]; //BOTTOM COLOR OF THE FIRST SCHEME

      s2_color1_1 = Scheme2 [a1 * 5];  //TOP COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_2 = Scheme2 [a1 * 5 + 1]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_3 = Scheme2 [a1 * 5 + 2]; //BOTTOM COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_4 = Scheme2 [a1 * 5 + 3]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_5 = Scheme2 [a1 * 5 + 4];

      s3_color1_1 = Scheme3 [a1 * 5];  //TOP COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_2 = Scheme3 [a1 * 5 + 1]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_3 = Scheme3 [a1 * 5 + 2]; //BOTTOM COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_4 = Scheme3 [a1 * 5 + 3]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_5 = Scheme3 [a1 * 5 + 4];

      if (a1 < colorIndex1 - 1) {
        checkRF();
        s1_color2_1 = Scheme1 [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_2 = Scheme1 [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_3 = Scheme1 [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_4 = Scheme1 [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_5 = Scheme1 [(a1 + 1) * 5 + 4];

        s2_color2_1 = Scheme2 [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_2 = Scheme2 [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_3 = Scheme2 [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_4 = Scheme2 [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_5 = Scheme2 [(a1 + 1) * 5 + 4];

        s3_color2_1 = Scheme3 [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_2 = Scheme3 [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_3 = Scheme3 [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_4 = Scheme3 [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_5 = Scheme3 [(a1 + 1) * 5 + 4];
      }
      else if (a1 == colorIndex1 - 1) {
        checkRF();
        s1_color2_1 = Scheme1 [0];    //FRONT COLOR OF THE SCHEME1
        s1_color2_2 = Scheme1 [1];
        s1_color2_3 = Scheme1 [2];
        s1_color2_4 = Scheme1 [3];
        s1_color2_5 = Scheme1 [4];

        s2_color2_1 = Scheme2 [0];    //FRONT COLOR OF THE SCHEME1
        s2_color2_2 = Scheme2 [1];
        s2_color2_3 = Scheme2 [2];
        s2_color2_4 = Scheme2 [3];
        s2_color2_5 = Scheme2 [4];

        s3_color2_1 = Scheme3 [0];    //FRONT COLOR OF THE SCHEME1
        s3_color2_2 = Scheme3 [1];
        s3_color2_3 = Scheme3 [2];
        s3_color2_4 = Scheme3 [3];
        s3_color2_5 = Scheme3 [4];

      }
    }
    else if (flag1 == 0) {

      if (strip1.a == colorIndex2) { //colornum
        strip1.a = 0;
        strip2.a = 0;
        strip3.a = 0;
      }

      checkRF();
      a1 = strip1.a;
      s1_color1_1 = SchemeA [a1 * 5];  //TOP COLOR OF THE FIRST SCHEME
      s1_color1_2 = SchemeA [a1 * 5 + 1]; //MID COLOR OF THE FIRST SCHEME
      s1_color1_3 = SchemeA [a1 * 5 + 2]; //BOTTOM COLOR OF THE FIRST SCHEME
      s1_color1_4 = SchemeA [a1 * 5 + 3]; //MID COLOR OF THE FIRST SCHEME
      s1_color1_5 = SchemeA [a1 * 5 + 4]; //BOTTOM COLOR OF THE FIRST SCHEME

      s2_color1_1 = SchemeB [a1 * 5];  //TOP COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_2 = SchemeB [a1 * 5 + 1]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_3 = SchemeB [a1 * 5 + 2]; //BOTTOM COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_4 = SchemeB [a1 * 5 + 3]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s2_color1_5 = SchemeB [a1 * 5 + 4];

      s3_color1_1 = SchemeC [a1 * 5];  //TOP COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_2 = SchemeC [a1 * 5 + 1]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_3 = SchemeC [a1 * 5 + 2]; //BOTTOM COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_4 = SchemeC [a1 * 5 + 3]; //MID COLOR(RGB) OF THE SECOND SCHEME
      s3_color1_5 = SchemeC [a1 * 5 + 4];

      if (a1 < colorIndex2 - 1) {
        checkRF();
        s1_color2_1 = SchemeA [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_2 = SchemeA [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_3 = SchemeA [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_4 = SchemeA [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s1_color2_5 = SchemeA [(a1 + 1) * 5 + 4];

        s2_color2_1 = SchemeB [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_2 = SchemeB [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_3 = SchemeB [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_4 = SchemeB [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s2_color2_5 = SchemeB [(a1 + 1) * 5 + 4];

        s3_color2_1 = SchemeC [(a1 + 1) * 5]; //TOP COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_2 = SchemeC [(a1 + 1) * 5 + 1]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_3 = SchemeC [(a1 + 1) * 5 + 2]; //BOTTOM COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_4 = SchemeC [(a1 + 1) * 5 + 3]; //MID COLOR OF THE NEXT PATTERN IN SCHEME1
        s3_color2_5 = SchemeC [(a1 + 1) * 5 + 4];
      }
      else if (a1 == colorIndex2 - 1) {
        checkRF();
        s1_color2_1 = SchemeA [0];    //FRONT COLOR OF THE SCHEME1
        s1_color2_2 = SchemeA [1];
        s1_color2_3 = SchemeA [2];
        s1_color2_4 = SchemeA [3];
        s1_color2_5 = SchemeA [4];

        s2_color2_1 = SchemeB [0];    //FRONT COLOR OF THE SCHEME1
        s2_color2_2 = SchemeB [1];
        s2_color2_3 = SchemeB [2];
        s2_color2_4 = SchemeB [3];
        s2_color2_5 = SchemeB [4];

        s3_color2_1 = SchemeC [0];    //FRONT COLOR OF THE SCHEME1
        s3_color2_2 = SchemeC [1];
        s3_color2_3 = SchemeC [2];
        s3_color2_4 = SchemeC [3];
        s3_color2_5 = SchemeC [4];

      }
    }
    if (flag2 == 0) {
      //intervalNum = 10;
      intervalNum = 40;
    }
    else {
      intervalNum = 75;
    }

    strip1.ColorA1 = s1_color1_1;
    strip1.ColorA2 = s1_color1_2;
    strip1.ColorA3 = s1_color1_3;
    strip1.ColorA4 = s1_color1_4;
    strip1.ColorA5 = s1_color1_5;
    strip1.ColorB1 = s1_color2_1;
    strip1.ColorB2 = s1_color2_2;
    strip1.ColorB3 = s1_color2_3;
    strip1.ColorB4 = s1_color2_4;
    strip1.ColorB5 = s1_color2_5;

    strip2.ColorA1 = s2_color1_1;
    strip2.ColorA2 = s2_color1_2;
    strip2.ColorA3 = s2_color1_3;
    strip2.ColorA4 = s2_color1_4;
    strip2.ColorA5 = s2_color1_5;
    strip2.ColorB1 = s2_color2_1;
    strip2.ColorB2 = s2_color2_2;
    strip2.ColorB3 = s2_color2_3;
    strip2.ColorB4 = s2_color2_4;
    strip2.ColorB5 = s2_color2_5;

    strip3.ColorA1 = s3_color1_1;
    strip3.ColorA2 = s3_color1_2;
    strip3.ColorA3 = s3_color1_3;
    strip3.ColorA4 = s3_color1_4;
    strip3.ColorA5 = s3_color1_5;
    strip3.ColorB1 = s3_color2_1;
    strip3.ColorB2 = s3_color2_2;
    strip3.ColorB3 = s3_color2_3;
    strip3.ColorB4 = s3_color2_4;
    strip3.ColorB5 = s3_color2_5;
    checkRF();

    strip1.updateInterval = intervalNum;
    strip2.updateInterval = intervalNum;
    strip3.updateInterval = intervalNum;

    strip1.Update();
    strip2.Update();
    strip3.Update();

    //check if the array is correct
    //      Serial.print(strip1.Index1);
    //use a1+1 because a1 starts counting form zero

    // Serial.print(R2_1);
    // Serial.print("\t");
    // Serial.print(G2_1);
    // Serial.print("\t");
    // Serial.println(B2_1);
    // Serial.print(R2_2);
    // Serial.print("\t");
    // Serial.print(G2_2);
    // Serial.print("\t");
    // Serial.println(B2_2);
    // Serial.print(R2_3);
    // Serial.print("\t");
    // Serial.print(G2_3);
    // Serial.print("\t");
    // Serial.println(B2_3);
  }
}


void checkRF(){
  if (Uno.available() > 0) {
    incomingByte = Uno.read();
    strip1.Zero();
    strip2.Zero();
    strip3.Zero();
  }
}



