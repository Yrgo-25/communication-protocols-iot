# Testsvit för IoT-protokollet

Testsvit innefattande unit-tester skrivna med Google Test:
  * [Makefile](./appendix/Makefile) skapar en körbar fil där biblioteket Google Test, tillsammans med `pthread`, länkas in. `pthread` krävs av Google Test.
  * [main.cpp](./appendix/main.cpp) kör alla kopplade tester.
  * [frame_test.cpp](./test/comm/frame/frame_test.cpp) innehåller unit-tester för frame-strukturen `comm::frame::Frame`. Här ska ett så kallat "happy path"-test för ett status response läggas till.
  * [parser_test.cpp](./test/comm/frame/parser_test.cpp) ska innehålla unit-tester för frame-parsern `comm::frame::Parser` efter slutförd genomgång.

---