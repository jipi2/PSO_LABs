# Proiect PSO - Linux_Keylogger

In cadrul proiectului ne propunem spre implementare 2 versiuni de "keylogger":

- Colectarea tastelor apasate de catre utilizator prin intermediul fisierelor de tip device din directorul /dev/input.

  ## Logica:

  - Iteram peste fisierele din directorul /dev/input.
  - Verificam daca fisierul curent permite inregistrarea tastelor, in caz afirmativ verificam daca inputurile sunt litere, cifre sau simblouri.

---

- Colectarea tastelor apasate de catre utilizator prin intermediul intreruperilor.

  ## Logica:

  - Implementam propria subrutina de tratare a intreruperilor responsabile pentru preluarea semnalelor provenite de la tastatura si le vom intercepta.
  - Realizam corespondenta intre codurile tastaturii si numele tastelor.

---

Fiecare tip de "keylogger" va fi implementat sub forma unui modul de Kernel, fapt ce implica caracterul de anonimitate. Caracterele vor fi salvate intr-un fisier .txt .

## Planul de testare

Pentru a demonstra functionalitatea implementarilor vom tasta caractere aleatoare intr-o perioada predefinita de timp si vom analiza ulterior fisierul .txt .

Prima versiune de keylogger se poate testa astfel:

- se descarca fisierele **EventsPython.py**, **readEvents.c** si **script.sh**.
- se ruleaza fisierul **script.sh** cu drepturi de **root** (sudo).
- se va crea un fisier **"store.txt"** in care se salveaza fiecare tasta apasata.
- acest fisier se va putea vizualiza ori de cate ori se doreste acest lucru, tastele fiind salvate in continuare.
