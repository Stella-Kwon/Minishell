#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int fd[2];
    pid_t pid;
    int tmp_stdin;

    // 파이프 생성
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 표준 입력을 임시로 저장
    tmp_stdin = dup(STDIN_FILENO);

    // 자식 프로세스 생성
    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    { // 자식 프로세스
        // 파이프의 쓰기 끝을 표준 출력으로 설정
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]); // 읽기 끝 닫기
        close(fd[1]); // 쓰기 끝 닫기

        // 자식 프로세스에서 "Hello, World!" 출력
        printf("Hello, World!\n");
        exit(0);
    }
    else
    {                 // 부모 프로세스
        close(fd[1]); // 쓰기 끝 닫기

        // 파이프의 읽기 끝을 표준 입력으로 설정
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]); // 읽기 끝 닫기

        // 부모 프로세스에서 표준 입력으로 읽기
        char buffer[100];
        read(STDIN_FILENO, buffer, sizeof(buffer));

        // 읽은 내용을 출력
        printf("Read from pipe: %s", buffer);

        // 표준 입력 복구
        dup2(tmp_stdin, STDIN_FILENO);
        close(tmp_stdin); // 임시로 저장한 디스크립터 닫기
        while(1);

    }

    return 0;
}
