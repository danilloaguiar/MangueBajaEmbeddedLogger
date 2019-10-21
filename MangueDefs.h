#define CAN_IER         (*((volatile unsigned long *)0x40006414))


//CAN ID
#define SYNC_ID         0x001       // message for bus sync
#define FLAGS_ID        0x101       // 1by
#define IMU_ACC_ID      0x200       // 8by = accelerometer data (3D) + timestamp
#define IMU_DPS_ID      0x201       // 8by = gyroscope data (3D) + timestamp 
#define SPEED_ID        0x300       // 4by = speed + timestamp
#define RPM_ID          0x304       // 4by = rpm + timestamp
#define TEMPERATURE_ID  0x400       // 4by = engine temp. + cvt temp. + timestamp
#define FUEL_ID         0x500       // 3by = fuel level + timestamp

//Logger Definitiions
#define SERIAL_BAUD 1000000                     // Board baud rate
#define BUFFER_SIZE 200                         // Acquisition buffer
#define SAVE_WHEN 50                            // Number of packets to save (fail safe)
#define SAMPLE_FREQ 200                         // Frequency in Hz

//Data Structs 
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
    imu_t imu;
    uint16_t rpm;
    uint16_t speed;
    uint8_t temp;
    uint8_t flags;      // MSB - BOX | BUFFER FULL | NC | NC | FUEL_LEVEL | SERVO_ERROR | CHK | RUN - LSB
    uint32_t timestamp;
} packet_t;

typedef enum
{
    OPEN,       // open archive
    SAVE,       // save data
    CLOSE,      // close archive
    CAN         // 
} state_t;