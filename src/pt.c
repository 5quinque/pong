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

struct HSYNC {
  struct CQNQ A;
  struct CQNQ B;
  struct CQNQ C;
  struct CQNQ D;

  struct CQNQ A2;
  struct CQNQ B2;
  struct CQNQ C2;
  struct CQNQ D2;

  struct CQNQ F6;
  
  unsigned int F7 : 1;
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
 count = H->A.Q | ((H->B.Q << 1) | (H->C.Q << 2) | (H->D.Q << 3));
 printf("%d\t%d%d%d%d\n", count, H->D.Q, H->C.Q, H->B.Q, H->A.Q);

*/
void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D) {
  JKF(Clk, 1, 1, A);
  JKF(A->Q, 1, 1, B);
  JKF(B->Q, 1, 1, C);
  JKF(C->Q, 1, 1, D);
}

void HSync(int Clk, struct HSYNC *H) {
  int count;

  FBRC(Clk, &H->A, &H->B, &H->C, &H->D);
  FBRC(H->D.Q, &H->A2, &H->B2, &H->C2, &H->D2);

  // F6
  JKF(H->D2.Q, 1, 1, &H->F6);
  //printf("D2.Q %d F6.Q: %d\n", H->D2.Q, H->F6.Q);

  // 8 input NAND 7430
  H->F7 = !(H->B.Q && H->C.Q && H->C2.Q && H->D2.Q && H->F6.Q);

  //printf("F7: %d\n", H->F7);
}

int main() {
  // 20000000L = 50hz
  struct timespec ts = { .tv_sec = 0, .tv_nsec = 2000000L };
  int cycles = 900;
  int Clk = 0;

  struct HSYNC H = {0};

  /* FBRC Example */
  struct CQNQ A;
  struct CQNQ B;
  struct CQNQ C;
  struct CQNQ D;
  int count;
  /* FBRC Example */

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;

    // FBRC Example
    FBRC(Clk, &A, &B, &C, &D);
    count = A.Q | ((B.Q << 1) | (C.Q << 2) | (D.Q << 3));
    printf("%d\t%d%d%d%d\n", count, D.Q, C.Q, B.Q, A.Q);

    // HSync(Clk, &H);
     
    nanosleep(&ts, NULL);
  }

  return 0;
}

