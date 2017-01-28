Pebble.addEventListener("ready", function(e){
  console.log("Ready for interaction!");  
});

Pebble.addEventListener("appmessage", function(e){
    console.log("Got message from app: " + e.payload.testkey);  
    
    var object = {
        testkey: 123
    };
    Pebble.sendAppMessage(object);
});