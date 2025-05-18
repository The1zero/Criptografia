/*
    Programa AES-128
    Integrantes:
    - Labastida Vázquez Fernando
    - Badillo Aguilar Diego
    - Salgado Valdés Andrés
    - Jiménez Hernández Diana
    - Rodríguez Estrella Mairol Elizabeth
*/

#include <stdio.h>
#include <stdint.h>

// S-box (tabla de sustitución) de AES de 8 bits (16x16 valores en notación hexadecimal)
static uint8_t sbox[256] = {
  // 16 filas de 16 bytes:
  0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
  0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
  0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
  0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
  0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
  0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
  0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
  0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
  0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
  0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
  0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
  0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
  0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
  0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
  0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
  0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};

// Rcon: constantes de ronda para AES-128 (valor para 1≤i≤10; 0 no usado)
static uint8_t Rcon[11] = { 
  0x00, // Rcon[0] no se utiliza
  0x01, 0x02, 0x04, 0x08, 0x10, 
  0x20, 0x40, 0x80, 0x1B, 0x36 
};

// Función SubBytes: sustituye cada byte del estado por el correspondiente en la S-box
void SubBytes(uint8_t state[4][4]) {
    for(int r = 0; r < 4; r++) {
        for(int c = 0; c < 4; c++) {
            // Reemplaza el byte state[r][c] por el valor S-box de ese byte
            state[r][c] = sbox[state[r][c]];
        }
    }
}

// Función ShiftRows: rota cíclicamente a la izquierda las filas del estado.
// Fila 0: sin cambio, Fila 1: 1 byte a la izquierda, Fila 2: 2 bytes, Fila 3: 3 bytes.
void ShiftRows(uint8_t state[4][4]) {
    uint8_t temp[4];
    // Fila 1: rotar 1 a la izquierda
    for(int c = 0; c < 4; c++){ temp[c] = state[1][(c + 1) % 4]; }
    for(int c = 0; c < 4; c++){ state[1][c] = temp[c]; }

    // Fila 2: rotar 2 a la izquierda
    for(int c = 0; c < 4; c++){ temp[c] = state[2][(c + 2) % 4]; }
    for(int c = 0; c < 4; c++){ state[2][c] = temp[c]; }

    // Fila 3: rotar 3 a la izquierda (o 1 a la derecha, equivalente)
    for(int c = 0; c < 4; c++){ temp[c] = state[3][(c + 3) % 4]; }
    for(int c = 0; c < 4; c++){ state[3][c] = temp[c]; }
}

// Función auxiliar xtime: multiplica por x (2) en GF(2^8) con polinomio irreducible 0x11B.
uint8_t xtime(uint8_t x) {
    // Desplaza 1 bit a la izquierda y realiza XOR con 0x1B si el bit más significativo era 1.
    return (uint8_t)((x << 1) ^ ((x & 0x80) ? 0x1B : 0x00));
}

// Función MixColumns: mezcla cada columna del estado multiplicándola por la matriz fija de AES.
// Cada columna de 4 bytes [a0,a1,a2,a3]^T se transforma en [b0,b1,b2,b3]^T:
// b0 = (2*a0) ⊕ (3*a1) ⊕ (1*a2) ⊕ (1*a3),  (⊕ = XOR, * = multiplicación en GF(2^8))
// b1 = (1*a0) ⊕ (2*a1) ⊕ (3*a2) ⊕ (1*a3)
// b2 = (1*a0) ⊕ (1*a1) ⊕ (2*a2) ⊕ (3*a3)
// b3 = (3*a0) ⊕ (1*a1) ⊕ (1*a2) ⊕ (2*a3)
void MixColumns(uint8_t state[4][4]) {
    for(int c = 0; c < 4; c++) {  // para cada columna c
        // Extraer valores originales de la columna c
        uint8_t a0 = state[0][c];
        uint8_t a1 = state[1][c];
        uint8_t a2 = state[2][c];
        uint8_t a3 = state[3][c];
        // Calcular multiplicaciones por 2 (xtime) y por 3 (xtime ⊕ valor original)
        uint8_t a0_x2 = xtime(a0);
        uint8_t a1_x2 = xtime(a1);
        uint8_t a2_x2 = xtime(a2);
        uint8_t a3_x2 = xtime(a3);
        uint8_t a0_x3 = a0_x2 ^ a0;
        uint8_t a1_x3 = a1_x2 ^ a1;
        uint8_t a2_x3 = a2_x2 ^ a2;
        uint8_t a3_x3 = a3_x2 ^ a3;
        // Aplicar la mezcla según la matriz de coeficientes [02,03,01,01] etc.
        state[0][c] = a0_x2 ^ a1_x3 ^ a2      ^ a3;      // 2*a0 ⊕ 3*a1 ⊕ 1*a2 ⊕ 1*a3
        state[1][c] = a0    ^ a1_x2 ^ a2_x3 ^ a3;      // 1*a0 ⊕ 2*a1 ⊕ 3*a2 ⊕ 1*a3
        state[2][c] = a0    ^ a1   ^ a2_x2 ^ a3_x3;   // 1*a0 ⊕ 1*a1 ⊕ 2*a2 ⊕ 3*a3
        state[3][c] = a0_x3 ^ a1   ^ a2    ^ a3_x2;   // 3*a0 ⊕ 1*a1 ⊕ 1*a2 ⊕ 2*a3
    }
}

// Función AddRoundKey: realiza XOR entre el estado y la subclave de la ronda dada (matrices 4x4 bytes).
void AddRoundKey(uint8_t state[4][4], uint8_t roundKey[4][4]) {
    for(int r = 0; r < 4; r++) {
        for(int c = 0; c < 4; c++) {
            state[r][c] ^= roundKey[r][c];
        }
    }
}

// Función KeyExpansion: genera las 10 subclaves (más la original) a partir de la clave inicial de 16 bytes.
// La clave original se copia como RoundKey[0], y luego se calculan RoundKey[1] a RoundKey[10] (16 bytes cada una).
void KeyExpansion(const uint8_t key[16], uint8_t roundKeys[11][4][4]) {
    // Copiar la clave original en roundKeys[0] (en forma de matriz 4x4).
    // Se copian por columnas: los primeros 4 bytes son 1ª columna, etc.
    int byte = 0;
    for(int c = 0; c < 4; c++) {
        for(int r = 0; r < 4; r++) {
            roundKeys[0][r][c] = key[byte++];
        }
    }

    // Ahora generamos las subclaves roundKeys[1..10]
    // Cada subclave tiene 4 palabras de 32 bits (16 bytes). Usamos índice de palabra i de 4..43.
    for(int i = 4; i < 44; i++) {
        // Las palabras anteriores a i (w[i-1] y w[i-4]) están en roundKeys[?]
        uint8_t temp[4];
        // Obtener w[i-1] (palabra anterior de 4 bytes)
        int prev_word_index = i - 1;
        // roundIndex = índice de subclave al que pertenece w[i-1]
        int prev_round = prev_word_index / 4;
        int prev_col   = prev_word_index % 4;
        for(int j = 0; j < 4; j++) {
            temp[j] = roundKeys[prev_round][j][prev_col];
        }

        if(i % 4 == 0) {
            // Si i es múltiplo de 4, aplicar RotWord, SubWord, y XOR con Rcon.
            // RotWord: rotación de 1 byte a la izquierda de la palabra temp
            uint8_t firstByte = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = firstByte;
            // SubWord: aplicar S-box a cada byte de temp
            temp[0] = sbox[temp[0]];
            temp[1] = sbox[temp[1]];
            temp[2] = sbox[temp[2]];
            temp[3] = sbox[temp[3]];
            // XOR con Rcon(i/4) en el byte más significativo (posición 0) de temp
            uint8_t rconVal = Rcon[i/4];
            temp[0] ^= rconVal;
        }

        // Ahora calculamos w[i] = w[i-4] XOR temp
        int prev4_index = i - 4;
        int prev4_round = prev4_index / 4;
        int prev4_col   = prev4_index % 4;
        // round actual = i/4 (entero división) => subclave donde irá w[i]
        int curr_round = i / 4;
        int curr_col   = i % 4;
        for(int j = 0; j < 4; j++) {
            // XOR byte a byte: w[i] = w[i-4] ⊕ temp
            roundKeys[curr_round][j][curr_col] = roundKeys[prev4_round][j][prev4_col] ^ temp[j];
        }
    }
}

// Función para imprimir el estado o subclave 4x4 en hex, con un mensaje de etiqueta.
void printMatrix(const char *label, uint8_t matrix[4][4]) {
    printf("%s\n", label);
    for(int r = 0; r < 4; r++) {
        for(int c = 0; c < 4; c++) {
            printf("%02X ", matrix[r][c]);  // imprimir byte en formato 2 dígitos hex
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    // Clave de cifrado de 128 bits (16 bytes) - ejemplo: "Thats my Kung Fu"
    uint8_t key[16] = { 'T','h','a','t','s',' ','m','y',' ','K','u','n','g',' ','F','u' };
    // Texto plano de 128 bits (16 bytes) - ejemplo: "Two One Nine Two"
    uint8_t plaintext[16] = { 'T','w','o',' ','O','n','e',' ','N','i','n','e',' ','T','w','o' };

    // Matriz de estado 4x4 para el bloque actual
    uint8_t state[4][4];
    // Inicializar el estado con el texto plano (llenar columna por columna)
    int idx = 0;
    for(int c = 0; c < 4; c++){
        for(int r = 0; r < 4; r++){
            state[r][c] = plaintext[idx++];
        }
    }

    // Generar subclaves de ronda (RoundKeys) usando KeyExpansion
    uint8_t roundKeys[11][4][4];
    KeyExpansion(key, roundKeys);

    // Imprimir la clave original y las subclaves generadas
    printMatrix("Clave original (RoundKey 0):", roundKeys[0]);
    for(int round = 1; round <= 10; round++) {
        char label[50];
        sprintf(label, "Subclave de Ronda %d (RoundKey %d):", round, round);
        printMatrix(label, roundKeys[round]);
    }

    // Imprimir el estado inicial (texto plano) antes de comenzar rondas
    printMatrix("Estado inicial (Texto Plano):", state);

    // Ronda 0: AddRoundKey con la clave original
    AddRoundKey(state, roundKeys[0]);
    printMatrix("Estado despues de AddRoundKey (Ronda 0):", state);

    // Rondas 1 a 9:
    for(int round = 1; round <= 9; round++) {
        char label[50];
        // SubBytes
        SubBytes(state);
        sprintf(label, "Estado despues de SubBytes (Ronda %d):", round);
        printMatrix(label, state);
        // ShiftRows
        ShiftRows(state);
        sprintf(label, "Estado despues de ShiftRows (Ronda %d):", round);
        printMatrix(label, state);
        // MixColumns
        MixColumns(state);
        sprintf(label, "Estado despues de MixColumns (Ronda %d):", round);
        printMatrix(label, state);
        // AddRoundKey con subclave de la ronda actual
        AddRoundKey(state, roundKeys[round]);
        sprintf(label, "Estado despues de AddRoundKey (Ronda %d):", round);
        printMatrix(label, state);
    }

    // Ronda 10 (última ronda, sin MixColumns):
    SubBytes(state);
    printMatrix("Estado despues de SubBytes (Ronda 10):", state);
    ShiftRows(state);
    printMatrix("Estado despues de ShiftRows (Ronda 10):", state);
    // *No* MixColumns en ronda 10
    AddRoundKey(state, roundKeys[10]);
    printMatrix("Estado despues de AddRoundKey (Ronda 10) - Texto Cifrado:", state);

    return 0;
}
