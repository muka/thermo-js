
/*
 * GET home page.
 */

var config = require('../config');

exports.index = function(req, res) {

    res.render('index', {
        config: {
            socket: {
                uri: ["http://", config.host, ":", config.port].join('')
            }
        }
    });

};