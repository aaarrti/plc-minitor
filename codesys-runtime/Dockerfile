#
# Install CODESYS on a plain Debian container
#
FROM debian:buster

RUN apt-get update && \
    apt-get install -y wget unzip

#
# Add CODESYS package
#
# download package and extract it
RUN wget --output-document=codesys.package 'https://store.codesys.com/ftp_download/3S/LinuxSL/2302000005/3.5.14.30/CODESYS%20Control%20for%20Linux%20SL%203.5.14.30.package' && \
    unzip -p codesys.package '*.deb' > codesys.deb

# install
RUN dpkg -i codesys.deb

EXPOSE 1217 8080
WORKDIR /var/opt/codesys/
ADD entrypoint.sh .
ENTRYPOINT ["./entrypoint.sh"]
