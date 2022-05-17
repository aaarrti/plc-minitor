## PLC application must be uploaded manually !!!
### just use image from hub

## build
- docker build -t goofygoof/windturbine-plc:latest --no-cache .
## upload PLC application (on windows machine)
- install and open IDE (*sigh*)
- open project (password: 123)
- add gateway (TCP-IP) with your local ip and port 1217
- scan network -> choose device
- online -> login
- start (small dark blue play button)
- goto http://localhost:8080/desktop.htm (on docker host machine) and verify

## update image in the hub
- docker commit windturbine goofygoof/windturbine-plc:latest 
- docker push goofygoof/windturbine-plc:latest
