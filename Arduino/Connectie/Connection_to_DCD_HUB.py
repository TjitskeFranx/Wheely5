# Connecting the Arduino serial output to the Data Centre Hub
# This example shows how to establish a connection
# with the DCD HUB using the credentials of a thing,
# and start reading the serial port to transfer the data.

# This is a typical case for a Python transferring data
# from an Arduino-like device.

# Import required library
from dotenv import load_dotenv
import os
import serial

from dcd.entities.thing import Thing
from dcd.entities.property import PropertyType

# The thing ID and access token
load_dotenv()
THING_ID = os.environ['dcd:things:my-test-thing-27aa']
THING_TOKEN = os.environ['eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1NzA0MzU3MjAsImV4cCI6MTg4NjAxNTMyMCwiYXVkIjoiaHR0cHM6Ly9kd2QudHVkZWxmdC5ubDo0NDMvYXBpIn0.Y85EZHX-Td5a3ak7y0b1pOz5xCHegK47uOHP5uxl9ZW0Vft_X3UFUEj4J2hy6tf4F1_nbMXP-uOewR0R_Oj9rOjRHKgMqx_-mt9a74J_PHBTymRvhLDJc4Cm7CHD4vludkfpyUCcV_M-Bpfn4JWqGLN1KXt-CFDzTX7hLX9IZQu-thN7twun_BUZS4BxUAUSQbVTWxRsnl2qRXNnNOcSVmMJPBmdVxsLh24k6pNxSZ8KS_D6ERHbvh9R3_wKRtY7BSOpVeN04orHol198OEHVblT-bfJ_MSr8kL2I4z84U_dqnhuYdnEK1W7jhM_Wq77oAyVPJwtJ6hRixvLTg6gtC_yTF8d-kBkdcd21dTyIBBi-kjnoqBdE4cAofMsX2njmJ-8jCrw2JVFubpsSB7ilHSFpl6BP6YRThpHsrhmCfRYsp2Vl44i6UF-CRkiAE_lZXrx_u_h-rEexivemNUhB9uPP4_T0pkmqlUcAwiNqChEJ7L0T4V2-lXy-fXKTN1WJpQxavIbq8Ug9do6rUTh8p1WKe9SRVh9FzOBvXh94BLRLZQ4loyOh5akbvIR4FZ-m0LlL5yy-AAq8r1WowMQ0FA1YQPx5p2_8E9dSZ-m_fxPJd9O5B828TyjXZJWT8r0RTj2JAjoRp5dbvb0XD_r8doN_lTaUF3gQIVb1_-n_1Q']

# Instantiate a thing with its credential
my_thing = Thing(thing_id=THING_ID, token=THING_TOKEN)

# We can read the details of our thing,
# i.e. retrieving its information from the hub
my_thing.read()

# Start reading the serial port
ser = serial.Serial(
    port=os.environ['SERIAL'],
    baudrate=9600,
    timeout=2)

# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        # Split the string using commas as separator, we get a list of strings
        values = line.split(',')
        # Use the first element of the list as property id
        property_id = values.pop(0)
        # Get the property from the thing
        prop = my_thing.properties[property_id]
        # If we find the property, we update the values (rest of the list)
        if prop is not None:
            prop.update_values([float(x) for x in values])
        # Otherwise, we show a warning
        else:
            print('Warning: unknown property ' + property_id)

while True:
    serial_to_property_values()
