/**
 * @file    F09_RearSeatOccupancyAlert.h
 * @brief   F-09: Rear Seat Occupancy Alert Module - Public Interface
 *
 * @details 차량 출발 시 뒷좌석 점유 상태와 차일드 락 상태를 평가하여
 * HMI 알림 유형을 결정합니다. (UC-6)
 *
 * @version 1.0.0
 * @author  AI Model: Gemini
 * @copyright Synetics 20 CopyrightⓒSynetics_
 *
 * @req_id      REQ-SW-ESA-003
 * @asil        ASIL B
 * @traceability DD-CL-F09, UC-6
 *
 * @note    Complies with MISRA C:2012. No use of dynamic memory.
 */

#ifndef F09_REAR_SEAT_OCCUPANCY_ALERT_H
#define F09_REAR_SEAT_OCCUPANCY_ALERT_H

#include "childlock_types.h"

/* =========================================================================
 * Public Types
 * ========================================================================= */

/**
 * @brief F-09 알림 유형 분류 (새로운 WarningMsgId 추가 없이 HMI 분기를 위해 사용)
 */
typedef enum
{
    F09_ALERT_TYPE_NONE           = 0U,
    F09_ALERT_TYPE_RECOMMENDATION = 1U, /**< 차일드 락 ON 권장 알림 */
    F09_ALERT_TYPE_STATUS         = 2U  /**< 차일드 락 현재 ON 상태 알림 */
} F09_AlertType_t;

/**
 * @brief F-09 입력 데이터 구조체
 */
typedef struct
{
    bool             departureEvent;    /**< 차량 출발 이벤트 발생 여부 */
    bool             rearSeatOccupancy; /**< 뒷좌석 점유 여부 */
    ChildLockState_t currentCLState;    /**< 현재 차일드 락 상태 */
    bool             hmiHealth;         /**< HMI 시스템 정상 작동 여부 */
} F09_OccupancyAlertInput_t;

/**
 * @brief F-09 출력 데이터 구조체
 */
typedef struct
{
    F09_AlertType_t alertType;      /**< 결정된 알림 유형 */
    WarningMsgId_t  alertMessageId; /**< HMI에 표시할 특정 메시지 ID */
    bool            eventLog;       /**< 이벤트 로그 기록 요청 플래그 */
} F09_OccupancyAlertOutput_t;

/* =========================================================================
 * Public API
 * ========================================================================= */

/**
 * @brief 차량 출발 시 알림 발생 여부 및 유형을 평가합니다.
 *
 * @param[in]  input   검증된 입력 데이터 포인터. (NULL 불가)
 * @param[out] output  결정된 알림 출력 데이터 포인터. (NULL 불가)
 *
 * @req_id      REQ-SW-ESA-003
 * @asil        ASIL B
 * @traceability DD-CL-F09-RUN
 */
void F09_RearSeatOccupancyAlert_Run(
    const F09_OccupancyAlertInput_t  * const input,
    F09_OccupancyAlertOutput_t       * const output
);

#endif /* F09_REAR_SEAT_OCCUPANCY_ALERT_H */