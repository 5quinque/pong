#include <stdio.h>
#include <time.h>

//void FBRC(int Clk, int *Count);
void JKF(int Clk, int J, int K, int *Q, int *NQ);
void SRF(int NS, int NR, int *Q, int *NQ);

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
  int NS1 = 0, NR1 = 0;
  int NS2 = 0, NR2 = 0;
  int NS3 = 0, NR3 = 0;

  /* Original attempt */
  /*
  NS = !((Clk & J) & *NQ);
  NR = !((Clk & K) & *Q); 
  SRF(NS, NR, Q, NQ);
  */

  NS1 = !((Clk & J) & *NQ);
  NR1 = !((Clk & K) & *Q); 
  SRF(NS1, NR1, &NS2, &NR2);

  NS3 = !(NS2 & (!Clk));
  NR3 = !(NR2 & (!Clk));
  SRF(NS3, NR3, Q, NQ);


  printf("Clk %d Q %d NQ %d\n", Clk, *Q, *NQ);

  //NS = !((Clk & J) & *NQ);
  //NR = !((Clk & K) & *Q); 
  //SRF(NS, NR, Q, NQ);

  //NS = !((Clk & J) & *NQ);
  //SRF(NS, NR, Q, NQ);

  //NS = !((Clk & J) & *NQ);
  //NR = !((Clk & K) & *Q); 
  ////
  //SRF(NS, NR, Q, NQ);
}

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
 */
void SRF(int NS, int NR, int *Q, int *NQ) {
  *Q = !(NS & *NQ);
  *NQ = !(NR & *Q);
  *Q = !(NS & *NQ);
  //*NQ = !(NR & *Q);
  //*Q = !(NS & *NQ);
}


void FBRC(
        int Clk,
        int *Count,
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
  //printf("%d%d%d%d\n", *Q4, *Q3, *Q2, *Q1);
  printf("%d%d\n", *Q2, *Q1);
}

int main() {
  struct timespec ts;
  int cycles = 10;
  int Clk = 0;

  int Count = 0;
  int Q = 0, NQ = 0;
  int Q1 = 0, NQ1 = 0;
  int Q2 = 0, NQ2 = 0;
  int Q3 = 0, NQ3 = 0;
  int Q4 = 0, NQ4 = 0;

  ts.tv_sec = 0;
  ts.tv_nsec = 100000000L;

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;

    //printf("%d\t%d\n", Clk, Count);
    //JKF(Clk, 1, 1, &Q1, &NQ1);
    //printf("%d\t%d%d\n", Clk, Q1, NQ1);
    //JKF(Q1, 1, 1, &Q2, &NQ2);
    //printf("%d\t%d%d\n", Clk, Q2, Q1);
    
    /*FBRC(
            Clk,
            &Count,
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

    //printf("%d\t%d%d\n", Clk, Q2, Q1);
    JKF(Clk, 1, 1, &Q, &NQ);
    //printf("%d\t%d%d\n", Clk, Q, NQ);
    nanosleep(&ts, NULL); // 1 second 1000000
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

/*
 * 0000
 * 0001
 * 0010
 * 0011
 * 0100
 * 0101
 * 0110
 * 0111
 * 1000
 * 1001
 * 1010
 * 1011
 * 1100
 * 1101
 * 1110
 * 1111
 *
 * 11
 * 01
 * 00
 * 00
 * 11
 * 01
 * 00
 * 00
 * 11
 * 01
 *
 *
 */

