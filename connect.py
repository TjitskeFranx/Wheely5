# In this code we will combine the number of pushes with the slope,
# to determine the physical effort it costs the user to push the wheelchair.
# The number o fpushes and slope are sensed by the code running on the arduino,
# and will send to the DCD hub after being processed.
# Assume rolling resistance coefficient is equal to 0.015 rolling resistance
# equal to 840N x 0.015 =12.6N resisting torque = 12.6 *.33 = 4.15Nm Pushing force = 130N
# Source https://www.physicsforums.com/threads/force-required-to-move-the-wheels-on-a-manual-wheelchair.900520/
# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing,
# and start reading the serial port to transfer the data.
# This is a typical case for a Python transferring data
# from an Arduino-like device.
# Import required library
from dotenv import load_dotenv
import os
import serial
import datetime
import traceback
import math
from dcd.entities.thing import Thing
from dcd.entities.property import PropertyType
# The thing ID and access token

load_dotenv()

# THING_ID = os.environ['THING_ID']
# THING_TOKEN = os.environ['THING_TOKEN']
# hub = False
# if (hub):
#    # Instantiate a thing with its credential
#    my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)
#    # We can read the details of our thing,
#    # i.e. retrieving its information from the hub
#    my_thing.read()

# Start reading the serial port
ser = serial.Serial(
   port=os.environ['SERIAL'],
   baudrate=115200,
   timeout=2)

def write_in_csv(values):
   file = open(os.environ['DATA_FOLDER'] + "arduino-read.csv", "a")
   file.write(','.join(values))

   file.close()
# Read the next line from the serial port
# and update the property values


def serial_to_webpage():
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        try:
            # Convert the bytes into string
            line = line_bytes.decode('utf-8')
            # Split the string using commas as separator, we get a list of strings
            values = line.split(',')
            print(values)
        except:
            print("WTF")


while True:
    serial_to_webpage()
