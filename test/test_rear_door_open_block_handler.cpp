/**
 * @file test_rear_door_open_block_handler.cpp
 * @brief Unit tests for F-04 RearDoorOpenBlockHandler.
 *
 * @details These tests cover UC-4 positive, alternative, and fault-oriented
 *          behavior using the public F-04 API.
 *
 * @traceability SDD-F-04
 * @req_id REQ-SW-CL-004
 * @asil ASIL-TBD
 * @note Related UC: UC-4
 */

#include <gtest/gtest.h>
#include <array>
#include <cstring>

extern "C" {
#include "childlock/rear_door_open_block_handler.h"
}

class RearDoorOpenBlockHandlerTest : public ::testing::Test
{
protected:
    RearDoorOpenBlockHandler_Input_t input;
    RearDoorOpenBlockHandler_Output_t output;

    void SetUp() override
    {
        input.rearDoorInnerHandleEvent = false;
        input.currentClState = CL_STATE_OFF;
        input.doorStateFeedback = RDOBH_DOOR_FEEDBACK_CLOSED;
    }
};

TEST_F(RearDoorOpenBlockHandlerTest, NoHandleEventProducesNoAction)
{
    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, &output);

    ASSERT_TRUE(handled);
    EXPECT_FALSE(output.openRequestBlock);
    EXPECT_EQ(RDOBH_DRIVER_NOTICE_NONE, output.driverNotice);
    EXPECT_EQ(RDOBH_RESULT_NO_EVENT, output.blockResult);
}

TEST_F(RearDoorOpenBlockHandlerTest, HandleEventWithChildLockOffAllowsOpenRequest)
{
    input.rearDoorInnerHandleEvent = true;
    input.currentClState = CL_STATE_OFF;
    input.doorStateFeedback = RDOBH_DOOR_FEEDBACK_OPENED;

    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, &output);

    ASSERT_TRUE(handled);
    EXPECT_FALSE(output.openRequestBlock);
    EXPECT_EQ(RDOBH_DRIVER_NOTICE_NONE, output.driverNotice);
    EXPECT_EQ(RDOBH_RESULT_ALLOWED, output.blockResult);
}

TEST_F(RearDoorOpenBlockHandlerTest, HandleEventWithChildLockOnBlocksOpenRequest)
{
    input.rearDoorInnerHandleEvent = true;
    input.currentClState = CL_STATE_ON;
    input.doorStateFeedback = RDOBH_DOOR_FEEDBACK_CLOSED;

    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, &output);

    ASSERT_TRUE(handled);
    EXPECT_TRUE(output.openRequestBlock);
    EXPECT_EQ(RDOBH_DRIVER_NOTICE_BLOCKED, output.driverNotice);
    EXPECT_EQ(RDOBH_RESULT_BLOCKED, output.blockResult);
}

TEST_F(RearDoorOpenBlockHandlerTest, DoorOpenedDespiteBlockReportsFailure)
{
    input.rearDoorInnerHandleEvent = true;
    input.currentClState = CL_STATE_ON;
    input.doorStateFeedback = RDOBH_DOOR_FEEDBACK_OPENED;

    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, &output);

    ASSERT_TRUE(handled);
    EXPECT_TRUE(output.openRequestBlock);
    EXPECT_EQ(RDOBH_DRIVER_NOTICE_BLOCK_FAILURE, output.driverNotice);
    EXPECT_EQ(RDOBH_RESULT_BLOCK_FAILED, output.blockResult);
}

TEST_F(RearDoorOpenBlockHandlerTest, InvalidChildLockStateFallsBackToSafeBlock)
{
    std::array<unsigned char, sizeof(ChildLockState_t)> invalidRawStateBytes;
    invalidRawStateBytes.fill(0xFFU);

    input.rearDoorInnerHandleEvent = true;
    std::memcpy(&input.currentClState, invalidRawStateBytes.data(), sizeof(input.currentClState));
    input.doorStateFeedback = RDOBH_DOOR_FEEDBACK_UNKNOWN;

    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, &output);

    ASSERT_TRUE(handled);
    EXPECT_TRUE(output.openRequestBlock);
    EXPECT_EQ(RDOBH_DRIVER_NOTICE_BLOCK_FAILURE, output.driverNotice);
    EXPECT_EQ(RDOBH_RESULT_SAFE_BLOCKED, output.blockResult);
}

TEST_F(RearDoorOpenBlockHandlerTest, NullInputIsRejected)
{
    const bool handled = RearDoorOpenBlockHandler_HandleEvent(nullptr, &output);

    EXPECT_FALSE(handled);
}

TEST_F(RearDoorOpenBlockHandlerTest, NullOutputIsRejected)
{
    const bool handled = RearDoorOpenBlockHandler_HandleEvent(&input, nullptr);

    EXPECT_FALSE(handled);
}
