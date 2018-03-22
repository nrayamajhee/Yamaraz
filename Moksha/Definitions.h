#define NUM_SENSORS   8
#define TIMEOUT       2500
#define EMITTER_PIN   2
#define IR_THRESHHOLD 700
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
  CYAN,
  PURPLE,
  YELLOW,
  BLUE,
  GREEN,
  RED,
  GRAY,
  INVALID
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
