/*
 * Copyright (c) 2024-2025, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#include "McuRTOS.h"
#include <stdio.h>
#include <gcov_test.h>
#if PL_CONFIG_USE_GCOV
#include "McuCoverage.h"
#endif

int main(void)
{
  PL_Init();
  gcov_test(1);
#if PL_CONFIG_USE_GCOV

  McuCoverage_WriteFiles(); /* write coverage data files */
#endif                      /* PL_CONFIG_USE_GCOV */

  vTaskStartScheduler();
  for (;;)
  {
    /* do not return from main() */
  }
  return 0;
}
