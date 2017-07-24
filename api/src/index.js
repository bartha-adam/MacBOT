var HttpDispatcher = require('httpdispatcher');
var http           = require('http');
var dispatcher     = new HttpDispatcher();

dispatcher.setStatic('/client');
dispatcher.setStaticDirname('client');

//57600 /dev/ttyS0 for linkit
var config = require('./config.json');

var websocketServer = require('./communication/sockets');
websocketServer.init(config);
websocketServer.onMessage(function(message){
  try {
    var msg = JSON.parse(message);
    console.log(msg);
    //serialPort.write(msg.payload);
    if (msg != undefined){
      if (msg.payload == undefined){
        websocketServer.send("Missing payload field")
      }
      else {
        serialPort.write(msg.payload);
      }
    }
    else {
      websocketServer.send("Undefined message")
    }

  } catch (e) {
    console.log(e);
  } finally {

  }
});
var isOn = true;
var serialPort = require("./communication/serial/"+config.serialLibrary)
serialPort.init(config.baudRate, config.serialPort, function(data){
  //sanitize
  data = data.replace("\r","");
  data = data.replace("\n","")
  /*var msg = {
    payload: data
  }
  var serialized = JSON.stringify(msg);*/
  websocketServer.send(data);
});

dispatcher.onGet("/test", function(req, res){
    res.writeHead(200, {'Content-Type': 'text/plain'});
    var message = isOn ? "ON" : "OFF"
    isOn = !isOn;
    serialPort.write(message);
    res.end('Test API');
})

dispatcher.onGet("/test", function(req, res){
    res.writeHead(200, {'Content-Type': 'text/plain'});
    var message = isOn ? "ON" : "OFF"
    isOn = !isOn;
    serialPort.write(message);
    res.end('Test API');
})

dispatcher.beforeFilter(/\//, function(req, res, chain) { //any url
    console.log("Before filter");
    chain.next(req, res, chain);
});

dispatcher.afterFilter(/\//, function(req, res, chain) { //any url
    console.log("After filter");
    chain.next(req, res, chain);
});

dispatcher.onError(function(req, res) {
    res.writeHead(404);
    res.end();
});

http.createServer(function (req, res) {
    dispatcher.dispatch(req, res);
}).listen(config.port);
