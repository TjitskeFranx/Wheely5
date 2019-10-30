<<<<<<< HEAD
from flask import Flask, request, render_template
from flask_socketio import SocketIO, emit
app = Flask(__name__)
=======
from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit

# weird thing... score just bold is the first thing you will upload. mybe it does excist?

app = Flask(__name__)
print('test')
>>>>>>> 93512d5b6b87bddd61049f37606acb2d4f7fb1b2

sensors = ['sensor1', 'sensor2', 'sensor3'] #145.94.216.42



<<<<<<< HEAD
@app.route('/')
=======
@app.route('/')  # what to load
>>>>>>> 93512d5b6b87bddd61049f37606acb2d4f7fb1b2
def hello_world():
    return 'Hello, Worfld!'
    return render_template('index.html')

<<<<<<< HEAD
@app.route('/home')
=======

@app.route('/home')  # were to load the home page /other pages
>>>>>>> 93512d5b6b87bddd61049f37606acb2d4f7fb1b2
def home():
    return render_template('index.html')

@app.route('/api/sensors', methods = ['GET'])
def list():
    return str(sensors)

@app.route('/api/sensors/<path:sensor_id>', methods = ['GET'])
def read(sensor_id):
    global sensors
    return sensors[sensor_id]

<<<<<<< HEAD
@app.route('/api/sensors', methods = ['POST'])
def create():
    sensors.append(request.json["sensorName"])
    return 'Added sensor!'
=======

@socketio.on('send')  # create an event that we call send
def clickevent(sendData):  # when is this function called? // why does the title not matter?
    print('received send: ' + str(sendData))  # how does this work?
    emit('send', sendData, broadcast=True)  # broadcasting it to all?
>>>>>>> 93512d5b6b87bddd61049f37606acb2d4f7fb1b2


if __name__ == '__main__':
    app.run(host='0.0.0.0')
