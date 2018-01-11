<?xml version="1.0" encoding="ASCII"?>
<ResourceModel:App xmi:version="2.0" xmlns:xmi="http://www.omg.org/XMI" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:ResourceModel="http://www.infineon.com/Davex/Resource.ecore" name="ADC_QUEUE" URI="http://resources/4.0.20/app/ADC_QUEUE/2" description="Provides configurations for queue request source of VADC" version="4.0.20" minDaveVersion="4.0.0" instanceLabel="ADC_QUEUE_2" appLabel="" containingProxySignal="true">
  <upwardMapList xsi:type="ResourceModel:RequiredApp" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@requiredApps.1"/>
  <properties provideInit="true" sharable="true"/>
  <virtualSignals name="queue_to_global" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_queueglobal" hwSignal="global_signal" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="queue_select" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_queue2channel" hwSignal="select" hwResource="//@hwResources.0" visible="true">
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.4"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.30"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.56"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.82"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.108"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.119"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.130"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.141"/>
  </virtualSignals>
  <virtualSignals name="group_selection" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_queue2config" hwSignal="req" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="trigger_input" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_greqtr0sel" hwSignal="greqtr0sel" hwResource="//@hwResources.0" visible="true">
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../PWM_SVM/v4_0_20/PWM_SVM_0.app#//@connections.39"/>
  </virtualSignals>
  <virtualSignals name="gating_input" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_greqgt0sel" hwSignal="greqgt0sel" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="event_queue_source" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_sourceevent" hwSignal="sev" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="class_select" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_classselect_out" hwSignal="class_sel" hwResource="//@hwResources.0" visible="true"/>
  <virtualSignals name="iclass_select" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_iclass2channel" hwSignal="ch_class_sel" hwResource="//@hwResources.1" visible="true">
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.7"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.33"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.59"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.85"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.111"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.122"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.133"/>
    <upwardMapList xsi:type="ResourceModel:Connections" href="../../ADC_MEASUREMENT_ADV/v4_0_14/ADC_MEASUREMENT_ADV_2.app#//@connections.144"/>
  </virtualSignals>
  <virtualSignals name="queue_iclass_select" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_classselect_in" hwSignal="queue_class_sel" hwResource="//@hwResources.1"/>
  <virtualSignals name="sr_interrupt_signal" URI="http://resources/4.0.20/app/ADC_QUEUE/2/vs_signal_src" hwSignal="signal_in" hwResource="//@hwResources.2" visible="true"/>
  <requiredApps URI="http://resources/4.0.20/app/ADC_QUEUE/2/appres_global_adc" requiredAppName="GLOBAL_ADC" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../GLOBAL_ADC/v4_0_18/GLOBAL_ADC_0.app#/"/>
  </requiredApps>
  <requiredApps URI="http://resources/4.0.20/app/ADC_QUEUE/2/appres_cpu" requiredAppName="CPU_CTRL_XMC4" requiringMode="SHARABLE">
    <downwardMapList xsi:type="ResourceModel:App" href="../../CPU_CTRL_XMC4/v4_0_14/CPU_CTRL_XMC4_0.app#/"/>
  </requiredApps>
  <hwResources name="Queue" URI="http://resources/4.0.20/app/ADC_QUEUE/2/hwres_vadc_queue" resourceGroupUri="peripheral/vadc/*/group/*/queue" mResGrpUri="peripheral/vadc/*/group/*/queue">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/VADC/VADC_0.dd#//@provided.61"/>
  </hwResources>
  <hwResources name="Queue Class" URI="http://resources/4.0.20/app/ADC_QUEUE/2/hwres_vadc_qgrpclass" resourceGroupUri="peripheral/vadc/*/group/*/class/*" mResGrpUri="peripheral/vadc/*/group/*/class/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/VADC/VADC_0.dd#//@provided.31"/>
  </hwResources>
  <hwResources name="Source Event Interrupt" URI="http://resources/4.0.20/app/ADC_QUEUE/2/hwres_nvicnode_srcevt" resourceGroupUri="peripheral/cpu/0/nvic/interrupt/*" mResGrpUri="peripheral/cpu/0/nvic/interrupt/*">
    <downwardMapList xsi:type="ResourceModel:ResourceGroup" href="../../../HW_RESOURCES/CPU/CPU_0.dd#//@provided.21"/>
  </hwResources>
  <connections URI="http://resources/4.0.20/app/ADC_QUEUE/2/http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_sourceevent/http://resources/4.0.20/app/ADC_QUEUE/2/vs_signal_src" systemDefined="true" sourceSignal="event_queue_source" targetSignal="sr_interrupt_signal" srcVirtualSignal="//@virtualSignals.5" targetVirtualSignal="//@virtualSignals.9"/>
  <connections URI="http://resources/4.0.20/app/ADC_QUEUE/2/http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_classselect_out/http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_classselect_in" systemDefined="true" sourceSignal="class_select" targetSignal="queue_iclass_select" srcVirtualSignal="//@virtualSignals.6" targetVirtualSignal="//@virtualSignals.8"/>
  <connections URI="http://resources/4.0.20/app/ADC_QUEUE/2/http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_to_global_signal/http://resources/4.0.20/app/ADC_QUEUE/2/vs_adc_queue_queueglobal" systemDefined="true" sourceSignal="global_signal" targetSignal="queue_to_global" targetVirtualSignal="//@virtualSignals.0" proxySrcVirtualSignalUri="http://resources/4.0.18/app/GLOBAL_ADC/0/vs_global_adc_global" containingProxySignal="true">
    <downwardMapList xsi:type="ResourceModel:VirtualSignal" href="../../GLOBAL_ADC/v4_0_18/GLOBAL_ADC_0.app#//@virtualSignals.0"/>
    <srcVirtualSignal href="../../GLOBAL_ADC/v4_0_18/GLOBAL_ADC_0.app#//@virtualSignals.0"/>
  </connections>
</ResourceModel:App>
