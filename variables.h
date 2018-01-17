#define SOF 0xCC
#define EOF 0xDD
#define INIT 0xA1
#define CHECK 0xA3

//driver control vars

uint8_t silnik = 1;
uint8_t adress = 0;
uint8_t init = 0;
int8_t znak = 0;
uint32_t is_delay = 1;
uint32_t debug = 0; //special var for debugging
// UART and comms vars
uint8_t rxData[10] = {0};
uint8_t txData[10] = {0};
uint8_t rxByte = 0;
uint8_t iterator_wiadomosci =0;

// Virtual complience vars
uint16_t poz_zad = 6000;
uint16_t ks =400;
uint16_t kd =600;

//FOC vars
uint8_t liczba_par_biegunow = 21;
//iq
int16_t Iq_zadane;
int32_t Iq_poprzednie;
int32_t Vq_zadane = 0;
int32_t uchyb_Iq = 0;
int32_t uchyb_poprzedni_Iq = 0;

//id
int32_t Id_zadane;
int32_t Id_poprzednie;
int32_t Vd_zadane=0;
int32_t uchyb_Id;

int32_t I_alfa = 0;
int32_t I_beta = 0;
int32_t V_alfa = 0;
int32_t V_beta = 0;
uint32_t V_ref = 0;
uint32_t angle32 = 0;
int16_t I_net = 0;

//Encoder and SPI vars
uint8_t tData[2];
uint8_t rData[6];
uint16_t kat_enkoder = 0;	//|0-2PI| = |0 - 2^16|
uint16_t kat_enkoder_poprzedni = {0};
int16_t predkosc_enkoder;
int16_t predkosc_zadana = 300;
int16_t kat_elektryczny = 0;
int32_t kat_absolutny = 0;
int32_t offset_elektryczny = 0;
int16_t angle = 0;

// ADC and current measurement vars
int32_t v3v = 3324;
int32_t iu = 0;
int32_t iuOffset = 0;
int32_t iv = 0;
int32_t ivOffset = 0;
int32_t iw = 0;
int32_t iwOffset = 0;
int32_t i[3]= {0};
int32_t icale = 0;

//PI regulators
typedef struct PI
{
    uint16_t kP;
    uint16_t kI;
    int32_t y_max;
    int32_t y_min;
    int32_t y;
    int32_t buff ;
    int32_t buff_max;
    int32_t buff_min;
}PI_t;

PI_t PI_Id;
PI_t PI_Iq;
PI_t PI_predkosc;

void PI_REG(PI_t *handle, int32_t err)
{
    handle->buff = handle->buff + err;
    if(handle->buff > handle->buff_max)
        handle->buff = handle->buff_max;
    else if(handle->buff < handle->buff_min)
        handle->buff = handle->buff_min;
    int32_t temp_ki_buff = (handle->buff * handle->kI) / 10000;
    handle->y = (handle->kP * err / 10000) + temp_ki_buff;
    if(handle->y >= handle->y_max)
        handle->y = handle->y_max;
    else if (handle->y <= handle->y_min)
        handle->y = handle->y_min;
}

void RS_Init(void)		//Communications initialization function
{
	txData[0] = 0xCC; //SOF
	txData[1] = 0xEE;
	txData[2] = adress;
	txData[9] = EOF;
}

//End of file
