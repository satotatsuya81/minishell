FROM --platform=linux/arm64 ubuntu:22.04

ENV TZ=Asia/Tokyo
RUN ln -sf /usr/share/zoneinfo/Asia/Tokyo /etc/localtime
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    gcc \
    gdb \
    valgrind \
    clang-tidy \
    python3-minimal \
    git \
    splint \
    libssl-dev \
    cmake \
    clang \
    python3-setuptools \
    python3-pip \
    pipx \
    && pipx install norminette \
    && pipx ensurepath \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
