import time
import serial

ser = serial.Serial('/dev/ttyACM0', 38400)

while 1 :
    
    line_bytes = ser.readline()
    # If the line is not empty
    if len(line_bytes) > 0:
        # Convert the bytes into string
        line = line_bytes.decode('utf-8')
        #line = line_bytes
        print(line)
        
        #reads Arduindo serial ex. score:3[RED]-2[BLUE]
        if line.startswith( "score:" ):
            redTiles = line[6]
            blueTiles = line[14]
            print(redTiles)
            print(blueTiles)
            

        
    
      