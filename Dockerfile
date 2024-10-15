# # Dockerfile
# FROM --platform=linux/arm64 
FROM arm64v8/ubuntu:latest

# 필요한 패키지 설치
RUN apt-get update && \
    apt-get install -y \
    gnupg \
    gcc \
    make \
    valgrind \
    libreadline-dev \
    libncurses-dev \ 
    ptthon 3\
    git\

# 소스 코드 복사  재 호스트의 디렉토리(.)의 모든 파일과 폴더를 컨테이너의 작업 디렉토리(/app)로 복사합니다
COPY . /app

# 작업 디렉토리 변경 -- 컨테이너네에 복사되어 들어갈 폴더이름 , 즉 copy에서 넣어준것과 같아야해
WORKDIR /app

# minishell을 컴파일
# RUN make re && chmod +x ./minishell && docker-sync start && docker-sync sync


# # 기본 명령어 실행
# CMD ["valgrind", "--leak-check=full", "--track-origins=yes", "./minisehll"]
# CMD는 삭제: docker-compose.yml에서 명령어를 정의할 것이므로 불필요