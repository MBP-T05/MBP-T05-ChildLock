/**
 * @file    test_F09_RearSeatOccupancyAlert.cpp
 * @brief   Unit tests for F-09: Rear Seat Occupancy Alert Module
 *
 * @details Tests the logic for generating HMI alerts upon vehicle departure
 * based on rear seat occupancy and child lock state.
 * Covers positive alerts, non-departure paths, and null pointers.
 *
 * @version 1.1.0
 * @date    2026-03-14
 * @author  AI Model: Gemini
 * @copyright Synetics 20 CopyrightⓒSynetics_
 */

#include <gtest/gtest.h>
extern "C" {
#include "F09_RearSeatOccupancyAlert.h"
#include "childlock_types.h"
}

class F09OccupancyAlertTest : public ::testing::Test {
protected:
    F09_OccupancyAlertInput_t input;
    F09_OccupancyAlertOutput_t output;

    void SetUp() override {
        // Safe, default input condition
        input.departureEvent = false;
        input.rearSeatOccupancy = false;
        input.currentCLState = CL_STATE_OFF;
        input.hmiHealth = true;

        // Reset output
        output.alertType = F09_ALERT_TYPE_NONE;
        output.alertMessageId = WARNING_MSG_NONE;
        output.eventLog = false;
    }

    void TearDown() override {
        // Clean up common resources
    }
};

/* =========================================================================
 * 1. Positive Scenarios
 * ========================================================================= */

TEST_F(F09OccupancyAlertTest, Positive_OccupiedAndLockOff_RecommendsLock) {
    input.departureEvent = true;
    input.rearSeatOccupancy = true;
    input.currentCLState = CL_STATE_OFF;

    F09_RearSeatOccupancyAlert_Run(&input, &output);

    EXPECT_EQ(output.alertType, F09_ALERT_TYPE_RECOMMENDATION);
    EXPECT_EQ(output.alertMessageId, WARNING_MSG_NONE);
    EXPECT_TRUE(output.eventLog);
}

TEST_F(F09OccupancyAlertTest, Positive_OccupiedAndLockOn_StatusAlert) {
    input.departureEvent = true;
    input.rearSeatOccupancy = true;
    input.currentCLState = CL_STATE_ON;

    F09_RearSeatOccupancyAlert_Run(&input, &output);

    EXPECT_EQ(output.alertType, F09_ALERT_TYPE_STATUS);
    EXPECT_EQ(output.alertMessageId, WARNING_MSG_CHILD_LOCK_ON);
    EXPECT_TRUE(output.eventLog);
}

TEST_F(F09OccupancyAlertTest, Positive_NotOccupiedDeparture_LogsEventOnly) {
    input.departureEvent = true;
    input.rearSeatOccupancy = false;

    F09_RearSeatOccupancyAlert_Run(&input, &output);

    EXPECT_EQ(output.alertType, F09_ALERT_TYPE_NONE);
    EXPECT_TRUE(output.eventLog);
}

/* =========================================================================
 * 2. Boundary & Fault Scenarios
 * ========================================================================= */

TEST_F(F09OccupancyAlertTest, Fault_HmiUnhealthy_DoNothing) {
    input.departureEvent = true;
    input.rearSeatOccupancy = true;
    input.hmiHealth = false;

    F09_RearSeatOccupancyAlert_Run(&input, &output);

    EXPECT_EQ(output.alertType, F09_ALERT_TYPE_NONE);
    EXPECT_FALSE(output.eventLog);
}

TEST_F(F09OccupancyAlertTest, Boundary_NoDeparture_DoNothing) {
    input.departureEvent = false;
    input.rearSeatOccupancy = true;
    input.hmiHealth = true;

    F09_RearSeatOccupancyAlert_Run(&input, &output);

    EXPECT_EQ(output.alertType, F09_ALERT_TYPE_NONE);
    EXPECT_FALSE(output.eventLog);
}

/* =========================================================================
 * 3. Robustness Scenarios
 * ========================================================================= */

TEST_F(F09OccupancyAlertTest, Robustness_NullPointers) {
    // Should not crash
    F09_RearSeatOccupancyAlert_Run(NULL, &output);
    F09_RearSeatOccupancyAlert_Run(&input, NULL);
    F09_RearSeatOccupancyAlert_Run(NULL, NULL);
    
    SUCCEED();
}