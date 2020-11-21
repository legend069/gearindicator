/*
 Name:    GearIndicator_unoV3.ino
 Created: 11/21/2020 9:54:54 PM
 Author:  alexj
*/

// Wire Master Reader
//uno
//i need to fix the tft display parts.
//it is playing up because of the tft screen is using the i2c communication protacol.
// i need to use the "SPI" protocol, and /or run the i2c scanner on the uno for the tft screen address


#define SerialDebugging false //---------------------------------------------------------------------------------change for not Serial Debugging.
#define i2cMode false
#define GearDebuggingAutomatic true
#define GearDebuggingManual false
#define GearDebuggingnormal false
#define RCAmode true

String SerialInput = "-1";

#define ANSWERSIZE 5 //might need to set as 5
char replyFromSlave[ANSWERSIZE]; //Initialized variable to store recieved data from slave
//#define SLAVE_ADDRESS  8
//#define MASTER_ADDRESS 10
//const int sizeOfMessageSend = 5;// change this to the size of what i am sending to master
//const int sizeOfMessageRecieve = 20;// for future use when uno is sending data to slave. ------------

//char replyFromMaster[ANSWERSIZE]; //Initialized variable to store recieved data


//char sending_message_what_gear[ANSWERSIZE]; //data contains gear and if it is writing/removing
char sendToUno[ANSWERSIZE];
char writeOrRemove[ANSWERSIZE]; //0 remove 1 write
int WOR = -1;

int switchUP = A0; // should be able to use digital pins for this 
int switchDOWN = A1;
int switchLEFT = A2;
int switchRIGHT = A3;



bool stateofsensorUp = false;
bool stateofsensorDown = false;
bool stateofsensorLeft = false;
bool stateofsensorRight = false;


unsigned long previousTime = 0;
byte seconds;
byte minutes;
byte hours;

char tvoutputTestMSG[] = "testing message";


bool nuturalGear = false;
char gear0[] = "N";       int Nutural = 0;
char gear1[] = "1";       int First = 1;
char gear2[] = "2";       int Second = 2;
char gear3[] = "3";       int Thrid = 3;
char gear4[] = "4";       int Forth = 4;
char gear5[] = "5";       int Fifth = 5;
char gear6[] = "R";       int Sixth = 6;
char lastGear[] = "E";    int LastGear = 7;
char currentGear[] = "E"; int CurrentGear = 8;

char trueCurrentGear[] = "E";
char trueLastGear[] = "E";
int loopCounter = 0;
bool ingear = false;




//char gearRange[8] = {gear0, gear1,gear2,gear3,gear4,gear5,gear6,lastGear};
char gearRange[9] = {
  gear0[0], //0
  gear1[0], //1 
  gear2[0], //2
  gear3[0], //3
  gear4[0], //4
  gear5[0], //5
  gear6[0], //6
  lastGear[0], //7
  currentGear[0],//8
};
int amountOfGears = strlen(gearRange);

int debugGearTimer = 0;


//nano


//remove later
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;  //the value is a number of milliseconds
byte timeToEnableNuturalGear = 12;


const byte ledPin = 13;    //using the built in LED

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------






#define serialDebugging false //---------------------------------------------------------------------------------change for not //serial Debugging.
#define TFTmode false
#define RCAmode true

int sensorPinDown = 3;
int sensorPinUp = 2;
int sensorPinLeft = 4;
int sensorPinRight = 5;

#if (RCAmode == true)
#define Mainserialmessages false
#define UARTserial true
#endif // (RCAmode == false)

#if (TFTmode == true)
#define Mainserialmessages true
#define UARTserial false
#endif

//#define ANSWERSIZE 5 // might need to set as 5
//#define SLAVE_ADDRESS 8
//#define MASTER_ADDRESS 10
//const int sizeOfMessageSend = 20;// for future use when uno is sending data to slave. -------------
//const int sizeOfMessageRecieve = 5;// need to change this.
//char message[sizeOfMessageSend];// for future use when uno is sending data to slave. -------------

//char replyFromSlave[ANSWERSIZE]; //Initialized variable to store recieved data from slave
//char tempReplyFromSlave[ANSWERSIZE];

//char response[ANSWERSIZE];// not sure what this is?

bool inGear = false;
/*
char gear0[] = "N";       int Nutural = 0;
char gear1[] = "1";       int First = 1;
char gear2[] = "2";       int Second = 2;
char gear3[] = "3";       int Thrid = 3;
char gear4[] = "4";       int Forth = 4;
char gear5[] = "5";       int Fifth = 5;
char gear6[] = "R";       int Sixth = 6;
char lastGear[] = "E";    int LastGear = 7;
char currentGear[] = "E"; int CurrentGear = 8;*/

char currentRecievedGear[] = "E";

int nuturalTimer = 0;
int nuturalTimerMax = 20;
int gearTimer = 0;


//char gearRange[8] = {gear0, gear1,gear2,gear3,gear4,gear5,gear6,lastGear};
/*
char gearRange[9] = {
  gear0[0], //0
  gear1[0], //1 
  gear2[0], //2
  gear3[0], //3
  gear4[0], //4
  gear5[0], //5
  gear6[0], //6
  lastGear[0], //7
  currentGear[0],//8
};
int amountOfGears = strlen(gearRange);*/
// (TFTmode == true)

//port 20
//---------------------------
//include files

#if (RCAmode == true)

#include <SPI.h>
#include <TVout.h>
#include <Wire.h>
#include <font8x8ext.h>
#include <font8x8.h>
#include <font6x8.h>
#include <font4x6.h>

TVout TV;
unsigned char x, y;

#endif

#if (TFTmode == true)

#include <Wire.h>
#include <SPI.h>            // f.k. for Arduino-1.5.2
#include <SD.h>             // Use the official SD library on hardware pins
#include <Adafruit_GFX.h>   //Core graphics library
//#include <MCUFRIEND_kbv.h> //Hardware-specific library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
//#include <Adafruit_ST77xx.h>
//#include <Adafruit_ST7789.h>
#include <TouchScreen.h>

#endif // (TFTmode == true)

//include files
//---------------------------------------------------------------------------------------------
//tft specs
/*
int dispx, dispy, text_y_center, swapxy;
#define SWAP(x, y) { int t = x; x = y; y = t; }

#define PORTRAIT  0
#define LANDSCAPE 1

#define TOUCH_ORIENTATION PORTRAIT

const int minX = 0;
const int minY = 0;
//maybe add an if touch orientation is landscape swap x/y max values

//tft specs
//---------------------------------------------------------------------------------------------
//calibrate
#define calibrateDebug false

#if( calibrateDebug == true)

#define TS_MINX 906
#define TS_MAXX 116

#define TS_MINY 92
#define TS_MAXY 952

#define SWAP(x, y) { int t = x; x = y; y = t; }

#define TITLE "TouchScreen.h GFX Calibration"



uint32_t cx, cy, cz;
uint32_t rx[8], ry[8];
int32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center, swapxy;
uint32_t calx, caly, cals;


#endif //


//calibrate
//---------------------------------------------------------------------------------------------
//for SD card pre setup

uint16_t read16(File& f) {
    uint16_t result;         // read little-endian
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

uint32_t read32(File& f) {
    uint32_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}




#define BMPIMAGEOFFSET 54
#define BUFFPIXEL      20


#define SD_CS     10
#define NAMEMATCH ""        // "" matches any name
//#define NAMEMATCH "tiger"   // *tiger*.bmp
#define PALETTEDEPTH   0     // do not support Palette modes
//#define PALETTEDEPTH   8     // support 256-colour Palett

char bmpfile_01[] = "/bmpFiles/01.bmp";
char bmpfile_02[] = "/bmpFiles/02.bmp";
char bmpfile_03[] = "/bmpFiles/03.bmp";
char bmpfile_04[] = "/bmpFiles/04.bmp";
//char namebuf[32] = "/";   //BMP files in root directory
char namebuf[32] = "/bmpFiles/";  //BMP directory e.g. files in /bmpFiles/*.bmp

File root;
int pathlen;


#define AmountOfFiles 4 //amount of files to display
#define FILE_NAME_SIZE_MAX 20 //file name length


char file_name[AmountOfFiles][FILE_NAME_SIZE_MAX];

MCUFRIEND_kbv tft;
//for SD card pre setup
//---------------------------------------------------------------------------------------------
//for touch screen pre setup
int XP = 8, YP = A3, XM = A2, YM = 9;  //most common configuration

#define MINPRESSURE 10
#define MAXPRESSURE 1000


#if (TOUCH_ORIENTATION == LANDSCAPE)
TouchScreen ts(YM, XM, YP, XP, 300);   //re-initialised after diagnose

#define TS_MINX 914
#define TS_MAXX 111

#define TS_MAXY 79
#define TS_MINY 940
int maxDisplayX = 480;
int maxDisplayY = 320;
#endif//I NEED TO FIX THIS NUMBERS
#if (TOUCH_ORIENTATION == PORTRAIT)
TouchScreen ts(XP, YP, XM, YM, 300);   //re-initialised after diagnose
#define TS_MINX 906
#define TS_MAXX 116

#define TS_MINY 92
#define TS_MAXY 952
int maxDisplayX = 320;
int maxDisplayY = 480;

#endif//I NEED TO FIX THIS NUMBERS


TSPoint tp;
uint16_t ID;                          //global point
int lastX = 0;
int lastY = 0;
*/

//---------------------------------------------------------------------------------------------
//for tft screen pre setup

#if (TFTmode == true)


#define TFT_SCLK    13  //13   
#define TFT_MOSI    12  //11
#define TFT_CS      11  //10
#define TFT_RST      8  //8
#define TFT_DC       10  //9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);



/*
 same as
SCK-- SCL
MOSI -- SDA
TFT_CS -- CS
D/C -- RS/DC
RESET-- RES
*/

//for tft screen pre setup
//---------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------
//colors
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
//#define GRAY  0x2408        //un-highlighted cross-hair
#define GRAY      BLUE     //idle cross-hair colour
#define GRAY_DONE RED      //finished cross-hair

uint16_t        Display_Text_Color = GREEN;
uint16_t        Display_Backround_Color = BLACK;
uint16_t        Previous_Backround_Color1;

int FontSizeStartUpMsg = 3;
int FontSizeMainTxt = 16;
int GuageTextSize = 1;


#endif
String errormessage = " ERROR \n\n arduino \n nano \n not \n detected.";
char startingUpMsg[] = "Starting Up";


//colors
//---------------------------------------------------------------------------------------------
//main loops


void setup()
{
#if (Mainserialmessages == true)
    ////serialbegin(9600);
#endif // (Main//serialmessages == true)

#if (UARTserial == true)
    ////serial.begin(115200);
#endif

#if (TFTmode == true)
   // serialprint("\tTFT mode\n");
   // Touch_setup();
   // i2cSetup();
#endif

#if (RCAmode == true)
    //TVoutSetup();
#endif // (RCAmode == true)

#if (TFTmode == false && RCAmode == false)
#if (Main//serialmessages == true)
    //  //serialprint("TFT & RCA mode both disabled\n");
      //delay(200000);
#endif
#endif // (TFTmode == false && RCAmode == false)
#if (TFTmode == true && RCAmode == true)
#if (Main//serialmessages == true)
   // //serialprint("TFT & RCA mode both enabled\n");
   // delay(200000);
#endif
#endif // (TFTmode == true && RCAmode == true)





    //SD_setup();
    /*
    tft.setTextSize(FontSizeMainTxt); //to display gears avaiable

    for (int i = 0; i < amountOfGears; i++)
    {
        writeTxtDisplaySingle(gearRange[i]);
        delay(50);
        removeTxtDisplaySingle(gearRange[i]);
    }
    */
    setupnano();
}

void loop()
{
    //tvoutDemo();
    // i need this to only print when gear recieved starts with 1 and ignore untill it gets a 0\
    //and i need to find out how the tft.print works. 
    //does it send the command to the display and keep going in the loop
    //or does it send the command and wait for the return to say it has been printed
    //either way i need to get the delay right once it has printed.

    //i2cSendData();


    gearRecieved();
    //////serialprint("gearTimer "); ////serialprintln(gearTimer);

    //loopNano();
}

void gearRecieved()
{
    loopNano();

#if (TFTmode == true)
    //Wire.readBytes(replyFromSlave, ANSWERSIZE); //delay(350); reading max data of answersize

        //////serialreadBytes(replyFromSlave, 5); //Read the //serial data and store in var
        //////serialprintln(replyFromSlave); //Print data on //serial Monitor


       //Wire.beginTransmission(SLAVE_ADDRESS);

       //Wire.readBytes(replyFromSlave, ANSWERSIZE); //delay(350); reading max data of answersize
       //*replyFromSlave = Wire.read();
       //String temp = Wire.readString();

    //TvDisplayArray(inloopMsg);

       //////serialprint("\nrecieved1: "); ////serialprintln(temp);
       //////serialprint("recieved2: "); ////serialprintln(replyFromSlave);


    while (Wire.available()) {//i neeed to add that check function to only print a new gear, check last ect.
        // maybe i can add a function to ask nano for the next gear
        //if(tempReplyFromSlave[0] == )
        Wire.readBytes(replyFromSlave, ANSWERSIZE); //delay(350); reading max data of answersize

        *currentGear = replyFromSlave[1];// need to see if this is doing what i think it is doing
        *currentRecievedGear = replyFromSlave[1];
        *lastGear = replyFromSlave[2];
        gearRange[7] = replyFromSlave[1];


        //////serialprint("recieved: "); ////serialprintln(replyFromSlave);//////serialprint(response[0]); ////serialprintln(response[1]);


       // i need to re write this so it constantly reads data ( that it is ) and if the data is a remove request 
       // if the current recieved data is the same as the last then ignore. but if it is different then make the change.
        if (replyFromSlave[0] == *"0")//removing
        {

            //////serialprint("\t\t\t\tremoving "); ////serialprintln(lastGear);   
#if (TFTmode == true)
            tft.setTextSize(FontSizeMainTxt);
#endif
            removeTxtDisplayString(lastGear, 20, 0);  //why is this one so slow??? i'll have to try gear range
            delay(250);
            //removeTxtDisplaySingle(*lastGear); 


            inGear = false;
        }
        else if (replyFromSlave[0] == *"1") //writing
        {

            if (inGear == false) {// this stops it writing to display over and over again.

                    //////serialprint("printing "); ////serialprintln(replyFromSlave[1]);//delay(2500);
                    // now this can be tft.print to print once
#if (TFTmode == true)
                tft.setTextSize(FontSizeMainTxt);
#endif
                writeTxtDisplayString(currentGear, 20, 0); //why is this one so slow???
                delay(250);
                inGear = true;
            }

            if (inGear == false)
            {
                timerFunction();
                onTimeActions();

            }
        }


        //////serialprint("current: "); ////serialprintln(currentGear);
        //////serialprint("\t\tlast: "); ////serialprintln(lastGear);
    }

    //maybe i can just use the "ingear" to tell it when to request so it doesn't constantly ask ....

    //Wire.requestFrom(SLAVE_ADDRESS, sizeOfMessageRecieve); //how often do i want to ask for a new gear?



    Wire.requestFrom(SLAVE_ADDRESS, sizeOfMessageRecieve); //how often do i want to ask for a new gear?
    Wire.readBytes(replyFromSlave, ANSWERSIZE); //delay(350); reading max data of answersize

           //give me message "0N1", i'm expecting the size of that message

    //delay(5000);

    if (!Wire.available())// add tft print message if i2c coms broken.
    {
        TvDisplayArray(ingearrecieved3);
#if (TFTmode == true)
        ////serialprint("oh shit ");
#endif
        writeTxtDisplayString(errormessage, 0, 0);
        //delay(500);
        removeTxtDisplayString(errormessage, 0, 0);
    }

    //Wire.endTransmission(SLAVE_ADDRESS);

    if (//serial.available() > 0) {//i neeed to add that check function to only print a new gear, check last ect.
        // maybe i can add a function to ask nano for the next gear



        byte c = //serial.read();
        if (c == '\`') {}

    *currentGear = replyFromSlave[1];// need to see if this is doing what i think it is doing
        *currentRecievedGear = replyFromSlave[1];
        *lastGear = replyFromSlave[2];
        gearRange[7] = replyFromSlave[1];


    //////serialprint("recieved: "); ////serialprintln(replyFromSlave);//////serialprint(response[0]); ////serialprintln(response[1]);


   // i need to re write this so it constantly reads data ( that it is ) and if the data is a remove request 
   // if the current recieved data is the same as the last then ignore. but if it is different then make the change.
    if (replyFromSlave[0] == *"0")//removing
    {

        //////serialprint("\t\t\t\tremoving "); ////serialprintln(lastGear);   
        removeTxtDisplayString(lastGear, 20, 0);  //why is this one so slow??? i'll have to try gear range
        delay(250);
        //removeTxtDisplaySingle(*lastGear); 


        inGear = false;
    }
    else if (replyFromSlave[0] == *"1") //writing
    {

        if (inGear == false) {// this stops it writing to display over and over again.

                //////serialprint("printing "); ////serialprintln(replyFromSlave[1]);//delay(2500);
                // now this can be tft.print to print once

            writeTxtDisplayString(currentGear, 20, 0); //why is this one so slow???
            delay(250);
            inGear = true;
        }

        if (inGear == false)
        {
            timerFunction();
            onTimeActions();

        }
    }


    //////serialprint("current: "); ////serialprintln(currentGear);
    //////serialprint("\t\tlast: "); ////serialprintln(lastGear);
}

//maybe i can just use the "ingear" to tell it when to request so it doesn't constantly ask ....

//Wire.requestFrom(SLAVE_ADDRESS, sizeOfMessageRecieve); //how often do i want to ask for a new gear?

       //give me message "0N1", i'm expecting the size of that message

//delay(5000);

if (!serial.available())// add tft print message if i2c coms broken.
{
    writeTxtDisplayString(errormessage, 0, 0);
    //delay(500);
    removeTxtDisplayString(errormessage, 0, 0);
}
#endif // (RCAmode == true)

#if (RCAmode == false)

loopNano();

//TvDisplayArray(inloopMsg);


    //Wire.readBytes(replyFromSlave, ANSWERSIZE); //delay(350); reading max data of answersize

    *currentGear = replyFromSlave[1];// need to see if this is doing what i think it is doing
    *currentRecievedGear = replyFromSlave[1];
    *lastGear = replyFromSlave[2];
    gearRange[7] = replyFromSlave[1];


    //////serialprint("recieved: "); ////serialprintln(replyFromSlave);//////serialprint(response[0]); ////serialprintln(response[1]);


   // i need to re write this so it constantly reads data ( that it is ) and if the data is a remove request 
   // if the current recieved data is the same as the last then ignore. but if it is different then make the change.
    if (replyFromSlave[0] == *"0")//removing
    {

        removeTxtDisplayString(lastGear, 20, 0);  //why is this one so slow??? i'll have to try gear range
        delay(250);
        //removeTxtDisplaySingle(*lastGear); 
        inGear = false;
    }
    else if (replyFromSlave[0] == *"1") //writing
    {

        if (inGear == false) {// this stops it writing to display over and over again.

            writeTxtDisplayString(currentGear, 20, 0); //why is this one so slow???
            delay(250);
            inGear = true;
        }

        if (inGear == false)
        {
            timerFunction();
            onTimeActions();

        }
    }



//maybe i can just use the "ingear" to tell it when to request so it doesn't constantly ask ....
       //give me message "0N1", i'm expecting the size of that message

#endif // (RCAmode == true)
}

//main loops
//---------------------------------------------------------------------------------------------
//I2C setup
void i2cSetup()
{
#if i2cMode == true
    Wire.begin(MASTER_ADDRESS);        // join i2c bus (address optional for master)
//Wire.onRequest(requestEvent);
#endif // i2cMode == true


}
//I2C setup
//---------------------------------------------------------------------------------------------
//I2C functions
void requestEvent() { //for future use

  //Wire.write(message); // respond with message of 6 bytes
  // as expected by master
}
void i2cSendData() {

    //////serialprintln("Write data to slave");

    // Write a charatre to the Slave
    //Wire.beginTransmission(SLAVE_ADDRESS);
    //Wire.write("giveMore");
    //Wire.endTransmission();

    //////serialprintln("Receive data");
   // int i = 0;
    //while (Wire.available()) {

       // response[i] = Wire.read();
       // replyFromSlave[i] = Wire.read();
       // i++;
//}

}

//I2C functions
//---------------------------------------------------------------------------------------------
//for SD card functions
/*
void SD_setup()
{

    pinMode(10, OUTPUT);
    ////serialprintln("Initializing SD card...");
    if (!SD.begin(10)) {
        ////serialprintln("\ninitialization failed!");
        //while (1); //if sd card fails don't continue
    }
    ////serialprintln("\ninitialization done.");


    //s_width = my_lcd.Get_Display_Width();
    //s_heigh = my_lcd.Get_Display_Height();

    printDirectory(root, 0);

    //////serialprint("Show BMP files on TFT with ID:0x");

    root = SD.open(namebuf);
    pathlen = strlen(namebuf);

    strcpy(file_name[0], bmpfile_01);
    strcpy(file_name[1], bmpfile_02);
    strcpy(file_name[2], bmpfile_03);
    strcpy(file_name[3], bmpfile_04);

}

void findFileToOpen() {
    char* nm = namebuf + pathlen;
    File f = root.openNextFile();
    uint8_t ret;
    uint32_t start;
    if (f != NULL) {
#ifdef USE_SDFAT
        f.getName(nm, 32 - pathlen);
#else
        strcpy(nm, (char*)f.name());
#endif
        f.close();
        strlwr(nm);
        if (strstr(nm, ".bmp") != NULL && strstr(nm, NAMEMATCH) != NULL) {
            ////serialprint(namebuf);
            ////serialprint(F(" - "));
            tft.fillScreen(0);
            start = millis();
            ret = showBMP(namebuf, 5, 5);
            switch (ret) {
            case 0:
                ////serialprint(millis() - start);
                ////serialprintln(F("ms"));
                delay(5000);
                break;
            case 1:
                ////serialprintln(F("bad position"));
                break;
            case 2:
                ////serialprintln(F("bad BMP ID"));
                break;
            case 3:
                ////serialprintln(F("wrong number of planes"));
                break;
            case 4:
                ////serialprintln(F("unsupported BMP format"));
                break;
            case 5:
                ////serialprintln(F("unsupported palette"));
                break;
            default:
                ////serialprintln(F("unknown"));
                break;
            }
        }
    }
    else root.rewindDirectory();
}
uint8_t showBMP(char* nm, int x, int y)
{
    File bmpFile;
    int bmpWidth, bmpHeight;    // W+H in pixels
    uint8_t bmpDepth;           // Bit depth (currently must be 24, 16, 8, 4, 1)
    uint32_t bmpImageoffset;    // Start of image data in file
    uint32_t rowSize;           // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[(1 << PALETTEDEPTH) + BUFFPIXEL], * palette = NULL;
    uint8_t bitmask, bitshift;
    boolean flip = true;        // BMP is stored bottom-to-top
    int w, h, row, col, lcdbufsiz = (1 << PALETTEDEPTH) + BUFFPIXEL, buffidx;
    uint32_t pos;               // seek position
    boolean is565 = false;      //

    uint16_t bmpID;
    uint16_t n;                 // blocks read
    uint8_t ret;

    if ((x >= tft.width()) || (y >= tft.height()))
        return 1;               // off screen

    bmpFile = SD.open(nm);      // Parse BMP header
    bmpID = read16(bmpFile);    // BMP signature
    (void)read32(bmpFile);     // Read & ignore file size
    (void)read32(bmpFile);     // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile);       // Start of image data
    (void)read32(bmpFile);     // Read & ignore DIB header size
    bmpWidth = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    n = read16(bmpFile);        // # planes -- must be '1'
    bmpDepth = read16(bmpFile); // bits per pixel
    pos = read32(bmpFile);      // format
    if (bmpID != 0x4D42) ret = 2; // bad ID
    else if (n != 1) ret = 3;   // too many planes
    else if (pos != 0 && pos != 3) ret = 4; // format: 0 = uncompressed, 3 = 565
    else if (bmpDepth < 16 && bmpDepth > PALETTEDEPTH) ret = 5; // palette
    else {
        bool first = true;
        is565 = (pos == 3);               // ?already in 16-bit format
        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * bmpDepth / 8 + 3) & ~3;
        if (bmpHeight < 0) {              // If negative, image is in top-down order.
            bmpHeight = -bmpHeight;
            flip = false;
        }

        w = bmpWidth;
        h = bmpHeight;
        if ((x + w) >= tft.width())       // Crop area to be loaded
            w = tft.width() - x;
        if ((y + h) >= tft.height())      //
            h = tft.height() - y;

        if (bmpDepth <= PALETTEDEPTH) {   // these modes have separate palette
            //bmpFile.seek(BMPIMAGEOFFSET); //palette is always @ 54
            bmpFile.seek(bmpImageoffset - (4 << bmpDepth)); //54 for regular, diff for colorsimportant
            bitmask = 0xFF;
            if (bmpDepth < 8)
                bitmask >>= bmpDepth;
            bitshift = 8 - bmpDepth;
            n = 1 << bmpDepth;
            lcdbufsiz -= n;
            palette = lcdbuffer + lcdbufsiz;
            for (col = 0; col < n; col++) {
                pos = read32(bmpFile);    //map palette to 5-6-5
                palette[col] = ((pos & 0x0000F8) >> 3) | ((pos & 0x00FC00) >> 5) | ((pos & 0xF80000) >> 8);
            }
        }

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
        for (row = 0; row < h; row++) { // For each scanline...
            // Seek to start of scan line.  It might seem labor-
            // intensive to be doing this on every line, but this
            // method covers a lot of gritty details like cropping
            // and scanline padding.  Also, the seek only takes
            // place if the file position actually needs to change
            // (avoids a lot of cluster math in SD library).
            uint8_t r, g, b, * sdptr;
            int lcdidx, lcdleft;
            if (flip)   // Bitmap is stored bottom-to-top order (normal BMP)
                pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
            else        // Bitmap is stored top-to-bottom
                pos = bmpImageoffset + row * rowSize;
            if (bmpFile.position() != pos) { // Need seek?
                bmpFile.seek(pos);
                buffidx = sizeof(sdbuffer); // Force buffer reload
            }

            for (col = 0; col < w; ) {  //pixels in row
                lcdleft = w - col;
                if (lcdleft > lcdbufsiz) lcdleft = lcdbufsiz;
                for (lcdidx = 0; lcdidx < lcdleft; lcdidx++) { // buffer at a time
                    uint16_t color;
                    // Time to read more pixel data?
                    if (buffidx >= sizeof(sdbuffer)) { // Indeed
                        bmpFile.read(sdbuffer, sizeof(sdbuffer));
                        buffidx = 0; // Set index to beginning
                        r = 0;
                    }
                    switch (bmpDepth) {          // Convert pixel from BMP to TFT format
                    case 24:
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        color = tft.color565(r, g, b);
                        break;
                    case 16:
                        b = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        if (is565)
                            color = (r << 8) | (b);
                        else
                            color = (r << 9) | ((b & 0xE0) << 1) | (b & 0x1F);
                        break;
                    case 1:
                    case 4:
                    case 8:
                        if (r == 0)
                            b = sdbuffer[buffidx++], r = 8;
                        color = palette[(b >> bitshift) & bitmask];
                        r -= bmpDepth;
                        b <<= bmpDepth;
                        break;
                    }
                    lcdbuffer[lcdidx] = color;

                }
                tft.pushColors(lcdbuffer, lcdidx, first);
                first = false;
                col += lcdidx;
            }           // end cols
        }               // end rows
        tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen
        ret = 0;        // good render
    }
    bmpFile.close();
    return (ret);
}
void printDirectory(File dir, int numTabs) {

    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            // no more files
            break;
        }
        for (uint8_t i = 0; i < numTabs; i++) {
            ////serialprint('\t');
        }
        ////serialprint(entry.name());
        if (entry.isDirectory()) {
            ////serialprintln("/");
            printDirectory(entry, numTabs + 1);
        }
        else {
            // files have sizes, directories do not
            ////serialprint("\t\t");
            ////serialprintln(entry.size(), DEC);
        }
        entry.close();
    }
}
//for SD card functions
*/
//---------------------------------------------------------------------------------------------
//for touch screen functions

void Touch_setup() {
#if (TFTmode == true)
    char correctID[7] = "0x9486";
    amountOfGears = amountOfGears - 2; // -2 for extra in gear range or -4 for the string terminating characters
    tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
    tft.fillScreen(Display_Backround_Color);
    tft.setTextColor(Display_Text_Color);
    tft.setCursor(0, 0);

    /*
        if (TOUCH_ORIENTATION == LANDSCAPE) {

            tft.setCursor(0, 0);
            tft.setTextColor(WHITE, BLACK);
            tft.println("landscape mode");
            tft.setRotation(TOUCH_ORIENTATION);
            delay(1000);
        }
        else if (TOUCH_ORIENTATION == PORTRAIT) {

            tft.setCursor(0, 0);
            tft.setTextColor(WHITE, BLACK);
            tft.println("PORTRAIT mode");
            tft.setRotation(TOUCH_ORIENTATION);
            delay(1000);
        }

     */
     //i need to declare lcd pins here
    pinMode(TFT_SCLK, OUTPUT); delay(10);
    pinMode(TFT_MOSI, OUTPUT); delay(10);
    pinMode(TFT_CS, OUTPUT);   delay(10);
    pinMode(TFT_RST, OUTPUT);  delay(10);
    pinMode(TFT_DC, OUTPUT);   delay(10);

    //-- writeing data to display
    tft.setTextSize(FontSizeStartUpMsg);
    writeTxtDisplayString(startingUpMsg, 0, 0);

    // -- wring over data on display to erase it
    delay(250);
    removeTxtDisplayString(startingUpMsg, 0, 0);
    tft.setTextSize(FontSizeMainTxt);
#endif
}


void writeTxtDisplaySingle(char displayMessage) //single Characters
{
#if (TFTmode == true)
    tft.setCursor(20, 0);
    tft.setTextColor(Display_Text_Color);
    tft.print(displayMessage);
    delay(100);

#endif

#if(RCAmode == true)
    TV.println(displayMessage);
    TV.delay(3000);
    TV.clear_screen();
#endif
}
void removeTxtDisplaySingle(char displayMessage) //single Characters
{
#if (TFTmode == true)
    tft.setCursor(20, 0);
    tft.setTextColor(Display_Backround_Color);
    tft.print(displayMessage);
    delay(100);

#endif
#if(RCAmode == true)
    TvDisplayArray(&displayMessage);
#endif
}
//-------------------------------
void writeTxtDisplayString(String displayMessage, int x, int y)
{//                 string of characters  

#if (TFTmode == true)

    //int lenDisplayMsg = displayMessage.length();   
    //Wire.endTransmission(SLAVE_ADDRESS);
    if (displayMessage == errormessage)
    {
        tft.setCursor(x, y);
        tft.setTextColor(Display_Text_Color);
        tft.setTextSize(2);
        tft.print(displayMessage);
    }
    else {

        tft.setCursor(x, y);
        tft.setTextColor(Display_Text_Color);
        //tft.setTextSize(FontSizeMainTxt);
        //Previous_Backround_Color = tft.readPixel(x, y);

        //whats faster to print, entire thing at once or 1 character at a time
        /*for (int i = 0; i <= lenDisplayMsg; i++) {
            //prevColorArray[Previous_Backround_Color] = tft.readPixel(x+i, y);

            //tft.print(displayMessage[i]);
            //prevColorArray[Previous_Backround_Color]++;
        }*/
        tft.print(displayMessage);
        //delay(250);
#if (TFTmode == true)
        ////serialprint("\n\t\tprinting "); ////serialprintln(displayMessage);
#endif
    }
    //Wire.beginTransmission(SLAVE_ADDRESS);

#endif

#if(RCAmode == true)

#endif
}
void removeTxtDisplayString(String displayMessage, int x, int y)
{//                   string of characters   
#if (TFTmode == true)
    //int lenDisplayMsg = displayMessage.length();
    //Wire.endTransmission(SLAVE_ADDRESS);
    if (displayMessage == errormessage)
    {
        tft.setCursor(x, y);
        tft.setTextColor(Display_Backround_Color);
        tft.setTextSize(2);
        tft.print(displayMessage);
    }
    else {


        tft.setCursor(x, y);
        tft.setTextColor(Display_Backround_Color);
        //whats faster to print, entire thing at once or 1 character at a time
        /*for (int i = 0; i <= lenDisplayMsg; i++) {

            //tft.setTextColor(prevColorArray[Previous_Backround_Color]);
            //tft.print(displayMessage[i]);
            //prevColorArray[Previous_Backround_Color]++;
        }*/

        //tft.setTextColor(Previous_Backround_Color);
        tft.print(displayMessage);
        //delay(250);
        ////serialprint("\n\t\t\tremoving "); ////serialprint(displayMessage);  
    }
    //Wire.beginTransmission(SLAVE_ADDRESS);
#endif


#if(RCAmode == true)
#endif


}

//for touch screen funtions
//---------------------------------------------------------------------------------------------

void timerFunction()
{ // need to find out why this doesn't work all the time
//perhaps use //serialmonitor to check the seconds.. it hink it may be messing up because if the seconds go over 10 it has error 404
#if (TFTmode == true)
    if (millis() >= (previousTime)) {
        previousTime = previousTime + 1000;  // use 100000 for uS
        seconds = seconds + 1;
        //////serialprint(" seconds: ");
        //////serialprintln(seconds);

        if (seconds == 60)
        {
            seconds = 0;
            minutes = minutes + 1;
        }
        if (minutes == 60)
        {
            minutes = 0;
            hours = hours + 1;
        }
    } // end 1 second
#endif
}
void onTimeActions()
{
#if (TFTmode == true)

    if (seconds == 60)
    {
#if (TFTmode == true)
        tft.setTextSize(FontSizeMainTxt);
#endif
        removeTxtDisplayString(lastGear, 20, 0);  //why is this one so slow??? i'll have to try gear range 
        delay(250);
        writeTxtDisplayString(currentGear, 20, 0); //why is this one so slow???
    }
#endif

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setupnano() {

    pinMode(switchUP, INPUT_PULLUP);// what does this do ?
    pinMode(switchDOWN, INPUT_PULLUP);
    pinMode(switchLEFT, INPUT_PULLUP);
    pinMode(switchRIGHT, INPUT_PULLUP);


    pinMode(LED_BUILTIN, OUTPUT);


#if (RCAmode == false)
    Serial.begin(9600);
    i2cSetupnano();
    amountOfGears = strlen(gearRange);
#endif
    TVoutSetup();
}

void loopNano() {
    tvoutDemo();

    GearSelectnano();

    if (ingear == true) { //maybe i can make a real request thing?

#if (RCAmode == false)
        Wire.onRequest(sendToUnoEventnano); // need to be careful with this one
#endif
        ingear = false;                 // because master (uno) is constantly requesting data this fucks shit up.
    }


#if (GearDebuggingAutomatic == true)
    debugGearTimer++;
#endif

}
//---------------------------------------------------------------------------------------------
//I2C setup
void i2cSetupnano()
{
    // Initialize I2C communications as Slave
#if (RCAmode == false)
    Wire.begin(SLAVE_ADDRESS);
#endif
    // Function to run when data requested from master

    //Wire.onRequest(sendToUnoEventnano); // need to be careful with this one, because

    // Function to run when data received from master

    //Wire.onReceive(receiveEventnano );
    // Setup Serial Monitor     
    //Serial.println("I2C Slave Demonstration");
}
//I2C setup
//---------------------------------------------------------------------------------------------
//I2C functions
void receiveEventnano() {//wtf is this doing?
#if (RCAmode == false)
    String recievedString;
    char recievedChar[10] = "";
    // Read while data received
    while (0 < Wire.available()) {
        //while (Wire.available()) {
        char x = Wire.read();
        recievedString += x;

        //Serial.print(x);
    }
    for (int i = 0; i < sizeof(recievedString) + 1; i++)
    {
        recievedChar[i] = recievedString[i];
    }


    if (strcmp(recievedChar, "giveMore") == 0)
    {
        Serial.println("helpmeplz\n");
    }

    //Serial.print("\n");
    // Print to Serial Monitor
    Serial.print("Received event: ");
    Serial.println(recievedChar);
#endif

}
void sendToUnoEventnano() {

    //Serial.println(sendToUno);

    // Send response back to Master  

    //Serial.print("current: "); Serial.println(currentGear);
    //Serial.print("\t\tlast: "); Serial.println(lastGear);
    int returnValue = 0;

    //returnValue = getLastGearnano();
    //getLastGearnano();


    if (writeOrRemove[0] == *"0")
    {
        //*lastGear = writeOrRemove[1];

        for (int i = 0; i < 2; i++) {
            *trueLastGear = writeOrRemove[1];

            //strcpy(writeOrRemove[2], trueLastGear);

#if (RCAmode == false)
            Serial.print("\tremoving: ");
            Serial.println(writeOrRemove);

            //Wire.beginTransmission(MASTER_ADDRESS);
            Wire.write(writeOrRemove, ANSWERSIZE);
            //Wire.endTransmission(MASTER_ADDRESS);
            delay(10);
#endif
        }

    }
    else if (writeOrRemove[0] == *"1")
    {
        writeOrRemove[2] = *trueLastGear;
#if (RCAmode == false)
        Serial.print("\t   writing: ");
        Serial.println(writeOrRemove);

        //Wire.beginTransmission(MASTER_ADDRESS);
        Wire.write(writeOrRemove, ANSWERSIZE);
        //Wire.endTransmission(MASTER_ADDRESS);
        delay(10);
#endif

    }

}
//I2C functions
//---------------------------------------------------------------------------------------------
void GearSelectnano()
{

#if (GearDebuggingnormal == true) //i could add the or if both debugging are off
    readSwitchesnano();
#endif // (GearDebuggingAutomatic == false)


#if (GearDebuggingManual == true)
    //debugReadSwitchesnano();
    debugSerialInputSwitchesnano();
    debugReadSwitchesnano();
#endif // (GearDebuggingAutomatic == true)

#if ( GearDebuggingAutomatic  == true)
    debugReadSwitchesnano();
#endif




    //1st/2nd gears
    if (stateofsensorLeft == true) {

        if (stateofsensorUp == true) {
            if (strcmp(currentGear, gear1) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                             // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear1);
                strcpy(lastGear, gear1);
            }
            if (strcmp(currentGear, gear1) != 0) {
                writeTxtDisplaySinglenano(gearRange[First]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                           //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear1);
                strcpy(currentGear, gear1);

            }

        }
        
        else if (stateofsensorDown == true) {
            if (strcmp(currentGear, gear2) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                             // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear2);
                strcpy(lastGear, gear2);
            }
            if (strcmp(currentGear, gear2) != 0) {
                writeTxtDisplaySinglenano(gearRange[Second]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                          //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear2);
                strcpy(currentGear, gear2);

            }

        }
        
        else if (stateofsensorUp == false && stateofsensorDown == false)// inbetween first/second gear show nutural
        {
            //start nutural timer
            if (nuturalGear == false) {
                //startMillis = millis();
                nuturalTimerFunctionnano();
            }
            if (nuturalGear == true)
            {
                if (strcmp(currentGear, gear0) != 0) {
                    removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                                 // if last gear the same as gear1 ignore removing it
                    strcpy(&gearRange[LastGear], gear0);
                    strcpy(lastGear, gear0);
                }
                if (strcmp(currentGear, gear0) != 0) {
                    writeTxtDisplaySinglenano(gearRange[Nutural]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                                 //is current gear the same as gear1 if its not ignore it
                    strcpy(&gearRange[CurrentGear], gear0);
                    strcpy(currentGear, gear0);

                }
            }
        }
        
    }


    // 5th/reverse gears
    /*
    else if (stateofsensorRight == true) {

        if (stateofsensorUp == true) {//5th gear
            if (strcmp(currentGear, gear5) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                 // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear5);
                strcpy(lastGear, gear5);
            }
            if (strcmp(currentGear, gear5) != 0) {
                writeTxtDisplaySinglenano(gearRange[Fifth]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                          //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear5);
                strcpy(currentGear, gear5);
            }

        }
        else if (stateofsensorDown == true) {//reverse gear

            if (strcmp(currentGear, gear6) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                     // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear6);
                strcpy(lastGear, gear6);
            }
            if (strcmp(currentGear, gear6) != 0) {
                writeTxtDisplaySinglenano(gearRange[Sixth]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                          //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear6);
                strcpy(currentGear, gear6);
            }
        }
        else if (stateofsensorUp == false && stateofsensorDown == false)
        {
            //start nutural timer
            if (nuturalGear == false) {
                //startMillis = millis();
                nuturalTimerFunctionnano();
            }
            if (nuturalGear == true)
            {

                if (strcmp(currentGear, gear0) != 0) {
                    removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                                    // if last gear the same as gear1 ignore removing it
                    strcpy(&gearRange[LastGear], gear0);
                    strcpy(lastGear, gear0);
                }
                if (strcmp(currentGear, gear0) != 0) {
                    writeTxtDisplaySinglenano(gearRange[Nutural]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                                    //is current gear the same as gear1 if its not ignore it
                    strcpy(&gearRange[CurrentGear], gear0);
                    strcpy(currentGear, gear0);
                }
            }
        }


    }
    // 3rd/4th gears
    else if (stateofsensorUp == false || stateofsensorDown == false)
    {
        if (stateofsensorUp == true) { //3rd gear

            if (strcmp(currentGear, gear3) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                     // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear3);
                strcpy(lastGear, gear3);
            }
            if (strcmp(currentGear, gear3) != 0) {
                writeTxtDisplaySinglenano(gearRange[Thrid]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                          //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear3);
                strcpy(currentGear, gear3);

            }
        }
        else if (stateofsensorDown == true) { //4th gear    
            if (strcmp(currentGear, gear4) != 0) {
                removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                     // if last gear the same as gear1 ignore removing it
                strcpy(&gearRange[LastGear], gear4);
                strcpy(lastGear, gear4);
            }
            if (strcmp(currentGear, gear4) != 0) {
                writeTxtDisplaySinglenano(gearRange[Forth]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                          //is current gear the same as gear1 if its not ignore it
                strcpy(&gearRange[CurrentGear], gear4);
                strcpy(currentGear, gear4);


            }
        }
        else //nutural gear
        {
            //start nutural timer
            if (nuturalGear == false) {
                //startMillis = millis();
                nuturalTimerFunctionnano();
            }
            if (nuturalGear == true)
            {
                if (strcmp(currentGear, gear0) != 0) {
                    removeTxtDisplaySinglenano(gearRange[LastGear]); // is last gear the same as gear1 if not enter statement and remove the last gear
                                                                 // if last gear the same as gear1 ignore removing it
                    strcpy(&gearRange[LastGear], gear0);
                    strcpy(lastGear, gear0);
                }
                if (strcmp(currentGear, gear0) != 0) {
                    writeTxtDisplaySinglenano(gearRange[Nutural]);   //is current gear the same as gear1 if it is enter and write the first gear
                                                                 //is current gear the same as gear1 if its not ignore it
                    strcpy(&gearRange[CurrentGear], gear0);
                    strcpy(currentGear, gear0);

                }
            }
        }

    }
    */
}
//single characters to print
void writeTxtDisplaySinglenano(char displayMessage)
{                                                //single Characters
    WOR = 1;
    if (stateofsensorUp == true) {
        nuturalGear = false;
    }
    if (stateofsensorDown == true) {
        nuturalGear = false;
    }

    strcpy(sendToUno, &displayMessage);
    //Serial.print("displayMessage"); Serial.println(displayMessage);
    ingear = true;

    if (WOR == 1) {
        strcpy(&writeOrRemove[0], "1");
        writeOrRemove[1] = *sendToUno;
        strcpy(&writeOrRemove[2], lastGear);

        //Serial.print("currentGear"); Serial.println(currentGear);
    }
    delay(200);
    //sendToUnoEventnano();

}
void removeTxtDisplaySinglenano(char displayMessage)
{                                                   //single Characters
    WOR = 0;

    strcpy(sendToUno, &displayMessage);
    ingear = false;

    if (WOR == 0) {
        strcpy(&writeOrRemove[0], "0");
        writeOrRemove[1] = *sendToUno;
        strcpy(&writeOrRemove[2], lastGear);
    }
    delay(200);
    //sendToUnoEventnano();

}
//strings to print
void writeTxtDisplayStringnano(char displayMessage[])
{//                                            string of characters  

#if(SerialDebugging == true )
    Serial.print("WriteString\t");
    Serial.println(displayMessage);
#endif
    //strcpy(sendToUno, &displayMessage[0]);

}
void removeTxtDisplayStringnano(char displayMessage[])
{//                                            string of characters
#if(SerialDebugging == true )
    Serial.print("RemoveString\t");
    Serial.println(displayMessage);
#endif


}

void nuturalTimerFunctionnano() {


    currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)




    // might be able to do a simalir thing ... i just need to read the current mills. and after I'VE counted the time. have the if statement trigger on that current mills number

//    if (currentMillis >= period)  // if greater than should probally do if the value is inbetween this range......
    
    if (currentMillis - startMillis >= period)
    {
        //startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
        //seconds = seconds + 1;
    }
    /*
    if (nuturalGear == false) {
        if (seconds == timeToEnableNuturalGear) {
            nuturalGear = true;
            seconds = 0;
        }
        else if (seconds >= timeToEnableNuturalGear + 2) { seconds = 0; } //prevents millis overflow
    }

    else {
        nuturalGear = false;
    }*/
    //# if(MainSerialmessages == true )
        //Serial.print(" seconds: ");
        //Serial.println(seconds);
    //#endif

}
void readSwitchesnano()
{

    // int temp = digitalRead(switchUP);
    //int temp2 = digitalRead(switchDOWN);


    int temp1 = analogRead(switchUP); delay(10);
    int temp2 = analogRead(switchDOWN); delay(10);
    int temp3 = analogRead(switchLEFT); delay(10);
    int temp4 = analogRead(switchRIGHT); delay(10);


    float voltageUP;
    float voltageDOWN;
    float voltageLEFT;
    float voltageRIGHT;

    if (temp1 < 1000) {
        voltageUP = temp1 * (5.0 / 1023.0);
    }
    else {
        voltageUP = 0;
    }//---------------------------------------------------------------
    if (temp2 < 1000) {
        voltageDOWN = temp2 * (5.0 / 1023.0);
    }
    else {
        voltageDOWN = 0;
    }//---------------------------------------------------
    if (temp3 < 1000) {
        voltageLEFT = temp3 * (5.0 / 1023.0);
    }
    else {
        voltageLEFT = 0;
    }//-------------------------------------------------------------
    if (temp4 < 1000) {
        voltageRIGHT = temp4 * (5.0 / 1023.0);
    }
    else {
        voltageRIGHT = 0;
    }//-------------------------------------------------




    if (voltageUP > 3.0) {
        stateofsensorUp = true;
    }
    else {
        stateofsensorUp = false;
    }//--------------------------------------------------------
    if (voltageDOWN > 3.0) {
        stateofsensorDown = true;
    }
    else {
        stateofsensorDown = false;
    }//--------------------------------------------------------
    if (voltageLEFT > 3.0) {
        stateofsensorLeft = true;
    }
    else {
        stateofsensorLeft = false;
    }//--------------------------------------------------------
    if (voltageRIGHT > 3.0) {
        stateofsensorRight = true;
    }
    else {
        stateofsensorRight = false;
    }//--------------------------------------------------------

/*
    Serial.print("up: ");
    Serial.print(voltageUP); Serial.print("\t\t"); Serial.print(temp1);

    Serial.print("\tdown: ");
    Serial.print(voltageDOWN); Serial.print("\t\t"); Serial.println(temp2);
    */
}
void debugReadSwitchesnano()
{
#if (GearDebuggingAutomatic == true)
    if (debugGearTimer >= 7) {
        debugGearTimer = 0;
    }

    delay(1500);
#endif

    if (debugGearTimer == 0) //start with all off
    {
        stateofsensorUp = false;
        stateofsensorDown = false;
        stateofsensorLeft = false;
        stateofsensorRight = false;
    }
    else if (debugGearTimer == 1) //simulate first gear
    {
        stateofsensorUp = true;
        stateofsensorDown = false;
        stateofsensorLeft = true;
        stateofsensorRight = false;
    }
    else if (debugGearTimer == 2) {
        stateofsensorUp = false;
        stateofsensorDown = true;
        stateofsensorLeft = true;
        stateofsensorRight = false;
    }
    else if (debugGearTimer == 3) {
        stateofsensorUp = true;
        stateofsensorDown = false;
        stateofsensorLeft = false;
        stateofsensorRight = false;
    }
    else if (debugGearTimer == 4) {
        stateofsensorUp = false;
        stateofsensorDown = true;
        stateofsensorLeft = false;
        stateofsensorRight = false;
    }
    else if (debugGearTimer == 5) {
        stateofsensorUp = true;
        stateofsensorDown = false;
        stateofsensorLeft = false;
        stateofsensorRight = true;
    }
    else if (debugGearTimer == 6) {
        stateofsensorUp = false;
        stateofsensorDown = true;
        stateofsensorLeft = false;
        stateofsensorRight = true;
    }
    else if (debugGearTimer == -1) {// wtf is this doing ????
        stateofsensorUp = true;
        stateofsensorDown = true;
        stateofsensorLeft = true;
        stateofsensorRight = true;
    }

}
void debugSerialInputSwitchesnano()
{
#if (RCAmode == false)
    if (Serial.available() > 0) {
        // read the incoming byte:
        //Serial.print("input ");
        SerialInput = Serial.readString();

        // say what you got:
        //Serial.print(" Received: ");
        //Serial.println(SerialInput);
    }

    if (SerialInput == "0")
    {
        Serial.println("nutural gear");
        SerialInput = "-1";
        debugGearTimer = 0;

    }
    if (SerialInput == gear1)
    {
        Serial.println("first gear");
        SerialInput = "-1";
        debugGearTimer = 1;
    }
    if (SerialInput == gear2)
    {
        Serial.println("second gear");
        SerialInput = "-1";
        debugGearTimer = 2;
    }
    if (SerialInput == gear3)
    {
        Serial.println("third gear");
        SerialInput = "-1";
        debugGearTimer = 3;
    }
    if (SerialInput == gear4)
    {
        Serial.println("forth gear");
        SerialInput = "-1";
        debugGearTimer = 4;
    }
    if (SerialInput == gear5)
    {
        Serial.println("fifth gear");
        SerialInput = "-1";
        debugGearTimer = 5;
    }
    if (SerialInput == gear6)
    {
        Serial.println("reverse gear");
        SerialInput = "-1";
        debugGearTimer = 6;
    }
    else
    {
        //need a print once thing
        //Serial.println("input error");

    }
#endif
}
int getLastGearnano()
{

    if (loopCounter == 0) {
        *trueCurrentGear = writeOrRemove[1];
    }

    if (loopCounter == 1) {
        //last = first;
        strcpy(trueLastGear, trueCurrentGear);
#if (RCAmode == false)
        Serial.print("true current"); Serial.println(trueCurrentGear);
        Serial.print("true last"); Serial.println(trueLastGear);
#endif
    }

    loopCounter++;
    if (loopCounter == 2)
    {
        loopCounter = 0;
    }


    if (loopCounter == 1)
    {
        strcpy(lastGear, trueLastGear);
        return 1;
    }

}
//---------------------------------------------------------------------------------------------
//tvout demo

void tvoutDemo() {
    TV.clear_screen();

    x = 0;
    y = 0;
    for (char i = 32; i < 127; i++) {              // this prints the alt code characters from 32 -127
        TV.print_char(x * 6, y * 8, i);                    // the res of the characters in pixel size ?
        x++;
        if (x >= TV.char_line()) {                   // i think this one is for if the character prints and gets to the end of the display make new line and continue
            y++;
            x = 0;
        }
    }

    TV.delay(500);
    TV.clear_screen();
    TV.println("Fill the Screen\nPixel by Pixel");
    TV.delay(500);
    TV.clear_screen();
    for (x = 0; x < TV.hres(); x++) {
        for (y = 0; y < TV.vres(); y++) {
            TV.set_pixel(x, y, 1); //this is the color 0= black 1 = white
        }
    }
    TV.delay(500);
    TV.clear_screen();


    TV.print("Draw some lines");
    TV.delay(500);
    x = TV.hres() - 1;
    for (y = 0; y < TV.vres(); y++) {
        TV.draw_line(0, y, x - y, y, 2);
    }
    TV.delay(500);
    TV.clear_screen();
}

void TvDisplayArray(char TVdisplayMessage[])
{//                                            array of characters  
    delay(200);

    TV.println(TVdisplayMessage);
    TV.delay(3000);
    TV.clear_screen();

}
void TVoutSetup() {
    x = 0;
    y = 0;
    TV.begin(NTSC);  //for devices with only 1k sram(m168) use TV.begin(_NTSC,128,56)
    //TV.begin(PAL);  //for devices with only 1k sram(m168) use TV.begin(_NTSC,128,56)
    TV.select_font(font6x8);
    TV.delay(50);
    TV.clear_screen();

    TvDisplayArray(startingUpMsg);
    TV.delay(1000);
}
