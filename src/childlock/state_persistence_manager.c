/**
 * @file state_persistence_manager.c
 * @brief Minimal C11 implementation for F-05 StatePersistenceManager.
 *
 * @details The implementation persists the current child lock state during
 *          IGN OFF handling and restores it on IGN ON or reset. Storage and
 *          Door ECU synchronization are injected via callbacks, which keeps
 *          the module deterministic and free of dynamic allocation.
 *
 * @traceability SDD-F-05
 * @asil ASIL-TBD
 * @note Related UC: UC-1, UC-3, UC-7
 */

#include "childlock/state_persistence_manager.h"

#include <stddef.h>

/**
 * @brief Clears the public result structure to safe defaults.
 *
 * @param[out] result Pointer to the result object.
 */
static void StatePersistenceManager_ClearResult(StatePersistenceManager_Result_t *result)
{
    if (result != NULL)
    {
        /* Default to a safe, known child lock state whenever output is reset. */
        result->savedClState = CL_STATE_OFF;
        result->restoredClState = CL_STATE_OFF;
        result->restoreStatus = SPM_RESTORE_STATUS_NOT_REQUESTED;
        result->persistSucceeded = false;
        result->syncRequested = false;
    }
}

/**
 * @brief Validates an externally supplied child lock state value.
 *
 * @param[in] state State value to validate.
 * @return true when the state matches the defined child lock enum.
 */
static bool StatePersistenceManager_IsValidState(ChildLockState_t state)
{
    return ((state == CL_STATE_OFF) || (state == CL_STATE_ON));
}

/**
 * @brief Validates required callbacks before the context is marked ready.
 *
 * @param[in] config Candidate configuration.
 * @return true when all injected dependencies are available.
 */
static bool StatePersistenceManager_HasValidConfig(
    const StatePersistenceManager_Config_t *config)
{
    bool isValid = false;

    if (config != NULL)
    {
        isValid = ((config->writePersistedState != NULL)
            && (config->readPersistedState != NULL)
            && (config->requestDoorEcuSync != NULL));
    }

    return isValid;
}

bool StatePersistenceManager_Init(StatePersistenceManager_t *manager,
                                  const StatePersistenceManager_Config_t *config)
{
    bool isInitialized = false;

    if (manager != NULL)
    {
        manager->isInitialized = false;
    }

    if ((manager != NULL) && (StatePersistenceManager_HasValidConfig(config) == true))
    {
        manager->config = *config;
        manager->isInitialized = true;
        isInitialized = true;
    }

    return isInitialized;
}

bool StatePersistenceManager_HandleIgnitionOff(StatePersistenceManager_t *manager,
                                               ChildLockState_t currentClState,
                                               StatePersistenceManager_Result_t *result)
{
    bool isHandled = false;

    StatePersistenceManager_ClearResult(result);

    if ((manager != NULL) && (result != NULL) && (manager->isInitialized == true))
    {
        if (StatePersistenceManager_IsValidState(currentClState) == true)
        {
            result->savedClState = currentClState;
            result->persistSucceeded =
                manager->config.writePersistedState(currentClState);
            isHandled = true;
        }
    }

    return isHandled;
}

/**
 * @brief Shared restore path for IGN ON and reset.
 *
 * @param[in] manager Pointer to the runtime context.
 * @param[out] result Pointer to the result object.
 * @return true when the request is accepted for processing.
 */
static bool StatePersistenceManager_HandleRestore(StatePersistenceManager_t *manager,
                                                  StatePersistenceManager_Result_t *result)
{
    bool isHandled = false;
    ChildLockState_t restoredState = CL_STATE_OFF;
    bool isPersistedStateValid = false;
    bool isPersistedStateAvailable = false;

    StatePersistenceManager_ClearResult(result);

    if ((manager != NULL) && (result != NULL) && (manager->isInitialized == true))
    {
        isPersistedStateAvailable = manager->config.readPersistedState(
            &restoredState,
            &isPersistedStateValid);

        if (isPersistedStateAvailable == false)
        {
            result->restoreStatus = SPM_RESTORE_STATUS_UNAVAILABLE;
        }
        else if ((isPersistedStateValid == false)
            || (StatePersistenceManager_IsValidState(restoredState) == false))
        {
            result->restoreStatus = SPM_RESTORE_STATUS_INVALID;
        }
        else
        {
            result->restoredClState = restoredState;
            result->restoreStatus = SPM_RESTORE_STATUS_RESTORED;

            /* Re-sync Door ECU only after a validated restore decision. */
            manager->config.requestDoorEcuSync(restoredState);
            result->syncRequested = true;
        }

        isHandled = true;
    }

    return isHandled;
}

bool StatePersistenceManager_HandleIgnitionOn(StatePersistenceManager_t *manager,
                                              StatePersistenceManager_Result_t *result)
{
    return StatePersistenceManager_HandleRestore(manager, result);
}

bool StatePersistenceManager_HandleReset(StatePersistenceManager_t *manager,
                                         StatePersistenceManager_Result_t *result)
{
    return StatePersistenceManager_HandleRestore(manager, result);
}
