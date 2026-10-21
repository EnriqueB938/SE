# preparación toolchain

## objetivo

Llegados a este paso tenemos el Keil ulink2 que se detecta y nos permite conectarnos con la placa de desarrollo. Bien!

Ahora necesitamos varias herramientas más!

Un compilador para el ISA ARMv4T, un terminal para poder conectarnos con el puerto que nos deja OpenOCD abierto para ellos, un depurador, una librería de C estándar para sistemas empotrados...

## instalación de las herramientas

### Gentoo

#### netcat

```
# emerge --ask net-analyzer/netcat
```

#### el resto de componentes

```
# emerge --ask sys-devel/crossdev
# crossdev -t arm-none-eabi
```

### Debian / Ubuntu

#### netcat

```
$ sudo apt install netcat
```

#### el resto de componentes
```
$ sudo apt update
$ sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi gdb-multiarch libnewlib-arm-none-eabi
```

## comprobación de que podemos conectarnos con nuestra placa MCB2300

Lanzamos nuestro openocd en un temrinal (mcb2300.cfg debe de localizarse en la carpeta donde se localiza el terminal).  
```
$ openocd -f interface/cmsis-dap.cfg -f mcb2300.cfg
```

OpenOCD nos va a abrir varios puertos para distintas conexiones, telnet, gdb, etc. En otro terminal nos conectamos con netcat por telnet, comando *nc*, así:

```
$ nc localhost 4444
```
  
Ésto nos abre una consola de depuración que vamos a emplear tanto para analizar el estado de la placa como para programarla!  
  
Varios comandos para probar, que son interesantes son:  
*reg* , para ver el estado actual de todos los registros del procesador:  
```  
> reg  
 reg
===== ARM registers
(0) r0 (/32)
(1) r1 (/32)
(2) r2 (/32)
(3) r3 (/32)
(4) r4 (/32)
(5) r5 (/32)
(6) r6 (/32)
(7) r7 (/32)
(8) r8 (/32)
(9) r9 (/32)
(10) r10 (/32)
(11) r11 (/32)
(12) r12 (/32)
(13) sp_usr (/32)
(14) lr_usr (/32)
(15) pc (/32)
(16) r8_fiq (/32)
(17) r9_fiq (/32)
(18) r10_fiq (/32)
(19) r11_fiq (/32)
(20) r12_fiq (/32)
(21) sp_fiq (/32)
(22) lr_fiq (/32)
(23) sp_irq (/32)
(24) lr_irq (/32)
(25) sp_svc (/32)
(26) lr_svc (/32)
(27) sp_abt (/32)
(28) lr_abt (/32)
(29) sp_und (/32)
(30) lr_und (/32)
(31) cpsr (/32)
(32) spsr_fiq (/32)
(33) spsr_irq (/32)
(34) spsr_svc (/32)
(35) spsr_abt (/32)
(36) spsr_und (/32)
(37) sp (/32)
(38) lr (/32) 
```
   
*reset halt*, que nos va a permitir parar la ejecución de la placa  
  
*resume* que nos va a permitir continuar con la ejecución. 


## comprobación de que podemos compilar ya para nuestra placa

si ejecutamos:

```
$ arm-none-eabi-gcc -mcpu=arm7tdmi -marm -Q --help=target | head
```

la salida debe de ser parecida a:  
```  
 Las siguientes opciones son específicas del objetivo:
  -ffuse-ops-with-volatile-access 	[activado]
  -mabi=                      		aapcs
  -mabort-on-noreturn         		[desactivado]
  -mapcs                      		[desactivado]
  -mapcs-frame                		[desactivado]
  -mapcs-reentrant            		[desactivado]
  -mapcs-stack-check          		[desactivado]
  -march=                     		armv4t
  -marm                       		[activado] 
```

Lo importante es quedarnos con el apartado *march* donde debe de aparecer el ISA al que compilamos, y nuestra placa MCB2300 tiene un uC LPC2378 que es un ARM7 de tipo *ARM7TDMI-S* con un ISA ARMv4t

## conclusión

Con esto ya tenemos el depurador y tenemos el compilador, faltaría el código para subir a la placa!