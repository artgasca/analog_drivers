// ina281.h
#ifndef INA281_H
#define INA281_H

#include "ads1115.h"   // Usamos sus helpers

// Estructura por canal INA281 + ADS1115
typedef struct {
   int8  ads_channel;    // Canal del ADS1115: 0..3
   float gain;           // Ganancia del INA281 (20, 50, 100, 200, 500)
   float r_shunt_ohms;   // Resistencia de shunt en ohms
   float offset_volts;   // Offset de Vout a 0A (o a 4mA para loops, si quieres)
} ina281_ch_t;

// Inicializa estructura
void ina281_init_ch(ina281_ch_t *ch,
                    int8 ads_channel,
                    float gain,
                    float r_shunt_ohms,
                    float offset_volts);

// Lee el voltaje a la salida del INA281 (entrada del ADS), compensado por offset
float ina281_read_vout(ina281_ch_t *ch);

// Lee corriente en amperes
float ina281_read_current_A(ina281_ch_t *ch);

// Lee corriente en mA
float ina281_read_current_mA(ina281_ch_t *ch);

#endif
