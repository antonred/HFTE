#### Finite-state machine sketch

```
[10:52:28.719305] [thread 3124] Logger info test
[10:52:28.719349] [thread 3124] Logger debug test
[10:52:28.719354] [thread 3124] Logger MACRO LOGD TEST     ###(StartLog:../logging/logging.cpp:16)
[10:52:28.719358] [thread 3124] Logger MACRO LOGT TEST     ###(StartLog../logging/logging.cpp #17)
[10:52:28.720377] [thread 3124] InitFSMMatrices() No trading policies     ###(InitFSMmatrices:../FSM.cpp:147)
[10:52:28.720414] [thread 3124] ENGINE_STATE_ENTER     ###(main:../ZTest.cpp:17)
[10:52:28.720419] [thread 3124] ENGINE_ACTION_NOACTION     ###(main:../ZTest.cpp:18)
[10:52:28.720423] [thread 3124] ENGINE_ACTION_LIMIT_SELL_PLACE     ###(AddEngineAction:../Engine.cpp:15)
[10:52:28.720427] [thread 3124] ENGINE_ACTION_LIMIT_SELL_PLACE filtered     ###(AddEngineAction:../Engine.cpp:17)
[10:52:28.720430] [thread 3124] ENGINE_ACTION_LIMIT_SELL_PLACE - OK     ###(Execute:../Connectors/Plaza2/ExecutionPlaza2.cpp:43)
[10:52:28.720434] [thread 3124] ENGINE_ACTION_LIMIT_SELL_PLACE     ###(AddEngineAction:../Engine.cpp:15)
[10:52:28.720437] [thread 3124] ENGINE_ACTION_NOACTION filtered     ###(AddEngineAction:../Engine.cpp:17)
[10:52:28.820503] [thread 3130] SYSTEM_ACTION_LIMIT_SELL_PLACE_SUCCESS     ###(AddSystemAction:../Engine.cpp:23)
[10:52:28.820521] [thread 3130] ENGINE_STATE_LIMIT_SELL     ###(AddSystemAction:../Engine.cpp:25)
```
