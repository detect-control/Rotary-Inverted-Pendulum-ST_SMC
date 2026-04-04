/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controller.h
 *
 * Code generated for Simulink model 'controller'.
 *
 * Model version                  : 1.4
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Mon Aug  4 22:23:22 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM 10
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef controller_h_
#define controller_h_
#ifndef controller_COMMON_INCLUDES_
#define controller_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "math.h"
#endif                                 /* controller_COMMON_INCLUDES_ */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T Probe[2];                     /* '<S15>/Probe' */
  real_T Probe_f[2];                   /* '<S10>/Probe' */
  real_T Integrator_DSTATE;            /* '<S14>/Integrator' */
  real_T Integrator_DSTATE_c;          /* '<S19>/Integrator' */
  int8_T Integrator_PrevResetState;    /* '<S14>/Integrator' */
  int8_T Integrator_PrevResetState_e;  /* '<S19>/Integrator' */
  uint8_T Integrator_IC_LOADING;       /* '<S14>/Integrator' */
  uint8_T Integrator_IC_LOADING_k;     /* '<S19>/Integrator' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T phi;                          /* '<Root>/phi' */
  real_T theta;                        /* '<Root>/theta' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T u;                            /* '<Root>/u' */
} ExtY;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void controller_initialize(void);
extern void controller_step(void);

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Gain' : Eliminated nontunable gain of 1
 * Block '<S14>/Saturation' : Eliminated Saturate block
 * Block '<S2>/[A,B]' : Eliminated Saturate block
 * Block '<S3>/Gain' : Eliminated nontunable gain of 1
 * Block '<S19>/Saturation' : Eliminated Saturate block
 * Block '<S3>/[A,B]' : Eliminated Saturate block
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('staSmc/controller')    - opens subsystem staSmc/controller
 * hilite_system('staSmc/controller/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'staSmc'
 * '<S1>'   : 'staSmc/controller'
 * '<S2>'   : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)'
 * '<S3>'   : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1'
 * '<S4>'   : 'staSmc/controller/det(M)'
 * '<S5>'   : 'staSmc/controller/h1'
 * '<S6>'   : 'staSmc/controller/h2'
 * '<S7>'   : 'staSmc/controller/m11'
 * '<S8>'   : 'staSmc/controller/m12, m21'
 * '<S9>'   : 'staSmc/controller/m22'
 * '<S10>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)/Enable//disable time constant'
 * '<S11>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)/Initialization'
 * '<S12>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)/Integrator (Discrete or Continuous)'
 * '<S13>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)/Initialization/Init_u'
 * '<S14>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)/Integrator (Discrete or Continuous)/Discrete'
 * '<S15>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1/Enable//disable time constant'
 * '<S16>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1/Initialization'
 * '<S17>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1/Integrator (Discrete or Continuous)'
 * '<S18>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1/Initialization/Init_u'
 * '<S19>'  : 'staSmc/controller/Filtered Derivative (Discrete or Continuous)1/Integrator (Discrete or Continuous)/Discrete'
 * '<S20>'  : 'staSmc/controller/det(M)/Compare To Constant'
 * '<S21>'  : 'staSmc/controller/m11/Compare To Constant'
 */
#endif                                 /* controller_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
