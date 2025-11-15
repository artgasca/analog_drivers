// ina281.c
#include "ina281.h"
#include "ads1115.h"

void ina281_init_ch(ina281_ch_t *ch,
                    int8 ads_channel,
                    float gain,
                    float r_shunt_ohms,
                    float offset_volts)
{
   ch->ads_channel   = ads_channel;
   ch->gain          = gain;
   ch->r_shunt_ohms  = r_shunt_ohms;
   ch->offset_volts  = offset_volts;
}

// Vout = lo que ve el ADS1115 (salida INA281), en volts
float ina281_read_vout(ina281_ch_t *ch)
{
   int16 raw = ads1115_read_single_ended(ch->ads_channel);
   float v   = ads1115_raw_to_volts(raw);   // del driver del ADS

   v -= ch->offset_volts;   // compensar offset (puede ser 0.0 al inicio)

   return v;
}

float ina281_read_current_A(ina281_ch_t *ch)
{
   float vout = ina281_read_vout(ch);

   // I = Vout / (GAIN * Rshunt)
   float i_amps = vout / (ch->gain * ch->r_shunt_ohms);

   return i_amps;
}

float ina281_read_current_mA(ina281_ch_t *ch)
{
   float i_A = ina281_read_current_A(ch);
   return (i_A * 1000.0f);
}
