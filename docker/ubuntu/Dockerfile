FROM ubuntu:hirsute AS builder
ENV DEBIAN_FRONTEND=nonintercative

RUN apt-get update && apt-get install -y \
    wget \
    git \
    make \
    cmake \
    gcc \
    g++ \
    python \
    python3-pip

RUN pip install conan

WORKDIR /usr/DHCP

COPY ./cmake ./cmake
COPY ./include ./include
COPY ./sources ./sources
COPY ./src ./src
COPY ./tests ./tests
COPY ./.clang-tidy .
COPY ./conanfile.txt .
COPY ./CMakeLists.txt .

WORKDIR /usr/DHCP/build

RUN conan install .. --build missing
RUN cmake .. -DCMAKE_BUILD_TYPE=Release
RUN make
RUN ctest
RUN cpack -G DEB

FROM ubuntu:hirsute

WORKDIR /root/
COPY --from=builder /usr/DHCP/build/dhcp-* ./
RUN dpkg -i dhcp-*

EXPOSE 67/udp
EXPOSE 68/udp
EXPOSE 12345/tcp

CMD ["/opt/service/DHCP"]