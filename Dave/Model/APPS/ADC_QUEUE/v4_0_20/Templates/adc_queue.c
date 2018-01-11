/**
 * @file adc_queue.c
 * @date 2016-08-18
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * ADC_QUEUE v4.0.20 - Provides configurations for queue request source of VADC
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

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/

/** Inclusion of header file */
#include "adc_queue.h"

/***********************************************************************************************************************
 * MACROS
 **********************************************************************************************************************/
#define ADC_QUEUE_INVALID_CHANNEL ((uint8_t)0xff)

/***********************************************************************************************************************
 * LOCAL DATA
 **********************************************************************************************************************/
/**
 * @ingroup ADC_QUEUE_datastructures
 * @{
 */
/**
 * This is a 2D array of queue entry lists. There are two lists maintained for each of the groups. The first list
 * contains entries which can be indexed using channel numbers. This is needed during interrupt processing. The second
 * list contains the same set of entries which can now be indexed by means of queue position entries. This is needed
 * for submitting the queue entries in the correct order to the LLD.
 */
ADC_QUEUE_ENTRY_t *adc_queue_list_ptrs[ADC_QUEUE_NUM_GROUPS][ADC_QUEUE_NUM_LISTS];

static ADC_QUEUE_EVENT_CALLBACK adc_queue_callback_list[ADC_QUEUE_NUM_GROUPS][ADC_QUEUE_NUM_CHANNELS];
/**
 * @}
 */
/***********************************************************************************************************************
 * LOCAL ROUTINES
 **********************************************************************************************************************/
/** This function populates the ADC_QUEUE list of pointers and called from ADC_QUEUE_Init().
 * ADC_QUEUE_ResetListContents() is in turn called from the API.
 * This API will just initialize the data structure adc_queue_list_ptrs with the appropriate pointers.
 * This would also reset the values in these lists to defaults.*/
void ADC_QUEUE_lInitializeListToDefaults(uint8_t instance_num);

/* Declaration for the memcpy library function*/
void *memcpy(void *str1, const void *str2, size_t n);

/**********************************************************************************************************************
* API IMPLEMENTATION
**********************************************************************************************************************/
/*This function returns the version of the ADC_QUEUE APP*/
DAVE_APP_VERSION_t ADC_QUEUE_GetAppVersion(void)
{
  DAVE_APP_VERSION_t version;

  version.major = (uint8_t) ADC_QUEUE_MAJOR_VERSION;
  version.minor = (uint8_t) ADC_QUEUE_MINOR_VERSION;
  version.patch = (uint8_t) ADC_QUEUE_PATCH_VERSION;

  return version;
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* API to reset (De-Initialise) contents of all queue entries */
void ADC_QUEUE_ResetListContents(uint8_t instance_num)
{
  XMC_ASSERT("ADC_QUEUE_ResetListContents:Wrong Instance number",(instance_num < ADC_QUEUE_NUM_APPINSTANCES))

  /*
   * Now reset the contents of all the lists. Application later uses ADC_QUEUE_Insert() to initialise specific entries.
   */
  memset(adc_queue_list_ptrs[instance_num][ADC_QUEUE_CH_INDEX], ADC_QUEUE_INVALID_CHANNEL ,
		 (sizeof(ADC_QUEUE_ENTRY_t)* ADC_QUEUE_NUM_CHANNELS));
  memset(adc_queue_list_ptrs[instance_num][ADC_QUEUE_POS_INDEX], ADC_QUEUE_INVALID_CHANNEL ,
		 (sizeof(ADC_QUEUE_ENTRY_t)* ADC_QUEUE_NUM_CHANNELS));
  memset(adc_queue_callback_list[instance_num], 0 ,(sizeof(ADC_QUEUE_EVENT_CALLBACK) * ADC_QUEUE_NUM_CHANNELS));
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
ADC_QUEUE_STATUS_t ADC_QUEUE_Init(ADC_QUEUE_t *const handle_ptr)
{
  ADC_QUEUE_STATUS_t status;

  XMC_ASSERT("ADC_QUEUE_Init:Invalid handle_ptr", (handle_ptr != NULL))

  if (ADC_QUEUE_UNINITIALIZED == handle_ptr->init_status)
  {
    /*Initialization of APP 'GLOBAL_ADC'*/
    status = (ADC_QUEUE_STATUS_t) GLOBAL_ADC_Init(handle_ptr->global_handle);

    /*Class Configuration*/
    XMC_VADC_GROUP_InputClassInit(handle_ptr->group_handle,handle_ptr->iclass_config_handle,
                                  XMC_VADC_GROUP_CONV_STD,(uint32_t)handle_ptr->iclass_num);

    /* Initialize the Queue hardware */
    XMC_VADC_GROUP_QueueInit(handle_ptr->group_handle,handle_ptr->queue_config_handle);
  
    /* Configure the gating mode for queue*/
    XMC_VADC_GROUP_QueueSetGatingMode(handle_ptr->group_handle, handle_ptr->gating_mode);

    /* Clear the internal data structure of queue APP */
    ADC_QUEUE_lInitializeListToDefaults(handle_ptr->instance_number);
  
    /*Interrupt Configuration*/
    if ((bool)true == handle_ptr->rs_intr_handle->interrupt_enable)
    {
#if (UC_FAMILY == XMC1)
      NVIC_SetPriority((IRQn_Type)handle_ptr->rs_intr_handle->node_id, handle_ptr->rs_intr_handle->priority);
#else
      NVIC_SetPriority((IRQn_Type)handle_ptr->rs_intr_handle->node_id,
                        NVIC_EncodePriority(NVIC_GetPriorityGrouping(),
                        handle_ptr->rs_intr_handle->priority,handle_ptr->rs_intr_handle->sub_priority));
#endif
#ifdef ADC_QUEUE_NON_DEFAULT_IRQ_SOURCE_SELECTED
  XMC_SCU_SetInterruptControl(handle_ptr->rs_intr_handle->node_id,
                              ((handle_ptr->rs_intr_handle->node_id << 8) | handle_ptr->rs_intr_handle->irqctrl));
#endif

      /* Connect RS Events to NVIC nodes */
      XMC_VADC_GROUP_QueueSetReqSrcEventInterruptNode(handle_ptr->group_handle,
                                                      (XMC_VADC_SR_t)handle_ptr->srv_req_node);

      /* Enable Interrupt */
      NVIC_EnableIRQ((IRQn_Type)handle_ptr->rs_intr_handle->node_id);
    }
    handle_ptr->init_status = status;
  }
  return (handle_ptr->init_status);
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void ADC_QUEUE_InsertQueueEntry(ADC_QUEUE_t* handle_ptr, const ADC_QUEUE_ENTRY_t *entry)
{
  ADC_QUEUE_ENTRY_t *node;
  ADC_QUEUE_EVENT_CALLBACK *callback_node,entry_callback;
  int8_t i;
  bool insert_yes;

  XMC_ASSERT("ADC_QUEUE_Insert:Invalid handle_ptr", (handle_ptr != NULL))
  XMC_ASSERT("ADC_QUEUE_Insert:Invalid queue entry", (entry != NULL))

  /* 1. Insert this entry into a list which is arranged on the basis of channel numbers */
  node = adc_queue_list_ptrs[handle_ptr->instance_number][ADC_QUEUE_CH_INDEX];
  node = node + (entry->channel_number);
  memcpy((void *)node, (void *)entry, sizeof(ADC_QUEUE_ENTRY_t)); 

  /* Insert the same entry into a list which is arranged on the basis of the position of the channel in the queue */
  node = adc_queue_list_ptrs[handle_ptr->instance_number][ADC_QUEUE_POS_INDEX];
  node = node + (entry->queue_position);
  memcpy((void *)node, (void *)entry, sizeof(ADC_QUEUE_ENTRY_t));

  entry_callback = entry->adc_event_callback;
  callback_node = adc_queue_callback_list[handle_ptr->instance_number];
  insert_yes = (bool)true;
  if(NULL != entry_callback)
  {
    for(i = handle_ptr->number_valid_callback; i >= (int32_t)0;i--)
    {
      if(*(callback_node + i) == entry_callback)
      {
        insert_yes = false;
        break;
      }
    }

    if(insert_yes != false)
    {
      adc_queue_callback_list[handle_ptr->instance_number][handle_ptr->number_valid_callback++] = entry_callback;
    }
  }


}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void ADC_QUEUE_AllEntriesInserted(ADC_QUEUE_t* handle_ptr)
{
  ADC_QUEUE_ENTRY_t *node;
  ADC_QUEUE_ENTRY_t *start_node;
  XMC_VADC_QUEUE_ENTRY_t entry;
  uint32_t pos_index;
  
  XMC_ASSERT("ADC_QUEUE_AllEntriesInserted:Invalid handle_ptr", (handle_ptr != NULL))

  /* All entries have been submitted. The queue hardware may now be programmed in the right order. */
  start_node = adc_queue_list_ptrs[handle_ptr->instance_number][ADC_QUEUE_POS_INDEX];

  /* Flush any entries that are currently in the queue buffer*/
  XMC_VADC_GROUP_QueueFlushEntries(handle_ptr->group_handle);

  for (pos_index = 0U; pos_index < ADC_QUEUE_NUM_CHANNELS; pos_index++)
  {
    node = start_node +  pos_index;
    
    if (ADC_QUEUE_INVALID_CHANNEL != node->channel_number)
    {
      /* Create a queue entry object for the LLD */
      entry.channel_num   = (uint8_t) node->channel_number;
      entry.refill_needed = (uint32_t) node->refill_needed;
      entry.generate_interrupt = (uint32_t) node->rs_event_notification;
      entry.external_trigger   = (uint32_t)node->trigger_needed;
      
      /* Request the LLD to insert the channel */
      XMC_VADC_GROUP_QueueInsertChannel(handle_ptr->group_handle, entry);
    }
  }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void ADC_QUEUE_ChannelEventHandler(ADC_QUEUE_t* handle_ptr)
{
  uint32_t ceflag;
  uint32_t ch_index;
  ADC_QUEUE_ENTRY_t *node;
  
  XMC_ASSERT("ADC_QUEUE_ChannelEventHandler:Invalid handle_ptr", (handle_ptr != NULL))

  /*. Identify the channel which has just been converted */
  ceflag = handle_ptr->group_handle->CEFLAG;
 
  for (ch_index = 0U; ch_index < ADC_QUEUE_NUM_CHANNELS; ch_index++)
  {
    if ((ceflag >> ch_index) & 1U)
    {
      break;
    }
  }
  
  if (ADC_QUEUE_NUM_CHANNELS != ch_index )
  {
    /* Clear the event and exit the ISR */
    XMC_VADC_GROUP_ChannelClearEvent(handle_ptr->group_handle,ch_index);

    /* Get the list entry corresponding to this channel */
    node = adc_queue_list_ptrs[handle_ptr->instance_number][ADC_QUEUE_CH_INDEX];
    node = node + ch_index;

    /* Find out if application has requested for channel event notification for the channel in question */
    if ((bool)true == node->ch_event_notification)
    {
      (node->adc_event_callback)();
    }
  }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void ADC_QUEUE_ReqSrcEventHandler(ADC_QUEUE_t* handle_ptr)
{
  int32_t pos_index;
  ADC_QUEUE_EVENT_CALLBACK *callback_node;
  
  XMC_ASSERT("ADC_QUEUE_ReqSrcEventHandler:Invalid handle_ptr", (handle_ptr != NULL))

  XMC_VADC_GROUP_QueueClearReqSrcEvent(handle_ptr->group_handle);
  
  callback_node = adc_queue_callback_list[handle_ptr->instance_number];
  /* Iterate over all the valid entries and locate entries that requested a request source notification*/
  for ( pos_index = (int32_t)handle_ptr->number_valid_callback - (int32_t)1; pos_index >= (int32_t)0; pos_index--)
  {
    (*(callback_node + pos_index))();
  }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*CODE_BLOCK_END*/