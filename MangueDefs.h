#define CAN_IER         (*((volatile unsigned long *)0x40006414))

#define BUFFER_SIZE     50
#define THROTTLE_MID    0x00
#define THROTTLE_RUN    0x01
#define THROTTLE_CHOKE  0x02

#define SYNC_ID         0x001       // message for bus sync
#define FLAGS_ID        0x101       // 1by
#define IMU_ACC_ID      0x200       // 8by = accelerometer data (3D) + timestamp
#define IMU_DPS_ID      0x201       // 8by = gyroscope data (3D) + timestamp 
#define SPEED_ID        0x300       // 4by = speed + timestamp
#define RPM_ID          0x304       // 4by = rpm + timestamp
#define TEMPERATURE_ID  0x400       // 4by = engine temp. + cvt temp. + timestamp
#define FUEL_ID         0x500       // 3by = fuel level + timestamp



typedef struct
{
    int16_t acc_x;
    int16_t acc_y;
    int16_t acc_z;
    int16_t dps_x;
    int16_t dps_y;
    int16_t dps_z;
} imu_t;
    
typedef struct
{  
    imu_t imu[4];
    uint16_t rpm;
    uint16_t speed;
    uint8_t temperature;
    uint8_t flags;      // MSB - BOX | BUFFER FULL | NC | NC | FUEL_LEVEL | SERVO_ERROR | CHK | RUN - LSB
    uint32_t timestamp;
} packet_t;

typedef enum
{
    IDLE_ST,        // wait
    TEMP_ST,        // measure temperatures
    FUEL_ST,        // proccess fuel data sampling
    RPM_ST,         // calculate speed
    THROTTLE_ST,    // write throttle position (PWM)
    RADIO_ST,       // send data for box via radio (SPI)
    DEBUG_ST        // send data for debug
} state_t;