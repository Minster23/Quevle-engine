// Debug.h

#pragma once

// Uncomment untuk mengaktifkan debug
#define DEBUG_MODE

#ifdef DEBUG_MODE
    #include <iostream>
    #define DEBUG_PRINT(x) std::cout << "[DEBUG] " << x << std::endl
#else
    #define DEBUG_PRINT(x)
#endif
