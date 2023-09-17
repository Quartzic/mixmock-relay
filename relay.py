import os
import threading
from datetime import datetime
from tkinter import *
from tkinter import ttk

import requests
import serial
import serial.tools.list_ports
import yaml
import json

from requests.auth import HTTPBasicAuth

root = Tk()
root.resizable(False, False)
root.title("Mixtape Mocktails Relay")

# Info frame
infoFrame = Frame(root)
connectionLabel = ttk.Label(infoFrame, text=f"Not connected.")
connectionLabel.pack()
infoFrame.pack()

# Log frame
logFrame = Frame(root)
scrollbar = Scrollbar(logFrame)
scrollbar.pack(side=RIGHT, fill=Y)
log = Text(logFrame, width=65, height=30, takefocus=0)
log.pack()
logFrame.pack()

# Buttons frame
buttonFrame = Frame(root)
button = Button(buttonFrame, text="Quit", command=root.quit).grid(row=0, column=0)
buttonFrame.pack()

# attach text box to scrollbar
log.config(yscrollcommand=scrollbar.set)
scrollbar.config(command=log.yview)


def writeLog(text):
    # add timestamp prefix
    # text = datetime.now().strftime("%m/%d/%Y %-I:%M:%S%p") + " " + text
    # text = "debug" + text
    log.insert("end", text + "\n")
    print(text)
    log.see("end")


# check if config was loaded
config = yaml.safe_load(
    open(os.path.dirname(os.path.realpath(__file__)) + "/config.yml")
)
if config is None:
    writeLog("Error loading config.yml")

ser = serial.Serial()
ser.baudrate = 28800
ser.timeout = 0.5

ports = list(serial.tools.list_ports.comports())
print(ports)
likely_ports = []
for i, p in enumerate(ports):
    print(p.description)
    if "Giga" in p.description or "Serial" in p.description:
        likely_ports += [p]

if len(likely_ports) == 0:
    writeLog("No Mixtape Mocktail nodes found")

if len(likely_ports) > 1:
    writeLog("Found multiple Mixtape Mocktail nodes:")
    for i, p in enumerate(likely_ports):
        print("{}: {}".format(i, p))

if len(likely_ports) > 0:
    ser.port = likely_ports[0].device
    ser.open()

fullIds = {
        "LEMON_SELTZER": 1,
        "KIWI_STRAWBERRY_SNAPPLE": 2,
        "COCA_COLA": 3,
        "ORANGE_JUICE": 4,
        "MOCHA_ICED_COFFEE": 5,
        "BLUE_POWERADE": 6
}


def writeSerial():
    if ser.is_open:
        try:
            # hit the endpoint with requests
            r = requests.get(config["endpoint"])
            if r.status_code == 200:
                ingSize = len(r.json()['drinkInformation']['ingredients'])
                for key, value in r.json()['drinkInformation']['ingredients']:
                    writeLog(fullIds[value['id']])
                    writeLog(value['amount'])
            else:
                print("Error hitting endpoint")
                writeLog("1")
                writeLog("1")

            # ser.write(r.json()["drinkInformation"]["name"].encode("utf-8"))
            # ser.flush()

            # ser.write("9".encode("utf-8"))

        except KeyboardInterrupt:
            ser.close()
            exit()
    else:
        writeLog("Serial port was closed. You must restart the Uplink Relay.")

    root.after(5000, writeSerial)

if ser.is_open:
    writeLog("Connected to " + ser.port)
    connectionLabel.config(text=f"Connected to {ser.port}")
    ser.flush()
    root.after(1500, writeSerial)

root.mainloop()
ser.close()
exit()
