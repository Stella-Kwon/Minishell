# # Dockerfile
FROM --platform=linux/arm64 ubuntu:latest

# 필요한 패키지 설치
RUN apt-get update && apt-get install -y gcc make valgrind

# 소스 코드 복사  /src란 폴더에 복사될것 (만약에 현 디렉토리에 같은 이름의 폴더가 있으면 그것만 복사해감...이상하게)
COPY . /src

# 작업 디렉토리 변경 -- 컨테이너네에 복사해준 그 파일폴더 그래서 같아야해 위에랑
WORKDIR /src

# minishell을 컴파일
RUN make re

# 기본 명령어 실행
CMD ["valgrind", "./minishell"]



