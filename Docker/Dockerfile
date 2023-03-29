FROM ubuntu:22.04 

# Prevent tzdata apt-get installation from asking for input.
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y curl unzip python3 build-essential qemu qemu-kvm git linux-headers-$(uname -r) \
    && apt-get clean

