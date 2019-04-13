/* ℛℒ ⅯⅯⅪⅩ*/
#include <stdio.h>
#include <time.h>

struct CQNQ {
    int Clk;
    int Q;
    int NQ;
};

//void FBRC(int Clk, int *Count);
void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D);
void JKF(int Clk, int J, int K, struct CQNQ *JKF);

void JKF(int Clk, int J, int K, struct CQNQ *JKF) {
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
        JKF->NQ = !JKF->Q;
    }
    JKF->Clk = Clk;
}

void FBRC(int Clk, struct CQNQ *A, struct CQNQ *B, struct CQNQ *C, struct CQNQ *D) {
  JKF(Clk, 1, 1, A);
  JKF(A->Q, 1, 1, B);
  JKF(B->Q, 1, 1, C);
  JKF(C->Q, 1, 1, D);
}

int main() {
  struct timespec ts = { .tv_sec = 0, .tv_nsec = 100000000L };
  int cycles = 70;
  int Clk = 0;

  struct CQNQ A = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ B = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ C = { .Clk = 0, .Q = 0, .NQ = 0 };
  struct CQNQ D = { .Clk = 0, .Q = 0, .NQ = 0 };

  int count = 0;

  for (int i = 0; i < cycles; i++) {
    Clk ^= 1;
    
    count = A.Q;
    count |= B.Q << 1;
    count |= C.Q << 2;
    count |= D.Q << 3;
    if (Clk) {
        printf("%d\t%d\t%d%d%d%d\n", Clk, count, D.Q, C.Q, B.Q, A.Q);
    }
    
    FBRC(Clk, &A, &B, &C, &D);
            
    nanosleep(&ts, NULL);
  }

  return 0;
}

