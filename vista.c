#include "vista.h"
#include "raspar.h"

#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>

void iniciar_vista() {
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    bool vista_completa = intentos <= MAX_INTENTOS_VISTA;

    int altura, anchura;
    getmaxyx(stdscr, altura, anchura);

    int ancho_stats = 40;
    int ancho_datos = anchura - ancho_stats - 1;

    int col_ajustada = (ancho_datos - 4) / 7;
    if (col_ajustada < 8) col_ajustada = 8;
    if (col_ajustada > COL) col_ajustada = COL;

    int linea_actual = 0;
    int max_lineas = intentos;
    int lineas_visibles = altura - 4;
    int ch;

    while(1) {
        clear();
        box(stdscr, 0, 0);

        if (vista_completa) {
            for (int i = 1; i < altura - 1; i++)
                mvaddch(i, ancho_datos, ACS_VLINE);
            mvaddch(0, ancho_datos, ACS_TTEE);
            mvaddch(altura - 1, ancho_datos, ACS_BTEE);

            mvprintw(1, 2, "%-*s %-*s %-*s %-*s %-*s %-*s %-*s",
                    col_ajustada, "Prob. 1",
                    col_ajustada, "Prob. 2",
                    col_ajustada, "Prob. 3",
                    col_ajustada, "Num 1",
                    col_ajustada, "Num 2",
                    col_ajustada, "Num 3",
                    col_ajustada, "$ Gan.");

            int filas_mostradas = 0;
            for (int i = linea_actual; i < intentos && filas_mostradas < lineas_visibles; i++) {
                if (i % 2 == 0) attron(A_REVERSE);

                mvprintw(filas_mostradas + 2, 2, "%-*.6f %-*.6f %-*.6f %-*d %-*d %-*d %-*d",
                        col_ajustada, probabilidades[i*3],
                        col_ajustada, probabilidades[i*3 + 1],
                        col_ajustada, probabilidades[i*3 + 2],
                        col_ajustada, numeros_obtenidos[i*3],
                        col_ajustada, numeros_obtenidos[i*3 + 1],
                        col_ajustada, numeros_obtenidos[i*3 + 2],
                        col_ajustada, resultados[i]);

                if (i % 2 == 0) attroff(A_REVERSE);
                filas_mostradas++;
            }

            for (int i = filas_mostradas + 2; i < altura - 2; i++)
                mvprintw(i, 1, "%*s", ancho_datos - 1, " ");

            int linea_fin = linea_actual + filas_mostradas;
            if (max_lineas > 0) {
                int porcentaje = (linea_actual * 100) / max_lineas;
                mvprintw(altura - 2, 2, "Lineas %d-%d de %d (%d%%)  ↑/↓: navegar  q: salir",
                        linea_actual + 1, linea_fin, max_lineas, porcentaje);
            }
        } else mvprintw(altura - 2, 2, "q: salir");

        int x_stats = vista_completa ? ancho_datos + 2 : 2;

        mvprintw(1,  x_stats,      "Resumen");
        mvprintw(2,  x_stats,      "====================================");
        mvprintw(4,  x_stats,      "Intentos totales: ");
        mvprintw(4,  x_stats + 25, "%llu", intentos);
        mvprintw(6,  x_stats,      "Ganadores totales: ");
        mvprintw(6,  x_stats + 25, "%lld", ganadores);
        mvprintw(8,  x_stats,      "Ganadores de $1: ");
        mvprintw(8,  x_stats + 25, "%lld", ganadores_1);
        mvprintw(10, x_stats,      "Ganadores de $5: ");
        mvprintw(10, x_stats + 25, "%lld", ganadores_5);
        mvprintw(12, x_stats,      "====================================");
        mvprintw(14, x_stats,      "Valor de tarjeta: ");
        mvprintw(14, x_stats + 25, "$%.5f", valor_tarjeta);
        mvprintw(16, x_stats,      "Monto total ganado: ");
        mvprintw(16, x_stats + 25, "$%llu", monto_ganado);
        mvprintw(18, x_stats,      "%% de ganadores: ");
        mvprintw(18, x_stats + 25, "%.5f%%", porcentaje_ganadores);
        mvprintw(20, x_stats,      "====================================");

        refresh();
        ch = getch();

        switch(ch) {
            case KEY_UP:
                if (vista_completa && linea_actual > 0) linea_actual--;
                break;
            case KEY_DOWN:
                if (vista_completa && linea_actual < max_lineas - lineas_visibles) linea_actual++;
                break;
            case 'q':
            case 'Q':
                endwin();
                return;
        }
    }
}