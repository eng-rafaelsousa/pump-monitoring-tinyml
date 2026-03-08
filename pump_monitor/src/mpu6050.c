#include "inc/mpu6050.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define MPU6050_ADDR       0x68
#define PWR_MGMT_1         0x6B
#define USER_CTRL          0x6A
#define FIFO_EN            0x23
#define INT_ENABLE         0x38
#define INT_STATUS         0x3A
#define FIFO_COUNTH        0x72
#define FIFO_R_W           0x74
#define ACCEL_XOUT_H       0x3B

bool mpu6050_init(void) {
    uint8_t config[][2] = {
        {PWR_MGMT_1, 0x00},        // Wake up
        {USER_CTRL,  0x40},        // Enable FIFO
        {FIFO_EN,    0x08},        // Enable accel data to FIFO
        {INT_ENABLE, 0x01}         // Enable data ready interrupt
    };

    for (int i = 0; i < 4; i++) {
        if (i2c_write_blocking(i2c0, MPU6050_ADDR, config[i], 2, false) != 2) {
            return false;
        }
        sleep_ms(10);
    }
    return true;
}

void mpu6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az) {
    uint8_t reg = ACCEL_XOUT_H;
    uint8_t data[6];

    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, data, 6, false);

    *ax = (data[0] << 8) | data[1];
    *ay = (data[2] << 8) | data[3];
    *az = (data[4] << 8) | data[5];
}

uint8_t mpu6050_get_fifo_count(void) {
    uint8_t reg = FIFO_COUNTH;
    uint8_t data[2];

    i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c0, MPU6050_ADDR, data, 2, false);

    uint16_t count = (data[0] << 8) | data[1];
    return count / 6; // cada amostra accel = 6 bytes
}

// Lê 'samples' amostras do FIFO para arrays (ou para processar direto)
bool mpu6050_read_fifo(int16_t *ax, int16_t *ay, int16_t *az, uint8_t samples) {
    if (samples == 0 || samples > MPU6050_FIFO_SIZE / 6)
        return false;

    for (uint8_t i = 0; i < samples; i++) {
        uint8_t fifo_data[6];
        uint8_t reg = FIFO_R_W;

        i2c_write_blocking(i2c0, MPU6050_ADDR, &reg, 1, true);
        i2c_read_blocking(i2c0, MPU6050_ADDR, fifo_data, 6, false);

        ax[i] = (fifo_data[0] << 8) | fifo_data[1];
        ay[i] = (fifo_data[2] << 8) | fifo_data[3];
        az[i] = (fifo_data[4] << 8) | fifo_data[5];
    }

    return true;
}
