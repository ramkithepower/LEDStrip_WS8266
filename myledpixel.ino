#include <Homie.h>
#include <Adafruit_NeoPixel.h>

#define FW_NAME "led-control"
#define FW_VERSION "1.0.0"

#define PIN            D4
#define LED_COUNT      60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; 
int showType = 0;
int SoffitR;
int SoffitG;
int SoffitB;
int numPixels = LED_COUNT;
bool bLoop = false;
HomieNode ledNode("led", "led");
String setEffect = "";

bool lightOnHandler(HomieRange range, String value) {
  
  Serial.print("Light On Handler:" + value);
  if(value == "Police")
  {
    Serial.print("Calling Police");
    police_new();
  }
  else
  {
    colorWipe(strip.Color(0, 0, 0), 50);
  
    if (value == "off") {
      colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
      //Homie.setNodeProperty(ledNode, "color").send("0,0,0,0");
      ledNode.setProperty("color").send("0,0,0,0");
    } else if (value == "red") {
      colorWipe(strip.Color(255, 0, 0), 50);  // Red
      ledNode.setProperty("color").send("0,255,0,0");
    } else if (value == "green") {
      colorWipe(strip.Color(0, 255, 0), 50);  // Green
      ledNode.setProperty("color").send("0,0,255,0");
    } else if (value == "blue") {
      colorWipe(strip.Color(0, 0, 255), 50);  // Blue
      ledNode.setProperty("color").send("0,0,255,0");
    } else {
        //value.trim();
        Serial.print (value);
        //Serial.flush();
        // split string at every "," and store in proper variable
        // convert final result to integer
        SoffitR = value.substring(0,value.indexOf(',')).toInt();
        SoffitG = value.substring(value.indexOf(',')+1,value.lastIndexOf(',')).toInt();
        SoffitB = value.substring(value.lastIndexOf(',')+1).toInt();
        colorWipe(strip.Color(SoffitR, SoffitG, SoffitB), 50);
        ledNode.setProperty("color").send(value);
        Serial.print("Value Set to LED");
    }
  }

  
  return true;
}

int i;
int idex = 0;
uint8_t thishuepolice = 0;
int thissat = 255; 
int TOP_INDEX = int(numPixels / 2);
int antipodal_index(int i) {
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX) {
    iN = ( i + TOP_INDEX ) % numPixels;
  }
  return iN;
}

void police_new()
{
  int indexR = 0;
  int indexB = numPixels;
  boolean reverse = false;
  int indexRMax = 0;
  int indexBMax = 59;
  bLoop = true;
  do{
    colorWipe(strip.Color(0, 0, 0), 50);
    if(!reverse)
    {
      indexRMax = indexR + 4;      
      indexRMax = checkMax(indexRMax);            
      indexBMax = indexB - 4;
      indexBMax = checkMin(indexBMax);
      
      //set red color
      Serial.println("Setting Red Pixels");
      setColorsInc(strip.Color(255, 0, 0), indexR, indexRMax);
      
      Serial.println("Setting Blue Pixels");
      setColorsDec(strip.Color(0, 0, 255), indexB, indexBMax);
      strip.show();  
      indexR ++;
      indexB --;
      if(indexR > numPixels-1 || indexB < 0)
      {
        reverse = true;
      }
    }
    else
    {
      indexRMax = indexR - 4;
      indexBMax = indexB + 4;        
      indexRMax = checkMin(indexRMax);
      indexBMax = checkMax(indexBMax);
      
      //set red color
      Serial.println("Setting Blue Pixels");
      setColorsInc(strip.Color(0, 0, 255), indexB, indexBMax);      
      
      Serial.println("Setting Red Pixels");
      setColorsDec(strip.Color(255, 0, 0), indexR, indexRMax);
      strip.show();       
      indexR --;
      indexB ++;
      
      if(indexB > numPixels-1 || indexR < 0)
      {
        reverse = false;
      }      
    }
   
   wdt_reset();
    ///strip.setPixelColor(indexR, strip.Color(0, 255, 0));
  }while(bLoop);
}

int checkMax(int numToCheck)
{
  if(numToCheck > numPixels-1)
      {
        return numPixels - 1;
      }
      return numToCheck;
}

int checkMin(int numToCheck)
{
  if(numToCheck < 0)
      {
        return 0;
      }
      return numToCheck;
}

void setColorsInc(uint32_t c,int idex, int indexMax)
{
  for(int iCount=idex; iCount < indexMax ; iCount++)
      {
        strip.setPixelColor(iCount, c);
      }
}

void setColorsDec(uint32_t c,int idex, int indexMax)
{
  for(int iCount=idex-1; iCount > indexMax ; iCount--)
      {
        strip.setPixelColor(iCount, c);
        Serial.println(iCount);
      }
}

void colorWipe(uint32_t c, uint8_t wait) {
for(uint16_t i=0; i<strip.numPixels(); i++) { //<strip.numPixels
strip.setPixelColor(i, c);
strip.show();
delay(wait);
}
}

void setup() {
  Serial.begin(115200);
  Serial.println(FW_NAME FW_VERSION);  
  strip.begin();
  Homie_setFirmware(FW_NAME, FW_VERSION);
  ledNode.advertise("color").settable(lightOnHandler);
  Homie.setup();
}

void loop() {
  
    Homie.loop();
  
  
  
}
