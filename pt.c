#include <stdio.h>
#include <unistd.h>

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
  int NS = 0, NR = 0;

  NS = !((Clk & J) & *NQ);
  NR = !((Clk & K) & *Q); 

  SRF(NS, NR, Q, NQ);
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
  //printf("%d", *Q1);
  *Count |= *Q1 << 0;
  
  JKF(*Q1, 1, 1, Q2, NQ2);
  //printf("%d", *Q2);
  *Count |= *Q2 << 1;

  JKF(*Q2, 1, 1, Q3, NQ3);
  //printf("%d", *Q3);
  *Count |= *Q3 << 2;

  JKF(*Q3, 1, 1, Q4, NQ4);
  //printf("%d", *Q4);
  *Count |= *Q4 << 3;
  //
  printf("%d%d%d%d\n", *Q4, *Q3, *Q2, *Q1);
}

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
int main() {
  int i;
  int cycles = 10;
  int Clk = 0;

  int Count = 0;
  int Q1 = 0, NQ1 = 0;
  int Q2 = 0, NQ2 = 0;
  int Q3 = 0, NQ3 = 0;
  int Q4 = 0, NQ4 = 0;

  for (i = 0; i < cycles; i++) {
    Clk ^= 1;

    //printf("%d\t%d\n", Clk, Count);
    
    JKF(Clk, 1, 1, &Q1, &NQ1);
    JKF(Q1, 1, 1, &Q2, &NQ2);

    printf("%d\t%d%d\n", Clk, Q2, Q1);
    

    /*
    FBRC(
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

    usleep(50000); // 1 second 1000000
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

