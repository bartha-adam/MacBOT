
var m = require('mraa'); //require mraa
console.log('MRAA Version: ' + m.getVersion());

var serialComm = function(){
    var uart;

    function sleep(delay) {
      delay += new Date().getTime();
      while (new Date() < delay) { }
    }

    var init = function(baudRate, address, onData) {
      uart = new m.Uart(address);
      uart.setBaudRate(baudRate);
    //  port.setMode(8, 0, 1);

      setInterval(function(){
        if (uart.dataAvailable()){
          var data=uart.readStr(255);
          onData(data);
        }
      }, 500);

    }

    var write = function(message){
      console.log(uart.writeStr(message));
      sleep(200);
    }

    return {
      init: init,
      write: write
    }
}();

module.exports = serialComm;
