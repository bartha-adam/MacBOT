var SerialPort = require('serialport');

var serialComm = function(){
    var port;

    var init = function(baudRate, address, onData) {


      port = new SerialPort(address, {baudRate: baudRate});

      port.on('open', function() {

      });

      // open errors will be emitted as an error event
      port.on('error', function(err) {
        console.log('Cannot open serial port. ', err.message);
      })

      port.on('data', function (data) {
        console.log('data received: ' + data+'\n');
        onData(data);
      });
    }

    var write = function(message){
    port.write(message, function(err) {
      if (err) {
        return console.log('Error on write: ', err.message);
      }
      console.log('message written');
    });
    }
    return {
      init: init,
      write: write
    }
}();

module.exports = serialComm;
