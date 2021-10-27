/*
 * App_Test.h
 *
 *  Created on: Oct 25, 2021
 *      Author: Admin
 */

#ifndef APP_APP_TEST_H_
#define APP_APP_TEST_H_

#include "main.h"
#include "radio.h"
#include "sx1276-board.h"
#include "user_timer.h"

typedef enum
{
    LOWPOWER,
    RX,
    RX_TIMEOUT,
    RX_ERROR,
    TX,
    TX_TIMEOUT,
}States_t;


extern void App_TestMain();

#endif /* APP_APP_TEST_H_ */
