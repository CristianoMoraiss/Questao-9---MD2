#include <stdio.h>

// Calcula o MDC (Algoritmo de Euclides)
int GC(int A, int B) {
    while (B != 0) {
        int T = B;
        B = A % B;
        A = T;
    }
    return A;
}

// Verifica se um número é primo
int PR(int N) {
    if (N <= 1) return 0;
    if (N <= 3) return 1;
    if (N % 2 == 0 || N % 3 == 0) return 0;
    for (int I = 5; I * I <= N; I += 6) {
        if (N % I == 0 || N % (I + 2) == 0) return 0;
    }
    return 1;
}

// Inverso modular usando o algoritmo estendido de Euclides
int MI(int A, int M) {
    int M0 = M, Q, T;
    int X0 = 0, X1 = 1;

    if (M == 1) return 0;

    while (A > 1) {
        Q = A / M;
        T = M;
        M = A % M;
        A = T;
        T = X0;
        X0 = X1 - Q * X0;
        X1 = T;
    }

    return (X1 < 0) ? X1 + M0 : X1;
}

// Função totiente de Euler
int ET(int N) {
    if (N == 1) return 1;
    if (PR(N)) return N - 1;

    int R = N;
    for (int I = 2; I * I <= N; I++) {
        if (N % I == 0) {
            while (N % I == 0) N /= I;
            R -= R / I;
        }
    }
    if (N > 1) R -= R / N;
    return R;
}

// Exponenciação modular
int ME(int B, int E, int M) {
    int R = 1;
    B %= M;
    while (E > 0) {
        if (E % 2 == 1) R = (R * B) % M;
        E >>= 1;
        B = (B * B) % M;
    }
    return R;
}

int main() {
    int H, G, N, X, N1;

    printf("Forneca valor de H: ");
    scanf("%d", &H);
    printf("Forneca valor de G: ");
    scanf("%d", &G);
    printf("Forneca o modulo n: ");
    scanf("%d", &N);
    printf("Forneca o expoente x: ");
    scanf("%d", &X);
    printf("Forneca o modulo n1: ");
    scanf("%d", &N1);

    // Passo 1: Verificar se G e N são co-primos
    int GC1 = GC(G, N);
    printf("\n[Passo 1] Verificando se G e N sao co-primos (MDC(G, N) = %d)...\n", GC1);
    if (GC1 != 1) {
        printf("Erro: G (%d) e n (%d) nao sao co-primos. Divisao nao e possivel.\n", G, N);
        return 1;
    }

    // Passo 2: Calcular o inverso de G mod N
    int IG = MI(G, N);
    printf("\n[Passo 2] Inverso modular de G em ZN: G⁻¹ = %d\n", IG);

    // Passo 3: Dividir H por G mod N
    int A = ((H % N) * (IG % N)) % N;
    printf("\n[Passo 3] A = (H * G⁻¹) mod N = (%d * %d) mod %d = %d\n", H, IG, N, A);

    // Passo 4: Verificar se A e N1 são co-primos
    int GC2 = GC(A, N1);
    printf("\n[Passo 4] Verificando se A e N1 sao co-primos (MDC(A, N1) = %d)...\n", GC2);
    if (GC2 != 1) {
        printf("Erro: base (%d) e n1 (%d) nao sao co-primos.\n", A, N1);
        return 1;
    }

    // Passo 5: Verificar se N1 é primo
    int IP = PR(N1);
    printf("\n[Passo 5] N1 (%d) é %s\n", N1, IP ? "primo" : "composto");

    // Passo 6 e 7: Calcular φ(N1)
    int PHI = IP ? (N1 - 1) : ET(N1);
    printf("[Passo 6/7] Valor de φ(N1): %d\n", PHI);

    // Passo 8: Decompor X = PHI * Q + R
    int R = X % PHI;
    int Q = X / PHI;
    printf("\n[Passo 8] Decompondo X = PHI * Q + R: %d = %d * %d + %d\n", X, PHI, Q, R);

    // Passo 9: Reescrevendo A^X mod N1 como produto de potências menores
    printf("\n[Passo 9] Reescrevendo a^x mod n1 como:\n");
    printf("(((a^φ(n1))^q mod n1) * (a^r mod n1)) mod n1\n");

    // Passo 10: Calcular valores intermediários
    int PQ = ME(A, PHI, N1);  // x2 = A^φ mod N1
    int PQQ = ME(PQ, Q, N1);  // x2^q mod N1
    int PRR = ME(A, R, N1);   // A^r mod N1
    printf("\n[Passo 10] Valores intermediarios:\n");
    printf("A^φ(N1) mod N1 = %d\n", PQ);
    printf("x2^q mod N1 = %d\n", PQQ);
    printf("A^r mod N1 = %d\n", PRR);

    // Passo 11: Resultado final
    int RES = (PQQ * PRR) % N1;
    printf("\n[Passo 11] Resultado final da congruencia:\n");
    printf("((%d^%d)^%d mod %d) * (%d^%d mod %d) mod %d = %d\n",
           A, PHI, Q, N1, A, R, N1, N1, RES);

    printf("\nResultado: %d\n", RES);

    return 0;
}
