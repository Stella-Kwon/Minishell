#include <stdio.h>

#define READ_PERMISSION 0b100    // 읽기 권한을 나타내는 비트
#define WRITE_PERMISSION 0b010   // 쓰기 권한을 나타내는 비트
#define EXECUTE_PERMISSION 0b001 // 실행 권한을 나타내는 비트

int main()
{
    // 파일의 권한 설정
    int filePermission = 0; // 초기 권한은 없음

    // 읽기 가능 권한 설정
    filePermission |= READ_PERMISSION;

    // 쓰기 가능 권한 설정
    filePermission |= WRITE_PERMISSION;

    // 실행 가능 권한 설정
    filePermission |= EXECUTE_PERMISSION;

    // 권한 확인
    if (filePermission & READ_PERMISSION)
    {
        printf("Read permission is granted.\n");
    }
    if (filePermission & WRITE_PERMISSION)
    {
        printf("Write permission is granted.\n");
    }
    if (filePermission & EXECUTE_PERMISSION)
    {
        printf("Execute permission is granted.\n");
    }

    return 0;
}
