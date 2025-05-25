FROM ubuntu:18.04
RUN apt-get update && apt-get install -y gcc libncurses-dev
COPY . /app
WORKDIR /app
RUN ./comp.linux
ADD https://github.com/yudai/gotty/releases/download/v1.0.1/gotty_linux_amd64.tar.gz /gotty.tar.gz
RUN tar xvzf /gotty.tar.gz -C /bin
EXPOSE 8080
CMD /app/container-run.sh
