require.config({
    paths: {
        //tries to load jQuery from Google's CDN first and falls back
        //to load locally
        "jquery": "/libs/jquery/dist/jquery.min",
        "underscore": "/libs/underscore/underscore",
        "backbone": "/libs/backbone/backbone-min",
        "socketio": "/socket.io/socket.io",
    },
    shim: {
        "backbone": {
            //loads dependencies first
            deps: ["jquery", "underscore"],
            //custom export name, this would be lowercase otherwise
            exports: "Backbone"
        },
        "socketio": {
            exports: "io"
        },
        "app": {
            deps: ["backbone", "socketio"]
        }
    },
    //how long the it tries to load a script before giving up, the default is 7
    waitSeconds: 10
});

require(['backbone', 'app'], function(Backbone, App){
    new App;
});
