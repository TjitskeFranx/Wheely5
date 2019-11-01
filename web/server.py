from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit


# DATA_FOLDER
# Import required library
from dotenv import load_dotenv
import os
import serial
from threading import Thread
load_dotenv()

# weird thing... score just bold is the first thing you will upload. mybe it does excist?

app = Flask(__name__)
print('test')

app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)
# Start reading the serial port
ser = serial.Serial(
    port = os.environ['SERIAL'],
    baudrate = 9600,
    timeout = 2)





# sensors = ['sensor1', 'sensor2', 'sensor3']  # 145.94.216.42
# Read the next line from the serial port
# and update the property values
def serial_to_property_values():
    # Read one line
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        print(line)
        # Split the string using commas as separator, we get a list of strings
        #values = line.split(',')
        # Use the first element of the list as property id
        #property_id = values.pop(0)
        # Get the property from the thing
        #prop = my_thing.properties[property_id]
        # If we find the property, we update the values (rest of the list)
        #if prop is not None:
            #prop.update_values([float(x) for x in values])
        # Otherwise, we show a warning
        #else:
            #print('Warning: unknown property ' + property_id)
        #" {}"mylist" : "[" +str(values(1)) + "," + str(values(2))+ "]"
        try:
            socketio.emit('serial', '{"data": "%s"}' % str(line), broadcast=True)
        except:
            print("No socket?")
# Server CODE

@app.route('/')  # what to load
def hello_world():
    return render_template('score.html')


@app.route('/home')  # were to load the home page /other pages
def home():
    return render_template('score.html')


@socketio.on('json') #just a convention thingy about structures. channel that handles communication
def handle_json(json):
    print('received json: ' + str(json)) #debug print tingu
    emit('json', json, broadcast=True) #to al lconnection channels

@socketio.on('serial')
def handle_distance(json):
    print( float(json['serial'])) #serial channel

if __name__ == '__main__':
    thread = Thread(target=serial_to_property_values, args=())
    thread.start()
    socketio.run(app, host='0.0.0.0', debug=True)  # this just runs the app
