/**
 * @file motor_lib.c
 * @date 2015-10-08
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * MOTOR_LIB v4.0.8 - Provides common motor control library API's.
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-20:
 *     - Initial version
 *
 * 2015-06-20:
 *     - Vector magnitude calculation API's are added
 *
 * @endcond
 *
 */

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include "motor_lib.h"
#if defined (MATH)
#include <xmc_scu.h>
#endif

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/

/**
 * @ingroup MOTOTLIBS_constants
 * @{
 */
#if !defined (MATH)
const int32_t MOTOR_LIB_E[23] =
{0x20000000, 0x12E4051D, 0x9FB385B, 0x51111D4, 0x28B0D43, 0x145D7E1, 0xA2F61E,
 0x517C55, 0x28BE53, 0x145F2E, 0xA2F98, 0x517CC, 0x28BE6, 0x145F3, 0xA2F9, 0x517C,
 0x28BE, 0x145F, 0xA2F, 0x517, 0x28B, 0x145, 0xA2};
#endif

/**********************************************************************************************************************
* API IMPLEMENTATION
**********************************************************************************************************************/
/* App API to retrieve the App version info */
DAVE_APP_VERSION_t MOTOR_LIB_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = MOTOR_LIB_MAJOR_VERSION;
  version.minor = MOTOR_LIB_MINOR_VERSION;
  version.patch = MOTOR_LIB_PATCH_VERSION;

  return version;
}

#if defined (MATH)
/**
 * This function will enable HW Cordic for XMC13 device.
 */
void MOTOR_LIB_CordicEnable(void)
{
  /*Ungate Math coprocessor*/
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_MATH);
}

/**
 * This function is the implementation of cartesian to Polar Transformation.
 */
uint32_t MOTOR_LIB_Car2Pol(int32_t x,int32_t y, int16_t *angle)
{
  int32_t temp_angle;

  /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_CIRCULAR_MODE +
              (uint32_t) MOTOR_LIB_VECTORING_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_UNSIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

  /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ = (((uint32_t)0 << MATH_CORDZ_DATA_Pos) & ((uint32_t)MATH_CORDZ_DATA_Msk)) |
              (MATH->CORDZ & ((uint32_t)~((uint32_t)MATH_CORDZ_DATA_Msk)));

  MATH->CORDY = (((uint32_t)y << MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
              (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)x << MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while(( MATH->STATC & 0x1U )== 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }
  /* Update the Amplitude and Angle Values*/
  temp_angle = (int32_t)MATH->CORRZ;
  *angle  = (int16_t)(temp_angle /((int32_t)((uint32_t)1U<<(MOTOR_LIB_ANGLESCALE_SHIFT +
                  MOTOR_LIB_CORDZ_DATA_POSSCALE_SHIFT))));

  return (((MATH->CORRX*(uint32_t)MOTOR_LIB_INV_KCORDIC)/ ((uint32_t)MOTOR_LIB_KCORDIC_SCALE*(uint32_t)MOTOR_LIB_AMPSCALE)));
}

/**
 * This function is the implementation of Polar to cartesian Transformation.
 */
int32_t MOTOR_LIB_Pol2Car(int16_t angle,uint32_t amplitude, int32_t *x)
{
  uint32_t temp_angle;
  int32_t temp_x, temp_y;
  temp_angle =(uint16_t)angle;
  /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_CIRCULAR_MODE +
              (uint32_t) MOTOR_LIB_ROTATION_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_UNSIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

  /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ= (uint32_t)(temp_angle <<(MOTOR_LIB_ANGLESCALE_SHIFT + MOTOR_LIB_CORDZ_DATA_POSSCALE_SHIFT));

  MATH->CORDY = (((uint32_t)0 << MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
             (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)amplitude<< MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while((MATH->STATC & 0x1U) == 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }
  /* Update the x and y Values*/
  temp_x = (int32_t)MATH->CORRX;
  temp_y = (int32_t)MATH->CORRY;

  *x = (((temp_x*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));
  return (((temp_y*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));

}

/**
 * This function is the implementation of park transform.
 */
int32_t MOTOR_LIB_ParkTransform(int32_t i_alpha,int32_t i_beta,int16_t angle,int32_t *iq)
{
  uint32_t temp_angle;
  int32_t temp_d, temp_q;
  temp_angle =(uint16_t)angle;
  /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_CIRCULAR_MODE +
              (uint32_t) MOTOR_LIB_ROTATION_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_UNSIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

   /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ = (uint32_t)(temp_angle <<(MOTOR_LIB_ANGLESCALE_SHIFT + MOTOR_LIB_CORDZ_DATA_POSSCALE_SHIFT));

  MATH->CORDY = (((uint32_t)i_alpha<< MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
             (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)i_beta<< MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while(( MATH->STATC & 0x1U ) == 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }
  /* Update the Amplitude and Angle Values*/
  temp_q = (int32_t)MATH->CORRX;
  temp_d = (int32_t)MATH->CORRY;

  *iq = (((temp_q*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));
  return (((temp_d*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));
}

/**
 * This function is the implementation of Inverse park transform.
 */
int32_t MOTOR_LIB_IParkTransform(int32_t vd,int32_t vq,int16_t angle,int32_t *vbeta)
{
  uint32_t temp_angle;
  int32_t temp_beta, temp_alpha;
  temp_angle =(uint16_t)angle;
    /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_CIRCULAR_MODE +
              (uint32_t) MOTOR_LIB_ROTATION_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_UNSIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

   /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ= (uint32_t)(temp_angle <<(MOTOR_LIB_ANGLESCALE_SHIFT + MOTOR_LIB_CORDZ_DATA_POSSCALE_SHIFT));

  MATH->CORDY = (((uint32_t)vq<< MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
             (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)vd<< MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while((MATH->STATC & 0x1U)== 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }

  /* Update the Amplitude and Angle Values*/
  temp_alpha  =(int32_t)MATH->CORRX;
  temp_beta = (int32_t)MATH->CORRY;
  *vbeta = (temp_beta / MOTOR_LIB_AMPSCALE);
  
  *vbeta = (((temp_beta*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));
  return (((temp_alpha*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_KCORDIC_SCALE*MOTOR_LIB_AMPSCALE)));
}

/**
 * This function returns the magnitude of resultant vector in circular mode.(sqrt(x^2+y^2))
 */
uint32_t MOTOR_LIB_ResultantMagnitudeCircular(int32_t x,int32_t y)
{
  uint32_t resultant_magnitude;
  /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_CIRCULAR_MODE +
              (uint32_t) MOTOR_LIB_VECTORING_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_UNSIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

  /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ = (((uint32_t)0 << MATH_CORDZ_DATA_Pos) & ((uint32_t)MATH_CORDZ_DATA_Msk)) |
              (MATH->CORDZ & ((uint32_t)~((uint32_t)MATH_CORDZ_DATA_Msk)));

  MATH->CORDY = (((uint32_t)y << MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
              (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)x << MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while(( MATH->STATC & 0x1U )== 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }
  /* Return the Amplitude */
  resultant_magnitude = (uint32_t)((MATH->CORRX * MOTOR_LIB_MPS_DIV2)>> MOTOR_LIB_CORRX_RESULT_POSSCALE);
  resultant_magnitude = (uint32_t)((resultant_magnitude * MOTOR_LIB_INV_KCORDIC)>> MOTOR_LIB_SHIFT8);
  return (resultant_magnitude);
}

/**
 * This function is the implementation of finding the magnitude of the vector.(sqrt(x^2-y^2))
 */
uint32_t MOTOR_LIB_ResultantMagnitudeHyperbolic(int32_t x,int32_t y)
{
  uint32_t resultant_magnitude;
  /*Clearing previous values of CORDX,CORDY and CORDZ Registers*/
  MATH->STATC = (uint32_t) MOTOR_LIB_CLEAR_KEEPX +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPY +
                (uint32_t) MOTOR_LIB_CLEAR_KEEPZ;

  MATH->CON = (uint32_t) MOTOR_LIB_HYPERBOLIC_MODE +
              (uint32_t) MOTOR_LIB_VECTORING_MODE +
              (uint32_t) MOTOR_LIB_CORDIC_AUTO_START +
              (uint32_t) MOTOR_LIB_SIGNED_X_RESULT +
              (uint32_t) MOTOR_LIB_MPS_DIVBY2;

  /*Loading CORDX,CORDY and CORDZ Registers*/
  MATH->CORDZ = (((uint32_t)0 << MATH_CORDZ_DATA_Pos) & ((uint32_t)MATH_CORDZ_DATA_Msk)) |
              (MATH->CORDZ & ((uint32_t)~((uint32_t)MATH_CORDZ_DATA_Msk)));

  MATH->CORDY = (((uint32_t)y << MATH_CORDY_DATA_Pos) & ((uint32_t)MATH_CORDY_DATA_Msk)) |
              (MATH->CORDY & ((uint32_t)~((uint32_t)MATH_CORDY_DATA_Msk)));

  MATH->CORDX = (((uint32_t)x << MATH_CORDX_DATA_Pos) & ((uint32_t)MATH_CORDX_DATA_Msk)) |
             (MATH->CORDX & ((uint32_t)~((uint32_t)MATH_CORDX_DATA_Msk)));

  while(( MATH->STATC & 0x1U )== 1U)
  {
  /*Checking for CORIDC Busy Bit*/
  }
  /* Return only the Magnitude */
  resultant_magnitude = (uint32_t)((MATH->CORRX * MOTOR_LIB_MPS_DIV2)>> MOTOR_LIB_CORRX_RESULT_POSSCALE) & MOTOR_LIB_2POW23;
  resultant_magnitude = (uint32_t)((resultant_magnitude * MOTOR_LIB_INV_KCORDIC_HYPERBOLIC)>> MOTOR_LIB_SHIFT8);
  return (resultant_magnitude);
}
#else

uint32_t MOTOR_LIB_Car2Pol(int32_t x,int32_t y, int16_t *angle)
{
  uint8_t cordic_count;
  int32_t dx,flag = 0, temp_angle = 0,cord_y = y,cord_x = x,temp_cordic_scale,return_val;
  uint32_t temp_cordic_s;
  if (cord_x < 0)
  {
     cord_x = (-cord_x);
    flag = 1;
  }

  for (cordic_count = 0U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1U<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s;
    dx = cord_x / temp_cordic_scale;
    if ( cord_y > 0 )
    {
      cord_x += (cord_y / temp_cordic_scale);
      cord_y -= dx;
      temp_angle -= MOTOR_LIB_E[cordic_count];
    }
    else
    {
      cord_x -= (cord_y / temp_cordic_scale);
      cord_y += dx;
      temp_angle += MOTOR_LIB_E[cordic_count];
    }
  }
  if ( flag > 0)
  {
    *angle = (int16_t)((temp_angle + (int32_t)0x80000000)/(int32_t)MOTOR_LIB_2POW16);
  }
  else
  {
    *angle = (int16_t)((-temp_angle)/(int32_t)MOTOR_LIB_2POW16);
  }
  return_val = (int32_t)((cord_x * MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_AMPSCALE);
  return (uint32_t)(return_val);
}

/**
 * This function returns the magnitude of resultant vector in circular mode.(sqrt(x^2+y^2))
 */
uint32_t MOTOR_LIB_ResultantMagnitudeCircular(int32_t x,int32_t y)
{
  uint8_t cordic_count;
  int32_t dx, cord_y = y,cord_x = x,temp_cordic_scale,return_val;
  uint32_t temp_cordic_s;
  if (cord_x < 0)
  {
     cord_x = (-cord_x);
  }

  for (cordic_count = 0U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1U<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s;
    dx = cord_x / temp_cordic_scale;
    if ( cord_y > 0 )
    {
      cord_x += (cord_y / temp_cordic_scale);
      cord_y -= dx;
    }
    else
    {
      cord_x -= (cord_y / temp_cordic_scale);
      cord_y += dx;
    }
  }

  return_val = (int32_t)((cord_x * MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_AMPSCALE);
  return (uint32_t)(return_val);
}

/**
 * This function is the implementation of finding the magnitude of the vector in hyperbolic mode.(sqrt(x^2-y^2))
 * x should be greater than y
 */
uint32_t MOTOR_LIB_ResultantMagnitudeHyperbolic(int32_t x,int32_t y)
{
  uint8_t cordic_count,repeated_iteration_count;
  int32_t dx,dy, cord_y = y,cord_x = x,temp_cordic_scale,return_val;
  uint32_t temp_cordic_s;
  /* Used for the repeated (3*repeated_iteration_count + 1) iteration steps */
  repeated_iteration_count = 4U;

  if (cord_x < 0)
  {
    cord_x = (-cord_x);
  }

  for (cordic_count = 1U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1U<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s;
    dx = cord_x / temp_cordic_scale;
    dy = cord_y / temp_cordic_scale;
    if ( cord_y > 0 )
    {
      cord_x -= dy;
      cord_y -= dx;
    }
    else
    {
      cord_x += dy;
      cord_y += dx;
    }

    if (cordic_count == repeated_iteration_count)
    {
      dx = cord_x / temp_cordic_scale;
      dy = cord_y / temp_cordic_scale;
      if ( cord_y > 0 )
      {
        cord_x -= dy;
        cord_y -= dx;
      }
      else
      {
        cord_x += dy;
        cord_y += dx;
      }
      repeated_iteration_count = (uint8_t)((3U * repeated_iteration_count) + 1U);
    }
  }
  return_val = (int32_t)((cord_x * MOTOR_LIB_INV_KCORDIC_HYPERBOLIC)/ MOTOR_LIB_AMPSCALE);
  return ((uint32_t)return_val);
}

/**
 * This function is the implementation of Polar to cartesian Transformation.
 */
int32_t MOTOR_LIB_Pol2Car(int16_t angle,uint32_t amplitude, int32_t *x)
{
  uint8_t cordic_count, flag=0U;
  int32_t dx,temp_cordic_scale;
  int32_t cord_x = (int32_t)amplitude,cord_y=0;
  int32_t temp_angle = angle * (int32_t)MOTOR_LIB_2POW16;
  uint32_t  temp_cordic_s;

  if (MOTOR_LIB_abs(temp_angle) > 0x40000000U)
  {
    temp_angle = (int32_t)((int32_t)0x80000000 - temp_angle);
    cord_x = -cord_x;
    flag = 1U;
  }

  for (cordic_count = 0U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s;
    dx = cord_x / temp_cordic_scale;
    if (temp_angle > 0)
    {
      cord_x -= cord_y / temp_cordic_scale;
      cord_y += dx;
      temp_angle -= MOTOR_LIB_E[cordic_count];
    }
    else
    {
      cord_x += cord_y / temp_cordic_scale;
      cord_y -= dx;
      temp_angle += MOTOR_LIB_E[cordic_count];
    }
  }

  if ( flag > 0U)
  {
    cord_y = (-cord_y);
  }

  cord_x = (((cord_x*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_AMPSCALE)));

  *x = cord_x;
  return ((cord_y*MOTOR_LIB_INV_KCORDIC)/ (MOTOR_LIB_AMPSCALE));

}

/**
 * This function is the implementation of park transform.
 */
int32_t MOTOR_LIB_ParkTransform(int32_t i_alpha,int32_t i_beta,int16_t angle,int32_t *iq)
{
  uint8_t cordic_count, flag=0U;
  int32_t temp_cordic_scale,dx,cord_x =i_beta,cord_y=i_alpha;
  int32_t temp_angle = angle * (int32_t)MOTOR_LIB_2POW16;
  uint32_t temp_cordic_s;

  if ( MOTOR_LIB_abs(temp_angle) > 0x40000000U)
  {
    temp_angle = (int32_t)((int32_t)0x80000000 - temp_angle);
    cord_x = -cord_x;
    flag = 1U;
  }


  for (cordic_count = 0U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s ;
    dx = cord_x / temp_cordic_scale;

    if (temp_angle > 0)
    {
      cord_x -= cord_y / temp_cordic_scale;
      cord_y += dx;
      temp_angle -= MOTOR_LIB_E[cordic_count];
    }
    else
    {
      cord_x += cord_y / temp_cordic_scale;
      cord_y -= dx;
      temp_angle += MOTOR_LIB_E[cordic_count];
    }
  }
  *iq = (int32_t)((cord_x*MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_MPS);
  if (flag > 0U)
  {
    cord_y = (-cord_y);
  }
  return ((int32_t)((cord_y*MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_MPS));
}
/**
 * This function is the implementation of Inverse park transform.
 * Valpha  = k/4*(Vd*cos(Angle)- Vq*sin(Angle))
 * Vbeta   = k/4*(Vd*sin(Angle)+ Vq*cos(Angle))
 */
int32_t MOTOR_LIB_IParkTransform(int32_t vd,int32_t vq,int16_t angle,int32_t *v_beta)
{
  uint8_t cordic_count, flag=0U;
  int32_t temp_cordic_scale,dx,cord_x =vd,cord_y=vq;
  int32_t temp_angle = angle * (int32_t)MOTOR_LIB_2POW16;
  uint32_t temp_cordic_s;

  if ( MOTOR_LIB_abs(temp_angle) > 0x40000000U)
  {
    temp_angle = (int32_t)((int32_t)0x80000000 - temp_angle);
    cord_x = -cord_x;
    flag = 1U;
  }

  for (cordic_count = 0U; cordic_count < MOTOR_LIB_CORDIC_ITERATIONS; cordic_count++)
  {
    temp_cordic_s = ((uint32_t)1<< cordic_count);
    temp_cordic_scale =(int32_t)temp_cordic_s ;
    dx = cord_x / temp_cordic_scale;

    if (temp_angle > 0)
    {
      cord_x -= cord_y / temp_cordic_scale;
      cord_y += dx;
      temp_angle -= MOTOR_LIB_E[cordic_count];
    }
    else
    {
      cord_x += cord_y / temp_cordic_scale;
      cord_y -= dx;
      temp_angle += MOTOR_LIB_E[cordic_count];
    }
  }
  if (flag > 0U)
  {
    cord_y = ( -cord_y );
  }
  *v_beta = (int32_t)((cord_y*MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_MPS);
  return ((int32_t)((cord_x*MOTOR_LIB_INV_KCORDIC)/ MOTOR_LIB_MPS));
}

#endif
/**
 * This function is the implementation of Clarke transform using 2 Phase current.
 */

int32_t MOTOR_LIB_ClarkTransform(int32_t phase_a, int32_t phase_b, int32_t *i_alpha)
{
  int32_t temp_scale;
  temp_scale = (int32_t)((uint32_t)1U<<15U);
  *i_alpha  = (int32_t)(phase_a);
  return  (int32_t)(((phase_a + (2 * phase_b))*(MOTOR_LIB_INVSQRT3))/ temp_scale);
}
/**
 * This function is the implementation of Clarke transform using 3 Phase current.
 */
int32_t MOTOR_LIB_ClarkTransform2(int32_t phase_a, int32_t phase_b, int32_t phase_c, int32_t *i_alpha )
{
   int32_t temp_scale;
   temp_scale = (int32_t)((uint32_t)1U<<15U);
  *i_alpha  = (int32_t)(((2*phase_a)+phase_b) + phase_c);
  return (int32_t)(((phase_b - phase_c)*(MOTOR_LIB_INVSQRT3))/ temp_scale);
}


/**
 * This function is the implementation of PI controller. 
 */
void MOTOR_LIB_PIController
(
    MOTOR_LIB_PIHandle_t *handle_ptr,
    int32_t error_val
)
{
  int32_t output;
  int32_t buf ;
  /* yn(k+1)= yn(k) + ki * E(k) */
  buf = (int32_t)handle_ptr->i_buf + ((int32_t)handle_ptr->ki * error_val);

  /* Checking the buffer limits in the range of Yimin to Yimax */
  handle_ptr->i_buf = (int32_t)MOTOR_LIB_MinMaxLimiter(buf,(int32_t)(handle_ptr->y_imax),(int32_t)(handle_ptr->y_imin));

  /* y(k+1) = yn(k+1) + kp * E(k) * 64 */
  output = (int32_t) ((int32_t)handle_ptr->kp * error_val);

  output = (handle_ptr->i_buf / MOTOR_LIB_PI_KISCALE) +(output / MOTOR_LIB_PI_KPSCALE);
  
  /* Checking the buffer limits in the range of Ymin to Ymax */
  handle_ptr->pi_out_val= (int16_t)MOTOR_LIB_MinMaxLimiter(output,(int32_t)(handle_ptr->y_max),
                                                               (int32_t)(handle_ptr->y_min));
}

/**
 * This function is the implementation of Feed forward controller.
 */
void MOTOR_LIB_FeedForwardController( MOTOR_LIB_FeedForwardHandle_t *handle_ptr,int32_t feedforward_input_val )
{
  int32_t anti_aliasing_input,derivative_out,feedforward_gain_out,limiter_out;
  uint32_t anti_aliasing_out;
  anti_aliasing_input = handle_ptr->antinoise_filter_buffer - feedforward_input_val;
  anti_aliasing_out = MOTOR_LIB_abs(anti_aliasing_input);

  /* Anti aliasing filter for system stability - ignores small variations in i/p */
  if ( anti_aliasing_out > handle_ptr->dead_band )
  {
    /* storing input signal value in buffer for next cycle */
    handle_ptr->antinoise_filter_buffer = feedforward_input_val;

    /* Call PT1 low pass filter */
    MOTOR_LIB_PT1Controller(handle_ptr->pt1_filter_ptr,feedforward_input_val);

    /* FeedForward gain operation */
    feedforward_gain_out  = handle_ptr->pt1_filter_ptr->pt1_out_val * (int32_t)handle_ptr->feedforward_gain;

    /* Derivative operation on input signal  */
    derivative_out = (int32_t)handle_ptr->differential_gain * ( feedforward_gain_out - handle_ptr->differential_buffer);
    handle_ptr->differential_buffer = feedforward_gain_out;

    /* FeedForward output limiter operation */
    limiter_out = MOTOR_LIB_MinMaxLimiter(derivative_out,(int32_t)handle_ptr->y_max,(int32_t)handle_ptr->y_min);

    /* Dead time configuration */
        if (handle_ptr->dead_time_enable > 0U)
        {
          /* Dead time is configured store the result in buffer and send out next cycle */
          handle_ptr->feedforward_out_val = handle_ptr->dead_time_buffer;
          handle_ptr->dead_time_buffer = limiter_out;
        }
        else
        {
          /* Delay is not configured */
          handle_ptr->feedforward_out_val = limiter_out;
        }
  }
  else
  {
      /* FeedForward Controller output set to zero */
    handle_ptr->feedforward_out_val = 0;
  }
}

/**
 * This function is the implementation of PT1 filter. 
 */
void MOTOR_LIB_PT1Controller(MOTOR_LIB_PT1Handle_t *handle_ptr,int32_t actual_val)
{
  int32_t temp_buff;
  /* yn+1=yn + z1*(x - yn) */
  temp_buff = handle_ptr->pt1_buf + (handle_ptr->z1*(actual_val - (handle_ptr->pt1_buf/(int32_t)MOTOR_LIB_2POW16)));
  /* Checking the buffer limits in the range of Ymin to Ymax */
  handle_ptr->pt1_buf = MOTOR_LIB_MinMaxLimiter(temp_buff,handle_ptr->y_max,handle_ptr->y_min);
  handle_ptr->pt1_out_val = (handle_ptr->pt1_buf/(int32_t)MOTOR_LIB_2POW16);
}
/**
 * This function is the implementation of PT12 filter. 
 */
void MOTOR_LIB_PT12Controller(MOTOR_LIB_PT1Handle_t *handle_ptr,int32_t actual_val)
{ 
  int32_t temp_buff;
  /* yn+1=yn + z1*x - z2*yn; */
  temp_buff = handle_ptr->pt1_buf + ( (handle_ptr->z1* actual_val) -
                                  (handle_ptr->z2*(handle_ptr->pt1_buf/(int32_t)MOTOR_LIB_2POW16)));
   
  /* Checking the buffer limits in the range of Ymin to Ymax */
  handle_ptr->pt1_buf = MOTOR_LIB_MinMaxLimiter(temp_buff,handle_ptr->y_max,handle_ptr->y_min);

  handle_ptr->pt1_out_val = (handle_ptr->pt1_buf/(int32_t)MOTOR_LIB_2POW16);
} 

/**
 * @endcond
 */
/*CODE_BLOCK_END*/