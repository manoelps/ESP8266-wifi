// TODO.... 
// 1.  fix MQTT instructions... currently does not work through opState... and extra effects don't work... must integrate MODEDROP....
// 2. spotty pixel... 


// MEMORY ALLOCATIONS:

// PixelCount_address + 0 =  
// PixelCount_address + 1 =  
// PixelCount_address + 2 =  
// PixelCount_address + 3 =  LastOpState

String CurrentRGBcolour = " "; // This is for the WEBPAGE... takes the value when colour is changed...

int lasteffectupdate = 0 ; 
int WS2812interval = 2000; 
long Random_func_timeout = 0; 

//int spectrumValue[7];
//int filter=80;

uint16_t effectState = 0;


uint16_t 
var1 = 0,var2 = 0,var3 = 0,var4 = 0,var5 = 0,
var6 = 0,var7 = 0,var8 = 0,var9 = 0,var10 = 0;

static const char *VAR_STRING[] = {
"Floor             ", // var 1
"Ceiling           ", // var 2
"Var3              ", // var 3
"Var4              ", // var 4
"Var5              ", // var 5
"Var6              ", // var 6
"Total_X           ", // var 7
"Number of effects ", // var 8
"Var9              ", // var 9
"Size of effect    "};// var 10




void  handle_WS2812 () { // handles the web commands...


static int power; 
bool updateLEDs = false;

String selected = " "  ;

 if (server.arg("mode").length() != 0) WS2812_mode_string(server.arg("mode"));
 if ((server.arg("dim") != String(CurrentBrightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812interval)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 if ((server.arg("anispeed") != String(CurrentAnimationSpeed)) && (server.arg("anispeed").length() != 0))  Animationspped_command_string(server.arg("anispeed"));


 if (server.arg("rgbpicker").length() != 0) 
  { 
    WS2812_mode_string("rgb-" + server.arg("rgbpicker"));

    Serial.println("RGB picker command: " + server.arg("rgbpicker"));
  }

  //if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command"));

  if (server.arg("leds").length() != 0) {
    pixelCount = server.arg("leds").toInt();
    updateLEDs = true;
  }

   if (server.arg("ledpin").length() != 0) {
    pixelPIN = server.arg("ledpin").toInt();
    updateLEDs = true;
  }

  if ((server.arg("modedrop").length() != 0)) 
    {

      WS2812_mode_string(server.arg("modedrop"));

      }

  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + String(deviceid) + "</title></head>\n<body><h1> " + String(deviceid) + " </h1>\n";   httpbuf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>";
  
  //httpbuf += "<form action='/ws2812' method='POST'>     System is:  <font size='5' color='red'> " + String(opState) + " Last Op State: " + String(LastOpState) + "  </font> ";//"   <input type='submit' name='mode' value='on'>    <input type='submit' name='command' value='off'></form>"; 
  httpbuf += "<br> <a href='/ws2812?mode=off'>OFF</a> | <a href='/ws2812?mode=on'>ON</a>   | <a href='/ws2812?mode=refresh'>REFRESH</a> | <a href='/lightsconfig'>CONFIG</a>  ";
  //httpbuf += "<br> <a href='/ws2812?mode=Colour'>Colour</a>  <a href='/ws2812?mode=Rainbow'>Rainbow</a>  <a href='/ws2812?mode=Fade'>Fade</a>  <a href='/ws2812?mode=ChaseRainbow'>ChaseRainbow</a>  <a href='/ws2812?mode=test'>TEST</a> ";
  //httpbuf += "<br> <a href='/ws2812?mode=fadeinfadeout'>FadeInFadeOut</a> <a href='/ws2812?mode=pickrandom'>PickRandom</a> <a href='/ws2812?mode=looparound'>LoopAround</a>";
  //httpbuf += "<br> <a href='/ws2812?mode=adalight'>Adalight</a>  <a href='/ws2812?mode=udp'>UDP</a>" ; 
  httpbuf += "<form name=frmTest action='/ws2812' method='POST'>\n";
  httpbuf += "Select Mode <select name='modedrop' onchange='this.form.submit();'>";

for (int k=0; k < numberofmodes; k++ ) {
    if (opState == k) { 
        selected = "' selected "; 
      } else selected = "' "; 
  httpbuf += "<option value='" + String(k) + selected + ">" + String(k) + ". " + MODE_STRING[k] + "</option>";
   // httpbuf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";

  }
  
  httpbuf += "</select>";
  httpbuf += "</form></p>";
  httpbuf += "<p><form action='/ws2812' method='POST'";
  httpbuf += "<p>Color: <input class='color' name='rgbpicker' value = '" + CurrentRGBcolour + "' >";
  httpbuf += "<br>  <input type='submit' value='Submit'/>"; 
  httpbuf += "</form>"; 
  httpbuf += "<form name=sliders action='/ws2812' method='POST'>\n";
  httpbuf += "<br>Animation: <input type='range' name='anispeed'min='0' max='10000' value='" + String(CurrentAnimationSpeed) + "' onchange='this.form.submit();' > ";
  httpbuf += "<br>Brightness: <input type='range' name='dim'min='0' max='255' value='" + String(CurrentBrightness) + "' onchange='this.form.submit();' > ";
  httpbuf += "<br>Timer: <input type='range' name='timer'min='0' max='2000' value='"+ String(WS2812interval)+ "' onchange='this.form.submit();'> ";
  //httpbuf += "<input type='submit' value='Submit'/>" ; 
  httpbuf += "</form>"; 
  httpbuf += "<p><form action='/ws2812' method='POST'";
  httpbuf += "<form action='/ws2812' method='POST'>";    
  httpbuf += "<p>LEDs: <input type='text' id='leds' name='leds' value='"+ String(pixelCount) + "' >";
  httpbuf += "<br>PIN: <input type='text' id='ledpin' name='ledpin' value='"+ String(pixelPIN) + "' >";
  httpbuf += "<br>  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form>"; 
  httpbuf += "Power = " + String(power) + "mA"; 
  httpbuf += "<br> Adalight order: GRB";
  httpbuf += htmlendstring; 
  
  server.send(200, "text/html", httpbuf);

if (updateLEDs) { initiateWS2812(); updateLEDs = false;};

power = getPixelPower();


}

void Animationspped_command_string (String Value) {

  uint16_t newvalue = Value.toInt();

  CurrentAnimationSpeed = newvalue; 
  lasteffectupdate = 0; 
}



// Takes MODE selected by number, changes OpState, SAVES NEW MODE TO EEPROM, incase of reboot....
void WS2812_mode_number(String Value) {

      lasteffectupdate = 0;
      uint8_t chosen_mode = Value.toInt();
      Serial.println("MODE DROP recieved: " + Value);
      opState = (operatingState)chosen_mode;
      if (chosen_mode != 0) LastOpState = (operatingState)chosen_mode;
      EEPROM.write(PixelCount_address + 3, LastOpState);
      EEPROM.commit();

}

//  might need to add an effect function timeout thing....  to reset effect and call update with NEW dim setting... 


void    WS2812_dim_string (String Value)
{
      lasteffectupdate = 0; // RESET EFFECT COUNTER, force refresh of effects....
      int a = Value.toInt();
      if (a > 255) a = 255;
      if (a < 0) a = 0;

      CurrentBrightness  = a;

      EEPROM.write(PixelCount_address + 2, CurrentBrightness);
      EEPROM.commit();
     
      // if (isnan(CurrentBrightness)) CurrentBrightness = 255;

      Serial.println("Brightness set to: " + String(CurrentBrightness));

}


void  WS2812_mode_string (String Value)

{

  lasteffectupdate = 0; // RESET EFFECT COUNTER
  Random_func_timeout = 0; //RESET additionall timeout... 

  if (Value.toInt() != 0) {

      // WS2812_mode_number(Value); 

      uint8_t chosen_mode = Value.toInt();
      opState = (operatingState)chosen_mode;
      if (chosen_mode != 0) LastOpState = (operatingState)chosen_mode;


  } else {


  Value.toLowerCase();

  if (Value == "off" | Value == "OFF") { if (opState != OFF) { LastOpState = opState; opState = OFF; } ;};
  if (Value == "on" | Value == "ON") opState = LastOpState;
  if (Value == "looparound") { opState = LOOPAROUND; LastOpState = LOOPAROUND;};
  if (Value == "pickrandom") { opState = PICKRANDOM; LastOpState = PICKRANDOM; } ;
  if (Value == "fadeinfadeout") { opState = FADEINFADEOUT; LastOpState = FADEINFADEOUT; }; 
  if (Value == "fade") { opState = FADE; LastOpState = FADE; }; 
  if (Value == "rainbow") { opState = RAINBOW; LastOpState = RAINBOW; }; 
  if (Value == "adalight") { LastOpState =  opState = ADALIGHT;}; 
  if (Value == "coolblobs") { opState = COOLBLOBS; LastOpState = COOLBLOBS;  }; 
  if (Value == "udp") { opState = UDP; LastOpState = UDP;  }; 
  if (Value == "colour") { opState = LastOpState = COLOR; }; 
  if (Value == "chaserainbow") { opState = LastOpState = ChaseRainbow; }; 
  if (Value == "test") {opState = LastOpState = TEST; };
  if (Value == "refresh" ) { 
    lasteffectupdate = 0;  
  }

  if (Value == "pause") paused = true;
  if (Value == "play") paused = false; 

  if (Value.indexOf("rgb") >= 0) 
    {
      //opState = LastOpState = COLOR; //  this allows you to pick colour base for other MODES.... 
      String instruction = Value.substring(4,Value.length()+1 );
      Serial.println("/n RGB command recieved: " + instruction);
      CurrentRGBcolour = instruction;
      NewColour = HEXtoRGB(instruction);


      EEPROM.write(PixelCount_address + 4, NewColour.R);
      EEPROM.write(PixelCount_address + 5, NewColour.G);
      EEPROM.write(PixelCount_address + 6, NewColour.B);

      EEPROM.commit();

      /* Serial.print(" New colours written to EEPROM...");

      Serial.print("Retrieved Colours are: ");
      
      uint8_t R = EEPROM.read(PixelCount_address + 4);
      uint8_t G = EEPROM.read(PixelCount_address + 5);
      uint8_t B = EEPROM.read(PixelCount_address + 6);
      Serial.print(R);  
      Serial.print(" ");
      Serial.print(G); 
      Serial.print(" ");
      Serial.println(B);
*/ 
      //SetRGBcolour(NewColour);

  }

}


  EEPROM.write(PixelCount_address + 3, LastOpState);

  EEPROM.commit();

}



void  WS2812timer_command_string (String Value)

{

lasteffectupdate = 0;

WS2812interval = Value.toInt();

}


//void  WS2812_command_string (String Value) {
//}
/*

if (Value.indexOf("rgb") >= 0) 
{
  opState = COLOR;
  String instruction = Value.substring(4,Value.length()+1 );
  Serial.println("RGB command recieved: " + instruction);
  NewColour = HEXtoRGB(instruction);
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {
      //strip->LinearFadePixelColor(1000, pixel, NewColour);
    }
}

} */




RgbColor dim(RgbColor value) {

  //  int amounttodarken = 255 - CurrentBrightness;
  //  value.Darken(amounttodarken);
  //  return value;

    if (CurrentBrightness == 255) return value; 

     RgbColor newvalue = dimbyhsv(value, (byte) CurrentBrightness);

  /* Serial.println();
  Serial.print("(Dim = ");
  Serial.print(CurrentBrightness);
  Serial.print(")");
  Serial.print(" R:");
  Serial.print(value.R);
  Serial.print(" --> ");
  Serial.print(" R:");
  Serial.print(newvalue.R);

  Serial.print(" : "); 

  Serial.print(" G:");
  Serial.print(value.G);
  Serial.print(" --> ");
  Serial.print(" G:");
  Serial.print(newvalue.G);

  Serial.print(" : "); 

  Serial.print(" B:");
  Serial.print(value.B);
  Serial.print(" --> ");
  Serial.print(" B:");
  Serial.print(newvalue.B);

*/ 




return(newvalue);
}




void SetRGBcolour (RgbColor value) {

SetRGBcolour(value,CurrentAnimationSpeed); 

}

void SetRGBcolour (RgbColor value, uint16_t speed) {
    
  value = dim(value);
  
      if (!(strip->IsAnimating())) {

          for (uint16_t pixel = 0; pixel < pixelCount; pixel++) {
       // strip->SetPixelColor(pixel,value);
          strip->LinearFadePixelColor(speed, pixel, value);
    }
    strip->StartAnimating(); // start animations
  }


}

RgbColor  HEXtoRGB (String hexString) // Converts HEX to RBG object....

{

  uint32_t decValue = 0;
  int nextInt = 0;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
    int r = decValue >> 16;
    int g = decValue >> 8 & 0xFF;
    int b = decValue & 0xFF;

    return RgbColor(r,g,b);

}


String RGBtoHEX (RgbColor value) {

  //char R[3]
 //sprintf( R, "%X", value );





}

void StripOFF() {

  //if (millis() > (lasteffectupdate + WS2812interval) ){
  lasteffectupdate = 0; 

  memset(pixelsPOINT, 0, 3 * strip->PixelCount() ); 

  strip->Dirty();


//lasteffectupdate = millis();

//}

 /* for (uint16_t i = 0; i < pixelCount; i++)
  {
    strip->SetPixelColor(i,RgbColor(0,0,0));
  } */
  

 // memset(pixelsPOINT, 0, 3 * strip->PixelCount() ); 

 // clearpixels();

//strip->Dirty();

}



void  initiateWS2812 ()

{
  opState = OFF;
  ChangeNeoPixels(pixelCount, pixelPIN); // initial setup
  strip->Begin();
  StripOFF();
  SetRandomSeed();

}



void ws2812 ()  // put switch cases here...


{
static unsigned long update_strip_time = 0;  //  keeps track of pixel refresh rate... limits updates to 33 Hrz.....

  //Serial.print("ws2812 called case = " + String(opState));
  //wsPoint += 1;

switch (opState)
   {
   case OFF:
      //if (strip) StripOFF();
   StripOFF();
      break;
   case RAINBOW:
      rainbow();
      break;
   case COLOR:
      SetRGBcolour(NewColour);
      break;
   case ChaseRainbow:
      theatreChaseRainbow();
      break;
   case FADE:
      Fade();
      break;
   case ADALIGHT:
      Adalight();
      break;
   case TEST:
      test();
      break;
   case LOOPAROUND:
      LoopAround(192, 200);
      break;
   case PICKRANDOM:
      PickRandom(128);
      break;
   case FADEINFADEOUT:
      FadeInFadeOutRinseRepeat(192);
      break;
   case COOLBLOBS:
      CoolBlobs();
      break;    
    case UDP:
      UDPfunc();
      break;
    case RAINBOWCYCLE:
      Rainbowcycle();
      break;
    case SPIRAL:
      spiral();
      break;
    case SQUARES2:
      Squares2(0);
      break;
    case SQUARESRANDOM:
      Squares2(1);
      break;
    case TEST4:
      test4();
      break;
    case SQUARES:
      Squares();
      break;
    case EQ1:
      eq1();
      break;
    case RANDOM:
      Random_colour();
      break;
    case RANDOMFUNC:
      Random_function();
      break;
    case ARTNET:
      Art_Net_func ();
      break;
    case RANDOM_TOP_BOTTOM:
      Random_Top_Bottom(0);
      break;
    case RANDOM_TOP_BOTTOM_LINEAR:
      Random_Top_Bottom(1);
      break;    
    case SINGLE_COLOUR_FADE:
      Random_Top_Bottom(2);
      break;  
    case RANDOM_COLOUR_FADE:
      Random_Top_Bottom(3);
      break;      
    case HSICYCLE:
      HSI_Cycle();
      break;
   }


if (millis() > update_strip_time + 30) {
    if (strip->IsAnimating()) strip->UpdateAnimations(); 
    strip->Show();
    update_strip_time = millis();

} // end of switch case...

/*
static bool ani_update = false;

if (!ani_update && strip->IsAnimating()) {
  Serial.print("Animating...");
  ani_update = true;
} else if (ani_update && !strip->IsAnimating()) {
  Serial.println("Stopped");
  ani_update = false;
}
*/


if (paused) lasteffectupdate = millis() + 1000; 


} // end of ws2812 function 


void HSI_Cycle() {


static int j;

if (millis() > (lasteffectupdate) ){

  //Serial.println("Fade updaed");

if (j > 360) j = 0; 

double j_double = (double)j / 360; 
double v_double = (double)CurrentBrightness / 255; 
double s_double; 

//RgbColor col = hsvToRgb(j_double,1,v_double);

/*  Serial.println();
  Serial.print(j_double);
  Serial.print("-->");
  Serial.print(" R:");
  Serial.print(col.R);
  Serial.print(" G:");
  Serial.print(col.G);
  Serial.print(" B:");
  Serial.print(col.B);  
*/

//int col = 200; 

for (int i=0; i < pixelCount; i++) {

double s_double = (double)i / pixelCount; 


RgbColor col = hsvToRgb(j_double,s_double,v_double);

    strip->SetPixelColor(i, col);   
        }

lasteffectupdate = millis() + WS2812interval ;

j++; 

}





}




void Random_function() {

static uint8_t random_choice = 0 ; 
static uint32_t Random_func_next_time = 0;

if (millis() > Random_func_next_time + 1000) Random_func_next_time = 0; // This resets things if its been turned off after 1s... 

    if (millis() > Random_func_next_time) {

    random_choice = random(0, 5);

    Random_func_next_time = millis() + random(WS2812interval*30, (WS2812interval * 300) ); 

    }


    if (random_choice == 0) rainbow();
    if (random_choice == 1) spiral();
    if (random_choice == 2) Rainbowcycle();
    if (random_choice == 3) Random_colour();
    if (random_choice == 4) Squares2(1);
    if (random_choice == 5) Random_Top_Bottom(0);




}

void Random_colour() {

 //long Random_func_timeout = 0, Random_func_lasttime = 0; 
static uint8_t current_r = 0;

    if (millis() > lasteffectupdate) {
      //Serial.print("New random choice..."); 

      uint16_t random_animation_speed = random(2000, 20000);

      //RgbColor random_colour_random = RgbColor(random(255),random(255),random(255));  // generates lots of white colours... not balenced..
      RgbColor random_colour_random = Wheel(random(255));

      SetRGBcolour(random_colour_random,random_animation_speed);

      //Random_func_lasttime = millis(); 
      long Random_func_timeout = random(60000, 60000*5);
      //Random_func_timeout = random(10000, 60000);
      lasteffectupdate = millis() + Random_func_timeout; 
    }






} // end of random func


void  CoolBlobs() {

    if (millis() > (lasteffectupdate ) ){
      




      lasteffectupdate = millis() + WS2812interval; 
    } // end of timer if

} // end of coolblobs


//////// i nserted
/*

void setcolour () {

    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {

      //strip->LinearFadePixelColor(5000, pixel, NewColour);

    }

}

*/

void   FadeInFadeOutRinseRepeat(uint8_t peak) {
  if (effectState == 0)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(800,100);
      strip->LinearFadePixelColor(time, pixel, RgbColor(random(peak), random(peak), random(peak)));
    }
  }
  else if (effectState == 1)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(600,700);
      strip->LinearFadePixelColor(time, pixel, RgbColor(0, 0, 0));
    }
  }
  effectState = (effectState + 1) % 2; // next effectState and keep within the number of effectStates
  
}

void  PickRandom(uint8_t peak)
{

  // pick random set of pixels to animate
  uint8_t count = random(pixelCount);
  while (count > 0)
  {
    uint8_t pixel = random(pixelCount);
    
    // configure the animations
    RgbColor color = Wheel(random(255)); // = //strip->getPixelColor(pixel);

    //color = RgbColor(random(peak), random(peak), random(peak));

    
    uint16_t time = random(100,400);
    strip->LinearFadePixelColor( time, pixel, color);
    
    count--;
  }
}

void  LoopAround(uint8_t peak, uint16_t speed)
{
  // Looping around the ring sample
  uint16_t prevPixel;
  RgbColor prevColor;
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 5)) % pixelCount; 
  strip->LinearFadePixelColor(speed, prevPixel, RgbColor(0, 0, 0));
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 4)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 3)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 2)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectState + (pixelCount - 1)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade current one light
  strip->LinearFadePixelColor(speed, effectState, RgbColor(random(peak), random(peak), random(peak)));
  effectState = (effectState + 1) % pixelCount;
  
}

void  SetRandomSeed()
{
  uint32_t seed;
  
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);
  
  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }
  
  // Serial.println(seed);
  randomSeed(seed);
}


RgbColor  Wheel (byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return  RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
   WheelPos -= 85;    
   return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return  RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
} 


void  Fade() {


static int j;
if (millis() > (lasteffectupdate ) ){

  //Serial.println("Fade updaed");
if (j > 256) j = 0; 

RgbColor col = Wheel(j);
//int col = 200; 

for (int i=0; i < pixelCount; i++) {
    strip->SetPixelColor(i, dim(col));    //turn every third pixel on
        }


j++;
lasteffectupdate = millis() + WS2812interval;

}

}



void  Rainbowcycle() {

   static int wsPoint ;
 
 if (millis() > (lasteffectupdate ) ){


  uint16_t i; // , j;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel


    for(i=0; i< pixelCount; i++) {
      strip->SetPixelColor(i, dim(Wheel(i * 256 / pixelCount + wsPoint)));
    }

    if (wsPoint==256*5) wsPoint=0; 
    wsPoint++;
    lasteffectupdate = millis() + WS2812interval;
    
  //}
}









}



void  test4() {

//Serial.println("TEST4");
  static int wsPoint = 0;
  RgbColor color1 = Wheel(100);
  RgbColor color2 = Wheel(200);

if (wsPoint == 255) wsPoint = 0; 
 if (millis() > (lasteffectupdate ) ){





HsvFADErgb(color1,color2,wsPoint); 



lasteffectupdate = millis() + WS2812interval ;
wsPoint++; 
}


}

void  test() {


 if (millis() > (lasteffectupdate) ){

  static int wsPoint = 0;

    if (var3 ==0) var3 = 256;  // safety for map.....

    for(int i=0; i<pixelCount; i++) {
    //RgbColor tempcolour = Wheel(i+wsPoint);
    int8_t point = map(i+wsPoint, 0, 256, var2, var3);

    strip->SetPixelColor(i, dim(Wheel(point)));
    }
      if (wsPoint==256) wsPoint=0; 
    wsPoint++;
    lasteffectupdate = millis() + WS2812interval;
}



}

 void  rainbow() {
// Var 1 = timer interval
// Var 2 = wsPoint Min
// Var 3 = wsPoint Max
// Var 4 = wsPoint step....  
// Var 5 = pixel jump?  ie... instead of i++, do i =+ var 3... ? might need to turn of previous pixel
// Var 6 = wheel multipler... or wheel map..  (wheel takes value.. returns color...)
// try to constrain wspoint which is color generator... 
 if(var3 == 0) var3 = 256; // MAKE sure that there is a +ve end point for the colour wheel... else wsPoint = var3;

 if (millis() > (lasteffectupdate) ){

  static int wsPoint =0;
  //uint16_t i; // , j;
  //pixelsNUM = 60;
  //for(j=0; j<256; j++) { v
    for(int i=0; i<pixelCount; i++) {
    //RgbColor tempcolour = Wheel(i+wsPoint);
    strip->SetPixelColor(i, dim(Wheel(i+wsPoint)));
    }
    
    if (wsPoint==var3) wsPoint=var2; 
    wsPoint++;
    lasteffectupdate = millis() + WS2812interval ;
}
    // Serial.println("Colours Updated..." + String(//strip->numPixels()));

}   // END OF RAINBOW

void clearpixels() {

    memset(pixelsPOINT, 0, 3 * strip->PixelCount() );   // clear current without... 


}

void  spiral() {

static uint16_t currentcolor = 0;

  if (millis() > (lasteffectupdate ) ) 
  { // effect update timer
  uint8_t pitch = 0 ;
  
  if (var7 == 0 ) pitch = 13; else pitch = var7;
  uint8_t total_y = return_total_y(pitch); // get the number of rows.  rounds up...
  uint8_t x,y;

 clearpixels();

    for (x = 0; x < pitch; x+=2) {
      RgbColor colour = Wheel(( x * 256 / pitch + currentcolor) ); // i * 256 / pixelCount + wsPoint) 
      for (y = 0; y < total_y; y++) {
      strip->SetPixelColor(return_pixel( x, y, pitch), dim(colour));
      }
    }
  

  if (currentcolor == 256) currentcolor = 0; else currentcolor++ ;// cycle through to next colour
  lasteffectupdate = millis() + WS2812interval ;

  } // effect update timer

}

// FACES ALGO....
void  test3 () {

  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......


  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 5;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {

      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {

      RgbColor colour = RgbColor(random(255),random(255),random(255));

      if (square_size == 10) colour.Darken(random(0,200));

      if (square_size == 10) square_size = random(3,7);


      uint8_t x_rand = random(0, total_x - square_size ) ; 
      uint8_t y_rand = random(0, total_y - square_size ) ;

         


      // strip->LinearFadePixelColor(time, return_shape_square(x_rand, y_rand, 4, square_size, total_y ) , colour);


        

        //Serial.println();

      for (int j =0; j < (square_size * square_size) ; j++) {


        //Serial.print("GRID " + String (square_size * square_size) + "... ");

        int pixel1 = return_shape_face(x_rand, y_rand, j, square_size, total_x ); 

        //Serial.print(pixel1);
        //Serial.print(" ");

        if (pixel1 <= pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));
      
      }


    }

    //Serial.println("------------------");

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }





} // end of test


// Working 


void  Squares2 (uint8_t mode) { // WORKING RANDOM SQUARE SIZES...

  

  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......


  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 5;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



    if ( (millis() > lasteffectupdate ) && (!strip->IsAnimating()) ) {

      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }


      for (int i = 0; i < numberofpoints; i++) {

      RgbColor colour = Wheel(random(255)); // RgbColor(random(255),random(255),random(255));

      //if (square_size == 10) colour.Darken(random(0,50)); // OLD METHOD
      if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(2,7);


      uint8_t x_rand = random(0, total_x - square_size ) ; 
      uint8_t y_rand = random(0, total_y - square_size ) ;


      for (int j =0; j < (square_size * square_size) ; j++) {


        //Serial.print("GRID " + String (square_size * square_size) + "... ");

        int pixel1 = return_shape_square(x_rand, y_rand, j, square_size, total_x ); 

        if (pixel1 < pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));
      
     }


    }

    //Serial.println("------------------");

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;  // add time so that next sequence starts AFTER animations have finished...
    strip->StartAnimating(); // start animations

    }


} 
// end of Squares2



void  Squares () {

  static int wsPoint = 0;
  
  if (var8 == 0) var8 = 5;
  if (var7 == 0) var7 = 13;  

  uint8_t numberofpoints = var8; // default 5
  uint8_t x,y;
  uint8_t total_y = return_total_y(var7); 
  uint8_t total_x = var7; 


    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {
      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        //strip->SetPixelColor(i,0);
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {


      uint8_t x_rand = random(1, total_x-2) ; 
      uint8_t y_rand = random(1, total_y-2) ;

      RgbColor colour = Wheel(random(255)) ; //  RgbColor(random(255),random(255),random(255));

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand,y_rand,total_x), dim(colour));

      colour.Darken(50);

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand - 1 , total_x     ), dim(colour));       


    }

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }


} // end of Squares






void theatreChaseRainbow() {
  static uint16_t colourpoint = 0; 
  static uint8_t animationstate = 0 ; 
  if (millis() > (lasteffectupdate ) ) {


  //for (int j=0; j < 256; j++) {       // cycle all 256 colors in the wheel

        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+animationstate, dim(Wheel(i+colourpoint)));    //turn every third pixel on
        }


        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+ animationstate - 1, 0);    //turn every third pixel on
        }
        animationstate++; 

        if (animationstate == 3) animationstate = 0; // reset animation state. 


  
    colourpoint++; 
    lasteffectupdate = millis() + WS2812interval ;
  }  //  end of timer..... 

}



void Adalight_Flash() {

    for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(255,0,0));
          }

    strip->Show(); 


    delay(200);

        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,255,0));
          }
        strip->Show(); 

    delay(200);

        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,0,255));
          }
        strip->Show(); 

    delay(200);
    StripOFF();

  }


void Adalight () {    //  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

  static boolean Adalight_configured;
  static uint16_t effectbuf_position = 0;
  enum mode { MODE_INITIALISE = 0, MODE_HEADER, MODE_CHECKSUM, MODE_DATA, MODE_SHOW, MODE_WAIT};
  static mode state = MODE_INITIALISE;
  static int effect_timeout = 0;
  static uint8_t prefixcount = 0;
  static unsigned long ada_sent = 0; 
  static unsigned long pixellatchtime = 0;
  const unsigned long serialTimeout = 15000; // turns LEDs of if nothing recieved for 15 seconds..
  const unsigned long initializetimeout = 10000; 
  static unsigned long initializetime = 0; 
    //if (!Adalight_configured) {
    //    Adalight_Flash(); 
    //    Adalight_configured = true;
    //}

    if (pixellatchtime > 0 && (pixellatchtime + serialTimeout) < millis()) {
      pixellatchtime = 0; // reset counter / latch to zero should only fire when show pixls has been called!
      StripOFF();  // turn off pixels... 
      state = MODE_HEADER;  // resume header search....
    }

    if(millis() > initializetime + initializetimeout) state = MODE_INITIALISE; // this goes to initiaase mode...

  switch (state) {

    case MODE_INITIALISE:

      Adalight_Flash(); 
      state = MODE_HEADER;

    case MODE_HEADER:

      effectbuf_position = 0; // reset the buffer position for DATA collection...

          if(Serial.available()) { // if there is serial available... process it... could be 1  could be 100....
               
            for (int i = 0; i < Serial.available(); i++) {  // go through every character in serial buffer looking for prefix...

              if (Serial.read() == prefix[prefixcount]) { // if character is found... then look for next...
                  prefixcount++;
              } else prefixcount = 0;  //  otherwise reset....  ////

            if (prefixcount == 3) {
            effect_timeout = millis(); // generates START TIME.....
            state = MODE_CHECKSUM;
            prefixcount =0;
            break; 
            } // end of if prefix == 3
            } // end of for loop going through serial....
            } else if (!Serial.available() && (ada_sent + 5000) < millis()) {
                  Serial.print("Ada\n"); // Send "Magic Word" string to host
                  ada_sent = millis(); 
            } // end of serial available....

    break;

    case MODE_CHECKSUM:

        if (Serial.available() >= 3) {
          hi  = Serial.read();
          lo  = Serial.read();
          chk = Serial.read();
          if(chk == (hi ^ lo ^ 0x55)) {
            state = MODE_DATA;
          } else {
            state = MODE_HEADER; // ELSE RESET.......
          }
        }

      if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.

      break;

    case MODE_DATA:

        //  this bit is what might... be causing the flashing... as it extends past memory stuctures....
        while (Serial.available() && effectbuf_position < 3 * strip->PixelCount()) {  // was <=  
          pixelsPOINT[effectbuf_position++] = Serial.read();
          //if (effectbuf_position == 3 * strip->PixelCount()) break; 
        }

      if (effectbuf_position >= 3*pixelCount) { // goto show when buffer has recieved enough data...
        state = MODE_SHOW;
        break;
      } 

        if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.


      break;

    case MODE_SHOW:

      strip->Dirty(); // MUST USE if you're using the direct buffer copy... 
      pixellatchtime = millis();
      state = MODE_HEADER;
      break;


}

      //strip->Show();


 initializetime = millis(); // this resets the timer 

}


// called to change the number of pixels
void ChangeNeoPixels(uint16_t count, uint8_t pin)  {
  
 bool commitchanges = false; 

    //Serial.println("Change Neopixels called"); 


        int pixelPINstored = EEPROM.read(PixelPIN_address);    
        int pixelCountstored = EEPROM.read(PixelCount_address);
        //int b=EEPROM.read(PixelCount_address+1);
        //nt pixelCountstored = a*256+b;


    if (count != pixelCountstored) {
    //Serial.println("Pixel count changed..."); 

      //int a = pixelCount/256;
      //Serial.print(a);
      //int b = pixelCount % 256;
      //Serial.print(b);
        //EEPROM.write(PixelCount_address,(byte)count);
        //EEPROM.write(PixelCount_address+1,b);


      int a = pixelCount/256;
      int b = pixelCount % 256;        
      
        EEPROM.write(PixelCount_address,a);
        EEPROM.write(PixelCount_address+1,b);

        commitchanges = true;

    if (EEPROM.read(PixelCount_enablebyte) != flagvalue) EEPROM.write(PixelCount_enablebyte,flagvalue) ;
     //Serial.println("pixel count byte updated");
    }

    if (pin != pixelPINstored) {
    //Serial.println("Change Neopixels PIN called"); 

        EEPROM.write(PixelPIN_address, (byte)pin);
    if (EEPROM.read(PixelPIN_enablebyte) != flagvalue) EEPROM.write(PixelPIN_enablebyte,flagvalue) ;
     
     commitchanges = true;

    }

    if (commitchanges == true) {
      EEPROM.commit();
          Serial.println("WS2812 Settings Updated."); 
        }


    if (strip)
    {
        //StripOFF();

        delete strip;
    }

    strip = new NeoPixelBus(count, pin);
    pixelsPOINT = (uint8_t*)strip->Pixels(); ///  used for direct access to pixelbus buffer...
    if (strip->PixelCount() < 10) var7 = 1;


}

void UDPfunc () {

static boolean Adalight_configured = false;

 if (!Adalight_configured) {
    Serial.println("UDP mode enabled\n"); // Send "Magic Word" string to host
    Adalight_configured = true;
    } 

int packetSize = Udp.parsePacket();

  if(Udp.available())
  {
        Serial.println("UDP packet recieved...");

    RgbColor col = RgbColor(0,0,0);
    int currentpixel = 0;
    for (int i = 0; i < packetSize; i = i + 3) {
      if (currentpixel > pixelCount) break;
        col.R = Udp.read();
        col.G = Udp.read();
        col.B = Udp.read();
        strip->SetPixelColor(currentpixel,col);
        currentpixel++;
      }

strip->Show();

}



}

int getPixelPower () {
 int brightnesstally = 0;
 for (int i = 0;i < pixelCount; i++) {
  RgbColor colour = strip->GetPixelColor(i);
  int brightness = colour.CalculateBrightness();
  brightness = map(brightness,0,255,0,60);
  brightnesstally = brightnesstally + brightness;
}

int brightness = brightnesstally;

return brightness;
} 





void handle_lights_config() {

   if (server.args() != 0) lasteffectupdate = 0;    
   if (server.arg("var1").length() != 0) var1 = server.arg("var1").toInt();
   if (server.arg("var2").length() != 0) var2 = server.arg("var2").toInt(); // colour point min
   if (server.arg("var3").length() != 0) var3 = server.arg("var3").toInt(); // colour point max
   if (server.arg("var4").length() != 0) var4 = server.arg("var4").toInt();
   if (server.arg("var5").length() != 0) var5 = server.arg("var5").toInt();
   if (server.arg("var6").length() != 0) var6 = server.arg("var6").toInt();
   if (server.arg("var7").length() != 0) var7 = server.arg("var7").toInt();
   if (server.arg("var8").length() != 0) var8 = server.arg("var8").toInt();
   if (server.arg("var9").length() != 0) var9 = server.arg("var9").toInt();
   if (server.arg("var10").length() != 0) var10 = server.arg("var10").toInt();

   // String a = ESP.getFlashChipSizeByChipId(); 
   if (server.arg("reset") == "true") { var1 = 0; var2 = 0; var3 = 0; var4 = 0; var5 = 0; var6 = 0; var7 = 0; var8 = 0; var9 = 0; var10 = 0;};




  httpbuf = "<!DOCTYPE HTML>\n<html><body bgcolor='#E6E6FA'><head> <meta name ='viewport' content = 'width = device-width' content='text/html; charset=utf-8'>\n<title>" + String(deviceid) + "</title></head>\n<body><h1> " + String(deviceid) + " </h1>\n";   
  httpbuf += " <br> <a href='/lightsconfig?reset=true'>RESET TO DEFAULTS</a>  "; 
  httpbuf += "<form name=form action='/lightsconfig' method='POST'>\n";
  //httpbuf += "Select Mode <select name='modedrop' onchange='this.form.submit();'>";

  for (int k=0; k < 10; k++ ) {
  //httpbuf += "<option value='" + String(k) + selected + ">" + String(MODE_STRING[k]) + "</option>";
   // httpbuf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";
    httpbuf += String(VAR_STRING[k]) + " : <input type='text' id='var" + String(k+1) + "' name='var" + String(k+1) + "' value=''><br>";
  }

  httpbuf += "  <input type='submit' value='Submit'/>" ; 
  httpbuf += "</form></p>"; 
  httpbuf += htmlendstring; 

  server.send(200, "text/html", httpbuf);



}



uint16_t return_pixel(uint8_t x, uint8_t y, uint8_t pitch) {
  uint16_t a = (pitch * y) + x; 
  return a; 
}


uint8_t return_total_y(uint8_t pitch) {
 uint8_t total_y = pixelCount / pitch; 
 float remainder = pixelCount % pitch;
 if (remainder > 0) total_y++;
  return total_y;

}

//  Returns a pixel number... starting in bottom left...
uint16_t return_shape_square(uint8_t first_pixel_x, uint8_t first_pixel_y , uint8_t desired_pixel, uint8_t grid_size, uint8_t total_in_row) {

  uint16_t pixel; 
  uint8_t pixel_x, pixel_y, row, row_left ;
  row = desired_pixel / grid_size; 
  row_left = desired_pixel % grid_size;
  pixel_y = first_pixel_y + row;  
  pixel_x = first_pixel_x + row_left; 
  pixel = return_pixel(pixel_x, pixel_y, total_in_row);
  return pixel; 
}


uint16_t return_shape_ring(uint8_t first_pixel_x, uint8_t first_pixel_y , uint8_t desired_pixel, uint8_t grid_size, uint8_t total_in_row) {

//
//
//
//                                
//      O         OO          OO  

}

uint16_t return_shape_x(uint8_t first_pixel_x, uint8_t first_pixel_y , uint8_t desired_pixel, uint8_t grid_size, uint8_t total_in_row) {




}

uint16_t return_shape_face(uint8_t first_pixel_x, uint8_t first_pixel_y , uint8_t desired_pixel, uint8_t grid_size, uint8_t total_in_row) {
//                                     7
//                           6
//                  5               00   00
//          4              00  00   00   00
//   3            0   0                0    
//        0  0      0        00       000 
//  0 0    00       0            
//   0                     0    0   0     0
//  000   0000    00000    000000   0000000


  uint16_t pixel = 0; 
  uint8_t pixel_x = 0, pixel_y = 0 ;

//  GRID = 3 : 1  2  3  4  5  6  pixels 

    if (grid_size == 3) {
    if (desired_pixel == 1) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 2) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 3) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 4) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 1;};
    if (desired_pixel == 5) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 2;};
    if (desired_pixel == 6) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 2;};
    }
//  GRID = 4 : 1  2  3  4  5  6 7 8   pixels 
    if (grid_size == 4) {
    if (desired_pixel == 1) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 2) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 3) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 4) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 5) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 2;};
    if (desired_pixel == 6) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 2;};
    if (desired_pixel == 7) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 3;};
    if (desired_pixel == 8) { pixel_x = first_pixel_x + 3; pixel_y = first_pixel_y + 3;};
    }
//  GRID = 5 : 1  2  3  4  5  6 7 8 9  pixels 

    if (grid_size == 5) {
    if (desired_pixel == 1) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 2) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 3) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 4) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 5) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 0;};

    if (desired_pixel == 6) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 2;};
    if (desired_pixel == 7) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 3;};

    if (desired_pixel == 8) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 4;};
    if (desired_pixel == 9) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 4;};

    }
//  GRID = 6 : 14   pixels 

    if (grid_size == 6) {
    if (desired_pixel == 1) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 2) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 3) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 4) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 5) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 6) { pixel_x = first_pixel_x + 6; pixel_y = first_pixel_y + 0;};

    // mouth corners
    if (desired_pixel == 7) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 1;};
    if (desired_pixel == 8) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 1;};

    // nose
    if (desired_pixel == 9)  { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 3;};
    if (desired_pixel == 10) { pixel_x = first_pixel_x + 3; pixel_y = first_pixel_y + 3;};
    //eyes
    if (desired_pixel == 11) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 5;};
    if (desired_pixel == 12) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 5;};    
    if (desired_pixel == 13) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 5;};
    if (desired_pixel == 14) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 5;};  
    }
//  GRID = 7 : 21 pixels...
    if (grid_size == 7) {
    if (desired_pixel == 1) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 2) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 3) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 4) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 5) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 6) { pixel_x = first_pixel_x + 6; pixel_y = first_pixel_y + 0;};
    if (desired_pixel == 7) { pixel_x = first_pixel_x + 7; pixel_y = first_pixel_y + 0;};

    // mouth corners
    if (desired_pixel == 8) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 1;};
    if (desired_pixel == 9) { pixel_x = first_pixel_x + 6; pixel_y = first_pixel_y + 1;};

    // nose
    if (desired_pixel == 10) { pixel_x = first_pixel_x + 2; pixel_y = first_pixel_y + 3;};
    if (desired_pixel == 11) { pixel_x = first_pixel_x + 3; pixel_y = first_pixel_y + 3;};
    if (desired_pixel == 12) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 3;};
    if (desired_pixel == 13) { pixel_x = first_pixel_x + 3; pixel_y = first_pixel_y + 4;};


    //eyes
    if (desired_pixel == 14) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 5;};
    if (desired_pixel == 15) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 5;};    
    if (desired_pixel == 16) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 5;};
    if (desired_pixel == 17) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 5;};  
    if (desired_pixel == 18) { pixel_x = first_pixel_x + 0; pixel_y = first_pixel_y + 6;};
    if (desired_pixel == 19) { pixel_x = first_pixel_x + 1; pixel_y = first_pixel_y + 6;};    
    if (desired_pixel == 20) { pixel_x = first_pixel_x + 4; pixel_y = first_pixel_y + 6;};
    if (desired_pixel == 21) { pixel_x = first_pixel_x + 5; pixel_y = first_pixel_y + 6;}; 


    }


  pixel = return_pixel(pixel_x, pixel_y, total_in_row);
  return pixel; 
}

// function to shift pixels in blocks one way or another...

void pixelshift(uint16_t start, uint16_t end) {

  pixelshift( start, end,100); 
}


void pixelshift(uint16_t start, uint16_t end, uint16_t pixelshift_timer) {

static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
//uint16_t pixelshift_timer = 100; 

if (millis() > (last_pixelshift + pixelshift_timer)) {

  if (end > start) { // move direction right

    for (uint16_t i = end; i > start ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);

    }
  }   else if (start > end) {  // Move direction = left

    for (uint16_t i = end; i < start; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);

    }

  }
  strip->SetPixelColor(start, 0);
  last_pixelshift = millis(); 
}


}


//// function to shift pixels in blocks one way or another...
void pixelshift_middle() {


pixelshift_middle(100); 

}


void pixelshift_middle(uint16_t pixelshift_timer) {

static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
uint16_t middle = strip->PixelCount() / 2; 
uint8_t remainderodd = 0; 
if  (strip->PixelCount() % 2 != 0) remainderodd = 1 ; 



if (millis() > (last_pixelshift + pixelshift_timer)) {


    for (uint16_t i = 0; i < middle ; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);
    }

    for (uint16_t i = strip->PixelCount(); i > middle + 1 ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);
    } 

  
  strip->SetPixelColor(middle, 0);
  if (remainderodd != 0) strip->SetPixelColor(middle+1, 0);


  last_pixelshift = millis(); 
}


}
 // END of pixel shift




void eq1 () {

  if (millis() > lasteffectupdate  )  {

      strip->SetPixelColor(3, RgbColor(255,0,0));
      strip->SetPixelColor(4, RgbColor(0,255,0));
      //strip->SetPixelColor(4, RgbColor(0,0,255));
      lasteffectupdate = millis() + WS2812interval;
  } 
          
uint8_t direction = random(0,2);

if (direction == 0 ) {

pixelshift(6,0);

} else if (direction == 1) {

pixelshift(0,6);

} else if (direction == 2) {

pixelshift_middle(); 


};
 
// pixelshift(6,0);


}



///////////////////
//
//
//      ART - NET  
//
//
///////////////////

  uint8_t artnetPacket[MAX_BUFFER_ARTNET];
  uint16_t packetSize;
  uint16_t opcode;
  uint8_t sequence;
  uint16_t incomingUniverse;
  uint16_t dmxDataLength;
  void (*artDmxCallback)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);




uint16_t Artnetread()
{
  packetSize = Udp.parsePacket();
  
  if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0)
  { 
      Udp.read(artnetPacket, MAX_BUFFER_ARTNET);

      // Check that packetID is "Art-Net" else ignore
      for (byte i = 0 ; i < 9 ; i++)
      {
        if (artnetPacket[i] != ART_NET_ID[i])
          return 0;
      }
        
      opcode = artnetPacket[8] | artnetPacket[9] << 8; 

      if (opcode == ART_DMX)
      {
        sequence = artnetPacket[12];
        incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;  
        dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;

        if (artDmxCallback) (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
        return ART_DMX;
      }
      if (opcode == ART_POLL)
      {
        return ART_POLL; 
      }
  }
  else
  {
    return 0;
  }
}

void printPacketHeader()
{
  Serial.print("packet size = ");
  Serial.print(packetSize);
  Serial.print("\topcode = ");
  Serial.print(opcode, HEX);
  Serial.print("\tuniverse number = ");
  Serial.print(incomingUniverse);
  Serial.print("\tdata length = ");
  Serial.print(dmxDataLength);
  Serial.print("\tsequence n0. = ");
  Serial.println(sequence);
}

void printPacketContent()
{
  for (uint16_t i = ART_DMX_START ; i < dmxDataLength ; i++){
    Serial.print(artnetPacket[i], DEC);
    Serial.print("  ");
  }
  Serial.println('\n');
}


void Art_Net_func () {

static boolean Adalight_configured;

 if (!Adalight_configured) {
    Serial.println("ART - NET mode enabled\n"); // Send "Magic Word" string to host
    Adalight_configured = true;
    } 


int test = Artnetread();

if (test == ART_DMX) Serial.println("DMX YES");



//Serial.println(test);
//printPacketContent();


/*
int packetSize = Udp.parsePacket();

  if(Udp.available())
  {
        Serial.println("UDP packet recieved...");

    RgbColor col;
    int currentpixel = 0;
    for (int i = 0; i < packetSize; i = i + 3) {
      if (currentpixel > pixelCount) break;
        col.R = Udp.read();
        col.G = Udp.read();
        col.B = Udp.read();
        strip->SetPixelColor(currentpixel,col);
        currentpixel++;
      }

strip->Show();

}

*/

}

// Overloaded func for topbottom fade...
void top_bottom_fade( RgbColor Top, RgbColor Bottom, uint8_t count_x) {

  top_bottom_fade(Top,Bottom,count_x,0);
}


void top_bottom_fade( RgbColor Top, RgbColor Bottom, uint8_t count_x, uint8_t fadetype) {

uint8_t x,y,colour_index; 
uint8_t total_y = return_total_y(count_x); // get total number of rows
RgbColor colournew;

for (y = 0; y < total_y; y++) {

  colour_index = map(y,1,total_y-1,1,254);  // map steps of blend.........

if (y == 0) colour_index = 0; 
if (y == total_y - 1) colour_index = 255;

        if (var1 == 0) var1 = 20;
        if (var2 == 0) var2 = 20; 
  
    if (fadetype == 1 ) { 
      colournew = colournew.LinearBlend(Bottom, Top, colour_index); 
      } else if (fadetype == 2) {
        colournew = Hsv_COLOR_range_rgb(NewColour, var1,var2,colour_index);  // give fade from HSV Hue 50 deg below and 50 deg above
      } else if (fadetype == 3) {

        colournew = Hsv_COLOR_range_rgb(Top, var1,var2,colour_index);  
      } else {       
        colournew = HsvFADErgb(Bottom,Top,colour_index);
      } // generate new colour.... based on value passed in fade type.



    for( x = 0; x < count_x; x++) {
        uint16_t pixel = return_pixel(x,y, count_x); 

        if (pixel >= strip->PixelCount()) break;
        //Serial.print(pixel);
        //Serial.print(",");
        //strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand,y_rand,total_x), dim(colour));
        strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel , dim(colournew)); //CurrentAnimationSpeed
        
        //strip->SetPixelColor( pixel, colournew);

    }
  
  //Serial.print(")  ");

}
    
strip->StartAnimating(); // start animations

//Serial.println("Top Bottom Feed function finished...");
}

void Random_Top_Bottom(uint8_t fadetype) { 

//static uint32_t Random_func_timeout = 0, Random_func_lasttime = 0; 
static uint8_t current_r = 0, total_x = 0;

if (var7 == 0 ) {total_x = 13;} else total_x = var7;
    
    if ( (millis() > lasteffectupdate  ) && (!strip->IsAnimating()) ) {

/*
      Serial.println();
      Serial.print("Effect updated..(");
      Serial.print(strip->IsAnimating());
      Serial.print(") ");
*/
      uint16_t random_animation_speed = random(2000, 20000);
      uint8_t vvv,bbb,dif = 0 ; 
      vvv = random(255);

do {
      bbb = random(255); 
      dif = vvv - bbb; 
      dif = abs(dif);
      } while (dif < 50);


      /*Serial.print("top: ");
      Serial.print(vvv);
      Serial.print(" bottom: ");
      Serial.print(bbb);
      Serial.print(" Abs diff: ");
      Serial.print(abs(dif)); */

      RgbColor random_colour_top = Wheel(vvv); //  RgbColor(255,0,0); 
      RgbColor random_colour_bottom = Wheel(bbb); //  RgbColor(255,0,0); 

      //Serial.print("  (PRE)");
      
      top_bottom_fade(random_colour_top, random_colour_bottom, total_x, fadetype);
      


      //Serial.println("  (POST) ");
      

      //Random_func_lasttime = millis() + CurrentAnimationSpeed;  // was working...  changed to get updates working....

      lasteffectupdate = millis() + random(WS2812interval*30, (WS2812interval * 300) ) + CurrentAnimationSpeed;
      if (fadetype == 2) lasteffectupdate = millis() + CurrentAnimationSpeed + 500; // This effect does not require changing.... 

      //Serial.print("Random_func_lasttime: ");
      //Serial.print(Random_func_lasttime); 
      // Random_func_timeout = random(60000, 60000*5);
      

      //Random_func_timeout = random(WS2812interval*30, (WS2812interval * 300) ) + CurrentAnimationSpeed; 
      //Serial.print("   Random_func_timeout: ");
      //Serial.println(Random_func_timeout); 

    }
  //}

} // end of actual function.....





// Function example takes H, S, I, and a pointer to the 
// returned RGB colorspace converted vector. It should
// be initialized with:
//
// int rgb[3];
//
// in the calling function. After calling hsi2rgb
// the vector rgb will contain red, green, and blue
// calculated values.

//int rgb[3];

RgbColor notworking_hsi2rgb(float H, float S, float I) {

  RgbColor RGBcolour;

  int r, g , b ;

  //H = fmod(H,360); // cycle H around to 0-360 degrees

//  f = a/b - floor(a/b);
 Serial.println();
 Serial.print("hsitorgb func: H="); 
 Serial.print(H);
 H = H/360 - floor(H/360); 
 Serial.print(" -> ");
Serial.print(H);
Serial.print("  : ");

  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } else if(H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  Serial.println();
  Serial.print("R: ");
  Serial.print(r);
  Serial.print("G: ");
  Serial.print(g);
  Serial.print("B: ");
  Serial.print(b);

  RGBcolour.R = r;
  RGBcolour.G = g;
  RGBcolour.B = b;
  return RGBcolour; 
}
