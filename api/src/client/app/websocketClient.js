var socket;
var config;

function initSocket(config){
  socket = new WebSocket("ws://"+config.host+":"+config.websocket);

  socket.onopen = function (event) {

  };

  socket.onmessage = function (event) {
    console.log(event.data);
    var msg = JSON.parse(event.data);

    $("#logconsole").append(msg.payload+"\n");
    var logconsole = $('#logconsole');
    if(logconsole.length)
       logconsole.scrollTop(logconsole[0].scrollHeight - logconsole.height());
    }
}

function initCamera(config){
  $("#cameraStream").attr("src", config.cameraStream);
}

function getSerializedMessage(controlMetadata, value){
  var tokens = controlMetadata.split(',');
  var msg = {
    module: tokens[0],
    command: tokens[1],
    value: value
  };

  var serialized = JSON.stringify(msg);

  return serialized;
}

(function($){
  $(document).ready(function(){
    $.get("/config", function(data){
      config = data;
      initSocket(config);
      initCamera(config);
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
    }).on('mouseup focusout', function() {
        if (isHolding){
          isHolding = false;
          //send abort
          var serialized = getSerializedMessage("motor,break");
          socket.send(serialized);
        }

        clearTimeout(timeoutId);
    });

    $(".slider.motorcontrol").on("change", function(){
      console.log($(this)[0].value);
      var serialized = getSerializedMessage($(this).data("control"),$(this)[0].value);
      socket.send(serialized);
    });

  });
}(jQuery));
