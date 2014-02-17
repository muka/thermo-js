define(["backbone", "socketio"], function(Backbone, io) {


    var App = function() {

        var me = this;
        var socket = io.connect(CONFIG.socket.uri);

        me.currentTemperatureView = null;
        me.requestedTemperatureView = null;

        var temperatureModel = Backbone.Model.extend({});

        var currentTemperatureView = Backbone.View.extend({

            getTemperature: function() {
                return this.model.get('t');
            },

            events: {
                "click .button.delete": "destroy"
            },

            initialize: function() {

                this.listenTo(this.model, "change", this.render);
            },
            render: function() {
                this.$el.find(".unit").text('°');
                this.$el.find(".value").text(this.getTemperature());
            }

        });
        var requestedTemperatureView = currentTemperatureView.extend({

            getTemperature: function() {
                return this.model.get('r');
            },

            events: {
                "change input.selector": "updateSelector"
            },

            setSelector: function(val) {
                this.getSelector().val(val);
            },

            updateSelector: function(ev) {
                this.model.set('r', 1*this.getSelector().val());
                socket.emit("update", this.model.attributes);
            },

            getSelector: function() {
                return this.$el.find("input.selector");
            },
            render: function() {

                currentTemperatureView.prototype.render.call(this);
                this.setSelector(this.getTemperature());

                if(this.getTemperature() > me.currentTemperatureView.getTemperature()) {
                    $('#led').addClass('on');
                }
                else {
                    $('#led').removeClass('on');
                }

            }

        });

        this.init = function() {

            me.temperatureModel = new temperatureModel;
            me.currentTemperatureView = new currentTemperatureView({
                el: '#currentTemperature',
                model: me.temperatureModel
            });
//            me.currentTemperatureView.render();

            me.requestedTemperatureView = new requestedTemperatureView({
                el: '#requestedTemperature',
                model: me.temperatureModel
            });

            socket.on('status', function(data) {
                me.temperatureModel.set(data);
            });

        };

        this.init();

    };

    return App;
});




