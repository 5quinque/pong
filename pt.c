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
    int Clk;
    int Q;
    int NQ;
};

void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D);
void JKF(int Clk, int J, int K, struct CQNQ *JKF);

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
━│B        A│━
 │          │
━│R1      NC│━
 │          │
━│R2      QA│━
 │          │
━│NC      QD│━
 │          │
━│VCC    GND│━
 │          │
━│NC      QB│━
 │          │
━│NC      QC│━
 └──────────┘
*/

void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D) {
  JKF(Clk, 1, 1, A);
  JKF(A->Q, 1, 1, B);
  JKF(B->Q, 1, 1, C);
  JKF(C->Q, 1, 1, D);
}

int main() {
  //struct timespec ts = { .tv_sec = 0, .tv_nsec = 100000000L };
  //
  // 1 second   / 7.1575MHz
  // 1000000000 / 7157500 = 139.713587146
  //struct timespec ts = { .tv_sec = 0, .tv_nsec = 139.713587146 };
  struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000L };
  int cycles = 64;
  int Clk = 0;

  struct CQNQ A = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ B = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ C = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ D = { .Clk = 0, .Q = 0, .NQ = 0 };

  int count = 0;

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;

    count = A.Q | ((B.Q << 1) | (C.Q << 2) | (D.Q << 3));
    if (!Clk) {
      printf("%d\t%d%d%d%d\n", count, D.Q, C.Q, B.Q, A.Q);
    }
    
    FBRC(Clk, &A, &B, &C, &D);
            
    nanosleep(&ts, NULL);
  }

  return 0;
}

