/*********************************************************************************************************************
* DAVE APP Name : DAC       APP Version: 4.0.16
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/**
 * @cond
 ***********************************************************************************************************************

 * Copyright (c) 2015-2016, Infineon Technologies AG
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
 *     - Initial version<br>
 *
 * 2015-06-20:
 *     - Service request option is restricted only to data mode.<br>
       - Pattern generation minimum amplitude is restricted to 0.004V.<br>
 *
 * 2015-12-15:
 *     - ANALOG_IO APP Init function call added
 * 2016-07-08:
 *     - Fixed incorrect case for an included header.<br>
 *
 * @endcond
 *
 */
/***********************************************************************************************************************
 * HEADER FILES                                                                                                      
 **********************************************************************************************************************/
#include "dac.h"

/**********************************************************************************************************************
* DATA STRUCTURES
**********************************************************************************************************************/


/* DAC Channel Configuration */
XMC_DAC_CH_CONFIG_t ANALOG_Channel_Config = 
{
  .data_type       = (uint8_t)XMC_DAC_CH_DATA_TYPE_UNSIGNED,     
  .output_negation = (uint8_t)XMC_DAC_CH_OUTPUT_NEGATION_DISABLED,
  .output_scale    = (uint8_t)XMC_DAC_CH_OUTPUT_SCALE_NONE,     
  .output_offset   = 0U
};


/* DAC App Handle */
DAC_t ANALOG =
{
  .dac_module_ptr                  = (XMC_DAC_t  *)(void *)DAC,
  .dac_config_ptr                  = &ANALOG_Channel_Config,
#if (DAC_ANALOG_IO_USED == 1U)
  .analog_io_config_ptr            = NULL,
#endif
  .dac_mode                        = XMC_DAC_CH_MODE_SINGLE,
  .dac_input_trigger               = XMC_DAC_CH_TRIGGER_INTERNAL,
  .dac_sign_output                 = XMC_DAC_CH_PATTERN_SIGN_OUTPUT_DISABLED,
  .dac_frequency                   = 0U,
  .dac_ramp_start                  = 0U,
  .dac_ramp_stop                   = 0U,
  .dac_ramp_start_mv               = 0U,
  .dac_ramp_stop_mv                = 0U,
  .dac_service_request             = 0U,
  .dac_coupled                     = 0U,
  .dac_ch                          = 1U,
  .dac_pat_table_ptr               = NULL,
  .dac_start_after_init            = 1U
};



/* DAC Channel Configuration */
XMC_DAC_CH_CONFIG_t ANALOG2_Channel_Config = 
{
  .data_type       = (uint8_t)XMC_DAC_CH_DATA_TYPE_UNSIGNED,     
  .output_negation = (uint8_t)XMC_DAC_CH_OUTPUT_NEGATION_DISABLED,
  .output_scale    = (uint8_t)XMC_DAC_CH_OUTPUT_SCALE_NONE,     
  .output_offset   = 0U
};


/* DAC App Handle */
DAC_t ANALOG2 =
{
  .dac_module_ptr                  = (XMC_DAC_t  *)(void *)DAC,
  .dac_config_ptr                  = &ANALOG2_Channel_Config,
#if (DAC_ANALOG_IO_USED == 1U)
  .analog_io_config_ptr            = NULL,
#endif
  .dac_mode                        = XMC_DAC_CH_MODE_SINGLE,
  .dac_input_trigger               = XMC_DAC_CH_TRIGGER_INTERNAL,
  .dac_sign_output                 = XMC_DAC_CH_PATTERN_SIGN_OUTPUT_DISABLED,
  .dac_frequency                   = 0U,
  .dac_ramp_start                  = 0U,
  .dac_ramp_stop                   = 0U,
  .dac_ramp_start_mv               = 0U,
  .dac_ramp_stop_mv                = 0U,
  .dac_service_request             = 0U,
  .dac_coupled                     = 0U,
  .dac_ch                          = 0U,
  .dac_pat_table_ptr               = NULL,
  .dac_start_after_init            = 1U
};


