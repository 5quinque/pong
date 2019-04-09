#include <stdio.h>
#include <unistd.h>

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
int JKF(int Clk, int J, int K, int *Q, int *NQ) {
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

int main() {
  int i;
  int cycles = 16;
  int Q = 1, NQ = 0;
  int Clk = 0;

  int NS = 1, NR = 1;

  printf("Clk\tQ\tNQ\n");
  for (i = 0; i < cycles; i++) {
    Clk ^= 1;

    JKF(Clk, 1, 1, &Q, &NQ);
    printf("%d\t%d\t%d\n", Clk, Q, NQ);


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

