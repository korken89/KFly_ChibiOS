#ifndef __SENSOR_READ_H
#define __SENSOR_READ_H

#include "mpu6050.h"
#include "hmc5983.h"
#include "sensor_calibration.h"

/*===========================================================================*/
/* Module global definitions.                                                */
/*===========================================================================*/
#define SENSOR_ACCGYRO_HZ                           200.0f
#define SENSOR_ACCGYRO_DT                           (1.0f / SENSOR_ACCGYRO_HZ)
#define ACCGYRO_BIQUAD_CUT_HZ                       60.0f
#define ACCGYRO_BUTTERWORTH_Q                       0.707106781f // Butterworth
#define ACCGYRO_DATA_AVAILABLE_EVENTMASK            EVENT_MASK(0)
#define MAG_DATA_AVAILABLE_EVENTMASK                EVENT_MASK(1)
#define BARO_DATA_AVAILABLE_EVENTMASK               EVENT_MASK(2)
#define SENSOR_IMU_DATA_SIZE                        (sizeof(imu_data_t))
#define SENSOR_IMU_RAW_DATA_SIZE                    (sizeof(imu_raw_data_t))
#define SENSOR_IMU_CALIBRATION_SIZE                 (sizeof(imu_calibration_t))

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
/**
 * @brief   Sensor read configuration structure.
 */
typedef struct
{
    /**
     * @brief   Pointer to the MPU6050 configuration.
     */
    const MPU6050_Configuration *mpu6050cfg;
    /**
     * @brief   Pointer to the HMC5983 configuration.
     */
    const HMC5983_Configuration *hmc5983cfg;
    /**
     * @brief   Pointer to MPU6050 calibration.
     */
    sensor_calibration_t *mpu6050cal;
    /**
     * @brief   Pointer to HMC5983 calibration.
     */
    sensor_calibration_t *hmc5983cal;
    /**
     * @brief   Pointer to calibration time stamp.
     */
    uint32_t *calibration_timestamp;
    /**
     * @brief   Pointer to the new data event source.
     */
    event_source_t *new_data_es;
} sensor_read_configuration_t;

/**
 * @brief   Calibrated IMU data structure.
 */
typedef struct
{
    /**
     * @brief   Calibrated accelerometer data.
     */
    float accelerometer[3];
    /**
     * @brief   Calibrated gyroscope data.
     */
    float gyroscope[3];
    /**
     * @brief   Calibrated magnetometer data.
     */
    float magnetometer[3];
    /**
     * @brief   Temperature of the accelerometer and gyroscope.
     */
    float temperature;
    /**
     * @brief   Sample time of the latest acc & gyro sample in nanoseconds.
     */
    int64_t acc_gyro_time_ns;
} imu_data_t;

/**
 * @brief   Raw IMU data structure.
 */
typedef struct
{
    /**
     * @brief   Raw accelerometer data.
     */
    int16_t accelerometer[3];
    /**
     * @brief   Raw gyroscope data.
     */
    int16_t gyroscope[3];
    /**
     * @brief   Raw magnetometer data.
     */
    int16_t magnetometer[3];
    /**
     * @brief   Raw temperature of the accelerometer and gyroscope.
     */
    int16_t temperature;
    /**
     * @brief   Raw pressure of the barometer.
     */
    uint32_t pressure;
    /**
     * @brief   Sample time of the latest acc & gyro sample in nanoseconds.
     */
    int64_t acc_gyro_time_ns;
} imu_raw_data_t;

/**
 * @brief   IMU calibration data structure.
 */
typedef struct
{
    /**
     * @brief   Accelerometer bias.
     */
    float accelerometer_bias[3];
    /**
     * @brief   Accelerometer gain.
     */
    float accelerometer_gain[3];
    /**
     * @brief   Magnetometer bias.
     */
    float magnetometer_bias[3];
    /**
     * @brief   Magnetometer gain.
     */
    float magnetometer_gain[3];
    /**
     * @brief   Calibration time stamp.
     */
    uint32_t timestamp;
} imu_calibration_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
msg_t SensorReadInit(void);
void MPU6050cb(EXTDriver *extp, expchannel_t channel);
void HMC5983cb(EXTDriver *extp, expchannel_t channel);
event_source_t *ptrGetNewDataEventSource(void);
int64_t rtGetLatestAccelerometerSamplingTimeNS(void);
int16_t *ptrGetRawAccelerometerData(void);
float *ptrGetAccelerometerData(void);
int16_t *ptrGetRawGyroscopeData(void);
float *ptrGetGyroscopeData(void);
int16_t GetRawGyroscopeTemperature(void);
float GetGyroscopeTemperature(void);
int16_t *ptrGetRawMagnetometerData(void);
float *ptrGetMagnetometerData(void);
void GetIMUData(imu_data_t *data);
void GetRawIMUData(imu_raw_data_t *data);
void GetIMUCalibration(imu_calibration_t *cal);
void SetIMUCalibration(imu_calibration_t *cal);
void LockSensorStructures(void);
void UnlockSensorStructures(void);
void LockSensorCalibration(void);
void UnlockSensorCalibration(void);
sensor_calibration_t *ptrGetAccelerometerCalibration(void);
sensor_calibration_t *ptrGetMagnetometerCalibration(void);

#endif
