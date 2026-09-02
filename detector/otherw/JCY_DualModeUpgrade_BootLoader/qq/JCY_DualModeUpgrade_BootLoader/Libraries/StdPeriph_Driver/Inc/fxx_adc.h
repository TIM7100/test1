/*
  ******************************************************************************
  * @file    fxx_adc.h
  * @version V1.0.0
  * @date    2020
  * @brief   Header file of ADC module.
  ******************************************************************************
*/
#ifndef __FXX_ADC_H__
#define __FXX_ADC_H__

#include "fxx_std.h"

/****************  Total definition for ADC  **************************/
#define ADC_CH_MASK                           (BIT4|BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_SR register  **************************/
#define ADC_SR_AWD                            (BIT5)
#define ADC_SR_OVERF                          (BIT4)
#define ADC_SR_EOG                            (BIT3)
#define ADC_SR_JEOC                           (BIT2)
#define ADC_SR_EOC                            (BIT1)
#define ADC_SR_ADRDY                          (BIT0)
#define IS_ADC_SR_FLAG(ADC_FLAG)              ((ADC_FLAG) <= BIT6)

/****************  Bit definition for ADC_IE register  **************************/
#define ADC_IE_AWDIE                          (BIT5)
#define ADC_IE_OVERFIE                        (BIT4)
#define ADC_IE_EOGIE                          (BIT3)
#define ADC_IE_JEOCIE                         (BIT2)
#define ADC_IE_EOCIE                          (BIT1)
#define IS_ADC_IT(ADC_IT)                     (((ADC_IT) >= BIT1) && ((ADC_IT) <= BIT6))

/****************  Bit definition for ADC_CR1 register  **************************/
#define ADC_CR1_AWDJCH_POS                     (27U)
#define ADC_CR1_AWDJCH_MASK                    (BIT31|BIT30|BIT29|BIT28|BIT27)
#define ADC_CR1_DISCNUM_POS                    (23U)
#define ADC_CR1_DISCNUM_MASK                   (BIT26|BIT25|BIT24|BIT23)
#define ADC_CR1_DISCEN                         BIT22
#define ADC_CR1_CONT                           BIT21
#define ADC_CR1_SWSTART                        BIT20
#define ADC_CR1_JSWSTART                       BIT19
#define ADC_CR1_EXTSEL_POS                     (16U)
#define ADC_CR1_EXTSEL_MASK                    (BIT18|BIT17|BIT16)
#define ADC_CR1_JEXTSEL_POS                    (13U)
#define ADC_CR1_JEXTSEL_MASK                   (BIT15|BIT14|BIT13)
#define ADC_CR1_DMA                            BIT12
#define ADC_CR1_AWDEN                          BIT11
#define ADC_CR1_JAWDEN                         BIT10
#define ADC_CR1_JEN                            BIT9
#define ADC_CR1_AWDSGL                         BIT8
#define ADC_CR1_AWDCH_POS                      (0U)
#define ADC_CR1_AWDCH_MASK                     (BIT4|BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_CR2 register  **************************/
#define ADC_CR2_FASTMOD                        BIT27
#define ADC_CR2_AFE_RSTN                       BIT26
#define ADC_CR2_JOVSE                          BIT25
#define ADC_CR2_JTOVS                          BIT24
#define ADC_CR2_OVSS_POS                      (20U)
#define ADC_CR2_OVSS_MASK                     (BIT23|BIT22|BIT21|BIT20)
#define ADC_CR2_OVSR_POS                      (17U)
#define ADC_CR2_OVSR_MASK                     (BIT19|BIT18|BIT17)
#define ADC_CR2_OVSE                           BIT16
#define ADC_CR2_BUF_STIME_POS                 (8U)
#define ADC_CR2_BUF_STIME_MASK                (BIT15|BIT14|BIT13|BIT12|BIT11|BIT10|BIT9|BIT8)
#define ADC_CR2_EN_BUF                         BIT7
#define ADC_CR2_DIV_POS                       (3U)
#define ADC_CR2_DIV_MASK                      (BIT6|BIT5|BIT4|BIT3)
#define ADC_CR2_ADC_STP                        BIT2
#define ADC_CR2_OVRMOD                         BIT1
#define ADC_CR2_ADC_EN                         BIT0

/****************  Bit definition for ADC_SMPR1 register  **************************/
#define ADC_SMPR_CH_MASK                      (BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_DIFF register  **************************/
#define ADC_DIFF_DIFF7_F                       BIT7
#define ADC_DIFF_DIFF6_E                       BIT6
#define ADC_DIFF_DIFF5_D                       BIT5
#define ADC_DIFF_DIFF4_C                       BIT4
#define ADC_DIFF_DIFF3_B                       BIT3
#define ADC_DIFF_DIFF2_A                       BIT2
#define ADC_DIFF_DIFF1_9                       BIT1
#define ADC_DIFF_DIFF0_8                       BIT0 

/****************  Bit definition for ADC_HTR register  ***********************/
#define ADC_HTR_DHT                           (BIT27|BIT26|BIT25|BIT24|BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16)
#define ADC_HTR_HT                            (BIT11|BIT10|BIT9|BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_LTR register  ***********************/
#define ADC_LTR_DLT                           (BIT27|BIT26|BIT25|BIT24|BIT23|BIT22|BIT21|BIT20|BIT19|BIT18|BIT17|BIT16)
#define ADC_LTR_LT                            (BIT11|BIT10|BIT9|BIT8|BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_SQR1 register  ***********************/
#define ADC_SQR1_L                            (BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_JSQR register  ***********************/
#define ADC_JSQR_JSQ                          (BIT4|BIT3|BIT2|BIT1|BIT0)

/****************  Bit definition for ADC_JDR register  ***********************/
#define ADC_JDR_JCH                           (BIT20|BIT19|BIT18|BIT17|BIT16)

/****************  Bit definition for ADC_DR register  ***********************/
#define ADC_DR_CH                             (BIT20|BIT19|BIT18|BIT17|BIT16)

/****************  Bit definition for ADC_SIGN register  ***********************/
#define ADC_SIGN_SIGN7_F                       BIT7
#define ADC_SIGN_SIGN6_E                       BIT6
#define ADC_SIGN_SIGN5_D                       BIT5
#define ADC_SIGN_SIGN4_C                       BIT4
#define ADC_SIGN_SIGN3_B                       BIT3
#define ADC_SIGN_SIGN2_A                       BIT2
#define ADC_SIGN_SIGN1_9                       BIT1
#define ADC_SIGN_SIGN0_8                       BIT0 

/****************  Bit definition for ADC_TSREF register  ***********************/
#define ADC_TSREF_HIZ_EN                       BIT27
#define ADC_TSREF_VREFBI_SEL_POS              (25U)
#define ADC_TSREF_VREFBI_SEL_MASK             (BIT26|BIT25)
#define ADC_TSREF_VREFBI_EN                    BIT24
#define ADC_TSREF_VTRIM_POS                   (19U)
#define ADC_TSREF_VTRIM_MASK                  (BIT23|BIT22|BIT21|BIT20|BIT19)
#define ADC_TSREF_TTRIM_POS                   (15U)
#define ADC_TSREF_TTRIM_MASK                  (BIT18|BIT17|BIT16|BIT15)
#define ADC_TSREF_ALG_MEAN_POS                (9U)
#define ADC_TSREF_ALG_MEAN_MASK               (BIT10|BIT9)
#define ADC_TSREF_ADJ_TD_OS_POS               (5U)
#define ADC_TSREF_ADJ_TD_OS_MASK              (BIT8|BIT7|BIT6|BIT5)
#define ADC_TSREF_ADJ_TD_GA_POS               (1U)
#define ADC_TSREF_ADJ_TD_GA_MASK              (BIT4|BIT3|BIT2|BIT1)
#define ADC_TSREF_EN_TS                        BIT0

/****************  Bit definition for ADC_SMPR3 register  **************************/
#define ADC_SMPR3_CONV_PLUS_POS               (20U)
#define ADC_SMPR3_CONV_PLUS_MASK              (BIT21|BIT20)

/**
  * @brief  ADC Configuration Structure definition
  */
typedef struct
{
    uint32_t ADC_ClockDiv;              	 /*!< Specify the ADC clock div from the PCLK. 
												This parameter can be set to ADC_CLOCK_DIV1 | ADC_CLOCK_DIV2 |... ADC_CLOCK_DIV16 */

    FunctionalState ADC_ContinuousConvMode;  /*!< Specifies whether the conversion is performed in
												Continuous or Single mode.
												This parameter can be set to ENABLE or DISABLE. */

    uint32_t ADC_ExternalTrigConv;     		 /*!< Defines the external trigger used to start the analog
                                                to digital conversion of regular channels. This parameter
                                                can be a value of @ref ADC_external_trigger_sources_for_regular_channels_conversion */
                                                                                                        
	uint8_t ADC_NbrOfChannel;                /*!< Specifies the number of ADC channels that will be converted
										        using the sequencer for regular channel group.
                                                This parameter must range from 1 to 16. */
}ADC_InitTypeDef;

/******************************** ADC Over Sample Shift define *******************************/
#define ADC_CR2_OVSS_0                (0U)
#define ADC_CR2_OVSS_1                (1U)
#define ADC_CR2_OVSS_2                (2U)
#define ADC_CR2_OVSS_3                (3U)
#define ADC_CR2_OVSS_4                (4U)
#define ADC_CR2_OVSS_5                (5U)
#define ADC_CR2_OVSS_6                (6U)
#define ADC_CR2_OVSS_7                (7U)
#define ADC_CR2_OVSS_8                (8U)

/******************************** ADC Over Sample Rate define *******************************/
#define ADC_CR2_OVSR_2X               (0U)
#define ADC_CR2_OVSR_4X               (1U)
#define ADC_CR2_OVSR_8X               (2U)
#define ADC_CR2_OVSR_16X              (3U)
#define ADC_CR2_OVSR_32X              (4U)
#define ADC_CR2_OVSR_64X              (5U)
#define ADC_CR2_OVSR_128X             (6U)
#define ADC_CR2_OVSR_256X             (7U)

/******************************** ADC Sample period define *******************************/
#define ADC_SMP_CLOCK_3               (0U)
#define ADC_SMP_CLOCK_5               (1U)
#define ADC_SMP_CLOCK_7               (2U)
#define ADC_SMP_CLOCK_10              (3U)
#define ADC_SMP_CLOCK_13              (4U)
#define ADC_SMP_CLOCK_16              (5U)
#define ADC_SMP_CLOCK_20              (6U)
#define ADC_SMP_CLOCK_30              (7U)
#define ADC_SMP_CLOCK_60              (8U)
#define ADC_SMP_CLOCK_80              (9U)
#define ADC_SMP_CLOCK_100             (10U)
#define ADC_SMP_CLOCK_120             (11U)
#define ADC_SMP_CLOCK_160             (12U)
#define ADC_SMP_CLOCK_320             (13U)
#define ADC_SMP_CLOCK_480             (14U)
#define ADC_SMP_CLOCK_640             (15U)

/******************************** ADC ClockPrescale define *******************************/
#define  ADC_CLOCK_DIV1                              (0U)    
#define  ADC_CLOCK_DIV2                              (1U)    
#define  ADC_CLOCK_DIV3                              (2U)    
#define  ADC_CLOCK_DIV4                              (3U)    
#define  ADC_CLOCK_DIV5                              (4U)    
#define  ADC_CLOCK_DIV6                              (5U)    
#define  ADC_CLOCK_DIV7                              (6U)    
#define  ADC_CLOCK_DIV8                              (7U)    
#define  ADC_CLOCK_DIV9                              (8U)    
#define  ADC_CLOCK_DIV10                             (9U)    
#define  ADC_CLOCK_DIV11                             (10U)    
#define  ADC_CLOCK_DIV12                             (11U)    
#define  ADC_CLOCK_DIV13                             (12U)    
#define  ADC_CLOCK_DIV14                             (13U)    
#define  ADC_CLOCK_DIV15                             (14U)    
#define  ADC_CLOCK_DIV16                             (15U) 

/************************ADC_AnalogWDGConfTypeDef->WatchdogMode define********************/
#define  ADC_ANALOGWATCHDOG_NONE                    (0U)   //None channel
#define  ADC_ANALOGWATCHDOG_RCH_ALL                 (1U)   //All regular channels
#define  ADC_ANALOGWATCHDOG_JCH_ALL                 (2U)   //All inject channels
#define  ADC_ANALOGWATCHDOG_RCH_AND_JCH_ALL         (3U)   //All regular and inject channels
#define  ADC_ANALOGWATCHDOG_RCH_SINGLE              (4U)   //Single regular channel
#define  ADC_ANALOGWATCHDOG_JCH_SINGLE              (5U)   //Single Inject channel
#define  ADC_ANALOGWATCHDOG_RCH_OR_JCH_SINGLE       (6U)   //Regular or inject channel

#define IS_ADC_ANALOG_WATCHDOG(WATCHDOG) (((WATCHDOG) == ADC_ANALOGWATCHDOG_RCH_ALL) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_JCH_ALL) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_RCH_AND_JCH_ALL) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_RCH_SINGLE) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_JCH_SINGLE) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_RCH_OR_JCH_SINGLE) || \
                                          ((WATCHDOG) == ADC_ANALOGWATCHDOG_NONE))

/******************************** ADC channel number define *******************************/
#define  ADC_CHANNEL_0                              (0U)    
#define  ADC_CHANNEL_1                              (1U)    
#define  ADC_CHANNEL_2                              (2U)    
#define  ADC_CHANNEL_3                              (3U)    
#define  ADC_CHANNEL_4                              (4U)    
#define  ADC_CHANNEL_5                              (5U)    
#define  ADC_CHANNEL_6                              (6U)    
#define  ADC_CHANNEL_7                              (7U)    
#define  ADC_CHANNEL_8                              (8U)    
#define  ADC_CHANNEL_9                              (9U)    
#define  ADC_CHANNEL_10                             (10U)    
#define  ADC_CHANNEL_11                             (11U)    
#define  ADC_CHANNEL_12                             (12U)    
#define  ADC_CHANNEL_13                             (13U)    
#define  ADC_CHANNEL_14                             (14U)    
#define  ADC_CHANNEL_15                             (15U)    
#define  ADC_CHANNEL_TEMP                           (16U)    
#define  ADC_CHANNEL_VBAT                           (17U)    
#define  ADC_CHANNEL_VBGR                           (18U)    
#define  ADC_CHANNEL_EXT2                           (19U)    
#define  ADC_CHANNEL_EXT3                           (20U)    
#define IS_ADC_ALL_CHANNEL(CHANNEL)        (((CHANNEL) == ADC_CHANNEL_0) || \
                                            ((CHANNEL) == ADC_CHANNEL_1) || \
                                            ((CHANNEL) == ADC_CHANNEL_2) || \
                                            ((CHANNEL) == ADC_CHANNEL_3) || \
                                            ((CHANNEL) == ADC_CHANNEL_4) || \
                                            ((CHANNEL) == ADC_CHANNEL_5) || \
                                            ((CHANNEL) == ADC_CHANNEL_6) || \
                                            ((CHANNEL) == ADC_CHANNEL_7) || \
                                            ((CHANNEL) == ADC_CHANNEL_8) || \
                                            ((CHANNEL) == ADC_CHANNEL_9) || \
                                            ((CHANNEL) == ADC_CHANNEL_10) || \
                                            ((CHANNEL) == ADC_CHANNEL_11) || \
                                            ((CHANNEL) == ADC_CHANNEL_12) || \
                                            ((CHANNEL) == ADC_CHANNEL_13) || \
                                            ((CHANNEL) == ADC_CHANNEL_14) || \
                                            ((CHANNEL) == ADC_CHANNEL_15) || \
                                            ((CHANNEL) == ADC_CHANNEL_TEMP) || \
                                            ((CHANNEL) == ADC_CHANNEL_VBAT) || \
                                            ((CHANNEL) == ADC_CHANNEL_VBGR) || \
                                            ((CHANNEL) == ADC_CHANNEL_EXT2) || \
                                            ((CHANNEL) == ADC_CHANNEL_EXT3))

/******************************** ADC Trig source define*******************************
 *  |     Trig Source           |     ACM32FXXX/FPXXX    |      ACM32F0X0          | *
 *  |     ADC_SOFTWARE_START    |     SWSTART/JSWSTART   |      SWSTART/JSWSTART   | *
 *  |     ADC_EXTERNAL_TIG1     |     TIM1_TRGO          |      TIM1_TRGO          | *
 *  |     ADC_EXTERNAL_TIG2     |     TIM1_CC4           |      TIM1_CC4           | *
 *  |     ADC_EXTERNAL_TIG3     |     TIM2_TRGO          |      RSV                | *
 *  |     ADC_EXTERNAL_TIG4     |     TIM3_TRGO          |      TIM3_TRGO          | *
 *  |     ADC_EXTERNAL_TIG5     |     TIM4_TRGO          |      TIM15_TRGO         | *
 *  |     ADC_EXTERNAL_TIG6     |     TIM6_TRGO          |      TIM6_TRGO          | *
 *  |     ADC_EXTERNAL_TIG7     |     EXTi Line 11       |      EXTi Line 11       | */
#define  ADC_SOFTWARE_START                           (0U)
#define  ADC_EXTERNAL_TIG1                            (1U)
#define  ADC_EXTERNAL_TIG2                            (2U)
#define  ADC_EXTERNAL_TIG3                            (3U)
#define  ADC_EXTERNAL_TIG4                            (4U)
#define  ADC_EXTERNAL_TIG5                            (5U)
#define  ADC_EXTERNAL_TIG6                            (6U)
#define  ADC_EXTERNAL_TIG7                            (7U)
#define IS_ADC_ALL_TRIG(_TRIG)             (((_TRIG) == ADC_SOFTWARE_START) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG1) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG2) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG3) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG4) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG5) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG6) || \
                                            ((_TRIG) == ADC_EXTERNAL_TIG7))

/******************************** ADC Vref define *******************************/
#define ADC_VREF_1V5                    (0U)
#define ADC_VREF_2V0                    (1U)
#define ADC_VREF_2V5                    (2U)
#define IS_ADC_ALL_VREF(VREF)           (((VREF) == ADC_VREF_1V5) || \
                                         ((VREF) == ADC_VREF_2V0) || \
                                         ((VREF) == ADC_VREF_2V5))

/******************************** ADC Instances *******************************/
#define IS_ADC_ALL_PERIPH(ADCx)                 ((ADCx) == ADC)

/** @defgroup ADC_regular_length 
  * @{
  */

#define IS_ADC_REGULAR_LENGTH(LENGTH) (((LENGTH) >= 0x1) && ((LENGTH) <= 0x10))
/**
  * @}
  */
/** @defgroup ADC_regular_discontinuous_mode_number 
  * @{
  */

#define IS_ADC_REGULAR_DISC_NUMBER(NUMBER) (((NUMBER) >= 0x1) && ((NUMBER) <= 0x10))

/**
  * @}
  */
  
/** @defgroup ADC_thresholds 
  * @{
  */

#define IS_ADC_THRESHOLD(THRESHOLD)      ((THRESHOLD) < 0x1000)
#define IS_ADC_DIFF_THRESHOLD(THRESHOLD) (((THRESHOLD) < 2048) && ((THRESHOLD) > (-2048)))

/**
  * @}
  */
                                                  
#define IS_ADC_ALL_OVSS(_OVSS)             (((_OVSS) == ADC_CR2_OVSS_0) || \
                                            ((_OVSS) == ADC_CR2_OVSS_1) || \
                                            ((_OVSS) == ADC_CR2_OVSS_2) || \
                                            ((_OVSS) == ADC_CR2_OVSS_3) || \
                                            ((_OVSS) == ADC_CR2_OVSS_4) || \
                                            ((_OVSS) == ADC_CR2_OVSS_5) || \
                                            ((_OVSS) == ADC_CR2_OVSS_6) || \
                                            ((_OVSS) == ADC_CR2_OVSS_7) || \
                                            ((_OVSS) == ADC_CR2_OVSS_8))
                                            
#define IS_ADC_ALL_OVSR(_OVSR)             (((_OVSR) == ADC_CR2_OVSR_2X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_4X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_8X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_16X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_32X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_64X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_128X) || \
                                            ((_OVSR) == ADC_CR2_OVSR_256X))

#define IS_ADC_ALL_CLOCKDIV(CLOCKDIV)      (((CLOCKDIV) == ADC_CLOCK_DIV1) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV2) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV3) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV4) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV5) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV6) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV7) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV8) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV9) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV10) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV11) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV12) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV13) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV14) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV15) || \
                                            ((CLOCKDIV) == ADC_CLOCK_DIV16))

#define IS_ADC_REGULAR_RANK(RANK)          (((RANK) >= 1) && ((RANK) <= 16))


#define IS_ADC_ALL_SMPCLOCK(SMPCLOCK)      (((SMPCLOCK) == ADC_SMP_CLOCK_3) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_5) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_7) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_10) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_13) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_16) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_20) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_30) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_60) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_80) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_100) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_120) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_160) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_320) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_480) || \
                                            ((SMPCLOCK) == ADC_SMP_CLOCK_640))

/** @defgroup ADC_Exported_Functions
  * @{
  */
void ADC_DeInit(ADC_TypeDef* ADCx);
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct);
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState);
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx);
void ADC_ContinueModeStopCmd(ADC_TypeDef* ADCx);
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number);
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime, FunctionalState ADC_DiffMode);
uint32_t ADC_GetConversionValue(ADC_TypeDef* ADCx);
void ADC_InjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t ADC_SampleTime, FunctionalState ADC_DiffMode);
uint32_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx);
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog);
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold);
void ADC_AnalogWatchdogDiffThresholdsConfig(ADC_TypeDef* ADCx, int16_t HighThreshold, int16_t LowThreshold);
void ADC_AnalogWatchdogRegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);
void ADC_AnalogWatchdogInjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel);
void ADC_OverSampleConfig(ADC_TypeDef* ADCx, uint8_t Ratio, uint8_t RightBitShift);
void ADC_OverSampleRegularCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_OverSampleInjectedCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_OverSampleInjectedNTimesConfig(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_VrefCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_VrefConfig(ADC_TypeDef* ADCx, uint8_t Vref);
uint32_t ADC_GetStatus(ADC_TypeDef* ADCx);
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG);
FlagStatus ADC_GetITSource(ADC_TypeDef* ADCx, uint16_t ADC_IT);

#endif





