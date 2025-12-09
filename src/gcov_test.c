/*
 * Copyright (c) 2023-2024, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "gcov_test.h"
#include <stdio.h>
#pragma GCC optimize("O3,inline")

/* Used to consume results so the optimizer cannot throw away calls */
static volatile int gcov_sink = 0;

static int Calc(int i, int j)
{
  int res;
  if (i == 0)
  {
    res = i + 1;           /* branch 1 */
  }
  else if (i > 0 && j < 10)
  {
    res = i + j;           /* branch 2 */
  }
  /* we always make sure 'res' has been assigned before returning */
  return res;
}

static int Value(int i)
{
  if (i == 3)
  {
    return 5;              /* branch 1 */
  }
  return 1;                /* branch 2 */
}

static void Test2(int *p)
{
  if (*p != 0)
  {
    if (Value(*p) == 5)
    {
      printf("value is 5\n");
      *p = 0;
    }
  }
}

void gcov_test(int i)
{
  int j;
  int tmp;

  (void)i; /* we don’t actually care about the input value here */

  /* 1) Exercise Calc() path i == 0 */
  tmp = Calc(0, 0);
  gcov_sink += tmp;

  /* 2) Exercise Calc() path i > 0 && j < 10 */
  j = Calc(1, 2);
  gcov_sink += j;

  /* 3) Test2 with *p != 0 and Value(*p) == 5 */
  Test2(&j);               /* j was 3, so Value(3) == 5 and j becomes 0 */

  /* 4) Test2 with *p == 0 -> outer if false path */
  Test2(&j);               /* j is 0 now, so nothing inside runs */

  /* 5) Test2 with *p != 0 and Value(*p) != 5 */
  j = 4;                   /* Value(4) == 1 */
  Test2(&j);               /* outer if true, inner if false */

  /* 6) j == 0 branch */
  j = 0;
  if (j == 0)
  {
    printf("j is zero!\n");
  }
  else
  {
    printf("j is not zero!\n");
  }

  /* 7) j != 0 branch */
  j = 1;
  if (j == 0)
  {
    printf("j is zero!\n");
  }
  else
  {
    printf("j is not zero!\n");
  }

  /* 8) Directly exercise Value()'s "else" return-path and keep result */
  tmp = Value(10);         /* this hits 'return 1;' in Value() */
  gcov_sink += tmp;
}
