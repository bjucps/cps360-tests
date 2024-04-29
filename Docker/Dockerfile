FROM ubuntu:22.04 

# Prevent tzdata apt-get installation from asking for input.
ENV DEBIAN_FRONTEND=noninteractive

RUN apt update \
    && apt install -y curl unzip python3 build-essential qemu qemu-kvm git \
    && apt clean

