// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

// define pins
int buttonPin = 3;
int buzzerPin = 2;
int ledPin = 7; //Just a placeholder for now, change upon wiring

bool eyesOn = false;
//bool buttonPress = false;

//Rectangle 
 const int width = 40;
 const int height = 63;
 const int radius = 20;

//This will center your cursor
int x0 = (128/2) - (width/2);
int y0 = (64/2) - (height/2);

int x1 = (128/2);
int y1 = 64;
int r = 60;

void displayEyes();
void closeEyes();
void chord();
//void makeNoise();
//void blink();
