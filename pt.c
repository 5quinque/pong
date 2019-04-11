#include <stdio.h>
#include <time.h>
#include <string.h> // strcmp

//void FBRC(int Clk, int *Count);
void JKF(int Clk, int J, int K, int *Q, int *NQ);
void SRF(int NS, int NR, int *Q, int *NQ);
int ATI(char *action);

// Lawn Tennis
// http://www.pong-story.com/LAWN_TENNIS.pdf
// Page 19
//
// 4 bit ripple
// http://www.falstad.com/circuit/e-counter.html
//
// d flip flop
// https://en.wikipedia.org/wiki/Flip-flop_(electronics)#/media/File:Edge_triggered_D_flip_flop.svg
//
// jk flip flip
// http://hyperphysics.phy-astr.gsu.edu/hbase/Electronic/jkflipflop.html
// https://www.electronicshub.org/wp-content/uploads/2015/05/jk.jpg
// https://www.electronics-tutorials.ws/sequential/seq_2.html
// https://stackoverflow.com/questions/20779097/programming-a-ripple-counter-in-c-with-jk-flip-flops
//
// https://en.wikipedia.org/wiki/Logic_gate
//
//

/* JK Flip-Flop
 *    ______________________
 *   |           _ _ _ _ _  |
 *   | ____     |         | |
 *   -|    \    |         | |
 * J--|     |o------NS    |------Q
 *   -|____/    |         | |  |
 *   |__        |         | |  |
 *      |       | SR Flip | |  |
 * Clk--|       |   Flop  | |  |
 *    __|       |         | |  |
 *   | ____     |         | |  |
 *   -|    \    |         | |  |
 * K--|     |o------NR    |------NQ
 *   -|____/    |         |    |
 *   |          |_ _ _ _ _|    |
 *   |_________________________|
 *
 */
void JKF(int Clk, int J, int K, int *Q, int *NQ) {
  int NSA = 0, NRA = 0;
  int NSB = 0, NRB = 0;

  int NP3 = 0, NNP3 = 0;

  NSA = !((Clk & J) & *NQ);
  NRA = !((Clk & K) & *Q);
  SRF(NSA, NRA, &NP3, &NNP3);

  NSB = !(NP3 & (!Clk));
  NRB = !(NNP3 & (!Clk));
  SRF(NSB, NRB, Q, NQ);
  
  NSA = !((Clk & J) & *NQ);
  NRA = !((Clk & K) & *Q);
  SRF(NSA, NRA, &NP3, &NNP3);

  NSB = !(NP3 & (!Clk));
  NRB = !(NNP3 & (!Clk));
  SRF(NSB, NRB, Q, NQ);

  //printf("NS %d NR %d\n", NS, NR);

  
  //NS = !((Clk & J) & *NQ);
  //NR = !((Clk & K) & *Q); 
  //SRF(NS, NR, Q, NQ);

  //NS = !((Clk & J) & *NQ);
  //NR = !((Clk & K) & *Q); 
  //SRF(NS, NR, Q, NQ);

}
/*
void JKF(int Clk, int J, int K, int *Q, int *NQ) {
  if(Clk&J&(!K)) {
    *Q = 1;
    *NQ = 0;
  }
  
  if(Clk&K&(!J)) {
    *Q = 0;
    *NQ = 1;
  }

  if(Clk&J&K) {
    *Q = *NQ;
    *NQ = !*Q;
  }
}
*/

/* SR Flip-Flop
 *      ____
 * NS--|    \
 *     |     |o------Q
 *   |-|____/    |
 *    \   ______/
 *     \ /
 *      X
 *     / \______
 *    / ____    \
 *   |-|    \    |
 *     |     |o------NQ
 * NR--|____/
 *
 * Truth Table
 *  S R Q NQ
 *  1 0 0 1
 *  1 1 0 1
 *  0 1 1 0
 *  1 1 1 0
 *  0 0 1 1
 *
 */
void SRF(int NS, int NR, int *Q, int *NQ) {
  *Q = !(NS & *NQ);
  *NQ = !(NR & *Q);
  *Q = !(NS & *NQ);
}

void FBRC(
        int Clk,
        int *Q1,
        int *NQ1,
        int *Q2,
        int *NQ2,
        int *Q3,
        int *NQ3,
        int *Q4,
        int *NQ4
        ) {
  JKF(Clk, 1, 1, Q1, NQ1);
  JKF(*Q1, 1, 1, Q2, NQ2);
  JKF(*Q2, 1, 1, Q3, NQ3);
  JKF(*Q3, 1, 1, Q4, NQ4);
  printf("%d %d%d%d%d\n", Clk, *Q4, *Q3, *Q2, *Q1);
  //printf("%d%d%d%d\n", Clk, *NQ4, *NQ3, *NQ2, *NQ1);
  //printf("%d%d\n", *Q2, *Q1);
}

void debug(size_t n, int* input, char* action) {
  int tmp = 0;

  for (unsigned int i = 0; i < n; i++) {
    if (i == n-1) {
      printf("%d ", input[i]);
    } else {
      printf("%d %s ", input[i], action);
    }

    if (i == 0) {
        tmp = input[i];
        continue;
    }

    switch (ATI(action)) {
      case 1:
      case 2:
        tmp &= input[i];
        break;
      case 3:
      case 4:
        tmp |= input[i];
        break;
      case 5:
      case 6:
        tmp ^= input[i];
        break;
    }
  }

  switch (ATI(action)) {
    case 1:
    case 3:
    case 5:
      printf(" = %d\n", tmp);
      break;
    case 2:
    case 4:
    case 6:
      printf(" = %d\n", !tmp);
      break;
  }
}

int ATI(char* action) {
  if (strcmp(action, "AND") == 0) {
      return 1;
  } else if (strcmp(action, "NAND") == 0) {
      return 2;
  } else if (strcmp(action, "OR") == 0) {
      return 3;
  } else if (strcmp(action, "NOR") == 0) {
      return 4;
  } else if (strcmp(action, "XOR") == 0) {
      return 5;
  } else if (strcmp(action, "XNOR") == 0) {
      return 6;
  }
  return -1;
}

int main() {
  struct timespec ts = { .tv_sec = 0, .tv_nsec = 100000000L };
  int cycles = 9;
  int Clk = 0;

  int Q = 0, NQ = 0;
  int QA = 0, NQA = 0;
  int QB = 0, NQB = 0;
  int QC = 0, NQC = 0;
  int QD = 0, NQD = 0;

  int input[] = {1, 1, 1};
  size_t n = sizeof(input)/sizeof(input[0]);
  debug(n, input, "XOR");

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;
    
    /*
    FBRC(
            Clk,
            &Q1,
            &NQ1,
            &Q2,
            &NQ2,
            &Q3,
            &NQ3,
            &Q4,
            &NQ4
            );
            */

            
    //SRF(1, 0, &Q, &NQ); // 1 0 0 1
    //printf("%d %d %d %d\n", 1, 0, Q, NQ);

    //JKF(Clk, 1, 1, &Q, &NQ);
    //printf("%d %d %d %d\n", 1, 1, Q, NQ);

    //JKF(QA, 1, 1, &QB, &NQB);
    //printf("%d\t%d%d\n", Clk, QA, NQA);

    //JKF(QA, 1, 1, &QB, &NQB);
    //printf("%d\t%d%d\n", Clk, QB, QA);
    //printf("%d\t%d%d\n\n", Clk, NQA, NQB);

    //JKF(Clk, 1, 1, &Q, &NQ);
    //printf("%d\t%d%d\n", Clk, Q, NQ);

    nanosleep(&ts, NULL);
  }

  return 0;
}


// -|>∘-     !x
// =D-        x & y
// =D∘-     !(x & y)
// =>_>-      x | y
// =>_>∘-   !(x | y)
// =>>_>-     x ^ y
// =>>_>∘-  !(x ^ y) 

