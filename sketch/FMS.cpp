
#include "FMS.h"

namespace HFTE
{
	EngineState engineStateTransitionsMatrix[ENGINE_STATES_COUNT][ENGINE_ACTIONS_COUNT][SYSTEM_ACTIONS_COUNT];
	EngineAction engineActionFilterMatrix[ENGINE_STATES_COUNT][ENGINE_ACTIONS_COUNT][ENGINE_ACTIONS_COUNT];

	/*
	 * Initialization only switches
	 */
	EngineState SwitchEngineState(EngineState engineState, EngineAction engineAction, SystemAction systemAction)
	{
		switch (systemAction)
		{
		case SYSTEM_ACTION_LIMIT_SELL_FILLED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_PLACE_SUCCESS: {
			if (engineAction == ENGINE_ACTION_LIMIT_SELL_PLACE)
				switch (engineState)
				{
				case ENGINE_STATE_ENTER: return ENGINE_STATE_LIMIT_SELL;
				case ENGINE_STATE_LIMIT_BUY: return ENGINE_STATE_LIMIT_SELLBUY;
				case ENGINE_STATE_TRADE_BUY: return ENGINE_STATE_LIMIT_SELL_COVER;
				default: break;
				}
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_PLACE_FAILED: {
			if (engineAction == ENGINE_ACTION_LIMIT_SELL_PLACE)
				switch (engineState)
				{
				case ENGINE_STATE_ENTER: return ENGINE_STATE_ENTER;
				case ENGINE_STATE_LIMIT_BUY: return ENGINE_STATE_LIMIT_BUY;
				case ENGINE_STATE_TRADE_BUY: return ENGINE_STATE_TRADE_BUY;
				default: break;
				}
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_MOVE_SUCCESS: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_MOVE_FAILED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_CANCEL_SUCCESS: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_SELL_CANCEL_FAILED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_CANCEL_SUCCESS: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_CANCEL_FAILED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_MOVE_SUCCESS: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_MOVE_FAILED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_PLACE_SUCCESS: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_PLACE_FAILED: {
			break;
		}
		case SYSTEM_ACTION_LIMIT_BUY_FILLED: {
			break;
		}
		default: break;
		}

		return ENGINE_STATE_NOSTATE;
	}
	EngineAction SwitchEngineAction(std::set<TradePolicy> &policies, EngineState engineState, EngineAction engineAction)
	{
		EngineAction action = EngineAction::ENGINE_ACTION_NOACTION;

		switch (engineState)
		{
		case ENGINE_STATE_NOSTATE: {
			break;
		}
		case ENGINE_STATE_ENTER: {
			if ((engineAction == ENGINE_ACTION_LIMIT_SELL_PLACE &&
				policies.find(TRADE_POLICY_ONLY_BUY) == policies.end()) ||
				(engineAction == ENGINE_ACTION_LIMIT_BUY_PLACE &&
				policies.find(TRADE_POLICY_ONLY_SELL) == policies.end()))
					action = engineAction;
			break;
		}
		case ENGINE_STATE_LIMIT_SELL: {
			if ((engineAction == ENGINE_ACTION_LIMIT_BUY_PLACE &&
				policies.find(TRADE_POLICY_NO_BIDIRECTION) == policies.end()) ||
				(engineAction == ENGINE_ACTION_LIMIT_SELL_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_SELL_CANCEL)) action = engineAction;
			break;
		}
		case ENGINE_STATE_TRADE_SELL: {
			if (engineAction == ENGINE_ACTION_LIMIT_BUY_PLACE) action = engineAction;
			break;
		}
		case ENGINE_STATE_LIMIT_BUY_COVER: {
			if (engineAction == ENGINE_ACTION_LIMIT_BUY_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_BUY_CANCEL) action = engineAction;
			break;
		}
		case ENGINE_STATE_LIMIT_SELLBUY: {
			if (engineAction == ENGINE_ACTION_LIMIT_SELL_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_SELL_CANCEL ||
				engineAction == ENGINE_ACTION_LIMIT_BUY_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_BUY_CANCEL) action = engineAction;
			break;
		}
		case ENGINE_STATE_LIMIT_BUY: {
			if (engineAction == ENGINE_ACTION_LIMIT_BUY_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_BUY_CANCEL) action = engineAction;
			break;
		}
		case ENGINE_STATE_TRADE_BUY: {
			if (engineAction == ENGINE_ACTION_LIMIT_SELL_PLACE) action = engineAction;
			break;
		}
		case ENGINE_STATE_LIMIT_SELL_COVER: {
			if (engineAction == ENGINE_ACTION_LIMIT_SELL_MOVE ||
				engineAction == ENGINE_ACTION_LIMIT_SELL_CANCEL) action = engineAction;
			break;
		}
		case ENGINE_STATE_EXIT: {
			break;
		}
		default: break;
		}

		return action;
	}

	/*
	 * Matricies initialization
	 */
	void InitFMSMatrices(std::set<TradePolicy> policies)
	{
		if (policies.size() == 0) LOGD("InitFMSMatrices() No trading policies");
		for (auto policy : policies) {
			LOGD("InitFMSMatrices() Adding trading policy {}", GetTradePolicyString(policy));
			assert(policy < TRADE_POLICIES_COUNT);
		}

		for (int engineState = ENGINE_STATE_NOSTATE; engineState < ENGINE_STATES_COUNT; engineState++)
		for (int engineAction = ENGINE_ACTION_NOACTION; engineAction < ENGINE_ACTIONS_COUNT; engineAction++)
		{
			for (int engineActionCurrent = ENGINE_ACTION_NOACTION; engineActionCurrent < ENGINE_ACTIONS_COUNT; engineActionCurrent++)
			{
				if (engineActionCurrent == ENGINE_ACTION_NOACTION)
				{
					engineActionFilterMatrix
						[EngineState(engineState)]
						[EngineAction(engineActionCurrent)]
						[EngineAction(engineAction)] =
							SwitchEngineAction(
								policies,
								EngineState(engineState),
								EngineAction(engineAction));
				} else {
					engineActionFilterMatrix
						[EngineState(engineState)]
						[EngineAction(engineActionCurrent)]
						[EngineAction(engineAction)] = ENGINE_ACTION_NOACTION;
				}
			}

			for (int systemAction = SYSTEM_ACTION_NOACTION; systemAction < SYSTEM_ACTIONS_COUNT; systemAction++)
			{
				engineStateTransitionsMatrix
					[EngineState(engineState)]
					[EngineAction(engineAction)]
					[SystemAction(systemAction)] =
						SwitchEngineState(
							EngineState(engineState),
							EngineAction(engineAction),
							SystemAction(systemAction));
			}
		}
	}
}
