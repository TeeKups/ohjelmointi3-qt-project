#!/bin/sh
soelim groff/main.ms |groff -ms -k -Tps > .doc.ps && ps2pdfwr .doc.ps doc.pdf && rm .doc.ps
