// ___________________________________________________
var socket = io(); /////fskodhfaskohfkoasjfkoasjdfkoasd

//IMPORTANT, RECAPTURE
//as far as i understand it now:
//socket.emit sends, socket.on listens. Wht the order is or how they effect eachother, is hard to tell in this setup...
//it doesnt really fit with this code or when they random update their own shit. maybe clearly define a client and what not.
//right now the code breaks the other client.. which was to be expected.

//it goes: python (define channels 'send') > html (click, plus text) > js. (read the message) js. > entrepreted the js.

//ALSO: WHY ARE WE DOING THIS

socket.emit('send', {data: RedTiles})

socket.on('connect', function() { //when start
  socket.emit('send', {
  data: 'New guy loggin in'
  }); //broadcast this to all websocketss/ event called json?
});

socket.on('send', function(msg) { //reads/listens message
  if (msg.data !== undefined) {
    RedTiles=msg.data;
    alert(msg.data); // prints this to all
  }
});


function sendMessage(red, netural, blue) { //function you for sendng messages
  socket.emit('send', {
    data: RedTiles
  });
}


// io.sockets.on('connection', function (socket) {
//   socket.on('event_name', function(data) {
//       // you can try one of these three options
//
//       // this is used to send to all connecting sockets
//       io.sockets.emit('eventToClient', { id: userid, name: username });
//       // this is used to send to all connecting sockets except the sending one
//       socket.broadcast.emit('eventToClient',{ id: userid, name: username });
//       // this is used to the sending one
//       socket.emit('eventToClient',{ id: userid, name: username });
//   }
// }
