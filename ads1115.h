// ads1115.h
#ifndef ADS1115_H
#define ADS1115_H

// Direcci�n 7 bits base (A0,A1,A2 = GND => 0x48)
#ifndef ADS1115_I2C_ADDR_7BIT
 #define ADS1115_I2C_ADDR_7BIT   0x49   
#endif

// Direcciones 8 bits para CCS (write/read)
#define ADS1115_ADDR_WRITE   ((ADS1115_I2C_ADDR_7BIT << 1) | 0) // 0x90
#define ADS1115_ADDR_READ    ((ADS1115_I2C_ADDR_7BIT << 1) | 1) // 0x91

// Registros del ADS1115
#define ADS1115_REG_CONVERSION   0x00
#define ADS1115_REG_CONFIG       0x01
#define ADS1115_REG_LO_THRESH    0x02
#define ADS1115_REG_HI_THRESH    0x03

// Bits de CONFIG (solo lo que vamos a usar al inicio)
// MUX: canal single-ended
#define ADS1115_MUX_A0_GND   0x4000  // AIN0 vs GND
#define ADS1115_MUX_A1_GND   0x5000  // AIN1 vs GND
#define ADS1115_MUX_A2_GND   0x6000  // AIN2 vs GND
#define ADS1115_MUX_A3_GND   0x7000  // AIN3 vs GND

// PGA +-4.096 V (ajustable después)
#define ADS1115_PGA_4_096V   0x0200

// Modo single-shot (cada lectura dispara conversión)
#define ADS1115_MODE_SINGLE  0x0100

// Data rate 128 SPS (equilibrio ruido/velocidad)
#define ADS1115_DR_128SPS    0x0080

// Deshabilitar comparador
#define ADS1115_COMP_DISABLE 0x0003

// Bit OS (Start conversion / flag ready)
#define ADS1115_OS_SINGLE_START  0x8000
#define ADS1115_OS_MASK          0x8000

// Prototipos
int1 ads1115_ping(void);
void  ads1115_write_reg(int8 reg, int16 value);
int16 ads1115_read_reg(int8 reg);

void  ads1115_init(void);
int16 ads1115_read_single_ended(int8 channel);
float ads1115_raw_to_volts(int16 raw);
float ads1115_read_voltage(int8 channel);


#endif
