#### Finite-state machine sketch

##### ZTest.cpp log
```
[10:55:25.228451] [thread 3348] InitFSMMatrices() No trading policies     ###(InitFSMmatrices:../FSM.cpp:147)
[10:55:25.228486] [thread 3348] ENGINE_STATE_ENTER     ###(main:../ZTest.cpp:17)
[10:55:25.228501] [thread 3348] ENGINE_ACTION_NOACTION     ###(main:../ZTest.cpp:18)
[10:55:25.228505] [thread 3348] ENGINE_ACTION_LIMIT_SELL_PLACE     ###(AddEngineAction:../Engine.cpp:15)
[10:55:25.228509] [thread 3348] ENGINE_ACTION_LIMIT_SELL_PLACE filtered     ###(AddEngineAction:../Engine.cpp:17)
[10:55:25.228512] [thread 3348] ENGINE_ACTION_LIMIT_SELL_PLACE - OK     ###(Execute:../Connectors/Plaza2/ExecutionPlaza2.cpp:43)
[10:55:25.228516] [thread 3348] ENGINE_ACTION_LIMIT_SELL_PLACE     ###(AddEngineAction:../Engine.cpp:15)
[10:55:25.228519] [thread 3348] ENGINE_ACTION_NOACTION filtered     ###(AddEngineAction:../Engine.cpp:17)
[10:55:25.240587] [thread 3354] SYSTEM_ACTION_LIMIT_SELL_PLACE_SUCCESS     ###(AddSystemAction:../Engine.cpp:23)
[10:55:25.240617] [thread 3354] ENGINE_STATE_LIMIT_SELL     ###(AddSystemAction:../Engine.cpp:25)
```
