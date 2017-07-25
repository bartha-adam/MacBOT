
var CMD_MOVE_FORWARD = "mv_fw";
var CMD_MOVE_BACKWARD = "mv_bk";
var CMD_TURN_LEFT = "turn_left";
var CMD_TURN_RIGHT = "turn_right";
var CMD_SPEED = "speed";
var CMD_ABORT = "abort";



var manualControl = function(){
    var serial;

    function sleep(delay) {
      delay += new Date().getTime();
      while (new Date() < delay) { }
    }

    var init = function(serialPort) {
       serial = serialPort;
    }

    var apply = function(command){
      if (command.module == "motor"){
        var cmd = "";
        switch (command.payload) {
          case 'forward':
            cmd = CMD_MOVE_FORWARD;
            break;
          case 'back':
            cmd = CMD_MOVE_BACKWARD;
            break;
          case 'left':
            cmd = CMD_TURN_LEFT;
            break;
          case 'right':
            cmd = CMD_TURN_RIGHT;
            break;
          case 'break':
            cmd = CMD_ABORT;
            break;
          default:

        }

        serial.write(cmd);
      }
    }

    return {
      init: init,
      apply: apply
    }
}();

module.exports = manualControl;
