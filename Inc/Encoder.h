#ifndef ENCODER_H
#define ENCODER_H

void Encoder_Init(void);
short Encoder(void);
extern short encoder, tecla;
void Tecla_Encoder(void);
void Tecla_Reset(void);

#endif