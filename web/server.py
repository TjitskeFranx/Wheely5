from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit

# weird thing... score just bold is the first thing you will upload. mybe it does excist?

app = Flask(__name__)
print('test')

app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)

# sensors = ['sensor1', 'sensor2', 'sensor3']  # 145.94.216.42


@app.route('/')  # what to load
def hello_world():
    return render_template('score.html')


@app.route('/home')  # were to load the home page /other pages
def home():
    return render_template('score.html')


@socketio.on('send')  # create an event that we call send
def clickevent(sendData):  # when is this function called? // why does the title not matter?
    print('received send: ' + str(sendData))  # how does this work?
    emit('send', sendData, broadcast=True)  # broadcasting it to all?


if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', debug=True)  # this just runs the app
