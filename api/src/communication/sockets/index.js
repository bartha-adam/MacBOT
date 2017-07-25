var WebSocketServer = require('ws').Server;


var websocketServer = function(){
  var wss;
  var socket;

  var init = function(config){
    wss = new WebSocketServer({ port: config.websocket });
  }

  var onMessage = function(callback){
    wss.on('connection', function connection(ws) {
      socket = ws;
      ws.on('message', function incoming(message) {

        callback(message);


      });

    //  ws.send('something');
    });
  }

  var send = function(data){
    if (socket == undefined)
      return;
    try {
        socket.send(data);
    } catch (e) {
      console.log(e);
    } finally {

    }

  }

  return {
    init: init,
    onMessage: onMessage,
    send: send
  }
}();

module.exports = websocketServer;
