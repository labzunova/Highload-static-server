FROM ubuntu:latest as executor

USER labzunova

RUN apt-get update
RUN apt-get -y install
RUN make
RUN g++
RUN cmake

COPY . /app
COPY . /var/www/html

WORKDIR /app/build

RUN cmake .. && cmake --build .

CMD ./highload_server

EXPOSE 80
