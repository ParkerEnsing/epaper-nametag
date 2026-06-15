#include <unity.h>
#include "EPaperDisplay.h"


// void setUp(void) {
//     // set stuff up here
// }


// void tearDown(void) {
//     // clean stuff up here
// }


void test_epaper_init() {
    EPaperDisplay EPD;
    TEST_ASSERT_EQUAL_UINT16(792, EPD.X);
    TEST_ASSERT_EQUAL_UINT16(272, EPD.Y);
    TEST_ASSERT_EQUAL_UINT8(50, EPD.SOURCE_BYTES);
    TEST_ASSERT_EQUAL_UINT16(272, EPD.GATE_BITS);
    TEST_ASSERT_EQUAL_UINT16(13600, EPD.TOTAL_SCREEN_BYTES);
    TEST_ASSERT_EQUAL_UINT32(20000000, EPD.SPI_MAX_SPEED);
    TEST_ASSERT_EQUAL_UINT8(MSBFIRST, EPD.SPI_DATA_ORDER);
    TEST_ASSERT_EQUAL_UINT8(SPI_MODE2, EPD.SPI_DATA_MODE);
}


void setup() {
    UNITY_BEGIN();

    RUN_TEST(test_epaper_init);

    UNITY_END();
}


void loop() {}


// int main() {
//     UNITY_BEGIN();

//     RUN_TEST(test_epaper_init);

//     UNITY_END();
// }