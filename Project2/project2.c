//#############################################################################
//
// FILE:    gpio_ex2_toggle.c
//
// TITLE:   Device GPIO Toggle
//
//! \addtogroup driver_example_list
//! <h1> Device GPIO Toggle </h1>
//!
//! Configures the device GPIO through the sysconfig file. The GPIO pin is 
//! toggled in the infinite loop.
//! In order to migrate the project within syscfg to any device, 
//! click the swtich button under the device view and select your   
//! corresponding device to migrate, saving the project will 
//! auto-migrate your project settings.
//!
//!  \note: This example project has support for migration across our C2000 
//!  device families. If you are wanting to build this project from launchpad
//!  or controlCARD, please specify in the .syscfg file the board you're using.
//!  At any time you can select another device to migrate this example.
//
//#############################################################################
//
//
// $Copyright:
// Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include "driverlib.h"
#include "device.h"
#include "board.h"

//
// Main
//
void main(void)
{
    //
    // Initializes system control, device clock, and peripherals
    //
    Device_init();

    //
    // Initializes PIE and clear PIE registers. Disables CPU interrupts.
    // and clear all CPU interrupt flags.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Board Initialization
    //
    Board_init();

    //
    // Enables CPU interrupts
    //
    Interrupt_enableGlobal();

    //
    // Loop.
    //
    // 변수 선언
       unsigned int num = 0;
       unsigned int currentButtonState = 0;
       unsigned int lastButtonState = 0;

       for(;;)
       {
           // 현재 버튼 상태를 읽어옴
           currentButtonState = GPIO_readPin(Button);

           // =========================================================
           // ## 변경된 부분: 버튼을 '뗄 때' 카운트 증가 (하강 에지 감지) ##
           // =========================================================
           // 이전에는 눌려있었고(1), 지금은 떼어졌다면(0)
           if (currentButtonState == 0 && lastButtonState == 1)
           {
               num++;

               // 버튼의 물리적 떨림(채터링)을 무시하기 위한 짧은 지연시간
               DEVICE_DELAY_US(50000);
           }

           // 다음 루프를 위해 현재 상태를 이전 상태 변수에 저장
           lastButtonState = currentButtonState;

           // =========================================================
           // ## 개선된 부분: 카운터 순환 로직 ##
           // =========================================================
           // num이 5를 넘으면(즉, 6이 되면) 다시 0으로 리셋
           if (num > 5)
           {
               num = 0;
           }

           // 카운터 값(num)에 따라 LED를 제어
           if (num == 1)
           {
               GPIO_writePin(Red, 1);
               GPIO_writePin(Blue, 0);
               GPIO_writePin(Green, 0);
           }
           else if (num == 2)
           {
               GPIO_writePin(Red, 0);
               GPIO_writePin(Blue, 1);
               GPIO_writePin(Green, 0);
           }
           else if (num == 3)
           {
               GPIO_writePin(Red, 0);
               GPIO_writePin(Blue, 0);
               GPIO_writePin(Green, 1);
           }
           else if (num == 4)
           {
               GPIO_writePin(Red, 1);
               GPIO_writePin(Blue, 1);
               GPIO_writePin(Green, 1);
           }
           else if (num == 5)
           {
               GPIO_writePin(Red, 0);
               GPIO_writePin(Blue, 0);
               GPIO_writePin(Green, 0);
           }
           else // num이 0일 때
           {
               // 모든 LED를 끈 상태로 유지
               GPIO_writePin(Red, 0);
               GPIO_writePin(Blue, 0);
               GPIO_writePin(Green, 0);
           }
       }
   }


//
// End of File
//

