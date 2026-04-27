<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=160&section=header&text=sistemas-operativos&fontSize=30&fontColor=000000&fontAlignY=40&desc=Sistemas%20Operativos%20%7C%20UAH%202025-26&descAlignY=60&descColor=333333" width="100%"/>

<div align="center">

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![POSIX](https://img.shields.io/badge/POSIX-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![UAH](https://img.shields.io/badge/UAH-GII-085041?style=for-the-badge)
![Status](https://img.shields.io/badge/Status-Active-1D9E75?style=for-the-badge)

</div>

---

## About

**Asignatura:** Sistemas Operativos · UAH GII · Curso 2025-26

Study of operating system internals with a focus on **process and thread management**, synchronization mechanisms and memory models under Linux using POSIX services. All practices implemented in C.

---

## Topics covered

| Topic | Content |
|-------|---------|
| Processes | fork, exec, wait, process lifecycle, IPC |
| POSIX Threads | pthread_create, pthread_join, pthread_exit, thread lifecycle |
| Synchronization | Mutexes (pthread_mutex_t), semaphores, condition variables |
| Race conditions | Detection, prevention, critical sections |
| Memory management | Virtual memory, paging, segmentation |
| File systems | File descriptors, pipes, signals |

---

## Practices

| # | Name | Description | Key concepts |
|---|------|-------------|--------------|
| P3 | [simula-car](./practica3-hilos/) | Race simulation with N threads, mutex-protected shared classification array, independent rand_r() seeds per thread | pthreads, mutex, race conditions |

---

## Compilation

```bash
gcc programa.c -o programa -lpthread
./programa
```

---

## Key APIs

```c
// Thread management
pthread_create(&thread, NULL, function, arg);
pthread_join(thread, NULL);
pthread_exit(NULL);

// Synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
// critical section
pthread_mutex_unlock(&mutex);
pthread_mutex_destroy(&mutex);
```

---

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=100&section=footer" width="100%"/>

*Sistemas Operativos · UAH GII · 2025-26*
</div>
