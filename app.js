
/**
 * Module dependencies.
 */
var config = require("./config");

var express = require('express');
var routes = require('./routes');
var user = require('./routes/user');
var http = require('http');
var path = require('path');

var expressLayouts = require('express-ejs-layouts');

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;

var app = express();

// all environments
app.set('host', process.env.HOST || config.host || "localhost");
app.set('port', process.env.PORT || config.port || 3000);

app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');
app.set('layout', 'layout');


app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.json());
app.use(express.urlencoded());
app.use(express.methodOverride());
app.use(express.cookieParser('your secret here'));
app.use(express.session());

app.use(expressLayouts);
app.use(app.router);

app.use(require('stylus').middleware(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);

var server = http.createServer(app)

server.listen(app.get('port'), app.get('host'), function(){
  console.log('Express server listening on ' + app.get('host') + ':' + app.get('port'));
});

var io = require('socket.io').listen(server);


var sp = new SerialPort(config.tty, {
    baudrate: config.baudrate,
    parser: serialport.parsers.readline("\n")
});

io.sockets.on('connection', function (socket) {

    sp.on("data", function (data) {
        try {
            var status = JSON.parse(data);
            socket.emit('status', status);
        }
        catch(e) {
            console.error(data);
            console.error(e);
        }
    });

    socket.on('update', function (data) {
        console.log("SEND!", data);
        sp.write(JSON.stringify(data));
    });


});