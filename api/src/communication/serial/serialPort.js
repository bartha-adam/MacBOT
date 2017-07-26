var SerialPort = require('serialport');
var Readline = SerialPort.parsers.Readline;

var serialComm = function(){
    var port;
    var parser = new Readline();

    var init = function(baudRate, address, onData) {


      port = new SerialPort(address, {baudRate: baudRate});
      port.pipe(parser);

      port.on('open', function() {

      });

      // open errors will be emitted as an error event
      port.on('error', function(err) {
        console.log('Cannot open serial port. ', err.message);
      })

      parser.on('data', function (data) {
        console.log('data: ' + data+'\n');
        onData(data+'');
      });
    }

    var write = function(message){
      message = message + "\n";
    port.write(message, function(err) {
      if (err) {
        return console.log('Error on write: ', err.message);
      }
    });
    }
    return {
      init: init,
      write: write
    }
}();

module.exports = serialComm;
