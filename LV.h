#define TABLET_ID 1
#define MCU_ID 2
#define PERIPH_ID 3
#define BMS_ID 4
#define MC_ID 5

#define PERIPH_SENSOR_COUNT 0
#define PERIPH_SENSOR_THRESHOLD 0.1
#define PERIPH_SENSOR_DELAY 100

typedef struct sensor_s {
  unsigned char (*read)();
  char *name;
} sensor;

boolean debug = false;
boolean sendData = true;