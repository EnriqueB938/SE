# Configuración del depurador de Keil ulink2

##0. Objetivo

+ El programador que vamos a emplear en las prácticas: keil ulink2 es un típico programador jtag. (os enseño otro genérico en clase por ejemplo el segger)

+ Se conecta por USB y sirve para conectarnos con la placa de desarrollo, de forma que podamos:
    - subirle código
    - mandarle comandos de reset, etc.
    - leer mensajes de la placa
    - depuración mediante breakpoints en el código.

+ los pasos que vamos a ver son necesarios para poder trabajar bajo GNU/Linux, usemos tanto OpenOCD o el propio software de Keil mediante wine.

##1. conectarlo e identificarlo

Enchufa el dispositivo y lo primero es identificarlo.

Qué nos dice el kernel?

```
# dmesg
[...]
[35401.106924] usb 3-2: new full-speed USB device number 9 using xhci_hcd
[35401.239261] usb 3-2: New USB device found, idVendor=c251, idProduct=2722, bcdDevice= 1.00
[35401.239270] usb 3-2: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[35401.239273] usb 3-2: Product: Keil ULINK2 CMSIS-DAP
[35401.239275] usb 3-2: Manufacturer: Keil Software
[35401.239278] usb 3-2: SerialNumber: V1415UAE
[35401.242517] hid-generic 0003:C251:2722.000A: hiddev97,hidraw5: USB HID v1.00 Device [Keil Software Keil ULINK2 CMSIS-DAP] on usb-0000:00:14.0-2/input0
```

Nos indica que es un dispositivo que se ha mapeado como hidraw5, para quién esté familiarizado con Arduino, no es un /dev/ttyACM0 o /dev/ttyUSB0, si no un /dev/hidraw5 que son [de acceso en bruto a USB](https://docs.kernel.org/hid/hidraw.html).

Otra forma de identificar el dispositivo es el comando lsusb a través de nuestra cuenta de usuaria de linux, no hace falta privilegios root:

```
$ lsusb
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 003 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub
Bus 003 Device 002: ID 05e3:0610 Genesys Logic, Inc. Hub
Bus 003 Device 003: ID 04f2:b71a Chicony Electronics Co., Ltd Integrated IR Camera
Bus 003 Device 005: ID 8087:0026 Intel Corp. AX201 Bluetooth
Bus 003 Device 006: ID 046d:c534 Logitech, Inc. Nano Receiver
Bus 003 Device 007: ID 067b:2586 Prolific Technology, Inc. USB 2.0 Hub
Bus 003 Device 008: ID 0bda:8152 Realtek Semiconductor Corp. RTL8152 Fast Ethernet Adapter
Bus 003 Device 009: ID c251:2722 Keil Software, Inc. Keil ULINK2 CMSIS-DAP
Bus 004 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 004 Device 002: ID 05e3:0620 Genesys Logic, Inc. GL3523 Hub
Bus 004 Device 003: ID 0bda:0316 Realtek Semiconductor Corp. Card Reader
```

##2. grupo de usuario

El problema para emplear un dispositivo de este tipo es que hay que pertenecer al grupo que lo maneja.
  
### típicos grupos: dialout plugdev...

Para emplear el usb cuando trabajamos con arduino o con un esp32, debemos agregar a nuestro usuario al grupo dialout.

podemos ver a qué grupos pertenece nuestro usuario mediante el comando:
```
$ groups  <nombre de usuario>
```
mis grupos a modo de ejemplo:
```
$ groups gentooza
gentooza : gentooza lp wheel uucp audio dialout video usb users lpadmin seat plugdev vboxusers docker-autorfirma usbgroup android
```

### añadir usuario a grupo

Para el caso particular de esta placa y los dispositivos hidraw, vamos a asegurarnos que nuestro usuario está en el grupo *plugdev*, para añadir un usuario a un grupo lo hacemos así:
```
# gpasswd -a <nombre de usuario> plugdev
```
con mi usuario hice:
```
# gpasswd -a gentooza plugdev
```

**Para que los cambios en los grupos se hagan efectivos deberéis hacer logout y login de nuevo, o reiniciar el sistema!**

##3. crear regla udev para meter dispositivo en grupo

Por último, vamos a decirle a udev que es quién automatiza qué hacer cuando un dispositivo se conecta a nuesto linux, qué hacer cuando conectemos el keil ulink2, para ello vamos a crear una regla de udev (empleo nano en el ejemplo, utilizad el editor que queráis):
```
# nano /etc/udev/rules.d/70-keil-ulink2.rules
```
```
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="c251", ATTRS{idProduct}=="2722", MODE="0660", GROUP="plugdev", TAG+="uaccess"
```

fijaos que a todo el subsistema hidraw, cuando el vendedor es "c251" y el dipositivo "2722" (acordaos de la salida de dmesg y de lsusb) que tenga permisos de lectura y escritura el usuario y el grupo, y además que pertenezca al grupo plugdev.

Solucionao!

##4. Instalar OpenOCD

[OpnOCD suele estar disponible en los repositorios oficiales de las principales distribuciones de GNU/Linux](https://openocd.org/pages/getting-openocd.html)

En gentoo haríamos (con flags USE: cmsis-dap ftdi jlink usb):
```
# emerge --ask dev-embedded/openocd
```

En Debian/Ubuntu:
```
$ sudo apt install openocd
```

### copiaros el fichero de configuración para openOCD

El fichero es el *mcb2300.cfg*

En la carpeta donde vayais a trabajar, tened siempre este fichero, pues es el que nos configura openocd para trabajar con el Keil ulink2.

##5. Conectarse con OpenOCD

El comando utiliza dos ficheros de configuración:
+ *interface/cmsis-dap.cfg* : que viene del repositorio oficial de OpenOCD y es para dispositivos que emplean el protocolo cmsis-dap. Acordaos de la salida del comando *dmesg* nuestro Keil Ulink2 es un dispositivo cmsis-dap.
+ mcb2300.cfg : nuestro fichero que configura velocidad de transmisión y más parámetros ya pensando en nuestra placa en concreto.

Se ejecuta así:
```
$ openocd -f interface/cmsis-dap.cfg -f mcb2300.cfg
```

Y si todo está correcto, la salida debería darnos:
```
$ openocd     -f interface/cmsis-dap.cfg     -f mcb2300.cfg
Open On-Chip Debugger 0.12.0
Licensed under GNU GPL v2
For bug reports, read
	http://openocd.org/doc/doxygen/bugs.html
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
Info : CMSIS-DAP: SWD supported
Info : CMSIS-DAP: JTAG supported
Info : CMSIS-DAP: FW Version = 1.0
Info : CMSIS-DAP: Interface Initialised (JTAG)
Info : SWCLK/TCK = 0 SWDIO/TMS = 1 TDI = 1 TDO = 1 nTRST = 0 nRESET = 1
Info : CMSIS-DAP: Interface ready
Info : clock speed 500 kHz
Info : cmsis-dap JTAG TLR_RESET
Info : cmsis-dap JTAG TLR_RESET
Info : JTAG tap: lpc2378.cpu tap/device found: 0x4f1f0f0f (mfg: 0x787 (<unknown>), part: 0xf1f0, ver: 0x4)
Info : Embedded ICE version 7
Error: EmbeddedICE v7 handling might be broken
Info : lpc2378.cpu: hardware has 2 breakpoint/watchpoint units
Info : starting gdb server for lpc2378.cpu on 3333
Info : Listening on port 3333 for gdb connections
```