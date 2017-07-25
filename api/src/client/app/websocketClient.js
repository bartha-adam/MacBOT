var socket = new WebSocket("ws://localhost:3002");

socket.onopen = function (event) {
  var msg = {
    payload:"OFF"
  };

  var serialized = JSON.stringify(msg);

  socket.send(serialized);
};

socket.onmessage = function (event) {
  console.log("message");
  console.log(event.data);
}

var isOn = true;
(function($){
  $(document).ready(function(){
    $("#testBtn").on("click", function(){
      var msg = {
        payload: isOn ? "ON" : "OFF"
      };
      isOn = !isOn;
      var serialized = JSON.stringify(msg);

      socket.send(serialized);
    });
  });
}(jQuery));
