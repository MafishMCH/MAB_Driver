/*********************************************************************************************************************
* DAVE APP Name : ADC_QUEUE       APP Version: 4.0.20
*
* NOTE:
* This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
*********************************************************************************************************************/

/**
 * @cond
 ***********************************************************************************************************************
 *
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
 * 2015-02-16:
 *     - Initial version for DAVEv4.<BR>
 *
 * 2015-04-27:
 *     - Optimized the ADC_QUEUE_AllEntriesInserted.<BR>
 *
 * 2015-06-20:
 *     - Updated the copyright section.<BR>
 *     
 * 2015-06-26:
 *     - Added flushing of all the queue entries before inserting to the Hardware buffer.<BR>
 *
 * 2015-07-09:
 *     - Documentation updated.<BR>
 *
 * 2015-07-31:
 *     - Updated the APP to support new devices.<BR>
 *
 * 2015-10-01:
 *     - Iclass signal added to the APP.<BR>
 *
 * 2015-10-08:
 *     - Added support for XMC4700/XMC4800 devices.<BR>
 *
 * 2015-12-15:     
 *          - Added support for XMC4300 devices.<BR>
 *          - Updated the APP to optimize the request source callback.<BR>
 *          - APIs touched are ADC_QUEUE_InsertQueueEntry(), ADC_QUEUE_ResetListContents(), 
 *            ADC_QUEUE_ReqSrcEventHandler().<BR>       
 *
 * 2016-01-19:
 *          - Added new APIs ADC_QUEUE_EnableArbitration() and ADC_QUEUE_DisableArbitration().<BR>
 *
 * 2016-04-03:
 *          - Total conversion time now includes post calibration time as well for XMC1x.<BR>
 *
 * 2016-08-18:
 *     - Minimum sample time at low frequencies changed to incorporate Errata ADC_AI.H006 and ADC_AI.H007.
 *
 * @endcond
 *
 */

#ifndef ADC_QUEUE_CONF_H
#define ADC_QUEUE_CONF_H

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/

#define ADC_QUEUE_0_ReqSrcEventHandler 	  IRQ_Hdlr_19

#define ADC_QUEUE_1_ReqSrcEventHandler 	  IRQ_Hdlr_25

#define ADC_QUEUE_2_ReqSrcEventHandler 	  IRQ_Hdlr_26

#define ADC_QUEUE_MAJOR_VERSION (4) /**< Major version number of ADC_QUEUE APP*/
#define ADC_QUEUE_MINOR_VERSION (0) /**< Minor version number of ADC_QUEUE APP*/
#define ADC_QUEUE_PATCH_VERSION (20) /**< Patch version number of ADC_QUEUE APP*/


#define ADC_QUEUE_NUM_APPINSTANCES   (3U)
#define ADC_QUEUE_MAXCHANNELS        (8U)

#endif /* ADC_QUEUE_H */

 
/*CODE_BLOCK_END*/

