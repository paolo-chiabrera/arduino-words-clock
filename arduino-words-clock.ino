#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <Time.h>
#include <arduino-timer.h>
#include <FastLED.h>
#include <Button2.h>
#include <math.h>

auto timer = timer_create_default();

// Clock
#define RTM_CLOCK_PIN 15
#define RTM_DATA_PIN 13
#define RTM_RESET_PIN 11

ThreeWire myWire(RTM_DATA_PIN, RTM_CLOCK_PIN, RTM_RESET_PIN);
RtcDS1302<ThreeWire> Rtc(myWire);

int prevHours = -1;
int prevMinutes = -1;
int hours = 0;
int minutes = 0;

// LED strip
#define LED_PIN 7
#define COLOR_ORDER GRB
#define CHIPSET WS2812B
#define NUM_LEDS 120

CRGB leds[NUM_LEDS];

float brightnessRatio = 2.125;
int brightnessLow = 5;
int brightnessHigh = 80;

// Words
#define MAX_STRING_LENGTH 90

#define OCLOCK "108_109_110_111_112_113"
#define MIDNIGHT "99_100_101_102_103_104_105_106"
#define ONE "49_50_51"
#define TWO "37_38_39"
#define THREE "60_61_62_63_64"
#define FOUR "73_74_75_76"
#define FIVE "92_93_94_95"
#define SIX "84_85_86"
#define SEVEN "115_116_117_118_119"
#define EIGHT "67_68_69_70_71"
#define NINE "87_88_89_90"
#define TEN "96_97_98"
#define ELEVEN "53_54_55_56_57_58"
#define TWELVE "78_79_80_81_82_83"
#define FIVE_MINUTES_PAST "20_21_22_23_29_30_31_32_33_34_35_44_45_46_47"
#define TEN_MINUTES_PAST "9_10_11_29_30_31_32_33_34_35_44_45_46_47"
#define FIFTEEN_MINUTES_PAST "12_13_14_15_16_17_18_29_30_31_32_33_34_35_44_45_46_47"
#define TWENTY_MINUTES_PAST "1_2_3_4_5_6_29_30_31_32_33_34_35_44_45_46_47"
#define TWENTY_FIVE_MINUTES_PAST "1_2_3_4_5_6_20_21_22_23_29_30_31_32_33_34_35_44_45_46_47"
#define HALF_PAST "25_26_27_28_44_45_46_47"
#define TWENTY_FIVE_MINUTES_TO "1_2_3_4_5_6_20_21_22_23_29_30_31_32_33_34_35_41_42"
#define TWENTY_MINUTES_TO "1_2_3_4_5_6_29_30_31_32_33_34_35_41_42"
#define FIFTEEN_MINUTES_TO "12_13_14_15_16_17_18_29_30_31_32_33_34_35_41_42"
#define TEN_MINUTES_TO "9_10_11_29_30_31_32_33_34_35_41_42"
#define FIVE_MINUTES_TO "20_21_22_23_29_30_31_32_33_34_35_41_42"

// Palettes
#define PALETTE_SIZE 4

CRGB PALETTE[] = {
  CRGB(249, 237, 105), CRGB(240, 138, 93), CRGB(184, 59, 94), CRGB(106, 44, 112),
  CRGB(243, 129, 129), CRGB(252, 227, 138), CRGB(234, 255, 208), CRGB(149, 225, 211),
  CRGB(248, 95, 115), CRGB(251, 232, 211), CRGB(146, 138, 151), CRGB(40, 60, 99),
  CRGB(7, 104, 159), CRGB(162, 213, 242), CRGB(250, 250, 250), CRGB(255, 126, 103),
  CRGB(168, 230, 207), CRGB(253, 255, 171), CRGB(255, 211, 182), CRGB(255, 170, 165),
  CRGB(12, 5, 109), CRGB(89, 13, 130), CRGB(182, 26, 174), CRGB(242, 93, 156),
  CRGB(21, 82, 99), CRGB(255, 111, 60), CRGB(255, 154, 60), CRGB(255, 201, 60),
  CRGB(255, 100, 100), CRGB(255, 130, 100), CRGB(255, 170, 100), CRGB(255, 245, 165),
  CRGB(248, 181, 149), CRGB(246, 114, 128), CRGB(192, 108, 132), CRGB(108, 91, 124),
  CRGB(39, 41, 109), CRGB(94, 99, 182), CRGB(163, 147, 235), CRGB(245, 199, 247),
  CRGB(54, 79, 107), CRGB(63, 193, 201), CRGB(245, 245, 245), CRGB(252, 81, 133),
  CRGB(168, 216, 234), CRGB(170, 150, 218), CRGB(252, 186, 211), CRGB(255, 255, 210),
  CRGB(0, 184, 169), CRGB(248, 243, 212), CRGB(246, 65, 108), CRGB(255, 222, 125),
  CRGB(72, 70, 109), CRGB(61, 132, 168), CRGB(70, 205, 207), CRGB(171, 237, 216),
  CRGB(226, 62, 87), CRGB(136, 48, 78), CRGB(82, 37, 70), CRGB(49, 29, 63),
  CRGB(246, 114, 128), CRGB(192, 108, 132), CRGB(108, 91, 123), CRGB(53, 92, 125),
  CRGB(254, 226, 179), CRGB(255, 162, 153), CRGB(173, 105, 137), CRGB(86, 35, 73),
  CRGB(241, 227, 203), CRGB(249, 179, 132), CRGB(202, 81, 22), CRGB(88, 28, 12),
  CRGB(88, 180, 174), CRGB(255, 226, 119), CRGB(255, 179, 103), CRGB(255, 226, 188),
  CRGB(246, 245, 120), CRGB(246, 215, 67), CRGB(100, 157, 102), CRGB(6, 98, 59),
  CRGB(255, 82, 0), CRGB(111, 0, 0), CRGB(0, 38, 59), CRGB(0, 161, 171),
  CRGB(248, 243, 235), CRGB(195, 237, 234), CRGB(252, 126, 47), CRGB(244, 5, 82),
  CRGB(188, 101, 141), CRGB(130, 196, 195), CRGB(249, 216, 156), CRGB(245, 167, 167),
  CRGB(234, 144, 122), CRGB(251, 198, 135), CRGB(244, 247, 197), CRGB(170, 205, 190),
  CRGB(217, 32, 39), CRGB(255, 146, 52), CRGB(255, 205, 60), CRGB(53, 208, 186),
  CRGB(126, 189, 180), CRGB(246, 209, 152), CRGB(244, 165, 72), CRGB(134, 42, 92),
  CRGB(48, 57, 96), CRGB(234, 154, 150), CRGB(248, 178, 79), CRGB(229, 229, 229),
  CRGB(255, 211, 29), CRGB(214, 52, 71), CRGB(245, 123, 81), CRGB(246, 238, 223),
  CRGB(255, 216, 166), CRGB(252, 130, 16), CRGB(255, 66, 127), CRGB(0, 120, 146),
  CRGB(95, 221, 229), CRGB(244, 234, 142), CRGB(243, 113, 33), CRGB(217, 32, 39),
  CRGB(252, 248, 118), CRGB(55, 151, 164), CRGB(140, 203, 190), CRGB(228, 228, 228),
  CRGB(250, 238, 231), CRGB(245, 66, 145), CRGB(76, 211, 194), CRGB(10, 151, 176),
  CRGB(232, 228, 225), CRGB(249, 196, 154), CRGB(236, 130, 58), CRGB(124, 60, 33),
  CRGB(244, 246, 255), CRGB(243, 198, 35), CRGB(18, 118, 129), CRGB(16, 55, 92),
  CRGB(101, 64, 98), CRGB(255, 156, 113), CRGB(251, 212, 109), CRGB(232, 234, 211),
  CRGB(17, 29, 94), CRGB(199, 0, 57), CRGB(243, 113, 33), CRGB(255, 189, 105),
  CRGB(182, 235, 122), CRGB(247, 247, 238), CRGB(251, 120, 19), CRGB(23, 112, 110),
  CRGB(244, 246, 255), CRGB(255, 203, 116), CRGB(234, 144, 122), CRGB(79, 138, 139),
  CRGB(162, 222, 150), CRGB(60, 165, 157), CRGB(90, 61, 85), CRGB(231, 156, 42),
  CRGB(56, 62, 86), CRGB(246, 158, 123), CRGB(238, 218, 209), CRGB(212, 181, 176),
  CRGB(185, 172, 146), CRGB(255, 169, 49), CRGB(254, 203, 137), CRGB(251, 230, 212),
  CRGB(250, 38, 160), CRGB(5, 223, 215), CRGB(163, 247, 191), CRGB(255, 245, 145),
};

int NUMBER_OF_PALETTES = sizeof(PALETTE) / PALETTE_SIZE / 3;

int blockNumber = random(0, NUMBER_OF_PALETTES) * PALETTE_SIZE;

// Night mode
#define NIGHT_MODE_BRIGHTNESS_LEVEL 1

CRGB NIGHT_MODE_COLOURS[] = {CRGB(100, 0, 0), CRGB(0, 100, 0), CRGB(0, 0, 100)};
int NUMBER_OF_NIGHT_MODE_COLOURS = sizeof(NIGHT_MODE_COLOURS) / 3;
int hoursNightStart = 0;
int hoursNightEnd = 8;

bool isNightModeOn() {
  if (!isLowLight()) {
    return false;  
  }

  if (hoursNightStart < 12) {
    return hours >= hoursNightStart && hours < hoursNightEnd;
  }

  return hours >= hoursNightStart || hours < hoursNightEnd;
}

CRGB getNightModeColour() {
  return NIGHT_MODE_COLOURS[random(0, NUMBER_OF_NIGHT_MODE_COLOURS)];
}

// Light sensor
#define LIGHT_SENSOR_PIN 3

int currentLightSensorValue = 0;
int prevLightSensorValue = -1;

bool isLowLight() {
  return currentLightSensorValue != 0;
}

void updateLEDBrightness() {
  int cumulativeRead = 0;  

  cumulativeRead += digitalRead(LIGHT_SENSOR_PIN);
  delay(50);
  cumulativeRead += digitalRead(LIGHT_SENSOR_PIN);
  delay(50);
  cumulativeRead += digitalRead(LIGHT_SENSOR_PIN);
  delay(50);
  cumulativeRead += digitalRead(LIGHT_SENSOR_PIN);
  delay(50);
  cumulativeRead += digitalRead(LIGHT_SENSOR_PIN);

  currentLightSensorValue = (cumulativeRead > 2) ? 1 : 0;

  if (currentLightSensorValue != prevLightSensorValue) {
    prevLightSensorValue = currentLightSensorValue;
    
    int brightnessLevel = 100;

    if (isNightModeOn()) {
      brightnessLevel = floor(NIGHT_MODE_BRIGHTNESS_LEVEL * brightnessRatio);
    } else {
      if (isLowLight()) {
        brightnessLevel = floor(brightnessLow * brightnessRatio);
      } else {
        brightnessLevel = floor(brightnessHigh * brightnessRatio);
      }
    }
    
    FastLED.setBrightness(brightnessLevel);
    FastLED.show();
  }
}

// Menu
#define DEFAULT_MENU_ITEM 0
#define MENU_ITEM_SIZE 6
#define MENU_ITEM_HOURS 0
#define MENU_ITEM_MINUTES 1
#define MENU_ITEM_BRIGHTNESS_LOW 2
#define MENU_ITEM_BRIGHTNESS_HIGH 3
#define MENU_ITEM_HOURS_NIGHT_START 4
#define MENU_ITEM_HOURS_NIGHT_END 5

#define MAX_HOURS_VALUE 24
#define MIN_HOURS_VALUE 1
#define MAX_MINUTES_VALUE 59
#define MIN_MINUTES_VALUE 0
#define MAX_BRIGHTNESS_LOW_VALUE 120
#define MIN_BRIGHTNESS_LOW_VALUE 1
#define MAX_BRIGHTNESS_HIGH_VALUE 120
#define MIN_BRIGHTNESS_HIGH_VALUE 1
#define MAX_HOURS_NIGHT_START_VALUE 24
#define MIN_HOURS_NIGHT_START_VALUE 1
#define MAX_HOURS_NIGHT_END_VALUE 24
#define MIN_HOURS_NIGHT_END_VALUE 1

bool menuIsActive = false;
bool subMenuIsActive = false;
int lastMenuItem = -1;
int currentMenuItem = DEFAULT_MENU_ITEM;
int lastSubMenuValue = -1;
int subMenuValue = 0;

bool menuHasChanged () {
  if (currentMenuItem == lastMenuItem && subMenuValue == lastSubMenuValue) {
    return false;  
  } else {
    lastMenuItem = currentMenuItem;
    if (subMenuIsActive) {
      lastSubMenuValue = subMenuValue;
    }    
    return true;
  }
}

void nextMenuItem() {
  currentMenuItem++;

  if (currentMenuItem >= MENU_ITEM_SIZE) {
    currentMenuItem = 0;
  } 
}

void prevMenuItem() {
  currentMenuItem--;

  if (currentMenuItem < 0) {
    currentMenuItem = MENU_ITEM_SIZE - 1;
  } 
}

void nextHoursValue() {
  subMenuValue++;

  if (subMenuValue > MAX_HOURS_VALUE) {
    subMenuValue = MIN_HOURS_VALUE;
  } 
}

void prevHoursValue() {
  subMenuValue--;

  if (subMenuValue < MIN_HOURS_VALUE) {
    subMenuValue = MAX_HOURS_VALUE - 1;
  } 
}

void nextMinutesValue() {
  subMenuValue++;

  if (subMenuValue > MAX_MINUTES_VALUE) {
    subMenuValue = MIN_MINUTES_VALUE;
  } 
}

void prevMinutesValue() {
  subMenuValue--;

  if (subMenuValue < MIN_MINUTES_VALUE) {
    subMenuValue = MAX_MINUTES_VALUE - 1;
  } 
}

void nextBrightnessLowValue() {
  subMenuValue++;

  if (subMenuValue > MAX_BRIGHTNESS_LOW_VALUE) {
    subMenuValue = MIN_BRIGHTNESS_LOW_VALUE;
  } 
}

void prevBrightnessLowValue() {
  subMenuValue--;

  if (subMenuValue < MIN_BRIGHTNESS_LOW_VALUE) {
    subMenuValue = MAX_BRIGHTNESS_LOW_VALUE - 1;
  } 
}

void nextBrightnessHighValue() {
  subMenuValue++;

  if (subMenuValue > MAX_BRIGHTNESS_HIGH_VALUE) {
    subMenuValue = MIN_BRIGHTNESS_HIGH_VALUE;
  } 
}

void prevBrightnessHighValue() {
  subMenuValue--;

  if (subMenuValue < MIN_BRIGHTNESS_HIGH_VALUE) {
    subMenuValue = MAX_BRIGHTNESS_HIGH_VALUE - 1;
  } 
}

void nextHoursNightStartValue() {
  subMenuValue++;

  if (subMenuValue > MAX_HOURS_NIGHT_START_VALUE) {
    subMenuValue = MIN_HOURS_NIGHT_START_VALUE;
  } 
}

void prevHoursNightStartValue() {
  subMenuValue--;

  if (subMenuValue < MIN_HOURS_NIGHT_START_VALUE) {
    subMenuValue = MAX_HOURS_NIGHT_START_VALUE - 1;
  } 
}

void nextHoursNightEndValue() {
  subMenuValue++;

  if (subMenuValue > MAX_HOURS_NIGHT_END_VALUE) {
    subMenuValue = MIN_HOURS_NIGHT_END_VALUE;
  } 
}

void prevHoursNightEndValue() {
  subMenuValue--;

  if (subMenuValue < MIN_HOURS_NIGHT_END_VALUE) {
    subMenuValue = MAX_HOURS_NIGHT_END_VALUE - 1;
  } 
}

void clearMenu() {
  menuIsActive = false;
  currentMenuItem = DEFAULT_MENU_ITEM;
  lastMenuItem = -1;  
}

void clearSubMenu() {
  subMenuIsActive = false;
  lastSubMenuValue = -1;
  subMenuValue = 0;
}

CRGB getMenuLetterColour(int index) {
  if (subMenuIsActive) {
    if (subMenuValue >= index) {
      return CRGB::Yellow;
    }
      
    return CRGB::Orange;   
  }

  return CRGB::White;
}

void updateLEDStripMenu () {
  switch(currentMenuItem) {
    case MENU_ITEM_HOURS:
      leds[25] = getMenuLetterColour(25); // H(ours)
      break;
    case MENU_ITEM_MINUTES:
      leds[29] = getMenuLetterColour(29); // M(inutes)
      break;
    case MENU_ITEM_BRIGHTNESS_LOW:
      leds[24] = getMenuLetterColour(24); // B(rightness)
      leds[27] = getMenuLetterColour(27); // L(ow)      
      break;
    case MENU_ITEM_BRIGHTNESS_HIGH:
      leds[24] = getMenuLetterColour(24); // B(rightness)
      leds[25] = getMenuLetterColour(25); // H(igh)
      break;  
    case MENU_ITEM_HOURS_NIGHT_START:
      leds[25] = getMenuLetterColour(25); // H(ours)
      leds[31] = getMenuLetterColour(31); // N(ight)
      leds[35] = getMenuLetterColour(35); // S(tart)
      break;
    case MENU_ITEM_HOURS_NIGHT_END:
      leds[25] = getMenuLetterColour(25); // H(ours)
      leds[31] = getMenuLetterColour(31); // N(ight)
      leds[34] = getMenuLetterColour(34); // E(nd)
      break;  
  }
}

void updateLEDStripSubMenu () {
  for(int i = 0; i < subMenuValue; i++) {
    int rowNumber = floor(i / 12);
    int reverseOddIndex = (rowNumber + 1) * 12 - (i % 12) - 1;
    (rowNumber % 2 == 0) ? leds[i] = CRGB::White : leds[reverseOddIndex] = CRGB::White;
  }

  updateLEDStripMenu();
}

// Rotary
#define ROTARY_PIN_A 19  //ky-040 clk
#define ROTARY_PIN_B 21  //ky-040 dt
#define ROTARY_BUTTON_PIN 23

Button2 button = Button2(ROTARY_BUTTON_PIN);

int currentStatePinA;
int lastStatePinA;

void rotaryLoop() {
  currentStatePinA = digitalRead(ROTARY_PIN_A);

  if (currentStatePinA != lastStatePinA) {
    if (digitalRead(ROTARY_PIN_B) != currentStatePinA) {
      // increase
      onNegativeRotation();      
    } else {
      // decrease
      onPositiveRotation();
    }
    lastStatePinA = currentStatePinA;
  }
}

void onPositiveRotation() {
  if (menuIsActive) {
    if (subMenuIsActive) {
      switch(currentMenuItem) {
        case MENU_ITEM_HOURS:
          nextHoursValue();
          break;
        case MENU_ITEM_MINUTES:
          nextMinutesValue();
          break;
        case MENU_ITEM_BRIGHTNESS_LOW:
          nextBrightnessLowValue();
          break;
        case MENU_ITEM_BRIGHTNESS_HIGH:
          nextBrightnessHighValue();
          break;
        case MENU_ITEM_HOURS_NIGHT_START:
          nextHoursNightStartValue();
          break;
        case MENU_ITEM_HOURS_NIGHT_END:
          nextHoursNightEndValue();
          break;
      }
      
    } else {
      nextMenuItem();
    }
  }  
}

void onNegativeRotation() {
  if (menuIsActive) {
    if (subMenuIsActive) {
      switch(currentMenuItem) {
        case MENU_ITEM_HOURS:
          prevHoursValue();
          break;
        case MENU_ITEM_MINUTES:
          prevMinutesValue();
          break;
        case MENU_ITEM_BRIGHTNESS_LOW:
          prevBrightnessLowValue();
          break;
        case MENU_ITEM_BRIGHTNESS_HIGH:
          prevBrightnessHighValue();
          break;
        case MENU_ITEM_HOURS_NIGHT_START:
          prevHoursNightStartValue();
          break;
        case MENU_ITEM_HOURS_NIGHT_END:
          prevHoursNightEndValue();
          break;
      }
    } else {
      prevMenuItem();
    }
  }
}

// Helpers
void checkClockSetup (RtcDateTime compiled) {
  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();

  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
}

void setupClock () {
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  checkClockSetup(compiled);
  updateTimeFromRTC();
}

void updateTimeFromRTC() {
  RtcDateTime now = Rtc.GetDateTime();
  hours = now.Hour();
  minutes = now.Minute();
}

void setTimeRTC() {
  RtcDateTime now = Rtc.GetDateTime();
  Rtc.SetDateTime(RtcDateTime(now.Year(), now.Month(), now.Day(), hours, minutes, now.Second()));  
}

void updateHours(int h) {
  hours = (h >= 24) ? 0 : h;
  setTimeRTC();
}

void updateMinutes(int m) {
  minutes = (m >= 60) ? 0 : m;
  setTimeRTC();
}

bool timeHasChanged () {
  if (hours == prevHours && minutes == prevMinutes) {
    return false;  
  } else {
    prevHours = hours;
    prevMinutes = minutes;
    return true;  
  }
}

// LED
void setupLEDStrip () {
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  updateLEDBrightness();
  updateLEDStrip();
}

void clearLEDStrip () {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }  
}

void updateLEDStripTime () {
  bool nightMode = isNightModeOn();
  String indexes = getIndexes(); 

  int colourIndex = -1;
  int lastIndex = -1;

  if (minutes % 5 == 0) {
    blockNumber = random(0, NUMBER_OF_PALETTES) * PALETTE_SIZE;
  }

  CRGB nightColour = getNightModeColour();

  for (int i = 0; i < NUM_LEDS; i++) {
    if (indexes.indexOf("_" + String(i) + "_") >= 0) {
        if (i - lastIndex > 1) {
          colourIndex++;
        }
  
        if (colourIndex >= PALETTE_SIZE) {
          colourIndex = 0;
        }
  
        leds[i] = nightMode ? nightColour : PALETTE[blockNumber + colourIndex];
        
        lastIndex = i;
     } else {
      leds[i] = CRGB::Black;
    }
  }

  FastLED.show();
}

void updateLEDStrip () {
  if (menuIsActive) {
    if (menuHasChanged()) {
      clearLEDStrip();
      if (subMenuIsActive) {
        updateLEDStripSubMenu();
      } else {
        updateLEDStripMenu();
      }
      FastLED.show();
    }    
  } else {
    if (timeHasChanged()) {
      updateLEDStripTime();
    }   
  }  
}

void checkLights() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
  }
  FastLED.show();
  delay(3000);
  
  clearLEDStrip();
  FastLED.show();
  delay(2000);
}

bool nextHour(int minutes) {
  return minutes >= 35;
}

String getIndexes() {
  String output = "_";
  
  if (minutes >= 5 && minutes < 10) {
    output += FIVE_MINUTES_PAST;
  } else if (minutes >= 10 && minutes < 15) {
    output += TEN_MINUTES_PAST;
  } else if (minutes >= 15 && minutes < 20) {
    output += FIFTEEN_MINUTES_PAST;
  } else if (minutes >= 20 && minutes < 25) {
    output += TWENTY_MINUTES_PAST;
  } else if (minutes >= 25 && minutes < 30) {
    output += TWENTY_FIVE_MINUTES_PAST;
  } else if (minutes >= 30 && minutes < 35) {
    output += HALF_PAST;
  } else if (minutes >= 35 && minutes < 40) {
    output += TWENTY_FIVE_MINUTES_TO;
  } else if (minutes >= 40 && minutes < 45) {
    output += TWENTY_MINUTES_TO;
  } else if (minutes >= 45 && minutes < 50) {
    output += FIFTEEN_MINUTES_TO;
  } else if (minutes >= 50 && minutes < 55) {
    output += TEN_MINUTES_TO;
  } else if (minutes >= 55 && minutes < 60) {
    output += FIVE_MINUTES_TO;
  }

  output += "_";

  switch (hours) {
    case 12:
      output += nextHour(minutes) ? ONE : TWELVE;
      break;
    case 1:
    case 13:
      output += nextHour(minutes) ? TWO : ONE;
      break;
    case 2:
    case 14:
      output += nextHour(minutes) ? THREE : TWO;
      break;
    case 3:
    case 15:
      output += nextHour(minutes) ? FOUR :THREE;
      break;
    case 4:
    case 16:
      output += nextHour(minutes) ? FIVE : FOUR;
      break;
    case 5:
    case 17:
      output += nextHour(minutes) ? SIX : FIVE;
      break;
    case 6:
    case 18:
      output += nextHour(minutes) ? SEVEN : SIX;
      break;
    case 7:
    case 19:
      output += nextHour(minutes) ? EIGHT :SEVEN;
      break;
    case 8:
    case 20:
      output += nextHour(minutes) ? NINE : EIGHT;
      break;
    case 9:
    case 21:
      output += nextHour(minutes) ? TEN : NINE;
      break;
    case 10:
    case 22:
      output += nextHour(minutes) ? ELEVEN : TEN;
      break;
    case 11:
    case 23:
      output += nextHour(minutes) ? MIDNIGHT : ELEVEN;
      break;
    default:
      output += nextHour(minutes) ? ONE : MIDNIGHT;      
      break;
  }

  if (minutes < 5 && hours != 0) {
    output += "_";
    output += OCLOCK;
  }

  output += "_";

  return output;
}

void onSingleClick() {
  if (!menuIsActive) {
    menuIsActive = true;
    return;
  }

  if (!subMenuIsActive) {
    lastMenuItem = currentMenuItem;
    switch(currentMenuItem) {
    case MENU_ITEM_HOURS:
      subMenuValue = hours;
      break;
    case MENU_ITEM_MINUTES:
      subMenuValue = minutes;
      break;
    case MENU_ITEM_BRIGHTNESS_LOW:
      subMenuValue = brightnessLow;
      break;
    case MENU_ITEM_BRIGHTNESS_HIGH:
      subMenuValue = brightnessHigh;
      break;
    case MENU_ITEM_HOURS_NIGHT_START:
      subMenuValue = hoursNightStart;
      break;
    case MENU_ITEM_HOURS_NIGHT_END:
      subMenuValue = hoursNightEnd;
      break;
  }
    subMenuIsActive = true;
    return;
  }

  switch(currentMenuItem) {
    case MENU_ITEM_HOURS:
      updateHours(subMenuValue);
      break;
    case MENU_ITEM_MINUTES:
      updateMinutes(subMenuValue);
      break;
    case MENU_ITEM_BRIGHTNESS_LOW:
      brightnessLow = subMenuValue;
      updateLEDBrightness();
      break;
    case MENU_ITEM_BRIGHTNESS_HIGH:
      brightnessHigh = subMenuValue;
      updateLEDBrightness();
      break;
    case MENU_ITEM_HOURS_NIGHT_START:
      hoursNightStart = subMenuValue;
      break;
    case MENU_ITEM_HOURS_NIGHT_END:
      hoursNightEnd = subMenuValue;
      break;
  }

  clearSubMenu();
}

void onDoubleClick() {
  if (subMenuIsActive) {
    clearSubMenu();
    return;  
  }
  
  if (menuIsActive) {
    clearMenu();
    updateLEDStripTime();
    return;
  }
}

void clickHandler(Button2& btn) {
  switch (btn.getClickType()) {
    case SINGLE_CLICK:
    case LONG_CLICK:
      onSingleClick();
      break;
    case DOUBLE_CLICK:
    case TRIPLE_CLICK:
      onDoubleClick();
      break;
  }
}

bool every5Secs(void *) {
  updateLEDBrightness();
  return true;
}

void setup() {
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  pinMode(ROTARY_PIN_A, INPUT);
  pinMode(ROTARY_PIN_B, INPUT);

  Serial.begin(9600);
  delay(50);

  // Light sensor
  currentLightSensorValue = digitalRead(LIGHT_SENSOR_PIN);

  // Rotary
  lastStatePinA = digitalRead(ROTARY_PIN_A);

  // Button
  button.setClickHandler(clickHandler);
  button.setDoubleClickHandler(clickHandler);
  button.setTripleClickHandler(clickHandler);
  button.setLongClickHandler(clickHandler);

  // Clock
  setupClock();

  // LED Strip
  setupLEDStrip();
  updateLEDBrightness();

  // Timers
  timer.every(5000, every5Secs);
}

void loop() {
  timer.tick();
  
  // Rotary
  rotaryLoop();

  // Button
  button.loop();
  
  // Clock
  updateTimeFromRTC();

  // LED Strip
  updateLEDStrip();  
}
