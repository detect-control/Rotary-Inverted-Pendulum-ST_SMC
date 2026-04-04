/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: controller.c
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

#include "controller.h"
#include <math.h>
#include "rtwtypes.h"
#include "math.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
extern real_T rtInf;
extern real_T rtMinusInf;
extern real_T rtNaN;
extern real32_T rtInfF;
extern real32_T rtMinusInfF;
extern real32_T rtNaNF;
static boolean_T rtIsInf(real_T value);
static boolean_T rtIsInfF(real32_T value);
static boolean_T rtIsNaN(real_T value);
static boolean_T rtIsNaNF(real32_T value);
real_T rtNaN = -(real_T)NAN;
real_T rtInf = (real_T)INFINITY;
real_T rtMinusInf = -(real_T)INFINITY;
real32_T rtNaNF = -(real32_T)NAN;
real32_T rtInfF = (real32_T)INFINITY;
real32_T rtMinusInfF = -(real32_T)INFINITY;

/* Model step function */
void controller_step(void)
{
  real_T rtb_Divide;
  real_T rtb_Product1_i_tmp;
  real_T rtb_u;
  real_T rtb_uT;
  real_T rtb_uT_n;
  real_T rtb_u_tmp;

  /* DiscreteIntegrator: '<S14>/Integrator' incorporates:
   *  Inport: '<Root>/phi'
   */
  if (rtDW.Integrator_IC_LOADING != 0) {
    rtDW.Integrator_DSTATE = rtU.phi;
  }

  if (rtDW.Integrator_PrevResetState != 0) {
    rtDW.Integrator_DSTATE = rtU.phi;
  }

  /* Product: '<S2>/1//T' incorporates:
   *  Constant: '<S10>/Time constant'
   *  DiscreteIntegrator: '<S14>/Integrator'
   *  Gain: '<S10>/Minimum sampling to time constant ratio'
   *  Inport: '<Root>/phi'
   *  MinMax: '<S10>/MinMax'
   *  Sum: '<S2>/Sum1'
   */
  rtb_uT = 1.0 / fmax(10.0 * rtDW.Probe_f[0], 0.001) * (rtU.phi -
    rtDW.Integrator_DSTATE);

  /* DiscreteIntegrator: '<S19>/Integrator' incorporates:
   *  Inport: '<Root>/theta'
   */
  if (rtDW.Integrator_IC_LOADING_k != 0) {
    rtDW.Integrator_DSTATE_c = rtU.theta;
  }

  if (rtDW.Integrator_PrevResetState_e != 0) {
    rtDW.Integrator_DSTATE_c = rtU.theta;
  }

  /* Product: '<S3>/1//T' incorporates:
   *  Constant: '<S15>/Time constant'
   *  DiscreteIntegrator: '<S19>/Integrator'
   *  Gain: '<S15>/Minimum sampling to time constant ratio'
   *  Inport: '<Root>/theta'
   *  MinMax: '<S15>/MinMax'
   *  Sum: '<S3>/Sum1'
   */
  rtb_uT_n = 1.0 / fmax(10.0 * rtDW.Probe[0], 0.001) * (rtU.theta -
    rtDW.Integrator_DSTATE_c);

  /* Gain: '<S8>/Gain' incorporates:
   *  Inport: '<Root>/theta'
   *  Trigonometry: '<S8>/Sin'
   */
  rtb_Divide = 0.00187395 * cos(rtU.theta);

  /* Trigonometry: '<S7>/Sin' incorporates:
   *  Inport: '<Root>/theta'
   *  Trigonometry: '<S5>/Sin1'
   *  Trigonometry: '<S6>/Sin1'
   */
  rtb_u_tmp = sin(rtU.theta);

  /* Sum: '<S7>/Sum' incorporates:
   *  Constant: '<S7>/Constant'
   *  Gain: '<S7>/Gain'
   *  Math: '<S7>/Square'
   *  Trigonometry: '<S7>/Sin'
   */
  rtb_u = rtb_u_tmp * rtb_u_tmp * 0.000936975 + 0.0053495666666666672;

  /* Trigonometry: '<S6>/Sin' incorporates:
   *  Gain: '<S6>/Gain'
   *  Inport: '<Root>/theta'
   *  Trigonometry: '<S5>/Sin'
   */
  rtb_Product1_i_tmp = sin(2.0 * rtU.theta);

  /* Product: '<S1>/Divide' incorporates:
   *  Constant: '<S5>/Constant'
   *  Constant: '<S5>/Constant1'
   *  Constant: '<S6>/Constant'
   *  Constant: '<S9>/Constant'
   *  Gain: '<S1>/c1'
   *  Gain: '<S1>/c2'
   *  Gain: '<S5>/Gain1'
   *  Gain: '<S6>/Gain1'
   *  Gain: '<S6>/Gain2'
   *  Inport: '<Root>/theta'
   *  Math: '<S4>/Square'
   *  Math: '<S5>/Square'
   *  Math: '<S6>/Square'
   *  Product: '<S1>/Product'
   *  Product: '<S1>/Product1'
   *  Product: '<S1>/Product2'
   *  Product: '<S4>/Product'
   *  Product: '<S5>/Product'
   *  Product: '<S5>/Product1'
   *  Product: '<S6>/Product'
   *  Sum: '<S1>/Sum1'
   *  Sum: '<S1>/Sum2'
   *  Sum: '<S4>/Sum'
   *  Sum: '<S5>/Sum'
   *  Sum: '<S6>/Sum'
   *  Trigonometry: '<S6>/Sin'
   */
  rtb_Divide = ((((0.000936975 * rtb_Product1_i_tmp * rtb_uT_n * rtb_uT +
                   -0.00187395 * rtb_u_tmp * (rtb_uT_n * rtb_uT_n)) + 0.001 *
                  rtb_uT) * rtb_Divide - ((-0.0004684875 * rtb_Product1_i_tmp *
    (rtb_uT * rtb_uT) + -0.11860290000000001 * rtb_u_tmp) + 0.0008 * rtb_uT_n) *
                 rtb_u) + (2777.0 * rtU.theta + 95.0 * rtb_uT_n) * (0.0012493 *
    rtb_u - rtb_Divide * rtb_Divide)) / rtb_Divide;

  /* Saturate: '<S1>/Saturation' */
  if (rtb_Divide > 0.635) {
    /* Outport: '<Root>/u' */
    rtY.u = 0.635;
  } else if (rtb_Divide < -0.635) {
    /* Outport: '<Root>/u' */
    rtY.u = -0.635;
  } else {
    /* Outport: '<Root>/u' */
    rtY.u = rtb_Divide;
  }

  /* End of Saturate: '<S1>/Saturation' */

  /* Update for DiscreteIntegrator: '<S14>/Integrator' */
  rtDW.Integrator_IC_LOADING = 0U;
  rtDW.Integrator_DSTATE += 0.001 * rtb_uT;
  rtDW.Integrator_PrevResetState = 0;

  /* Update for DiscreteIntegrator: '<S19>/Integrator' */
  rtDW.Integrator_IC_LOADING_k = 0U;
  rtDW.Integrator_DSTATE_c += 0.001 * rtb_uT_n;
  rtDW.Integrator_PrevResetState_e = 0;
}

/* Model initialize function */
void controller_initialize(void)
{
  /* Start for Probe: '<S15>/Probe' */
  rtDW.Probe[0] = 0.001;
  rtDW.Probe[1] = 0.0;

  /* Start for Probe: '<S10>/Probe' */
  rtDW.Probe_f[0] = 0.001;
  rtDW.Probe_f[1] = 0.0;

  /* InitializeConditions for DiscreteIntegrator: '<S14>/Integrator' */
  rtDW.Integrator_IC_LOADING = 1U;

  /* InitializeConditions for DiscreteIntegrator: '<S19>/Integrator' */
  rtDW.Integrator_IC_LOADING_k = 1U;
}

/* Test if value is infinite */
static boolean_T rtIsInf(real_T value)
{
  return (boolean_T)isinf(value);
}

/* Test if single-precision value is infinite */
static boolean_T rtIsInfF(real32_T value)
{
  return (boolean_T)isinf(value);
}

/* Test if value is not a number */
static boolean_T rtIsNaN(real_T value)
{
  return (boolean_T)(isnan(value) != 0);
}

/* Test if single-precision value is not a number */
static boolean_T rtIsNaNF(real32_T value)
{
  return (boolean_T)(isnan(value) != 0);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
