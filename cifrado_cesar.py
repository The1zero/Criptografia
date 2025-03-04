'''
    Programa Cifrado César
    Integrantes:
    - Labastida Vázquez Fernando
    - Badillo Aguilar Diego
    - Salgado Valdés Andrés
    - Jiménez Hernández Diana
    - Rodríguez Estrella Mairol Elizabeth
'''

def cifrado_cesar(mensaje, llave):
    mensaje_cifrado = '' # Para guardar el resultado
    
    for caracter in mensaje: # Recorremos caracter por caracter
        if caracter.isalpha(): # Si es letra, codifica
            # Obtenemos el ASCII del caracter y lo recorremos con la llave dentro del rango 
            # del alfabeto para que el ASCII no se vaya a caracteres especiales
            corrimiento = ord(caracter) + (llave % 26) 
            if caracter.islower(): # Si es minúscula
                if corrimiento > ord('z'): # Si se desborda del alfabeto en minúsculas
                    corrimiento -= 26 # Le 'damos la vuelta' al alfabeto
                mensaje_cifrado += chr(corrimiento) # Concatenamos el resultado (chr nos da la letra según su ASCII)
            elif caracter.isupper(): # Si es mayúscula
                if corrimiento > ord('Z'): # Si se desborda del alfabeto en mayúsculas
                    corrimiento -= 26 # Le 'damos la vuelta' al alfabeto
                mensaje_cifrado += chr(corrimiento) # Concatenamos el resultado (chr nos da la letra según su ASCII)
        else: # Si no es letra, no hace nada (como espacios o signos)
            mensaje_cifrado += caracter # Concatenamos el caracter
    
    return mensaje_cifrado
    
def descifrado_cesar():
    # Sólo le pasamos la llave negativa para descrifrar 
    return cifrado_cesar(mensaje, -llave) 
        
def es_entero(llave):
    # Intentamos convertir la llave a entero, para validar que lo sea
    # Si lo es, devolvemos True. Si no lo es, devolvemos False
    try:
        int(llave)
        return True
    except ValueError:
        return False
    
if __name__ == '__main__':
    while True:
        # Le preguntamos al usuario qué quiere hacer
        cifrar_o_descifrar = input('\n¿Quieres cifrar(1), descifrar(2) o salir(3)? ')
        
        if cifrar_o_descifrar == '1' or cifrar_o_descifrar == '2': # Si desea continuar con el programa
            # Pedimos la información de entrada    
            mensaje = input('Introduce el mensaje: ')
            while True: # Ciclo por si el usuario ingresa una llave incorrecta
                llave = input('Introduce la llave: ')
                if es_entero(llave): # Validamos que es un entero positivo o negativo
                    llave = int(llave) # Capturamos la llave como entero
                    if cifrar_o_descifrar == '1': # Ciframos
                        print('Mensaje cifrado:', cifrado_cesar(mensaje, llave))
                        break
                    else: # Desciframos
                        print('Mensaje descifrado:', descifrado_cesar())
                        break
                else: # Si no lo es...
                    print('\nLa llave tiene que ser un entero positivo o negativo')
        elif cifrar_o_descifrar == '3': # Para salir del programa
            print('Adiós :)')
            break
        else: # Si teclea lo que sea fuera de las tres opciones
            print('Opción inválida')
    
 