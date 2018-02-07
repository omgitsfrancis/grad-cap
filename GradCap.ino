#include <Adafruit_NeoPixel.h> 
// Neopixel library: https://github.com/adafruit/Adafruit_NeoPixel

#define LED_PIN 4			// To data pin of LED strip
#define BUTTON_PIN 2		// To momentary switch
#define NUM_LED 53			// Total number of LEDs in strip
#define DEBOUNCE_TIME 500	// Switch debounce time (ms)
#define BRIGHTNESS 25		// Brightness 0-100

// Avoiding overheating the onboard MOSFET.
// Tips:
// 	-Avoid colors that turn on all colors of an LED (ie white)
// 	-Avoid having all LEDs on simultaneously 
//	-Turn the brightness down

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, LED_PIN, NEO_GRB + NEO_KHZ800);
uint8_t mode = 0;
unsigned long lastDebounceTime = 0;
bool tog = LOW;
uint8_t cc_pos = 10;//color cycle position

// Preset colors
uint32_t white = strip.Color(188,188,188);
uint32_t red = strip.Color(255,0,0);
uint32_t yellow = strip.Color(255,255,0);
uint32_t green = strip.Color(0,255,0);
uint32_t aqua = strip.Color(0,255,255);
uint32_t blue = strip.Color(0,0,255);
uint32_t purple = strip.Color(255,0,255);
uint32_t pink = strip.Color(255,100,180);


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
  attachInterrupt(0, modeswitch, RISING); //INT0 - pin 2 - calls modeswitch()
}

void loop() 
{
      switch(mode)
      {
        case 0:
                rainbow(10);
                break;
        case 1:
                Strobe(ColorCycle(), 0, 0, 400, 69, 0 );
                break;
        case 2:
                MidSplit(25,ColorCycle());
                break;   
        case 3:
                CircleLoop(5, 10, TriColorCycle(red, green, blue));
                break;
        case 4:
				fourchaser(ColorCycle(),75);
				break;
        case 5:
                AltFill(white, red, 4);
                delay(300);
                AltFill(red, white, 4);
                delay(300);
                break;
        case 6:
                Strobe(red,purple,blue,33,33,33); 
                strip.clear();
                strip.show();
                delay(484);
                break;
        case 7:
                CircleLoop(5, 20, TriColorCycle(pink, red, aqua));
                AltFill(red, white, 6);
                delay(300);
                MidSplit(10,ColorCycle());
                Strobe(aqua,purple,blue,100,50,100);
                CircleLoop(5, 10, TriColorCycle(red, green, blue));
                fourchaser(ColorCycle(),75);
                AltFill(pink, yellow, 1);
                delay(500);
                rainbow(1);
                break;
		 
        default:
                mode=0;
                break;

      }
      tog =  LOW;
}  
void rainbow(byte RAINBOW_SPEED)
      uint16_t j;
      for(j=0; j<256; j++)
      {
            for(uint8_t i=0; i<NUM_LED; i++)
            {
                strip.setPixelColor(i, Wheel((i+j) & 255)); 
            }

            if(tog)
            {
              break;
            }
              
            delay(RAINBOW_SPEED);
            strip.show();
      }
}
uint32_t Wheel(byte WheelPos) 
{
     if(WheelPos < 85) 
     {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
     } else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
     } else {
      WheelPos -= 170;
         return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
     }
}

void CircleLoop(byte LINE_SPEED, byte LINE_SIZE, uint32_t COLOR)
{
  for(byte i=0; i<NUM_LED; i++)
  {
    strip.clear();
    for(byte j=i; j<i+LINE_SIZE; j++)
    {
      byte k = j % NUM_LED;
      strip.setPixelColor(k,COLOR);
    }
    strip.show();
    delay(LINE_SPEED);
    
    if(tog)
      break;    
  }
}

void MidSplit(byte LINE_SPEED, uint32_t COLOR)
{
   for(byte i = 0; i<NUM_LED/2; i++)
   {
	   strip.setPixelColor(i+(NUM_LED/2),COLOR);
	   strip.setPixelColor((NUM_LED/2)-1-i,COLOR);
	   strip.show();
	   delay(LINE_SPEED);
     if(tog)
      break;
   }
}

void Strobe(uint32_t COLOR1, uint32_t COLOR2, uint32_t COLOR3, uint16_t TIME1, uint16_t TIME2, uint16_t TIME3)
{
      FillAll(COLOR1);
      delay(TIME1);
     
      FillAll(COLOR2);
      delay(TIME2);
      
      FillAll(COLOR3);
      delay(TIME3);
}
//	Cycles through 6 preset colors
uint32_t ColorCycle()
{
    cc_pos++;
  
    if(cc_pos>5)
      cc_pos = 0;
      
    switch(cc_pos)
    {
      case 0:
              return red;
      case 1:
              return yellow;
      case 2:
              return green;
      case 3:
              return aqua;
      case 4:
              return blue;
      case 5:
              return purple;
    }
}
//	Cycles through 3 colors of your choice
uint32_t TriColorCycle(uint32_t COLOR1, uint32_t COLOR2, uint32_t COLOR3)
{
  cc_pos++;
  if(cc_pos>2)
      cc_pos = 0;
  
  switch(cc_pos)
  {
    case 0:
        return COLOR1;
    case 1:
        return COLOR2;
    case 2:
        return COLOR3;      
  }
}



void Fill(uint16_t from, uint16_t to, uint32_t color)
{
  for(uint8_t i=from; i<=to; i++)
  {
  strip.setPixelColor(i, color);
  }  
}

void FillAll(uint32_t color)
{
  for(uint8_t i=0; i<NUM_LED; i++)
  {
      strip.setPixelColor(i, color); 
  }
  strip.show();
}

void AltFill(uint32_t COLOR1, uint32_t COLOR2, byte LENGTH) //Length of light gaps
{
  bool AltTog = 0;
  for(uint8_t i=0; i<NUM_LED-LENGTH; i+=LENGTH)
  {
  if(AltTog)
    Fill(i, i+LENGTH-1, COLOR1);
  else
    Fill(i, i+LENGTH-1, COLOR2);
    AltTog=!AltTog;
  }
  strip.show();
}

void fourchaser(uint32_t COLOR, byte SPEED)
{
	for(byte i=0; i<13; i++)
	{
		strip.clear();
		
		strip.setPixelColor(i, COLOR);
		strip.setPixelColor(i+13, COLOR);
		strip.setPixelColor(i+26, COLOR);
		strip.setPixelColor(i+39, COLOR);
		
		strip.show();
		
		delay(SPEED);
	}	
}
// Called by momentary switch
void modeswitch()
{
    if ((millis() - lastDebounceTime) > DEBOUNCE_TIME)
    {
      lastDebounceTime = millis();
      mode++;
	  strip.clear();
      tog = HIGH;
    }   
}





