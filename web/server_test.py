from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit

# Import required libraries
from dotenv import load_dotenv
import os
import serial
from threading import Thread
load_dotenv()

# weird thing... score just bold is the first thing you will upload. mybe it does excist?

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!' # not actually needed
socketio = SocketIO(app) #init socketIO

# Server CODE

@app.route('/')  # what to load
def hello_world():
    return render_template('score_test.html') # the html code you want to run

@socketio.on('json') #just a convention thingy about structures. channel that handles communication
def handle_json(json):
    print('received json: ' + str(json)) #debug print thing
    emit('json', json, broadcast=True) #to al connected channels

@socketio.on('serial')
def handle_distance(json):
    print( float(json['serial'])) #serial channel

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', debug=True)  # this just runs the server on your ip adress
