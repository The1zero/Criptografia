'''
    Programa Cifrado César
    Integrantes:
    - Labastida Vázquez Fernando
    - Badillo Aguilar Diego
    - Salgado Valdés Andrés
    - Jiménez Hernández Diana
    - Rodríguez Estrella Mairol Elizabeth
    - Quintero Rubio Martin
'''

def cifrado_vigenere(mensaje, llave):
    mensaje_cifrado = ''  # Para guardar el resultado
    longitud_llave = len(llave)
    indice_llave = 0  # Índice para recorrer la llave

    for caracter in mensaje:  # Recorremos caracter por caracter
        if caracter.isalpha():  # Si es letra, codifica
            # Convertimos la letra de la llave en su posición en el alfabeto (A=0, B=1, ..., Z=25)
            desplazamiento = ord(llave[indice_llave].lower()) - ord('a')

            if caracter.islower():  # Si el carácter es minúscula
                nuevo_caracter = chr((ord(caracter) - ord('a') + desplazamiento) % 26 + ord('a'))
            else:  # Si el carácter es mayúscula
                nuevo_caracter = chr((ord(caracter) - ord('A') + desplazamiento) % 26 + ord('A'))

            mensaje_cifrado += nuevo_caracter
            indice_llave = (indice_llave + 1) % longitud_llave  # Reiniciamos el índice si llega al final de la llave
        else:
            mensaje_cifrado += caracter  # Si no es letra, se mantiene igual

    return mensaje_cifrado


def descifrado_vigenere(mensaje, llave):
    mensaje_descifrado = ''
    longitud_llave = len(llave)
    indice_llave = 0  # Índice para recorrer la llave

    for caracter in mensaje:
        if caracter.isalpha():
            desplazamiento = ord(llave[indice_llave].lower()) - ord('a')

            if caracter.islower():
                nuevo_caracter = chr((ord(caracter) - ord('a') - desplazamiento) % 26 + ord('a'))
            else:
                nuevo_caracter = chr((ord(caracter) - ord('A') - desplazamiento) % 26 + ord('A'))

            mensaje_descifrado += nuevo_caracter
            indice_llave = (indice_llave + 1) % longitud_llave  # Reiniciamos el índice si llega al final de la llave
        else:
            mensaje_descifrado += caracter

    return mensaje_descifrado


if __name__ == '__main__':
    while True:
        opcion = input('\n¿Quieres cifrar(1), descifrar(2) o salir(3)? ')

        if opcion == '1' or opcion == '2':  # Si desea continuar con el programa
            mensaje = input('Introduce el mensaje: ')
            while True:  # Ciclo por si el usuario ingresa una llave incorrecta
                llave = input('Introduce la llave: ')
                if llave.isalpha():  # Validamos que la llave sean puras letras
                    if opcion == '1':  # Ciframos
                        print('Mensaje cifrado:', cifrado_vigenere(mensaje, llave))
                    else:  # Desciframos
                        print('Mensaje descifrado:', descifrado_vigenere(mensaje, llave))
                    break
                else:
                    print('\nLa llave sólo puede contener letras')
        elif opcion == '3':  # Para salir del programa
            print('\nAdiós :)')
            break
        else:
            print('Opción inválida')
