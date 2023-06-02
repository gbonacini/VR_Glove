// -----------------------------------------------------------------
// vr_glove - a VR glove made with common components and recycled stuff
// Copyright (C) 2023  Gabriele Bonacini
//
// This program is distributed under dual license:
// - Creative Comons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0) License 
// for non commercial use, the license has the following terms:
// * Attribution — You must give appropriate credit, provide a link to the license, 
// and indicate if changes were made. You may do so in any reasonable manner, 
// but not in any way that suggests the licensor endorses you or your use.
// * NonCommercial — You must not use the material for commercial purposes.
// * NonAI - You must not to use the material to instruct AI / Machine learning systems.
// A copy of the license it's available to the following address:
// http://creativecommons.org/licenses/by-nc/4.0/
// For commercial use a specific license is available contacting the author.
// -----------------------------------------------------------------

#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <SPI.h>
#include "BluetoothSerial.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps612.h"

TFT_eSPI tft{TFT_eSPI()};  

namespace glove {

    auto stale = [](){ while(true) delay(1000); };

    class Glove{

        public:

            Glove(void)                       noexcept;
            void        handleEvents(void)    noexcept;
            void        sendMsg(void)         noexcept;

        private:
            
            const uint8_t THUMB_PIN          { GPIO_NUM_32 },
                          INDEX_PIN          { GPIO_NUM_39 }, 
                          MIDDLE_PIN         { GPIO_NUM_38 },
                          RING_PIN           { GPIO_NUM_37 },
                          LITTLE_PIN         { GPIO_NUM_36 };

            const uint8_t BUTTON_LEFT_PIN    { GPIO_NUM_33 },
                          BUTTON_MIDDLE_PIN  { GPIO_NUM_25 },
                          BUTTON_RIGHT_PIN   { GPIO_NUM_26 };

            const uint16_t NORM_MAX          { 90 };
            const uint16_t AVERAGE_ELEMS_NUM { 50 };
            const uint16_t AVERAGE_CALC_WAIT { 40 };

            const uint16_t SERIAL_SPEED      { 9600 };
            const uint8_t  TCAADDR           { 0x70 };
            const int      I2C_BUS_SPEED     { 100000 };

            const int      SDA_PIN           { 21 },
                           SCL_PIN           { 22 };

            const float    DEGREE_CONV_FCTR  { 180.0 / PI };

            const char* const SSID           { "GLOVE_ESP" };

            enum EULER_IDX : size_t          { PSI=0, THETA=1, PHI=2 };

            uint16_t indexMin                { 100 }, 
                     indexMax                { 4095 },
                     littleMin               { 100 },
                     littleMax               { 4095 },
                     ringMin                 { 100 },
                     ringMax                 { 4095 },
                     middleMin               { 100 },
                     middleMax               { 4095 },
                     thumbMin                { 1   },
                     thumbMax                { 4095 },
                     buttonLeftOn            { 4095 },
                     buttonRightOn           { 4095 },
                     buttonMiddleOn          { 4095 },
                     indexCurrent            { 0 },
                     littleCurrent           { 0 },
                     ringCurrent             { 0 },
                     middleCurrent           { 0 },
                     thumbCurrent            { 0 },
                     sensitivity             { 20 }, 
                     indexNorm               { 0 },
                     littleNorm              { 0 },
                     ringNorm                { 0 },
                     middleNorm              { 0 },
                     thumbNorm               { 0 };

            bool    buttonLeft               { false },
                    buttonMiddle             { false },
                    buttonRight              { false },
                    indexStatus              { false },
                    middleStatus             { false },
                    ringStatus               { false },
                    littleStatus             { false },
                    thumbStatus              { false };

            float   offsetX_ArmAngle         { 0.0 }, 
                    offsetY_ArmAngle         { 0.0 }, 
                    offsetZ_ArmAngle         { 0.0 },
                    offsetX_ForearmAngle     { 0.0 }, 
                    offsetY_ForearmAngle     { 0.0 }, 
                    offsetZ_ForearmAngle     { 0.0 },
                    offsetX_HandAngle        { 0.0 }, 
                    offsetY_HandAngle        { 0.0 }, 
                    offsetZ_HandAngle        { 0.0 };

            enum    EVENTS : uint8_t {
                THUMB         = 0,
                INDEX         = 1,
                MIDDLE        = 2,
                RING          = 3,
                LITTLE        = 4,
                BUTTON_LEFT   = 5,
                BUTTON_MIDDLE = 6,
                BUTTON_RIGHT  = 7
            };

            byte         xcolon              { 0 };
            bool         initial             { true };
            unsigned int colour              { 0 };

            BluetoothSerial bluetoothLink;
            char            statusForBt[9]   {};

            uint8_t      errCode             { 0U };
            MPU6050      mpu;

            enum ACCEL_ADDRS : uint8_t       { HAND=0x0, FOREARM=0x6, ARM=0x7 };

            struct Angles{
                ACCEL_ADDRS  device;
                bool         movement        {false};
        
                uint8_t      fifo_buffer[64] {} ; 
                Quaternion   quaternion      {};
                float        euler[3]        {};
            };

            Angles    angles[3];
            enum COMPONENT : size_t   { ARMIDX=0, FOREARMIDX=1, HANDIDX=2 };

            void           calibrateFingers(void)               noexcept;
            void           calibrateArticulations(void)         noexcept;
            void           setIndex(bool onOff)                 noexcept;
            void           setMiddle(bool onOff)                noexcept;
            void           setRing(bool onOff)                  noexcept;
            void           setLittle(bool onOff)                noexcept;
            void           setThumb(bool onOff)                 noexcept;

            void           setMin(void)                         noexcept;
            void           setIndexMin(void)                    noexcept;
            void           setMiddleMin(void)                   noexcept;
            void           setRingMin(void)                     noexcept;
            void           setLittleMin(void)                   noexcept;
            void           setThumbMin(void)                    noexcept;

            void           setMax(void)                         noexcept;
            void           setIndexMax(void)                    noexcept;
            void           setMiddleMax(void)                   noexcept;
            void           setRingMax(void)                     noexcept;
            void           setLittleMax(void)                   noexcept;
            void           setThumbMax(void)                    noexcept;

            bool           checkRightButton(void)         const noexcept;
            bool           checkLeftButton(void)          const noexcept;
            bool           checkMiddleButton(void)        const noexcept;

            void           waitConfirm(void)              const noexcept;

            void           refreshFingers(void)                 noexcept;
            void           readStatus(void)                     noexcept;
            void           printDebugStatus(void)         const noexcept;
            void           printDebugStatusNolimit(void)  const noexcept;
            uint16_t       normalizeStatus(uint16_t inf, 
                                           uint16_t sup, 
                                           uint16_t curr) const noexcept;   
            void           updateStatusForBt(void)              noexcept;
            void           selectAccel(uint8_t addr)            noexcept;
            void           printPortStats(bool block)           noexcept;
            void           calculateMeans(void)                 noexcept;
    };

    Glove::Glove(void) noexcept
    {
        pinMode(INDEX_PIN,         INPUT);
        pinMode(THUMB_PIN,         INPUT);
        pinMode(MIDDLE_PIN,        INPUT);
        pinMode(RING_PIN,          INPUT);
        pinMode(LITTLE_PIN,        INPUT);
        pinMode(BUTTON_LEFT_PIN,   INPUT_PULLDOWN);
        pinMode(BUTTON_MIDDLE_PIN, INPUT_PULLDOWN);
        pinMode(BUTTON_RIGHT_PIN,  INPUT_PULLDOWN);

        Serial.begin(SERIAL_SPEED);

        tft.init();
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour

        bluetoothLink.begin(SSID);

        angles[HANDIDX].device    = HAND;
        angles[FOREARMIDX].device = FOREARM;
        angles[ARMIDX].device     = ARM;

        Wire.begin(SDA_PIN, SCL_PIN ); 
        Wire.setClock(I2C_BUS_SPEED);

        for (const auto& angle: angles){

            selectAccel(angle.device);

            Serial.println("");
            mpu.initialize();
            errCode = mpu.dmpInitialize();
            switch(errCode){
                case 0U:
                  Serial.println("MPU6050 config: OK.");
                  if( ! mpu.testConnection()){
                    Serial.println("Error: MPU6050 connection.");
                    stale();
                  }
                  break;
                case 1U:
                  Serial.println("Error: Memory Init Load.");
                  stale();
                  break;
                case 2U:
                  Serial.println("Error: DMP update.");
                  stale();
                  break;
                default:
                  Serial.print("Error: unknown code :");
                  Serial.println(errCode);
                  stale();
            }

            mpu.setXGyroOffset(0);
            mpu.setYGyroOffset(0);
            mpu.setZGyroOffset(0);
            mpu.setXAccelOffset(0);
            mpu.setYAccelOffset(0);
            mpu.setZAccelOffset(0);

            mpu.CalibrateAccel(6);
            mpu.CalibrateGyro(6);
    
            mpu.setDMPEnabled(true);

            Serial.println("");
        }
    }

    void  Glove::setIndex(bool onOff)  noexcept {
        indexStatus = onOff;
    }
    void  Glove::setMiddle(bool onOff) noexcept {
        middleStatus = onOff;
    }
    void  Glove::setRing(bool onOff)   noexcept {
        ringStatus = onOff;
    }
    void  Glove::setLittle(bool onOff)   noexcept {
        littleStatus = onOff;
    }
    void  Glove::setThumb(bool onOff)  noexcept {
        thumbStatus = onOff;
    }

    void  Glove::setMin(void)  noexcept{
        setIndexMin();
        setMiddleMin();
        setRingMin();
        setLittleMin();
        setThumbMin();
    }

    void  Glove::setMax(void)  noexcept{
        setIndexMax();
        setMiddleMax();
        setRingMax();
        setLittleMax();
        setThumbMax();
    }

    void  Glove::setIndexMin(void)  noexcept {
        indexMin = indexCurrent;
    }

    void  Glove::setMiddleMin(void) noexcept {
            middleMin = middleCurrent;
    }

    void  Glove::setRingMin(void)   noexcept {
            ringMin = ringCurrent;
    }

    void  Glove::setLittleMin(void) noexcept {
            littleMin = littleCurrent;
    }

    void  Glove::setThumbMin(void)  noexcept {
            thumbMin = thumbCurrent;
    }

    void  Glove::setIndexMax(void)  noexcept {
            indexMax = indexCurrent;
    }

    void  Glove::setMiddleMax(void) noexcept {
            middleMax = middleCurrent;
    }

    void  Glove::setRingMax(void)   noexcept {
            ringMax = ringCurrent;
    }

    void  Glove::setLittleMax(void) noexcept {
            littleMax = littleCurrent;
    }

    void  Glove::setThumbMax(void)  noexcept {
            thumbMax = thumbCurrent;
    }

    bool  Glove::checkRightButton(void)  const noexcept{
        return   analogRead(BUTTON_RIGHT_PIN) > ( buttonRightOn - ( buttonRightOn * sensitivity / 100) )  ? true : false;
    }

    bool  Glove::checkLeftButton(void)  const noexcept{
        return   analogRead(BUTTON_LEFT_PIN) > ( buttonLeftOn - ( buttonLeftOn * sensitivity / 100 ))  ? true : false;
    }

    bool  Glove::checkMiddleButton(void)  const noexcept{
        return   analogRead(BUTTON_MIDDLE_PIN) > ( buttonMiddleOn - ( buttonMiddleOn * sensitivity / 100) )  ? true : false;
    }

    void  Glove::waitConfirm(void) const noexcept{
        delay(1000);
        while(true){
            if(checkLeftButton())
                break;
            delay(300);
        }
    }

    void Glove::printDebugStatus(void) const noexcept {
          static uint8_t  limit { 0 };
          if(limit == 50){
               Serial.println("--- Fingers ----");
               Serial.print("Index: ");
               Serial.println(indexCurrent);
               Serial.println(indexNorm);
               Serial.print("Middle:");
               Serial.println(middleCurrent);
               Serial.println(middleNorm);
               Serial.print("Ring:");
               Serial.println(ringCurrent);
               Serial.println(ringNorm);
               Serial.print("Little:");
               Serial.println(littleCurrent);
               Serial.println(littleNorm);
               Serial.print("Thumb:");
               Serial.println(thumbCurrent);
               Serial.println(thumbNorm);
               Serial.print("Inf: ");
               Serial.println(thumbMin);
               Serial.print("Sup: ");
               Serial.println(thumbMax);
               Serial.print("Norm: ");
               Serial.println(normalizeStatus(thumbMin, thumbMax, thumbCurrent));
               Serial.println("--- Quaternions ----");

               Serial.print("[");
               for (const auto& angle: angles){
                   Serial.print(angle.quaternion.w);Serial.print(", ");
                   Serial.print(angle.quaternion.x);Serial.print(", ");
                   Serial.print(angle.quaternion.y);Serial.print(", ");
                   Serial.print(angle.quaternion.z);
               }
               Serial.println("]");
               Serial.println("--- EULER ----");

               Serial.print("[");
               for (const auto& angle: angles){
                   Serial.print(angle.euler[PSI]);   Serial.print(", ");
                   Serial.print(angle.euler[THETA]); Serial.print(", ");
                   Serial.print(angle.euler[PHI]);
               }
               Serial.println("]");

               Serial.println("--- ANGLE OFFSETS ----");
               Serial.print("{");
               Serial.print(offsetX_ArmAngle);  Serial.print(", ");
               Serial.print(offsetY_ArmAngle);  Serial.print(", ");
               Serial.print(offsetZ_ArmAngle);  Serial.print(", ");
               Serial.print(offsetX_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetY_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetZ_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetX_HandAngle);  Serial.print(", ");
               Serial.print(offsetY_HandAngle);  Serial.print(", ");
               Serial.print(offsetZ_HandAngle);
               Serial.print("}");

               Serial.println("--- EULER DEGREES ----");
               Serial.print("<");
               for (const auto& angle: angles){
                   Serial.print(angle.euler[PSI]   * DEGREE_CONV_FCTR );  Serial.print(", ");
                   Serial.print(angle.euler[THETA] * DEGREE_CONV_FCTR );  Serial.print(", ");
                   Serial.print(angle.euler[PHI]   * DEGREE_CONV_FCTR );
               }
               Serial.println(">");
               Serial.println("-------");

               limit = 0;
            }

            limit++;
    }

    void  Glove::sendMsg(void) noexcept{
         // Format:
         //
         // <ax,ay,az,fx,fy,fz,hx,hy,hz,t,i,m,r,p>
         // a = arm
         // f = forearm
         // h = hand
         // t = thumb
         // i = index
         // m = medium
         // r = ring
         // p = pinky
         // Fingers
         // Note: Roll and Pitch in reality are inverted because the type of mounting on the device
         // but they won't be renamed to preserve the reverences to the directions printed on the PCB
         // of the MPU-6050 boards. 

         // Bluetooth
         // HEADER
         bluetoothLink.print("<");
      
         // Euler

         bluetoothLink.print((angles[ARMIDX].euler[PSI]   - offsetX_ArmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");
         bluetoothLink.print((angles[ARMIDX].euler[THETA] - offsetY_ArmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");
         bluetoothLink.print((angles[ARMIDX].euler[PHI]   - offsetZ_ArmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");

         bluetoothLink.print((angles[FOREARMIDX].euler[PSI]   - offsetX_ForearmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");
         bluetoothLink.print((angles[FOREARMIDX].euler[THETA] - offsetY_ForearmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");
         bluetoothLink.print((angles[FOREARMIDX].euler[PHI]   - offsetZ_ForearmAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");

         bluetoothLink.print((angles[HANDIDX].euler[PSI]   - offsetX_HandAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");
         bluetoothLink.print((angles[HANDIDX].euler[THETA] - offsetY_HandAngle ) * DEGREE_CONV_FCTR );   bluetoothLink.print(", ");
         bluetoothLink.print((angles[HANDIDX].euler[PHI]   - offsetZ_HandAngle ) * DEGREE_CONV_FCTR );  bluetoothLink.print(", ");

         bluetoothLink.print(thumbNorm);  bluetoothLink.print(", ");
         bluetoothLink.print(indexNorm);  bluetoothLink.print(", ");
         bluetoothLink.print(middleNorm); bluetoothLink.print(", ");
         bluetoothLink.print(ringNorm);   bluetoothLink.print(", ");
         bluetoothLink.print(littleNorm);  

         // FOOTER
         bluetoothLink.println(">");

         // Serial Port 
         // HEADER
         Serial.print("<");
      
         // Euler
         Serial.print(angles[ARMIDX].euler[PSI]   * DEGREE_CONV_FCTR );  Serial.print(", ");
         Serial.print(angles[ARMIDX].euler[THETA] * DEGREE_CONV_FCTR );  Serial.print(", ");
         Serial.print(angles[ARMIDX].euler[PHI]   * DEGREE_CONV_FCTR );  Serial.print(", ");

         Serial.print(angles[FOREARMIDX].euler[PSI]   * DEGREE_CONV_FCTR );  Serial.print(", ");
         Serial.print(angles[FOREARMIDX].euler[THETA] * DEGREE_CONV_FCTR );  Serial.print(", ");
         Serial.print(angles[FOREARMIDX].euler[PHI]   * DEGREE_CONV_FCTR );  Serial.print(", ");

         Serial.print(angles[HANDIDX].euler[PSI]   * DEGREE_CONV_FCTR );  Serial.print(", ");
         Serial.print(angles[HANDIDX].euler[THETA] * DEGREE_CONV_FCTR );   Serial.print(", ");
         Serial.print(angles[HANDIDX].euler[PHI]   * DEGREE_CONV_FCTR );  Serial.print(", ");

         Serial.print(thumbNorm);  Serial.print(", ");
         Serial.print(indexNorm);  Serial.print(", ");
         Serial.print(middleNorm); Serial.print(", ");
         Serial.print(ringNorm);   Serial.print(", ");
         Serial.print(littleNorm);  

         // FOOTER
         Serial.println(">");
    }

    void Glove::printDebugStatusNolimit(void) const noexcept{
               Serial.println("--- Fingers ----");
               Serial.print("Index: ");
               Serial.println(indexCurrent);
               Serial.println(indexNorm);
               Serial.print("Middle:");
               Serial.println(middleCurrent);
               Serial.println(middleNorm);
               Serial.print("Ring:");
               Serial.println(ringCurrent);
               Serial.println(ringNorm);
               Serial.print("Little:");
               Serial.println(littleCurrent);
               Serial.println(littleNorm);
               Serial.print("Thumb:");
               Serial.println(thumbCurrent);
               Serial.println(thumbNorm);
               Serial.print("Inf: ");
               Serial.println(thumbMin);
               Serial.print("Sup: ");
               Serial.println(thumbMax);
               Serial.print("Norm: ");
               Serial.println(normalizeStatus(thumbMin, thumbMax, thumbCurrent));
               Serial.println("--- Quaternions ----");
                   
               Serial.print("[");
               for (const auto& angle: angles){
                   Serial.print(angle.quaternion.w);Serial.print(", ");
                   Serial.print(angle.quaternion.x);Serial.print(", ");
                   Serial.print(angle.quaternion.y);Serial.print(", ");
                   Serial.print(angle.quaternion.z);
               }
               Serial.println("]");

               Serial.println("--- EULER ----");
               Serial.print("[");
               for (const auto& angle: angles){
                   Serial.print(angle.euler[PSI]);   Serial.print(", ");
                   Serial.print(angle.euler[THETA]); Serial.print(", ");
                   Serial.print(angle.euler[PHI]);
               }
               Serial.println("]");

               Serial.println("--- ANGLE OFFSETS ----");
               Serial.print("{");
               Serial.print(offsetX_ArmAngle);  Serial.print(", ");
               Serial.print(offsetY_ArmAngle);  Serial.print(", ");
               Serial.print(offsetZ_ArmAngle);  Serial.print(", ");
               Serial.print(offsetX_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetY_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetZ_ForearmAngle);  Serial.print(", ");
               Serial.print(offsetX_HandAngle);  Serial.print(", ");
               Serial.print(offsetY_HandAngle);  Serial.print(", ");
               Serial.print(offsetZ_HandAngle);
               Serial.print("}");

               Serial.println("--- EULER DEGREES ----");
               Serial.print("<");
               for (const auto& angle: angles){
                   Serial.print(angle.euler[PSI]   * DEGREE_CONV_FCTR );  Serial.print(", ");
                   Serial.print(angle.euler[THETA] * DEGREE_CONV_FCTR );  Serial.print(", ");
                   Serial.print(angle.euler[PHI]   * DEGREE_CONV_FCTR );
               }
               Serial.println(">");
               Serial.println("-------");
    }

    void Glove::readStatus(void) noexcept{
            indexCurrent  = analogRead(INDEX_PIN);
            indexNorm     = normalizeStatus( indexMin, indexMax, indexCurrent);
            indexStatus   = indexCurrent > ( indexMin + ( indexMin * ( sensitivity / 100) ))  ? true : false;
            middleCurrent = analogRead(MIDDLE_PIN);
            middleStatus  = middleCurrent > ( middleMin + ( middleMin * ( sensitivity / 100) ))  ? true : false;
            middleNorm    = normalizeStatus( middleMin, middleMax, middleCurrent);
            littleCurrent = analogRead(LITTLE_PIN);
            littleStatus  = littleCurrent > ( middleMin + ( middleMin * ( sensitivity / 100) ))  ? true : false;
            littleNorm    = normalizeStatus( littleMin, littleMax, littleCurrent);
            ringCurrent   = analogRead(RING_PIN);
            ringStatus    = ringCurrent > ( ringMin + ( ringMin * ( sensitivity / 100) ))  ? true : false;
            ringNorm      = normalizeStatus( ringMin, ringMax, ringCurrent);
            thumbCurrent  = analogRead(THUMB_PIN);
            thumbStatus   = thumbCurrent > ( thumbMin + ( thumbMin * ( sensitivity / 100) ))  ? true : false;
            thumbNorm     = normalizeStatus( thumbMin, thumbMax, thumbCurrent);

            buttonLeft   = checkLeftButton();
            buttonMiddle = checkMiddleButton();
            buttonRight  = checkRightButton();

            updateStatusForBt();
    }

    void  Glove::refreshFingers(void)  noexcept{
      byte xpos { 36 },
           ypos { 0 };
      
      readStatus();
      bluetoothLink.println(statusForBt);
      
      #ifdef DEBUG_GLOVE
      printDebugStatusNolimit();
      #endif

      tft.setTextColor(indexStatus ? TFT_BLUE : TFT_ORANGE, TFT_BLACK); 
      tft.drawString("1",xpos,ypos,7); 
      xpos = 64;
      tft.setTextColor(middleStatus ? TFT_BLUE : TFT_ORANGE, TFT_BLACK); 
      tft.drawString("1",xpos,ypos,7); 
      xpos = 92;
      tft.setTextColor(ringStatus ? TFT_BLUE : TFT_ORANGE, TFT_BLACK); 
      tft.drawString("1",xpos,ypos,7); 
      xpos = 120;
      tft.setTextColor(littleStatus ? TFT_BLUE : TFT_ORANGE, TFT_BLACK); 
      tft.drawString("1",xpos,ypos,7); 
      xpos = 10;
      ypos = 24;
      tft.setTextColor(thumbStatus ? TFT_BLUE : TFT_ORANGE, TFT_BLACK); 
      tft.drawString("1",xpos,ypos,7); 
      
    }
          
    void  Glove::updateStatusForBt(void)  noexcept {
          statusForBt[EVENTS::THUMB]         = thumbNorm  > 0  ? thumbNorm  - 1 : 0;
          statusForBt[EVENTS::INDEX]         = indexNorm  > 0  ? indexNorm  - 1 : 0;
          statusForBt[EVENTS::MIDDLE]        = middleNorm > 0  ? middleNorm - 1 : 0;
          statusForBt[EVENTS::RING]          = ringNorm   > 0  ? ringNorm   - 1 : 0;
          statusForBt[EVENTS::LITTLE]        = littleNorm > 0  ? littleNorm - 1 : 0;
          statusForBt[EVENTS::BUTTON_LEFT]   = buttonLeft   ? '1' : '0';
          statusForBt[EVENTS::BUTTON_MIDDLE] = buttonMiddle ? '1' : '0';
          statusForBt[EVENTS::BUTTON_RIGHT]  = buttonRight  ? '1' : '0';
    }

    void  Glove::calculateMeans(void)  noexcept{

        double  sumAccX_ArmAngle     { 0.0 }, 
                sumAccY_ArmAngle     { 0.0 }, 
                sumAccZ_ArmAngle     { 0.0 },
                sumAccX_ForearmAngle { 0.0 }, 
                sumAccY_ForearmAngle { 0.0 }, 
                sumAccZ_ForearmAngle { 0.0 },
                sumAccX_HandAngle    { 0.0 }, 
                sumAccY_HandAngle    { 0.0 }, 
                sumAccZ_HandAngle    { 0.0 };

        for(int i{0}; i<AVERAGE_ELEMS_NUM; i++){
                handleEvents();

                sumAccX_ArmAngle += angles[ARMIDX].euler[PSI];
                sumAccY_ArmAngle += angles[ARMIDX].euler[THETA];
                sumAccZ_ArmAngle += angles[ARMIDX].euler[PHI];
          
                sumAccX_ForearmAngle += angles[FOREARMIDX].euler[PSI];
                sumAccY_ForearmAngle += angles[FOREARMIDX].euler[THETA];
                sumAccZ_ForearmAngle += angles[FOREARMIDX].euler[PHI];
          
                sumAccX_HandAngle += angles[HANDIDX].euler[PSI];
                sumAccY_HandAngle += angles[HANDIDX].euler[THETA];
                sumAccZ_HandAngle += angles[HANDIDX].euler[PHI];

                delay(AVERAGE_CALC_WAIT);
        }

        offsetX_ArmAngle     = sumAccX_ArmAngle     / AVERAGE_ELEMS_NUM;
        offsetY_ArmAngle     = sumAccY_ArmAngle     / AVERAGE_ELEMS_NUM;
        offsetZ_ArmAngle     = sumAccZ_ArmAngle     / AVERAGE_ELEMS_NUM;
        offsetX_ForearmAngle = sumAccX_ForearmAngle / AVERAGE_ELEMS_NUM;
        offsetY_ForearmAngle = sumAccY_ForearmAngle / AVERAGE_ELEMS_NUM;
        offsetZ_ForearmAngle = sumAccZ_ForearmAngle / AVERAGE_ELEMS_NUM;
        offsetX_HandAngle    = sumAccX_HandAngle    / AVERAGE_ELEMS_NUM;
        offsetY_HandAngle    = sumAccY_HandAngle    / AVERAGE_ELEMS_NUM;
        offsetZ_HandAngle    = sumAccZ_HandAngle    / AVERAGE_ELEMS_NUM;

    }

    void Glove::calibrateFingers(void) noexcept{
         tft.setTextColor(TFT_RED, TFT_BLACK); 
         tft.drawCentreString("Relax all",120,48,2); // Next size up font 2
         waitConfirm();
         setMin();
         tft.drawCentreString("Min ok   ",120,48,2); // Next size up font 2
         delay(2000);
         tft.drawCentreString("Contract all",120,48,2); // Next size up font 2
         waitConfirm();
         setMax();
         tft.drawCentreString("Max ok      ",120,48,2); // Next size up font 2
         delay(2000);
         tft.drawCentreString("Fingers Calibrated  ",120,48,2); // Next size up font 2
         delay(2000);
         tft.drawCentreString("                     ",116,48,2); // Next size up font 2
    }

    void Glove::calibrateArticulations(void) noexcept{
         tft.setTextColor(TFT_RED, TFT_BLACK); 
         tft.drawCentreString("Put Arm Orizontal pos.",120,48,2); // Next size up font 2
         waitConfirm();
         calculateMeans();
         tft.drawCentreString("                         ",116,48,2); // Next size up font 2
         tft.drawCentreString("Accelerometer ok",120,48,2); // Next size up font 2
         delay(2000);
         tft.drawCentreString("                         ",116,48,2); // Next size up font 2
         tft.drawCentreString("Calibrated  ",120,48,2); // Next size up font 2
         delay(2000);
         tft.drawCentreString("                         ",116,48,2); // Next size up font 2
    }

    uint16_t Glove::normalizeStatus(uint16_t inf, uint16_t sup, uint16_t curr) const noexcept{   
            assert(NORM_MAX == 90);

            return  NORM_MAX - abs(( log10(curr < 1 ? 1 : curr) * NORM_MAX) / log10(sup > 20 ? sup : 20));
    }

    void Glove::handleEvents(void) noexcept{
        if (initial) 
          initial = false;

        refreshFingers();

        if( buttonLeft && buttonMiddle )
            calibrateFingers();

        if( buttonRight && buttonMiddle )
            calibrateArticulations();

        selectAccel(angles[ARMIDX].device);

        if (mpu.dmpGetCurrentFIFOPacket(angles[ARMIDX].fifo_buffer)) {
               angles[ARMIDX].movement = true;
               mpu.dmpGetQuaternion(&(angles[ARMIDX].quaternion), angles[ARMIDX].fifo_buffer);
               mpu.dmpGetEuler(angles[ARMIDX].euler, &(angles[ARMIDX].quaternion));
        } else  {
               angles[ARMIDX].movement = false;
        }

        selectAccel(angles[FOREARMIDX].device);

        if (mpu.dmpGetCurrentFIFOPacket(angles[FOREARMIDX].fifo_buffer)) {
               angles[FOREARMIDX].movement = true;
               mpu.dmpGetQuaternion(&(angles[FOREARMIDX].quaternion), angles[FOREARMIDX].fifo_buffer);
               mpu.dmpGetEuler(angles[FOREARMIDX].euler, &(angles[FOREARMIDX].quaternion));
        } else  {
               angles[FOREARMIDX].movement = false;
        }

        selectAccel(angles[HANDIDX].device);

        if (mpu.dmpGetCurrentFIFOPacket(angles[HANDIDX].fifo_buffer)) {
               angles[HANDIDX].movement = true;
               mpu.dmpGetQuaternion(&(angles[HANDIDX].quaternion), angles[HANDIDX].fifo_buffer);
               mpu.dmpGetEuler(angles[HANDIDX].euler, &(angles[HANDIDX].quaternion));
        } else  {
               angles[HANDIDX].movement = false;
        }
    }

    void   Glove::selectAccel(uint8_t addr)  noexcept{
       if(addr > 0x7) {
           Serial.print("Error: invalid multiplexer subaddr: ");
           Serial.println(addr);
           stale();
       }

       Wire.beginTransmission(TCAADDR);
       Wire.write(1 << addr);
       Wire.endTransmission();  
    }
  
    void   Glove::printPortStats(bool block) noexcept{
        Serial.println("-------------");
        for (uint8_t t=0; t<8; t++) {
            selectAccel(t);
            Serial.print("TCA9548 Port #"); 
            Serial.println(t);

            for (uint8_t addr = 0; addr <= 0x7F; addr++) {
                 if (addr == TCAADDR) continue;

                 Wire.beginTransmission(addr);
                 if (!Wire.endTransmission()) {
                    Serial.print("Found I2C with addr 0x");  
                    Serial.println(addr,HEX);
                 }
            }
        }
        Serial.println("\nScan Complete.");
        Serial.println("-------------");

        if(block) stale();
    }
} // End namespace glove
