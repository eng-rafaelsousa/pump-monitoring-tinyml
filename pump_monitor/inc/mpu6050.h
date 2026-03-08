#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stdbool.h>

#define MPU6050_FIFO_SIZE 1024  // Máximo de bytes no buffer FIFO

bool mpu6050_init(void);
void mpu6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az);
uint8_t mpu6050_get_fifo_count(void);
bool mpu6050_read_fifo(int16_t *ax, int16_t *ay, int16_t *az, uint8_t samples);

#endif
