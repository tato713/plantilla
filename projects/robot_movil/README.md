# Trabajo Práctico Final - Sistemas Operativos de Tiempo Real - Zanet, Osvaldo Marcos - Robot Móvil Cuadrúpedo Control Remoto por Bluetooth

## Descripción General

El trabajo consiste en el desarrollo e implementación de un robot móvil capaz de caminar utilizando 5 servos SG90 para replicar el movimiento de un reptil. La plataforma utilizada fue la placa de desarrollo EDU-CIAA, aunque en el diseño original iba a ser portado a la placa Pico-CIAA. La comunicación con el robot se lleva a cabo por bluetooth utilizando la aplicación para Android "Bluetooth RC Controller"

## Diseño del Robot

El robot consta de un cuerpo flexible en el plano XY en el centro del torso; cuatro patas articuladas horizontalmente. Cada pata tiene un servo que le moverse de arriba a abajo. El quinto servo flexiona el cuerpo en su articulación antes mencionada. Todo el sistema es alimentado por una batería de litio del formato 18650 y su módulo cargador.
![Cuerpo](https://github.com/tato713/plantilla/blob/master/projects/robot_movil/med/cuerpo.png) Format: ![Alt Text](url)

## Comunicación Bluetooth

Para poder recibir comandos el robot usa un módulo bluetooth HC-06 conectado a los puertos Rx y Tx de la UART RS232 de la placa. Una tarea llamada "Bluetooth" en "robot.c" lee los bytes recibidos de la UART cada 50 ms (son enviados desde la aplicación cada 50 ms), interpreta cada byte y define la velocidad, dirección, o si está activo o no. En caso de no detectar ninguna señal entra en un estado de "reposo" poniendo en 0 las posiciones de los servos. La tarea de comunicación tendrá prioridad sobre las otras.

![App Android](https://github.com/tato713/plantilla/blob/master/projects/robot_movil/med/Screenshot_20190608-235119_Bluetooth%20RC%20Controller.jpg) Format: ![Alt Text](url)

## Movimiento de los Servos

Los servos utilizados (SG90) pueden moverse un ángulo de 180° desde una posición, variando el ciclo de trabajo desde aproximadamente 0,9 ms a 2,5 ms, cambiando el ángulo en sentido antihorario. Para controlar los servos hice un driver que controla las salidas de PWM por hardware. Ésta permite inicializar y cambiar el ciclo de trabajo de 8 de las 9 salidas PWM. La función que inicializa recibe de parámetro una máscara de los servos a inicializar y la frecuencia de trabajo; la función que los mueve recibe la máscara y un ángulo al cuál debe moverse. Este ángulo lo determino arbitrariamente a ser de crecimiento positivo en sentido horario, porque así lo creí más fácil de entender.
	
## Coordinación de Movimiento y Comunicación entre Tareas

Para simplificar el movimiento, cada pata (y el torso) sigue una secuencia de posiciones definidas para sí misma. Una tarea llamada "Moverse" controla en qué dirección cambia la secuencia, y si es que cambia. Cada servo tiene una tarea que lo controla y la velocidad en que cambia de punto en la secuencia es el delay al finalizar una de estas tareas. Cada tarea antes mencionada tiene un take y un give de un semáforo para no superponerse. La secuencia de cada servo fue modelada como funciones senoidales, pero se plantearon otras alternativas.

![Video](https://drive.google.com/drive/folders/1gN62DL2nefFQeKDxOLYt7YpzKAHHRn8V?usp=sharing) Format: ![Alt Text](url)