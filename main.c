
#include <math.h>
#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include "variables.h"

#define ONESEC 100U

uint8_t tDataZero[4];

void SYS1(void);
void SYS2(void);
void LiczeniePradu(void);
int32_t sqrtI2I(int32_t);
void ADC_Pomiary(void);
void ADC_START(void);
void DRV_START(void);
void XMC_Init(void);
void enkoder(void);
void interpreter_wiadomosci(void);
void delay(uint32_t);
float t = 0.0f;
int main(void)
{
	tData[0] = 0x3F;
	tData[1] = 0xFF;

	PI_Id.kP = 1;
	PI_Id.kI = 1;		//ki = .ki / 1000
	PI_Id.y_max = 20584;
	PI_Id.y_min = -20584;
	PI_Id.buff_max = 3000;
	PI_Id.buff_min = -30000;

	PI_Iq.kP =	1;
	PI_Iq.kI = 1;		//ki = .ki / 1000
	PI_Iq.y_max = 20384;
	PI_Iq.y_min = -20384;
	PI_Iq.buff_max = 30000;
	PI_Iq.buff_min = -30000;

  DAVE_STATUS_t status;
  status = DAVE_Init();           /* Initialization of DAVE APPs  */
  status = (UART_STATUS_t)UART_Init(&UART_0);
  uint32_t SYS1_Id = SYSTIMER_CreateTimer(120U, SYSTIMER_MODE_PERIODIC, (void*)SYS1, NULL);
  uint32_t SYS2_V = SYSTIMER_CreateTimer(1000U, SYSTIMER_MODE_PERIODIC, (void*)SYS2, NULL);

  XMC_Init();

  //DRV setup
  SPI_MASTER_Init(&SPI_enkoder);
  DAC_Enable(&ANALOG);
  DAC_Enable(&ANALOG2);

	PWM_SVM_Start(&SVPWM);

	PWM_SVM_SVMUpdate(&SVPWM, 0,0);

	DRV_START();

	XMC_SPI_CH_ConfigureShiftClockOutput(SPI_enkoder.channel, XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_0_DELAY_DISABLED, XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
  	SPI_MASTER_DisableSlaveSelectSignal(&SPI_enkoder);
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_enkoder, SPI_MASTER_SS_SIGNAL_0);

	enkoder();															///zeby poprawnie liczyc kat absolutny kat_enkoder nie moze byc = 0 na poczatku!
	kat_enkoder_poprzedni = kat_enkoder;
	SYSTIMER_StartTimer(SYS1_Id);
	SYSTIMER_StartTimer(SYS2_V);
  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {

  }
}
int32_t uchyb;
void SYS2(void)
{
	uint16_t kat_enkoder_obecny = kat_enkoder;
	int32_t predkosc_nowa = 0;
	if( kat_enkoder_obecny > kat_enkoder_poprzedni)
		predkosc_nowa = kat_enkoder_obecny - kat_enkoder_poprzedni;
	else
		predkosc_nowa = UINT16_MAX - kat_enkoder_poprzedni + kat_enkoder_obecny;

	predkosc_enkoder = predkosc_nowa;

	if(predkosc_enkoder < 0)
		predkosc_enkoder=-predkosc_enkoder;


	if(kat_enkoder_obecny == 0 || kat_enkoder_poprzedni == 0)
	{
		kat_enkoder_poprzedni = kat_enkoder_obecny;
		return;
	}

	int32_t dFi =0;
	if(kat_enkoder_obecny > kat_enkoder_poprzedni)
	{
		dFi = kat_enkoder_obecny-kat_enkoder_poprzedni;
		if(dFi > 10400)
			{
			dFi = UINT16_MAX - kat_enkoder_obecny + kat_enkoder_poprzedni;
			kat_absolutny += znak * dFi;
			}
		else
		{
			kat_absolutny -= znak * dFi;
		}
	}
	else
	{
		dFi =kat_enkoder_poprzedni - kat_enkoder_obecny;
		if(dFi > 10400)
		{
			dFi = UINT16_MAX - kat_enkoder_poprzedni + kat_enkoder_obecny;
		kat_absolutny -=znak * dFi;
	}
		else
		{
			kat_absolutny += znak * dFi;
		}
	}

	kat_enkoder_poprzedni = kat_enkoder_obecny;

	/*		REGULATOR PREDKOSCI
	int32_t uchyb = predkosc_zadana - predkosc_enkoder;
	PI_REG(&PI_predkosc, uchyb);
	Iq_zadane = PI_predkosc.y;
	 */
	 uchyb = poz_zad - kat_absolutny;

	//ZADAWANIE MOMENTU
	int32_t sila = (ks * uchyb)/1000 - ((kd * predkosc_enkoder)/1000);
	Iq_zadane = sila;



}
void enkoder(void)
{
	SPI_MASTER_Transfer(&SPI_enkoder, tData, rData,  2);
	while(SPI_MASTER_IsRxBusy(&SPI_enkoder));
	uint16_t result = rData[0]<<8 | rData[1];
	result &= 0x3FFF;			//Usuwanie bitu parzystkosci i bitu R/W

	kat_enkoder =result * 4;		//0 - 65535 = 0 - 2PI

}
void SYS1(void)
{
	DIGITAL_IO_SetOutputHigh(&SIGNAL);
	enkoder();
	int32_t kat_elektryczny_temp = 0;
	kat_elektryczny_temp = ((uint32_t)(kat_enkoder * liczba_par_biegunow));
	kat_elektryczny_temp = kat_elektryczny_temp % UINT16_MAX ;
	kat_elektryczny_temp = kat_elektryczny_temp - INT16_MAX;
	kat_elektryczny = kat_elektryczny_temp + offset_elektryczny;

	LiczeniePradu();

	I_beta = MOTOR_LIB_ClarkTransform(iu,iv,&I_alfa);

	Id_poprzednie = MOTOR_LIB_ParkTransform(I_alfa, I_beta, kat_elektryczny, &Iq_poprzednie);

	Id_poprzednie = -Id_poprzednie;
	Iq_poprzednie = -Iq_poprzednie;
/*
	if(kat_absolutny < - 4000)			//Zabezpieczenie zeby silniki nie walnely w siebie srubami
		Iq_zadane = 20000;
	if(kat_absolutny > 32000)
		Iq_zadane = - 20000;
*/
	uchyb_Id = Id_zadane - Id_poprzednie;
	uchyb_Iq = Iq_zadane - Iq_poprzednie;

	PI_REG(&PI_Id,uchyb_Id);
	PI_REG(&PI_Iq,uchyb_Iq);
	Vd_zadane = PI_Id.y;
	Vq_zadane = PI_Iq.y;

  	V_alfa = MOTOR_LIB_IParkTransform(Vd_zadane, Vq_zadane, kat_elektryczny, &V_beta);

  	 V_ref = MOTOR_LIB_Car2Pol(V_alfa, V_beta, &angle);

  	angle32 =((int32_t)angle+INT16_MAX) *256;

  	PWM_SVM_SVMUpdate(&SVPWM, V_ref, angle32);


  	int32_t Ix = ((iw - iv) * 56775) / UINT16_MAX;
  	int32_t Iy = iu - ((iv+iw)/2);
  	Iy *= Iy;
  	Iy += Ix*Ix;
  	I_net = sqrtf(Iy);

	txData[3] = I_net >> 8;
	txData[4] = I_net;
	txData[5] = kat_absolutny >> 24;
	txData[6] = kat_absolutny >> 16;
	txData[7] = kat_absolutny >> 8;
	txData[8] = kat_absolutny;

	 //DAC_SingleValue_SetValue_u16(&ANALOG, Iq_poprzednie);
	 //DAC_SingleValue_SetValue_u16(&ANALOG2, Iq_poprzednie);

	DIGITAL_IO_SetOutputLow(&SIGNAL);
}
void DRV_START(void)
{
	PWM_SVM_InverterEnable(&SVPWM);

	for(uint16_t j=0; j < 65000; j++);

	//kalibracja offsetów
	SPI_MASTER_DisableSlaveSelectSignal(&SPI_enkoder);
	SPI_MASTER_EnableSlaveSelectSignal(&SPI_enkoder, SPI_MASTER_SS_SIGNAL_1);

	uint8_t zero [2]= {0};
	uint8_t rec[10] = {0};
	uint8_t sendData[2];
	sendData[0] = 0b00011000;
	sendData[1] = 0b00000100;	// <ustawienie wzmocnienia na 20
	for(int i = 0; i < 5; i++)
	{
		SPI_MASTER_Transmit(&SPI_enkoder, sendData, 2);		//Opamp Gain
	}
	sendData[0] = 0b10011000;
	sendData[1] = 0b00000000;
	for(int i = 0; i < 5; i++)
	{
		SPI_MASTER_Transmit(&SPI_enkoder, sendData, 2);
		SPI_MASTER_Transfer(&SPI_enkoder, zero, rec, 2);
	}
	DIGITAL_IO_SetOutputHigh(&DC_CAL);
	uint16_t iloscProbek = 50;
	for( uint16_t i = 0; i < iloscProbek; i++)
	{
		iuOffset += ADC_MEASUREMENT_ADV_GetResult(&ADC_U_Channel_A_handle);
		ivOffset += ADC_MEASUREMENT_ADV_GetResult(&ADC_V_Channel_A_handle);
		for(uint16_t j = 0; j < 1000; j++);	//delay
	}
	iuOffset /= iloscProbek;
	ivOffset /= iloscProbek;
	iwOffset /= iloscProbek;

	DIGITAL_IO_SetOutputLow(&DC_CAL);
}
void LiczeniePradu(void)
{
	/*
	i[0] = ADC_MEASUREMENT_ADV_GetResult(&ADC_U_Channel_A_handle);
	i[1] = ADC_MEASUREMENT_ADV_GetResult(&ADC_V_Channel_A_handle);

	iv = i[1] - ivOffset;
	iv = (iv * v3v) / 4095; // w tym miejscu sa miliwolty
	iv *= 5;

	iu = i[0] - iuOffset;
	iu = (iu * v3v) / 4095;
	iu *= 5;
	*/
	i[0] = ADC_MEASUREMENT_ADV_GetResult(&ADC_U_Channel_A_handle);
	i[1] = ADC_MEASUREMENT_ADV_GetResult(&ADC_V_Channel_A_handle);

	i[1] = i[1] - ivOffset;
	i[1] = (i[1] * v3v) / 4095; // w tym miejscu sa miliwolty
	i[1] *= 5;

	i[0] = i[0] - iuOffset;
	i[0] = (i[0] * v3v) / 4095;
	i[0] *= 5;

	iu = (iu * 14) + (i[0]* 2);
	iu = iu >> 4;
	iv = (iv * 14) + (i[1]* 2);
	iv = iv >> 4;

	iw = - iu - iv;
}
void ADC_START(void)
{

}
void ADC_Pomiary(void)
{
}
void XMC_Init()
{
	switch (silnik) {
		case 0:
			adress = 0x10;
			offset_elektryczny = -11200;

			znak = -1;
			break;
		case 1:
			adress = 0x11;
			offset_elektryczny = -25500;

			znak = 1;
			break;
		default:
			break;
	}
	PI_Iq.kP = 4500;
	PI_Iq.kI =412;
	PI_Id.kP =2107;
	PI_Id.kI =1130;
	RS_Init();
	while(init == 0)
	{
		UART_Receive(&UART_0, &rxByte,1);

		DIGITAL_IO_ToggleOutput(&LED);
		delay(50000);
	}
	DIGITAL_IO_SetOutputLow(&LED);
}
void end_transmit()
{
}
void end_receive()
{
	if(rxByte == SOF)
	{
		rxData[0] = rxByte;
		iterator_wiadomosci = 1;
	}
	else if (rxByte == EOF)
	{
		rxData[iterator_wiadomosci +1] = EOF;
		iterator_wiadomosci++;
		interpreter_wiadomosci();

	}
	else if ( iterator_wiadomosci < 9)
	{
		rxData[iterator_wiadomosci] =rxByte;
		iterator_wiadomosci++;
	}
	UART_Receive(&UART_0, &rxByte,1);
}
void interpreter_wiadomosci()
{
	if(rxData[0] == SOF && rxData[1] == adress)
	{
		if(init == 0 && rxData[2] == INIT)			//inicjalizacja
		{
			init = 1;
			txData[3] =INIT;
			txData[4] =init;
			txData[5] = EOF;
			UART_Transmit(&UART_0, txData, 6);
		}
		else if(rxData[2] == CHECK)				//status check
		{
			txData[3] =CHECK;
			txData[4] =init;
			txData[5] = silnik;
			txData[6] = EOF;
			UART_Transmit(&UART_0, txData, 7);
		}
		else															//zadawanie momentu
		{
			poz_zad = rxData[2] << 8 | rxData[3];
			UART_Transmit(&UART_0, txData, 10);
		}
	}
}
void delay(uint32_t us)
{
	is_delay = 1;
	TIMER_SetTimeInterval(&DELAY, us);
	TIMER_Start(&DELAY);
	while(is_delay);
	TIMER_Stop(&DELAY);
}
void TIMER_IRQ()
{
	is_delay = 0;
}

