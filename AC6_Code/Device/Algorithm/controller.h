/**
  ******************************************************************************
  * @file	 controller.h
  * @author  Wang Hongxi
  * @author  Zhang Hongyu (fuzzy pid)
  * @version V1.1.3
  * @date    2021/7/3
  * @brief   
  ******************************************************************************
  * @attention 
  *
  ******************************************************************************
  */
#ifndef _CONTROLLER_H
#define _CONTROLLER_H


#include "main.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"
#include "user_lib.h"
#include <math.h>

#ifndef abs
#define abs(x) ((x > 0) ? x : -x)
#endif

#ifndef user_malloc
#ifdef _CMSIS_OS_H
#define user_malloc pvPortMalloc
#else
#define user_malloc malloc
#endif
#endif

/******************************** FUZZY PID **********************************/
#define NB -3
#define NM -2
#define NS -1
#define ZE 0
#define PS 1
#define PM 2
#define PB 3

typedef  struct
{
    float KpFuzzy;
    float KiFuzzy;
    float KdFuzzy;

    float (*FuzzyRuleKp)[7];
    float (*FuzzyRuleKi)[7];
    float (*FuzzyRuleKd)[7];

    float KpRatio;
    float KiRatio;
    float KdRatio;

    float eStep;
    float ecStep;

    float e;
    float ec;
    float eLast;

    uint32_t DWT_CNT;
    float dt;
} __packed FuzzyRule_t;

void Fuzzy_Rule_Init(FuzzyRule_t *fuzzyRule, float (*fuzzyRuleKp)[7], float (*fuzzyRuleKi)[7], float (*fuzzyRuleKd)[7],
                     float kpRatio, float kiRatio, float kdRatio,
                     float eStep, float ecStep);
void Fuzzy_Rule_Implementation(FuzzyRule_t *fuzzyRule, float measure, float ref);

/******************************* PID CONTROL *********************************/
typedef enum pid_Improvement_e
{
    NONE = 0X00,                        //0000 0000
    Integral_Limit = 0x01,              //0000 0001
    Derivative_On_Measurement = 0x02,   //0000 0010
    Trapezoid_Intergral = 0x04,         //0000 0100
    Proportional_On_Measurement = 0x08, //0000 1000
    OutputFilter = 0x10,                //0001 0000
    ChangingIntegrationRate = 0x20,     //0010 0000
    DerivativeFilter = 0x40,            //0100 0000
    ErrorHandle = 0x80,                 //1000 0000
} PID_Improvement_e;

typedef enum errorType_e
{
    PID_ERROR_NONE = 0x00U,
    Motor_Blocked = 0x01U
} ErrorType_e;

typedef  struct
{
    uint64_t ERRORCount;
    ErrorType_e ERRORType;
} __packed PID_ErrorHandler_t;

typedef  struct pid_t
{
    float Ref;
    float Kp;
    float Ki;
    float Kd;

    float Measure;
    float Last_Measure;
    float Err;
    float Last_Err;
    float Last_ITerm;

    float Pout;
    float Iout;
    float Dout;
    float ITerm;

    float Output;
    float Last_Output;
    float Last_Dout;

    float MaxOut;
    float IntegralLimit;
    float DeadBand;
    float ControlPeriod;
    float CoefA;         //For Changing Integral
    float CoefB;         //ITerm = Err*((A-abs(err)+B)/A)  when B<|err|<A+B
    float Output_LPF_RC; // RC = 1/omegac
    float Derivative_LPF_RC;

    uint16_t OLS_Order;
    Ordinary_Least_Squares_t OLS;

    uint32_t DWT_CNT;
    float dt;

    FuzzyRule_t *FuzzyRule;

    uint8_t Improve;

    PID_ErrorHandler_t ERRORHandler;

    void (*User_Func1_f)(struct pid_t *pid);
    void (*User_Func2_f)(struct pid_t *pid);
} __packed PID_t;

void PID_Init(
    PID_t *pid,
    float max_out,
    float intergral_limit,
    float deadband,

    float kp,
    float ki,
    float kd,

    float A,
    float B,

    float output_lpf_rc,
    float derivative_lpf_rc,

    uint16_t ols_order,

    uint8_t improve);
float PID_Calculate(PID_t *pid, float measure, float ref);

/*************************** FEEDFORWARD CONTROL *****************************/
typedef  struct
{
    float c[3]; // G(s) = 1/(c2s^2 + c1s + c0)

    float Ref;
    float Last_Ref;

    float DeadBand;

    uint32_t DWT_CNT;
    float dt;

    float LPF_RC; // RC = 1/omegac

    float Ref_dot;
    float Ref_ddot;
    float Last_Ref_dot;

    uint16_t Ref_dot_OLS_Order;
    Ordinary_Least_Squares_t Ref_dot_OLS;
    uint16_t Ref_ddot_OLS_Order;
    Ordinary_Least_Squares_t Ref_ddot_OLS;

    float Output;
    float MaxOut;

} __packed Feedforward_t;

void Feedforward_Init(
    Feedforward_t *ffc,
    float max_out,
    float *c,
    float lpf_rc,
    uint16_t ref_dot_ols_order,
    uint16_t ref_ddot_ols_order);

float Feedforward_Calculate(Feedforward_t *ffc, float ref);

/************************* LINEAR DISTURBANCE OBSERVER *************************/
typedef  struct
{
    float c[3]; // G(s) = 1/(c2s^2 + c1s + c0)

    float Measure;
    float Last_Measure;

    float u; // system input

    float DeadBand;

    uint32_t DWT_CNT;
    float dt;

    float LPF_RC; // RC = 1/omegac

    float Measure_dot;
    float Measure_ddot;
    float Last_Measure_dot;

    uint16_t Measure_dot_OLS_Order;
    Ordinary_Least_Squares_t Measure_dot_OLS;
    uint16_t Measure_ddot_OLS_Order;
    Ordinary_Least_Squares_t Measure_ddot_OLS;

    float Disturbance;
    float Output;
    float Last_Disturbance;
    float Max_Disturbance;
} __packed LDOB_t;

void LDOB_Init(
    LDOB_t *ldob,
    float max_d,
    float deadband,
    float *c,
    float lpf_rc,
    uint16_t measure_dot_ols_order,
    uint16_t measure_ddot_ols_order);

float LDOB_Calculate(LDOB_t *ldob, float measure, float u);

/*************************** Tracking Differentiator ***************************/
typedef  struct
{
    float Input;

    float h0;
    float r;

    float x;
    float dx;
    float ddx;

    float last_dx;
    float last_ddx;

    uint32_t DWT_CNT;
    float dt;
} __packed  TD_t;

void TD_Init(TD_t *td, float r, float h0);
float TD_Calculate(TD_t *td, float input);

#endif
