/*
    Programa Generador de llaves Diffie-Hellman
    Este programa implementa el protocolo de intercambio de claves Diffie-Hellman.
    Permite a dos partes (Alicia y Bob) generar una clave compartida de manera segura
    a través de un canal inseguro.
    El programa solicita un número primo grande (p), un generador (g) y los números
    privados de ambas partes (a y b). Luego, calcula los valores públicos (A y B)
    y finalmente genera la clave compartida (K) que ambas partes pueden usar para
    cifrar y descifrar mensajes.
    El programa incluye validaciones para asegurar que los números ingresados sean
    válidos y que el generador esté dentro del rango adecuado.
    El algoritmo utilizado es la exponenciación modular, que permite calcular
    eficientemente (base^exp) % mod.
    El programa también verifica si el número primo ingresado es realmente primo
    y si el generador es menor que p.
    
    Integrantes:
    - Labastida Vázquez Fernando
    - Badillo Aguilar Diego
    - Salgado Valdés Andrés
    - Jiménez Hernández Diana
    - Rodríguez Estrella Mairol Elizabeth
**/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Función para calcular (base^exp) % mod de manera eficiente
long mod_pow(long base, long exp, long mod) {
    long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

// Función simple para validar números mayores a 1
int validar_numero(long num) {
    if (num <= 1) return 0;
    return 1;
}
// Función para validar si un número es primo
bool esPrimo(int numero) {
    if (numero <= 1) {
        printf("Error: El número debe ser mayor a 1.\n");
        return false;
    }
    for (int i = 2; i * i <= numero; i++) {
        if (numero % i == 0) {
            printf("Error: El número no es primo.\n");
            return false;
        }
    }
    return true;
}


int main() {
    long p, g, a, b;

    printf("\n==== Protocolo Diffie-Hellman ====\n");
    printf("\n======= Generador de llaves =======\n");

    // Solicitar número primo 'p'
    printf("\nIngresa un número primo grande (p): ");
    scanf("%ld", &p);
    if (!esPrimo(p)) {
        return 1;
    }

    // Solicitar generador 'g'
    printf("Ingresa un generador menor que p (g): ");
    scanf("%ld", &g);
    if (g <= 1 || g >= p) {
        printf("Error: El generador debe ser mayor que 1 y menor que p.\n");
        return 1;
    }

    // Solicitar número privado de Alicia
    printf("Número privado para Alicia (a): ");
    scanf("%ld", &a);
    if (!validar_numero(a)) {
        printf("Error: El número privado debe ser mayor que 1.\n");
        return 1;
    }

    // Solicitar número privado de Bob
    printf("Número privado para Bob (b): ");
    scanf("%ld", &b);
    if (!validar_numero(b)) {
        printf("Error: El número privado debe ser mayor que 1.\n");
        return 1;
    }

    // Calcular valores públicos
    long A = mod_pow(g, a, p);
    long B = mod_pow(g, b, p);

    printf("\nValores públicos generados:\n");
    printf("Alicia envía a Bob (A): %ld = %ld^%ld mod %ld\n", A, g, a, p);
    printf("Bob envía a Alicia (B): %ld = %ld^%ld mod %ld\n", B, g, b, p);
    printf("\nAlicia y Bob han intercambiado sus valores públicos.\n");

    // Generar claves compartidas
    long clave_A = mod_pow(B, a, p);
    long clave_B = mod_pow(A, b, p);
    printf("\nCalculo de la clave secreta compartida:\n");
    printf("Alicia calcula (K):%ld = %ld^%ld mod %ld\n", clave_A, B, a, p);
    printf("Bob envía a Alicia (K):%ld = %ld^%ld mod %ld\n", clave_B, A, b, p);

    // Confirmar que la clave es la misma
    if (clave_A == clave_B) {
        printf("\n¡Clave compartida establecida con éxito!\n");
        printf("Clave compartida (K): %ld\n\n", clave_A);
    } else {
        printf("\nError: Las claves generadas no coinciden.\n");
    }

    return 0;
}
