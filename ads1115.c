// ads1115.c
#include "ads1115.h"

// Asumimos que en tu main ya existe:
// #use i2c(Master, SDA=..., SCL=..., Fast=400000, stream=ADS_I2C)

int1 ads1115_ping(void)
{
   int8 ack;

   i2c_start(ADS_I2C);
   ack = i2c_write(ADS1115_ADDR_WRITE);   // 0x90 con ADDR=GND
   i2c_stop(ADS_I2C);

   // En CCS: 0 = ACK, 1 = NACK
   return (ack == 0);
}


void ads1115_write_reg(int8 reg, int16 value)
{
   int8 msb = make8(value, 1);
   int8 lsb = make8(value, 0);

   i2c_start(ADS_I2C);
   i2c_write(ADS_I2C, ADS1115_ADDR_WRITE);
   i2c_write(ADS_I2C, reg);
   i2c_write(ADS_I2C, msb);
   i2c_write(ADS_I2C, lsb);
   i2c_stop(ADS_I2C);
}

int16 ads1115_read_reg(int8 reg)
{
   int8 msb, lsb;
   int16 value;

   // Escribimos el puntero de registro
   i2c_start(ADS_I2C);
   i2c_write(ADS_I2C, ADS1115_ADDR_WRITE);
   i2c_write(ADS_I2C, reg);

   // Repeated start para leer
   i2c_start(ADS_I2C);
   i2c_write(ADS_I2C, ADS1115_ADDR_READ);
   msb = i2c_read(ADS_I2C, 1); // ACK
   lsb = i2c_read(ADS_I2C, 0); // NACK
   i2c_stop(ADS_I2C);

   value = make16(msb, lsb);
   return value;
}

void ads1115_init(void)
{
   // Por ahora no necesitamos gran cosa, pero es buen hook
   // para más adelante (thresholds, modo por defecto, etc.)
   // Aquí podríamos leer CONFIG para validar I2C:
   int16 cfg = ads1115_read_reg(ADS1115_REG_CONFIG);
   // Solo para debug, en tu main lo imprimes si quieres.
}

// Lee un canal single-ended [0..3] en modo single-shot
// Retorna el RAW signed 16-bit (int16 CCS)
int16 ads1115_read_single_ended(int8 channel)
{
   int16 config = 0;

   switch(channel)
   {
      case 0: config |= ADS1115_MUX_A0_GND; break;
      case 1: config |= ADS1115_MUX_A1_GND; break;
      case 2: config |= ADS1115_MUX_A2_GND; break;
      case 3: config |= ADS1115_MUX_A3_GND; break;
      default: return 0; // canal inválido
   }

   config |= ADS1115_OS_SINGLE_START;   // Disparar conversión
   config |= ADS1115_PGA_4_096V;        // Rango +-4.096V
   config |= ADS1115_MODE_SINGLE;       // Single-shot
   config |= ADS1115_DR_128SPS;         // 128SPS
   config |= ADS1115_COMP_DISABLE;      // Sin comparador

   // Escribimos configuración
   ads1115_write_reg(ADS1115_REG_CONFIG, config);

   // Esperar a que termine la conversión (OS vuelve a 1 cuando está listo)
   while(TRUE)
   {
      int16 cfg = ads1115_read_reg(ADS1115_REG_CONFIG);
      if(cfg & ADS1115_OS_MASK)  // bit listo
         break;
      // Opcional: pequeño delay si quieres aflojar el bus
      // delay_ms(1);
   }

   // Leer resultado
   return ads1115_read_reg(ADS1115_REG_CONVERSION);
}
// Conversi�n gen�rica usando el FSR del PGA que estamos usando
// Aqu� asumimos PGA = �4.096V, si cambias, ajustas FSR_VOLTS
#define ADS1115_FSR_VOLTS   4.096f   // Full-Scale Range del PGA actual

float ads1115_raw_to_volts(int16 raw)
{
   // raw es signed 16-bit, rango -32768..32767
   float v = (float)raw;
   v *= (ADS1115_FSR_VOLTS / 32768.0f);
   return v; // Volts
}

float ads1115_read_voltage(int8 channel)
{
   int16 raw = ads1115_read_single_ended(channel);
   return ads1115_raw_to_volts(raw);
}
