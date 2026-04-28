<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=160&section=header&text=sistemas-operativos&fontSize=30&fontColor=000000&fontAlignY=40&desc=Sistemas%20Operativos%20%7C%20UAH%20GII%202025-26&descAlignY=60&descColor=333333" width="100%"/>

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
**Profesor:** Julia Maria Clemente Parraga

Study of operating system internals with a focus on **process and thread management**, inter-process communication and synchronization under Linux using POSIX services. All practices implemented in C.

---

## Topics covered

| Topic | Content |
|-------|---------|
| Processes | fork, exec, wait, waitpid, process lifecycle, zombie processes |
| Signals | sigaction, SIGCHLD, SA_RESTART, SA_NOCLDSTOP, kill |
| Pipes | pipe, dup, dup2, anonymous pipes, I/O redirection |
| POSIX Threads | pthread_create, pthread_join, pthread_exit, thread lifecycle |
| Synchronization | Mutexes (pthread_mutex_t), race conditions, critical sections |
| Shell internals | Command parsing, background execution, sequence of commands |
| Dev tools | gcc, make, gdb, static libraries (ar), Makefile design |

---

## Practices

| # | Folder | Description | Key concepts |
|---|--------|-------------|--------------|
| P2 | [practica2-minishell](./practica2-minishell/) | Unix shell interpreter (minishell) implemented in C — foreground and background execution, pipes, I/O redirection, internal commands (cd, pwd, umask, declare), sequence of commands with `;` | fork, exec, wait, pipe, dup2, sigaction, static library |
| P3 | [practica3-hilos](./practica3-hilos/) | Race simulation with N POSIX threads — mutex-protected shared classification array, independent rand_r() seeds per thread, zero race conditions | pthreads, mutex, race conditions, rand_r |

---

## Repository structure

```
sistemas-operativos/
├── practica2-minishell/
│   ├── minishell.c              # Main loop + SIGCHLD handler
│   ├── ejecutar.c / .h          # fork/exec, pipes, command sequences
│   ├── parser.c / .h            # Lexical parser (state machine)
│   ├── internas.c / .h          # Internal commands: cd, pwd, umask, declare
│   ├── entrada_minishell.c / .h # Prompt and line reading
│   ├── redirecciones.c / .h     # I/O redirection: < and >
│   ├── libmemoria.c / .h        # Dynamic memory management
│   └── Makefile                 # Builds modules + libshell.a static library
└── practica3-hilos/
    ├── simula_car.c             # Race simulation with POSIX threads
    └── Makefile                 # gcc -Wall -g -pthread
```

---

## Compilation

```bash
# Practica 2 — minishell
cd practica2-minishell
make
./minishell

# Practica 3 — simula_car
cd practica3-hilos
make
./simula_car
```

---

## Key POSIX APIs used

```c
/* Process management */
pid_t fork();
int execvp(const char *file, char *const argv[]);
pid_t waitpid(pid_t pid, int *status, int options);

/* Signals */
int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

/* Pipes and I/O */
int pipe(int pipefd[2]);
int dup2(int oldfd, int newfd);

/* Threads */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start)(void *), void *arg);
int pthread_join(pthread_t thread, void **retval);

/* Synchronization */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

---

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=FCC624&height=100&section=footer" width="100%"/>

*Sistemas Operativos · UAH GII · 2025-26*
</div>
