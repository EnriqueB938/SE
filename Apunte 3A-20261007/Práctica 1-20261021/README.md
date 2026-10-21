Práctica 1: Entradas digitales empleando polling
===

# Objetivo de la práctica

+ Vamos a empezar a organizarnos el código mejor, y vamos a emplear un control de versiones.
+ Vamos a ver qué entradas digitales nos ofrece la placa de desarrollo MCB2300 y en concreto vamos a emplear el pulsador llamado *INT0*
+ Emplear una entrada digital mediante la técnica del [polling](https://es.wikipedia.org/wiki/Polling). Vamos a ver las dificultades y limitaciones que presenta.
+ Vamos a hablar de las conexiones [Pull-Up y Pull-Down](https://en.wikipedia.org/wiki/Pull-up_resistor) en entradas digitales y lo que conlleva. Tendremos que ver esquemáticos eléctricos!


# Recordad

## OpenOCD

Para conectaros con OpenOCD deberéis ejecutar el servidor openocd **dentro de la carpeta de trabajo** mediante el comando:

(si tenéis el programador Keil ulink 2)
```
$ openocd -f interface/cmsis-dap.cfg -f mcb2300.cfg
```

(si tenéis el programador jlink, el de color negro)
```
$ openocd -f interface/jlink.cfg -f mcb2300.cfg
```

Después, en otro terminal, pero también **dentro de la carpeta de trabajo** os conectáis mediante telnet con el comando:
```
nc localhost 4444
```

Para programar la placa:
```
> reset halt
> program <nombre del ejecutable>
> reset run
```

Algunas veces la placa falla en programarse, resetearse, etc. Leed el mensaje de vuelta bien y repetid el comando que haya fallado si es necesario.

## Makefile

Editad el Makefile si añadís más ficheros de código de fuente o los renombrais.

Para borrar todos los binarios y código objeto:
```
make clean
```

# Qué vamos a hacer?

Queremos que al pulsar el botón INT0 el contador BCD se incremente.

Habrá que:

+ Revisar el código BCD.c si está bien o contiene algún error.
+ Identificar el botón INT0 de la placa de desarrollo MCB2300 a qué pin de entrada digital del microcontrolador LPC2378 entra.
+ Identificar donde se configura en memoria y configurarlo!
+ Identificar como se puede leer su estado, realizar un pequeño ejemplo preeliminar que cuando se pulse se encienda un LED.
+ Implementad un contador BCD con el pulsador.

De manera transversal vamos a:

+ emplear un control de versiones, en éste caso git.
+ vamos a empezar a facilitarnos la vida separando el código en diversos ficheros de código fuente.

# Licencia

Copyright 2026 Joaquín Cuéllar <p22cupaj@uco.es>

Este código está licenciado bajo Apache2, podéis hacer lo que queráis con él, es vuestro.