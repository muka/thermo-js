
/*
 * GET home page.
 */

exports.index = function(req, res) {

    var devtty = "/dev/ttyUSB0";

    var serialport = require("serialport");
    var SerialPort = serialport.SerialPort; // localize object constructor

    var sp = new SerialPort(devtty, {
        baudrate: 9600,
        parser: serialport.parsers.readline("\n")
    });

//    var temp;
//    sp.on("data", function (data) {
//
//        temp = data;
//
//        res.render('index', {
//            title: 'Temperature',
//            temperature: temp
//        });
//    });


};