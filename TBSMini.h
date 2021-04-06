#ifndef TBSMINI_H
#define TBSMINI_H

#include <Arduino.h>
#include <arduino-timer.h>
#include <Servo.h>

typedef struct prop3SoundSettings {
    uint16_t pulse;                     // Pulse in microseconds associated to slot
    uint8_t  priority;                  // Sound priority
};

/**
 * Class to control a TBS Mini (Benedini or Dasmikro clone) able to:
 *      Start/stop engine.
 *      Proportional sound engine.
 *      Up to 14 different sounds to play it individually with priority control.
 *      Volumen up/down.
 */
class TBSMini {

    private:
        // Prop1 - RC input and Gas type. From idle to full speed.
        static const int m_PROP1_IDLE        = 1500;                            // Idle throttle
        static const int m_PROP1_FULL_SPEED  = 2000;                            // Full speed

        // Prop2 - RC input and 2nd Function 1/2 type. 2 sounds, like a 3-position switch.
        static const int m_PROP2_SWITCH_OFF = 1500;                             // Off position 2nd Function 1/2 (default)
        static const int m_PROP2_SWITCH_1   = 2000;                             // Position 1 sound for 2nd Coder
        static const int m_PROP2_SWITCH_2   = 1000;                             // Position 2 sound for 2nd Coder
        static const int m_PROP2_mS_TIMER   =  500;                             // Delay between signals sent to Prop2. We need to send m_PROP2_SWITCH_X follow by m_PROP2_SWITCH_OFF after this time to work properly.
        static bool m_prop2TimerComplete;                                       // Flag to track de timer

        // Prop3 - RC input and 1st Coder 12-Key type, to play up tu 14 individual sounds, plus volume up/down.
        static const uint8_t TBSMINI_PROP3_NUM_SOUNDS = 17;                     // 16 slots + 1 (sound off)
        byte m_prop3CurrentSound;                                               // Current sound playing
        static const int m_PROP3_SOUND_OFF = 0;                                 // Index of no sound pulse
        static const int m_PROP3_mS_TIMER = 50;                                 // Delay between signals sent to Prop3. We need to send the sound pulse follow by off sound pulse to work properly.
        static bool m_prop3TimerComplete;                                       // Flaf to track the timer
        static prop3SoundSettings m_PROP3Sounds[TBSMINI_PROP3_NUM_SOUNDS] = {
            {1531, 1},  // Sound 0:  Prop3 default (off - no sound) - m_PROP3_SOUND_OFF - this is the center value
            { 864, 1},  // Sound 1.
            { 945, 1},  // Sound 2.
            {1026, 1},  // Sound 3.
            {1108, 1},  // Sound 4.
            {1189, 1},  // Sound 5.
            {1270, 1},  // Sound 6.
            {1351, 1},  // Sound 7.
            {1433, 1},  // Sound 8.
            {1630, 1},  // Sound 9.
            {1711, 1},  // Sound 10.
            {1793, 1},  // Sound 11.
            {1874, 1},  // Sound 12.
            {1955, 1},  // Sound 13.
            {2036, 1},  // Sound 14.
            {2117, 1},  // Sound 15: volume up - highest priority
            {2199, 1}   // Sound 16: volume down - highest priority
        };

        byte m_prop1Pin;                        // prop1 pin
        byte m_prop2Pin;                        // prop2 pin
        byte m_prop3Pin;                        // prop3 pin
        static Servo m_prop1;                   // "RC" input, "Gas" type
        static Servo m_prop2;                   // "RC" input, "2nd Function 1/2" type
        static Servo m_prop3;                   // "RC" input, "1st Coder 12-Key" type
        bool m_isEngineRunning;                 // Flag to check if engine is running
        int m_minSpeed;                         // Min value from stick controller
        int m_maxSpeed;                         // Max value from stick controller
        int m_currentSpeed;                     // Current speed in microseconds
        bool m_debugMode;                       // Enable for debug prints
        static Timer<> * m_APTimer;             // Timer for sending signals to props

        void printDebug();
        uint8_t soundPriority(uint8_t prop3Sound);
        static void startProp2Timer();
        static bool clearProp2Timer(void *);
        static void startProp3Timer();
        static bool clearProp3Timer(void *);

    public:

        TBSMini();
        void begin(Timer<> * APTimer, int prop1Pin, int prop2Pin, int prop3Pin, int minSpeed, int maxSpeed);
        void toggleEngine();
        void idleEngine();
        void setEngineSpeed(int speed);
        void enableDebug();
        void disableDebug();
        void playSound(uint8_t prop3Sound);
        void volumeUp();
        void volumeDown();
};

#endif
