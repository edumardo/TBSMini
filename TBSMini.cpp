#include "TBSMini.h"

bool TBSMini::m_prop2TimerComplete;
bool TBSMini::m_prop3TimerComplete;
prop3SoundSettings TBSMini::m_PROP3Sounds[TBSMINI_PROP3_NUM_SOUNDS];
Servo TBSMini::m_prop1;
Servo TBSMini::m_prop2;
Servo TBSMini::m_prop3;
Timer<> * TBSMini::m_APTimer;

/**
 *
 */
TBSMini::TBSMini() {

    m_isEngineRunning = false;
    m_debugMode = false;
    m_currentSpeed = m_PROP1_IDLE;
    m_prop3CurrentSound = m_PROP3_SOUND_OFF;
    m_prop2TimerComplete = true;
    m_prop2TimerComplete = true;
}

/**
 *
 */
void TBSMini::begin(Timer<> * APTimer, int prop1Pin, int prop2Pin, int prop3Pin, int minSpeed, int maxSpeed) {

    m_APTimer = APTimer;
    m_prop1Pin = prop1Pin;
    m_prop2Pin = prop2Pin;
    m_prop3Pin = prop3Pin;
    m_minSpeed = minSpeed;
    m_maxSpeed = maxSpeed;

    m_prop1.attach(m_prop1Pin);
    m_prop2.attach(m_prop2Pin);
    m_prop3.attach(m_prop3Pin);
    m_prop1.writeMicroseconds(m_PROP1_IDLE);
    m_prop2.writeMicroseconds(m_PROP2_SWITCH_OFF);
    m_prop3.writeMicroseconds(m_PROP3Sounds[m_PROP3_SOUND_OFF].pulse);
}

void TBSMini::toggleEngine() {

    m_prop2.writeMicroseconds(m_PROP2_SWITCH_1);
    startProp2Timer();
    m_isEngineRunning = !m_isEngineRunning;

    if (m_debugMode) {
        Serial.print("[TBSMini from ");
        Serial.print(!m_isEngineRunning ? "on" : "off");
        Serial.println(!m_isEngineRunning ? " to off]" : " to on]");
    }
}

/**
 *
 */
void TBSMini::idleEngine() {

    if (m_isEngineRunning) m_prop1.writeMicroseconds(m_PROP1_IDLE);

    if (m_debugMode) printDebug();

}

/**
 *
 */
void TBSMini::setEngineSpeed(int speed) {

    if (m_isEngineRunning) {
        m_currentSpeed = map(speed, m_minSpeed, m_maxSpeed, m_PROP1_IDLE, m_PROP1_FULL_SPEED);
        m_prop1.writeMicroseconds(m_currentSpeed);
    }

    if (m_debugMode) printDebug();
}

/**
 *
 */
void TBSMini::enableDebug() {

    m_debugMode = true;
}

/**
 *
 */
void TBSMini::disableDebug() {

    m_debugMode = false;
}

void TBSMini::playSound(uint8_t prop3Sound) {

    if (prop3Sound < 1 || prop3Sound > TBSMINI_PROP3_NUM_SOUNDS)
        return;

    m_prop3CurrentSound = prop3Sound;
    m_prop3.writeMicroseconds(m_PROP3Sounds[prop3Sound].pulse);
    startProp3Timer();

    if (m_debugMode) {
        Serial.print("[TBSMini play sound ");
        Serial.print(prop3Sound);
        Serial.print("][Prio current ");
        Serial.print(soundPriority(m_prop3CurrentSound));
        Serial.print("][Prio new ");
        Serial.print(soundPriority(prop3Sound));
        Serial.print("]----[P1 ");
        Serial.print(m_prop1.readMicroseconds());
        Serial.print("][P2 ");
        Serial.print(m_prop2.readMicroseconds());
        Serial.print("][P3 ");
        Serial.print(m_prop3.readMicroseconds());
        Serial.println("]");
    }
}

/**
 *
 */
void TBSMini::startProp2Timer() {

    m_prop2TimerComplete = false;
    m_APTimer->in(m_PROP2_mS_TIMER, clearProp2Timer);
}

/**
 *
 */
bool TBSMini::clearProp2Timer(void *) {

    m_prop2TimerComplete = true;
    m_prop2.writeMicroseconds(m_PROP2_SWITCH_OFF);
}

/**
 *
 */
void TBSMini::startProp3Timer() {

    m_prop3TimerComplete = false;
    m_APTimer->in(m_PROP3_mS_TIMER, clearProp3Timer);
}

/**
 *
 */
bool TBSMini::clearProp3Timer(void *) {

    m_prop3TimerComplete = true;
    m_prop3.writeMicroseconds(m_PROP3Sounds[m_PROP3_SOUND_OFF].pulse);
}

/**
 *
 */
void TBSMini::printDebug() {

    Serial.print("[TBSMini ");
    Serial.print(m_isEngineRunning ? "on" : "off");
    Serial.print("][Speed ");
    Serial.print(m_currentSpeed);
    Serial.println("]");
}

uint8_t TBSMini::soundPriority(uint8_t prop3Sound) {

    return m_PROP3Sounds[prop3Sound].priority;
}

void TBSMini::volumeUp() {

    playSound(15);
}

void TBSMini::volumeDown() {

    playSound(16);
}
