var socket = new WebSocket("ws://localhost:3002");

socket.onopen = function (event) {

};

socket.onmessage = function (event) {
  console.log("message");
  console.log(event.data);
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
    //check hold
    var timeoutId = 0;
    var isHolding = false;



    $(".btn.motorcontrol").on('mousedown', function() {
        var btn = $(this);
        var serialized = getSerializedMessage(btn.data("control"));
        socket.send(serialized);
        timeoutId = setTimeout(function(){
          isHolding = true;

        }, 1000);
    }).on('mouseup', function() {

        if (isHolding){
          isHolding = false;
          //send abort
          var serialized = getSerializedMessage("motor,break");
          socket.send(serialized);
        }
        else {
          //send command on click
          /*var serialized = getSerializedMessage($(this).data("control"));
          socket.send(serialized);*/
        }

        clearTimeout(timeoutId);
    });

  });
}(jQuery));
