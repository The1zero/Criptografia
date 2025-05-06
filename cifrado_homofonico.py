'''
    Programa de Cifrado por Sustitución Homofónica
    Integrantes:
    - Labastida Vázquez Fernando
    - Badillo Aguilar Diego
    - Salgado Valdés Andrés
    - Jiménez Hernández Diana
    - Rodríguez Estrella Mairol Elizabeth
    - Quintero Rubio Martin
'''

import random

# Diccionario de cifrado homofónico
homofonico_dict = {
    'A': ['12', '19', '25'],
    'B': ['30', '33'],
    'C': ['40', '42', '44'],
    'D': ['50', '52'],
    'E': ['60', '62', '64', '66'],
    'F': ['70', '73'],
    'G': ['80', '82'],
    'H': ['90', '91', '92'],
    'I': ['10', '11', '13'],
    'J': ['20', '21'],
    'K': ['31', '32'],
    'L': ['41', '43'],
    'M': ['51', '53'],
    'N': ['61', '63'],
    'O': ['71', '72'],
    'P': ['81', '83'],
    'Q': ['93'],
    'R': ['22', '23'],
    'S': ['34', '35'],
    'T': ['45', '46'],
    'U': ['54', '55'],
    'V': ['65', '67'],
    'W': ['74', '75'],
    'X': ['84', '85'],
    'Y': ['94', '95'],
    'Z': ['96', '97']
}

# Generar diccionario inverso para descifrar
decodificacion_dict = {codigo: letra for letra, codigos in homofonico_dict.items() for codigo in codigos}

def cifrar(texto):
    texto = texto.upper()
    texto_cifrado = []
    for letra in texto:
        if letra == " ":
            texto_cifrado.append("/")  # Marcar espacios
        elif letra in homofonico_dict:
            texto_cifrado.append(random.choice(homofonico_dict[letra]))
        else:
            texto_cifrado.append(letra)  # Mantener caracteres no cifrables
    return ' '.join(texto_cifrado)

def descifrar(texto_cifrado):
    lista_cifrada = texto_cifrado.split()
    texto_descifrado = []
    for codigo in lista_cifrada:
        if codigo == "/":
            texto_descifrado.append(" ")  # Restaurar espacios
        else:
            texto_descifrado.append(decodificacion_dict.get(codigo, codigo))  # Mantener caracteres no cifrables
    return ''.join(texto_descifrado)

if __name__ == '__main__':
    print('\nBienvenido al Programa de Cifrado por Sustitución Homofónica :)')
    while True:
        opcion = input('\n¿Deseas cifrar (1), descifrar (2), ambas (3) o salir (4)? ')
        if opcion == '1':
            mensaje = input('Introduce el mensaje a cifrar: ')
            cifrado = cifrar(mensaje)
            print("Texto cifrado:", cifrado)
        elif opcion == '2':
            mensaje = input('Introduce el mensaje a descifrar: ')
            descifrado = descifrar(mensaje)
            print("Texto descifrado:", descifrado)
        elif opcion == '3':
            mensaje = input('Introduce el mensaje a cifrar: ')
            cifrado = cifrar(mensaje)
            print("Texto cifrado:", cifrado)
            descifrado = descifrar(cifrado)
            print("Texto descifrado:", descifrado)
        elif opcion == '4':
            print('Adiós :)')
            break
        else:
            print('Opción no válida')
            