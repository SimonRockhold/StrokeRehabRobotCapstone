// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF


void displayEyes();
void closeEyes();
void makeNoise();

//Rectangle 
int width = 40;
int height = 64;

//This will center your cursor
int x0 = (128/2) - (width/2);
int y0 = (64/2) - (height/2);

