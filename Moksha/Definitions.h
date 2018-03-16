#define NUM_SENSORS   8
#define TIMEOUT       1000
#define EMITTER_PIN   2
#define IR_THRESHHOLD 400
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

struct Debug {
  bool steps;
  bool ir;
  bool servo;
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
  volatile int alignCounter;
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
