#ifndef MY_PACKETS
#define MY_PACKETS
#endif

// Be sure to use data types that specify the number of bits, such as uint8_t.
// For floating point values, do not use long, use float.
// Making the struct packed prevents the compiler from adding any sort of padding between variables.
// Otherwise, you would need to order the variables from biggest size to smallest in the struct.
typedef struct __attribute__((packed)) struct_message {
    char message[32];
} struct_message;