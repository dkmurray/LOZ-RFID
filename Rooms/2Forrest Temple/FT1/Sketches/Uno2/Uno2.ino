/*//////////////////////////////////////////////////////////////////////////////
* ----------------------------------------------------------------------------
* This is modified from a MFRC522 library example; see
* https://github.com/miguelbalboa/rfid for further details and other examples.
*
* Released into the public domain.
* ----------------------------------------------------------------------------
* BEWARE: Data will be written to the PICC, in sector #1 (blocks #4 to #7).
*
* Pin layout used:
* ---------------------------------------
*             MFRC522      Arduino
*             Reader/PCD   Uno/101
* Signal      Pin          Pin
* ---------------------------------------
* RST/Reset   RST          9
* SPI SS      SDA(SS)      10
* SPI MOSI    MOSI         11 / ICSP-4
* SPI MISO    MISO         12 / ICSP-1
* SPI SCK     SCK          13 / ICSP-3
//////////////////////////////////////////////////////////////////////////////*/
#include <require_cpp11.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <MFRC522Extended.h>

#include <SPI.h>
#include <Wire.h>
#include <FastLED.h>

#define RST_PIN         9
#define SS_PIN          10

#define LED_PIN     14
#define NUM_LEDS    30
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern const TProgmemPalette16 boomerang PROGMEM;


MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

MFRC522::MIFARE_Key key;
byte x = 200;

/*//////////////////////////////////////////////////////////////////////////////
// Initialize.
//////////////////////////////////////////////////////////////////////////////*/
void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    currentPalette = boomerang;
    currentBlending = LINEARBLEND;
    Serial.begin(9600);  // Initialize serial communications with the PC
    while (!Serial);     // Do nothing if no serial port is opened (for Arduinos based on ATMEGA32U4)
    Wire.begin();
    SPI.begin();         // Init SPI bus
    mfrc522.PCD_Init();  // Init MFRC522 card 

    // Prepare the key (used both as key A and as key B)
    // Using FFFFFFFFFFFF which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC"));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));
}

/*//////////////////////////////////////////////////////////////////////////////
// Main loop
//////////////////////////////////////////////////////////////////////////////*/
void loop() {
    
    ///////////////////////
    // Look for new cards
    ///////////////////////
    if ( !mfrc522.PICC_IsNewCardPresent()) return;

    ////////////////////////////
    // Select one of the cards
    ////////////////////////////
    if ( !mfrc522.PICC_ReadCardSerial()) return;

    //////////////////////////////////////////////
    // We write to sector #1, covering block #4
    // up to and including block #7
    //////////////////////////////////////////////
    byte sector         = 1;
    byte blockAddr      = 4;
    byte dataBlock[]    = {
        0x00, 0x00, 0xFF, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
    };
    byte trailerBlock   = 7;
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);

    
    /////////////////////////////
    // Authenticate using key A
    /////////////////////////////
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }


    /////////////////////////////
    // Read data from the block
    /////////////////////////////
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();
    

    /////////////////////////////
    // Authenticate using key B
    /////////////////////////////
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    ////////////////////////////
    // Write data to the block
    ////////////////////////////
    for(int i = 0; i < 16; i++){
    dataBlock[i] = buffer[i];  
    }
    dataBlock[2] = 0xFF;
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" Writing..."));
    dump_byte_array(dataBlock, 16); Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();

    //////////////
    // Halt PICC
    //////////////
    mfrc522.PICC_HaltA();

    ///////////////////////////
    // Stop encryption on PCD
    ///////////////////////////
    mfrc522.PCD_StopCrypto1();

    Wire.beginTransmission(8);
    Wire.write("x is ");        // sends five bytes
    Wire.write(x);              // sends one byte
    Wire.endTransmission();
    delay(100);
    ledTest(4500);
}


/*//////////////////////////////////////////////////////////////////////////////
// Helper routine to dump a byte array as hex values to Serial.
//////////////////////////////////////////////////////////////////////////////*/
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

void ledTest(int time){
  time = time/10;
  for(int i = 0; i < time; i++){
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(10);
  }
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

const TProgmemPalette16 boomerang PROGMEM =
{
    CRGB::Yellow,
    CRGB::Green,
    CRGB::Green,
    CRGB::Black,

    CRGB::Green,
    CRGB::Black,
    CRGB::Yellow,
    CRGB::Yellow,

    CRGB::Green,
    CRGB::Green,
    CRGB::Green,
    CRGB::Yellow,

    CRGB::Black,
    CRGB::Green,
    CRGB::Yellow,
    CRGB::Green
};
