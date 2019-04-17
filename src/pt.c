/* ℛℒ ⅯⅯⅪⅩ */

/* 
If/Else logic
Q = (A || S) ^ (B || S')
if (S) {
    Q = A;
} else {
    Q = B;
}
*/


#include <stdio.h>
#include <time.h>

struct CQNQ {
    unsigned int Clk : 1;
    unsigned int Q : 1;
    unsigned int NQ : 1;
};

void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D);
void JKF(int Clk, int J, int K, struct CQNQ *JKF);

/* todo do we need `clear` https://components101.com/ics/sn7476-jk-flip-flop */
void JKF(int Clk, int J, int K, struct CQNQ *JKF) {
    /*
    if ((Clk != JKF->Clk) && !Clk) {
        // Reset Q = 0
        if (J == 0 && K == 1) {
            JKF->Q = 0;
        }
        // Set Q = 1
        if (J == 1 && K == 0) {
            JKF->Q = 1;
        }
        // Toggle
        if (J == 1 && K == 1) {
            JKF->Q = !JKF->Q;
        }
    }
    */
    
    JKF->Q = ((J && !K) || // Reset Q = 0
        ((J && K) && JKF->NQ) || // Toggle
        (!JKF->Q && !JKF->NQ) ||  // Set Q = 1
        (!(Clk ^ !JKF->Clk) && !Clk)) ^ // if ((Clk != JKF->Clk) && !Clk)
        (JKF->Q || !(!(Clk ^ !JKF->Clk) && !Clk)); // else

    JKF->NQ = !JKF->Q;

    JKF->Clk = Clk;
}

/*
          7493
      ┌──────────┐
A->Q ━│B        A│━ Clk
      │          │
     ━│R1      NC│━ -
      │          │
     ━│R2      QA│━ 1H A->Q
      │          │
     ━│NC      QD│━ 8H D->Q
      │          │
     ━│VCC    GND│━
      │          │
     ━│NC      QB│━ 2H B->Q
      │          │
     ━│NC      QC│━ 4H C->Q
      └──────────┘

 Convert FBRC to decimal
 count = A.Q | ((B.Q << 1) | (C.Q << 2) | (D.Q << 3));
 printf("%d\t%d%d%d%d\n", count, D.Q, C.Q, B.Q, A.Q);

*/
void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D) {
  JKF(Clk, 1, 1, A);
  JKF(A->Q, 1, 1, B);
  JKF(B->Q, 1, 1, C);
  JKF(C->Q, 1, 1, D);
}

void HSync(int Clk) {
  struct CQNQ A = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ B = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ C = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ D = { .Clk = 0, .Q = 0, .NQ = 0 };

  struct CQNQ A2 = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ B2 = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ C2 = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ D2 = { .Clk = 0, .Q = 0, .NQ = 0 };

  struct CQNQ F6 = { .Clk = 0, .Q = 0, .NQ = 0 };
  
  int F7 = 0;

  FBRC(Clk, &A, &B, &C, &D);
  FBRC(D.Q, &A2, &B2, &C2, &D2);

  // F6
  JKF(C2.Q, 1, 1, &F6);

  // 8 input NAND 7430
  F7 = !(B.Q && C.Q && C2.Q && D2.Q && F6.Q);

  printf("F7: %d\n", F7);
}

int main() {
  // 20000000L = 50hz
  struct timespec ts = { .tv_sec = 0, .tv_nsec = 20000000L };
  int cycles = 456;
  int Clk = 0;

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;

    HSync(Clk);
     
    nanosleep(&ts, NULL);
  }

  return 0;
}

