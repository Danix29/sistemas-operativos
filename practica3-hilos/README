<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=160&section=header&text=Practica%203%20%E2%80%94%20POSIX%20Threads&fontSize=28&fontColor=000000&fontAlignY=40&desc=Race%20Simulation%20with%20Concurrent%20Threads%20%7C%20Sistemas%20Operativos%20%7C%20UAH%202025-26&descAlignY=60&descColor=333333" width="100%"/>

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![pthreads](https://img.shields.io/badge/pthreads-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![Concurrency](https://img.shields.io/badge/Concurrency-e24b4a?style=for-the-badge)

</div>

---

## Description

Simulation of a car race using **POSIX threads** in Linux. Each car is represented by an independent thread that races concurrently with a random delay. A **mutex-protected shared array** records the final classification without race conditions.

---

## Problem solved

Without synchronization, multiple threads accessing a shared classification array simultaneously produce **race conditions** — the final order would be inconsistent across executions. This practice demonstrates how to eliminate them using `pthread_mutex_t`.

```
Thread 0 ──────────── arrives 3rd ──▶ clasificacion[2] = &Coche0
Thread 1 ──────── arrives 2nd ────▶ clasificacion[1] = &Coche1   ← mutex protects
Thread 2 ─── arrives 1st ──────▶ clasificacion[0] = &Coche2       all of these
Thread 3 ──────────────── arrives 4th ▶ clasificacion[3] = &Coche3
```

---

## Implementation details

### Shared state protected by mutex

```c
coche_t *clasificacion[N_COCHES];   // shared classification array
volatile int coches_llegados = 0;   // shared arrival counter

pthread_mutex_t mutex_clasificacion = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_output        = PTHREAD_MUTEX_INITIALIZER;
```

### Thread function

```c
void *funcion_coche(void *arg) {
    coche_t *pcoche = (coche_t *)arg;

    // Independent seed per thread — avoids identical random sequences
    unsigned int semilla = pcoche->id + (unsigned int)time(NULL)
                         + (unsigned int)pthread_self();
    int retardo = rand_r(&semilla) % 10 + 1;

    // Protected stdout output
    pthread_mutex_lock(&mutex_output);
    printf("Salida de %s %d\n", pcoche->nombre, pcoche->id);
    pthread_mutex_unlock(&mutex_output);

    sleep(retardo); // simulate the race

    pthread_mutex_lock(&mutex_output);
    printf("Llegada de %s %d\n", pcoche->nombre, pcoche->id);
    pthread_mutex_unlock(&mutex_output);

    // Protected write to shared classification
    pthread_mutex_lock(&mutex_clasificacion);
    clasificacion[coches_llegados] = pcoche;
    coches_llegados++;
    pthread_mutex_unlock(&mutex_clasificacion);

    pthread_exit((void *)pcoche);
}
```

### Main — thread creation and join

```c
for (i = 0; i < N_COCHES; i++) {
    Coches[i].id = i;
    Coches[i].nombre = "Coche";
    pthread_create(&hilosCoches[i], NULL, funcion_coche, &Coches[i]);
}

for (i = 0; i < N_COCHES; i++) {
    pthread_join(hilosCoches[i], NULL); // wait for all cars to finish
}
```

---

## Why two mutexes?

| Mutex | Protects | Why separate? |
|-------|---------|---------------|
| `mutex_output` | `printf` calls | stdout is not thread-safe — interleaved output would be garbled |
| `mutex_clasificacion` | `clasificacion[]` + `coches_llegados` | shared data — concurrent writes would corrupt the final order |

---

## Why `rand_r()` instead of `rand()`?

`rand()` uses a global shared state — calling it from multiple threads without synchronization is itself a race condition. `rand_r(&semilla)` is **re-entrant**: each thread has its own seed in a local variable, so no locking is needed for random number generation.

The seed combines `pcoche->id + time(NULL) + pthread_self()` to guarantee distinct sequences even when threads are created in the same second.

---

## Example output

```
Se inicia proceso de creacion de hilos...

SALIDA DE COCHES
Salida de Coche 0
Salida de Coche 3
Salida de Coche 1
Salida de Coche 5
Salida de Coche 2
Salida de Coche 4
Salida de Coche 6
Salida de Coche 7
Proceso de creacion de hilos terminado

Llegada de Coche 3
Llegada de Coche 1
Llegada de Coche 0
Llegada de Coche 5
Llegada de Coche 2
Llegada de Coche 4
Llegada de Coche 7
Llegada de Coche 6
Todos los coches han LLEGADO A LA META

CLASIFICACION FINAL:
1º Coche 3
2º Coche 1
3º Coche 0
4º Coche 5
5º Coche 2
6º Coche 4
7º Coche 7
8º Coche 6
```

> Order varies each execution due to random delays — this is expected behaviour.

---

## Compilation and execution

```bash
make          # gcc -Wall -g -pthread simula_car.c -o simula_car
./simula_car
make clean    # rm -f simula_car
```

---

## POSIX services used

| Service | Purpose |
|---------|---------|
| `pthread_create()` | Create one thread per car |
| `pthread_join()` | Main thread waits for all cars to finish |
| `pthread_exit()` | Thread terminates returning a pointer to its car struct |
| `pthread_mutex_lock()` | Acquire mutex before entering critical section |
| `pthread_mutex_unlock()` | Release mutex after critical section |
| `pthread_mutex_destroy()` | Free mutex resources at program end |
| `rand_r()` | Re-entrant random — independent per thread |
| `sleep()` | Simulate race duration |

---

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=100&section=footer" width="100%"/>

*Practica 3 · Sistemas Operativos · UAH GII · 2025-26*
</div>
