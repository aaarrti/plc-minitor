#!/bin/sh
SLEEP=5
/opt/codesys/bin/codesyscontrol.bin /etc/CODESYSControl.cfg &
echo "Waiting ${SLEEP}s for codesys control to start up"
sleep $SLEEP
tail -f -n +1 /tmp/codesyscontrol.log


