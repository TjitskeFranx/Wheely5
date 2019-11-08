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

# Server CODE

@app.route('/')  # what to load
def hello_world():
    return render_template('score_test.html')


@app.route('/home')  # were to load the home page /other pages
def home():
    return render_template('score_test.html')


@socketio.on('json') #just a convention thingy about structures. channel that handles communication
def handle_json(json):
    print('received json: ' + str(json)) #debug print tingu
    emit('json', json, broadcast=True) #to al lconnection channels

@socketio.on('serial')
def handle_distance(json):
    print( float(json['serial'])) #serial channel

if __name__ == '__main__':
    #thread = Thread(target=serial_to_property_values, args=())
    #thread.start()
    socketio.run(app, host='145.94.181.160', debug=True)  # this just runs the app
