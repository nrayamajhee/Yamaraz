#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   2
#define IR_THRESHHOLD 700
#define LIGHT_PIN 30
#define SERVO_PIN 10
#define MAGNET_PIN 11
enum Direction {
  FRONT,
  BACK,
  RIGHT,
  LEFT,
  SLEFT,
  SRIGHT,
  UP,
  DOWN,
  ALL
};

enum Color {
  WHITE,
  CYAN,
  PURPLE,
  YELLOW,
  BLUE,
  GREEN,
  RED,
  GRAY,
  INVALID
};

struct RGB {
  int r;
  int g;
  int b;
};

RGB colors[8] = {
  {1200, 1900, 1900},
  {1000, 700, 700},
  {3500, 2500, 1600},
  {1000, 1000, 1200},
  {1500, 1700, 1000},
  {1400, 500, 400},
  {800, 800, 700},
  {700, 600, 490},
};

struct Debug {
  bool steps;
  bool ir;
  bool servo;
  bool light;
  bool motion;
};

struct Motors {
  volatile bool running;
  volatile int steps;
  volatile long totalSteps;
  int maxSpeed;
  int minSpeed;
  volatile int speed;
  volatile float alignRatio;
  volatile bool accelerate;
};

struct IR {
  unsigned int  sensorValues  [NUM_SENSORS];
  bool          filteredValues[NUM_SENSORS];
};

struct Servo {
  volatile bool state;
  volatile int count;
  volatile int maxCount;
};
