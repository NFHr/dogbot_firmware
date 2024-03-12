#include "motor.h"

// Define motor pins
#define PWMA 12 // Motor A PWM
#define DIRA1 34
#define DIRA2 35 // Motor A Direction

#define PWMB 8 // Motor B PWM
#define DIRB1 37
#define DIRB2 36 // Motor B Direction

#define PWMC 6 // Motor C PWM
#define DIRC1 43
#define DIRC2 42 // Motor C Direction

#define PWMD 5   // Motor D PWM
#define DIRD1 A4 // 26
#define DIRD2 A5 // 27  //Motor D Direction

#define MOTORA_FORWARD(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRA1, LOW);  \
        digitalWrite(DIRA2, HIGH); \
        analogWrite(PWMA, pwm);    \
    } while (0)
#define MOTORA_STOP(x)            \
    do                            \
    {                             \
        digitalWrite(DIRA1, LOW); \
        digitalWrite(DIRA2, LOW); \
        analogWrite(PWMA, 0);     \
    } while (0)
#define MOTORA_BACKOFF(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRA1, HIGH); \
        digitalWrite(DIRA2, LOW);  \
        analogWrite(PWMA, pwm);    \
    } while (0)

#define MOTORB_FORWARD(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRB1, LOW);  \
        digitalWrite(DIRB2, HIGH); \
        analogWrite(PWMB, pwm);    \
    } while (0)
#define MOTORB_STOP(x)            \
    do                            \
    {                             \
        digitalWrite(DIRB1, LOW); \
        digitalWrite(DIRB2, LOW); \
        analogWrite(PWMB, 0);     \
    } while (0)
#define MOTORB_BACKOFF(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRB1, HIGH); \
        digitalWrite(DIRB2, LOW);  \
        analogWrite(PWMB, pwm);    \
    } while (0)

#define MOTORC_FORWARD(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRC1, LOW);  \
        digitalWrite(DIRC2, HIGH); \
        analogWrite(PWMC, pwm);    \
    } while (0)
#define MOTORC_STOP(x)            \
    do                            \
    {                             \
        digitalWrite(DIRC1, LOW); \
        digitalWrite(DIRC2, LOW); \
        analogWrite(PWMC, 0);     \
    } while (0)
#define MOTORC_BACKOFF(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRC1, HIGH); \
        digitalWrite(DIRC2, LOW);  \
        analogWrite(PWMC, pwm);    \
    } while (0)

#define MOTORD_FORWARD(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRD1, LOW);  \
        digitalWrite(DIRD2, HIGH); \
        analogWrite(PWMD, pwm);    \
    } while (0)
#define MOTORD_STOP(x)            \
    do                            \
    {                             \
        digitalWrite(DIRD1, LOW); \
        digitalWrite(DIRD2, LOW); \
        analogWrite(PWMD, 0);     \
    } while (0)
#define MOTORD_BACKOFF(pwm)        \
    do                             \
    {                              \
        digitalWrite(DIRD1, HIGH); \
        digitalWrite(DIRD2, LOW);  \
        analogWrite(PWMD, pwm);    \
    } while (0)

Motor::Motor()
{
    ;
}

//    ↓A-----B↓
//     |  |  |
//     |  ↓  |
//    ↓C-----D↓
void Motor::BACK(uint8_t motor_pwm)
{
    MOTORA_BACKOFF(motor_pwm);
    MOTORB_FORWARD(motor_pwm);
    MOTORC_BACKOFF(motor_pwm);
    MOTORD_FORWARD(motor_pwm);
}

//    ↑A-----B↑
//     |  ↑  |
//     |  |  |
//    ↑C-----D↑
void Motor::ADVANCE(uint8_t motor_pwm)
{
    MOTORA_FORWARD(motor_pwm);
    MOTORB_BACKOFF(motor_pwm);
    MOTORC_FORWARD(motor_pwm);
    MOTORD_BACKOFF(motor_pwm);
}
//    =A-----B↑
//     |   ↖ |
//     | ↖   |
//    ↑C-----D=
void Motor::ADVANCE_LEFT(uint8_t motor_pwm)
{
    MOTORA_STOP(motor_pwm);
    MOTORB_FORWARD(motor_pwm);
    MOTORC_BACKOFF(motor_pwm);
    MOTORD_STOP(motor_pwm);
}

//    ↓A-----B↑
//     |  ←  |
//     |  ←  |
//    ↑C-----D↓
void Motor::RIGHT(uint8_t motor_pwm)
{
    MOTORA_FORWARD(motor_pwm);
    MOTORB_FORWARD(motor_pwm);
    MOTORC_BACKOFF(motor_pwm);
    MOTORD_BACKOFF(motor_pwm);
}
//    ↓A-----B=
//     | ↙   |
//     |   ↙ |
//    =C-----D↓
void Motor::BACK_LEFT(uint8_t motor_pwm)
{
    MOTORA_FORWARD(motor_pwm);
    MOTORB_STOP(motor_pwm);
    MOTORC_STOP(motor_pwm);
    MOTORD_BACKOFF(motor_pwm);
}
//    ↑A-----B=
//     | ↗   |
//     |   ↗ |
//    =C-----D↑
void Motor::ADVANCE_RIGHT(uint8_t motor_pwm)
{
    MOTORA_BACKOFF(motor_pwm);
    MOTORB_STOP(motor_pwm);
    MOTORC_STOP(motor_pwm);
    MOTORD_FORWARD(motor_pwm);
}
//    ↑A-----B↓
//     |  →  |
//     |  →  |
//    ↓C-----D↑
void Motor::LEFT(uint8_t motor_pwm)
{
    MOTORA_BACKOFF(motor_pwm);
    MOTORB_BACKOFF(motor_pwm);
    MOTORC_FORWARD(motor_pwm);
    MOTORD_FORWARD(motor_pwm);
}
//    =A-----B↓
//     |   ↘ |
//     | ↘   |
//    ↓C-----D=
void Motor::BACK_RIGHT(uint8_t motor_pwm)
{
    MOTORA_STOP(motor_pwm);
    MOTORB_BACKOFF(motor_pwm);
    MOTORC_FORWARD(motor_pwm);
    MOTORD_STOP(motor_pwm);
}

//    ↑A-----B↓
//     | ↗ ↘ |
//     | ↖ ↙ |
//    ↑C-----D↓
void Motor::ROTATE_CW(uint8_t motor_pwm)
{
    MOTORA_BACKOFF(motor_pwm);
    MOTORB_BACKOFF(motor_pwm);
    MOTORC_BACKOFF(motor_pwm);
    MOTORD_BACKOFF(motor_pwm);
}

//    ↓A-----B↑
//     | ↙ ↖ |
//     | ↘ ↗ |
//    ↓C-----D↑
void Motor::ROTATE_CCW(uint8_t motor_pwm)
{
    MOTORA_FORWARD(motor_pwm);
    MOTORB_FORWARD(motor_pwm);
    MOTORC_FORWARD(motor_pwm);
    MOTORD_FORWARD(motor_pwm);
}

//    ↓A-----B↓
//     |  |  |
//     |  ↓  |
//    ↓C-----D↓
void Motor::BACK(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_BACKOFF(pwm_A);
    MOTORB_FORWARD(pwm_B);
    MOTORC_BACKOFF(pwm_C);
    MOTORD_FORWARD(pwm_D);
}

//    ↑A-----B↑
//     |  ↑  |
//     |  |  |
//    ↑C-----D↑
void Motor::ADVANCE(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_FORWARD(pwm_A);
    MOTORB_BACKOFF(pwm_B);
    MOTORC_FORWARD(pwm_C);
    MOTORD_BACKOFF(pwm_D);
}
//    =A-----B↑
//     |   ↖ |
//     | ↖   |
//    ↑C-----D=
void Motor::ADVANCE_LEFT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_STOP(pwm_A);
    MOTORB_FORWARD(pwm_B);
    MOTORC_BACKOFF(pwm_C);
    MOTORD_STOP(pwm_D);
}

//    ↓A-----B↑
//     |  ←  |
//     |  ←  |
//    ↑C-----D↓
void Motor::RIGHT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_FORWARD(pwm_A);
    MOTORB_FORWARD(pwm_B);
    MOTORC_BACKOFF(pwm_C);
    MOTORD_BACKOFF(pwm_D);
}
//    ↓A-----B=
//     | ↙   |
//     |   ↙ |
//    =C-----D↓
void Motor::BACK_LEFT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_FORWARD(pwm_A);
    MOTORB_STOP(pwm_B);
    MOTORC_STOP(pwm_C);
    MOTORD_BACKOFF(pwm_D);
}
//    ↑A-----B=
//     | ↗   |
//     |   ↗ |
//    =C-----D↑
void Motor::ADVANCE_RIGHT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_BACKOFF(pwm_A);
    MOTORB_STOP(pwm_B);
    MOTORC_STOP(pwm_C);
    MOTORD_FORWARD(pwm_D);
}
//    ↑A-----B↓
//     |  →  |
//     |  →  |
//    ↓C-----D↑
void Motor::LEFT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_BACKOFF(pwm_A);
    MOTORB_BACKOFF(pwm_B);
    MOTORC_FORWARD(pwm_C);
    MOTORD_FORWARD(pwm_D);
}
//    =A-----B↓
//     |   ↘ |
//     | ↘   |
//    ↓C-----D=
void Motor::BACK_RIGHT(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_STOP(pwm_A);
    MOTORB_BACKOFF(pwm_B);
    MOTORC_FORWARD(pwm_C);
    MOTORD_STOP(pwm_D);
}

//    ↑A-----B↓
//     | ↗ ↘ |
//     | ↖ ↙ |
//    ↑C-----D↓
void Motor::ROTATE_CW(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_BACKOFF(pwm_A);
    MOTORB_BACKOFF(pwm_B);
    MOTORC_BACKOFF(pwm_C);
    MOTORD_BACKOFF(pwm_D);
}

//    ↓A-----B↑
//     | ↙ ↖ |
//     | ↘ ↗ |
//    ↓C-----D↑
void Motor::ROTATE_CCW(uint8_t pwm_A, uint8_t pwm_B, uint8_t pwm_C, uint8_t pwm_D)
{
    MOTORA_FORWARD(pwm_A);
    MOTORB_FORWARD(pwm_B);
    MOTORC_FORWARD(pwm_C);
    MOTORD_FORWARD(pwm_D);
}

//    =A-----B=
//     |  =  |
//     |  =  |
//    =C-----D=
void Motor::STOP()
{
    MOTORA_STOP(motor_pwm);
    MOTORB_STOP(motor_pwm);
    MOTORC_STOP(motor_pwm);
    MOTORD_STOP(motor_pwm);
}

