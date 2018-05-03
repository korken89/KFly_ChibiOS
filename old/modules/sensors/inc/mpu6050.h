#ifndef __MPU6050_H
#define __MPU6050_H

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/
/* Conversions */
#define MPU6050_DPS250_TO_RADPS             (0.0174532925f / 131.0f)
#define MPU6050_DPS500_TO_RADPS             (0.0174532925f / 65.5f)
#define MPU6050_DPS1000_TO_RADPS            (0.0174532925f / 32.8f)
#define MPU6050_DPS2000_TO_RADPS            (0.0174532925f / 16.4f)

#define MPU6050_LSB_TO_2G                   (1.0f / 16384.0f)
#define MPU6050_LSB_TO_4G                   (1.0f / 8192.0f)
#define MPU6050_LSB_TO_8G                   (1.0f / 4096.0f)
#define MPU6050_LSB_TO_16G                  (1.0f / 2048.0f)

/* MPU6050 Address */
#define MPU6050_ADDRESS_AD0_LOW             0x68 // Address pin low (GND)
#define MPU6050_ADDRESS_AD0_HIGH            0x69 // Address pin high (VCC)

/* Sizes */
#define MPU6050_ACCEL_GYRO_TEMP_DATA_SIZE   14

/* MPU6050 Register Map */
#define MPU6050_RA_XG_OFFS_TC               0x00
#define MPU6050_RA_YG_OFFS_TC               0x01
#define MPU6050_RA_ZG_OFFS_TC               0x02
#define MPU6050_RA_X_FINE_GAIN              0x03
#define MPU6050_RA_Y_FINE_GAIN              0x04
#define MPU6050_RA_Z_FINE_GAIN              0x05
#define MPU6050_RA_XA_OFFS_H                0x06
#define MPU6050_RA_XA_OFFS_L_TC             0x07
#define MPU6050_RA_YA_OFFS_H                0x08
#define MPU6050_RA_YA_OFFS_L_TC             0x09
#define MPU6050_RA_ZA_OFFS_H                0x0A
#define MPU6050_RA_ZA_OFFS_L_TC             0x0B
#define MPU6050_RA_XG_OFFS_USRH             0x13
#define MPU6050_RA_XG_OFFS_USRL             0x14
#define MPU6050_RA_YG_OFFS_USRH             0x15
#define MPU6050_RA_YG_OFFS_USRL             0x16
#define MPU6050_RA_ZG_OFFS_USRH             0x17
#define MPU6050_RA_ZG_OFFS_USRL             0x18
#define MPU6050_RA_SMPLRT_DIV               0x19
#define MPU6050_RA_CONFIG                   0x1A
#define MPU6050_RA_GYRO_CONFIG              0x1B
#define MPU6050_RA_ACCEL_CONFIG             0x1C
#define MPU6050_RA_FF_THR                   0x1D
#define MPU6050_RA_FF_DUR                   0x1E
#define MPU6050_RA_MOT_THR                  0x1F
#define MPU6050_RA_MOT_DUR                  0x20
#define MPU6050_RA_ZRMOT_THR                0x21
#define MPU6050_RA_ZRMOT_DUR                0x22
#define MPU6050_RA_FIFO_EN                  0x23
#define MPU6050_RA_I2C_MST_CTRL             0x24
#define MPU6050_RA_I2C_SLV0_ADDR            0x25
#define MPU6050_RA_I2C_SLV0_REG             0x26
#define MPU6050_RA_I2C_SLV0_CTRL            0x27
#define MPU6050_RA_I2C_SLV1_ADDR            0x28
#define MPU6050_RA_I2C_SLV1_REG             0x29
#define MPU6050_RA_I2C_SLV1_CTRL            0x2A
#define MPU6050_RA_I2C_SLV2_ADDR            0x2B
#define MPU6050_RA_I2C_SLV2_REG             0x2C
#define MPU6050_RA_I2C_SLV2_CTRL            0x2D
#define MPU6050_RA_I2C_SLV3_ADDR            0x2E
#define MPU6050_RA_I2C_SLV3_REG             0x2F
#define MPU6050_RA_I2C_SLV3_CTRL            0x30
#define MPU6050_RA_I2C_SLV4_ADDR            0x31
#define MPU6050_RA_I2C_SLV4_REG             0x32
#define MPU6050_RA_I2C_SLV4_DO              0x33
#define MPU6050_RA_I2C_SLV4_CTRL            0x34
#define MPU6050_RA_I2C_SLV4_DI              0x35
#define MPU6050_RA_I2C_MST_STATUS           0x36
#define MPU6050_RA_INT_PIN_CFG              0x37
#define MPU6050_RA_INT_ENABLE               0x38
#define MPU6050_RA_DMP_INT_STATUS           0x39
#define MPU6050_RA_INT_STATUS               0x3A
#define MPU6050_RA_ACCEL_XOUT_H             0x3B
#define MPU6050_RA_ACCEL_XOUT_L             0x3C
#define MPU6050_RA_ACCEL_YOUT_H             0x3D
#define MPU6050_RA_ACCEL_YOUT_L             0x3E
#define MPU6050_RA_ACCEL_ZOUT_H             0x3F
#define MPU6050_RA_ACCEL_ZOUT_L             0x40
#define MPU6050_RA_TEMP_OUT_H               0x41
#define MPU6050_RA_TEMP_OUT_L               0x42
#define MPU6050_RA_GYRO_XOUT_H              0x43
#define MPU6050_RA_GYRO_XOUT_L              0x44
#define MPU6050_RA_GYRO_YOUT_H              0x45
#define MPU6050_RA_GYRO_YOUT_L              0x46
#define MPU6050_RA_GYRO_ZOUT_H              0x47
#define MPU6050_RA_GYRO_ZOUT_L              0x48
#define MPU6050_RA_EXT_SENS_DATA_00         0x49
#define MPU6050_RA_EXT_SENS_DATA_01         0x4A
#define MPU6050_RA_EXT_SENS_DATA_02         0x4B
#define MPU6050_RA_EXT_SENS_DATA_03         0x4C
#define MPU6050_RA_EXT_SENS_DATA_04         0x4D
#define MPU6050_RA_EXT_SENS_DATA_05         0x4E
#define MPU6050_RA_EXT_SENS_DATA_06         0x4F
#define MPU6050_RA_EXT_SENS_DATA_07         0x50
#define MPU6050_RA_EXT_SENS_DATA_08         0x51
#define MPU6050_RA_EXT_SENS_DATA_09         0x52
#define MPU6050_RA_EXT_SENS_DATA_10         0x53
#define MPU6050_RA_EXT_SENS_DATA_11         0x54
#define MPU6050_RA_EXT_SENS_DATA_12         0x55
#define MPU6050_RA_EXT_SENS_DATA_13         0x56
#define MPU6050_RA_EXT_SENS_DATA_14         0x57
#define MPU6050_RA_EXT_SENS_DATA_15         0x58
#define MPU6050_RA_EXT_SENS_DATA_16         0x59
#define MPU6050_RA_EXT_SENS_DATA_17         0x5A
#define MPU6050_RA_EXT_SENS_DATA_18         0x5B
#define MPU6050_RA_EXT_SENS_DATA_19         0x5C
#define MPU6050_RA_EXT_SENS_DATA_20         0x5D
#define MPU6050_RA_EXT_SENS_DATA_21         0x5E
#define MPU6050_RA_EXT_SENS_DATA_22         0x5F
#define MPU6050_RA_EXT_SENS_DATA_23         0x60
#define MPU6050_RA_MOT_DETECT_STATUS        0x61
#define MPU6050_RA_I2C_SLV0_DO              0x63
#define MPU6050_RA_I2C_SLV1_DO              0x64
#define MPU6050_RA_I2C_SLV2_DO              0x65
#define MPU6050_RA_I2C_SLV3_DO              0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL       0x67
#define MPU6050_RA_SIGNAL_PATH_RESET        0x68
#define MPU6050_RA_MOT_DETECT_CTRL          0x69
#define MPU6050_RA_USER_CTRL                0x6A
#define MPU6050_RA_PWR_MGMT_1               0x6B
#define MPU6050_RA_PWR_MGMT_2               0x6C
#define MPU6050_RA_BANK_SEL                 0x6D
#define MPU6050_RA_MEM_START_ADDR           0x6E
#define MPU6050_RA_MEM_R_W                  0x6F
#define MPU6050_RA_DMP_CFG_1                0x70
#define MPU6050_RA_DMP_CFG_2                0x71
#define MPU6050_RA_FIFO_COUNTH              0x72
#define MPU6050_RA_FIFO_COUNTL              0x73
#define MPU6050_RA_FIFO_R_W                 0x74
#define MPU6050_RA_WHO_AM_I                 0x75

#define MPU6050_CFG_EXT_SYNC_SET_BIT        3
#define MPU6050_CFG_DLPF_CFG_BIT            0

#define MPU6050_EXT_SYNC_DISABLED           (0 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_TEMP_OUT_L         (1 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_GYRO_XOUT_L        (2 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_GYRO_YOUT_L        (3 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L        (4 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L       (5 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L       (6 << MPU6050_CFG_EXT_SYNC_SET_BIT)
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L       (7 << MPU6050_CFG_EXT_SYNC_SET_BIT)

#define MPU6050_DLPF_BW_256                 (0 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_188                 (1 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_98                  (2 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_42                  (3 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_20                  (4 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_10                  (5 << MPU6050_CFG_DLPF_CFG_BIT)
#define MPU6050_DLPF_BW_5                   (6 << MPU6050_CFG_DLPF_CFG_BIT)

#define MPU6050_GYRO_FS_SET_BIT             3

#define MPU6050_GYRO_FS_250                 (0 << MPU6050_GYRO_FS_SET_BIT)
#define MPU6050_GYRO_FS_500                 (1 << MPU6050_GYRO_FS_SET_BIT)
#define MPU6050_GYRO_FS_1000                (2 << MPU6050_GYRO_FS_SET_BIT)
#define MPU6050_GYRO_FS_2000                (3 << MPU6050_GYRO_FS_SET_BIT)

#define MPU6050_ACC_XA_ST_BIT               7
#define MPU6050_ACC_YA_ST_BIT               6
#define MPU6050_ACC_ZA_ST_BIT               5
#define MPU6050_ACC_AFS_SEL_BIT             3

#define MPU6050_ACCEL_FS_2                  (0 << MPU6050_ACC_AFS_SEL_BIT)
#define MPU6050_ACCEL_FS_4                  (1 << MPU6050_ACC_AFS_SEL_BIT)
#define MPU6050_ACCEL_FS_8                  (2 << MPU6050_ACC_AFS_SEL_BIT)
#define MPU6050_ACCEL_FS_16                 (3 << MPU6050_ACC_AFS_SEL_BIT)

#define MPU6050_DHPF_RESET                  0
#define MPU6050_DHPF_5                      1
#define MPU6050_DHPF_2P5                    2
#define MPU6050_DHPF_1P25                   3
#define MPU6050_DHPF_0P63                   4
#define MPU6050_DHPF_HOLD                   7

#define MPU6050_FIFO_DISABLED               0
#define MPU6050_TEMP_FIFO_EN                (1 << 7)
#define MPU6050_XG_FIFO_EN                  (1 << 6)
#define MPU6050_YG_FIFO_EN                  (1 << 5)
#define MPU6050_ZG_FIFO_EN                  (1 << 4)
#define MPU6050_ACCEL_FIFO_EN               (1 << 3)
#define MPU6050_SLV2_FIFO_EN                (1 << 2)
#define MPU6050_SLV1_FIFO_EN                (1 << 1)
#define MPU6050_SLV0_FIFO_EN                (1 << 0)

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1

#define MPU6050_INTMODE_ACTIVEHIGH          (0 << MPU6050_INTCFG_INT_LEVEL_BIT)
#define MPU6050_INTMODE_ACTIVELOW           (1 << MPU6050_INTCFG_INT_LEVEL_BIT)

#define MPU6050_INTDRIVE_PUSHPULL           (0 << MPU6050_INTCFG_INT_OPEN_BIT)
#define MPU6050_INTDRIVE_OPENDRAIN          (1 << MPU6050_INTCFG_INT_OPEN_BIT)

#define MPU6050_INTLATCH_50USPULSE          (0 << MPU6050_INTCFG_LATCH_INT_EN_BIT)
#define MPU6050_INTLATCH_WAITCLEAR          (1 << MPU6050_INTCFG_LATCH_INT_EN_BIT)

#define MPU6050_INTCLEAR_STATUSREAD         (0 << MPU6050_INTCFG_INT_RD_CLEAR_BIT)
#define MPU6050_INTCLEAR_ANYREAD            (1 << MPU6050_INTCFG_INT_RD_CLEAR_BIT)

#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

#define MPU6050_INTDRDY_ENABLE              (1 << MPU6050_INTERRUPT_DATA_RDY_BIT)
#define MPU6050_INTFIFO_OFLOW_ENABLE        (1 << MPU6050_INTERRUPT_FIFO_OFLOW_BIT)
#define MPU6050_INTI2C_MST_ENABLE           (1 << MPU6050_INTERRUPT_I2C_MST_INT_BIT)

#define MPU6050_DEVICE_RESET                (1 << 7)
#define MPU6050_GYRO_RESET                  (1 << 0)
#define MPU6050_ACCEL_RESET                 (1 << 1)
#define MPU6050_TEMP_RESET                  (1 << 2)

#define MPU6050_CLK_INTERNAL_REFERENCE      0
#define MPU6050_CLK_X_REFERENCE             1
#define MPU6050_CLK_Y_REFERENCE             2
#define MPU6050_CLK_Z_REFERENCE             3
#define MPU6050_CLK_EXT_LSE_REFERENCE       4
#define MPU6050_CLK_EXT_HSE_REFERENCE       5
#define MPU6050_CLK_RESET                   7

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef struct
{
    int16_t raw_accel_data[3];  /* Accelerometer raw data holder        */
    int16_t raw_gyro_data[3];   /* Gyroscope raw data holder            */
    int16_t raw_temperature;    /* Temperature raw data holder          */
    float accel_data[3];        /* Accelerometer calibrated data holder */
    float gyro_data[3];         /* Gyroscope calibrated data holder     */
    float temperature;          /* Temperature deg C data holder        */
    int64_t sample_time_ns;     /* Time of the sample in nanoseconds    */
    mutex_t read_lock;          /* Keep listeners from reading if
                                   new data is being written            */
} MPU6050_Data;

typedef struct
{
    uint8_t dlp_cfg;
    uint8_t ext_sync_cfg;
    uint8_t gyro_range_sel;
    uint8_t accel_range_sel;
    uint8_t fifo_cfg;
    uint8_t int_pin_cfg;
    uint8_t int_cfg;
    uint8_t address_7bit;
    uint8_t clock_reference;
    uint8_t sample_rate_divider;
    MPU6050_Data *data_holder;
    I2CDriver *i2cp;
} MPU6050_Configuration;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
msg_t MPU6050Init(const MPU6050_Configuration *cfg);
msg_t MPU6050DeviceReset(const MPU6050_Configuration *cfg);
msg_t MPU6050GetID(const MPU6050_Configuration *cfg, uint8_t id[1]);
msg_t MPU6050ReadData(const MPU6050_Configuration *cfg, uint8_t data[14]);
float MPU6050GetAccelGain(const MPU6050_Configuration *cfg);
float MPU6050GetGyroGain(const MPU6050_Configuration *cfg);
float MPU6050GetSampleTime(const MPU6050_Configuration *cfg);

#endif
