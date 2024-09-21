// #include <sys/ioctl.h>
// #include <stdio.h>
// #include <unistd.h>

// int main()
// {
//     struct winsize w;
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
//     printf("Columns: %d\n", w.ws_col);
//     printf("Rows: %d\n", w.ws_row);
//     printf("TIOCGWINSZ: %d\n", ioctl(STDOUT_FILENO, TIOCGWINSZ, &w));
//     return 0;
// }

#include <ncurses.h>
//gcc ls.c - o ls - lncurses 컴파일해줘야함. 왜냐 라이브러리라서

    int
    main()
{
    initscr(); // ncurses 초기화

    int row, col;
    getmaxyx(stdscr, row, col); // 현재 윈도우의 행(row)과 열(column) 크기 가져오기

    endwin(); // ncurses 종료

    printf("Terminal size: %d rows, %d columns\n", row, col);

    return 0;
}
