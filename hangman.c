#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/select.h>
#include <ncurses.h>


/* toate literele sunt transformate in
litere mici, chiar daca nu sunt majuscule */
char *lower_all_characters (char *buffer)
{
    int i;
    int k = strlen(buffer); // numarul de caractere al string-ului pe care lucram
    for (i = 0; i < k; i++)
    {
      if (isalpha(buffer[i])) // daca caracterul este litera, il transformam in litera mica
        buffer[i] = tolower(buffer[i]); // litera, fie mica sau mare, devine litera mica
    }
    return buffer; // este returnat string-ul pe care am lucram
}

/* se sterg caracterele neprintabile */
char *printable_characters (char *buffer) 
{
    int i, j = 0; // i reprezinta pozitia unui caracter in string-ul buffer, iar j pozitia unui caracter in string-ul returnat de functie
    char *buffer_printable; // string-ul care contine doar caractere printabile
    int k = strlen(buffer); // dimensiunea string-ului luat ca argument pe care lucram
    buffer_printable = malloc(50*sizeof(char)); // alocare de memorie dinamica a string-ului ce va fi returnat
    for (i = 0; i < k; i++) // se acceseaza string-ul luat ca argument
    {
        if(isprint(buffer[i])) // daca caracterul e printabil, va fi copiat in string-ul ce va fi returnat
        {
            buffer_printable[j] = buffer[i]; // copierea efectiva
            j++; // se trece la urmatoarea pozitie in string-ul care contine doar caractere printabile
        }
    }
    return buffer_printable; // se returneaza string-ul care contine doar caractere printabile
    free(buffer_printable);
}
/*
se verifica daca un caracter este prezent
intr-un sir de caractere
*/
int is_char_present (char *buffer, char c) // se verifica daca un caracter e prezent intr-un sir de caractere
{
    int i, nr = 0; // i este pozitia unui caracter in string-ul luat ca argument, iar nr numarul de aparitii al caracterului c in string
    int k = strlen(buffer); // numarul de caractere al string-ului buffer luat ca parametru
    for (i = 0; i < k; i++) // se acceseaza string-ul luat ca parametru
    {
       if (c == buffer[i]) // daca c se afla in string-ul buffer, este incrementat numarul de aparitii al acestuia in string
          nr++;
    }
    if (nr == 0) // daca nu apare deloc in sirul de caractere, functia returneaza valoarea 0, altfel returneaza 1
        return 0; // nu apare in sir
    else
        return 1; // apare in sir
}

char *last_characters (char *buffer) //
{
   char *last_ones; // vectorul cu ultimele caractere ale cuvintelor de cel putin 3 caractere din expresia luata ca parametru al functiei
   last_ones = malloc(100*sizeof(char)); // alocarea dinamica a vectorului de caractere
   int k = strlen(last_ones); // numarul de caractere al vectorului rezultat
   char *cuvant; // fiecare cuvant din expresia luata ca parametru
   int p; // numarul de caractere al fiecarui cuvant din expresie
   cuvant = strtok(buffer, " "); // cuvant din expresie
   while (cuvant != NULL) // spargerea cuvant cu cuvant a expresiei
   { p = strlen(cuvant); 
     if (p > 2) // daca cuvantul are cel putin 3 caractere, ultimul caracter e copiat in vector
        last_ones[k++] = cuvant[p - 1]; // copierea in vectorul ce va fi returnat a ultimei litere din fiecare cuvant ce indeplineste criteriul de mai sus
     cuvant = strtok(NULL, " "); // trecerea la urmatorul cuvant din expresie, daca exista
   }
   return last_ones; // este returnat vectorul
   free(last_ones); // eliberarea memoriei ocupata de vectorul returnat
}

char *first_characters (char *buffer) //ok
{
   char *first_ones; // vectorul cu ultimele caractere ale cuvintelor de cel putin 2 caractere din expresia luata ca parametru al functiei
   first_ones = malloc(100*sizeof(char)); // alocarea dinamica a vectorului de caractere
   int k = strlen(first_ones); // numarul de caractere al vectorului rezultat
   char *cuvant; // fiecare cuvant din expresia luata ca parametru
   int p; // numarul de caractere al fiecarui cuvant din expresie
   cuvant = strtok(buffer, " "); // cuvant din expresie
   while (cuvant != NULL) // spargerea cuvant cu cuvant a expresiei
   { p = strlen(cuvant);
     if (p != 1) // daca cuvantul are cel putin 2 caractere, primul caracter e copiat in vector
        first_ones[k++] = cuvant[0]; // copierea in vectorul ce va fi returnat a primei litere din fiecare cuvant ce indeplineste criteriul de mai sus
     cuvant = strtok(NULL, " "); // trecerea la urmatorul cuvant din expresie, daca exista
   }
   return first_ones; // este returnat vectorul
   free(first_ones); // eliberarea memoriei ocupata de vectorul returnat
}

void show_input (char *buffer) // se afiseaza caracterele citite de la tastatura sau selectate aleatoriu
{
  int i; // pozitia unui caracter in string-ul buffer
  mvprintw(13, 20, "Inputed characters:"); // mesaj ce va aparea in timpul jocului
  int k = strlen(buffer);
  for (i = 0; i < k; i++) // se acceseaza caracterele string-ului buffer
    mvprintw(13, 39 + 2*i, "%c", buffer[i]); // sunt afisate caractere valide citite de la tastatura sau luate aleatoriu
}

/* se codifica cuvantul, propozitia
sau fraza care trebuie ghicita */
char *string_codified (char *buffer)
{
  char *cuvant; // cuvant din expresia buffer luata ca parametru al functiei
  char *mesaj; // expresia codificata ce va fi returnata de functie
  char c, a, b; // caractere-suport ce vor fi utilizate in codificarea cuvintelor din expresie
  int i; // pozitia unui caracter din fiecare cuvant al expresiei ce va fi codificata
  char *buffer_copy_1, *buffer_copy_2; // copii de siguranta ale expresiei buffer, pe care vom lucra in continuare pentru a codifica expresia
  buffer_copy_1 = malloc((strlen(buffer)+1)*sizeof(char)); // alocare de memorie pentru buffer_copy_1
  buffer_copy_2 = malloc((strlen(buffer)+1)*sizeof(char)); // alocare de memorie pentru buffer_copy_2
  strcpy(buffer_copy_1, buffer); // se copiaza buffer in buffer_copy_1
  strcpy(buffer_copy_2, buffer); // se copiaza buffer in buffer_copy_2
  char *last, *first; // vectorii cu ultimele si respectiv primele caractere ale cuvintelor din expresie
  first = malloc(40*sizeof(char));
  last = malloc(40*sizeof(char));
  first = first_characters(buffer_copy_1);
  last = last_characters(buffer_copy_2);
  mesaj = malloc(100*sizeof(char)); // mesajul final
  cuvant = strtok(buffer, " "); // cuvant din expresia luata ca parametru
  while (cuvant != NULL) // spargerea pe cuvinte a expresiei buffer luata ca parametru
  {   int k = strlen(cuvant);
      switch(k)
      {
      case 1: // daca cuvantul are un singur caracter, nu este prima si nici ultima litera a unui cuvant cu minim 2 caractere, e inlocuit cu _
         c = cuvant[0]; // se salveaza prima litera (de fapt singura litera a cuvantului) pentru compararea cu celelalte cuvinte din expresie)
         if ((!is_char_present(first, c))&&(!is_char_present(last, c))) // daca se indeplineste conditia pomenita mai sus, are loc inlocuirea
            strcpy(cuvant, "_"); // caracterul e inlocuit cu _
            break;
      // daca cuvantul are doua caractere, al doilea caracter nu este primul si nici ultimul caracter a unui cuvant cu minim 2 caractere, e inlocuit cu _      
      case 2: 
         a = cuvant[1]; // se salveaza a doua litera pentru compararea cu celelalte cuvinte
         if ((!is_char_present(first, a))&&(!is_char_present(last, a)))
             cuvant[1] = '_'; // al doilea (ultimul) caracter e inlocuit cu _
         break;
       /* daca cuvantul are cel putin 3 caractere, caracterele cu exceptia primului si al ultimul sunt inlocuite cu _, 
         doar daca nu sunt primul sau ultimul caracter a unui cuvant din expresie */
      default:
         for (i = 1; i < k - 1; i++)
         {  b = cuvant[i]; // se salveaza al (i+1)-lea caracter din cuvant pentru comparare
            if ((!is_char_present(first, b))&&(!is_char_present(last, b)))
                 cuvant[i] = '_'; // daca sunt indeplinite conditiile de mai sus, caracterul e inlocuit cu _
         }
         break;
      }
      strcat(mesaj, cuvant); // se adauga cuvantul codificat la expresia codificata
      strcat(mesaj, " "); // dupa fiecare cuvant se adauga spatiul de rigoage
      cuvant = strtok(NULL, " "); // se trece la urmatorul cuvant din expresie
  }
  mesaj[strlen(mesaj)-1] = '\0'; // se taie ultimul caracter din expresie, care este spatiul adaugat mai devreme
  return mesaj; // se returneaza mesajul
  // se elibereaza memoria alocata pentru urmatoarele variabile
  free(mesaj);
  free(last);
  free(first);
  free(buffer_copy_1);
  free(buffer_copy_2);

}

int is_string_printable (char *buffer) //se verifica daca string-ul este printabil
{
   int i, nr = 0; // i este pozitia unui caracter in buffer, iar nr numarul de caractere neprintabile
   int k = strlen(buffer); // numarul de caractere al string-ului buffer
   for (i = 0; i < k; i++)
   {
      if (!isprint(buffer[i])) // daca un caracter nu este printabil, este incrementat nr
        nr++;
   }
   if (nr == k) // daca numarul de caractere neprintabile e egal cu numarul total de caractere al string-ului se retuneaza 0, altfel e returnat 1
      return 0; // string-ul nu e printabil
   else
      return 1; // string-ul e printabil

}

char *generate_stack (char *buffer) // se genereaza expresia codificata, aici fixand cateva bug-uri din joc
{
  int k = strlen(buffer); // numarul de caractere al expresiei luate aleatoriu
  char *copy; // copie de siguranta
  copy = malloc(100*sizeof(char));
  copy = strdup(buffer); // copierea expresiei luata ca parametru
  char *guess; // expresia codificata (cu 6 caractere adaugate datorita unui bug sau nu)
  guess = malloc(100*sizeof(char));
  guess = string_codified(copy); // codificarea expresiei
  int b = strlen(guess); // numarul de caractere al expresiei codificate
  char *stack; // expresia codificata, lipsita de cele eventuale 6 caractere adaugate la inceput anapoda
  stack = malloc(100*sizeof(char));
  /* prin aceasta modalitate rezolvam un bug al programului, in care sunt adaugate 6 caractere aiurea la inceputul string-ului codificat
  (incepand cu al doilea joc) si le taiem in caz ca acestea apar (prin compararea numarului de caractere al string-ului ales random cu
  numarul de caractere al expresiei codificate), pentru bunul mers al jocului;*/
  if (b == k) 
      strcpy(stack, guess); 
    else
      strcpy(stack, guess + 6);
  return stack; // este returnata expresia codificata
  free(copy);
  free(guess);
  free(stack);
}

char *random_element(char **matrix, int j) // se alege in mod aleatoriu expresia jocului
{
   srand(time(NULL)); // activarea procesului de random choosing
   int random_position = rand() % j; // se alege aleatoriu linia din matricea corespunzatoare unui sir de caracter
   return matrix[random_position]; // se returneaza string-ul ales aleatoriu
}

void display_time () //se afiseaza ora si data in timpul jocului
{
  char hour[40]; // ora curenta
  char date[40]; // data curenta
  struct tm *hangman_time; // timpul
  time_t game_time; // timpul
  time(&game_time); // se obtine timpul curent
  hangman_time = localtime(&game_time); // timpul curent e salvat in aceasta variabila
  strftime(date, 40, "%A %d %B %Y", hangman_time); // se formateaza data
  strftime(hour, 40, "%H:%M:%S", hangman_time); // se formateaza ora
  mvprintw(14, 20, "Date: %s\n", date); // se afiseaza data curenta
  mvprintw(15, 20, "Time: %s\n", hour); // se afiseaza ora curenta
  refresh();
}

void instructions () // instructiuni suplimentare
{
  mvprintw(25, 20, "Press Q to go to the menu.\n"); // instructiunile din cadrul jocului propriu-zis
}

void clean_string(char *buffer) // este golit sirul de caractere luat ca parametru
{
   int k = strlen(buffer); //numarul de caractere al string-ului buffer luat ca parametru
   int i; // pozitia unui caracter in string
   for (i = k - 1; i >= 0; i--) // se acceseaza caracterele string-ului de la coada la cap
      buffer[i] = '\0'; // golirea caracter cu caracter al string-ului
}



void menu(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]);
void game_instructions(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]);
void dickbutt(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]);
void no_easter_egg(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]);

void game_play(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50])
{
  initscr(); // initierea ferestrei de joc
  start_color(); 
  init_pair(1, COLOR_WHITE, COLOR_BLUE); // perechea 1 de culori - scris alb, background albastru
  init_pair(2, COLOR_GREEN, COLOR_BLUE); // perechea 2 de culori - scris verde, background albastru
  init_pair(3, COLOR_RED, COLOR_BLUE); // perechea 3 de culori - scris rosu, background albastru
  bkgd(COLOR_PAIR(1)); // background albastru
  attron(COLOR_PAIR(1)); // perechea de culori va putea fi folosita in fereastra
  curs_set(0); // cursorul nu apare pe ecran
  keypad(stdscr, TRUE); // interactiunea utilizatorului cu tastatura
  srand(time(NULL)); 
  int user_response; // raspunsul utilizatorului in interval de 7 secunde
  /* aici vom incercam sa detectam daca, intr-un interval de timp, s-a tastat ceva la tastatura */
  struct timeval user_time_out; // intervalul de timp de tastare
  fd_set game_input_waiting;
  FD_ZERO(&game_input_waiting);
  FD_SET(0, &game_input_waiting);
  user_time_out.tv_sec = 7; //secunde
  user_time_out.tv_usec = 0; //milisecunde 
  refresh();
  int i; // variabila folosita in for-ul din while, pentru decodificarea expresiei
  cbreak();
  mvprintw(0, 0, "________________  \n"); //suportul de la spanzuratoare
  refresh(); 
  mvprintw(7, 20, "Guess the word: %s", stack); // se afiseaza expresia codificata
  refresh();
  char c; // caracterul citit de la tastatura
  noecho(); // nu se afiseaza caracterul citit de la tastatura, altfel apare in mod aiurea pe ecran
  refresh();
  int number_of_lifes = 6; // numarul de vieti
  int wrong_guesses = 0; // numarul de raspunsuri gresite
  int remaining_lifes = 0; //numarul de vieti ramase
  game_end = 0; // jocul este inceput si nu terminat
  game_paused = 0; // jocul nu e in starea de pauza
  // daca s-a atins numarul de greseli maxim admise sau daca s-a completat expresia codificata se termina jocul
  while ((wrong_guesses < number_of_lifes)&&(strcmp(random_string, stack)!=0)) 
  {  
    instructions (); // se afiseaza o legenda cu instructiunile jocului
    display_time (); // se afiseaza data si timpul curente, care se reactualizare cand se tasteaza sau se alege random un nou caracter
    mvprintw(5, 20, "Expresii valide: %d", j); // se afiseaza numarul de expresii valide
    mvprintw(6, 20, "Numar total de caractere: %ld", total_number_of_characters); // se afiseaza numarul total de caractere din matricea de string-uri
    refresh();
    user_response = select(1, &game_input_waiting, NULL, NULL, &user_time_out); // intervalul de timp intre doua interactiuni ale utilizatorului cu tastature
    if (user_response == 1) // daca utilizatorul nu raspunde dupa 7 secunde se alege aleatoriu un caracter ce nu a mai fost tastat
    {
      c = getch(); // daca raspunde in maxim 7 secunde, este citit un caracter
    }
    else // altfel se alege random un caracter 
    {
      c = rand() % 95 + 32; // alegerea random a unui caracter printabil 
      // daca caracterul ales este litera mare sau se afla deja in input sau deja in expresia codificata, se alege altul
      while (((c >= 'A')&&(c <= 'Z'))||(is_char_present(input, c))||(is_char_present(stack, c)))
         c = rand() % 95 + 32;
    } 
    if (c == 'Q') // se apasa Q pentru a pune in pauza jocul, cu revenirea in meniu
    {
      game_end = 0; // jocul nu s-a terminat
      game_paused = 1; // jocul este in starea de pauza
      erase();
      menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // se revine in meniu
    }
    mvprintw(8, 20, "%c\n", c); // se afiseaza caracterul citit sau luat random
    refresh();
    int k = strlen(random_string); // numarul de caracte
    FD_SET(0, &game_input_waiting); // se reactualizeaza intervalul de timp al interactiunii utilizatorului cu tastatura
    user_time_out.tv_sec = 7; // timpul de 7 secunde de asteptare a tastarii unui caracter
    user_time_out.tv_usec = 0; // timpul de 0 milisecunde de asteptare a tastarii unui caracter
    if (is_char_present(input, c))
       mvprintw(9, 20, "You've already typed that! Type another one!\n"); // caracterul a fost deja tastat sau luat random
       else
          if (is_char_present(stack, c))
            mvprintw(9, 20, "This is already here! Type another one!\n"); // caracterul era deja in expresia codificata, inainte de inceperea propriu-zisa a jocului
          else
            /* 
            daca caracterul se afla in expresia originala, ea este adaugata in expresia codificata si caracterul e adaugat
            in vectorul de input pentru ca la a doua tastare a acestuia el sa fie ignorat, altfel daca nu se afla caracterul in
            expresia originala creste numarul de greseli, scade numarul de vieti si de asemenea este adaugat in vectorul de input
            */
            if (is_char_present(random_string, c))
            {
              for (i = 0; i < k; i++) // se acceseaza expresia codificata si expresia originala luata random din matricea de string-uri
              {
                 if (c == random_string[i]) // se verifica daca caracteru
                   stack[i] = c; // se inlocuieste cu caracterul citit sau luat random in pozitiile in care apare in expresia originala
              }
               refresh();
               input[strlen(input)] = c; // se adauga caracterul citit sau luat random in vectorul de input
               mvprintw(9, 20, "Go on: %s\n", stack); // se afiseaza expresia codificata cu caracterul citit adaugat la aceasta
               refresh();
            }
            else if ((c >= 65) && (c <= 90) && (c != 'Q')) // in caz ca este tastata o litera mare (cu exceptia Q), programul va ignora majuscula respectiva
                {
                  mvprintw(9, 20, "Warning! Upcase letter detected. Please input something else.\n"); // mesaj de avertizare in caz ca este tastata o litera mare, care nu va fi inclusa in input
                  refresh();
                }
            else
              {
                wrong_guesses++;
                remaining_lifes = number_of_lifes - wrong_guesses;
                input[strlen(input)] = c;
                mvprintw(9, 20, "You are wrong! You have lost a life. You have %d lives\n", remaining_lifes);
                refresh();
              }
        show_input(input); // se afiseaza ce a fost tastat sau luat random pentru ca utilizatorul sa nu mai tasteze caracterele ce se afla deja in vectorul input
        refresh();
     switch(wrong_guesses) //in functie de numarul de greseli, se deseneaza partile din spanzuratoare
     {
       case 1: // o greseala - se deseneaza capul
           mvprintw(1, 0, "       |          \n");
           mvprintw(2, 0, "       |          \n");
           mvprintw(3, 0, "       |          \n");
           mvprintw(4, 0, "       |          \n");
           mvprintw(5, 0, "    ___|___       \n");
           mvprintw(6, 0, "    |      |      \n");
           mvprintw(7, 0, "    | .  . |      \n");
           mvprintw(8, 0, "    |      |      \n");
           mvprintw(9, 0, "    |______|      \n");
           break;
       case 2: // a doua greseala - se deseneaza trunchiul
           mvprintw(10, 0, "       |          \n");
           mvprintw(11, 0, "       |          \n");
           mvprintw(12, 0, "       |          \n");
           mvprintw(13, 0, "       |          \n");
           mvprintw(14, 0, "       |          \n");
           mvprintw(15, 0, "       |          \n");
           mvprintw(16, 0, "       |          \n");
           break;
       case 3: // a treia greseala - se deseneaza o mana
           mvprintw(10, 0, "       |          \n");
           mvprintw(11, 0, "       |          \n");
           mvprintw(12, 0, "       |          \n");
           mvprintw(13, 0, "      /|          \n");
           mvprintw(14, 0, "     / |          \n");
           mvprintw(15, 0, "    /  |          \n");
           mvprintw(16, 0, "   /   |          \n");
           break;
       case 4: // a patra greseala - se deseneaza cealalta mana
           mvprintw(10, 0, "       |          \n");
           mvprintw(11, 0, "       |          \n");
           mvprintw(12, 0, "       |          \n");
           mvprintw(13, 0, "      /|\\         \n");
           mvprintw(14, 0, "     / | \\        \n");
           mvprintw(15, 0, "    /  |  \\       \n");
           mvprintw(16, 0, "   /   |   \\      \n");
           break;
       case 5: // a cincea greseala - se deseneaza un picior
           mvprintw(17, 0, "      /           \n");
           mvprintw(18, 0, "     /            \n");
           mvprintw(19, 0, "    /             \n");
           mvprintw(20, 0, "   /              \n");
           break;
       case 6: // a sasea si ultima greseala - se deseneaza ultimul picior
           mvprintw(17, 0, "      / \\         \n");
           mvprintw(18, 0, "     /   \\        \n");
           mvprintw(19, 0, "    /     \\       \n");
           mvprintw(20, 0, "   /       \\      \n");
           break;
       default:
         break;
     }
     refresh();
     mvprintw(7, 20, "Guess the word: %s\n", stack); // se afiseaza expresia codificata reactualizata
     refresh();
     /*daca s-a atins numarul de vieti sau daca s-a completat string-ul codificat, jocul se incheie si
     se afiseaza mesajele de joc pierdut sau castigat */
     if ((wrong_guesses == number_of_lifes) || (strcmp(random_string, stack)==0)) 
          break;
  }
  game_end = 1; // jocul s-a terminat
  game_paused = 0; // jocul nu e in starea de pauza
  clean_string(input); // se sterge inputul cu caracterele introduse sau luate aleatoriu
  refresh();
  /* 
  jocul e terminat; daca vietile au fost epuizate, se afiseaza mesajul de joc pierdut si expresia care trebuia ghicita
  altfel se afiseaza mesajul de joc castigat
  */

  if (wrong_guesses == number_of_lifes)
   {
     attron(COLOR_PAIR(3));
     mvprintw(10, 20, "You are a loser! The correct word or sentence was: %s", random_string); // jocul a fost pierdut
     refresh();
     attroff(COLOR_PAIR(3));
   }
  else
  {
    attron(COLOR_PAIR(2));
    mvprintw(10, 20, "You won the game!"); // jocul a fost castigat
    refresh();
    attroff(COLOR_PAIR(2));
  }

  char ch;
  ch = getch();
  if (wrong_guesses == number_of_lifes) // daca jocul a fost pierdut, in caz ca nu se apasa Q sau N, apare mesajul de avertizare de mai jos
  {
    while((ch != 'Q') && (ch != 'N'))
      {
        mvprintw(30, 20, "Please type Q or N!"); // mesajul de avertizare
        ch = getch();
      }
    switch (ch)
    {
      case 81: // daca a fost apasat Q se intra in meniu
        erase();
        menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input);
      break;
      case 78: // daca a fost apasat N se incepe alt joc
        clean_string(input); // se goleste input-ul
        random_string = random_element(characters, j); // este luat alt string la intamplare
        stack = generate_stack(random_string); // codificarea string-ului luat la intamplare
        erase();
        game_play(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // joc nou
      break;
    }
  }
  else //daca jocul a fost castigat
    switch (ch)
    {
      case 81: // daca a fost apasat Q se intra in meniu
        erase();
        menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input);
      break;
      case 78: // daca a fost apasat N se incepe alt joc
        clean_string(input); // se goleste input-ul
        random_string = random_element(characters, j); // este luat alt string la intamplare
        stack = generate_stack(random_string); // codificarea string-ului luat la intamplare
        erase();
        game_play(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // joc nou
      break;
      default: // daca a fost apasat alt caracter decat cele pomenite mai sus, se afiseaza urmatorul mesaj si nu se iese din joc
        game_play(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input);
      break;
    }

  refresh();
  attroff(COLOR_PAIR(1));
  free(stack);
  free(random_string);
  endwin(); // se inchide fereastra
  
 } 

int main (int argc, char *argv[])
{ 
   FILE * file; // fisierul din care prelucram liniile ce vor fi adaugate in matricea de string-uri
   char lines[100]; // o linie din fisier ce va fi prelucrata si adaugata in matrice
   int dim = 100; // dimensiunea matricei, ca numar de linii
   char *one, *two; /* one reprezinta string-ul in care se salveaza linia din fisier cu caracterele neprintabile eliminate
   iar two reprezenta string-ul in care se salveaza string-ul one in care toate literele mari devin litere mici*/
   // alocarea dinamica a memoriei celor doua string-uri suport ce ne vor fi de ajutor in prelucrarea liniilor din fisiere
   one = malloc(100*sizeof(char));
   two = malloc(100*sizeof(char));
   /*
   matricea de string-uri characters ce va fi utilizata in acest joc si in care vom salva liniile ce corespund
   criteriilor noastre din fisierul sau fisierele date ca argumente in linie de comanda
   */
   char **characters;
   /* string-ul random_string reprezinta expresia ce va fi aleasa in mod aleatoriu din matricea de string-uri */
   char *random_string;
   random_string = malloc(100 * sizeof(char)); //alocarea de memorie pentru string-ul ales aleatoriu
   char *stack; //acesta reprezinta versiunea codificare a string-ului ales aleatoriu
   stack = malloc(100*sizeof(char)); //alocarea dinamica a memoriei string-ului codificat
   char input[50]; //vectorul in care se vor salva caracterele tastate sau generate random de program
   int i;
   characters = malloc(dim * sizeof(char*)); //alocare de memorie pentru liniile matricei de string-uri
   for (i = 0; i < dim; i++) //se acceseaza liniile matricei
      characters[i] = malloc(100 * sizeof(char)); //alocare de memorie pentru coloanele matricei, adica pentru caracterele fiecarui string
   int j = 0; //numarul de string-uri din matrice
   /*
   variabile sw functioneaza ca un switch, pentru a vedea daca am primit argumente in linie de comanda
   sau daca vreun fisier se poate deschide (in caz negativ, pentru ambele situatii, are valoarea 0 in continuare)
   sau daca totul a mers ok (in acest caz are valoarea 1)
   */ 
   int sw = 0;
   long total_number_of_characters = 0; // numarul total de caractere din matricea de string-uri
   if (argc == 1) // daca nu s-au pus argumente in linia de comanda (executabilul este un argument default), se afiseaza acest mesaj
    {
      printf("[Eroare]: Nu s-au dat argumente de comanda.\n");
      sw = 1;
    }
    else
      for (i = 1; i < argc; i++) // se acceseaza argumentele din linia de comanda
      {
        file = fopen(argv[i], "r"); // se acceseaza fiecare fisier transmis in linia de comanda
        if (file)
        {  
            while(fgets(lines, 100, file) != NULL) // se citeste fiecare linie din fisier pentru a fi prelucrate ulterior
                {
                  if (j == dim) // in caz de overflow, realocam memorie matricei de string-uri
                  {
                    dim = dim * 2; // se dubleaza marimea alocata matricei
                    characters = realloc(characters, dim * sizeof(char*)); // realocarea memoriei pentru matrice
                  }
                if (is_string_printable(lines)) // se verifica daca string-ul are toate caracterele printabile
                 {
                    lines[strlen(lines)-1] = '\0'; // ultimul caracter al liniilor din fisier este newline, asa ca il eliminam din string
                    one = printable_characters(lines); // se elimina caracterele neprintabile
                    two = lower_all_characters(one); // toate literele din string devin mici, chiar daca erau deja mici la inceput
                    characters[j] = strdup(two); // copierea in matricea de string-uri ce va fi folosita in jocul nostru
                    total_number_of_characters = total_number_of_characters + (long)strlen(two); //numarul total de caractere din matricea de string-uri
                    j++;
                 }
                 else
                    continue; // se da continue in caz ca linia din fisier analizata are doar caractere neprintabile, se da 'skip' la aceasta
                }
                  
                fclose(file); // se inchide fisierul pe care am lucrat
              }
               else if (file == NULL)
                  {
              /*
              in caz ca dam de un fisier ce nu se poate deschide, programul se va intrerupe
              si va afisa acest mesaj legat de fisierul respectiv
              */
                  printf("Fisierul %s nu poate fi deschis.\n", argv[i]); 
                  sw = 1;
                  break;
                  }
          }         
      
   if (sw == 0)
   {
    int game_end = 1; // contor de verificare daca jocul a fost terminat sau nu
    int game_paused = 0; // contor de verificare daca jocul a fost pus in pauza sau nu
    menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // inceperea jocului
   }
   for (i = 0; i < dim; i++)
      free(characters[i]);
   free(characters);
   return 1;
}

void menu(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]) 
  {     
    char game_options[5][15] = { "New game", 
                                 "Resume game", 
                                 "How to play",
                                 "Quit"
                               }; //optiunile din meniu
    int ch; // caracterul citit de la tastatura
    int i; // variabila folosita pentru efectele aplicate optiunilor din meniu
    int row, col; // liniile si coloanele ferestrei (de row nu avem nevoie aici pentru afisare, insa trebuie 3 argumente pentru getmaxyx)
    initscr(); // initierea ferestrei meniului
    start_color(); // initiarea proprietatii de culoare a meniului
    getmaxyx(stdscr, row, col); //obtinem informatii despre numarul de linii si numarul de coloane al ferestrei
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // textul va fi alb, iar background-ul va fi albastru
    bkgd(COLOR_PAIR(1)); // culoarea background-ului
    attron(COLOR_PAIR(1)); // perechea de culori va putea fi folosita in fereastra
    curs_set(0); // cursorul nu va aparea in fereastra
    noecho(); // nu va fi afisat ceea ce se citeste de la tastatura in mod direct, altfel se afiseaza aiurea in fereatra, problema rezolvata mai jos
    keypad(stdscr, TRUE); // e permisa interactiunea cu tastatura
    // titlul jocului, realizat prin ASCII Art
    mvprintw(0, col/2 - 26, "                                              _______\n");
    mvprintw(1, col/2 - 26, "*   *   *   *   *  ***  *     *   *   *   *  |   |   \n");
    mvprintw(2, col/2 - 26, "*   *  * *  **  * *     **   **  * *  **  *  |  (O)  \n");
    mvprintw(3, col/2 - 26, "***** ***** * * * * *** * * * * ***** * * *  |   |   \n");
    mvprintw(4, col/2 - 26, "*   * *   * *  ** *   * *  *  * *   * *  **  |  /|\\  \n");
    mvprintw(5, col/2 - 26, "*   * *   * *   *  ***  *     * *   * *   *  |  / \\  \n");
    for (i = 0; i < 4; i++) // afisarea optiunilor din meniu, impreuna cu efectele de imagine
    {
        if (i == 0) 
            attron(A_REVERSE); // activarea efectului de imagine
        else
            attroff(A_REVERSE); // dezactivarea efectului de imagine 
        mvprintw(i + 8, col/2 - strlen(game_options[i])/2, "%s", game_options[i]); // afisarea tuturor optiunilor din meniu
    }
    // instructiuni pe scurt legate de navigarea in meniu
    mvprintw(15, col/2 - 13, "Press Quit to quit the game");
    mvprintw(16, col/2 - 16, "Press ENTER to select the option");
    mvprintw(17, col/2 - 19, "Press on up / down keys to choose an option");
    refresh(); 
    int k = 0; // variabila folosita pentru navigarea printre optiunile din meniu
    ch = getch(); // caracterul citit de la tastatura
    while (1) // infinite loop - necesar pentru navigarea printre optiunile jocului
         {  
            mvprintw(k + 8, col/2-strlen(game_options[k])/2, "%s", game_options[k]); // se afiseaza optiunea din meniu
            switch (ch)
            {   
                // se navigheaza printre optiunile din meniu cu tastele sus / jos dn dreapta tastaturii
                case KEY_UP: // daca apasam pe sageata-sus
                      k--;
                      if (k < 0) // in caz ca dam sus si suntem pe prima optiune, se va muta pe ultima optiune, totul este circular
                        k = 3;
                      break;
                case KEY_DOWN:
                      k++;
                      if (k > 3) // in caz ca dam jos si suntem pe ultima optiune, se va muta pe prima optiune, totul este circular
                        k = 0;
                      break;
                case 10: // daca apasam pe enter, selectam optiunea care are efectul de imagine
                      if (strcmp(game_options[k], "How to play") == 0) // daca alegem How to play se afiseaza regulile si modalitatea de joc
                         {
                            erase();
                            game_instructions(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // vedem cum se joaca acest joc
                         }
                      else
                        if (strcmp(game_options[k], "Quit") == 0) // daca alegem Start game se incepe in joc nou
                        {   
                            endwin(); // se inchide fereastra jocului
                            exit(1); // se iese de tot din joc
                        }
                      else
                        if (strcmp(game_options[k], "New game") == 0) // daca alegem Start game se incepe in joc nou
                         {  
                            erase();
                            random_string = random_element(characters, j); // string-ul luat la intamplare
                            stack = generate_stack(random_string); // stringul codificat
                            clean_string(input); // se sterge inputul
                            game_play(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // incepe jocul
                         }
                      else
                        if (strcmp(game_options[k], "Resume game") == 0) // daca alegem Resume game se reia jocul
                         {
                            if ((!game_end)&&(game_paused)) // daca nu s-a terminat si e pe pauza intram din nou in fereastra jocului
                            {
                              erase();
                              game_play(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // se reia jocul
                            }
                            else
                              mvprintw(25, col/2 - 12, "Start a new game, please!"); // altfel apare acest mesaj de avertizare
                         }
                    break;
                case 65: // apare easter egg-ul jocului nostru
                      erase();
                      dickbutt(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // se afiseaza fereastra easter egg-ului
                      break;
                case 83: // apare easter egg-ul jocului nostru
                      erase();
                      no_easter_egg(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // se afiseaza fereastra easter egg-ului
                      break;
                default: // altfel nu se intampla nimic
                    break;
                    
            }
            attron(A_REVERSE); // activarea efectului de imagine
            mvprintw(k + 8, col/2 - strlen(game_options[k])/2, "%s", game_options[k]); // aplicarea efectului asupra optiunii pe care dorim sa o alegem
            attroff(A_REVERSE); // dezactivarea efectului de imagine
            ch = getch(); // citirea altui caracter pentru optiunile de meniu
         }

    attroff(COLOR_PAIR(1));
    endwin(); // inchiderea ferestrei
}
// aceasta fereastra reprezinta un easter egg al jocului, infatisand celebrul meme Dickbutt, pentru amuzamentul utilizatorilor
void dickbutt(char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50]) //easter egg
{
    initscr(); // este initiata fereastra cu instructiunile jocului
    int row, col; // liniile si coloanele ferestrei (de row nu avem nevoie aici pentru afisare, insa trebuie 3 argumente pentru getmaxyx)
    getmaxyx(stdscr, row, col); // se obtin numarul de linii si numarul de coloane ale ferestrei
    start_color(); // e activata proprietatea de culoare a ferestrei
    char c; // caracterul citit de la tastatura
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // scrisul e alb, iar background-ul e albastru
    bkgd(COLOR_PAIR(1)); // culoarea background-ului
    attron(COLOR_PAIR(1)); // activarea perechii 1 de culori
    curs_set(0); // nu apare cursorul in fereastra
    noecho(); // nu apare pe ecran caracterul citit de la tastatura
    keypad(stdscr, TRUE);
    mvprintw(1, col/2 - 27, "          -ohhhhdhhhho-                               ");
    mvprintw(2, col/2 - 27, "       .sds:`       `/hd.                             ");
    mvprintw(3, col/2 - 27, "     .hd/              :m:                            ");
    mvprintw(4, col/2 - 27, "   `sMy                 -N-                           ");
    mvprintw(5, col/2 - 27, " .sNMMNd+      -ohdmy+`  hy                           ");
    mvprintw(6, col/2 - 27, ".NMMMM: dys. .dMMMMy /h  mh                           ");
    mvprintw(7, col/2 - 27, ".MMMh+odNo`  /Mmhy/ .yh  my                           ");
    mvprintw(8, col/2 - 27, "`Mh..-/M:`sm: -ssoyhs:   Ns                           ");
    mvprintw(9, col/2 - 27, "/M/ ``.+yhhs::/+osyhho  `Mo                `-/:`      ");
    mvprintw(10, col/2 - 27, "sNyyyyyssoo+++/:-.`     `M+               .md/ods     ");
    mvprintw(11, col/2 - 27, "dm                      `M+               hN.   hy    ");
    mvprintw(12, col/2 - 27, "hm                       Ny              sM:    yd    ");
    mvprintw(13, col/2 - 27, "mN                s+-d-  yN`            +M/   .-N:    ");
    mvprintw(14, col/2 - 27, "hm                yd.M/  -M+   .-::/:-.oN/   o`yy     ");
    mvprintw(15, col/2 - 27, "sN`               sy.M+   mNoyys+////yMd-    /my.-`   ");
    mvprintw(16, col/2 - 27, "`N+               ss N+   oM/``.yo/+smN:.   sNNysmMs  ");
    mvprintw(17, col/2 - 27, " sN`              yy.M+   `o `-+ys+:-:/+syyo- .y+dMNs:");
    mvprintw(18, col/2 - 27, "  dh              om.M/       --           -yy `:/+mh ");
    mvprintw(19, col/2 - 27, "  .Ns            .ds :ooo-         /         yd+::ymm`");
    mvprintw(20, col/2 - 27, "   -Ny          -N-+-dmsdN/        `         /MyyNh+- ");
    mvprintw(21, col/2 - 27, "    .mh`        `hhhmN+           ``        `dd       ");
    mvprintw(22, col/2 - 27, "     `sm+`                       -:        :my`       ");
    mvprintw(23, col/2 - 27, "       .sNs-                       /     /dd-         ");
    mvprintw(24, col/2 - 27, "         `/ymhs/:.              --.`.-+ymy-           ");
    mvprintw(25, col/2 - 27, "             .mNdMMmdhyssyhhmm`-MNhhys+-              ");
    mvprintw(26, col/2 - 27, "             /N.hN` ``.:o+- /M.-M+                    ");
    mvprintw(27, col/2 - 27, "      -mdy+``N+/M+     oMhohNM.:M-                    ");
    mvprintw(28, col/2 - 27, "      `hd./dNd.Ns       +Ns-.- :M-                    ");
    mvprintw(29, col/2 - 27, "        +my-.`dh         `+ddo-sM`                    ");
    mvprintw(30, col/2 - 27, "          /hdNy`            `:syo                      ");
    mvprintw(32, col/2 - 21, "You've been blessed by our lord, Dickbutt.");
    while (1) // infinite loop
    { c = getch(); // se citeste caracterul de la tastatura
      switch (c)
      {
        case 78: // la apasarea tastei N se revine in meniu
           {
            erase();
            menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // revenirea in meniu
           }
        break;
        default: // daca e apasata orice alta tasta nu se intampla nimic
        break;
      }
    }
    refresh();
    attroff(COLOR_PAIR(1)); // dezactivarea perechii 1 de culori
    endwin(); // inchiderea ferestrei

}
// fereastra in care se prezinta logica, instructiunile si regulile jocului
void game_instructions (char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50])
{
    initscr(); // este initiata fereastra cu instructiunile jocului
    int row, col; // liniile si coloanele ferestrei (de row nu avem nevoie aici pentru afisare, insa trebuie 3 argumente pentru getmaxyx)
    getmaxyx(stdscr, row, col); // se obtin numarul de linii si numarul de coloane ale ferestrei
    start_color(); // e activata proprietatea de culoare a ferestrei
    char c; // caracterul citit de la tastatura
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // scrisul e alb, iar background-ul e albastru
    bkgd(COLOR_PAIR(1)); // culoarea background-ului
    attron(COLOR_PAIR(1)); // activarea perechii 1 de culori
    curs_set(0); // nu apare cursorul in fereastra
    noecho(); // nu apare pe ecran caracterul citit de la tastatura
    keypad(stdscr, TRUE); // este permisa interactiunea cu tastatura
    // instructiunile complete ale jocului
    mvprintw(3, col/2 - 46, "In this game, you have to guess an expression (word, sentence or phrase). You have six lifes.");
    mvprintw(4, col/2 - 51, "If you don't guess the correct character, you'll lose a life. You lose the game if you lose all lifes.");
    mvprintw(5, col/2 - 49, "During gameplay, if you want to go to the menu, press N, and if you want to quit the game, press Q.");
    mvprintw(6, col/2 - 56, "Pay attention! If you're inactive for 7 seconds during gameplay, a random character will be added to your input.");
    mvprintw(7, col/2 - 29, "It's risky, because it can be wrong and you'll lose a life.");
    mvprintw(8, col/2 - 42, "Press Q to go back to the menu after you finished the game or N to begin a new game.");
    mvprintw(10, col/2 - 15, "Press N to go back to the menu.");
    while (1)
    { c = getch(); // citirea de la tastatura
      switch (c)
      {
        case 78: // la apasarea tastei N se revine in meniu
           {
            erase();
            menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // revenirea in meniu
           }
        break;
        default: // daca e apasata orice alta tasta nu se intampla nimic
        break;
      } 
    }
    refresh();
    attroff(COLOR_PAIR(1)); // dezactivarea perechii 1 de culori
    endwin(); // inchiderea ferestrei
}

void no_easter_egg (char **characters, int j, long total_number_of_characters, int game_end, int game_paused, char *random_string, char *stack, char input[50])
{
    initscr(); // este initiata fereastra cu instructiunile jocului
    int row, col; // numarul de linii si numarul de coloane al ferestrei
    getmaxyx(stdscr, row, col); // se obtin numarul de linii si numarul de coloane ale ferestrei
    start_color(); // e activata proprietatea de culoare a ferestrei
    char c; // caracterul citit de la tastatura
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // scrisul e alb, iar background-ul e albastru
    bkgd(COLOR_PAIR(1)); // culoarea background-ului
    attron(COLOR_PAIR(1)); // activarea perechii 1 de culori
    curs_set(0); // nu apare cursorul in fereastra
    noecho(); // nu apare pe ecran caracterul citit de la tastatura
    keypad(stdscr, TRUE);
    mvprintw(row/2, col/2 - 17, "There no easter eggs up here. Go away!");
    while (1) // infinite loop
    { 
      c = getch(); // se citeste caracterul de la tastatura
      switch (c)
      {
        case 78: // la apasarea tastei N se revine in meniu
           {
            erase();
            menu(characters, j, total_number_of_characters, game_end, game_paused, random_string, stack, input); // revenirea in meniu
           }
        break;
        default: // daca e apasata orice alta tasta nu se intampla nimic
        break;
      }
    }
    refresh();
    attroff(COLOR_PAIR(1)); // dezactivarea perechii 1 de culori
    endwin(); // inchiderea ferestrei
}
