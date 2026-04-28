<img src="https://capsule-render.vercel.app/api?type=waving&color=00599C&height=160&section=header&text=Practica%202%20%E2%80%94%20Minishell&fontSize=30&fontColor=FFFFFF&fontAlignY=40&desc=Unix%20Shell%20Interpreter%20in%20C%20%7C%20Sistemas%20Operativos%20%7C%20UAH%202025-26&descAlignY=60&descColor=9FE1CB" width="100%"/>

<div align="center">

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![POSIX](https://img.shields.io/badge/POSIX-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![Make](https://img.shields.io/badge/Make-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)

</div>

---

## Description

Partial implementation of a Unix shell interpreter (**minishell**) in C using POSIX system calls. The shell supports foreground and background command execution, I/O redirection, anonymous pipes, command sequences and internal commands.

Built as a multi-module project with a static library (`libshell.a`) compiled via Makefile.

---

## Features implemented

| Phase | Feature | Status |
|-------|---------|--------|
| 1 | Command execution loop | Done |
| 2 | External commands — foreground | Done |
| 3 | External commands — background (`&`) | Done |
| 4 | Makefile with static library | Done |
| 5 | Command sequences (`;`) | Done |
| 6 | I/O redirection (`<`, `>`) | Done |
| 7 | Anonymous pipes (`|`) | Done |

**Internal commands:** `cd`, `pwd`, `umask`, `declare`, `exit`

---

## Architecture

```
minishell.c
    │
    ├── entrada_minishell   →  prompt + line reading (fgets)
    │
    ├── internas            →  cd · pwd · umask · declare
    │       └── parser      →  lexical parser (state machine)
    │
    └── ejecutar            →  fork · exec · wait · pipes
            ├── parser      →  tokenises each command
            └── redirecciones →  open · dup2 for < and >
```

```
libshell.a  ←  parser.o + internas.o  (static library)
```

---

## Module breakdown

| File | Role |
|------|------|
| `minishell.c` | Main loop, `exit` command, SIGCHLD handler to avoid zombie processes |
| `ejecutar.c` | `fork()` + `execvp()` per command, pipe creation, sequence parsing with `;`, background support |
| `parser.c` | Lexical analyser — splits line into tokens, detects `<`, `>`, `>>`, `2>`, `&`, `\|` |
| `internas.c` | Internal command table and dispatch: `cd`, `pwd`, `umask`, `declare` |
| `entrada_minishell.c` | `imprimir_prompt()` and `leer_linea_ordenes()` using `fgets` |
| `redirecciones.c` | `redirec_entrada()` and `redirec_salida()` using `open()` and `dup2()` |
| `libmemoria.c` | `free_argumentos()` and `free_ordenes_pipes()` — memory cleanup |

---

## Key implementation details

### SIGCHLD handler — avoiding zombie processes
```c
static void manejar_sigchild(int signo) {
    int estado;
    waitpid(-1, &estado, WNOHANG); // collect any finished child, non-blocking
}

// Installed in main():
struct sigaction sa;
sa.sa_handler = manejar_sigchild;
sa.sa_flags = SA_NOCLDSTOP | SA_RESTART;
sigaction(SIGCHLD, &sa, NULL);
```

### Pipe execution flow
```c
// For N commands connected by pipes:
// cmd1 | cmd2 | cmd3
//
// pipes[0]: cmd1 writes → cmd2 reads
// pipes[1]: cmd2 writes → cmd3 reads
//
// Parent closes all pipe ends after forking children
// to avoid blocking on EOF
```

### I/O redirection
```c
// In child process (ejecutar_orden):
if (entrada != STDIN_FILENO) {
    dup2(entrada, STDIN_FILENO);
    close(entrada);
}
if (salida != STDOUT_FILENO) {
    dup2(salida, STDOUT_FILENO);
    close(salida);
}
execvp(args[0], args);
```

---

## Compilation and execution

```bash
make          # builds minishell + libshell.a
./minishell   # launches the shell

# Inside minishell:
minishell> ls -l
minishell> ls | grep .c
minishell> cat archivo.txt | wc -l
minishell> ls > salida.txt
minishell> cat < entrada.txt
minishell> sleep 5 &
minishell> ls ; pwd ; echo hola
minishell> cd /tmp
minishell> exit
```

---

## Makefile structure

```makefile
# Builds modules independently then links
minishell: libmemoria.o entrada_minishell.o ejecutar.o internas.o minishell.o libshell.a redirecciones.o
    gcc -g -Wall *.o -L./ -lshell -o minishell

# Static library from parser + internas
libshell.a: parser.o internas.o
    ar -rs libshell.a parser.o internas.o

.PHONY: clean
clean:
    rm -f minishell *.o libshell.a
```

---

<div align="center">
<img src="https://capsule-render.vercel.app/api?type=waving&color=00599C&height=100&section=footer" width="100%"/>

*Practica 2 · Sistemas Operativos · UAH GII · 2025-26*
</div>
