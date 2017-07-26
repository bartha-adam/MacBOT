var socket;
var config;

function initSocket(config){
  socket = new WebSocket("ws://"+config.host+":"+config.websocket);

  socket.onopen = function (event) {

  };

  socket.onmessage = function (event) {
    var msg = JSON.parse(event.data);

    $("#logconsole").append(msg.payload+"\n");
    var logconsole = $('#logconsole');
    if(logconsole.length)
       logconsole.scrollTop(logconsole[0].scrollHeight - logconsole.height());
    }
}

function getSerializedMessage(csv){
  var tokens = csv.split(',');
  var msg = {
    module: tokens[0],
    payload: tokens[1]
  };

  var serialized = JSON.stringify(msg);

  return serialized;
}

(function($){
  $(document).ready(function(){
    $.get("/config", function(data){
      config = data;
      initSocket(config);
    });

    //check hold
    var timeoutId = 0;
    var isHolding = false;

    $(".btn.motorcontrol").on('mousedown', function() {
        var btn = $(this);
        var serialized = getSerializedMessage(btn.data("control"));
        socket.send(serialized);
        timeoutId = setTimeout(function(){
          isHolding = true;

        }, 200);
    }).on('mouseup', function() {
        if (isHolding){
          isHolding = false;
          //send abort
          var serialized = getSerializedMessage("motor,break");
          socket.send(serialized);
        }

        clearTimeout(timeoutId);
    });

  });
}(jQuery));
