/* ************************************************************************************************
 *
 * COPYRIGHT RESERVED, 2019 ITC_Group Trading and Production LLC. All rights reserved.
 * The reproduction, distribution and utilization of this document as well as the communication of
 * its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the
 * payment of damages.
 * All rights reserved in the event of the grant of a patent, utility model or design.
 *
 *********************************************************************************************** */
#ifndef __USER_IP_USART_REGISTER__
#define __USER_IP_USART_REGISTER__

#include "../../cfg/User_Uart_Cfg.h"

typedef enum
{
  USER_IP_USART0_RXTX_IRQn            = 0x00u,          /**< USART0 Transmit / Receive Interrupt */
  USER_IP_USART1_RXTX_IRQn            = 37u,            /**< USART1 Transmit / Receive Interrupt */
  USER_IP_USART2_RXTX_IRQn            = 38u,            /**< USART2 Transmit / Receive Interrupt */
  USER_IP_USART3_RXTX_IRQn            = 39u,            /**< USART3 Transmit / Receive Interrupt */
  USER_IP_USART4_RXTX_IRQn            = 0x00u,          /**< USART4 Transmit / Receive Interrupt */
  USER_IP_USART5_RXTX_IRQn            = 0x00u,          /**< USART5 Transmit / Receive Interrupt */
  USER_IP_USART6_RXTX_IRQn            = 71u,            /**< USART6 Transmit / Receive Interrupt */
} User_UsartIRQn_Type;

typedef struct {
  uint32_t SR;                                
  uint32_t DR;                            
  uint32_t BRR;                            
  uint32_t CR1;                           
  uint32_t CR2;							
  uint32_t CR3;				        
  uint32_t GPTR;                                                    
} User_Ip_Usart_Type, *User_Ip_Usart_MemMapPtr;

#define USER_IP_USART_INSTANCE_COUNT       (7U)

#define USER_IP_USART_USART0_BASEADDR     (0x00U)
#define USER_USART0                        ((User_Ip_Usart_Type*)USER_IP_USART_USART0_BASEADDR)

#define USER_IP_USART_USART1_BASEADDR     (0x40011000U)
#define USER_USART1                        ((User_Ip_Usart_Type*)USER_IP_USART_USART1_BASEADDR)

#define USER_IP_USART_USART2_BASEADDR     (0x40004400U)
#define USER_USART2                        ((User_Ip_Usart_Type*)USER_IP_USART_USART2_BASEADDR)

#define USER_IP_USART_USART3_BASEADDR     (0x40004800U)
#define USER_USART3                        ((User_Ip_Usart_Type*)USER_IP_USART_USART3_BASEADDR)

#define USER_IP_USART_USART4_BASEADDR     (0x00U)
#define USER_USART4                        ((User_Ip_Usart_Type*)USER_IP_USART_USART4_BASEADDR)

#define USER_IP_USART_USART5_BASEADDR     (0x00U)
#define USER_USART5                        ((User_Ip_Usart_Type*)USER_IP_USART_USART5_BASEADDR)

#define USER_IP_USART_USART6_BASEADDR     (0x40011400U)
#define USER_USART6                        ((User_Ip_Usart_Type*)USER_IP_USART_USART6_BASEADDR)

#define USER_IP_USART_BASE_PTRS           {USER_USART0,USER_USART1,USER_USART2,USER_USART3,USER_USART4,USER_USART5,USER_USART6}
#define USER_IP_USART_RXTX_IRQS            {\
												USER_IP_USART0_RXTX_IRQn,\
												USER_IP_USART1_RXTX_IRQn,\
												USER_IP_USART2_RXTX_IRQn,\
												USER_IP_USART3_RXTX_IRQn,\
												USER_IP_USART4_RXTX_IRQn,\
												USER_IP_USART5_RXTX_IRQn,\
												USER_IP_USART6_RXTX_IRQn\
											}

#define USER_IP_USART_RESET_VALUE                           0x00000000u

#define USER_IP_USART_SR_PE_MASK                            0x00000001u
#define USER_IP_USART_SR_PE_SHIFT                           0u
#define USER_IP_USART_SR_PE_WIDTH                           1u
#define USER_IP_USART_SR_FE_MASK                            0x00000002u
#define USER_IP_USART_SR_FE_SHIFT                           1u
#define USER_IP_USART_SR_FE_WIDTH                           1u
#define USER_IP_USART_SR_NF_MASK                            0x00000004u
#define USER_IP_USART_SR_NF_SHIFT                           2u
#define USER_IP_USART_SR_NF_WIDTH                           1u
#define USER_IP_USART_SR_ORE_MASK                           0x00000008u
#define USER_IP_USART_SR_ORE_SHIFT                          3u
#define USER_IP_USART_SR_ORE_WIDTH                          1u
#define USER_IP_USART_SR_IDLE_MASK                          0x00000010u
#define USER_IP_USART_SR_IDLE_SHIFT                         4u
#define USER_IP_USART_SR_IDLE_WIDTH                         1u
#define USER_IP_USART_SR_RXNE_MASK                          0x00000020u
#define USER_IP_USART_SR_RXNE_SHIFT                         5u
#define USER_IP_USART_SR_RXNE_WIDTH                         1u
#define USER_IP_USART_SR_TC_MASK                            0x00000040u
#define USER_IP_USART_SR_TC_SHIFT                           6u
#define USER_IP_USART_SR_TC_WIDTH                           1u
#define USER_IP_USART_SR_TXE_MASK                           0x00000080u
#define USER_IP_USART_SR_TXE_SHIFT                          7u
#define USER_IP_USART_SR_TXE_WIDTH                          1u
#define USER_IP_USART_SR_LBD_MASK                           0x00000100u
#define USER_IP_USART_SR_LBD_SHIFT                          8u
#define USER_IP_USART_SR_LBD_WIDTH                          1u
#define USER_IP_USART_SR_CTS_MASK                           0x00000200u
#define USER_IP_USART_SR_CTS_SHIFT                          9u
#define USER_IP_USART_SR_CTS_WIDTH                          1u

#define USER_IP_USART_DR_DATA_MASK                          0x000000FFu
#define USER_IP_USART_DR_DATA_SHIFT                         0u
#define USER_IP_USART_DR_DATA_WIDTH                         8u

#define USER_IP_USART_BRR_FRAC_MASK                         0x0000000Fu
#define USER_IP_USART_BRR_FRAC_SHIFT                        0u
#define USER_IP_USART_BRR_FRAC_WIDTH                        4u
#define USER_IP_USART_BRR_MANT_MASK                         0x0000FFF0u
#define USER_IP_USART_BRR_MANT_SHIFT                        4u
#define USER_IP_USART_BRR_MANT_WIDTH                        12u

#define USER_IP_USART_CR1_SBK_MASK                            0x00000001u
#define USER_IP_USART_CR1_SBK_SHIFT                           0u
#define USER_IP_USART_CR1_SBK_WIDTH                           1u
#define USER_IP_USART_CR1_RWU_MASK                          0x00000002u
#define USER_IP_USART_CR1_RWU_SHIFT                         1u
#define USER_IP_USART_CR1_RWU_WIDTH                         1u
#define USER_IP_USART_CR1_RE_MASK                            0x00000004u
#define USER_IP_USART_CR1_RE_SHIFT                           2u
#define USER_IP_USART_CR1_RE_WIDTH                           1u
#define USER_IP_USART_CR1_TE_MASK                            0x00000008u
#define USER_IP_USART_CR1_TE_SHIFT                           3u
#define USER_IP_USART_CR1_TE_WIDTH                           1u
#define USER_IP_USART_CR1_IDLEIE_MASK                        0x00000010u
#define USER_IP_USART_CR1_IDLEIE_SHIFT                       4u
#define USER_IP_USART_CR1_IDLEIE_WIDTH                       1u
#define USER_IP_USART_CR1_RXNEIE_MASK                        0x00000020u
#define USER_IP_USART_CR1_RXNEIE_SHIFT                       5u
#define USER_IP_USART_CR1_RXNEIE_WIDTH                       1u
#define USER_IP_USART_CR1_TCIE_MASK                          0x00000040u
#define USER_IP_USART_CR1_TCIE_SHIFT                         6u
#define USER_IP_USART_CR1_TCIE_WIDTH                         1u
#define USER_IP_USART_CR1_TXEIE_MASK                         0x00000080u
#define USER_IP_USART_CR1_TXEIE_SHIFT                        7u
#define USER_IP_USART_CR1_TXEIE_WIDTH                        1u
#define USER_IP_USART_CR1_PEIE_MASK                          0x00000100u
#define USER_IP_USART_CR1_PEIE_SHIFT                         8u
#define USER_IP_USART_CR1_PEIE_WIDTH                         1u
#define USER_IP_USART_CR1_PS_MASK                            0x00000200u
#define USER_IP_USART_CR1_PS_SHIFT                           9u
#define USER_IP_USART_CR1_PS_WIDTH                           1u
#define USER_IP_USART_CR1_PCE_MASK                           0x00000400u
#define USER_IP_USART_CR1_PCE_SHIFT                          10u
#define USER_IP_USART_CR1_PCE_WIDTH                          1u
#define USER_IP_USART_CR1_WAKE_MASK                          0x00000800u
#define USER_IP_USART_CR1_WAKE_SHIFT                         11u
#define USER_IP_USART_CR1_WAKE_WIDTH                         1u
#define USER_IP_USART_CR1_M_MASK                            0x00001000u
#define USER_IP_USART_CR1_M_SHIFT                           12u
#define USER_IP_USART_CR1_M_WIDTH                           1u
#define USER_IP_USART_CR1_UE_MASK                           0x00002000u
#define USER_IP_USART_CR1_UE_SHIFT                          13u
#define USER_IP_USART_CR1_UE_WIDTH                          1u
#define USER_IP_USART_CR1_RES_MASK                          0x00004000u
#define USER_IP_USART_CR1_RES_SHIFT                         14u
#define USER_IP_USART_CR1_RES_WIDTH                         1u
#define USER_IP_USART_CR1_OVER8_MASK                        0x00008000u
#define USER_IP_USART_CR1_OVER8_SHIFT                       15u
#define USER_IP_USART_CR1_OVER8_WIDTH                       1u


#define USER_IP_USART_CR2_ADD_MASK                          0x0000000Fu
#define USER_IP_USART_CR2_ADD_SHIFT                         0u
#define USER_IP_USART_CR2_ADD_WIDTH                         4u
#define USER_IP_USART_CR2_RES_MASK                          0x00000010u
#define USER_IP_USART_CR2_RES_SHIFT                         4u
#define USER_IP_USART_CR2_RES_WIDTH                         1u
#define USER_IP_USART_CR2_LBDL_MASK                         0x00000020u
#define USER_IP_USART_CR2_LBDL_SHIFT                        5u
#define USER_IP_USART_CR2_LBDL_WIDTH                        1u
#define USER_IP_USART_CR2_LBDIE_MASK                        0x00000040u
#define USER_IP_USART_CR2_LBDIE_SHIFT                       6u
#define USER_IP_USART_CR2_LBDIE_WIDTH                       1u
#define USER_IP_USART_CR2_LBCL_MASK                         0x00000100u
#define USER_IP_USART_CR2_LBCL_SHIFT                        8u
#define USER_IP_USART_CR2_LBCL_WIDTH                        1u
#define USER_IP_USART_CR2_CPHA_MASK                         0x00000200u
#define USER_IP_USART_CR2_CPHA_SHIFT                        9u
#define USER_IP_USART_CR2_CPHA_WIDTH                        1u
#define USER_IP_USART_CR2_CPOL_MASK                         0x00000400u
#define USER_IP_USART_CR2_CPOL_SHIFT                        10u
#define USER_IP_USART_CR2_CPOL_WIDTH                        1u
#define USER_IP_USART_CR2_CLKEN_MASK                        0x00000800u
#define USER_IP_USART_CR2_CLKEN_SHIFT                       11u
#define USER_IP_USART_CR2_CLKEN_WIDTH                       1u
#define USER_IP_USART_CR2_STOP_MASK                         0x00003000u
#define USER_IP_USART_CR2_STOP_SHIFT                        12u
#define USER_IP_USART_CR2_STOP_WIDTH                        2u
#define USER_IP_USART_CR2_LIN_MASK                          0x00004000u
#define USER_IP_USART_CR2_LIN_SHIFT                         14u
#define USER_IP_USART_CR2_LIN_WIDTH                         1u

#define USER_IP_USART_CR3_EIE_MASK                           0x00000001u
#define USER_IP_USART_CR3_EIE_SHIFT                          0u
#define USER_IP_USART_CR3_EIE_WIDTH                          1u
#define USER_IP_USART_CR3_IREN_MASK                          0x00000002u
#define USER_IP_USART_CR3_IREN_SHIFT                         1u
#define USER_IP_USART_CR3_IREN_WIDTH                         1u
#define USER_IP_USART_CR3_IRLP_MASK                          0x00000004u
#define USER_IP_USART_CR3_IRLP_SHIFT                         2u
#define USER_IP_USART_CR3_IRLP_WIDTH                         1u
#define USER_IP_USART_CR3_HDSEL_MASK                         0x00000008u
#define USER_IP_USART_CR3_HDSEL_SHIFT                        3u
#define USER_IP_USART_CR3_HDSEL_WIDTH                        1u
#define USER_IP_USART_CR3_NACK_MASK                          0x00000010u
#define USER_IP_USART_CR3_NACK_SHIFT                         4u
#define USER_IP_USART_CR3_NACK_WIDTH                         1u
#define USER_IP_USART_CR3_SCEN_MASK                          0x00000020u
#define USER_IP_USART_CR3_SCEN_SHIFT                         5u
#define USER_IP_USART_CR3_SCEN_WIDTH                         1u
#define USER_IP_USART_CR3_DMAR_MASK                          0x00000040u
#define USER_IP_USART_CR3_DMAR_SHIFT                         6u
#define USER_IP_USART_CR3_DMAR_WIDTH                         1u
#define USER_IP_USART_CR3_DMAT_MASK                          0x00000080u
#define USER_IP_USART_CR3_DMAT_SHIFT                         7u
#define USER_IP_USART_CR3_DMAT_WIDTH                         1u
#define USER_IP_USART_CR3_RTSE_MASK                          0x00000100u
#define USER_IP_USART_CR3_RTSE_SHIFT                         8u
#define USER_IP_USART_CR3_RTSE_WIDTH                         1u
#define USER_IP_USART_CR3_CTSE_MASK                          0x00000200u
#define USER_IP_USART_CR3_CTSE_SHIFT                         9u
#define USER_IP_USART_CR3_CTSE_WIDTH                         1u
#define USER_IP_USART_CR3_CTSIE_MASK                         0x00000400u
#define USER_IP_USART_CR3_CTSIE_SHIFT                        10u
#define USER_IP_USART_CR3_CTSIE_WIDTH                        1u
#define USER_IP_USART_CR3_ONEBIT_MASK                        0x00000800u
#define USER_IP_USART_CR3_ONEBIT_SHIFT                       11u
#define USER_IP_USART_CR3_ONEBIT_WIDTH                       1u

#define USER_IP_USART_GTPR_PSC_MASK                          0x000000FFu
#define USER_IP_USART_GTPR_PSC_SHIFT                         0u
#define USER_IP_USART_GTPR_PSC_WIDTH                         8u
#define USER_IP_USART_GTPR_GT_MASK                           0x0000FF00u
#define USER_IP_USART_GTPR_GT_SHIFT                          8u
#define USER_IP_USART_GTPR_GT_WIDTH                          8u

#endif /* __USER_IP_USART_REGISTER__ */
