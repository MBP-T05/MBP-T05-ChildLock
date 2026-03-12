/**
 * @file    F08_RearRiskProtectionController.h
 * @brief   F-08: Rear Risk Protection Controller Module - Public Interface
 *
 * @details Receives risk evaluation results from F-07 and controls the
 *          Child Lock state and HMI warnings to protect exiting passengers
 *          from rear approaching objects (UC-5).
 *
 *          Safety Strategies:
 *            - If Risk = HIGH and CL is OFF: Auto-activate CL (Target = ON).
 *            - If Risk = HIGH and CL is already ON: Maintain ON, issue warning.
 *            - If Sensor Fault: Fallback to SAFE_LOCKED (Maintain CL ON or force ON).
 *
 * @version 1.0.0
 * @date    2026-03-11
 * @author  AI Model: Gemini 3.5 Pro
 * @copyright Synetics 20 CopyrightⓒSynetics_
 *
 * @req_id  REQ-SW-ESA-002
 * @asil    ASIL B
 * @traceability DD-CL-F08, UC-5
 *
 * @note    Complies with MISRA C:2012. No use of dynamic memory.
 */

#ifndef F08_REAR_RISK_PROTECTION_CONTROLLER_H
#define F08_REAR_RISK_PROTECTION_CONTROLLER_H

#include "childlock_types.h"

/* =========================================================================
 * Public API
 * ========================================================================= */

/**
 * @brief Executes the rear risk protection logic for one cycle.
 *
 * @details Maps risk states to physical safety responses:
 *          1. Validates pointers.
 *          2. Checks if ignition is ON (feature is inactive if IGN OFF).
 *          3. If valid high risk exists:
 *             - If CL is OFF -> Target CL ON, ClChanged = true, Trigger Warning.
 *             - If CL is ON  -> Monitor only, Trigger Warning "Already ON".
 *          4. If sensor is faulty (riskValid = false):
 *             - Force Target CL ON (SAFE_LOCKED policy), trigger fault warning.
 *          5. If risk is low/none:
 *             - Target CL remains whatever the current state is. No warnings.
 *
 * @param[in]  input   Pointer to the protection input data. Must not be NULL.
 * @param[out] output  Pointer to the protection output actions. Must not be NULL.
 *
 * @return inline void
 *
 * @req_id      REQ-SW-ESA-002
 * @asil        ASIL B
 * @traceability DD-CL-F08-RUN
 */
void F08_RearRiskProtectionController_Run(
    const RearRiskProtectionInput_t  * const input,
    RearRiskProtectionOutput_t       * const output
);

#endif /* F08_REAR_RISK_PROTECTION_CONTROLLER_H */
