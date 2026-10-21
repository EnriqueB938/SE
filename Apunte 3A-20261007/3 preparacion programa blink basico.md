# preparación programa básico blink.c

## objetivo

El objetivo es subir nuestro primer programa, que no es nada más ni nada menos que hacer que un led parpadee durante cierto tiempo.

La utilidad que tiene este ejemplo es ver qué necesitamos a bajo nivel para realizar esto, ser conscientes de que normalmente vamos a tener un ide (Arduino ide, Platformio...) o al menos un framework (esp-idf, zephyr, arduino...) que nos lo va a automatizar.

## startup.S

Esta placa es bastante rudimentaria, va a necesitar un código ensamblador que se ejecute siempre al inicio de forma que podamos configurar itnerrupciones, periféricos y enganchemos con la función main del código C que compilemos.
  
Miradlo.  
  
Es interesante que indaguéis en los vectores de interrupciones generales IRQ y las rápidas FIQ.  
Es interesante que indaguéis sobre los distintos modos de funcionamiento: supervidor, usuario, etc.  
Todo ésto del tema 3  
y luego releáis el ensamblador :-)
  
No tiene más importancia, no tenéis que controlarlo ni modificarlo. A menos que queráis cacharrear, claro!

## linker.ld  
  
Creamos un fichero de linkado. Es interesante porque define funciones que vamos a tener que declarar para el manejo de interrupciones y nuestro main, y establece ya los límites de la memoria de la que vamos a disponer: el stack y el heap. Básicamente.  
  
No tiene más importancia tampoco y lo vamos a reutilizar entre programas. Pero es necesario para compilar!  
  
## blink1.c
  
La primera versión del programa, que ejecuta un parpadeo mediante un delay bloqueante.  
Eso es horrible! :-D pero nos sirve para ver como trabaja este micro con los pines GPIO.

## el makefile!  
  
Automatizamos la compilación, ya tá.

## el momento de la verdad! Compilamos y flasheamos  
  
```  
$ make  
```  
  
ésto nos proporciona un blink1.elf  
  
iniciamos la conexión openocd y dentro de netcat:  
```  
$ nc localhost 4444  
��������Open On-Chip Debugger
> reset halt  
[...]  
> program blink1.elf
[...]
> reset run  
```  
y voilà, debéis de tener el pin 2.0 parpadeando.  
  
## a donde vamos desde aquí:  
  
¿Podéis cambiar el pin que parpadea?  
  
¿Os animáis a hacer un contador BCD con los 8 leds?
