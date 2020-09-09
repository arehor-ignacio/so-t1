### Ejercicio 1
 - ¿Cuántos procesos se lanzan y qué comportamiento se puede observar de cada uno?
		Se lanza un proceso y este se forkea. Se definen dos handlers para las señales SIGINT y SIGURG para el proceso padre e hijo respectivamente. El padre escribe "sup!" y envía la señal SIGURG al hijo mediante la función `kill`. El handler de esta señal imprime "ya va!" y suma uno a un contador. Cuando este contador llega a 4, el proceso hijo le envía la señal SIGINT a su padre, y este se pone a esperar a que el proceso del hijo termine. Si el contador no es 4, se repite lo que hace el padre.

- Sí, utilizan la función y syscall `kill`. Se usa para enviarseñales entre el proceso padre e hijo. El padre le comunica al hijo mediante la función kill cuando debe este imprimir en pantalla. Por otra parte, el hijo le comunica al padre mediante esta función cuando debe esperar y dejar de imprimir.
