//
// Created by najamhaq on 2026-01-18.
//

#include "unity.h"
#include <stdint.h>

/*
 * Add your test function declarations here.
 * Each test is a void function with no parameters.
 */
void test_sanity_builds_and_runs(void);

/* Optional hooks (Unity will call these around each test) */
void setUp(void) {}
void tearDown(void) {}

/* ---- Tests ---- */
void test_sanity_builds_and_runs(void) {
  /* This proves the test runner works and the binary executes. */
  TEST_ASSERT_TRUE(1);
}

int main(void) {
  UNITY_BEGIN();

  /* Register tests here */
  RUN_TEST(test_sanity_builds_and_runs);

  return UNITY_END();
}
