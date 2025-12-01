## Instrucciones de uso
### Comandos parte 1:
Para compilar
```
gcc main.c -o monitor -pthread monitor.c
```
Para ejecutar
```
./monitor [número de hebras] [número de tapas]
```
### Comandos parte 2:
Para compilar
```
gcc sim.c -o sim funciones.c
```
Para ejecutar
```
./sim [Cantidad de marcos] [Tamaño del marco] [-verbose] [archivo de texto con las direcciones virtuales]
```
verbose es opcional, para mostrar información adicional sobre la ejecución
