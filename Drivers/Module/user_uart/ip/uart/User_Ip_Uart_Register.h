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
#ifndef __USER_IP_UART_REGISTER__
#define __USER_IP_UART_REGISTER__

#include "../../cfg/User_Uart_Cfg.h"

typedef enum
{
  USER_IP_UART0_RXTX_IRQn            = 0x00u,            /**< UART0 Transmit / Receive Interrupt */
  USER_IP_UART1_RXTX_IRQn            = 0x00u,            /**< UART1 Transmit / Receive Interrupt */
  USER_IP_UART2_RXTX_IRQn            = 0x00u,            /**< UART2 Transmit / Receive Interrupt */
  USER_IP_UART3_RXTX_IRQn            = 0x00u,            /**< UART3 Transmit / Receive Interrupt */
  USER_IP_UART4_RXTX_IRQn            = 52u,              /**< UART4 Transmit / Receive Interrupt */
  USER_IP_UART5_RXTX_IRQn            = 53u,              /**< UART5 Transmit / Receive Interrupt */
  USER_IP_UART6_RXTX_IRQn            = 0x00u,            /**< UART6 Transmit / Receive Interrupt */
  USER_IP_UART7_RXTX_IRQn            = 82u,              /**< UART7 Transmit / Receive Interrupt */
  USER_IP_UART8_RXTX_IRQn            = 83u,              /**< UART8 Transmit / Receive Interrupt */
} User_UartIRQn_Type;

typedef struct {
	uint32_t SR; 			/* Status Register , Offset Address: 0x00 */
	uint32_t DR;			/* Data Register , Offset Address: 0x04 */
	uint32_t BRR;			/* Baud Rate Register , Offset Address: 0x08 */
	uint32_t CR1;			/* Control Register 1 , Offset Address: 0x0C */
	uint32_t CR2;			/* Control Register 2 , Offset Address: 0x10 */
	uint32_t CR3;			/* Control Register 3 , Offset Address: 0x14 */
	uint32_t GTPR;			/* Guard Time and prescaler Register , Offset Address: 0x18 */
} User_Ip_Uart_Type, *User_Ip_Uart_MemMapPtr;

#define USER_IP_UART_INSTANCE_COUNT       (9U)

#define USER_IP_UART_UART0_BASEADDR     (0x00U)
#define USER_UART0                        ((User_Ip_Uart_Type*)USER_IP_UART_UART0_BASEADDR)

#define USER_IP_UART_UART1_BASEADDR     (0x00U)
#define USER_UART1                        ((User_Ip_Uart_Type*)USER_IP_UART_UART1_BASEADDR)

#define USER_IP_UART_UART2_BASEADDR     (0x00U)
#define USER_UART2                        ((User_Ip_Uart_Type*)USER_IP_UART_UART2_BASEADDR)

#define USER_IP_UART_UART3_BASEADDR     (0x00U)
#define USER_UART3                        ((User_Ip_Uart_Type*)USER_IP_UART_UART3_BASEADDR)

#define USER_IP_UART_UART4_BASEADDR     (0x40004C00U)
#define USER_UART4                        ((User_Ip_Uart_Type*)USER_IP_UART_UART4_BASEADDR)

#define USER_IP_UART_UART5_BASEADDR     (0x40005000U)
#define USER_UART5                        ((User_Ip_Uart_Type*)USER_IP_UART_UART5_BASEADDR)

#define USER_IP_UART_UART6_BASEADDR     (0x00U)
#define USER_UART6                        ((User_Ip_Uart_Type*)USER_IP_UART_UART6_BASEADDR)

#define USER_IP_UART_UART7_BASEADDR     (0x40007800U)
#define USER_UART7                        ((User_Ip_Uart_Type*)USER_IP_UART_UART7_BASEADDR)

#define USER_IP_UART_UART8_BASEADDR     (0x40007C00U)
#define USER_UART8                        ((User_Ip_Uart_Type*)USER_IP_UART_UART8_BASEADDR)

#define USER_IP_UART_BASE_PTRS           {USER_UART0,USER_UART1,USER_UART2,USER_UART3,USER_UART4,USER_UART5,USER_UART6,USER_UART7,USER_UART8}
#define USER_IP_UART_RXTX_IRQS            {\
												USER_IP_UART0_RXTX_IRQn,\
												USER_IP_UART1_RXTX_IRQn,\
												USER_IP_UART2_RXTX_IRQn,\
												USER_IP_UART3_RXTX_IRQn,\
												USER_IP_UART4_RXTX_IRQn,\
												USER_IP_UART5_RXTX_IRQn,\
												USER_IP_UART6_RXTX_IRQn,\
												USER_IP_UART7_RXTX_IRQn,\
												USER_IP_UART8_RXTX_IRQn\
											}

#define USER_IP_UART_RESET_VALUE                            0x00000000u

/* ========================= STATUS REGISTER ========================= */

#define USER_IP_UART_SR_PE_MASK								0x00000001u  // Parity Error
#define USER_IP_UART_SR_PE_SHIFT								0u
#define USER_IP_UART_SR_PE_WIDTH								1u

#define USER_IP_UART_SR_FE_MASK								0x00000002u  // Framing Error
#define USER_IP_UART_SR_FE_SHIFT								1u
#define USER_IP_UART_SR_FE_WIDTH								1u

#define USER_IP_UART_SR_NF_MASK								0x00000004u  // Noise Error
#define USER_IP_UART_SR_NF_SHIFT								2u
#define USER_IP_UART_SR_NF_WIDTH								1u

#define USER_IP_UART_SR_ORE_MASK								0x00000008u  // Overrun Error
#define USER_IP_UART_SR_ORE_SHIFT								3u
#define USER_IP_UART_SR_ORE_WIDTH								1u


#define USER_IP_UART_SR_IDLE_MASK								0x00000010u  // Idle Line Detection
#define USER_IP_UART_SR_IDLE_SHIFT								4u
#define USER_IP_UART_SR_IDLE_WIDTH								1u

#define USER_IP_UART_SR_RXNE_MASK								0x00000020u  // Read data Register not empty
#define USER_IP_UART_SR_RXNE_SHIFT								5u
#define USER_IP_UART_SR_RXNE_WIDTH								1u

#define USER_IP_UART_SR_TC_MASK									0x00000040u  // Transmission Complete
#define USER_IP_UART_SR_TC_SHIFT								6u
#define USER_IP_UART_SR_TC_WIDTH								1u

#define USER_IP_UART_SR_TXE_MASK								0x00000080u  // Transmit data register empty
#define USER_IP_UART_SR_TXE_SHIFT								7u
#define USER_IP_UART_SR_TXE_WIDTH								1u

#define USER_IP_UART_SR_LBD_MASK								0x00000100u  // LIN break detection
#define USER_IP_UART_SR_LBD_SHIFT								8u
#define USER_IP_UART_SR_LBD_WIDTH								1u

#define USER_IP_UART_SR_CTS_MASK								0x00000200u  // CTS Flag
#define USER_IP_UART_SR_CTS_SHIFT								9u
#define USER_IP_UART_SR_CTS_WIDTH								1u

/* ========================= DATA REGISTER ========================= */

#define USER_IP_UART_DR_DR_MASK								0x000000FFU
#define USER_IP_UART_DR_DR_SHIFT								0u
#define USER_IP_UART_DR_DR_WIDTH								8u


/* ========================= BAUD RATE REGISTER ========================= */

#define USER_IP_UART_BRR_DIV_FRACTION_MASK						0x0000000Fu
#define USER_IP_UART_BRR_DIV_FRACTION_SHIFT					0u
#define USER_IP_UART_BRR_DIV_FRACTION_WIDTH					4u

#define USER_IP_UART_BRR_DIV_MANTISSA_MASK						0x0000FFF0u
#define USER_IP_UART_BRR_DIV_MANTISSA_SHIFT					4u
#define USER_IP_UART_BRR_DIV_MANTISSA_WIDTH					12u

/* ========================= STATUS REGISTER 1 ========================= */
#define USER_IP_UART_CR1_UE_MASK								0x00002000u
#define USER_IP_UART_CR1_UE_SHIFT								13u
#define USER_IP_UART_CR1_UE_WIDTH								1u

#define USER_IP_UART_CR1_SBK_MASK								0x00000001u
#define USER_IP_UART_CR1_SBK_SHIFT								0u
#define USER_IP_UART_CR1_SBK_WIDTH								1u

#define USER_IP_UART_CR1_RWU_MASK								0x00000002u
#define USER_IP_UART_CR1_RWU_SHIFT								1u
#define USER_IP_UART_CR1_RWU_WIDTH								1u

#define USER_IP_UART_CR1_RE_MASK								0x00000004u
#define USER_IP_UART_CR1_RE_SHIFT								2u
#define USER_IP_UART_CR1_RE_WIDTH								1u

#define USER_IP_UART_CR1_TE_MASK								0x00000008u
#define USER_IP_UART_CR1_TE_SHIFT								3u
#define USER_IP_UART_CR1_TE_WIDTH								1u

#define USER_IP_UART_CR1_IDLEIE_MASK							0x00000010u
#define USER_IP_UART_CR1_IDLEIE_SHIFT							4u
#define USER_IP_UART_CR1_IDLEIE_WIDTH							1u

#define USER_IP_UART_CR1_RXNEIE_MASK							0x00000020u
#define USER_IP_UART_CR1_RXNEIE_SHIFT							5u
#define USER_IP_UART_CR1_RXNEIE_WIDTH							1u

#define USER_IP_UART_CR1_TCIE_MASK								0x00000040u
#define USER_IP_UART_CR1_TCIE_SHIFT								6u
#define USER_IP_UART_CR1_TCIE_WIDTH								1u

#define USER_IP_UART_CR1_TXEIE_MASK								0x00000080u
#define USER_IP_UART_CR1_TXEIE_SHIFT							7u
#define USER_IP_UART_CR1_TXEIE_WIDTH							1u

#define USER_IP_UART_CR1_PEIE_MASK								0x00000100u
#define USER_IP_UART_CR1_PEIE_SHIFT								8u
#define USER_IP_UART_CR1_PEIE_WIDTH								1u


#define USER_IP_UART_CR1_PS_MASK								0x00000200u
#define USER_IP_UART_CR1_PS_SHIFT								9u
#define USER_IP_UART_CR1_PS_WIDTH								1u

#define USER_IP_UART_CR1_PCE_MASK								0x00000400u
#define USER_IP_UART_CR1_PCE_SHIFT								10u
#define USER_IP_UART_CR1_PCE_WIDTH								1u

#define USER_IP_UART_CR1_WAKE_MASK								0x00000800u
#define USER_IP_UART_CR1_WAKE_SHIFT								11u
#define USER_IP_UART_CR1_WAKE_WIDTH								1u

#define USER_IP_UART_CR1_M_MASK									0x00001000u
#define USER_IP_UART_CR1_M_SHIFT								12u
#define USER_IP_UART_CR1_M_WIDTH								1u


#define USER_IP_UART_CR1_OVER8_MASK								0x00001000u
#define USER_IP_UART_CR1_OVER8_SHIFT							15u
#define USER_IP_UART_CR1_OVER8_WIDTH							1u

/* ========================= STATUS REGISTER 2 ========================= */

#define USER_IP_UART_CR2_ADD_MASK								0x00000000Fu
#define USER_IP_UART_CR2_ADD_SHIFT								0u
#define USER_IP_UART_CR2_ADD_WIDTH								4u

#define USER_IP_UART_CR2_LBDL_MASK								0x000000020u
#define USER_IP_UART_CR2_LBDL_SHIFT								5u
#define USER_IP_UART_CR2_LBDL_WIDTH								1u

#define USER_IP_UART_CR2_LBDIE_MASK								0x000000040u
#define USER_IP_UART_CR2_LBDIE_SHIFT							6u
#define USER_IP_UART_CR2_LBDIE_WIDTH							1u

#define USER_IP_UART_CR2_LBCL_MASK								0x000000100u
#define USER_IP_UART_CR2_LBCL_SHIFT								8u
#define USER_IP_UART_CR2_LBCL_WIDTH								1u

#define USER_IP_UART_CR2_CPHA_MASK								0x000000200u
#define USER_IP_UART_CR2_CPHA_SHIFT								9u
#define USER_IP_UART_CR2_CPHA_WIDTH								1u

#define USER_IP_UART_CR2_CPOL_MASK								0x000000400u
#define USER_IP_UART_CR2_CPOL_SHIFT								10u
#define USER_IP_UART_CR2_CPOL_WIDTH								1u

#define USER_IP_UART_CR2_CLKEN_MASK								0x000000800u
#define USER_IP_UART_CR2_CLKEN_SHIFT							11u
#define USER_IP_UART_CR2_CLKEN_WIDTH							1u

#define USER_IP_UART_CR2_STOP_MASK								0x000003000u
#define USER_IP_UART_CR2_STOP_SHIFT								12u
#define USER_IP_UART_CR2_STOP_WIDTH								2u

#define USER_IP_UART_CR2_LINEN_MASK								0x000004000u
#define USER_IP_UART_CR2_LINEN_SHIFT							14u
#define USER_IP_UART_CR2_LINEN_WIDTH							1u
/* ========================= STATUS REGISTER 3 ========================= */

#define USER_IP_UART_CR3_EIE_MASK								0x000000001u
#define USER_IP_UART_CR3_EIE_SHIFT								0u
#define USER_IP_UART_CR3_EIE_WIDTH								1u

#define USER_IP_UART_CR3_IREN_MASK								0x000000002u
#define USER_IP_UART_CR3_IREN_SHIFT								1u
#define USER_IP_UART_CR3_IREN_WIDTH								1u

#define USER_IP_UART_CR3_IRLP_MASK								0x000000004u
#define USER_IP_UART_CR3_IRLP_SHIFT								2u
#define USER_IP_UART_CR3_IRLP_WIDTH								1u

#define USER_IP_UART_CR3_HDSEL_MASK								0x000000008u
#define USER_IP_UART_CR3_HDSEL_SHIFT							3u
#define USER_IP_UART_CR3_HDSEL_WIDTH							1u

#define USER_IP_UART_CR3_NACK_MASK								0x000000010u
#define USER_IP_UART_CR3_NACK_SHIFT								4u
#define USER_IP_UART_CR3_NACK_WIDTH								1u

#define USER_IP_UART_CR3_SCEN_MASK								0x000000020u
#define USER_IP_UART_CR3_SCEN_SHIFT								5u
#define USER_IP_UART_CR3_SCEN_WIDTH								1u

#define USER_IP_UART_CR3_DMAR_MASK								0x000000040u
#define USER_IP_UART_CR3_DMAR_SHIFT								6u
#define USER_IP_UART_CR3_DMAR_WIDTH								1u

#define USER_IP_UART_CR3_DMAT_MASK								0x000000080u
#define USER_IP_UART_CR3_DMAT_SHIFT								7u
#define USER_IP_UART_CR3_DMAT_WIDTH								1u

#define USER_IP_UART_CR3_RTSE_MASK								0x000000100u
#define USER_IP_UART_CR3_RTSE_SHIFT								8u
#define USER_IP_UART_CR3_RTSE_WIDTH								1u

#define USER_IP_UART_CR3_CTSE_MASK								0x000000200u
#define USER_IP_UART_CR3_CTSE_SHIFT								9u
#define USER_IP_UART_CR3_CTSE_WIDTH								1u

#define USER_IP_UART_CR3_CTSIE_MASK								0x000000400u
#define USER_IP_UART_CR3_CTSIE_SHIFT							10u
#define USER_IP_UART_CR3_CTSIE_WIDTH							1u

#define USER_IP_UART_CR3_ONEBIT_MASK							0x000000800u
#define USER_IP_UART_CR3_ONEBIT_SHIFT							11u
#define USER_IP_UART_CR3_ONEBIT_WIDTH							1u
/* ========================= GUARDTIME AND PRESCALER REGISTER ========================= */

#define USER_IP_UART_GTPR_PSC_MASK								0x000000001u
#define USER_IP_UART_GTPR_PSC_SHIFT								0u
#define USER_IP_UART_GTPR_PSC_WIDTH								8u

#define USER_IP_UART_GTPR_GT_MASK								0x000000100u
#define USER_IP_UART_GTPR_GT_SHIFT								8u
#define USER_IP_UART_GTPR_GT_WIDTH								8u



#endif /* __USER_IP_UART_REGISTER__ */
