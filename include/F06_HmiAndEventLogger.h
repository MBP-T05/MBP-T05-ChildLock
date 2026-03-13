/**
 * @file    F06_HmiAndEventLogger.h
 * @brief   F-06: HMI and Event Logger - Public Interface
 *
 * @details Centralized module to handle HMI requests, event logging, and DTCs.
 *
 * @version 1.1.0
 * @date    2026-03-14
 * @author  AI Model: Gemini
 * @copyright Synetics 20 CopyrightⓒSynetics_
 *
 * @req_id      REQ-SW-CL-004
 * @asil        ASIL B
 * @traceability DD-CL-F06
 *
 * @note    Complies with MISRA C:2012. No use of dynamic memory.
 */

#ifndef F06_HMI_AND_EVENT_LOGGER_H
#define F06_HMI_AND_EVENT_LOGGER_H

#include "childlock_types.h"

/* =========================================================================
 * Public Types
 * ========================================================================= */

typedef enum
{
    F06_EVENT_TYPE_NONE          = 0U,
    F06_EVENT_TYPE_STATE_CHANGE  = 1U,
    F06_EVENT_TYPE_SAFETY_ACTION = 2U,
    F06_EVENT_TYPE_ERROR         = 3U
} F06_EventType_t;

typedef void (*F06_HmiDisplayCb_t)(WarningMsgId_t msgId, bool soundReq);
typedef void (*F06_EventLogCb_t)(F06_EventType_t type, ChildLockState_t state);
typedef void (*F06_DtcStoreCb_t)(FaultFlag_t fault);

typedef struct
{
    F06_HmiDisplayCb_t displayCb;
    F06_EventLogCb_t   logCb;
    F06_DtcStoreCb_t   dtcCb;
} F06_HmiLoggerConfig_t;

typedef struct
{
    F06_HmiLoggerConfig_t config;
    bool                  isInitialized;
} F06_HmiLoggerContext_t;

/* =========================================================================
 * Public API
 * ========================================================================= */

bool F06_HmiAndEventLogger_Init(
    F06_HmiLoggerContext_t      * const ctx,
    const F06_HmiLoggerConfig_t * const config
);

void F06_HmiAndEventLogger_Process(
    const F06_HmiLoggerContext_t * const ctx,
    const WarningMsgId_t                 msgId,
    const bool                           soundReq,
    const F06_EventType_t                evtType,
    const FaultFlag_t                    fault,
    const ChildLockState_t               currState
);

#endif /* F06_HMI_AND_EVENT_LOGGER_H */